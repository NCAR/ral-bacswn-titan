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
 * @file PcVolume.cc
 * @brief A phase correction volume box
 */

//----------------------------------------------------------------
#include <vector>
#include <string>
#include <ConvWx/PcVolume.hh>
#include <ConvWx/MultiGridsForPc.hh>
#include <ConvWx/GridsForPc.hh>
#include <ConvWx/Grid.hh>
#include <ConvWx/UvOutput.hh>
#include <ConvWx/Score.hh>
using std::vector;
using std::string;

//----------------------------------------------------------------
PcVolume::PcVolume(const int x0, const int y0, const ParmSetUV &parms) :
  pX0(x0),
  pY0(y0),
  pParm(parms),
  pXoff(0),
  pYoff(0),
  pScoreBest(0.0),
  pSbest(),
  pStats()
{
  LOG(DEBUG_VERBOSE) << "constructed PcVolume lower left = " << x0 << "," << y0;
}

//----------------------------------------------------------------
PcVolume::PcVolume(const PcVolume &p)
{
  pX0 = p.pX0;
  pY0 = p.pY0;
  pParm = p.pParm;
  pXoff = p.pXoff;
  pYoff = p.pYoff;
  pVscore = p.pVscore;
  pScoreBest = p.pScoreBest;
  pSbest = p.pSbest;
  pStats = p.pStats;
  // pScoreBestInitial = p.pScoreBestInitial;
  // pXoffInitial = p.pXoffInitial;
  // pYoffInitial = p.pYoffInitial;
  // pNumRefineCandidates = p.pNumRefineCandidates;
}

//----------------------------------------------------------------
PcVolume::~PcVolume()
{
}

//----------------------------------------------------------------
PcVolume & PcVolume::operator=(const PcVolume &p)
{
  if (this == &p)
  {
    return *this;
  }

  pX0 = p.pX0;
  pY0 = p.pY0;
  pParm = p.pParm;
  pXoff = p.pXoff;
  pYoff = p.pYoff;
  pVscore = p.pVscore;
  pScoreBest = p.pScoreBest;
  pSbest = p.pSbest;
  pStats = p.pStats;
  // pScoreBestInitial = p.pScoreBestInitial;
  // pXoffInitial = p.pXoffInitial;
  // pYoffInitial = p.pYoffInitial;
  // pNumRefineCandidates = p.pNumRefineCandidates;
  return *this;
}


//----------------------------------------------------------------
bool PcVolume::operator==(const PcVolume &p) const
{
  return (pX0 == p.pX0 &&
	  pY0 == p.pY0 &&
	  // pParm == p.pParm &&
	  pXoff == p.pXoff &&
	  pYoff == p.pYoff &&
	  pVscore == p.pVscore &&
	  pScoreBest == p.pScoreBest &&
	  pSbest == p.pSbest &&
	  pStats == p.pStats
	  // pScoreBestInitial == p.pScoreBestInitial &&
	  // pXoffInitial == p.pXoffInitial &&
	  // pYoffInitial == p.pYoffInitial &&
	  // pNumRefineCandidates == p.pNumRefineCandidates
	  );
}

//----------------------------------------------------------------
void PcVolume::print(void) const
{    
}

//----------------------------------------------------------------
void PcVolume::getPcOfSmoothed(const MultiGridsForPc &pcGrids)
{
  pStats = PcStats1(); 
  pXoff = pYoff = 0;

  // make sure there is enough data to continue..fractional area test.
  if (!pcGrids.fractionalAreaTest(pX0, pY0, pParm.pVolSize, 
				  pParm.pFractionalAreaMinPcnt))
  {
    return;
  }

  // build the initial set of scores
  if (!pBuildScore(pcGrids))
  {
    LOG(DEBUG_VERBOSE) << "NO P.C. since no scores worked out";
    return;
  }

  // refine those scores
  if (!pRefineScore(pcGrids))
  {
    LOG(DEBUG_VERBOSE) << "NO P.C. since no scores worked out";
    return;
  }

  // do distance minimization to possibly choose a new best score.
  pDistanceMinimization();

  // pull out that offset, which is it.
  pSbest.getOffset(pXoff, pYoff);
  pStats.updateFinal(pScoreBest, pXoff, pYoff);
  LOG(DEBUG_VERBOSE) << "------- FINAL:at " << pX0 << "," << pY0 << " Chose offset " << pXoff << "," << pYoff << "------";
}

