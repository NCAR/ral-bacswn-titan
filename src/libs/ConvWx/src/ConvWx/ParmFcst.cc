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
 * @file ParmFcst.cc
 */

//----------------------------------------------------------------
#include <string>
#include <vector>
#include <algorithm>
#include <ConvWx/ParmFcst.hh>
#include <ConvWx/FcstGrid.hh>
#include <ConvWx/MultiGrid.hh>
#include <ConvWx/ConvWxConstants.hh>
#include <ConvWx/ConvWxTime.hh>
using std::string;
using std::vector;
using std::pair;

//----------------------------------------------------------------
ParmFcst::ParmFcst(void) :
  pOk(false),
  pName("not_set"),
  pDescription("not_set"),
  pIsRequired(false),
  pUrl("not_set"),
  pField(),
  pUnits(),
  pLtHour0(0),
  pLtHour1(0),
  pLtDeltaHours(0),
  pRemap(false),
  pGenFrequencySeconds(15*convWx::SECS_PER_MINUTE),
  pUniqueIntraHourLeadSets(false),
  pNumIntraHourGts(0),
  pLtSeconds0(0),
  pLtSeconds1(0),
  pLtDeltaSeconds(0)
{
}

//----------------------------------------------------------------
ParmFcst::ParmFcst(const string &name, const string &descr,
		   const bool isRequired) :
  pOk(true),
  pName(name),
  pDescription(descr),
  pIsRequired(isRequired),
  pUrl("not_set"),
  pField(),
  pUnits(),
  pLtHour0(0),
  pLtHour1(0),
  pLtDeltaHours(0),
  pRemap(false),
  pGenFrequencySeconds(15*convWx::SECS_PER_MINUTE),
  pUniqueIntraHourLeadSets(false),
  pNumIntraHourGts(0),
  pLtSeconds0(0),
  pLtSeconds1(0),
  pLtDeltaSeconds(0)
{
}

//----------------------------------------------------------------
ParmFcst::ParmFcst(const string &name, const string &descr,
		   const bool isRequired, const string &url,
		   const string &field, const string &units,
		   const double hour0, const double hour1,
		   const double deltaHours, const bool remap,
		   const int genDeltaMinutes, const bool uniqueIntraHourLeadSets) :
  pOk(true),
  pName(name),
  pDescription(descr),
  pIsRequired(isRequired),
  pUrl(url),
  pField(field),
  pUnits(units),
  pLtHour0(hour0),
  pLtHour1(hour1),
  pLtDeltaHours(deltaHours),
  pRemap(remap),
  pGenFrequencySeconds(genDeltaMinutes*convWx::SECS_PER_MIN),
  pUniqueIntraHourLeadSets(uniqueIntraHourLeadSets),
  pNumIntraHourGts(0),
  pLtSeconds0(0),
  pLtSeconds1(0),
  pLtDeltaSeconds(0)
{
  if (pUniqueIntraHourLeadSets)
  {
    pNumIntraHourGts = convWx::SECS_PER_HOUR/pGenFrequencySeconds;
  }
  pOk = pIsPeriodic();
  if (!pOk)
  {
    exit(1);
  }
  convertToSeconds(pLtHour0, pLtSeconds0);
  convertToSeconds(pLtHour1, pLtSeconds1);
  convertToSeconds(pLtDeltaHours, pLtDeltaSeconds);
  pCheckLtConsistency();
}

//----------------------------------------------------------------
ParmFcst::~ParmFcst()
{
}

