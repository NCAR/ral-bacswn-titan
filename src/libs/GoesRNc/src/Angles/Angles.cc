/////////////////////////////////////////////////////////////////////////
//
// Class:	Angles
//
// Author:	G M Cunning
//
// Date:	Tue Jan  9 15:34:07 2001
//
// Description: this class is responsible for calculating the satellite 
//		and solar zenith angles and the relative azimuth angle 
//		between the satellite and sun.
//
//
// Note: This is a refactoring of the SatAngles class in satDerive
//
//
//

// C++ include files
#include <cmath>
#include <cassert>
#include <iostream>

// System/RAP include files
#include <toolsa/toolsa_macros.h>
#include <toolsa/DateTime.hh>

// Local include files
#include <GoesRNc/Angles.hh>

using namespace std;


// define any constants
const string Angles::_className    = "Angles";
const double Angles::Earth2SunDist = 1.4959965e8;   // kilometers 
const double Angles::Geostat2EarthDist = 42164.0;   // kilometers
const double Angles::GeostatAltitude = 35786.0;     // kilometers
const double Angles::EquatorEarthRadius = 6378.145; // kilometers

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
// Constructors
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

Angles::Angles() {
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
// Destructors
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
  
Angles::~Angles()
{
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
// Method Name:	Angles::init
//
// Description:	this method initializes the object. Primary task is 
//		to calculate the sun's subsatellite point in terms of 
//		latitude and longitude given the time.
//
// Returns:	true if successful and false otherwise
//
// Notes:	this method is taken from a program written by R. Bullock.
//		The units of the angles are degrees. The reference system
//		for the grid is longitude is positive westward.
//

void
Angles::init(double sat_lon, double sat_alt, int units)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  if (static_cast<output_units_t>(units) == OUTPUT_DEGREES) {
    _outUnits = OUTPUT_DEGREES;
  }
  else {
    _outUnits = OUTPUT_RADIANS;
  }
  
  _satelliteLatitude = 0.0;
  _satelliteLongitude = -sat_lon*DEG_TO_RAD;
  _satelliteDistance = sat_alt + EquatorEarthRadius;
  _earthRadiusAtLat = _calcEarthRadius(_satelliteLatitude);
  _calcGeocentricP(_satelliteDistance, _satelliteLatitude, _satelliteLongitude, _satelliteGeoPos);

}

void
Angles::setLocation(double sat_lon, double sat_alt)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  _satelliteLatitude = 0.0;
  _satelliteLongitude = -sat_lon*DEG_TO_RAD;
  _satelliteDistance = sat_alt + EquatorEarthRadius;
  _earthRadiusAtLat = _calcEarthRadius(_satelliteLatitude);
  _calcGeocentricP(_satelliteDistance, _satelliteLatitude, _satelliteLongitude, _satelliteGeoPos);

  if (_debug == true) {
    cout.setf(ios::fixed);
    cout.precision(5);
    cout << methodName << endl;
    cout << "satellie lat, lon, dist: " << _satelliteLatitude*RAD_TO_DEG << "   " << _satelliteLongitude*RAD_TO_DEG << "   " << _satelliteDistance << endl;
    cout << "_satelliteGeoPos: " << _satelliteGeoPos.x << "   " << _satelliteGeoPos.y << "   " << _satelliteGeoPos.z << endl;
  }
}

void
Angles::setOutputUnits(int units)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  if (static_cast<output_units_t>(units) == OUTPUT_DEGREES) {
    _outUnits = OUTPUT_DEGREES;
  }
  else {
    _outUnits = OUTPUT_RADIANS;
  }
  
}

void
Angles::setSolarTime(time_t t)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  _theTime = t;
  _calcSolarPosition(_theTime);
  _calcGeocentricP(_solarDistance, _solarLatitude, _solarLongitude, _solarGeoPos); 

  if (_debug == true) {
    cout.setf(ios::fixed);
    cout.precision(5);
    cout << methodName << endl;
    cout <<  "time : " << DateTime::strm(_theTime) << endl;
    cout << "solar lat, lon, dist: " << _solarLatitude*RAD_TO_DEG << "   " << _solarLongitude*RAD_TO_DEG << "   " << _solarDistance << endl;
    cout << "_solarGeoPos: " << _solarGeoPos.x << "   " << _solarGeoPos.y << "   " << _solarGeoPos.z << endl;
  }
    
  
}

