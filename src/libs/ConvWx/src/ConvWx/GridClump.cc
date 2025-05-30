// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2016                                         
// ** University Corporation for Atmospheric Research (UCAR)                 
// ** National Center for Atmospheric Research (NCAR)                        
// ** Boulder, Colorado, USA                                                 
// ** BSD licence applies - redistribution and use in source and binary      
// ** forms, with or without modification, are permitted provided that       
// ** the following conditions are met:                                      
// ** 1) If the software is modified to produce derivative works,            
// ** such modified software should be clearly marked, so as not             
// ** to confuse it with the version available from UCAR.                    
// ** 2) Redistributions of source code must retain the above copyright      
// ** notice, this list of conditions and the following disclaimer.          
// ** 3) Redistributions in binary form must reproduce the above copyright   
// ** notice, this list of conditions and the following disclaimer in the    
// ** documentation and/or other materials provided with the distribution.   
// ** 4) Neither the name of UCAR nor the names of its contributors,         
// ** if any, may be used to endorse or promote products derived from        
// ** this software without specific prior written permission.               
// ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
// ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
// ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
/**
 * @file GridClump.cc
 */

#include <ConvWx/GridClump.hh>

#include <vector>
// #include <euclid/PointList.hh>
#include <toolsa/LogStream.hh>
using std::vector;
using std::pair;

//----------------------------------------------------------------
GridClump::GridClump(const GridData &g) : GridData(g), _iwork(g)
{
  _nx = g.getNx();
  _ny = g.getNy();
  _iwork.setAllToValue(NO_MARKER);
  for (int i=0; i<_nx*_ny; ++i)
  {
    if (isMissingAt(i))
    {
      _iwork.setv(i, PROCESSED);
    }
  }
  _n.clear();
}

//----------------------------------------------------------------
GridClump::GridClump(const GridData &g, const double bad) : GridData(g),
						    _iwork(g)
{
  _nx = g.getNx();
  _ny = g.getNy();
  _iwork.setAllToValue(NO_MARKER);
  for (int i=0; i<_nx*_ny; ++i)
  {
    double  v;
    if (getValue(i, v))
    {
      if (v == bad)
      {
	_iwork.setv(i, PROCESSED);
      }
    }
    else
    {
      _iwork.setv(i, PROCESSED);
    }
  }
  _n.clear();
}

//----------------------------------------------------------------
GridClump::~GridClump()
{
}

//----------------------------------------------------------------
std::vector<convwx_clump::Region_t> GridClump::buildRegions(void)
{
  int x, y;
  convwx_clump::Region_t l;
  vector<convwx_clump::Region_t> ret;

  for (y=0; y<_ny; ++y)
  {
    for (x=0; x<_nx; ++x)
    {
      if (_iwork.returnValue(x,y) == PROCESSED)
      {
	continue;
      }
      _buildRegion(x, y);
      if (!_n.empty())
      {
	ret.push_back(_n);
      }
    }
  }
  return ret;
}

#ifdef NOTYET
//----------------------------------------------------------------
std::vector<PointList> GridClump::buildRegionPointlists(void)
{
  int x, y;
  convwx_clump::Region_t l;
  vector<PointList> ret;

  for (y=0; y<_ny; ++y)
  {
    for (x=0; x<_nx; ++x)
    {
      if (_iwork(x,y) == PROCESSED)
      {
	continue;
      }
      _buildRegion(x, y);
      if (!_n.empty())
      {
	PointList P(_nx, _ny, _n);
	ret.push_back(P);
      }
    }
  }
  return ret;
}
#endif

//----------------------------------------------------------------
vector<convwx_clump::Region_t> GridClump::buildRegionsRecursive(void)
{
  int x, y;
  convwx_clump::Region_t l;
  vector<convwx_clump::Region_t> ret;

  for (y=0; y<_ny; ++y)
  {
    for (x=0; x<_nx; ++x)
    {
      if (_iwork.returnValue(x,y) == PROCESSED)
      {
	continue;
      }

      _buildRegionRecursive(x, y);
      if (!_n.empty())
      {
	ret.push_back(_n);
      }
    }
  }
  return ret;
}

//----------------------------------------------------------------
void GridClump::_buildRegion(int ix, int iy)
{
  _n.clear();
  
  while (true)
  {
    bool done;
    _iwork.setv(ix, iy, THISCLUMP_NOTDONE);
    pair<int, int> p = _growNonrecursive(ix, iy, done);
    if (done)
    {
      pair<int, int> p2 = _findNondone(done);
      if (done)
      {
	break;
      }
      ix = p2.first;
      iy = p2.second;
    }
    else
    {
      ix = p.first;
      iy = p.second;
    }
  }
  _buildN();
}

