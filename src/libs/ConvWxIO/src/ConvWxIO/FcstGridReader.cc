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
 * @file FcstGridReader.cc
 */

//----------------------------------------------------------------
#include <unistd.h>
#include <cstdio>
#include <string>
#include <vector>
#include <ConvWxIO/FcstGridReader.hh>
#include <ConvWxIO/InterfaceIO.hh>
#include <ConvWx/InterfaceLL.hh>
#include <ConvWx/ConvWxTime.hh>
#include <ConvWx/ConvWxConstants.hh>
using std::string;
using std::vector;

//----------------------------------------------------------------
FcstGridReader::FcstGridReader(void) :
  pIsObsData(false),
  pIsLatencyAdjusted(false),
  pReadParms(),
  pReadParmsLatencyAdjusted(),
  pProj(),
  pCurrentGrid(),
  pGtLatencyAdjusted(convWx::BAD_TIME),
  pLtLatencyAdjusted(-1),
  pGtActual(convWx::BAD_TIME),
  pLtActual(-1),
  pGridIsCurrent(false),
  pIsRealtime(false),
  pArchiveLatencySeconds(0),
  pMaxWaitSeconds(0),
  pAspireModel(false)
{
}

//----------------------------------------------------------------
FcstGridReader::FcstGridReader(const bool isObs, const ParmFcst &parms,
			       const ParmProjection &proj) :
  pIsObsData(isObs),
  pIsLatencyAdjusted(false),
  pReadParms(parms),
  pReadParmsLatencyAdjusted(parms), // no difference in this case
  pProj(proj),
  pCurrentGrid(),
  pGtLatencyAdjusted(convWx::BAD_TIME),
  pLtLatencyAdjusted(-1),
  pGtActual(convWx::BAD_TIME),
  pLtActual(-1),
  pGridIsCurrent(false),
  pIsRealtime(true), // doesn't matter in this case
  pArchiveLatencySeconds(0), // not used in this case
  pMaxWaitSeconds(0), // not used in this case
  pAspireModel(false) // not used in this case
{
}

//----------------------------------------------------------------
FcstGridReader::FcstGridReader(const ParmFcst &inputParms,
			       const ParmFcst &latencyParms,
			       const ParmProjection &proj, const bool realtime,
			       const int maxWaitSeconds, 
			       const int archiveLatencySeconds,
                               bool aspireModel) :
  pIsObsData(false),
  pIsLatencyAdjusted(true),
  pReadParms(inputParms),
  pReadParmsLatencyAdjusted(latencyParms),
  pProj(proj),
  pCurrentGrid(),
  pGtLatencyAdjusted(convWx::BAD_TIME),
  pLtLatencyAdjusted(-1),
  pGtActual(convWx::BAD_TIME),
  pLtActual(-1),
  pGridIsCurrent(false),
  pIsRealtime(realtime),
  pArchiveLatencySeconds(archiveLatencySeconds),
  pMaxWaitSeconds(maxWaitSeconds),
  pAspireModel(aspireModel)
{
}

//----------------------------------------------------------------
FcstGridReader::~FcstGridReader()
{
}

//----------------------------------------------------------------
bool FcstGridReader::obsUpdateState(const time_t &t, bool &isnew)
{
  if (!pIsObsData)
  {
    // wrong method, return false
    isnew = false;
    return false;
  }

  if (t == pGtLatencyAdjusted)
  {
    // no new data time, status is already set
    isnew = false;
    return pGridIsCurrent;
  }

  // it is a new situation
  isnew = true;
  pGridIsCurrent = false;

  // actual and latency adjusted are the same for observation data always
  pGtActual = pGtLatencyAdjusted = t;

  // get the data for this time
  time_t t0;
  if (!InterfaceIO::newestObsTime(t, t, pReadParms.pUrl, t0))
  {
    // not available
    return false;
  }

  // Load data into a FcstGrid from server for time t
  if (!pReadParms.loadObsData(t, pProj, pCurrentGrid))
  {
    return false;
  }
  else
  {
    pGridIsCurrent = true;
    return true;
  }
}

