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
 * @file AerosolOpticalDepthProduct.cc
 *
 * @class AerosolOpticalDepthProduct
 *
 * AerosolOpticalDepthProduct is the top level application class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/AerosolOpticalDepthProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

AerosolOpticalDepthProduct::AerosolOpticalDepthProduct()
{
}

AerosolOpticalDepthProduct::AerosolOpticalDepthProduct(const AerosolOpticalDepthProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

AerosolOpticalDepthProduct::~AerosolOpticalDepthProduct()
{
}

bool AerosolOpticalDepthProduct::applyQcTests()
{
  return true;
}

void AerosolOpticalDepthProduct::clearAndReset()
{
  GoesRProduct::clearAndReset();
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void AerosolOpticalDepthProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printVars(prefix, strarg);   
}


void AerosolOpticalDepthProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printAttrs(prefix, strarg);   
}


void AerosolOpticalDepthProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printDims(prefix, strarg);   
}


void AerosolOpticalDepthProduct::_printQC(const std::string &prefix, const std::string &strarg)
{
  
}




/*********************************************************************
 * _readDimensions()
 */

void AerosolOpticalDepthProduct::_readDimensions()
{
  
}

/*********************************************************************
 * _readCoordinateVars()
 */

void AerosolOpticalDepthProduct::_readCoordinateVars()
{
  GoesRProduct::_readCoordinateVars();
}


/*********************************************************************
 * _readConstantsVars()
 */

void AerosolOpticalDepthProduct::_readConstantsVars()
{

}
  
/*********************************************************************
 * _readQualityControlVars()
 */

void AerosolOpticalDepthProduct::_readQualityControlVars()
{

}
  
/*********************************************************************
 * _readDataVars()
 */

void AerosolOpticalDepthProduct::_readDataVars()
{

}

/////////////////////////////////////////////////////////////////////////////////////
// string constant instantiation
//

const char* AerosolOpticalDepthProduct::AEROSOL_OPTICAL_DEPTH = "AOD";
const char* AerosolOpticalDepthProduct::AOD_PRODUCT_WAVELENGTH = "aod_product_wavelength";
const char* AerosolOpticalDepthProduct::LAND_SENSOR_BAND_WAVELENGTHS = "land_sensor_band_wavelengths";
const char* AerosolOpticalDepthProduct::SEA_SENSOR_BAND_WAVELENGTHS = "sea_sensor_band_wavelengths";
const char* AerosolOpticalDepthProduct::LAND_SENSOR_BAND_IDS = "land_sensor_band_ids";
const char* AerosolOpticalDepthProduct::SEA_SENSOR_BAND_IDS = "sea_sensor_band_ids";
const char* AerosolOpticalDepthProduct::SNOW_FREE_LAND_AND_ICE_FREE_SEA =
"snow_free_land_and_ice_free_sea";
const char* AerosolOpticalDepthProduct::GOES_LAT_LON_PROJECTION = "goes_lat_lon_projection";
const char* AerosolOpticalDepthProduct::AOD550_RETRIEVALS_ATTEMPTED_LAND =
"aod550_retrievals_attempted_land";
const char* AerosolOpticalDepthProduct::AOD550_RETRIEVALS_ATTEMPTED_SEA =
"aod550_retrievals_attempted_sea";
const char* AerosolOpticalDepthProduct::AOD550_GOOD_LZA_RETRIEVALS_ATTEMPTED =
"aod550_good_LZA_retrievals_attempted";
const char* AerosolOpticalDepthProduct::AOD550_OUTLIER_PIXEL_COUNT = "aod550_outlier_pixel_count";
const char* AerosolOpticalDepthProduct::MIN_AOD550_LAND = "min_aod550_land";
const char* AerosolOpticalDepthProduct::MAX_AOD550_LAND = "min_aod550_land";
const char* AerosolOpticalDepthProduct::MEAN_AOD550_LAND = "mean_aod550_land";
const char* AerosolOpticalDepthProduct::STD_DEV_AOD550_LAND = "std_dev_aod550_land";
const char* AerosolOpticalDepthProduct::MIN_AOD550_SEA = "min_aod550_sea";
const char* AerosolOpticalDepthProduct::MAX_AOD550_SEA = "max_aod550_sea";
const char* AerosolOpticalDepthProduct::MEAN_AOD550_SEA = "mean_aod550_sea";
const char* AerosolOpticalDepthProduct::STD_DEV_AOD550_SEA = "std_dev_aod550_sea";
const char* AerosolOpticalDepthProduct::SENSOR_BAND_MIN_AOD_LAND = "sensor_band_min_aod_land";
const char* AerosolOpticalDepthProduct::SENSOR_BAND_MAX_AOD_LAND = "sensor_band_max_aod_land";
const char* AerosolOpticalDepthProduct::SENSOR_BAND_MEAN_AOD_LAND = "sensor_band_mean_aod_land";
const char* AerosolOpticalDepthProduct::SENSOR_BAND_STD_DEV_AOD_LAND =
  "sensor_band_std_dev_aod_land";
