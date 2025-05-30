// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// Copyright, University Corporation for Atmospheric Research (UCAR) 2009-2017. 
// The Government's right to use this data and/or software is restricted per 
// the terms of Cooperative Agreement between UCAR and the National  Science 
// Foundation, to government use only which includes the nonexclusive, 
// nontransferable, irrevocable, royalty-free license to exercise or have 
// exercised for or on behalf of the U.S. Government throughout the world. 
// All other rights are reserved. 
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
#include <toolsa/copyright.h>

/**
 * @file CiFlagHandler.cc
 */
#include <ConvWx/ConvWxTime.hh>
#include <ConvWx/InterfaceLL.hh>
#include <ConvWx/Xml.hh>
#include <ConvWxIO/InterfaceIO.hh>
#include <ConvWxIO/CiFlagHandler.hh>
#include <ConvWxIO/ConvWxThreadMgr.hh>
#include <toolsa/LogStream.hh>
#include <string>
#include <ctime>
#include <unistd.h>
using std::string;

//-----------------------------------------------------------------
CiFlagHandler::CiFlagHandler(void) :
  pGood(false),
  pGenTimeIsSet(false),
  pLtIsSet(false),
  pHasCiData(false)
{
}

//-----------------------------------------------------------------
CiFlagHandler::CiFlagHandler(const ParmProjection &proj,
			     const ParmFcst &parms, int maxSecondsBack,
			     int maxWaitSeconds) :


  pGood(true),
  pName("CiFlag"),
  pProj(proj),
  pParm(parms),
  pMaxSecondsBack(maxSecondsBack),
  pMaxWaitSeconds(maxWaitSeconds),
  pGenTime(convWx::BAD_TIME),
  pGenTimeIsSet(false),
  pLt(0),
  pLtIsSet(false),
  pHasCiData(false)
{
}

//-----------------------------------------------------------------
CiFlagHandler::~CiFlagHandler()
{
}

//----------------------------------------------------------------
void CiFlagHandler::startGenTime(const time_t &t)
{
  pGenTime = t;
  pGenTimeIsSet = true;
  LOG(DEBUG) << "InitMask gen time set to match, " << 
    ConvWxTime::stime(pGenTime);

  pLtIsSet = false;

  pHasCiData = false;

}

//----------------------------------------------------------------
void CiFlagHandler::init(const time_t &ciGt, int ciLt)
{
  pGenTime = ciGt;
  pGenTimeIsSet = true;

  pLt = ciLt;
  pLtIsSet = true;
  pLtOutOfRange = false;

  pHasCiData = false;
}

//----------------------------------------------------------------
void CiFlagHandler::processInit(const time_t &gt, int lt,
				ConvWxThreadMgr *thread)
{

  if (pGenTimeIsSet)
  {
    // make sure gt = local
    if (gt != pGenTime)
    {
      LOG(ERROR) << "Gen time mismatch "
		 << ConvWxTime::stime(pGenTime)
		 << ", change to "
		 << ConvWxTime::stime(gt);
      pGenTime = gt;
    }
    pLt = lt;
    pLtIsSet = pWaitForLeadTime(lt, thread);
  }
  else
  {
    pLtIsSet = false;
    pLtOutOfRange = false;
  }

  pHasCiData = false;
}

//-----------------------------------------------------------------
bool CiFlagHandler::loadData(ConvWxThreadMgr *thread)
{
  pHasCiData = false;
  if (pGenTimeIsSet && pLtIsSet)
  {
    LOG(DEBUG_VERBOSE) <<  "looking for mask data " << ConvWxTime::ftime(pGenTime, pLt);

    if (thread != NULL)
    {
      thread->lock();
    }

    pHasCiData = pParm.loadFcstData(pGenTime, pLt,
				    pProj, pCiData);
    LOG(DEBUG) << "CIFLAG: Loaded " << ConvWxTime::ftime(pGenTime, pLt) << ", status=" << pHasCiData;
    if (thread != NULL)
    {
      thread->unlock();
    }
  }
  return pHasCiData;
}

