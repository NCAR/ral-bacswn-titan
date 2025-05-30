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
 * @file DerivedStabilityIndicesProduct.hh
 *
 * @class DerivedStabilityIndicesProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef DERIVED_STABILITY_INDICES_PRODUCT_HH
#define DERIVED_STABILITY_INDICES_PRODUCT_HH

#include <string>

#include "GoesRProduct.hh"

/** 
 * @class DerivedStabilityIndicesProduct
 */

class DerivedStabilityIndicesProduct : public GoesRProduct
{

public:

  DerivedStabilityIndicesProduct();
  DerivedStabilityIndicesProduct(const DerivedStabilityIndicesProduct &from);
  ~DerivedStabilityIndicesProduct();
  
  void clearAndReset();
  bool applyQcTests();
  
  inline void setCheckQualityField(bool run) { _checkQualityField = run; }
  inline void setBadCountTest(bool run, int threshold)
  { _badPixel.run = run; _badPixel.threshold = threshold; }

  static const char* LIFTED_INDEX;
  static const char* CAPE;
  static const char* TOTAL_TOTALS;
  static const char* SHOWALTER_INDEX;
  static const char* K_INDEX;

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
  void _readQcDataVar(const std::string& name, QcDataContainer& qc_data);
  inline void _readConstantsVars() {}; // do nothing with virtual method

  DataContainer _liftedIndexData;
  DataContainer _capeData;
  DataContainer _totalTotalsData;
  DataContainer _showalterIndexData;
  DataContainer _kIndexData;

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

  double _finalAirPressure;
  
  // QC test variables
  bool _checkQualityField;
  QcCountTestParams _badPixel;
  int _badPixelCount;
  bool _passBadCountTest;

  static const char* FINAL_AIR_PRESSURE;
  static const char* CAPE_OUTLIER_PIXEL_COUNT;
  static const char* LIFTED_INDEX_OUTLIER_PIXEL_COUNT;
  static const char* SHOWALTER_INDEX_OUTLIER_PIXEL_COUNT;
  static const char* TOTAL_TOTALS_INDEX_OUTLIER_PIXEL_COUNT;
  static const char* K_INDEX_OUTLIER_PIXEL_COUNT;
  static const char* MIN_CAPE;
  static const char* MAX_CAPE;
  static const char* MEAN_CAPE;
  static const char* STD_DEV_CAPE;
  static const char* MIN_LIFTED_INDEX;
  static const char* MAX_LIFTED_INDEX;
  static const char* MEAN_LIFTED_INDEX;
  static const char* STD_DEV_LIFTED_INDEX;
  static const char* MIN_TOTAL_TOTALS_INDEX;
  static const char* MAX_TOTAL_TOTALS_INDEX;
  static const char* MEAN_TOTAL_TOTALS_INDEX;
  static const char* STD_DEV_TOTAL_TOTALS_INDEX;
  static const char* MIN_SHOWALTER_INDEX;
  static const char* MAX_SHOWALTER_INDEX;
  static const char* MEAN_SHOWALTER_INDEX;
  static const char* STD_DEV_SHOWALTER_INDEX;
  static const char* MIN_K_INDEX;
  static const char* MAX_K_INDEX;
  static const char* MEAN_K_INDEX;
  static const char* STD_DEV_K_INDEX;
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




#endif // DERIVED_STABILITY_INDICES_PRODUCT_HH 

