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

/**
 * @file FcstWithLatencyState.cc
 */

#include <toolsa/copyright.h>
#include <cstdio>
#include <ConvWxIO/FcstWithLatencyState.hh>
#include <ConvWxIO/FcstState.hh>
#include <ConvWx/ConvWxTime.hh>
#include <ConvWx/ConvWxConstants.hh>
#include <toolsa/LogStream.hh>
using std::vector;
using std::string;

//----------------------------------------------------------------
FcstWithLatencyState::FcstWithLatencyState(void) :
  pLatencyAdjustedGenTime(convWx::BAD_TIME),
  pFcstUsedIndex(convWx::BAD_INDEX),
  pState(),
  pParmOut()
{
}

//----------------------------------------------------------------
FcstWithLatencyState::FcstWithLatencyState(const time_t &t,
					   const double latencyHours,
					   const ParmFcst &inputParm,
					   const ParmFcst &outputParm) :
  pLatencyAdjustedGenTime(t),
  pFcstUsedIndex(0),
  pState(),
  pParmOut(outputParm)
{
  FcstState f(t - static_cast<int>(latencyHours*convWx::DOUBLE_SECS_PER_HOUR),
	      inputParm);
  pState.push_back(f);
}

//----------------------------------------------------------------
FcstWithLatencyState::~FcstWithLatencyState()
{
}

//----------------------------------------------------------------
void FcstWithLatencyState::print(void) const
{
  pPrint(LogStream::PRINT);
}

//----------------------------------------------------------------
void
FcstWithLatencyState::print(LogStream::Log_t severity) const
{
  pPrint(severity);
}

//----------------------------------------------------------------
void FcstWithLatencyState::rebuild(const time_t &latencyAdjustedGt,
				   const vector<time_t> &gt,
				   const ParmFcst &parmIn,
				   const ParmFcst &parmOut,
				   bool &didChange, bool &fIsComplete)
{
  // create a local object to store in new state info.
  FcstWithLatencyState fnew;

  // set some of the members
  fnew.pLatencyAdjustedGenTime = latencyAdjustedGt;
  LOG(DEBUG) << " Set latency adjusted gen time to " << ConvWxTime::stime(latencyAdjustedGt);
  fnew.pFcstUsedIndex = convWx::BAD_INDEX;
  fnew.pParmOut = parmOut;

  // initialize return status booleans
  didChange = false;
  fIsComplete = false;

  // rebuild each input generation time into this state
  LOG(DEBUG) << "Have " << gt.size() << " gen time inputs";
  fnew.pState.reserve(static_cast<int>(gt.size()));
  for (int i=0; i<static_cast<int>(gt.size()); ++i)
  {
    LOG(DEBUG) << "gen time [" << i << "] = " << ConvWxTime::stime(gt[i]);
    FcstState f1 = pRebuild(gt[i], parmIn, didChange);
    fnew.pState.push_back(f1);
  }

  // replace local state with new one.
  *this = fnew;

  // see if this state is complete.
  fIsComplete = isComplete();
  if (fIsComplete)
  {
     LOG(DEBUG) << "Is complete = True";
  }
  else
  {
     LOG(DEBUG) << "Is complete = False";
  }
}

//----------------------------------------------------------------
bool FcstWithLatencyState::determineBest(void)
{
  // initialize state value
  pFcstUsedIndex = convWx::BAD_INDEX;

  // start with oldest one. and replace with newer ones whenever number
  // of unavailable decreases.
  int best = convWx::BAD_INDEX;
  int minNmissing = 0;
  for (int i=0; i<static_cast<int>(pState.size()); ++i)
  {
    // how many are unavailable?
    int nmissing =
      pState[i].numUnavailableLatencyAdjustedFcsts(pLatencyAdjustedGenTime,
						   pParmOut);
    if (i==0)
    {
      best = 0;
      minNmissing = nmissing;
    }
    else
    {
      if (nmissing <= minNmissing)
      {
	best = i;
	minNmissing = nmissing;
      }
    }
  }
  if (best == convWx::BAD_INDEX)
  {
    // never got anything
    return false;
  }

  if (pState[best].noLatencyAdjustedFcstsAreAvailable(pLatencyAdjustedGenTime,
						      pParmOut))
  {
    //the best was completely bad, which is bad
    return false;
  }

  // best was good which is good... set the state value
  pFcstUsedIndex = best;
  return true;
}

//----------------------------------------------------------------
bool FcstWithLatencyState::getLatencyAdjustedGrid(const ParmProjection &proj,
						  const int leadIndex,
						  double &latencyHours,
						  FcstGrid &outGrid,
						  bool &hasData) const
{
  if (pFcstUsedIndex == convWx::BAD_INDEX)
  {
    hasData = false;
    return false;
  }
  else
  {
    const FcstState &f = pState[pFcstUsedIndex];
    hasData =
      f.getLatencyAdjustedGrid(pLatencyAdjustedGenTime,
			       pParmOut, proj, leadIndex, 
			       latencyHours, outGrid);
    return true;
  }
}