//----------------------------------------------------------------
bool FcstGridReader::fcstUpdateState(const time_t &gt, const int lt,
				     bool &isnew)
{
  if (pIsObsData)
  {
    // wrong method, return false
    isnew = false;
    return false;
  }

  if (gt == pGtLatencyAdjusted && lt == pLtLatencyAdjusted)
  {
    // inputs equal current state, nothing to do
    isnew = false;
    return pGridIsCurrent;
  }

  
  // new situation
  isnew = true;
  pGridIsCurrent = false;
  if (gt != pGtLatencyAdjusted)
  {
    pGtLatencyAdjusted = gt;
    if (pIsLatencyAdjusted)
    {
      // generation time change requires choosing a model run, which is
      // where the waiting can happen
      if (!pSetModelGenTime())
      {
	return false;
      }
    }
    else
    {
      // if not latency adjusted, the actual and latency adjusted are same
      pGtActual = gt;
    }
  }

  // set the latency adjusted lead time
  pLtLatencyAdjusted = lt;

  // see where the actual forecasts lie compared to the requested valid time
  time_t vt = gt + lt;
  if (vt < pGtActual)
  {
    // hopeless
    LOG(DEBUG) << "Don't process.. model gt=" << ConvWxTime::stime(pGtActual)
	       << " later than validtime";
  }
  else if (vt > pGtActual + pReadParms.ithLeadSeconds(pReadParms.nlead()-1))
  {
    // hopeless
    LOG(DEBUG) << "Don't process.. valid time " << ConvWxTime::stime(vt)
	       << " later than last model time";
    return false;
  }

  // Try to load the model data and abort if it is not available
  pLtActual = vt - pGtActual;
  if (!pLoadFcstData())
  {
    LOG(ERROR) << "model data at lt=" << pLtActual << " never available";
    return false;
  }
  else
  {
    pGridIsCurrent = true;
    return true;
  }
}

//----------------------------------------------------------------
void FcstGridReader::setStateNoData(void)
{
  pGtLatencyAdjusted = pGtActual = convWx::BAD_TIME;
  pLtLatencyAdjusted = pLtActual = -1;
  pGridIsCurrent = false;
}

//----------------------------------------------------------------
double FcstGridReader::getLatencyHours(void) const
{
  return static_cast<double>(pGtLatencyAdjusted -
			     pGtActual)/convWx::DOUBLE_SECS_PER_HOUR;
}

//----------------------------------------------------------------
bool FcstGridReader::pLoadFcstData(void)
{
  if (pReadParms.hasData(pGtActual, pLtActual))
  {
    if (pReadParms.loadFcstData(pGtActual, pLtActual, pProj, pCurrentGrid))
    {
      return true;
    }
    else
    {
      LOG(ERROR) << "loading model data at lt=" << pLtActual;
      return false;
    }
  }
  else
  {
    LOG(ERROR) << "model data at lt=" << pLtActual << " should be there but is not";
    return false;
  }
}

//----------------------------------------------------------------
bool FcstGridReader::pSetModelGenTime(void)
{
  if (pIsRealtime)
  {
    return pSetModelGenTimeRealtime();
  }
  else
  {
    return pSetModelGenTimeArchive();
  }
}

