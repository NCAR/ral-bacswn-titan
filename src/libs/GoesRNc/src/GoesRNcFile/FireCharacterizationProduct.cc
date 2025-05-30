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
 * @file FireCharacterizationProduct.cc
 *
 * @class FireCharacterizationProduct
 *
 * FireCharacterizationProduct is a derived class for the fire 
 * characterization Level-2 GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/FireCharacterizationProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

FireCharacterizationProduct::FireCharacterizationProduct()
{
  _badPixelCount = 0;
  _passBadCountTest = true;
}

FireCharacterizationProduct::FireCharacterizationProduct(const FireCharacterizationProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

FireCharacterizationProduct::~FireCharacterizationProduct()
{
}

bool FireCharacterizationProduct::applyQcTests()
{
 static const string methodName = _className + "::" + __FUNCTION__;

  bool testRecord = true;
  
  for(size_t i = 0; i < _numX*_numY; i++) {

    if(_qcDataContainer.values[i] == DATA_QUALITY_GOOD) { 
      continue;
    }

    if(_checkQualityField == true) {
      for(auto it = _outDataFields.begin(); it != _outDataFields.end(); it++) {
	it->second->values[i] = BAD_DATA_VALUE;
      }
      _badPixelCount++;
    }
  }

  
  if ((_badPixel.run == true) &&
      (_badPixelCount > _badPixel.threshold)) {
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << ERROR_STR << methodName << endl;
      cerr << "  Total number of bad point replacements exceeded threshold." << endl;
    }
    _passBadCountTest = false;
    testRecord = false;    
  }

  if( (testRecord == false) && (_debugLevel == GoesRProduct::DEBUG_OFF)) {
    cerr << ERROR_STR << methodName << endl;
    cerr << "  Failed one or more QC tests." << endl;
  }
  
  return testRecord;
}

void FireCharacterizationProduct::clearAndReset()
{
  GoesRProduct::clearAndReset();
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void FireCharacterizationProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printVars(prefix, strarg);  
}


void FireCharacterizationProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printAttrs(prefix, strarg);
}


void FireCharacterizationProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printDims(prefix, strarg);   
}


void FireCharacterizationProduct::_printQC(const std::string &prefix, const std::string &strarg)
{
  
}

void FireCharacterizationProduct::_readDimensions()
{
 
}
 
void FireCharacterizationProduct::_readCoordinateVars()
{
  GoesRProduct::_readCoordinateVars();
}

  
void FireCharacterizationProduct::_readQualityControlVars()
{
  
}
  
void FireCharacterizationProduct::_readDataVars()
{
 
}

/////////////////////////////////////////////////////////////////////////////////////
// string constant instantiation
//
const char* FireCharacterizationProduct::FIRE_AREA = "Area";
const char* FireCharacterizationProduct::FIRE_TEMP = "Temp";
const char* FireCharacterizationProduct::FIRE_MASK = "Mask";
const char* FireCharacterizationProduct::FIRE_POWER = "Power";
const char* FireCharacterizationProduct::TOTAL_NUMBER_OF_PIXELS_WITH_FIRES_DETECTED =
  "total_number_of_pixels_with_fires_detected";
const char* FireCharacterizationProduct::TOTAL_NUMBER_OF_PIXELS_WITH_FIRE_TEMPERATURE_AND_AREA =
  "total_number_of_pixels_with_fire_temperature_and_area";
const char* FireCharacterizationProduct::TOTAL_NUMBER_OF_PIXELS_WITH_FIRE_RADIATIVE_POWER =
  "total_number_of_pixels_with_fire_radiative_power";
const char* FireCharacterizationProduct::FIRE_TEMPERATURE_OUTLIER_PIXEL_COUNT =
  "fire_temperature_outlier_pixel_count";
const char* FireCharacterizationProduct::FIRE_AREA_OUTLIER_PIXEL_COUNT =
  "fire_area_outlier_pixel_count";
const char* FireCharacterizationProduct::FIRE_RADIATIVE_POWER_OUTLIER_PIXEL_COUNT =
  "fire_radiative_power_outlier_pixel_count";
const char* FireCharacterizationProduct::MIN_FIRE_TEMPERATURE = "minimum_fire_temperature";
const char* FireCharacterizationProduct::MAX_FIRE_TEMPERATURE = "maximum_fire_temperature";
const char* FireCharacterizationProduct::MEAN_FIRE_TEMPERATURE = "mean_fire_temperature";
const char* FireCharacterizationProduct::STD_DEV_FIRE_TEMPERATURE =
  "standard_deviation_fire_temperature";
const char* FireCharacterizationProduct::MIN_FIRE_AREA = "minimum_fire_area";
const char* FireCharacterizationProduct::MAX_FIRE_AREA = "maximum_fire_area";
const char* FireCharacterizationProduct::MEAN_FIRE_AREA = "mean_fire_area";
const char* FireCharacterizationProduct::STD_DEV_FIRE_AREA = "standard_deviation_fire_area";
const char* FireCharacterizationProduct::MIN_FIRE_RADIATIVE_POWER = "minimum_fire_radiative_power";
const char* FireCharacterizationProduct::MAX_FIRE_RADIATIVE_POWER = "maximum_fire_radiative_power";
const char* FireCharacterizationProduct::MEAN_FIRE_RADIATIVE_POWER = "mean_fire_radiative_power";
const char* FireCharacterizationProduct::STD_DEV_FIRE_RADIATIVE_POWER =
  "standard_deviation_fire_radiative_power";