//----------------------------------------------------------------
void PcVolume::addToMotion(UvOutput &uvGrid, Grid &countGrid) const
{
  // update the motion values at the 'from' locations.
  for (int iy=pY0; iy<pY0+pParm.pVolSize[1]; ++iy)
  {
    for (int ix=pX0; ix<pX0+pParm.pVolSize[0]; ++ix)
    {
      int fy = iy - pYoff;
      int fx = ix - pXoff;
      if (countGrid.inRange(fx, fy))
      {
	// pXoff, pYoff is the motion, stored at fx, fy
	uvGrid.incrementValueAtPoint(fx, fy, pXoff, pYoff);
	countGrid.incrementValueAtPoint(fx, fy, 1.0);
      }
    }
  }
}

//----------------------------------------------------------------
bool PcVolume::pBuildScore(const MultiGridsForPc &pcGrids)
{
  int nscore = pParm.numScore();
  bool first= true;
  pScoreBest=0.0;
  pVscore.clear();
  LOG(DEBUG_VERBOSE) << "--------- " << nscore << "*" << nscore <<" scores to check-------";
  for (int y = 0; y<nscore; ++y)
  {
    for (int x=0; x<nscore; ++x)
    {
      int iy = pParm.ithOffset(y);
      int ix = pParm.ithOffset(x);
      pBuildScore(ix, iy, pcGrids, first);
    }
  }
  return !first;
}

//----------------------------------------------------------------
void PcVolume::pBuildScore(const int ix, const int iy,
			   const MultiGridsForPc &pcGrids,
			   bool &first)
{
  Score s(ix, iy);
  string debugMsg;
  double sxy = s.setScoreFromSmoothed(pX0, pY0, pcGrids, pParm, debugMsg);
  LOG(DEBUG_VERBOSE) << "Compute for offset " << ix << "," << iy << "  ."<<  debugMsg;
  pVscore.push_back(s);
  if (!s.isBadScore())
  {
    if (first)
    {
      first = false;
      pScoreBest = sxy;
    }
    else
    {
      if (sxy < pScoreBest)
      {
	pScoreBest = sxy;
	pStats.updateInitial(sxy, ix, iy);
      }
    }
  }
}

//----------------------------------------------------------------
bool PcVolume::pRefineScore(const MultiGridsForPc &pcGrids)
{
  int nrefine = 0;
  int nbest = 0;
  char buf[1000];
  sprintf(buf, "--------- best score %.2lf-----------",
	  pScoreBest);
  LOG(DEBUG_VERBOSE) << buf;

  vector<Score>::iterator si;
  for (si=pVscore.begin(); si!=pVscore.end(); ++si)
  {
    if (si->isBadScore())
    {
      continue;
    }
    double sxy = si->getScore();
    if (sxy <= pParm.pGoodScaling*pScoreBest)
    {
      ++nrefine;
      sxy = si->refineFromSmoothed(pX0, pY0, pcGrids, pParm);
    }
  }

  pStats.pNumRefineTested = nrefine;
  // pNumRefineCandidates = nrefine;

  // go through now and find the new best
  bool first = true;
  for (si=pVscore.begin(); si!=pVscore.end(); ++si)
  {
    if (si->isBadScore())
    {
      continue;
    }
    double sxy = si->getScore();
    if (first)
    {
      pSbest = *si;
      pScoreBest = sxy;
      ++nbest;
      first = false;
    }
    else
    {
      if (sxy < pScoreBest)
      {
	pScoreBest = sxy;
	pSbest = *si;
	++nbest;
      }
    }
  }
  return !first;
}

//----------------------------------------------------------------
void PcVolume::pDistanceMinimization(void)
{
  double dbest = pSbest.getDistance();
  double dxy;
  LOG(DEBUG_VERBOSE) << "---------- best=", pSbest.sprint();

  // adjust best to minimize distance
  vector<Score>::iterator si;
  for (si=pVscore.begin(); si!=pVscore.end(); ++si)
  {
    if (si->isBadScore())
    {
      continue;
    }
    double sxy = si->getScore();
    if (sxy <= pParm.pGoodDistScaling*pScoreBest)
    {
      dxy = si->getDistance();
      if (dxy < dbest)
      {
	pSbest = *si;
	LOG(DEBUG_VERBOSE) << "Change to smaller p.c. distance " << pSbest.sprint();
	dbest = dxy;
      }
      else
      {
	LOG(DEBUG_VERBOSE) << "   Pretty good but dist not smaller " << si->sprint();
      }
    }
  }
}

//----------------------------------------------------------------
void PcVolume::updateStats(PcStats &stats) const
{
  stats.update(pStats);
}