const char* AerosolOpticalDepthProduct::SENSOR_BAND_MIN_AOD_SEA = "sensor_band_min_aod_sea";
const char* AerosolOpticalDepthProduct::SENSOR_BAND_MAX_AOD_SEA = "sensor_band_max_aod_sea";
const char* AerosolOpticalDepthProduct::SENSOR_BAND_MEAN_AOD_SEA = "sensor_band_mean_aod_sea";
const char* AerosolOpticalDepthProduct::SENSOR_BAND_STD_DEV_AOD_SEA =
  "sensor_band_std_dev_aod_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_AOD550_RETRIEVALS_ATTEMPTED_LAND =
  "lat_band_aod550_retrievals_attempted_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_AOD550_RETRIEVALS_ATTEMPTED_SEA =
  "lat_band_aod550_retrievals_attempted_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_AOD550_GOOD_LZA_RETRIEVALS_ATTEMPTED_LAND =
  "lat_band_aod550_good_LZA_retrievals_attempted_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_AOD550_GOOD_LZA_RETRIEVALS_ATTEMPTED_SEA =
  "lat_band_aod550_good_LZA_retrievals_attempted_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_AOD550_PERCENT_GOOD_RETRIEVALS_LAND =
  "lat_band_aod550_percent_good_retrievals_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_AOD550_PERCENT_BAD_RETRIEVALS_LAND =
  "lat_band_aod550_percent_bad_retrievals_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_AOD550_PERCENT_GOOD_RETRIEVALS_SEA =
  "lat_band_aod550_percent_good_retrievals_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_AOD550_PERCENT_BAD_RETRIEVALS_SEA =
  "lat_band_aod550_percent_bad_retrievals_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_MIN_AOD550_LAND = "lat_band_min_aod550_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_MAX_AOD550_LAND = "lat_band_max_aod550_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_MEAN_AOD550_LAND = "lat_band_mean_aod550_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_STD_DEV_AOD550_LAND =
  "lat_band_std_dev_aod550_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_MIN_AOD550_SEA = "lat_band_min_aod550_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_MAX_AOD550_SEA = "lat_band_max_aod550_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_MEAN_AOD550_SEA = "lat_band_mean_aod550_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_STD_DEV_AOD550_SEA =
  "lat_band_std_dev_aod550_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MIN_AOD_LAND =
  "lat_band_sensor_band_min_aod_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MAX_AOD_LAND =
  "lat_band_sensor_band_max_aod_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MEAN_AOD_LAND =
  "lat_band_sensor_band_mean_aod_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_STD_DEV_AOD_LAND =
  "lat_band_sensor_band_std_dev_aod_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MIN_AOD_SEA =
  "lat_band_sensor_band_min_aod_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MAX_AOD_SEA =
  "lat_band_sensor_band_max_aod_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MEAN_AOD_SEA =
  "lat_band_sensor_band_mean_aod_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_STD_DEV_AOD_SEA =
  "lat_band_sensor_band_std_dev_aod_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MIN_SURFACE_REFLECTIVITY_LAND =
  "lat_band_sensor_band_min_surface_reflectivity_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MAX_SURFACE_REFLECTIVITY_LAND =
  "lat_band_sensor_band_max_surface_reflectivity_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MEAN_SURFACE_REFLECTIVITY_LAND =
  "lat_band_sensor_band_mean_surface_reflectivity_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_STD_DEV_SURFACE_REFLECTIVITY_LAND =
  "lat_band_sensor_band_std_dev_surface_reflectivity_land";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MIN_SURFACE_REFLECTIVITY_SEA =
  "lat_band_sensor_band_min_surface_reflectivity_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MAX_SURFACE_REFLECTIVITY_SEA =
  "lat_band_sensor_band_max_surface_reflectivity_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_MEAN_SURFACE_REFLECTIVITY_SEA =
  "lat_band_sensor_band_mean_surface_reflectivity_sea";
const char* AerosolOpticalDepthProduct::LAT_BAND_SENSOR_BAND_STD_DEV_SURFACE_REFLECTIVITY_SEA =
  "lat_band_sensor_band_std_dev_surface_reflectivity_sea";