//----------------------------------------------------------------
void ParmFcst::setDetails(const string &url, const string &field,
			  const string &units, const double hour0,
			  const double hour1, const double deltaHours,
			  const bool remap,  const int genDeltaMinutes,
                          const bool uniqueIntraHourLeadSets)
{
  pUrl = url;
  pField = field;
  pUnits = units;
  pLtHour0 = hour0;
  pLtHour1 = hour1;
  pLtDeltaHours = deltaHours;
  pRemap = remap;
  pGenFrequencySeconds = genDeltaMinutes*convWx::SECS_PER_MIN;
  pUniqueIntraHourLeadSets = uniqueIntraHourLeadSets;
  pNumIntraHourGts = convWx::SECS_PER_HOUR/pGenFrequencySeconds;
  pOk = pIsPeriodic();
  if (!pOk)
  {
    exit(1);
  }
  convertToSeconds(pLtHour0, pLtSeconds0);
  convertToSeconds(pLtHour1, pLtSeconds1);
  convertToSeconds(pLtDeltaHours, pLtDeltaSeconds);
  pCheckLtConsistency();
}

//----------------------------------------------------------------
void ParmFcst::setUniqueIntraHourLeadSets()
{
  pUniqueIntraHourLeadSets = true;
  pNumIntraHourGts = convWx::SECS_PER_HOUR/pGenFrequencySeconds;
}

#ifdef NOTDEF
//----------------------------------------------------------------
void ParmFcst::setDetails(const string &url, const string &field,
			  const string &units, const double hour0,
			  const double hour1, const double deltaHours,
			  const bool remap)
{
  pUrl = url;
  pField = field;
  pUnits = units;
  pLtHour0 = hour0;
  pLtHour1 = hour1;
  pLtDeltaHours = deltaHours;
  pRemap = remap;
  pGen15 = false;
  pGenUnknown = true;
  
  pOk = true;
}
#endif

//----------------------------------------------------------------
bool ParmFcst::isLastLt(const int ltSeconds) const
{
  return pLtSeconds1 == ltSeconds;
}

//----------------------------------------------------------------
int ParmFcst::nlead(const time_t gt) const
{
   int gtIndex = getIntraHourGenIndex(gt);
  
   if (gtIndex == 0)
   {
     return nlead();
   }
   else
   {
     // one less lead time for the gen times with minutes 0 < gt < 60 mins
     return nlead() - 1;
   }
}

//----------------------------------------------------------------
int ParmFcst::nlead(const int lt0, const int lt1) const
{
  int num=0;
  for (int lti=pLtSeconds0; lti<=pLtSeconds1; lti += pLtDeltaSeconds)
  {
    if (lti >= lt0 && lti <= lt1)
    {
      ++num;
    }
  }
  return num;
}

//----------------------------------------------------------------
double ParmFcst::ithLeadHours(const time_t gt, const int i) const
{
  double lh;

  int gtIndex = getIntraHourGenIndex(gt);

  if ( !pUniqueIntraHourLeadSets || gtIndex == 0)
  {
      lh = ithLeadHours(i);
  }
  else
  { 
    if (gtIndex == 0)
    {
       // assumes the i=0 lead hour is 0
       //lh = i * pLtDeltaHours;
       lh = (double)(i*pLtDeltaSeconds)/convWx::DOUBLE_SECS_PER_HOUR;
    }
    else
    { 
       lh = (double)((i+1)*pLtDeltaSeconds)/convWx::DOUBLE_SECS_PER_HOUR  -
          gtIndex * static_cast<double>(pGenFrequencySeconds)/convWx::DOUBLE_SECS_PER_HOUR ;
    }
    if (lh > pLtHour1)
    {
       return convWx::BAD_INDEX;
    }
  } 
  return lh;
}

//----------------------------------------------------------------
int ParmFcst::ithLeadSeconds(const time_t gt, const int i) const
{
   int lt;
   int gtIndex = getIntraHourGenIndex(gt);
   if ( !pUniqueIntraHourLeadSets || (gtIndex == 0))
   {
      lt = ithLeadSeconds(i);
   }
   else
   {
      if (gtIndex ==0)
      {
         lt = i*pLtDeltaSeconds;
      }
      else
      {
         lt = (i+1)*pLtDeltaSeconds - gtIndex*pGenFrequencySeconds;     
      }

      if (lt > pLtSeconds1)
      {
         return convWx::BAD_INDEX;
      }
   }
   return lt;
}

