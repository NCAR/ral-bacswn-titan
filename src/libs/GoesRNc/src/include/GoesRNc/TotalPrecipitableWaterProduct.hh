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
 * @file TotalPrecipitableWaterProduct.hh
 *
 * @class TotalPrecipitableWaterProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef TOTAL_PRECIPITABLE_WATER_PRODUCT_HH
#define TOTAL_PRECIPITABLE_WATER_PRODUCT_HH

#include <string>

#include "GoesRProduct.hh"

/** 
 * @class TotalPrecipitableWaterProduct
 */

class TotalPrecipitableWaterProduct : public GoesRProduct
{

public:

  static const char* TOTAL_PRECIPITABLE_WATER;

  TotalPrecipitableWaterProduct();
  TotalPrecipitableWaterProduct(const TotalPrecipitableWaterProduct &from);
  ~TotalPrecipitableWaterProduct();
  
  void clearAndReset();
  bool applyQcTests();
  
  inline void setCheckQualityField(bool run) { _checkQualityField = run; }
  inline void setBadCountTest(bool run, int threshold)
  { _badPixel.run = run; _badPixel.threshold = threshold; }
  
protected:
  


private:

  void _printVars(const std::string &prefix, const std::string &strarg = "");
  void _printAttrs(const std::string &prefix, const std::string &strarg = "");
  void _printDims(const std::string &prefix, const std::string &strarg = "");
  void _printQC(const std::string &prefix, const std::string &strarg = "");
  void _readVariables();
  void _readDimensions();
  void _readCoordinateVars();
  void _readQualityControlVars();
  void _readDataVars();
  void _readQcDataVar(const std::string& name, QcDataContainer& qc_data);
  inline void _readConstantsVars() {}; // do nothing with vitual method 

  class OverallQcDataContainer : public QcDataContainer {
  public:
    double percentGoodQuality;
    double percentInvalidDueToNotGeolocatedOrRetrievalLZAThresholdExceeded;
    double percentDegradedDueToLatitudeThresholdExceeded;
    double percentDegradedDueToQuantitativeLZAThresholdExceeded;
    double percentInvalidDueToInsufficientClearPixelsInFieldOfRegard;
    double percentInvalidDueToMissingNWPData;
    double percentInvalidDueToMissingL1bDataOrFatalProcessingError;
    double percentInvalidDueToBadNWPSurfacePressureIndex;
    double percentInvalidDueToIndeterminateLandSurfaceEmissivity;
    double percentInvalidDueToBadTPWSigmaPressureLevelIndex;
    double percentInvalidDueToOccurrenceOfNotANumber;
  };

  class RetreivalQcDataContainer : public QcDataContainer {
  public:
    double percentGoodRetrieval;
    double percentNonconvergentRetrieval;
    double percentBrightnessTempResidualExceedsThreshold;
    double percentIncompleteConvergenceOfRetrieval;
    double percentUnrealisticRetrievedValue;
    double percentInvalidRadiativeTransferModelBrightnessTempValue;
  };

  class SkintempQcDataContainer : public QcDataContainer {
  public:
    double percentGoodFirstGuessSkinTemp;
    double percentFirstGuessSkinTempExceeds_upperThreshold;
    double percentFirstGuessSkinTempExceedsLowerThreshold;
  };
  
  OverallQcDataContainer _overallDataQuality;
  RetreivalQcDataContainer _retrievalDataQuality;
  SkintempQcDataContainer _skintempDataQuality;

  int _outlierPixelCount;
  int _totalAttemptedRetrievals;
  std::vector<float> _meanObsModeledDiffSoundingEmissiveBands;
  std::vector<float> _stdDevObsModeledDiffSoundingEmissiveBands;
  std::vector<float> _soundingEmissiveWavelengths;
  std::vector<uint8_t> _soundingEmissiveIDs;

  // QC test variables
  bool _checkQualityField;
  QcCountTestParams _badPixel;
  int _badPixelCount;
  bool _passBadCountTest;

  static const char* MIN_TOTAL_PRECIPITABLE_WATER;
  static const char* MAX_TOTAL_PRECIPITABLE_WATER;
  static const char* MEAN_TOTAL_PRECIPITABLE_WATER;
  static const char* STD_DEV_TOTAL_PRECIPITABLE_WATER;
  static const char* OUTLIER_PIXEL_COUNT;
  static const char* TOTAL_ATTEMPTED_RETRIEVALS;
  static const char* MEAN_OBS_MODELED_DIFF_SOUNDING_EMISSIVE_BANDS;
  static const char* STD_DEV_OBS_MODELED_DIFF_SOUNDING_EMISSIVE_BANDS;
  static const char* SOUNDING_EMISSIVE_WAVELENGTHS;
  static const char* SOUNDING_EMISSIVE_BAND_IDS;

  static const char* PERCENT_GOOD_QUALITY_QF;
  static const char* PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_RETRIEVAL_LZA_THRESHOLD_EXCEEDED_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_LATITUDE_THRESHOLD_EXCEEDED_QF;
  static const char* PERCENT_DEGRADED_DUE_TO_QUANTITATIVE_LZA_THRESHOLD_EXCEEDED_QF;
  static const char* PERCENT_INVALID_DUE_TO_INSUFFICIENT_CLEAR_PIXELS_IN_FIELD_OF_REGARD_QF;
  static const char* PERCENT_INVALID_DUE_TO_MISSING_NWP_DATA_QF;
  static const char* PERCENT_INVALID_DUE_TO_MISSING_L1B_DATA_OR_FATAL_PROCESSING_ERROR_QF;
  static const char* PERCENT_INVALID_DUE_TO_BAD_NWP_SURFACE_PRESSURE_INDEX_QF;
  static const char* PERCENT_INVALID_DUE_TO_INDETERMINATE_LAND_SURFACE_EMISSIVITY_QF;
  static const char* PERCENT_INVALID_DUE_TO_BAD_TPW_SIGMA_PRESSURE_LEVEL_INDEX_QF;
  static const char* PERCENT_INVALID_DUE_TO_OCCURRENCE_OF_NOT_A_NUMBER_QF;
  
  static const char* PERCENT_GOOD_RETRIEVAL_QF;
  static const char* PERCENT_NONCONVERGENT_RETRIEVAL_QF;
  static const char* PERCENT_BRIGHTNESS_TEMP_RESIDUAL_EXCEEDS_THRESHOLD_QF;
  static const char* PERCENT_INCOMPLETE_CONVERGENCE_OF_RETRIEVAL_QF;
  static const char* PERCENT_UNREALISTIC_RETRIEVED_VALUE_QF;
  static const char* PERCENT_INVALID_RADIATIVE_TRANSFER_MODEL_BRIGHTNESS_TEMP_VALUE_QF;
  
  static const char* PERCENT_GOOD_FIRST_GUESS_SKIN_TEMP_QF;
  static const char* PERCENT_FIRST_GUESS_SKIN_TEMP_EXCEEDS_UPPER_THRESHOLD_QF;
  static const char* PERCENT_FIRST_GUESS_SKIN_TEMP_EXCEEDS_LOWER_THRESHOLD_QF;

};




#endif // TOTAL_PRECIPITABLE_WATER_PRODUCT_HH
