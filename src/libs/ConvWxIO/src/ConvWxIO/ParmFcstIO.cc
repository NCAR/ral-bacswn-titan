// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// � University Corporation for Atmospheric Research (UCAR) 2009-2010. 
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
 * @file ParmFcstIO.cc
 */

//----------------------------------------------------------------
#include <string>
#include <vector>
#include <algorithm>
#include <ConvWxIO/ParmFcstIO.hh>
#include <ConvWxIO/InterfaceIO.hh>

#include <ConvWx/FcstGrid.hh>
#include <ConvWx/MultiGrid.hh>
#include <ConvWx/ConvWxTime.hh>
using std::string;
using std::vector;
using std::pair;

//----------------------------------------------------------------
ParmFcstIO::ParmFcstIO(void) : ParmFcst()
{
}

//----------------------------------------------------------------
ParmFcstIO::ParmFcstIO(const ParmFcst &parm) : ParmFcst(parm)
{
}

//----------------------------------------------------------------
ParmFcstIO::~ParmFcstIO()
{
}

//----------------------------------------------------------------
bool ParmFcstIO::hasData(const time_t &gt, const int lt) const
{
  vector<time_t> vtimes = InterfaceIO::getAvailableValidTimes(pUrl, gt);
  vector<int> ltSeconds = getLtSeconds(gt);
  for (size_t i=0; i<ltSeconds.size(); ++i)
  {
    if (lt == ltSeconds[i])
    {
      return true;
    }
  }
  return false;
}

//----------------------------------------------------------------
bool ParmFcstIO::hasDataIgnoreConfig(const time_t &gt, const int lt) const
{
  vector<time_t> vtimes = InterfaceIO::getAvailableValidTimes(pUrl, gt);
  for (int i=0; i<static_cast<int>(vtimes.size()); ++i)
  {
    int ilt = vtimes[i] - gt;
    if (ilt == lt)
    {
      return true;
    }
  }
  return false;
}

//----------------------------------------------------------------
bool ParmFcstIO::hasData(const time_t &gt) const
{
  vector<time_t> vtimes = InterfaceIO::getAvailableValidTimes(pUrl, gt);
  return !vtimes.empty();
}

//----------------------------------------------------------------
bool ParmFcstIO::hasAllData(const time_t &gt) const
{
  vector<time_t> vtimes = InterfaceIO::getAvailableValidTimes(pUrl, gt);
  vector<int> ltSeconds = getLtSeconds(gt);
  for (size_t i=0; i<ltSeconds.size(); ++i)
  {
    time_t vt = gt + ltSeconds[i];
    if (find(vtimes.begin(), vtimes.end(), vt) == vtimes.end())
    {
      return false;
    }
  }
  return true;
}

//----------------------------------------------------------------
bool ParmFcstIO::matchingGentimeNewestLeadLessOrEqual(const time_t &gt,
						      int &lt) const
{
  vector<time_t> vtimes = InterfaceIO::getAvailableValidTimes(pUrl, gt);
  bool first=true;
  int bestLt = 0;
  for (int i=0; i<static_cast<int>(vtimes.size()); ++i)
  {
    int ilt = vtimes[i] - gt;
    if (ilt <= lt)
    {
      if (first)
      {
	first = false;
	bestLt = ilt;
      }
      else
      {
	if (ilt > bestLt)
	{
	  bestLt = ilt;
	}
      }
    }
  }
  if (first)
  {
    return false;
  }
  else
  {
    lt = bestLt;
    return true;
  }
}

//----------------------------------------------------------------
int ParmFcstIO::numAvailable(const time_t &gt, const int lt0,
			     const int lt1) const
{
  vector<time_t> vtimes = InterfaceIO::getAvailableValidTimes(pUrl, gt);
  int count = 0;
  for (int i=0; i<static_cast<int>(vtimes.size()); ++i)
  {
    if (vtimes[i] >= lt0 && vtimes[i] <= lt1)
    {
      ++count;
    }
  }
  return count;
}

//----------------------------------------------------------------
bool ParmFcstIO::allAvailable(const time_t &gt, const int lt0,
			      const int lt1, int &num) const
{
  vector<time_t> vtimes = InterfaceIO::getAvailableValidTimes(pUrl, gt);
  num = 0;
  for (int lts = pLtSeconds0; lts<= pLtSeconds1; lts+=pLtDeltaSeconds)
  {
    if (lts >= lt0 && lts <= lt1)
    {
      time_t vt = gt + lts;
      if (find(vtimes.begin(), vtimes.end(), vt) == vtimes.end())
      {
	return false;
      }
      else
      {
	++num;
      }
    }
  }
  return true;
}