//----------------------------------------------------------------
int ParmFcst::leadSecondsIndex(const int lt) const
{
  
  for (int i=0, lti=pLtSeconds0; lti<=pLtSeconds1; lti += pLtDeltaSeconds, ++i)
  {
    if (lti == lt)
    {
      return i;
    }
  }
  return convWx::BAD_INDEX;
}

//----------------------------------------------------------------
int ParmFcst::leadSecondsIndex(const time_t gt, const int lt) const
{
   int gtIndex = getIntraHourGenIndex(gt);
   
   int maxLeads = nlead();
   
   for (int i=0; i< maxLeads; ++i)
   { 
      int lti;

      if (gtIndex == 0)
      {
         lti = i * pLtDeltaSeconds;
      }
      else
      {
         lti = (i+1) * pLtDeltaSeconds - gtIndex * pGenFrequencySeconds;
      }
      if (lti == lt)
      {
         return i;
      }
   }
   return convWx::BAD_INDEX;
}

//----------------------------------------------------------------
vector<double> ParmFcst::getLtHours(void) const
{
  vector<double> ret;
  for (int s=pLtSeconds0; s<=pLtSeconds1; s += pLtDeltaSeconds)
  {
    double h = (double)s/convWx::DOUBLE_SECS_PER_HOUR;
    ret.push_back(h);
  }
  return ret;
}

//----------------------------------------------------------------
vector<double> ParmFcst::getLtHours(const time_t gt) const
{
  vector<double> ret;
  
  if (!pUniqueIntraHourLeadSets)
  {
     ret = getLtHours();
  }
  else
  {
     for (int i=0; i<nlead(); ++i)
     {
        int leadSecs = ithLeadSeconds(gt,i);
        if (leadSecs != convWx::BAD_INDEX)
        {
           double ltHour = static_cast<double>(leadSecs ) / convWx::DOUBLE_SECS_PER_HOUR; 

           ret.push_back( ltHour);
        }
     }
  }
  return ret;
}

//----------------------------------------------------------------
std::vector<int> ParmFcst::getLtSeconds() const
{
  vector<int> ret;

  for (int s=pLtSeconds0; s<=pLtSeconds1; s += pLtDeltaSeconds)
  {
    ret.push_back(s);
  }
  return ret;
}

//----------------------------------------------------------------
std::vector<int> ParmFcst::getLtSeconds(const time_t gt) const
{
  vector<int> ret;

  if (!pUniqueIntraHourLeadSets)
  {
     ret = getLtSeconds();
  }
  else
  {
     int gtIndex = getIntraHourGenIndex( gt);

     for (int i=0; i< nlead(gt); ++i)
     {
        int leadSecs;

        if (gtIndex == 0)
        {
           leadSecs = i * pLtDeltaSeconds;
        }
        else
        {
           leadSecs = (i+1) * pLtDeltaSeconds - gtIndex * pGenFrequencySeconds;
        }

        ret.push_back(leadSecs);
     }
  } 
  return ret;
}

//----------------------------------------------------------------
std::vector<int> ParmFcst::getAllLtSeconds() const
{
  vector<int> ret;

  LOG(DEBUG) << "pUniqueIntraHourLeadSets: " << pUniqueIntraHourLeadSets;
  if ( !pUniqueIntraHourLeadSets)
  {
     ret = getLtSeconds();
  }
  else
  {
     for (int j=0; j < pNumIntraHourGts; ++j)
     {
        for (int i=0; i<nlead(); ++i)
        {
     
           int leadSecs = i * pLtDeltaSeconds - j * pGenFrequencySeconds;

           if ( leadSecs >= 0)
           {
              ret.push_back(leadSecs);
           }
        }
     }
  }
  return ret;
}