void 
Angles::preCompute(double lat, double lon)
{ 
  static const string methodName = _className + "::" + __FUNCTION__;

  _latitude = lat*DEG_TO_RAD;
  _longitude = -lon*DEG_TO_RAD;

  _earthRadiusAtLat = _calcEarthRadius(_latitude);
  
  _calcGeocentricP(_earthRadiusAtLat, _latitude, _longitude, _gridGeoPos);

  _calcTangentPlaneE(_longitude, _tangentPlaneE);
  _calcTangentPlaneN(_latitude, _longitude, _tangentPlaneN);

  _calcGeocentricQ(_gridGeoPos, _satelliteGeoPos, _satelliteGeoQVec);
  _calcGeocentricQ(_gridGeoPos, _solarGeoPos, _solarGeoQVec);

//   if (_debug == true) {
//     cout.setf(ios::fixed);
//     cout.precision(5);
//     cout << methodName << endl;
//     cout << "grid lat, lon, dist : " << _latitude*RAD_TO_DEG << "   " << _longitude*RAD_TO_DEG << "   " << _earthRadiusAtLat << endl;
//     cout << "_gridGeoPos: " << _gridGeoPos.x << "   " << _gridGeoPos.y << "   " << _gridGeoPos.z << endl;
//     cout << "_tangentPlaneE: " << _tangentPlaneE.x << "   " << _tangentPlaneE.y << "   " << _tangentPlaneE.z << endl;
//     cout << "_tangentPlaneN: " << _tangentPlaneN.x << "   " << _tangentPlaneN.y << "   " << _tangentPlaneN.z << endl;
//     cout << "_satelliteGeoQVec: " << _satelliteGeoQVec.x << "   " << _satelliteGeoQVec.y << "   " << _satelliteGeoQVec.z << endl;
//     cout << "_solarGeoQVec: " << _solarGeoQVec.x << "   " << _solarGeoQVec.y << "   " << _solarGeoQVec.z << endl;
//   }

}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
// Private Methods
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////
//
// Method Name:	Angles::_calcSolarPosition
//
// Description:	this method determines the sun's subsatellite point on 
//		the earth given the time.
//
// Returns:	
//
// Globals:	
//
// Notes:
//
//

void
Angles::_calcSolarPosition(const time_t data_time)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double rightAscension;
  double declination;

  _calcSolarRadec(data_time, rightAscension, declination);

  _solarLongitude = ((_gmt2gst(data_time) - rightAscension) + 360.0);
  _solarLongitude -= 360.0*floor(_solarLongitude/360.0);
  _solarLatitude = declination;


  // there may be a bug in solar distance. the possible correct line commented out below
  //_solarDistance = Earth2SunDist + EquatorEarthRadius;
  _solarDistance = Earth2SunDist + _calcEarthRadius(_solarLatitude);

  // convert to radians
  _solarLongitude *= DEG_TO_RAD;
  _solarLatitude *= DEG_TO_RAD;
  

  return;
}

double
Angles::calcSolarAzimuth()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double azimuth = _calcAzimuth(_solarGeoQVec, _tangentPlaneE, _tangentPlaneN);

  if (_outUnits == OUTPUT_DEGREES) {
    azimuth *= RAD_TO_DEG;
  }

  return azimuth;
}

double
Angles::calcSolarZenith()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double zenith = _calcZenith(_solarGeoQVec, _gridGeoPos);

  if (_outUnits == OUTPUT_DEGREES) {
    zenith *= RAD_TO_DEG;
  }

  return zenith;
}

double
Angles::calcSatelliteAzimuth()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double azimuth = _calcAzimuth(_satelliteGeoQVec, _tangentPlaneE, _tangentPlaneN);

  if (_outUnits == OUTPUT_DEGREES) {
    azimuth *= RAD_TO_DEG;
  }

  return azimuth;
}

