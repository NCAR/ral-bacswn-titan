/////////////////////////////////////////////////////////////////////////
//
// Class:	Normalize
//
// Author:	G M Cunning
//
// Date:	april 12, 2021
//
// Description: This class is responsible for calculating a normalized albedo.
//              The calculation is separated out into a class, so it can be shared
//              with the GoesRnetCDF2MDv and SatDerive
//

// C++ include files
#include <cmath>
#include <iostream>

// System/RAP include files
#include <toolsa/toolsa_macros.h>

// Local include files
#include <GoesRNc/Normalize.hh>

using namespace std;

// define any constants
const string Normalize::_className    = "Normalize";

Normalize::Normalize() :
  _method{COS_SOL_ZEN},
  _minCosSza{0.0},
  _maxAlbedo{0.0},
  _debug{false}
{

}


Normalize::Normalize(int method, double max_sza, double max_albedo) {
  this->init(method, max_sza, max_albedo);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
// Public Methods
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
//
// Method Name:	Normalize::init
//
// Description:	this method initializes the object.
//

void
Normalize::init(int method, double max_sza, double max_albedo)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  _maxAlbedo = max_albedo;
  _minCosSza = cos(max_sza*DEG_TO_RAD);

  if (static_cast<norm_albedo_method_t>(method) == COS_SOL_ZEN) {
    _method = COS_SOL_ZEN;
  }
  else {
    _method = SQRT_COS_SOL_ZEN;
  }

}

/////////////////////////////////////////////////////////////////////////
//
// Method Name:	Normalize::init
//
// Description:	this method initializes the object.
//

double
Normalize::compute(double albedo, double sza)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double cosSolZen = cos(sza*DEG_TO_RAD);
  double factor = max(cosSolZen, _minCosSza);
  double normAlbedo = albedo/factor;

  if (_method == Normalize::SQRT_COS_SOL_ZEN) {
    normAlbedo = sqrt(normAlbedo);
  }

  normAlbedo = min(max(normAlbedo, 0.0), _maxAlbedo);

  return normAlbedo;
}