//----------------------------------------------------------------
vector<pair<time_t,int> > ParmFcst::fcstsValidAt(const time_t &vt) const
{
  vector<pair<time_t,int> > ret;

  int lt0 = pLtSeconds0; //static_cast<int>(pLtHour0*convWx::DOUBLE_SECS_PER_HOUR);
  int max = pLtSeconds1; //static_cast<int>(pLtHour1*convWx::DOUBLE_SECS_PER_HOUR);
  int dlt = pLtDeltaSeconds; //static_cast<int>(pLtDeltaHours*convWx::DOUBLE_SECS_PER_HOUR);
  time_t t0 = vt - max;
  t0 = smallestGenGe(t0);

  // put all the appropriate generation times onto the list in ascending order
  // (those with a lead time that is in range)
  vector<time_t> gtList;
  while (t0 <= vt)
  {
    int lt = vt - t0;
    // is this lead time a valid lead time for this object?
    if (lt >= lt0 && lt <= max)
    {
      double dk = static_cast<double>(lt-lt0)/static_cast<double>(dlt);
      int k = static_cast<int>(dk);
      if (k*dlt + lt0 == lt)
      {
	gtList.push_back(t0);
      }
      else
      {
	LOG(WARNING) << "lead not on list " << lt;
      }
    }
    t0 += pGenFrequencySeconds;
  }

  // pull back out in reverse order for return
  vector<time_t>::reverse_iterator ig;
  for (ig=gtList.rbegin(); ig!=gtList.rend(); ++ig)
  {
    int lt = vt - *ig;
    pair<time_t, int> p(*ig, lt);
    ret.push_back(p);
  }
  return ret;
}


//----------------------------------------------------------------
vector<time_t> ParmFcst::genPossible(const time_t t0,
				     const int latencyHours[2],
				     const time_t oldest) const
{
  vector<time_t> ret;

  // this is the absolute largest generation time we could have.
  time_t t1 = t0 - static_cast<int>(latencyHours[0]*
				    convWx::DOUBLE_SECS_PER_HOUR);

  // adjust to allowed generation times
  t1 = largestGenLe(t1);

  // this is absolute smallest generation time we could have
  time_t tmin = t0 - static_cast<int>(latencyHours[1]*
				      convWx::DOUBLE_SECS_PER_HOUR);

  // adjust to allowed generation times
  tmin = smallestGenGe(tmin);

  // adjust using oldest
  if (tmin < oldest)
  {
    tmin = oldest;
  }

  // now build up all generation times in this ranges
  time_t ti;

  for (ti=tmin; ti<=t1; ti += pGenFrequencySeconds)
  {
    ret.push_back(ti);
  }

  LOG(DEBUG_VERBOSE) << "Range of possible gen times:";
  for (int i=0; i<static_cast<int>(ret.size()); ++i)
  {
    LOG(DEBUG_VERBOSE) <<  ConvWxTime::stime(ret[i]);
  }
  return ret;
}

//----------------------------------------------------------------
time_t ParmFcst::smallestGenGe(time_t t) const
{
  // pull  out year/month/day of the time
  int y, m, d, h, min, sec;
  ConvWxTime::expandTime(t, y, m, d, h, min, sec);

  // t0 = earliest gen time in this day  ASSUMED TO BE 0,0,0
  time_t t0 = ConvWxTime::setTime(y, m, d, 0, 0, 0);
  while (t0 < t)
  {
    t0 += pGenFrequencySeconds;
  }
  // here t0 is the smallest gen time >= t
  return t0;
}

//----------------------------------------------------------------
time_t ParmFcst::largestGenLe(time_t t) const
{
  int y, m, d, h, min, sec;

  // pull out year/month/day of the time
  ConvWxTime::expandTime(t, y, m, d, h, min, sec);

  // t0 = earliest lead time in this day ASSUMED TO BE 0,0,0
  time_t t0 = ConvWxTime::setTime(y, m, d, 0, 0, 0);
  // t1 = gen time one beyond t0
  time_t t1 = t0 + pGenFrequencySeconds;
  while (t1 <= t)
  {
    t0 = t1;
    t1 += pGenFrequencySeconds;
  }
  // here t1 = 1st gen time > t, so t0 = last gen time <= t
  return t0;
}

