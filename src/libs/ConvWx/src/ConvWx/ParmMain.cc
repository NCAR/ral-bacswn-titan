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
 * @file ParmMain.cc
 */

//----------------------------------------------------------------
#include <string>
#include <ConvWx/ParmMain.hh>
#include <ConvWx/ParmFcst.hh>
#include <ConvWx/ConvWxConstants.hh>
using std::string;

//----------------------------------------------------------------
ParmMain::ParmMain(void) :
  pOk(false),
  pOkErr("Not initialized"),
  pProcessName(),
  pDebug(false),
  pDebugVerbose(false),
  pDebugTrigger(false),
  pDebugShowRealtime(false),
  pDebugShowClassAndMethod(false),
  pInstance(),
  pRegistrationSeconds(0),
  pTriggerType(ParmMain::REALTIME),
  pTriggerUrl(),
  pGtSubsampleMin(),
  pIsLtSubsample(false),
  pLt0(0),
  pLt1(0),
  pDlt(0),
  pArchiveT0(convWx::BAD_TIME),
  pArchiveT1(convWx::BAD_TIME)
{
}

//----------------------------------------------------------------
ParmMain::ParmMain(const string &procname, const bool debug,
		   const bool debugVerbose, const bool debugTrigger,
		   const bool debugShowRealtime,
		   const bool debugShowClassAndMethod, 
		   const string &instance,
		   const int registrationIntervalSeconds,
		   const string &triggerUrl, const int numSubsetGenTimeMinutes,
		   const int *subsetGenTimeMinutes,
		   const bool isSubsetLeadTimes,
		   const double leadTimeHour0, const double leadTimeHour1, 
		   const double leadTimeHourDelta) :
  pOk(true),
  pOkErr(),
  pProcessName(procname),
  pDebug(debug),
  pDebugVerbose(debugVerbose),
  pDebugTrigger(debugTrigger),
  pDebugShowRealtime(debugShowRealtime),
  pDebugShowClassAndMethod(debugShowClassAndMethod),
  pInstance(instance),
  pRegistrationSeconds(registrationIntervalSeconds),
  pTriggerType(ParmMain::REALTIME),
  pTriggerUrl(triggerUrl),
  pGtSubsampleMin(),
  pIsLtSubsample(isSubsetLeadTimes),
  pLt0(0),
  pLt1(0),
  pDlt(0),
  pArchiveT0(convWx::BAD_TIME),
  pArchiveT1(convWx::BAD_TIME)
{
  for (int i=0; i<numSubsetGenTimeMinutes; ++i)
  {
    pGtSubsampleMin.push_back(subsetGenTimeMinutes[i]);
  }
  ParmFcst::convertToSeconds(leadTimeHour0, pLt0);
  ParmFcst::convertToSeconds(leadTimeHour1, pLt1);
  ParmFcst::convertToSeconds(leadTimeHourDelta, pDlt);
}

//----------------------------------------------------------------
ParmMain::ParmMain(const string &procname, const bool debug,
		   const bool debugVerbose, const bool debugTrigger,
		   const bool debugShowRealtime,
		   const bool debugShowClassAndMethod,
		   const string &instance,
		   const int registrationIntervalSeconds,
		   const string &triggerUrl, const int numSubsetGenTimeMinutes,
		   const int *subsetGenTimeMinutes,
		   const bool isSubsetLeadTimes,
		   const double leadTimeHour0, const double leadTimeHour1, 
		   const double leadTimeHourDelta,
		   const time_t &t0, const time_t &t1) :
  pOk(true),
  pOkErr(),
  pProcessName(procname),
  pDebug(debug),
  pDebugVerbose(debugVerbose),
  pDebugTrigger(debugTrigger),
  pDebugShowRealtime(debugShowRealtime),
  pDebugShowClassAndMethod(debugShowClassAndMethod),
  pInstance(instance),
  pRegistrationSeconds(registrationIntervalSeconds),
  pTriggerType(ParmMain::ARCHIVE),
  pTriggerUrl(triggerUrl),
  pGtSubsampleMin(),
  pIsLtSubsample(isSubsetLeadTimes),
  pLt0(0),
  pLt1(0),
  pDlt(0),
  pArchiveT0(t0),
  pArchiveT1(t1)
{
  for (int i=0; i<numSubsetGenTimeMinutes; ++i)
  {
    pGtSubsampleMin.push_back(subsetGenTimeMinutes[i]);
  }
  ParmFcst::convertToSeconds(leadTimeHour0, pLt0);
  ParmFcst::convertToSeconds(leadTimeHour1, pLt1);
  ParmFcst::convertToSeconds(leadTimeHourDelta, pDlt);
}

//----------------------------------------------------------------
ParmMain::~ParmMain()
{
}

//----------------------------------------------------------------
std::vector<int> ParmMain::getLeadTimes(void) const
{
  std::vector<int> ret;

  if (pIsLtSubsample)
  {
    int lt = pLt0;
    while (lt <= pLt1)
    {
      ret.push_back(lt);
      lt += pDlt;
    }
  }
  return ret;
}