//----------------------------------------------------------------
vector<int> ParmFcstIO::getAvailableLeadTimes(const time_t &gt) const
{
  return InterfaceIO::getAvailableLeads(gt, pUrl, pLtSeconds0,
                                        pLtSeconds1, pLtDeltaSeconds);
}

//----------------------------------------------------------------
vector<pair<time_t,int> >
ParmFcstIO::getAvailableFcts(const time_t &t0, const time_t &t1) const
{
  LOG(DEBUG_VERBOSE) << "t0: " << ConvWxTime::stime(t0) << "  t1: " << ConvWxTime::stime(t1);
  vector<pair<time_t,int> > ret;

  // adjust to allowed generation times
  time_t lt1, lt0;

  if (largestAvailableGenLe(t1, t0, lt1) && smallestAvailableGenGe(t0, t1, lt0))
  {
    LOG(DEBUG_VERBOSE) << "lt0: " << ConvWxTime::stime(lt0) << "  lt1: " << ConvWxTime::stime(lt1);

    // now build up all generation times in this range
    time_t ti;

    for (ti=lt0; ti<=lt1; ti+= pGenFrequencySeconds)
    {
      vector<int> lt = InterfaceIO::getAvailableLeads(ti, pUrl,
                                                      pLtSeconds0,
                                                      pLtSeconds1,
                                                      pLtDeltaSeconds);
      for (int i=0; i<static_cast<int>(lt.size()); ++i)
      {
        ret.push_back(pair<time_t,int>(ti, lt[i]));
      }
    }
  }
  return ret;
}

//----------------------------------------------------------------
bool ParmFcstIO::smallestAvailableGenGe(time_t t, time_t t1, time_t &ret) const
{
  LOG(DEBUG_VERBOSE) << "t: " << ConvWxTime::stime(t) << "  t1: " << ConvWxTime::stime(t1);
  ret = smallestGenGe(t);
  LOG(DEBUG_VERBOSE) << "ret:  " << ConvWxTime::stime(ret);
  while (!hasData(ret))
  {
    LOG(DEBUG_VERBOSE) << "Hmmm, hasData returned false.";
    ret = ret + pGenFrequencySeconds;
    LOG(DEBUG_VERBOSE) << "ret:  " << ConvWxTime::stime(ret);
    if (ret > t1)
    {
      LOG(DEBUG_VERBOSE) << "Uh-oh, going to return false!";
      return false;
    }
    else
    {
      LOG(DEBUG_VERBOSE) << "Will try " << pGenFrequencySeconds << " seconds later";
    }
  }
  return true;
}

//----------------------------------------------------------------
bool ParmFcstIO::largestAvailableGenLe(time_t t, time_t t0, time_t &ret) const
{
  LOG(DEBUG_VERBOSE) << "t: " << ConvWxTime::stime(t) << "  t0: " << ConvWxTime::stime(t0);
  ret = largestGenLe(t);
  LOG(DEBUG_VERBOSE) << "ret:  " << ConvWxTime::stime(ret);
  while (!hasData(ret))
  {
    LOG(DEBUG_VERBOSE) << "Hmmm, hasData returned false.";
    ret = ret - pGenFrequencySeconds;
    LOG(DEBUG_VERBOSE) << "ret:  " << ConvWxTime::stime(ret);
    if (ret < t0)
    {
      LOG(DEBUG_VERBOSE) << "Uh-oh, going to return false!";
      return false;
    }
    else
    {
      LOG(DEBUG_VERBOSE) << "Will try " << pGenFrequencySeconds << "seconds earlier";
    }
  }
  return true;
}

//----------------------------------------------------------------
time_t ParmFcstIO::similarOlderGenTime(const time_t &gt,
				       const int secondsBack) const
{
  vector<time_t> vgt;
  vgt = InterfaceIO::availableFcstGenTimesInARange(pUrl, gt - secondsBack, gt);
  if (vgt.empty())
  {
    return convWx::BAD_TIME;
  }

  // find minimum positive delta in hour/min/seconds between now and the times
  // want smallest positive or 0 hour
  double hms0 = ConvWxTime::getTimeFraction(gt);
  bool first = true;
  double bestDiff = 0.0;
  for (int i=0; i<static_cast<int>(vgt.size()); ++i)
  {
    pAdjustHmsDiff(vgt[i], hms0, bestDiff, first);
  }
  
  // now choose the most recent time with this delta that has all lead times
  // (if any)
  time_t bestGt = convWx::BAD_TIME;
  first = true;
  bool hasAll = false;
  for (int i=static_cast<int>(vgt.size())-1; i>=0; --i)
  {
    if (pHmsDiffIsMinAndHasAllLeadTimes(vgt[i], hms0, bestDiff, bestGt, first))
    {
      hasAll = true;
      break;
    }
  }

  if (first)
  {
    LOG(ERROR) << "No gentime had minimum difference as expected";
  }    
  else
  {
    if (!hasAll)
    {
      LOG(WARNING) << "No gentime had all leads, use gen time with minimum delta:"
		   << ConvWxTime::stime(bestGt);
    }
  }    
  return bestGt;
}