//----------------------------------------------------------------
vector<int> ParmFcst::hourMinuteSeconds(void) const
{
  vector<int> ret;

  // take any old year/month/dayto build this up, starting at h,m,s=0,0,0
  int secondsIntoDay = 0;
  time_t t0 = ConvWxTime::setTime(2012, 07, 12, 0, 0, 0);
  time_t t1 = ConvWxTime::setTime(2012, 07, 13, 0, 0, 0);
  while (t0 < t1)
  {
    ret.push_back(secondsIntoDay);
    t0 += pGenFrequencySeconds;
    secondsIntoDay += pGenFrequencySeconds;
  }
  return ret;
}

//----------------------------------------------------------------
int ParmFcst::nearestLeadSeconds(int lt) const
{
  double minDiff = 0.0;
  int bestLt = 0;
  bool first = true;
  double dlt = static_cast<double>(lt);
  for (int s=pLtSeconds0; s<=pLtSeconds1; s += pLtDeltaSeconds)
  {
    double lts = s;
    if (first)
    {
      minDiff = fabs(lts - dlt);
      bestLt = s;
      first = false;
    }
    else
    {
      double x = fabs(lts - dlt);
      if (x < minDiff)
      {
	minDiff = x;
	bestLt = s;
      }
    }
    if (minDiff == 0.0)
    {
      return bestLt;
    }
  }
  if (first)
  {
    LOG(ERROR) << "Empty lead time list";
    return -1;
  }
  else
  {
    return bestLt;
  }
}

//----------------------------------------------------------------
int
ParmFcst::nearestLeadSecondsValidTime(const time_t &validTime,
				      const time_t &generationTime) const
{
  double minDiff = 0.0;
  int bestLt = 0;
  bool first = true;
  for (int lts=pLtSeconds0; lts<=pLtSeconds1; lts += pLtDeltaSeconds)
  {
    time_t vt = generationTime + lts;
    double diff = fabs(static_cast<double>(validTime - vt));
    if (first)
    {
      minDiff = diff;
      bestLt = lts;
      first = false;
    }
    else
    {
      if (diff < minDiff)
      {
	minDiff = diff;
	bestLt = lts;
      }
    }
    if (minDiff == 0.0)
    {
      return bestLt;
    }
  }
  if (first)
  {
    LOG(ERROR) << "Empty lead time list";
    return -1;
  }
  else
  {
    return bestLt;
  }
}

//----------------------------------------------------------------
void ParmFcst::pAdjustHmsDiff(const time_t &gt, const double hms0,
			      double &bestDiff, bool &first) const
{
  double hms = ConvWxTime::getTimeFraction(gt);
  double diffi = hms0 - hms;
  if (diffi >= 0)
  {
    LOG(DEBUG_VERBOSE) << "checking " << ConvWxTime::stime(gt);
    if (first)
    {
      bestDiff = diffi;
      first = false;
      LOG(DEBUG_VERBOSE) << "Initialize min delta to "<< bestDiff;
    }
    else
    {
      if (diffi < bestDiff) 
      {
	bestDiff = diffi;
	LOG(DEBUG_VERBOSE) << "Adjusting min delta to "<< bestDiff;
      }
    }
  }
  else
  {
    LOG(DEBUG_VERBOSE) << "skipping, too new:" << ConvWxTime::stime(gt);
  }
}

//----------------------------------------------------------------
bool ParmFcst::pIsPeriodic(void) const
{
  // take 24 hours and see if the gen delta is an exact multiple of that
  // Do this the dumb way to make sure it works, using an arbitrary day

  time_t t0 = ConvWxTime::setTime(2012, 07, 12, 0, 0, 0);
  time_t t1 = ConvWxTime::setTime(2012, 07, 13, 0, 0, 0);

  while (t0 < t1)
  {
    if (pGenFrequencySeconds == 0)
    {
      LOG(ERROR) << "Trying to check periodicity using gen frequency 0";
      return false;
    }
    t0 += pGenFrequencySeconds;
  }
  if (t0 != t1)
  {
    LOG(ERROR) << "Non periodic gen time delta seconds " << pGenFrequencySeconds
	       <<  ", url=" << pUrl;
    return false;
  }
  else
  {
    return true;
  }
}

