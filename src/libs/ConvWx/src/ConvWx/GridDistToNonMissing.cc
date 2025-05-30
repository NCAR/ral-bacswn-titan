/**
 * @file GridDistToNonMissing.cc
 */

#include <ConvWx/GridDistToNonMissing.hh>
#include <toolsa/LogStream.hh>
#include <cmath>

//----------------------------------------------------------------
GridDistToNonMissing::GridDistToNonMissing(int maxSearch, int searchScale) :
  pMaxSearch(maxSearch), pSearchScale(searchScale), pNx(0), pNy(0)
{
}

//----------------------------------------------------------------
GridDistToNonMissing::~GridDistToNonMissing()
{
}

//----------------------------------------------------------------
void GridDistToNonMissing::update(const GridData &g)
{
  int nx, ny;
  g.getDim(nx, ny);
  if (nx != pNx || ny != pNy)
  {
    pNx = nx;
    pNy = ny;
    pXIndex = GridData(nx, ny, MISSING);
    pYIndex = GridData(nx, ny, MISSING);
    pRebuild(g);
  }
  else
  {
    if (pMissingChanged(g))
    {
      pRebuild(g);
    }
  }
}

//----------------------------------------------------------------
bool GridDistToNonMissing::distanceToNonMissing(const ParmProjection &proj,
						const GridData &data,
						GridData &distOut,
						GridData &valOut)
{      
  // check dimensions for consistency
  if (!data.sizeEqual(distOut) || !data.sizeEqual(valOut))
  {
    LOG(ERROR) << "Dimensions inconsistent";
    return false;
  }

  // update if update is needed
  update(data);

  double maxRadius = pMaxSearch*proj.kmPerPixel();

  // At each grid point x,y find and record minimum distances to non-missing
  // data and the non missing data value  
  for (int j = 0; j < pNy; j++)
  {
    for (int i = 0; i < pNx; i++)
    { 
      
      // Taxicab metric distance to non-missing data from (i,j)
      double minDist = maxRadius;   

      // Value of data at minimum distance
      double closeVal;

      // Flag to indicate successful search
      bool distFound = false;
      
      // Check right away if point is not missing or bad
      // since we need not go further and distance = 0 at such points
      if (!data.isMissingAt(i, j))
      { 
	distFound = true;
	minDist = 0.0;
	data.getValue(i, j, closeVal);
      }
      else
      {
	int nearI, nearJ;
	if ((distFound = nearestPoint(i, j, nearI, nearJ)))
	{
	  double di = fabs(nearI - i);
	  double dj = fabs(nearJ - j);
	  if (dj > di)
	  {
	    di = dj;
	  }
	  minDist = di*proj.kmPerPixel();
	  if (!data.getValue(nearI, nearJ, closeVal))
	  {
	    LOG(WARNING) << "Unexpected missing where not expected (" << nearI << "," << nearJ <<")";
	    distFound = false;
	  }
	}
      }
      
      if (!distFound)
      {
	// At points where no nearby data is non-missing, set result to max
	distOut.setv(i, j, maxRadius);
	valOut.setToMissing(i, j);
      }
      else
      {
	// Set the minimum distance to non missing data and the value 
	distOut.setv(i, j, minDist);
	valOut.setv(i, j, closeVal);
      }
    } // End for
  }// End for

  return true;
}

//----------------------------------------------------------------
bool GridDistToNonMissing::distanceToNonMissingSlow(const ParmProjection &proj,
                                                const GridData &data,
                                                GridData &distOut,
                                                GridData &valOut)
{

  int nx, ny;
  data.getDim(nx, ny);
  if (nx != pNx || ny != pNy)
  { 
    pNx = nx; 
    pNy = ny; 
  }
  LOG(ERROR) << "pNx " << pNx << "pNy " << pNy;
  // check dimensions for consistency
  if (!data.sizeEqual(distOut) || !data.sizeEqual(valOut))
  {
    LOG(ERROR) << "Dimensions inconsistent";
    return false;
  }

  LOG(ERROR) << "GridDistToNonMissing::distanceToNonMissingSlow";
  double maxRadius = pMaxSearch*proj.kmPerPixel();

  // At each grid point x,y find and record minimum distances to non-missing
  // data and the non missing data value  
  for (int j = 0; j < pNy; j++)
  {
    for (int i = 0; i < pNx; i++)
       {

          double minDist = maxRadius;

          // Value of data at minimum distance
          double closeVal;

          // Flag to indicate successful search
          bool distFound = false;

          //
          // Check right away if point is not missing or bad
          // since we need not go further and distance = 0 at such points
          //
          if (!data.isMissingAt(i, j))
          {
            distFound = true;
            minDist = 0.0;
            data.getValue(i, j, closeVal);
          }
          else
          {
             int  r = 1;

              double dist;

              dist = r*proj.kmPerPixel();

              while(!distFound && dist < maxRadius)
              {
                  //
                  // Check bottom edge and top edge of square around (i,j)
                  //
                  for (int y = j - r; y<= j + 2*r; y = y + 2*r )
                    {
                     for (int x = i - r; x<= i + r; x++)
                       {
                          dist = sqrt( pow( i-x,2) + pow(j-y,2));
                          if (x > 0 && x < pNx && y > 0 && y < pNy)
                            {

                               if (data.getValue(x, y, closeVal))
                                {
                                  if (dist < minDist)
                                  {
                                    minDist = dist;
                                    distFound = true;
                                  }
                                }
                            } //end if          
                       }
                    }
                  //
                  // Check the left edge and the right edge around (i,j)
                  //
                  for ( int x = i-r ; x <= i + r ; x = x + 2*r)
                    {
                      //
                      // We already checked corners so dont check twice
                      //
                      for (int y = j - r + 1 ;  y <= j + r -1; y++)
                        {
                          dist = sqrt( pow( i-x,2) + pow(j-y,2));
                          if (x > 0 && x < pNx && y > 0 && y < pNy)
                            {
                                if (data.getValue(x, y, closeVal))
                                {
                                  if (dist < minDist)
                                  {
                                    minDist = dist;
                                    distFound = true;
                                  }
                                }
                            } //end if           
                        }
                    }
                  r++;
                  //dist = r*proj.kmPerPixel();
                }// end while
          }// end else
        
         if( !distFound)
         {
            distOut.setv(i,j, maxRadius);
            valOut.setToMissing(i,j);
         }
         else
         {
            distOut.setv(i,j,minDist);
            valOut.setv(i,j,closeVal);
         }
      }
  }
  return true;
}