//----------------------------------------------------------------
bool ParmFcstIO::loadObsData(const time_t t, const ParmProjection &proj,
			     Grid &outGrid,  bool suppresswarnings) const
{
  return InterfaceIO::loadObs(t, proj, pUrl, pField, pRemap, outGrid,
			      suppresswarnings);
}

//----------------------------------------------------------------


bool ParmFcstIO::loadObsData(const time_t t, const ParmProjection &proj,
                             Grid &outGrid, std::vector<double> &vertLevels) const

{
  return InterfaceIO::loadObs(t, proj, pUrl, pField, pRemap, outGrid,vertLevels);
}

//----------------------------------------------------------------
bool ParmFcstIO::loadObsData(const time_t t, const ParmProjection &proj,
			     const string &fName, Grid &outGrid) const
{
  return InterfaceIO::loadObs(t, proj, pUrl, fName, pRemap, outGrid);
}


//----------------------------------------------------------------
bool ParmFcstIO::loadObsData(const time_t t, const ParmProjection &proj, 
			     FcstGrid &outGrid) const
{
  return InterfaceIO::loadObs(t, proj, pUrl, pField, pRemap, outGrid);
}

//----------------------------------------------------------------
bool ParmFcstIO::loadObsData(const time_t t, const ParmProjection &proj,
			     const string &fName, FcstGrid &outGrid) const
{
  return InterfaceIO::loadObs(t, proj, pUrl, fName, pRemap, outGrid);
}


//----------------------------------------------------------------
bool ParmFcstIO::loadFcstData(const time_t gt, const int lt,
			      const ParmProjection &proj,
			      FcstGrid &outGrid) const
{
  return InterfaceIO::loadFcst(gt, lt, proj, pUrl, pField, pRemap, outGrid);
}

//----------------------------------------------------------------
bool ParmFcstIO::loadFcstData(const time_t gt, const int lt,
			      const ParmProjection &proj,
			      const string  &fname, FcstGrid &outGrid) const
{
  return InterfaceIO::loadFcst(gt, lt, proj, pUrl, fname, pRemap, outGrid);
}

//----------------------------------------------------------------
bool ParmFcstIO::loadFcstDataByNum(const time_t gt, const int lt,
				   const ParmProjection &proj,
				   int fieldNum, const std::string &field,
				   FcstGrid &outGrid) const
{
  return InterfaceIO::loadFcstByNum(gt, lt, proj, pUrl, fieldNum, field,
				    pRemap, outGrid);
}

//----------------------------------------------------------------
bool ParmFcstIO::loadFcstDataWithWildcard(const time_t gt, int lt,
                                          const ParmProjection &proj,
                                          const std::string &wildcardFieldName,
                                          FcstGrid &outGrid) const
{
  return InterfaceIO::loadFcstWithWildcard(gt, lt, proj, pUrl, wildcardFieldName,
                                           pRemap, outGrid);
}

//----------------------------------------------------------------
bool ParmFcstIO::loadMultiFcstData(const time_t gt, const int lt,
				   const ParmProjection &proj,
				   const vector<string> &fname,
				   MultiFcstGrid &outGrids,
				   bool suppressErrorMessages) const
{
  return InterfaceIO::loadMultiFcst(gt, lt, proj, pUrl, fname, pRemap,
				    outGrids, suppressErrorMessages);
}

//----------------------------------------------------------------
bool ParmFcstIO::loadAllFcstData(const time_t gt, const ParmProjection &proj,
				 vector<FcstGrid> &outGrids) const
{
  outGrids.clear();
  int lt0 = pLtSeconds0;
  int lt1 = pLtSeconds1;
  int dt  = pLtDeltaSeconds;

  for (int lt=lt0; lt<=lt1; lt+=dt)
  {
    FcstGrid fg;
    if (loadFcstData(gt, lt, proj, fg))
    {
      outGrids.push_back(fg);
    }
    else
    {
      LOG(ERROR) << "loading lead = " << lt;
      outGrids.clear();
      return false;
    }
  }    
  return true;
}