//----------------------------------------------------------------
int ParmFcst::getIntraHourGenIndex( const time_t gt) const
{
   // Get the minutes after the hour of this gt. 
   struct tm * tmPtr;
   tmPtr = gmtime ( &gt );
   int min = tmPtr->tm_min;

  // For intra hour forecasts, enumerate the index this gen tima within the hour
  int intraHourGenIndex = (min * 60) / pGenFrequencySeconds;

  return intraHourGenIndex;
}

//----------------------------------------------------------------
void ParmFcst::convertToSeconds(double hours,  int& seconds)
{

   //
   // Lead delta should divide 3600 seconds
   // hard code for now for a min delta of 5 minutes
   //
   float floatDeltaSecs = 0.08333*3600;
    
   int deltaSecs;
   if ( (3600.0/floatDeltaSecs - int(3600.0/floatDeltaSecs)) <= .5)
       deltaSecs =  3600/ int(3600.0/floatDeltaSecs);
   else
      deltaSecs = 3600/(int(3600.0/floatDeltaSecs) + 1);

   //
   // deltaSecs should divide floatLeadHours evenly
   //
   float floatLeadSecs = hours * 3600.0;

   int leadSecs;
   if ( floatLeadSecs/deltaSecs - int(floatLeadSecs/deltaSecs) <= .5)
   {
       leadSecs = int(floatLeadSecs/deltaSecs) * deltaSecs;
   }
   else
   {
      leadSecs = (int(floatLeadSecs/deltaSecs) + 1) * deltaSecs;
   }
   seconds =  leadSecs;
}

//----------------------------------------------------------------
void ParmFcst::convertToSecondsWithinHour(double hours,  int& seconds)
{
   //
   // Lead delta should divide 3600 seconds
   // hard code for now for a min delta of 5 minutes
   //
   float floatDeltaSecs = 0.08333*3600;
    
   int deltaSecs;

   if ( (3600.0/floatDeltaSecs - int(3600.0/floatDeltaSecs)) <= .5)
       deltaSecs =  3600/ int(3600.0/floatDeltaSecs);
   else
      deltaSecs = 3600/(int(3600.0/floatDeltaSecs) + 1);

   //
   // deltaSecs should divide floatLeadHours evenly
   //
   float floatLeadSecs = hours * 3600.0;
   
   int leadSecs;

   if ( floatLeadSecs/deltaSecs - int(floatLeadSecs/deltaSecs) <= .5)
   {
       leadSecs = int(floatLeadSecs/deltaSecs) * deltaSecs;
   }
   else
   {
      leadSecs = (int(floatLeadSecs/deltaSecs) + 1) * deltaSecs;
   }
   seconds =  leadSecs;
}

//----------------------------------------------------------------
void ParmFcst::pCheckLtConsistency(void) const
{
   if (pLtDeltaSeconds == 0)
   {
      if (pLtSeconds0 != 0 || pLtSeconds1 != 0)
      {
         LOG(ERROR) << "Called consistency method without setting delta seconds";
         exit(1);
      }
      else
      {
         // empty state
         return;
      }
   }

   // expect pLtSeconds0 + X*pLtDeltaSeconds = pLtSeconds1 exactly, for some integer X
   int totalDelta = pLtSeconds1 - pLtSeconds0;
   double ndelta = (double)totalDelta/(double)pLtDeltaSeconds;
   int indelta = (int)ndelta;
   int sec1 = pLtSeconds0 + indelta*pLtDeltaSeconds;
   if (sec1 != pLtSeconds1)
   {
      LOG(ERROR) << "Inconsistent lead time settings " << pLtSeconds0 << ","
                 << pLtSeconds1 << "," << pLtDeltaSeconds;
      exit(1);
   }
}