//----------------------------------------------------------------
bool FcstGridReader::pSetModelGenTimeRealtime(void)
{
  // Get min/max (output) latency adjust lead times
  int minLtOut = pReadParmsLatencyAdjusted.ithLeadSeconds(0);
  int nlead = pReadParmsLatencyAdjusted.nlead();
  int maxLtOut = pReadParmsLatencyAdjusted.ithLeadSeconds(nlead-1);

  // Get max input (actual) forecast lead time
  int maxLt = pReadParms.ithLeadSeconds(pReadParms.nlead()-1);

  // figure out range of valid times that are wanted.
  // pGtLatencyAdjusted is the output (requested) generation time, so here
  // it is:
  time_t minValidT = pGtLatencyAdjusted + minLtOut;
  time_t maxValidT = pGtLatencyAdjusted + maxLtOut;

  // the minimum actual generation time is that for which generation + maxLt
  //  = maxValidT
  time_t minGt = maxValidT - maxLt;

  if (pAspireModel)
  {
     // add in 3 more hours)
     minGt = minGt - 3*3600;
  }

  LOG(DEBUG) << "Looking back " << static_cast<double>(pGtLatencyAdjusted - minGt)/convWx::DOUBLE_SECS_PER_HOUR << " hours";

  // Ask server for the generation times in the range of this minimum up to now.
  vector<time_t> availableModelGt;
  availableModelGt =
    InterfaceIO::availableFcstGenTimesInARange(pReadParms.pUrl, minGt,
					       pGtLatencyAdjusted);
  if (availableModelGt.empty())
  {
    LOG(ERROR) << "no model gen times found in range "
	       << ConvWxTime::stime(minGt) << " to "
	       << ConvWxTime::stime(pGtLatencyAdjusted);
    return false;
  }

  // find the one that has enough lead times available to cover out to
  // the maximum latency adjusted (output) lead time, accounting for latency.
  LOG(DEBUG) << "trying model gen = " <<  ConvWxTime::stime(*availableModelGt.rbegin());

  // Do this by repeated tries until wait parameter or success
  int wait = 0;
  while (true)
  {
    InterfaceLL::doRegister("Looping");
    if (pSetModelGen1(availableModelGt, minValidT, maxValidT, wait))
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool FcstGridReader::pSetModelGen1(const vector<time_t> &availableModelGt, 
				   const time_t &minValidT,
				   const time_t &maxValidT,
				   int &waitSeconds)
{
  // Try the newest model generation time, and see if the lead times in the
  // minimum to maximum valid time range are available or not

  vector<time_t>::const_reverse_iterator modelGt;

  if (pAspireModel)
  {
     for (modelGt = availableModelGt.rbegin(); modelGt != availableModelGt.rend(); ++modelGt)
     {
        if (pReadParms.hasAllData(*modelGt))
        {
           LOG(DEBUG) << "Using Model gen=" << ConvWxTime::stime(*modelGt) << " (all lead times available)";
           pGtActual = *modelGt;
        }
        return true;
     }
     modelGt = availableModelGt.rbegin();
     pGtActual = *modelGt;
     LOG(DEBUG) << "Using Model gen=" << ConvWxTime::stime(*modelGt) << " (newest, but not all lead times available)";
     return true;
  }
  
  modelGt = availableModelGt.rbegin();
  pGtActual = *modelGt;
  int minModelLead = minValidT - *modelGt;
  int maxModelLead = maxValidT - *modelGt;
  int maxNlead = pReadParms.nlead(minModelLead, maxModelLead);
  int nlead;
  if (pReadParms.allAvailable(pGtActual, minModelLead, maxModelLead, nlead))
  {
    LOG(DEBUG) << "Model gen=" << ConvWxTime::stime(pGtActual) << " (all lead times available)";
    // If so, use that newest time as the model generation time (set above)
    return true;
  }
  LOG(DEBUG) <<	"Waiting on model min_lt=" << minModelLead << ", max_lt=" << maxModelLead << ", "
	     << nlead << " of " << maxNlead << " leads available";
  // If sleep is an option based on parameters and sleep so far, do that and 
  // return false

  const int sleepSeconds = 10;
  waitSeconds += sleepSeconds;
  if (waitSeconds <= pMaxWaitSeconds)
  {
    sleep(sleepSeconds);
    InterfaceLL::doRegister("setModelGen1");
    return false;
  }
  else
  {
    // If no more sleep, adjust to an older model generation time and return
    //  true
    pAdjustToOlderModelGen(availableModelGt, minValidT, maxValidT, nlead,
			   maxNlead);
    return true;
  }
}

//----------------------------------------------------------------
void
FcstGridReader::pAdjustToOlderModelGen(const vector<time_t> &availableModelGt,
				       const time_t &minValidT,
				       const time_t &maxValidT,
				       const int origNlead, const int maxNlead)
{
  if (availableModelGt.size() <= 1)
  {
    LOG(WARNING) <<  "Select model gen="  << ConvWxTime::stime(pGtActual)
		 << ", " << origNlead << " lead times of " << maxNlead << "available, none older";
    return;
  }

  time_t origModelGen = pGtActual;

  // try older generation times until get one that has full set of lead times,
  vector<int> vnlead;
  for (int i=static_cast<int>(availableModelGt.size())-2; i>=0; --i)
  {
    pGtActual = availableModelGt[i];
    int minModelLead = minValidT - pGtActual;
    int maxModelLead = maxValidT - pGtActual;
    int nlead;
    if (pReadParms.allAvailable(pGtActual, minModelLead, maxModelLead, nlead))
    {
      LOG(DEBUG) << "Selected older model gen, has all lead times "
		 << ConvWxTime::stime(pGtActual);
      return;
    }
    else
    {
      vnlead.push_back(nlead);
    }
  }

  // or if that doesn't work, use the one that has the most lead times,
  // initialize to input:
  pGtActual = origModelGen;
  int bestNlead=origNlead;
  int i, j;
  for (i=static_cast<int>(availableModelGt.size())-2,j=0; i>=0; --i,++j)
  {
    // use '>' so that newest will win out.
    if (vnlead[j] > bestNlead) 
    {
      bestNlead = vnlead[j];
      pGtActual = availableModelGt[i];
    }
  }
  LOG(DEBUG) << "Selected model gen=" << ConvWxTime::stime(pGtActual)
	     << "..has most lead times " << bestNlead << " of maximum " <<  maxNlead;
}

//----------------------------------------------------------------
bool FcstGridReader::pSetModelGenTimeArchive(void)
{
  static const int archiveLookbackSeconds = 10000;

  /**
   * Limit the model generation times to a fixed latency behind GenTime
   */
  time_t t1 = pGtLatencyAdjusted - pArchiveLatencySeconds;
  time_t t0 = t1 - archiveLookbackSeconds;

  /**
   * Get all model generation times from server in a range back 
   * archiveLookbackSeconds from that
   */
  vector<time_t> availableModelGt = 
    InterfaceIO::availableFcstGenTimesInARange(pReadParms.pUrl, t0, t1);

  /**
   * Use the newest model generation time in this range
   */
  if (availableModelGt.empty())
  {
    LOG(ERROR) << "no model gen times found";
    return false;
  }
  else
  {
    pGtActual = availableModelGt[static_cast<int>(availableModelGt.size())-1];
    LOG(DEBUG) << "Selected model gen=" << ConvWxTime::stime(pGtActual);
    return true;
  }
}