//-----------------------------------------------------------------
bool CiFlagHandler::loadData(void)
{
  pHasCiData = false;
  if (pGenTimeIsSet && pLtIsSet)
  {
    LOG(DEBUG_VERBOSE) << "looking for mask data " << ConvWxTime::ftime(pGenTime, pLt);
    pHasCiData = pParm.loadFcstData(pGenTime, pLt,
				    pProj, pCiData);
    LOG(DEBUG) << "Loading CI DATA:" << ConvWxTime::ftime(pGenTime, pLt) << ", status=" << pHasCiData;
  }
  return pHasCiData;
}

//-----------------------------------------------------------------
std::string CiFlagHandler::metaDataXml(void) const
{
  string s = "";
  string tag = "CiFlag_hasCiFlag";
  s = Xml::writeBoolean(tag, 0, pHasCiData);
  tag = "CiFlag_hasGenTime";
  s += Xml::writeBoolean(tag, 0, pGenTimeIsSet);
  if (pGenTimeIsSet)
  {
    tag = "CiFlag_genTime";
    s += Xml::writeTime(tag, 0, pGenTime);
  }
  tag = "CiFlag_hasLeadTime";
  s += Xml::writeBoolean(tag, 0, pLtIsSet);
  if (pLtIsSet)
  {
    tag = "CiFlag_leadtime";
    s += Xml::writeInt(tag, 0, pLt);
  }
  
  tag = "CiFlag_LtOutOfRange";
  s += Xml::writeBoolean(tag, 0, pLtOutOfRange);

  return s;
}

//-----------------------------------------------------------------
bool CiFlagHandler::constructMaskGrid(int nptExpand, const Grid &extrapGrid,
				      double minValue, Grid &maskGrid) const
{
  if (pHasCiData)
  {
    maskGrid = pCiData;
    maskGrid.max(nptExpand, nptExpand, false);

    // mask out all points where extrap is too small
    maskGrid.maskBelowThreshToMissing(extrapGrid, minValue);
    string newName = "Expanded" + pName;
    maskGrid.changeName(newName);
    LOG(DEBUG) << "Created masked expanded CI grid " 
	       << ConvWxTime::ftime(pGenTime, pLt);
    return true;
  }
  else
  {
    if (pLtOutOfRange)
    {
      LOG(DEBUG) << "CIFLAG:Lead out of range, set to 1 everywhere "
	       << ConvWxTime::ftime(pGenTime, pLt);
    }
    else
    {
      LOG(WARNING) << "CIFLAG:timed out, set to 1 everywhere "
		   << ConvWxTime::ftime(pGenTime, pLt);
    }
    maskGrid = extrapGrid;
    maskGrid.setAllToValue(1.0);
    maskGrid.maskBelowThreshToMissing(extrapGrid, minValue);
    string newName = "Expanded" + pName;
    maskGrid.changeName(newName);
    return true;
  }
}

//-----------------------------------------------------------------
bool CiFlagHandler::parseXml(const std::string &xml)
{

  bool status = true;
  
  if (!Xml::readBoolean(xml, "CiFlag_hasCiFlag", true, pHasCiData))
  {
    status = false;
  }
  if (!Xml::readBoolean(xml, "CiFlag_hasGenTime", true, pGenTimeIsSet))
  {
    status = false;
  }
  else
  {
    pGenTime = 0;
    if (pGenTimeIsSet)
    {
      if (!Xml::readTime(xml, "CiFlag_genTime", true, pGenTime))
      {
	status = false;
      }
    }
  }

  if (!Xml::readBoolean(xml, "CiFlag_hasLeadTime", true, pLtIsSet))
  {
    status = false;
  }
  else
  {
    if (pLtIsSet)
    {
      if (!Xml::readInt(xml, "CiFlag_leadtime", true, pLt))
      {
	status = false;
      }
    }
  }
  
  pLtOutOfRange = false;
  if (!Xml::readBoolean(xml, "CiFlag_LtOutOfRange", true, pLtOutOfRange))
  {
    status = false;
  }
  return status;
}