//----------------------------------------------------------------
bool GridDistToNonMissing::nearestPoint(int x, int y, int &nearX,
					int &nearY) const
{
  double dx, dy;
  if (pXIndex.getValue(x, y, dx) && pYIndex.getValue(x, y, dy))
  {
    if (dx == HAS_DATA)
    {
      nearX = x;
      nearY = y;
      return true;
    }
    else
    {
      nearX = static_cast<int>(dx);
      nearY = static_cast<int>(dy);
      return true;
    }
  }
  else
  {
    // nothing
    return false;
  }
}

//----------------------------------------------------------------
bool GridDistToNonMissing::pMissingChanged(const GridData &g) const
{
  int diff = 0;
  for (int i=0; i<g.getNdata(); ++i)
  {
    // is the new grid data missing at i?
    bool newMissing = g.isMissingAt(i);

    // the state says data was missing at i if either an index is set
    // indicating an offset 
    // (the value is not missing and is not HAS_DATA) or the
    // index is not set (the point has missing data and is out of the
    // max search distance)
    bool oldMissing;
    double v;
    if (pXIndex.getValue(i, v))
    {
      oldMissing = v != HAS_DATA;
    }
    else
    {
      oldMissing = true;
    }
    if (oldMissing != newMissing)
    {
      ++diff;
    }
  }
  if (diff > 0)
  {
    LOG(DEBUG_VERBOSE) << "Rebuild because " << diff << " points different";
    return true;
  }
  else
  {
    LOG(DEBUG_VERBOSE) << "Do not Rebuild because 0 points different";
    return false;
  }
}

//----------------------------------------------------------------
void GridDistToNonMissing::pRebuild(const GridData &g)
{
  // initialize to MISSING everywhere (MISSING is the missing value)
  pXIndex.setAllMissing();
  pYIndex.setAllMissing();


  // create a copy of the input grid  (gradually to be eroded away)
  GridData s(g);

  for (int r=pSearchScale; r<pMaxSearch; r += pSearchScale)
  {
    LOG(DEBUG_VERBOSE) << "Checking r=" << r << ",, nmissing=" <<s.nmissing();
    int totalNew=0;
    for (int y=0; y<pNy; ++y)
    {
      for (int x=0; x<pNx; ++x)
      {
	if (!s.isMissingAt(x, y))
	{
	  int nnew = 0;
	  nnew = pRebuild1(r, x, y, g);
	  totalNew += nnew;
	  if (nnew == 0)
	  {
	    // at larger search scales, we will want to skip over this point
	    // because the point was not used at a smaller scale.  This makes
	    // things more efficient.
	    s.setToMissing(x, y);
	  }
	}
      }
    }
    LOG(DEBUG_VERBOSE) << "R=" << r << " had " << totalNew << " new";
  }

  for (int y=0; y<pNy; ++y)
  {
    for (int x=0; x<pNx; ++x)
    {
      if (!g.isMissingAt(x, y))
      {
	pXIndex.setv(x, y, HAS_DATA);
	pYIndex.setv(x, y, HAS_DATA);
      }
    }
  }
}

//----------------------------------------------------------------
int GridDistToNonMissing::pRebuild1(int r, int x, int y, const GridData &g)
{
  // x,y is the point with data

  int nnew = 0;

  //
  // Search horizontal edges of box surrounding point (x, y)
  // with box lengths 2r
  for (int iy=y-r; iy<=y+r; iy += 2*r)
  {
    if (iy < 0 || iy >= pNy)
    {
      continue;
    }
    for (int ix = x-r; ix<=x+r; ix += pSearchScale)
    {
      if (ix >= 0 && ix < pNx)
      {
	if (g.isMissingAt(ix, iy))  
	{
	  if (pXIndex.isMissingAt(ix, iy))
	  {
	    // not yet set, set now
	    pXIndex.setv(ix, iy, x);
	    pYIndex.setv(ix, iy, y);
	    ++nnew;
	  }
	}
      }
    }
    //
    // Search vertical edges of box surrounding point (i,j)
    // with box lengths 2r
    // 
    for (int ix=x-r; ix<=x+r; ix+= 2*r)
    {
      if (ix < 0 || ix >= pNx)
      {
	continue;
      }
      for (int iy=y-r; iy<=y+r; iy += pSearchScale)
      {
	if (iy >= 0 && iy < pNy)
	{
	  if (g.isMissingAt(ix, iy))
	  {
	    if (pXIndex.isMissingAt(ix, iy))
	    {
	      // not yet set, set now
	      pXIndex.setv(ix, iy, x);
	      pYIndex.setv(ix, iy, y);
	      ++nnew;
	    }
	  }
	}
      }
    }
  }
  return nnew;
}