double
Angles::calcSatelliteZenith()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double zenith = _calcZenith(_satelliteGeoQVec, _gridGeoPos);

  if (_outUnits == OUTPUT_DEGREES) {
    zenith *= RAD_TO_DEG;
  }

  return zenith;
}

double
Angles::calcRelativeAzimuth()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double satAzimuth = _calcAzimuth(_satelliteGeoQVec, _tangentPlaneE, _tangentPlaneN);
  double solarAzimuth = _calcAzimuth(_solarGeoQVec, _tangentPlaneE, _tangentPlaneN);

  double relAzimuth = fabs(satAzimuth - solarAzimuth);

  if (relAzimuth > M_PI) {
    relAzimuth = TWO_PI - relAzimuth;
  }

  if (_outUnits == OUTPUT_DEGREES) {
    relAzimuth *= RAD_TO_DEG;
  }

  return relAzimuth;
  
}

/////////////////////////////////////////////////////////////////////////
//
// Method Name:	Angles::_calcGeocentricP
//
// Description:	this method calculates the (x,y,z) components of a 
//		vector in a geocentric coordinate system
//
// Returns:	
//
// Globals:	
//
// Notes:	This method assumes a unit magnitude for the vector. 
//		The units of the angles are degrees.
//
//

void 
Angles::_calcGeocentricP(double rng, double lat, double lon, vector_t& p)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  p.x = (rng/_earthRadiusAtLat)*cos(lat)*sin(lon);
  p.y = (rng/_earthRadiusAtLat)*cos(lat)*cos(lon);
  p.z = (rng/_earthRadiusAtLat)*sin(lat);
}

/////////////////////////////////////////////////////////////////////////
//
// Method Name:	Angles::_calcGeocentricQ
//
// Description:	this method calculates the normalized vector that 
//		represents the baering between two point in a geocentric 
//		coordinate system.
//
// Returns:	
//
// Globals:	
//
// Notes:	This method assumes a unit magnitude for the vectors.
//
//

void 
Angles::_calcGeocentricQ(const vector_t& p_i,
			 const vector_t& p_s, 
			 vector_t& q)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double x = p_s.x - p_i.x;
  double y = p_s.y - p_i.y;
  double z = p_s.z - p_i.z;

  double mag = sqrt(x*x + y*y + z*z);

  q.x = x/mag;
  q.y = y/mag;
  q.z = z/mag;

  return;
}

/////////////////////////////////////////////////////////////////////////
//
// Method Name:	Angles::_calcTangentPlaneE
//
// Description:	This method the e-w basis vector for a tangent plane 
//		at a point in a geocentric coordinate system.
//
// Returns:	
//
// Globals:	
//
// Notes:	The units of the angles are degrees.
//
//

void 
Angles::_calcTangentPlaneE(double lon, vector_t& e)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  e.x = -cos(lon);
  e.y = sin(lon);
  e.z = 0.0;
}

/////////////////////////////////////////////////////////////////////////
//
// Method Name:	Angles::_calcTangentPlaneN
//
// Description:	This method the n-s basis vector for a tangent plane 
//		at a point in a geocentric coordinate system.
//
// Returns:	
//
// Globals:	
//
// Notes:	The units of the angles are degrees.
//
//

void 
Angles::_calcTangentPlaneN(double lat, double lon, 
			      vector_t& n)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  n.x = -sin(lat)*sin(lon);
  n.y = -sin(lat)*cos(lon);
  n.z = cos(lat);
}


/////////////////////////////////////////////////////////////////////////
//
// Method Name:	Angles::_calcZenith
//
// Description:	this method calculates the zenith angle.
//
// Returns:	
//
// Globals:	
//
// Notes:
//
//

double 
Angles::_calcZenith(const vector_t& q, const vector_t& p)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double dotProduct = p.x*q.x + p.y*q.y + p.z*q.z;
  double zenith = M_PI/2.0 - asin(dotProduct);

//   if (_debug == true) {
//     cout.setf(ios::fixed);
//     cout.precision(5);
//     cout << methodName << endl;
//     cout << "zenith: " << zenith*RAD_TO_DEG << endl;
//   }

  return zenith;
}


/////////////////////////////////////////////////////////////////////////
//
// Method Name:Angles::_calcAzimuth
//
// Description:	this method calculates the azimuth angle.
//
// Returns:	
//
// Globals:	
//
// Notes:
//
//