//----------------------------------------------------------------
void GridClump::_buildRegionRecursive(int ix, int iy)
{
  _n.clear();
  _growRecursive(ix, iy);
  _buildN();
}

//----------------------------------------------------------------
void GridClump::_growRecursive(int x, int y)
{
  int ix, iy;

  // make sure input point is part of the current clump.
  if (_iwork.returnValue(x,y) != NO_MARKER)
  {
    return;
  }

  _iwork.setv(x,y, THISCLUMP_DONE);

  // Recursively add neighboring points
  for ( iy=y-1; iy<=y+1; ++iy )
  {
    for ( ix=x-1; ix<=x+1; ++ix )
    {
      if (!_growOk(ix, iy, x, y))
      {
	continue;
      }
      _growRecursive(ix, iy);
    }
  }
}

//----------------------------------------------------------------
pair<int,int> GridClump::_growNonrecursive(int x, int y, bool &done)
{
  int ix, iy;
  int ix_ret, iy_ret;
  bool did_grow = false;

  // make sure input point is set properly.
  if (_iwork.returnValue(x, y) != THISCLUMP_NOTDONE)
  {
    LOG(WARNING) << "in clump";
  }
  
  // by the time through here, it'll be done.
  _iwork.setv(x, y, THISCLUMP_DONE);

  // Recursively add neighboring points and set one for return 
  for ( iy=y-1; iy<=y+1; ++iy )
  {
    for ( ix=x-1; ix<=x+1; ++ix )
    {
      if (!_growOkNonrecursive(ix, iy, x, y))
      {
	continue;
      }
      _iwork.setv(ix, iy, THISCLUMP_NOTDONE);
      ix_ret = ix;
      iy_ret = iy;
      did_grow = true;
    }
  }

  if (did_grow)
  {
    pair<int, int> p(ix_ret, iy_ret);
    done = false;
    return p;
  }
  else
  {
    done = true;
    pair<int, int> p(0,0);
    return p;
  }
}

//----------------------------------------------------------------
void GridClump::_buildN(void)
{
  _n.clear();

  int x, y;
  for (y=0; y<_ny; ++y)
  {
    for (x=0; x<_nx; ++x)
    {
      if (_iwork.returnValue(x, y) == THISCLUMP_DONE)
      {
	pair<int, int> p(x, y);
	_n.push_back(p);
	_iwork.setv(x, y, PROCESSED);
      }
      else if (_iwork.returnValue(x,  y) == THISCLUMP_NOTDONE)
      {
	LOG(ERROR) << "in GridClump";
      }
    }
  }
}

//----------------------------------------------------------------
pair<int, int> GridClump::_findNondone(bool &done) const
{
  int x, y;
  for (y=0; y<_ny; ++y)
  {
    for (x=0; x<_nx; ++x)
    {
      if (_iwork.returnValue(x, y) == THISCLUMP_NOTDONE)
      {
	pair<int, int> p(x, y);
	done = false;
	return p;
      }
    }
  }
  pair<int, int> p(0,0);
  done = true;
  return p;
}

//----------------------------------------------------------------
bool GridClump::_growOk(int ix, int iy, int x, int y) const
  {
    // The point has been dealt with (just added)
    if ( ix == x && iy == y )
    {
      return(false);
    }
    // Out of range of the data
    if ( ix < 0 || ix >= _nx)
    {
      return(false);
    }
    if ( iy < 0 || iy >= _ny)
    {
      return(false);
    }
    // Previously dealt with
    if (_iwork.returnValue(ix, iy) != NO_MARKER)
    {
      return false;
    }
    return(true); 
  }

//----------------------------------------------------------------
bool GridClump::_growOkNonrecursive(int ix, int iy, int x, int y) const
{
  // The point has been dealt with (just added)
  if ( ix == x && iy == y )
  {
    return(false);
  }
  // Out of range of the data
  if ( ix < 0 || ix >= _nx)
  {
    return(false);
  }
  if ( iy < 0 || iy >= _ny)
  {
    return(false);
  }

  // Previously dealt with
  if (_iwork.returnValue(ix, iy) == PROCESSED || _iwork.returnValue(ix, iy) == THISCLUMP_DONE)
  {
    return false;
  }

  // here either NO_MARKER or THISCLUMP_NOTDONE.
  return(true); 
}

