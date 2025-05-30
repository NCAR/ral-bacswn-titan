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
///////////////////////////////////////////////////////////////
// VectorsAdvector.cc
//
// VectorsAdvector class
//
// Nancy Rehak, RAP, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// March 2001
//
///////////////////////////////////////////////////////////////

#include <advect/VectorsAdvector.hh>
#include <euclid/CircularTemplate.hh>
#include <euclid/EllipticalTemplate.hh>
#include <euclid/Pjg.hh>
#include <toolsa/toolsa_macros.h>
#include <toolsa/pjg_flat.h>
#include <toolsa/pmu.h>
#include <math.h>

//////////////
// Constructor

VectorsAdvector::VectorsAdvector(const double vector_spacing,
                                 const double smoothing_radius,
                                 const bool debug_flag) :
  _debugFlag(debug_flag),
  _vectorSpacing(vector_spacing),
  _smoothingRadius(smoothing_radius),
  _motionWtData(0),
  _motionWtCount(0),
  _motionUData(0),
  _motionAdvectUData(0),
  _motionVData(0),
  _motionAdvectVData(0),
  _soundingUComp(0),
  _soundingVComp(0) {
  if (_debugFlag) {
    cerr << "In debug mode" << endl;
  }
  _mbuf = MEMbufCreate();
  _nVectors = 0;
  _vectors = (vectors_t *) MEMbufPtr(_mbuf);

}

/////////////
// destructor

VectorsAdvector::~VectorsAdvector() {
  MEMbufDelete(_mbuf);

  delete[] _motionWtData;
  delete[] _motionWtCount;
  delete[] _motionUData;
  delete[] _motionAdvectUData;
  delete[] _motionVData;
  delete[] _motionAdvectVData;
}

////////////////////////////
// loadVectors()
//
// Load up the list of vectors to be used
// for the forecast.
//
// Returns true on success, false on failure

bool VectorsAdvector::loadVectors(const Pjg &projection,
                                  const fl32 *u_data, const fl32 u_missing,
                                  const fl32 *v_data, const fl32 v_missing) {
  const string method_name = "VectorsAdvector::loadVectors()";

  // Clear out the old vectors

  MEMbufReset(_mbuf);
  _nVectors = 0;
  _vectorsPrecomputed = false;

  // Load the vectors

  int xSpacing, ySpacing;

  xSpacing = (int) MAX(projection.km2xGrid(_vectorSpacing), 1.0);
  ySpacing = (int) MAX(projection.km2yGrid(_vectorSpacing), 1.0);

  // loop through y and x loading up the vectors

  int nx = projection.getNx();
  int ny = projection.getNy();
  double dx = projection.getDx();
  double dy = projection.getDy();
  double minx = projection.getMinx();
  double miny = projection.getMiny();

  for (int iy = ySpacing / 2; iy < ny; iy += ySpacing) {
    int i = iy * nx + xSpacing / 2;

    PMU_auto_register("VectorsAdvector::loadVectors ... loading ...");

    for (int ix = xSpacing / 2; ix < nx; ix += xSpacing, i += xSpacing) {
      fl32 u_value = u_data[i];
      fl32 v_value = v_data[i];

      if (u_value != u_missing && v_value != v_missing) {
        // compute vector

        vectors_t vec;

        vec.u = u_value;
        vec.v = v_value;

        double xx = minx + dx * ix;
        double yy = miny + dy * iy;

        projection.xy2latlon(xx, yy, vec.lat, vec.lon);

        // add to membuf

        MEMbufAdd(_mbuf, &vec, sizeof(vectors_t));
        _nVectors++;

      } // if (u_value != u_missing && v_value != v_missing) 

    } // ix

  } // iy

  // set vector array pointer

  _vectors = (vectors_t *) MEMbufPtr(_mbuf);

  return true;

}


////////////////////////////
// loadSounding()
//
// Load the sounding to be used for the forecast.  Sounding components
// are used to fill in the missing data spaces in the motion grid when
// the forecast vectors are recomputed in precompute().  The sounding
// components default to 0.0 when this object is created.
//
// Returns true on success, false on failure

bool VectorsAdvector::loadSounding(const double u_comp, const double v_comp) {
  _soundingUComp = u_comp;
  _soundingVComp = v_comp;

  return true;
}

////////////////////////////
// precompute()
//
// Precompute the forecast vectors.
//
// Returns true on success, false on failure