//----------------------------------------------------------------
time_t FcstWithLatencyState::getModelGenTimeUsed(void) const
{
  if (pFcstUsedIndex != convWx::BAD_INDEX)
  {
    return pState[pFcstUsedIndex].getGenTime();
  }
  else
  {
    return convWx::BAD_TIME;
  }
}

//----------------------------------------------------------------
time_t FcstWithLatencyState::getModelGenTimeNewest(void) const
{
  int n = static_cast<int>(pState.size());
  if (n == 0)
  {
    return convWx::BAD_TIME;
  }
  else
  {
    return pState[n-1].getGenTime();
  }
}

//----------------------------------------------------------------
bool FcstWithLatencyState::isComplete(void) const
{
  if (pState.empty())
  {
    return false;
  }
  int i = static_cast<int>(pState.size())-1;
  return pState[i].allLatencyAdjustedFcstsAreAvailable(pLatencyAdjustedGenTime,
						       pParmOut);
}

//----------------------------------------------------------------
bool FcstWithLatencyState::findMatchingModelGt(const time_t &gt,
					       FcstState &state) const
{
  for (int i=0; i<static_cast<int>(pState.size()); ++i)
  {
    if (pState[i].genTimeEquals(gt))
    {
      state = pState[i];
      return true;
    }
  }
  return false;
}

//----------------------------------------------------------------
void FcstWithLatencyState::getLogInfo(string &gt, string &latency,
				      string &missing, string &outGt,
				      string &outMissing) const
{
  if (pFcstUsedIndex == convWx::BAD_INDEX)
  {
    gt = "none";
    latency = "none";
    missing = "ALL";
    outGt = "none";
    outMissing = "ALL";
  }
  else
  {
    pState[pFcstUsedIndex].getLogInfo(pLatencyAdjustedGenTime,
				      pParmOut, gt, latency, missing,
				      outGt, outMissing);
  }
}

//----------------------------------------------------------------
double FcstWithLatencyState::getLatencyHours(void) const
{
  if (pFcstUsedIndex == convWx::BAD_INDEX)
  {
    return convWx::BAD_LATENCY_HOURS;
  }
  else
  {
    double dt;
    dt = static_cast<double>(pLatencyAdjustedGenTime -
			     pState[pFcstUsedIndex].getGenTime());
    return dt/convWx::DOUBLE_SECS_PER_HOUR;
  }
}

//----------------------------------------------------------------
bool FcstWithLatencyState::noneAvailable(void) const
{
  if (pFcstUsedIndex < 0)
  {
    return true;
  }
  else
  {
    return pState[pFcstUsedIndex].noFcstsAreAvailable();
  }
}

//----------------------------------------------------------------
FcstState FcstWithLatencyState::pRebuild(const time_t &gt,
					 const ParmFcst &parmIn,
					 bool &didChange)
{
  FcstState f1;

  // is this generation time part of existing state?
  if (findMatchingModelGt(gt, f1))
  {
    // yes, now in f1... update it and if changed, make note of that.
    LOG(DEBUG) << "Trying to update availability of " << ConvWxTime::stime(gt);
    if (f1.updateAvailability())
    {
      LOG(DEBUG) << "state changed at " << ConvWxTime::stime(gt);
      didChange = true;
    }
  }
  else
  {
    LOG(DEBUG) << "Gen time not in existing state " << ConvWxTime::stime(gt) << ". Create new state";
    // no.. start a new state NOTE this method will query server to set
    // internal availability state
    f1 = FcstState(gt, parmIn);
    didChange = true;
    LOG(DEBUG) << "Gen time added to state: " << ConvWxTime::stime(gt);
  }
  return f1;
}

//----------------------------------------------------------------
string FcstWithLatencyState::pPrint(void) const
{
  char buf[convWx::ARRAY_LEN_VERY_LONG];
  sprintf(buf, "LadjGen:%s used:%d\n",
	  ConvWxTime::stime(pLatencyAdjustedGenTime).c_str(),
	  pFcstUsedIndex);
  string ret = buf;
  for (int i=0; i<static_cast<int>(pState.size()); ++i)
  {
    ret += pState[i].sprint();
  }
  return ret;
}

//----------------------------------------------------------------
void
FcstWithLatencyState::pPrint(const LogStream::Log_t severity) const
{
  LOGV(severity) <<  "LadjGen:" << ConvWxTime::stime(pLatencyAdjustedGenTime)
		 << " used:" << pFcstUsedIndex;
  for (int i=0; i<static_cast<int>(pState.size()); ++i)
  {
    pState[i].print(severity);
  }
}
