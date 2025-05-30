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
 * @file ClearSkyMaskProduct.hh
 *
 * @class ClearSkyMaskProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef CLEAR_SKY_MASK_PRODUCT_HH
#define CLEAR_SKY_MASK_PRODUCT_HH

#include <string>

#include "GoesRProduct.hh"

/** 
 * @class ClearSkyMaskProduct
 */

class ClearSkyMaskProduct : public GoesRProduct
{

public:


  ClearSkyMaskProduct();
  ClearSkyMaskProduct(const ClearSkyMaskProduct &from);
  ~ClearSkyMaskProduct();
  
  void clearAndReset();
  bool applyQcTests();
  
  inline void setCheckQualityField(bool run) { _checkQualityField = run; }
  inline void setBadCountTest(bool run, int threshold)
  { _badPixel.run = run; _badPixel.threshold = threshold; }

  static const char* CLEAR_SKY_MASK;

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

  float _retrievalSolarZenithAngle;
  float _twilightSolarZenithAngle;
  std::vector< float > _retrievalSolarZenithAngleBounds;
  std::vector< float > _twilightSolarZenithAngleBounds;

  size_t _numBands;
  int _totalNumberOfCloudMaskPoints;	
  int _numberOfClearPixels;	
  int _numberOfProbablyClearPixels;	
  int _numberOfProbablyCloudyPixels;	
  int _numberOfCloudyPixels;	
  float _percentClearPixels;	
  float _percentProbablyClearPixels;	
  float _percentProbablyCloudyPixels;	
  float _percentCloudyPixels;	
  float _percentTerminatorPixels;

  float _percentGoodQualityQF;
  float _percentBadQualityQF;
  float _percentSpaceQF;
  float _percentDegradedQualityQF;
  float _percentInvalidDueToNotGeolocatedOrAlgorithmNonExecutionQF;
  float _percentDegradedDueToLZAThresholdExceededQF;	
  float _percentInvalidDueToBadOrMissingInputBand14BrightnessTemperatureQF;	
  float _percentDegradedDueToBadInputBand7PixelQF;
  float _percentDegradedDueToFailed_band2TestsQF;	
  float _percentDegradedDueToOtherBadBandsQF;

  //  std::string _flagMeanings;
  std::string _clearPixelDefinition;	
  std::string _probablyClearPixelDefinition;	
  std::string _probablyCloudyPixelDefinition;	
  std::string _cloudyPixelDefinition;

  // QC test variables
  bool _checkQualityField;
  QcCountTestParams _badPixel;
  int _badPixelCount;
  bool _passBadCountTest;

  static const char* TOTAL_NUMBER_OF_CLOUD_MASK_POINTS;
  static const char* NUMBER_OF_CLOUDY_PIXELS;
  static const char* NUMBER_OF_CLEAR_PIXELS;
  static const char* NUMBER_OF_PROBABLY_CLEAR_PIXELS;
  static const char* NUMBER_OF_PROBABLY_CLOUDY_PIXELS;
  static const char* PERCENT_CLEAR_PIXELS;
  static const char* PERCENT_PROBABLY_CLEAR_PIXELS;
  static const char* PERCENT_PROBABLY_CLOUDY_PIXELS;
  static const char* PERCENT_CLOUDY_PIXELS;
  static const char* PERCENT_TERMINATOR_PIXELS;
  static const char* CLEAR_PIXEL_DEFINITION;
  static const char* PROBABLY_CLEAR_PIXEL_DEFINITION;
  static const char* PROBABLY_CLOUDY_PIXEL_DEFINITION;
  static const char* CLOUDY_PIXEL_DEFINITION;
  static const char* PERCENT_GOOD_QUALITY_QF;
  static const char* PERCENT_BAD_QUALITY_QF;
  static const char* PERCENT_SPACE_QF;
  static const char* PERCENT_DEGRADED_QUALITY_QF;
  static const char* PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_ALGORITHM_NON_EXECUTION_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF;
  static const char* PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_INPUT_BAND_14_BRIGHTNESS_TEMPERATURE_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_BAD_INPUT_BAND_7_PIXEL_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_FAILED_BAND_2_TESTS_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_OTHER_BAD_BANDS_QF;
  static const char* MIN_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_ALL_SKY;
  static const char* MAX_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_ALL_SKY;
  static const char* MEAN_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_ALL_SKY;
  static const char* STD_DEV_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_ALL_SKY;
  static const char* MIN_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_CLEAR_SKY;
  static const char* MAX_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_CLEAR_SKY;
  static const char* MEAN_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_CLEAR_SKY;
  static const char* STD_DEV_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_CLEAR_SKY;
  static const char* RTM_BT_COMPARISON_WAVELENGTHS;
  static const char* RTM_BT_COMPARISON_BAND_IDS;

  static const char* PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_ALGORITHM_NON_M_EXECUTION_QF;
  static const char* PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_INPUT_BAND_1119UM_BRIGHTNESS_TEMPERATURE_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_BAD_INPUT_BAND_389UM_PIXEL_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_FAILED_BAND_064UM_TESTS_QF;

};




#endif // CLEAR_SKY_MASK_PRODUCT_HH
