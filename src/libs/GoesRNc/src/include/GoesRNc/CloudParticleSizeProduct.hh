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
 * @file CloudParticleSizeProduct.hh
 *
 * @class CloudParticleSizeProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef CLOUD_PARTICLE_SIZE_PRODUCT_HH
#define CLOUD_PARTICLE_SIZE_PRODUCT_HH

#include <string>

#include "GoesRProduct.hh"

/** 
 * @class CloudParticleSizeProduct
 */

class CloudParticleSizeProduct : public GoesRProduct
{

public:


  CloudParticleSizeProduct();
  CloudParticleSizeProduct(const CloudParticleSizeProduct &from);
  ~CloudParticleSizeProduct();
  
  void clearAndReset();
  bool applyQcTests();
  
  inline void setCheckQualityField(bool run) { _checkQualityField = run; }
  inline void setBadCountTest(bool run, int threshold)
  { _badPixel.run = run; _badPixel.threshold = threshold; }
  
  static const char* CLOUD_PARTICLE_SIZE;

protected:
  


private:
  
  void _printVars(const std::string &prefix, const std::string &strarg = "");
  void _printAttrs(const std::string &prefix, const std::string &strarg = "");
  void _printDims(const std::string &prefix, const std::string &strarg = "");
  void _printQC(const std::string &prefix, const std::string &strarg = "");
  void _readDimensions();
  void _readCoordinateVars();
  void _readQualityControlVars();
  void _readDataVars();
  inline void _readConstantsVars() {}; // do nothing with vitual method

  float _minimuimPSDDay;
  float _maximuimPSDDay;
  float _meanPSDDay;
  float _stdDevPSDDay;
  float _minimuimPSDNight;
  float _maximuimPSDNight;
  float _meanPSDNight;
  float _stdDevPSDNight;
  int _outlierPSDDay;
  int _outlierPSDNight;

  float _percentDayAlgorithmPixelQf;
  float _percentNightAlgorithmPixelQf;
  float _percentGoodQualityQf;
  float _percentDegradedQualityDueToSnowOrSeaIceQf;
  float _percentDegradedQualityDueToTwilightQf;
  float _percentInvalidDueToClearConditionsQf;
  float _percentInvalidDueLZAThresholdExceededQf;
  float _percentDegradedDueToLZAThresholdExceededQf;
  float _percentInvalidDueToNotGeolocatedQf;
  float _percentInvalidDueToMissingOrBadInputDataQf;
  float _percentDegradedDueToNonconvergenceQf;

  // QC test variables
  bool _checkQualityField;
  QcCountTestParams _badPixel;
  int _badPixelCount;
  bool _passBadCountTest;

  static const char* OUTLIER_PSD_DAY;
  static const char* OUTLIER_PSD_NIGHT;
  static const char* MIN_PSD_DAY;
  static const char* MAX_PSD_DAY;
  static const char* MEAN_PSD_DAY;
  static const char* STD_DEV_PSD_DAY;
  static const char* MIN_PSD_NIGHT;
  static const char* MAX_PSD_NIGHT;
  static const char* MEAN_PSD_NIGHT;
  static const char* STD_DEV_PSD_NIGHT;

  static const char* NUMBER_OF_QF_VALUES;
  static const char* PERCENT_DAY_ALGORITHM_PIXEL_QF;
  static const char* PERCENT_NIGHT_ALGORITHM_PIXEL_QF;
  static const char* PERCENT_GOOD_QUALITY_QF;
  static const char* PERCENT_DEGRADED_QUALITY_DUE_TO_SNOW_OR_SEA_ICE_QF;
  static const char* PERCENT_DEGRADED_QUALITY_DUE_TO_TWILIGHT_QF;
  static const char* PERCENT_INVALID_DUE_TO_CLEAR_CONDITIONS_QF;
  static const char* PERCENT_INVALID_DUE_LZA_THRESHOLD_EXCEEDED_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF;
  static const char* PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_QF;
  static const char* PERCENT_INVALID_DUE_TO_MISSING_OR_BAD_INPUT_DATA_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_NONCONVERGENCE_QF;
  

};




#endif // CLOUD_PARTICLE_SIZE_PRODUCT_HH
