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
 * @file CloudTopPhaseProduct.hh
 *
 * @class CloudTopPhaseProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef CLOUD_TOP_PHASE_PRODUCT_HH
#define CLOUD_TOP_PHASE_PRODUCT_HH

#include <string>

#include "GoesRProduct.hh"

/** 
 * @class CloudTopPhaseProduct
 */

class CloudTopPhaseProduct : public GoesRProduct
{

public:


  CloudTopPhaseProduct();
  CloudTopPhaseProduct(const CloudTopPhaseProduct &from);
  ~CloudTopPhaseProduct();
  
  void clearAndReset();
  bool applyQcTests();
  
  inline void setCheckQualityField(bool run) { _checkQualityField = run; }
  inline void setBadCountTest(bool run, int threshold)
  { _badPixel.run = run; _badPixel.threshold = threshold; }

  static const char* CLOUD_TOP_PHASE;
  
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
  
  int _totalNumberCloudyPixels;
  int _numberOfCloudTopPhaseCategoryValues;
  float _percentClearSky;
  float _percentLiquidWater;
  float _percentSuperCooledLiquidWater;
  float _percentMixedPhase;
  float _percentIce;
  float _percentUnknown;

  int _numberOfOverallQfValues;
  int _numberOfLZAQfValues;
  int _numberOfSurfaceEmissivityQfValues;
  int _numberOfIceCloudQfValues;
  int _numberOfBetaRatioQfValues;
  int _numberOfL1bQfValues;
  float _percentOverallGoodQualityQf;
  float _percentOverallDegradedQualityQf;
  float _percentGoodQualityL1bDataQf;
  float _percentDegradedQualityL1bDataQf;
  float _percentGoodQualityBetaRatioQf;
  float _percentDegradedQualityBetaRatioQf;
  float _percentIceCloudDeterminationBasedOnStrongRadiativeSignalQf;
  float _percentIceCloudDeterminationBasedOnWeakRadiativeSignalQf;
  float _percentGoodQualitySurfaceEmissivityQf;
  float _percentDegradedQualitySurfaceEmissivityQf;
  float _percentGoodWithinLZAThresholdQf;
  float _percentDegradedDueToLZAThresholdExceededQf;

  // QC test variables
  bool _checkQualityField;
  QcCountTestParams _badPixel;
  int _badPixelCount;
  bool _passBadCountTest;

  static const char* TOTAL_NUMBER_CLOUDY_PIXELS;
  static const char* NUMBER_OF_CLOUD_TOP_PHASE_CATEGORY_VALUES;
  static const char* PERCENT_CLEAR_SKY;
  static const char* PERCENT_LIQUID_WATER;
  static const char* PERCENT_SUPER_COOLED_LIQUID_WATER;
  static const char* PERCENT_MIXED_PHASE;
  static const char* PERCENT_ICE;
  static const char* PERCENT_UNKNOWN;
  
  static const char* NUMBER_OF_OVERALL_QF_VALUES;
  static const char* PERCENT_OVERALL_GOOD_QUALITY_QF;
  static const char* PERCENT_OVERALL_DEGRADED_QUALITY_QF;
  static const char* NUMBER_OF_L1B_QF_VALUES;
  static const char* PERCENT_GOOD_QUALITY_L1B_DATA_QF;
  static const char* PERCENT_DEGRADED_QUALITY_L1B_DATA_QF;
  static const char* NUMBER_OF_BETA_RATIO_QF_VALUES;
  static const char* PERCENT_GOOD_QUALITY_BETA_RATIO_QF;
  static const char* PERCENT_DEGRADED_QUALITY_BETA_RATIO_QF;
  static const char* NUMBER_OF_ICE_CLOUD_QF_VALUES;
  static const char* PERCENT_ICE_CLOUD_DETERMINATION_BASED_ON_STRONG_RADIATIVE_SIGNAL_QF;
  static const char* PERCENT_ICE_CLOUD_DETERMINATION_BASED_ON_WEAK_RADIATIVE_SIGNAL_QF;
  static const char* NUMBER_OF_SURFACE_EMISSIVITY_QF_VALUES;
  static const char* PERCENT_GOOD_QUALITY_SURFACE_EMISSIVITY_QF;
  static const char* PERCENT_DEGRADED_QUALITY_SURFACE_EMISSIVITY_QF;
  static const char* NUMBER_OF_LZA_QF_VALUES;
  static const char* PERCENT_GOOD_WITHIN_LZA_THRESHOLD_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF;
  static const char* NUMBER_OF_SATELLITE_ZENITH_THRESHOLD_QF_VALUES;
  static const char* PERCENT_GOOD_WITHIN_SATELLITE_ZENITH_THRESHOLD_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_SATELLITE_ZENITH_THRESHOLD_EXCEEDED_QF;

};




#endif // CLOUD_TOP_PHASE_PRODUCT_HH