double 
Angles::_calcAzimuth(const vector_t& q, const vector_t& e,  const vector_t& n)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double num = e.x*q.x + e.y*q.y + e.z*q.z;
  double den = n.x*q.x + n.y*q.y + n.z*q.z;
  double azimuth = atan2(den,num);

  // if (_debug == true) {
  //   cout.setf(ios::fixed);
  //   cout.precision(5);
  //   cout << methodName << endl;
  //   cout << "azimuth: " << azimuth*RAD_TO_DEG << endl;
  // }


  
  return azimuth;
}


/************************************************************************

Function Name: 	gmt2gst

Description:	this function take the time in GMT and converts it to 
		Greenwich Sidereal Time (GST)

Returns:	returns the GST as a double

Notes:		Function originally written by R. Bullock

************************************************************************/

double Angles::_gmt2gst(time_t gmt)
{
  static const string methodName = _className + "::" + __FUNCTION__;


  int date = gmt/86400;
  int time = gmt%86400;
  const double eta = 1.00273790935;

  double t = (date - 10957.5)/36525.0;

  double ans  = 100.46061837 + 36000.770053608*t + 0.000387933*t*t - t*t*t/38710000.0;

  ans /= 360.0;

  ans -= floor(ans);

  ans += eta*(time/86400.0);

  ans -= floor(ans);

  ans *= 360.0;

  return ans;

}

/************************************************************************

Function Name: 	calcSolarRadec

Description:	this functions calculates the solar right ascension and
		declination.

Returns:	none

Notes:		This function originally written by R. Bullock.

************************************************************************/

void 
Angles::_calcSolarRadec(time_t gmt, double& ra, double& dec)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  const double tol = 1.0e-6;

  double t = ((gmt/86400.0) - 10957.5)/36525.0;

  double lzero = 280.46645 + 36000.76983*t + 0.0003032*t*t;
  lzero -= 360.0*floor(lzero/360.0);

  double m = 357.52910 + 35999.05030*t - 0.0001559*t*t - 0.00000048*t*t*t;
  m -= 360.0*floor(m/360.0);
  double mrad = m*DEG_TO_RAD;

  double eps = 23.439291111111 - (46.815*t + 0.00059*t*t - 0.001813*t*t*t)/3600.0;

  //double ecc = 0.016708617 - 0.000042037*t - 0.0000001236*t*t;

  double c = (1.9146 - 0.004817*t - 0.000014*t*t)*sin(mrad)
    + (0.019933 - 0.000101*t)*sin(2.0*mrad)+ 0.00029*sin(3*mrad);

  double theta = lzero + c;

  double omega = 125.04 - 1934.136*t;
  omega -= 360.0*floor(omega/360.0);
  double omegarad = omega*DEG_TO_RAD;

  double lambda = theta - 0.00569 - 0.00478*sin(omegarad);
  double lambdarad = lambda*DEG_TO_RAD;

  eps += 0.00256*cos(omegarad);
  double epsrad = eps*DEG_TO_RAD;

  double x = cos(lambda*DEG_TO_RAD);
  double y = cos(epsrad)*sin(lambdarad);
  double z = sin(epsrad)*sin(lambdarad);

  double d = fabs(x) + fabs(y);

  dec = RAD_TO_DEG*asin(z);
  ra = ( (d < tol) ? 0.0 : (RAD_TO_DEG*atan2(y, x)) );
  ra -= 360.0*floor(ra/360.0);

  return;

}

/************************************************************************

Function Name: 	_calcEarthRadius

Description:	this function calculates the earth's radius as a 
		function of geographic latitude. This function determines
		the International Ellipsoid. 

Returns:	radius of earth in kilometers

Notes:		taken from "Orbital Motion" 3rd ed. by A. E. Roy, pg 305

************************************************************************/

double 
Angles::_calcEarthRadius(double lat)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  double sinLat = sin(lat);
  double sin2Lat = sin(2.0*lat);
  double radius = EquatorEarthRadius - 0.0033670*sinLat*sinLat + 7.1e-6*sin2Lat*sin2Lat;
  return radius;
}