bool VectorsAdvector::precompute(const Pjg &projection,
                                 const int lead_time_secs) {
  int nx = projection.getNx();
  int ny = projection.getNy();
  int old_nx = _motionProjection.getNx();
  int old_ny = _motionProjection.getNy();

  // If none of the important information has changed, we are
  // already precomputed.

  if (lead_time_secs == _leadTimeSecs &&
      projection == _motionProjection &&
      _vectorsPrecomputed) {
    // Save the new projection in case some of the other information
    // is different.

    _motionProjection = projection;

    // Return because we don't need to do any of the calculations

    return true;
  }

  // allocate motion grids

  if (nx != old_nx ||
      ny != old_ny) {
    delete[] _motionWtData;
    delete[] _motionWtCount;
    delete[] _motionUData;
    delete[] _motionVData;
    delete[] _motionAdvectUData;
    delete[] _motionAdvectVData;

    _motionWtData = new fl32[nx * ny];
    _motionWtCount = new int16_t[nx * ny];
    _motionUData = new fl32[nx * ny];
    _motionVData = new fl32[nx * ny];
    _motionAdvectUData = new fl32[nx * ny];
    _motionAdvectVData = new fl32[nx * ny];
  }

  memset(_motionWtData, 0, nx * ny * sizeof(fl32));
  memset(_motionWtCount, 0, nx * ny * sizeof(int16_t));
  memset(_motionUData, 0, nx * ny * sizeof(fl32));
  memset(_motionVData, 0, nx * ny * sizeof(fl32));
  memset(_motionAdvectUData, 0, nx * ny * sizeof(fl32));
  memset(_motionAdvectVData, 0, nx * ny * sizeof(fl32));

  // Save the new motion projection and lead time

  _motionProjection = projection;
  _leadTimeSecs = lead_time_secs;

  // load up the grid with forecast motion vectors

  double major_axis = MAX(2.0 * projection.km2xGrid(_smoothingRadius), 1.0);
  double minor_axis = MAX(2.0 * projection.km2yGrid(_smoothingRadius), 1.0);

  EllipticalTemplate grid_template(0.0,
                                   major_axis, minor_axis);

  _loadMotionGrid(grid_template,
                  lead_time_secs);

  _vectorsPrecomputed = true;

  return true;
}


////////////////////////////
// calcFcstIndex()
//
// Calculate the grid index of the original grid location from this
// forcast grid location.
//
// Returns the calculated grid index if successful, returns -1 if
// the original location is outside of the grid or if there is no
// motion in that location.

int VectorsAdvector::calcFcstIndex(const int x_index,
                                   const int y_index) {
  int nx = _motionProjection.getNx();
  int ny = _motionProjection.getNy();

  int index = x_index + (y_index * nx);
  
  // compute the number of grid points moved
      
  double x_km = _motionUData[index] * (double)_leadTimeSecs / 1000.0;
  double y_km = _motionVData[index] * (double)_leadTimeSecs / 1000.0;
	
  int x_grid_offset = (int)(_motionProjection.km2xGrid(x_km) + 0.5);
  int y_grid_offset = (int)(_motionProjection.km2yGrid(y_km) + 0.5);
	
  int fcst_x = x_index - x_grid_offset;
  int fcst_y = y_index - y_grid_offset;

  int fcst_index;
  if (fcst_x < 0 || fcst_x >= nx ||
      fcst_y < 0 || fcst_y >= ny) {
    fcst_index = -1;
  }
  else {
    fcst_index = fcst_x + (fcst_y * nx);
  }

  // Compute a vector forecast index for vector extrapolation.
  // In this case the vector is pushed forward instead of
  // looking back and pulling forward.

  int vector_fcst_x = x_index + x_grid_offset;
  int vector_fcst_y = y_index + y_grid_offset;

  if (vector_fcst_x < 0 || vector_fcst_x >= nx ||
      vector_fcst_y < 0 || vector_fcst_y >= ny) {
    return fcst_index;
  }

  int vector_fcst_index = vector_fcst_x + (vector_fcst_y * nx);

  if (vector_fcst_index != index) {
    _motionAdvectUData[vector_fcst_index] = _motionUData[index];
    _motionAdvectVData[vector_fcst_index] = _motionVData[index];
  }

  return fcst_index;

  // Removing this piece of code, at least for now. There are situations where
  // this leaves gaps in the middle of large data areas. This can be seen in
  // satellite data where higher cloud tops are moving and lower cloud tops
  // are not. It's not realistic to leave areas of missing data in those areas.

  // Fixes an issue where a storm just outside a vector field stays in place
  // and can also get moved when looking back from a vector and pulling forward.
  // This will duplicate the feature in the image data.

  double fcst_x_km = _motionUData[fcst_index] * (double) _leadTimeSecs / 1000.0;
  double fcst_y_km = _motionVData[fcst_index] * (double) _leadTimeSecs / 1000.0;
  int fcst_x_grid_offset = (int) (_motionProjection.km2xGrid(fcst_x_km) + 0.5);
  int fcst_y_grid_offset = (int) (_motionProjection.km2yGrid(fcst_y_km) + 0.5);

  if (fcst_index != index &&
      fcst_x_grid_offset == 0 &&
      fcst_y_grid_offset == 0) {
    return -1;
  } else {
    return fcst_index;
  }

}

