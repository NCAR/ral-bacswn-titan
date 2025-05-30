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
 * @file FireCharacterizationProduct.hh
 *
 * @class FireCharacterizationProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef FIRE_CHARACTERIZATION_PRODUCT_HH
#define FIRE_CHARACTERIZATION_PRODUCT_HH

#include <string>

#include "GoesRProduct.hh"

/** 
 * @class FireCharacterizationProduct
 */

class FireCharacterizationProduct : public GoesRProduct
{

public:


  FireCharacterizationProduct();
  FireCharacterizationProduct(const FireCharacterizationProduct &from);
  ~FireCharacterizationProduct();
  
  void clearAndReset();
  bool applyQcTests();
  
  static const char* FIRE_AREA;
  static const char* FIRE_TEMP;
  static const char* FIRE_MASK;
  static const char* FIRE_POWER;
  
protected:
  


private:

  void _printVars(const std::string &prefix, const std::string &strarg = "");
  void _printAttrs(const std::string &prefix, const std::string &strarg = "");
  void _printDims(const std::string &prefix, const std::string &strarg = "");
  void _printQC(const std::string &prefix, const std::string &strarg = "");
  void _readDimensions();
  void _readCoordinateVars();
  void _readProjectionVars();
  inline void _readConstantsVars() {};  // do nothing with virtual method
  void _readQualityControlVars();
  void _readDataVars();

  // QC test variables
  bool _checkQualityField;
  QcCountTestParams _badPixel;
  int _badPixelCount;
  bool _passBadCountTest;
  
  static const char* TOTAL_NUMBER_OF_PIXELS_WITH_FIRES_DETECTED;
  static const char* TOTAL_NUMBER_OF_PIXELS_WITH_FIRE_TEMPERATURE_AND_AREA;
  static const char* TOTAL_NUMBER_OF_PIXELS_WITH_FIRE_RADIATIVE_POWER;
  static const char* FIRE_TEMPERATURE_OUTLIER_PIXEL_COUNT;
  static const char* FIRE_AREA_OUTLIER_PIXEL_COUNT;
  static const char* FIRE_RADIATIVE_POWER_OUTLIER_PIXEL_COUNT;
  static const char* MIN_FIRE_TEMPERATURE;
  static const char* MAX_FIRE_TEMPERATURE;
  static const char* MEAN_FIRE_TEMPERATURE;
  static const char* STD_DEV_FIRE_TEMPERATURE;
  static const char* MIN_FIRE_AREA;
  static const char* MAX_FIRE_AREA;
  static const char* MEAN_FIRE_AREA;
  static const char* STD_DEV_FIRE_AREA;
  static const char* MIN_FIRE_RADIATIVE_POWER;
  static const char* MAX_FIRE_RADIATIVE_POWER;
  static const char* MEAN_FIRE_RADIATIVE_POWER;
  static const char* STD_DEV_FIRE_RADIATIVE_POWER;

};




#endif // FIRE_CHARACTERIZATION_PRODUCT_HH
