// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// © University Corporation for Atmospheric Research (UCAR) 2009-2010. 
// All rights reserved.  The Government's right to use this data and/or 
// software (the "Work") is restricted, per the terms of Cooperative 
// Agreement (ATM (AGS)-0753581 10/1/08) between UCAR and the National 
// Science Foundation, to a "nonexclusive, nontransferable, irrevocable, 
// royalty-free license to exercise or have exercised for or on behalf of 
// the U.S. throughout the world all the exclusive rights provided by 
// copyrights.  Such license, however, does not include the right to sell 
// copies or phonorecords of the copyrighted works to the public."   The 
// Work is provided "AS IS" and without warranty of any kind.  UCAR 
// EXPRESSLY DISCLAIMS ALL OTHER WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE.  
//  
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
#include <toolsa/copyright.h>
/**
 * @file FcstWait.cc
 */
#include <unistd.h>
#include <cstring>
#include <vector>
#include <ConvWxIO/FcstWait.hh>
#include <ConvWx/InterfaceLL.hh>
#include <ConvWx/ParmPc.hh>
#include <ConvWx/ConvWxTime.hh>
#include <ConvWx/ConvWxConstants.hh>
using std::string;
using std::vector;

//----------------------------------------------------------------
FcstWait::FcstWait() :
  pRealtime(true),   // assume realtime as the default
  pInputParms(),
  pOutputParms(),
  pProj(),
  pRtWaitMaxSeconds(0),
  pRtWaitSleepSeconds(0),
  pState(),
  pFull(false),
  pDidChange(false),
  pPerfect(false)
{  
}

//----------------------------------------------------------------
FcstWait::~FcstWait()
{
}

//----------------------------------------------------------------
void FcstWait::setRealtime(const bool isRealtime)
{
  pRealtime = isRealtime;
}

//----------------------------------------------------------------
void FcstWait::setParms(const ParmPc &parms, const int maxWaitSec,
			const int waitSleepSec)
{
  pInputParms = parms.pModel;
  pOutputParms = parms.pOutput;
  pProj = parms.pProj;
  pRtWaitMaxSeconds = maxWaitSec;
  pRtWaitSleepSeconds = waitSleepSec;
}

//----------------------------------------------------------------
void FcstWait::setParms(const ParmFcst &inputParm, const ParmFcst &outputParm,
			const ParmProjection &proj, const int maxWaitSeconds,
			const int sleepSeconds)
{
  pInputParms = inputParm;
  pOutputParms = outputParm;
  pProj = proj;
  pRtWaitMaxSeconds = maxWaitSeconds;
  pRtWaitSleepSeconds = sleepSeconds;
}

//----------------------------------------------------------------
bool FcstWait::waitForModelData(const time_t &t, 
				const int modelLatencyHours[2])
{
  // retrieve last model data used and last model data available
  time_t genUsed, genNewest;
  pWaitForModelInit(t, genUsed, genNewest);

  // Construct the set of generation times allowed by the pInputParms parameters
  // from t-modelLatencyHours[0] to t-modelLatencyHours[1],
  // but no older than genUsed.   uses parameters only, no server queries.
  vector<time_t> gt = pInputParms.genPossible(t, modelLatencyHours, genUsed);
  if (gt.empty())
  {
    LOG(DEBUG) <<  "No model data in range, trigger=" << ConvWxTime::stime(t);
    return false;
  }

  // do the waiting now.
  return pWaitAlg(genUsed, genNewest, gt, t);
}

//----------------------------------------------------------------
bool FcstWait::waitForModelData(const time_t &t, const time_t &gt)
{
  // retrieve last model data used and last model data available
  time_t genUsed, genNewest;
  pWaitForModelInit(t, genUsed, genNewest);

  // set generation time to the input as the only entry in a vector
  vector<time_t> lgt;
  lgt.push_back(gt);

  // do the waiting now.
  return pWaitAlg(genUsed, genNewest, lgt, t);
}

//----------------------------------------------------------------
bool FcstWait::loadLatencyAdjustedFcst(const int leadTimeIndex,
				       FcstGrid &outGrid, double &latencyHours,
				       bool &hasData) const
{
  return pState.getLatencyAdjustedGrid(pProj, leadTimeIndex,
				       latencyHours, outGrid, hasData);
}