/////////////////////
// _loadMotionGrid()
//
// Load up grid with forecast motion vector values.
// The values are placed in the grid at the forecast location rather
// than the starting location, so that we can look BACK to
// find the pixels from which the forecast is constructed.
//
// Returns true on success, false on failure.
//

bool VectorsAdvector::_loadMotionGrid(GridTemplate &grid_template,
                                      const int lead_time_secs) {
  // loop through the vectors

  vectors_t *vecs = _vectors;

  for (int i = 0; i < _nVectors; i++, vecs++) {
    PMU_auto_register("VectorsAdvector::_loadMotionGrid ... loading ...");

    // Compute the ending lat/lon position of each vector

    double speed, direction;

    _calcSpeedDirection(vecs->u, vecs->v, speed, direction);

    double r = speed * (double) lead_time_secs / 1000.0;

    double end_lat, end_lon;

    PJGLatLonPlusRTheta(vecs->lat, vecs->lon, r, direction,
                        &end_lat, &end_lon);

    // load grid for this vector with weighted u,v values

    _loadGridForVector(end_lat, end_lon,
                       vecs->u, vecs->v,
                       grid_template);

  } // i

  // Compute the means of the velocity vectors for each grid point

  int nx = _motionProjection.getNx();
  int ny = _motionProjection.getNy();

  for (int i = 0; i < nx * ny; i++) {
    if (_motionWtData[i] > 0 && _motionWtCount[i] > 5) {
      _motionUData[i] /= _motionWtData[i];
      _motionVData[i] /= _motionWtData[i];
      _motionAdvectUData[i] = _motionUData[i];
      _motionAdvectVData[i] = _motionVData[i];

    } else {
      _motionUData[i] = _soundingUComp;
      _motionVData[i] = _soundingVComp;
      _motionAdvectUData[i] = _motionUData[i];
      _motionAdvectVData[i] = _motionVData[i];
    }

  } // i

  return true;

}


///////////////////////////
// _loadGridForVector()
//
// Load up u and v components for grid squares affected by a
// given vector.
//

void VectorsAdvector::_loadGridForVector(const double lat, const double lon,
                                         const double u, const double v,
                                         GridTemplate &grid_template) {
  int x_index, y_index;

  _motionProjection.latlon2xyIndex(lat, lon, x_index, y_index);

  // Loop through the template to calculate the weighted vector

  int nx = _motionProjection.getNx();
  int ny = _motionProjection.getNy();

  GridPoint *point;

  for (point = grid_template.getFirstInGrid(x_index, y_index,
                                            nx, ny);
       point != 0;
       point = grid_template.getNextInGrid()) {
    int motion_index = point->getIndex(nx, ny);

    // Calculate the distance from the template grid point to the
    // center grid point

    double x_dist = _motionProjection.xGrid2km((const double) (x_index - point->x));
    double y_dist = _motionProjection.yGrid2km((const double) (y_index - point->y));

    double distance = sqrt((x_dist * x_dist) + (y_dist * y_dist));

    // velocity components are summmed, weighted inversely with
    // distance from the point, dropping to 0 at smoothing radius

    double wt = 1.0 - (distance / _smoothingRadius);

    if ( wt < 0) {
      wt = 0;
    }

    _motionUData[motion_index] += u * wt;
    _motionVData[motion_index] += v * wt;
    _motionWtData[motion_index] += wt;
    if (wt > 0) {
      _motionWtCount[motion_index] += 1;
    }
  } /* endfor - point */

}


///////////////////////
// _calcSpeedDirection()
//
// Calculate the speed and direction based on the U and V vectors
//

void VectorsAdvector::_calcSpeedDirection(const double u, const double v,
                                          double &speed, double &direction) {
  speed = sqrt((u * u) + (v * v));

  if (u == 0.0 || v == 0.0)
    direction = 0.0;
  else
    direction = atan2(u, v) * RAD_TO_DEG;
}
