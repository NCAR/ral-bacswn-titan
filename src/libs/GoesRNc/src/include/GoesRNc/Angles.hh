/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 ** Copyright (c) 1992-2000 UCAR
 ** University Corporation for Atmospheric Research(UCAR)
 ** National Center for Atmospheric Research(NCAR)
 ** Research Applications Program(RAP)
 ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
 ** All rights reserved. Licenced use only.
 ** $Date: 2020/03/03 23:20:21 $
 *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

//////////////////////////////////////////////////////////////////////////
// 
// Header: Angles
// 
// Author: G M Cunning
// 
// Date:	Fri Jan  5 16:34:05 2001
// 
// Description:	This class is responsible for calculating the satellite and
//		solar zenith angles and relative azimuth for points in a 
//		specified grid.
// 
// Note: This is a refactoring of the SatAngles class in satDerive
// 
// 


# ifndef    ANGLES_H
# define    ANGLES_H

// C++ include files
#include <string>
#include <vector>

// System/RAP include files
#include <toolsa/os_config.h>
#include <dataport/port_types.h>



class Angles {
  
public:

  typedef enum {
    OUTPUT_DEGREES = 0,
    OUTPUT_RADIANS = 1
  } output_units_t;

  ////////////////////
  // public members //
  ////////////////////

  // mean distance from the center of the sun to the earth
  static const double Earth2SunDist; // 1.4959965e8 kilometers 

  // mean distance from a geostationary satellite to the center of the earth
  static const double Geostat2EarthDist; // 42164.0 kilometers

  static const double GeostatAltitude; // 35786.0 kilometers

  static const double EquatorEarthRadius; // 6378.145 kilometers

  ////////////////////
  // public methods //
  ////////////////////

  // constructor
  Angles();

  // destructor
  virtual ~Angles();

  // initialize -- setup the object
  void init(double sat_lon, double sat_alt, int units);
  void setOutputUnits(int units);
  void setLocation(double sat_lon, double sat_alt);
  void setSolarTime(time_t time);
  inline void setDebug(bool debug = true) { _debug = debug; }
  
  void preCompute(double lat, double lon);
  double calcSolarAzimuth();
  double calcSolarZenith();
  double calcSatelliteAzimuth();
  double calcSatelliteZenith();
  double calcRelativeAzimuth();

  
protected:

  ///////////////////////
  // protected members //
  ///////////////////////
  

  ///////////////////////
  // protected methods //
  ///////////////////////

private:

  ///////////////////
  // private types //
  ///////////////////

  typedef struct {
    double x;
    double y;
    double z;
  } vector_t;


  /////////////////////
  // private members //
  /////////////////////
  static const std::string _className;

  output_units_t _outUnits;

  bool _debug;

  time_t _theTime;
  double _satelliteLatitude;
  double _satelliteLongitude;
  double _satelliteDistance;
  double _solarLatitude;
  double _solarLongitude;
  double _solarDistance;
  double _earthRadiusAtLat;
  
  double _latitude;
  double _longitude;
  vector_t _gridGeoPos;
  vector_t _satelliteGeoPos;
  vector_t _solarGeoPos;
  vector_t _satelliteGeoQVec;
  vector_t _solarGeoQVec;
  vector_t _tangentPlaneE;
  vector_t _tangentPlaneN;
  double _satZenith;
  double _satAzimuth;
  
  /////////////////////
  // private methods //
  /////////////////////

  // the units of all the angles are radians. Angles can be converted to degrees for the calling class

  void _calcSolarPosition(std::time_t data_time);
  void _calcGeocentricP(double rng, double lat, double lon, 
			vector_t& p);
  void _calcGeocentricQ(const vector_t& p_i, const vector_t& p_s, 
			vector_t& q);
  void _calcTangentPlaneE(double lon, vector_t& e);
  void _calcTangentPlaneN(double lat, double lon, vector_t& n);
  double _calcZenith(const vector_t& q, const vector_t& p);
  double _calcAzimuth(const vector_t& q, const vector_t& e, 
		      const vector_t& n);
  double _gmt2gst(time_t gmt);
  void _calcSolarRadec(time_t gmt, double& ra, double& dec);
  double _calcEarthRadius(double lat);

  double _calcAzimuth();
  double _calcZenith();
 
};



# endif     /* ANGLES_H */
