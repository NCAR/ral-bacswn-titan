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
 *
 * @file AerosolOpticalDepthProduct.hh
 *
 * @class AerosolOpticalDepthProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef AEROSOL_OPTICAL_DEPTH_PRODUCT_HH
#define AEROSOL_OPTICAL_DEPTH_PRODUCT_HH

#include <string>

#include "GoesRProduct.hh"

/** 
 * @class AerosolOpticalDepthProduct
 */

class AerosolOpticalDepthProduct : public GoesRProduct
{

public:


  AerosolOpticalDepthProduct();
  AerosolOpticalDepthProduct(const AerosolOpticalDepthProduct &from);
  ~AerosolOpticalDepthProduct();
  
  void clearAndReset();
  bool applyQcTests();
  
  static const char* AEROSOL_OPTICAL_DEPTH;
  
protected:
  


private:

  void _printVars(const std::string &prefix, const std::string &strarg = "");
  void _printAttrs(const std::string &prefix, const std::string &strarg = "");
  void _printDims(const std::string &prefix, const std::string &strarg = "");
  void _printQC(const std::string &prefix, const std::string &strarg = "");
  void _readDimensions();
  void _readCoordinateVars();
  void _readConstantsVars();
  void _readQualityControlVars();
  void _readDataVars();
  
  static const char* AOD_PRODUCT_WAVELENGTH;
  static const char* LAND_SENSOR_BAND_WAVELENGTHS;
  static const char* SEA_SENSOR_BAND_WAVELENGTHS;
  static const char* LAND_SENSOR_BAND_IDS;
  static const char* SEA_SENSOR_BAND_IDS;
  static const char* SNOW_FREE_LAND_AND_ICE_FREE_SEA;
  static const char* GOES_LAT_LON_PROJECTION;
  static const char* AOD550_RETRIEVALS_ATTEMPTED_LAND;
  static const char* AOD550_RETRIEVALS_ATTEMPTED_SEA;
  static const char* AOD550_GOOD_LZA_RETRIEVALS_ATTEMPTED;
  static const char* AOD550_OUTLIER_PIXEL_COUNT;
  static const char* MIN_AOD550_LAND;
  static const char* MAX_AOD550_LAND;
  static const char* MEAN_AOD550_LAND;
  static const char* STD_DEV_AOD550_LAND;
  static const char* MIN_AOD550_SEA;
  static const char* MAX_AOD550_SEA;
  static const char* MEAN_AOD550_SEA;
  static const char* STD_DEV_AOD550_SEA;
  static const char* SENSOR_BAND_MIN_AOD_LAND;
  static const char* SENSOR_BAND_MAX_AOD_LAND;
  static const char* SENSOR_BAND_MEAN_AOD_LAND;
  static const char* SENSOR_BAND_STD_DEV_AOD_LAND;
  static const char* SENSOR_BAND_MIN_AOD_SEA;
  static const char* SENSOR_BAND_MAX_AOD_SEA;
  static const char* SENSOR_BAND_MEAN_AOD_SEA;
  static const char* SENSOR_BAND_STD_DEV_AOD_SEA;
  static const char* LAT_BAND_AOD550_RETRIEVALS_ATTEMPTED_LAND;
  static const char* LAT_BAND_AOD550_RETRIEVALS_ATTEMPTED_SEA;
  static const char* LAT_BAND_AOD550_GOOD_LZA_RETRIEVALS_ATTEMPTED_LAND;
  static const char* LAT_BAND_AOD550_GOOD_LZA_RETRIEVALS_ATTEMPTED_SEA;
  static const char* LAT_BAND_AOD550_PERCENT_GOOD_RETRIEVALS_LAND;
  static const char* LAT_BAND_AOD550_PERCENT_BAD_RETRIEVALS_LAND;
  static const char* LAT_BAND_AOD550_PERCENT_GOOD_RETRIEVALS_SEA;
  static const char* LAT_BAND_AOD550_PERCENT_BAD_RETRIEVALS_SEA;
  static const char* LAT_BAND_MIN_AOD550_LAND;
  static const char* LAT_BAND_MAX_AOD550_LAND;
  static const char* LAT_BAND_MEAN_AOD550_LAND;
  static const char* LAT_BAND_STD_DEV_AOD550_LAND;
  static const char* LAT_BAND_MIN_AOD550_SEA;
  static const char* LAT_BAND_MAX_AOD550_SEA;
  static const char* LAT_BAND_MEAN_AOD550_SEA;
  static const char* LAT_BAND_STD_DEV_AOD550_SEA;
  static const char* LAT_BAND_SENSOR_BAND_MIN_AOD_LAND;
  static const char* LAT_BAND_SENSOR_BAND_MAX_AOD_LAND;
  static const char* LAT_BAND_SENSOR_BAND_MEAN_AOD_LAND;
  static const char* LAT_BAND_SENSOR_BAND_STD_DEV_AOD_LAND;
  static const char* LAT_BAND_SENSOR_BAND_MIN_AOD_SEA;
  static const char* LAT_BAND_SENSOR_BAND_MAX_AOD_SEA;
  static const char* LAT_BAND_SENSOR_BAND_MEAN_AOD_SEA;
  static const char* LAT_BAND_SENSOR_BAND_STD_DEV_AOD_SEA;
  static const char* LAT_BAND_SENSOR_BAND_MIN_SURFACE_REFLECTIVITY_LAND;
  static const char* LAT_BAND_SENSOR_BAND_MAX_SURFACE_REFLECTIVITY_LAND;
  static const char* LAT_BAND_SENSOR_BAND_MEAN_SURFACE_REFLECTIVITY_LAND;
  static const char* LAT_BAND_SENSOR_BAND_STD_DEV_SURFACE_REFLECTIVITY_LAND;
  static const char* LAT_BAND_SENSOR_BAND_MIN_SURFACE_REFLECTIVITY_SEA;
  static const char* LAT_BAND_SENSOR_BAND_MAX_SURFACE_REFLECTIVITY_SEA;
  static const char* LAT_BAND_SENSOR_BAND_MEAN_SURFACE_REFLECTIVITY_SEA;
  static const char* LAT_BAND_SENSOR_BAND_STD_DEV_SURFACE_REFLECTIVITY_SEA;

};




#endif // AEROSOL_OPTICAL_DEPTH_PRODUCT_HH