//----------------------------------------------------------------
bool ParmFcstIO::loadAllFcstData(const time_t gt, const ParmProjection &proj,
				 const string &fname,
				 vector<FcstGrid> &outGrids) const
{
  outGrids.clear();
  int lt0 = pLtSeconds0;
  int lt1 = pLtSeconds1;
  int dt  = pLtDeltaSeconds;

  for (int lt=lt0; lt<=lt1; lt+=dt)
  {
    FcstGrid fg;
    if (loadFcstData(gt, lt, proj, fname, fg))
    {
      outGrids.push_back(fg);
    }
    else
    {
      LOG(ERROR) << "loading lead = " << lt;
      outGrids.clear();
      return false;
    }
  }    
  return true;
}

//----------------------------------------------------------------
bool ParmFcstIO::loadValidFcstData(const time_t vt,
				   const int latencyHours[2],
				   const ParmProjection &proj,
				   vector<FcstGrid> &outGrids) const
{
  // try to load all generation times/lead times for which lead time is valid
  //  at vt
  outGrids.clear();

  vector<time_t> gt = genPossible(vt, latencyHours, 0);
  if (gt.empty())
  {
    LOG(ERROR) << "None possible";
    return false;
  }
  int lt0 = pLtSeconds0;
  int dt  = pLtDeltaSeconds;
  for (int i=0; i<static_cast<int>(gt.size()); ++i)
  {
    int lt = vt - gt[i];
    int n = (lt-lt0)/dt;
    if (lt == lt0 + n*dt)
    {
      // exact match
      FcstGrid fg;
      if (loadFcstData(gt[i], lt, proj, fg))
      {
	outGrids.push_back(fg);
      }
    }
  }    
  return (!outGrids.empty());
}

//----------------------------------------------------------------
std::vector<std::pair<time_t, int> >  ParmFcstIO::genLeadVaidAt(const time_t vt)
{
  std::vector<std::pair<time_t, int> >  ret;
  
  // Pull up all the gen times that might have a lead valid at vt
  int lt0 = pLtSeconds0;
  int lt1 = pLtSeconds1;
  std::vector<time_t> gtimes = 
    InterfaceIO::availableFcstGenTimesInARange(pUrl, vt - lt1, vt - lt0);
  for (size_t ig=0; ig < gtimes.size(); ++ig)
  {
    time_t gt = gtimes[ig];
    std::vector<time_t> vtimes = InterfaceIO::getAvailableValidTimes(pUrl, gt);
    for (size_t iv=0; iv<vtimes.size(); ++iv)
    {
      // if valid time is the obs time, process, otherwise not
      if (vtimes[iv] == vt)
      {
	ret.push_back(pair<time_t,int>(gt, vt-gt));
      }
    }
  }
  return ret;
}

//----------------------------------------------------------------
void ParmFcstIO::write(const time_t &gt, const int lt, 
		       const ParmProjection &proj, const MultiGrid &outGrids,
		       const MetaData &metadata) const
{
  InterfaceIO::write(gt, lt, pUrl, proj, outGrids, metadata);
}

//----------------------------------------------------------------
void ParmFcstIO::writeObs(const time_t &gt, const ParmProjection &proj,
			  const MultiGrid &outGrids,
			  const MetaData &metadata) const
{
  InterfaceIO::write(gt, pUrl, proj, outGrids, metadata);
}

//----------------------------------------------------------------
void ParmFcstIO::write(const time_t &gt, const int lt,
		       const ParmProjection &proj,
		       const FcstGrid &outGrid) const
{
  MultiGrid mg(outGrid);
  InterfaceIO::write(gt, lt, pUrl, proj, mg, outGrid.getMetadata());
}

//----------------------------------------------------------------
void ParmFcstIO::writeObs(const time_t &gt, const ParmProjection &proj,
			  const FcstGrid &outGrid) const
{
  MultiGrid mg(outGrid);
  InterfaceIO::write(gt, pUrl, proj, mg, outGrid.getMetadata());
}

//----------------------------------------------------------------
bool ParmFcstIO::pHmsDiffIsMinAndHasAllLeadTimes(const time_t &gt, 
						 const double hms0,
						 const double bestDiff,
						 time_t &bestGt,
						 bool &first) const
{
  double hms = ConvWxTime::getTimeFraction(gt);
  double diffi = hms0 - hms;
  if (diffi == bestDiff)
  {
    if (first)
    {
      first = false;
      // this newest (first) one is our first choice in case no
      // generation times have all lead times
      bestGt = gt;
      LOG(DEBUG_VERBOSE) << "Newest gen time with minimum delta is "
			 << ConvWxTime::stime(gt);
    }
    if (hasAllData(gt))
    {
      // This is now the newest generation time with all data..done!
      LOG(DEBUG_VERBOSE) << "Choose as best:" <<  ConvWxTime::stime(gt);
      bestGt = gt;
      return true;
    }
  }
  return false;
}