//-----------------------------------------------------------------
void CiFlagHandler::extractGenLeadFromMetadata(bool hasGrid,
					       const FcstGrid &grid,
					       const time_t &gridGt,
					       int gridLt)
{
  pGenTimeIsSet = false;
  pLtIsSet = false;
  pHasCiData = false;
  if (hasGrid)
  {
    string xml = grid.getMetadataXml().getFreeformXml();
    if (xml.empty())
    {
      LOG(ERROR) << "No freeform xml in input data "
		 << ConvWxTime::ftime(gridGt, gridLt);
      // placeholder gen and lead
      pGenTime = gridGt;
      pLt = gridLt;
      return;
    }
    else
    {
      if (!parseXml(xml))
      {
	LOG(ERROR) << "Couldn't read ciFlag gen/lead, problem with metadata "
		   << ConvWxTime::ftime(gridGt, gridLt);
	pGenTimeIsSet = false;
	pLtIsSet = false;
	pHasCiData = false;
	// placeholder gen and lead
	pGenTime = gridGt;
	pLt = gridLt;
      }
    }
  }
  else
  {
    // placeholder gen and lead
    pGenTime = gridGt;
    pLt = gridLt;
    LOG(ERROR) << "Could not use metadata to set ciMask gen/lead, "
	       << "none for triggering time "
	       << ConvWxTime::ftime(gridGt, gridLt);
  }
}

//-----------------------------------------------------------------
bool CiFlagHandler::pWaitForLeadTime(int lt, ConvWxThreadMgr *thread)
{
  int lt0 = pParm.pLtSeconds0;
  int ltLast = pParm.pLtSeconds1;
  if (lt < lt0 || lt > ltLast)
  {
    LOG(DEBUG) << "CIFLAG: Lead time out of range "
	       << ConvWxTime::ftime(pGenTime, lt);
    pLtOutOfRange = true;
    return false;
  }
  else
  {
    pLtOutOfRange = false;
    int wait = 0;
    time_t t0 = time(0);
    while (wait < pMaxWaitSeconds)
    {
      if (thread != NULL)
      {
	thread->lock();
      }
      if (pParm.hasData(pGenTime, lt))
      {
	LOG(DEBUG) << "CIFLAG: Found matching data "
		   << ConvWxTime::ftime(pGenTime, lt);
	pLt = lt;
	if (thread != NULL)
	{
	  thread->unlock();
	}
	return true;
      }
      if (thread != NULL)
      {
	thread->unlock();
      }
      wait = time(0) - t0;
      if (wait < pMaxSecondsBack)
      {
	sleep(1);
      }
    }
    LOG(ERROR) << "CIFLAG: NEVER GOT "
	       << ConvWxTime::ftime(pGenTime, lt);
    return false;
  }
}

//-----------------------------------------------------------------
bool CiFlagHandler::pComputeWeightsLeadTime(int lt)
{
  if (pParm.nlead() == 0)
  {
    return false;
  }

  if (pParm.nlead() == 1)
  {
    // pLt = LtInterp(pParm.ithLeadSeconds(0), 1.0);
    pLt = pParm.ithLeadSeconds(0);
    return true;
  }
  else
  {
    double lt0 = pParm.pLtSeconds0;
    double ltLast = pParm.pLtSeconds1;
    if (lt <= lt0)
    {
      pLt = pParm.ithLeadSeconds(0);
      LOG(DEBUG) << "--******---LATENCY LEAD TIME set to [0]= " << pLt;
    }
    else if (lt >= ltLast)
    {
      pLt = pParm.ithLeadSeconds(pParm.nlead()-1);
      LOG(DEBUG) << "--******---LATENCY LEAD TIME set to [last]= " << pLt;
    }
    else
    {
      pLt = lt;
      LOG(DEBUG) << "--******---LATENCY LEAD TIME set to [mid]= " <<  pLt;
    }
    return true;
  }

  return true;
}

