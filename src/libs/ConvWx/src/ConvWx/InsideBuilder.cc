/**
 * @file InsideBuilder.cc
 */

#include <ConvWx/InsideBuilder.hh>
#include <ConvWx/EdgeFinder.hh>
#include <toolsa/LogStream.hh>

//------------------------------------------------------------------
InsideBuilder::InsideBuilder(const EdgeFinder &e)
{
  // get a range
  e.getRange(pX0, pX1, pY0, pY1);
  
  // set our subset and make a boolean array for that
  pNx = pX1 - pX0 + 1;
  pNy = pY1 - pY0 + 1;
  pIsInside = new bool[pNx*pNy];

  LOG(DEBUG_VERBOSE) << "Range (" << pX0 << "," << pY0 <<") to (" << pX1 << "," << pY1 << ") so nx,ny=" <<  pNx << "," << pNy;


  // start with inside everywhere
  for (int i=0; i<pNx*pNy; ++i)
  {
    pIsInside[i] = true;
  }

  // erode away in all 4 search directions.
  Inside_t *dirX = new Inside_t[pNx*pNy];
  Inside_t *dirY = new Inside_t[pNx*pNy];

  LOG(DEBUG_VERBOSE) << "Setting INSIDE/OUTSIDE at each point";
  for (int iy=0; iy<pNy; ++iy)
  {
    LOG(DEBUG_VERBOSE) << "Checking y=" << iy << " left to right start OUT";
    bool out = true;
    for (int ix=0; ix<pNx; ++ix)
    {
      if (e.notEdge(ix, iy))
      {
	if (out)
	{
	  dirX[pPtr(ix, iy)] = OUT;
	}
	else
	{
	  LOG(DEBUG_VERBOSE) << "[" << ix << "," << iy << "] = MAYBE";
	  dirX[pPtr(ix, iy)] = MAYBE;
	}
      }
      else
      {
	out = false;
	LOG(DEBUG_VERBOSE) << "[" << ix << "," << iy << "] = IN";
	dirX[pPtr(ix, iy)] = IN;
      }
    }
    LOG(DEBUG_VERBOSE) << "Checking y=" << iy << "  right to left start OUT";
    out = true;
    for (int ix=pNx-1; ix>=0; --ix)
    {
      if (e.notEdge(ix, iy))
      {
	if (out)
	{
	  dirX[pPtr(ix, iy)] = OUT;
	}
      }
      else
      {
	out = false;
      }
    }
  }

  for (int ix=0; ix<pNx; ++ix)
  {
    LOG(DEBUG_VERBOSE) << "Checking x=" << ix << "  bottom to top start OUT";
    bool out = true;
    for (int iy=0; iy<pNy; ++iy)
    {
      if (e.notEdge(ix, iy))
      {
	if (out)
	{
	  //ILOGF(DEBUG_VERBOSE, "[%d,%d] = OUT", ix, iy);
	  dirY[pPtr(ix, iy)] = OUT;
	}
	else
	{
	  LOG(DEBUG_VERBOSE) << "[" << ix << "," << iy << "] = MAYBE";
	  dirY[pPtr(ix, iy)] = MAYBE;
	}
      }
      else
      {
	out = false;
	LOG(DEBUG_VERBOSE) << "[" << ix << "," << iy << "] = YES";
	dirY[pPtr(ix, iy)] = IN;
      }
    }
    LOG(DEBUG_VERBOSE) << "Checking x=" << ix << " top to bottom start out OUT";
    out = true;
    for (int iy=pNy-1; iy>=0; --iy)
    {
      if (e.notEdge(ix, iy))
      {
	if (out)
	{
	  // ILOGF(DEBUG_VERBOSE, "[%d,%d] = OUT", ix, iy);
	  dirY[pPtr(ix, iy)] = OUT;
	}
      }
      else
      {
	out = false;
      }
    }
  }

  LOG(DEBUG_VERBOSE) << "Fill inside/outside values now";
  for (int i=0; i<pNx*pNy; ++i)
  {
    if (dirY[i] == IN || dirX[i] == IN)
    {
      pIsInside[i] = true;
    }
    else if (dirY[i] == OUT || dirX[i] == OUT)
    {
      pIsInside[i] = false;
    }
    else
    {
      // at these 'ambiguous' points we leave things inside i.e. true
      // 'maybe' in both directions
      LOG(DEBUG_VERBOSE) << "AMBIGUOUS Point " << i;
    }
  }

  delete [] dirX;
  delete [] dirY;
}

//------------------------------------------------------------------
InsideBuilder::~InsideBuilder(void)
{
  delete [] pIsInside;
}

//------------------------------------------------------------------
bool InsideBuilder::isInside(const int x, const int y) const
{
  return pIsInside[pPtr(x, y)];
}

//------------------------------------------------------------------
int InsideBuilder::pPtr(const int x, const int y) const
{
  return y*pNx + x;
}