//----------------------------------------------------------------
bool FcstWait::loadLatencyAdjustedFcstWithTimeCheck(const time_t &t,
						    const time_t &gt, 
						    const int ltIndex,
						    FcstGrid &outGrid,
						    bool &hasData)
{
  // check the input generation time against state
  time_t mgt = pState.getModelGenTimeUsed();
  if (mgt != gt)
  {
    LOG(ERROR) << "input gt= " << ConvWxTime::stime(gt)
	       << " local=" << ConvWxTime::stime(mgt);
    return false;
  }

  // get the forecast specified by index values
  double latencyHours;
  if (pState.getLatencyAdjustedGrid(pProj, ltIndex, latencyHours, outGrid,
				    hasData))
  {
    // make sure latency agrees with inputs
    if (latencyHours != static_cast<double>(t-gt)/convWx::DOUBLE_SECS_PER_HOUR)
    {
      LOG(ERROR) << "expected latency=" << static_cast<double>(t-gt)/convWx::DOUBLE_SECS_PER_HOUR
		 << " got " << latencyHours;
      return false;
    }
    else
    {
      return true;
    }
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------
void FcstWait::pWaitForModelInit(const time_t &t, 
				 time_t &genUsed, time_t &genNewest) const
{
  InterfaceLL::doRegister("waiting for model data");

  LOG(DEBUG) << "trigger=" << ConvWxTime::stime(t);

  // get previous state generation time info. Note we cannot go back and use 
  // an older generation time.
  genUsed = pState.getModelGenTimeUsed();
  genNewest = pState.getModelGenTimeNewest();
  if (genUsed == convWx::BAD_TIME)
  {
    LOG(DEBUG) << "last state model gen time used = NONE";
  }
  else
  {
    LOG(DEBUG) << "last state model gen time used = " << ConvWxTime::stime(genUsed);
  }
}

//----------------------------------------------------------------
// gt   = all possible generation times to check
// t = trigger time.
bool FcstWait::pWaitAlg(const time_t &genUsed, const time_t &genNewest, 
			const vector<time_t> &gt, const time_t &t)
{
  // build initial state using input set of generation times and current time.
  pSetStateBeforeWaiting(genUsed, genNewest, gt, t);

  LOG(DEBUG) << "Start the waiting algorithm now";

  int waitSeconds=0;

  // keep waiting while still need to
  while (pNeedToWait(waitSeconds))
  {
    InterfaceLL::doRegister("waiting for data");

    // wait
    pWait(waitSeconds);

    // adjust state 
    pAdjustStateAfterWaiting(t, gt);
  }

  // no more waiting .. set best state (if any)
  bool ret = pState.determineBest();
  LOG(DEBUG) << "Final state from which to set";
  pState.print(LogStream::DEBUG);
  return ret;
}

//----------------------------------------------------------------
void FcstWait::pSetStateBeforeWaiting(const time_t &genUsed,
				      const time_t &genNewest, 
				      const vector<time_t> &gt,
				      const time_t &t)
{
  // invoke an algorithm to determine 'full' waiting or not based on the
  // inputs. ('pFull' if the vector has stuff newer than genNewest)
  pFullWaitingTest0(genUsed, genNewest, gt[static_cast<int>(gt.size())-1]);

  pDidChange = false;
  pPerfect = false;

  // build state using the range of generation times and the previous
  // state (so don't have to get served information over and over)
  pState.rebuild(t, gt, pInputParms, pOutputParms, pDidChange, pPerfect);
  if (pDidChange)
  {
    LOG(DEBUG) << "State changed from last trigger";
    pState.print(LogStream::DEBUG_VERBOSE);
  }
  else
  {
    LOG(DEBUG) << "State did not change from last trigger";
    if (pFull)
    {
      // print out state if in the 'full wait' mode.
      pState.print(LogStream::DEBUG_VERBOSE);
    }
  }
}

//----------------------------------------------------------------
//
// full waiting only if something changed from last time
//
bool FcstWait::pFullWaitingTest0(const time_t &previousUsedGen,
				 const time_t &previousNewestGen,
				 const time_t &newestGen)
{
  string pt0, pt1, nt;
  pFull = false;
  if (previousUsedGen != convWx::BAD_TIME)
  {
    pt0 = ConvWxTime::stime(previousUsedGen);
  }
  else
  {
    pt0 = "none";
  }
  if (previousNewestGen != convWx::BAD_TIME)
  {
    pt1 = ConvWxTime::stime(previousNewestGen);
  }
  else
  {
    pt1 = "none";
  }
  if (newestGen != convWx::BAD_TIME)
  {
    nt = ConvWxTime::stime(newestGen);
  }
  else
  {
    nt = "none";
  }

  LOG(DEBUG) << "last used gen=" << pt0 << " last new gen=" << pt1 << " current=" << nt;

  // is there now a new generation time to look for compared to last time?
  if (previousNewestGen != newestGen)
  {
    pFull = true;
    LOG(DEBUG) << "full=true, change in newest gen time from " << pt1 << " to " << nt;
  }
  return pFull;
}

//----------------------------------------------------------------
bool FcstWait::pNeedToWait(const int waitSeconds) const
{
  // we do no waiting at all if not in realtime mode.
  if (!pRealtime)
  {
    return false;
  }

  // we do not wait if things are already 'perfect' (newest possible
  // generation time having all needed lead times)
  if (pPerfect)
  {
    LOG(DEBUG) << "Situation is Perfect..no waiting";
    return false;
  }
  
  if ((pFull || pDidChange) && pShouldWait(waitSeconds))
  {
    // explain the waiting reason
    string reason = "";
    if (pFull && pDidChange)
    {
      reason = "Full waiting enabled and recent change";
    }
    else if (pFull && !pDidChange)
    {
      reason = "Full waiting enabled";
    }
    else if (!pFull && pDidChange)
    {
      reason = "Recent change";
    }
    LOG(DEBUG) << "Waiting because " << reason << " (waitTime=" << waitSeconds << ")";
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------
void FcstWait::pAdjustStateAfterWaiting(const time_t &t, 
					const vector<time_t> &gt)
{
  // rebuild the state 
  pState.rebuild(t, gt, pInputParms, pOutputParms, pDidChange, pPerfect);

  // something may have changed
  if (pDidChange)
  {
    LOG(DEBUG) << "State has just changed";
    pState.print(LogStream::DEBUG);
  }
  else
  {
    LOG(DEBUG) << "State did not changed";
  }
    
  // it might now be perfect
  if (pPerfect)
  {
    LOG(DEBUG) << "Finish waiting..perfect state";
  }
}

//---------------------------------------------------------------------
bool FcstWait::pShouldWait(const int elapsedSeconds) const
{
  return elapsedSeconds < pRtWaitMaxSeconds;
}

//----------------------------------------------------------------
void FcstWait::pWait(int &waitSeconds) const
{
  // if no waiting yet, don't wait
  if (waitSeconds > 0)
  {
    sleep(pRtWaitSleepSeconds);
  }
  // increment wait
  waitSeconds += pRtWaitSleepSeconds;
  return;
}

