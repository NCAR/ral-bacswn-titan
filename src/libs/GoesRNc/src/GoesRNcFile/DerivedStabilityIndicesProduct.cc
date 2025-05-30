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
 * @file DerivedStabilityIndicesProduct.cc
 *
 * @class DerivedStabilityIndicesProduct
 *
 * DerivedStabilityIndicesProduct is a derived class for the dervived
 * stability indicies Level-2 GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/DerivedStabilityIndicesProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

DerivedStabilityIndicesProduct::DerivedStabilityIndicesProduct()
{
  _badPixelCount = 0;
  _passBadCountTest = true;
}

DerivedStabilityIndicesProduct::DerivedStabilityIndicesProduct(const DerivedStabilityIndicesProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

DerivedStabilityIndicesProduct::~DerivedStabilityIndicesProduct()
{
}

bool DerivedStabilityIndicesProduct::applyQcTests()
{
 static const string methodName = _className + "::" + __FUNCTION__;

  bool testRecord = true;
  
  for(size_t i = 0; i < _numX*_numY; i++) {

    if(_outQcDataFields[DQF_OVERALL]->values[i] == DATA_QUALITY_GOOD) { 
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

void DerivedStabilityIndicesProduct::clearAndReset()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::clearAndReset();

  _badPixelCount = 0;
  _passBadCountTest = true;
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void DerivedStabilityIndicesProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printVars(prefix, strarg);  

  cout << prefix << "   " << FINAL_AIR_PRESSURE << ":  " << _finalAirPressure << endl;

  cout << prefix << "CAPE Vars: " << endl;
  cout << prefix << "   " << CAPE_OUTLIER_PIXEL_COUNT << ":  " << _capeData.outlierPixelCount << endl;
  cout << prefix << "   " << MIN_CAPE << ":  " << _capeData.minValue << endl;
  cout << prefix << "   " << MAX_CAPE << ":  " << _capeData.maxValue << endl;
  cout << prefix << "   " << MEAN_CAPE << ":  " << _capeData.meanValue << endl;
  cout << prefix << "   " << STD_DEV_CAPE << ":  " << _capeData.stdDevValue << endl;

  cout << prefix << "Lifted Index Vars: " << endl;
  cout << prefix << "   " << LIFTED_INDEX_OUTLIER_PIXEL_COUNT << ":  " << _liftedIndexData.outlierPixelCount << endl;
  cout << prefix << "   " << MIN_LIFTED_INDEX << ":  " << _liftedIndexData.minValue << endl;
  cout << prefix << "   " << MAX_LIFTED_INDEX << ":  " << _liftedIndexData.maxValue << endl;
  cout << prefix << "   " << MEAN_LIFTED_INDEX << ":  " << _liftedIndexData.meanValue << endl;
  cout << prefix << "   " << STD_DEV_LIFTED_INDEX << ":  " << _liftedIndexData.stdDevValue << endl;

  cout << prefix << "Total Totals Vars: " << endl;
  cout << prefix << "   " << TOTAL_TOTALS_INDEX_OUTLIER_PIXEL_COUNT << ":  " << _totalTotalsData.outlierPixelCount << endl;
  cout << prefix << "   " << MIN_TOTAL_TOTALS_INDEX << ":  " << _totalTotalsData.minValue << endl;
  cout << prefix << "   " << MAX_TOTAL_TOTALS_INDEX << ":  " << _totalTotalsData.maxValue << endl;
  cout << prefix << "   " << MEAN_TOTAL_TOTALS_INDEX << ":  " << _totalTotalsData.meanValue << endl;
  cout << prefix << "   " << STD_DEV_TOTAL_TOTALS_INDEX << ":  " << _totalTotalsData.stdDevValue << endl;

  cout << prefix << "Showalter Index Vars: " << endl;
  cout << prefix << "   " << SHOWALTER_INDEX_OUTLIER_PIXEL_COUNT << ":  " << _showalterIndexData.outlierPixelCount << endl;
  cout << prefix << "   " << MIN_SHOWALTER_INDEX << ":  " << _showalterIndexData.minValue << endl;
  cout << prefix << "   " << MAX_SHOWALTER_INDEX << ":  " << _showalterIndexData.maxValue << endl;
  cout << prefix << "   " << MEAN_SHOWALTER_INDEX << ":  " << _showalterIndexData.meanValue << endl;
  cout << prefix << "   " << STD_DEV_SHOWALTER_INDEX << ":  " << _showalterIndexData.stdDevValue << endl;

  cout << prefix << "K Index Vars: " << endl;
  cout << prefix << "   " << K_INDEX_OUTLIER_PIXEL_COUNT << ":  " << _kIndexData.outlierPixelCount << endl;
  cout << prefix << "   " << MIN_K_INDEX << ":  " << _kIndexData.minValue << endl;
  cout << prefix << "   " << MAX_K_INDEX << ":  " << _kIndexData.maxValue << endl;
  cout << prefix << "   " << MEAN_K_INDEX << ":  " << _kIndexData.meanValue << endl;
  cout << prefix << "   " << STD_DEV_K_INDEX << ":  " << _kIndexData.stdDevValue << endl;

}


void DerivedStabilityIndicesProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printAttrs(prefix, strarg);
  cout << prefix << " Attributes: " << endl;

  _printDataAttrs(prefix, "CAPE", _capeData);
  _printDataAttrs(prefix, "Lifted Index", _liftedIndexData);
  _printDataAttrs(prefix, "Total Totals Index", _totalTotalsData);
  _printDataAttrs(prefix, "Showalter Index", _showalterIndexData);
  _printDataAttrs(prefix, "K-Index", _kIndexData);
  _printQcDataAttrs(prefix, "Overall Quality", _overallDataQuality, false);
  _printQcDataAttrs(prefix, "Retrieval Quality", _retrievalDataQuality, false);
  _printQcDataAttrs(prefix, "Skin Temperature Quality", _skintempDataQuality, false);
}


void DerivedStabilityIndicesProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printDims(prefix, strarg);   
}


void DerivedStabilityIndicesProduct::_printQC(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  cout << prefix << "QC test info" << endl;
  
  if(_badPixel.run == true) {
    cout << prefix << "  Bad count test" << endl << prefix;
    if(_passBadCountTest == false) {
      cout << "      Failed ";
    }
    else {
      cout << "      Passed ";
    }
    cout << "test --  count: " << _badPixelCount << "  threshold: "
	 << _badPixel.threshold << endl;
  }
  else {
    cout << prefix << "      No test" << endl;
  }
}

void DerivedStabilityIndicesProduct::_readDimensions()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  try {
    _readVar(SOLAR_ZENITH_ANGLE, _solarZenithAngle);
    _readVar(RETRIEVAL_LOCAL_ZENITH_ANGLE, _retrievalLocalZenithAngle);
    _readVar(QUANTITATIVE_LOCAL_ZENITH_ANGLE, _quantitativeLocalZenithAngle);
    _readVar(LATITUDE, _latitude);
    _readArray(NUMBER_OF_LZA_BOUNDS, RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS, _retrievalLocalZenithAngleBounds);
    _readArray(NUMBER_OF_LZA_BOUNDS, QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS, _quantitativeLocalZenithAngleBounds);
    _readArray(NUMBER_OF_LAT_BOUNDS, LATITUDE_BOUNDS, _latitudeBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, SOLAR_ZENITH_ANGLE_BOUNDS, _solarZenithAngleBounds);
  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    cerr << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
}
 
void DerivedStabilityIndicesProduct::_readCoordinateVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_readCoordinateVars();
}


void DerivedStabilityIndicesProduct::_readQualityControlVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {

    _readQcDataVar(DQF_OVERALL, _overallDataQuality);
    _readQcDataVar(DQF_RETRIEVAL, _retrievalDataQuality);
    _readQcDataVar(DQF_SKINTEMP, _skintempDataQuality);
    
   // create list (map) of containers
    _outQcDataFields[DQF_OVERALL] = &_overallDataQuality;
    _outQcDataFields[DQF_RETRIEVAL] = &_retrievalDataQuality;
    _outQcDataFields[DQF_SKINTEMP] = &_skintempDataQuality;
    
  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    cerr << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
}
  
void DerivedStabilityIndicesProduct::_readDataVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
 
  try {
    NcxxVar var;

    _readVar(FINAL_AIR_PRESSURE, _finalAirPressure);

    // CAPE
    _readDataVar(CAPE, _capeData, var);
    _readVar(CAPE_OUTLIER_PIXEL_COUNT, _capeData.outlierPixelCount);
    _readVar(MIN_CAPE, _capeData.minValue);
    _readVar(MAX_CAPE, _capeData.maxValue);
    _readVar(MEAN_CAPE, _capeData.meanValue);
    _readVar(STD_DEV_CAPE, _capeData.stdDevValue);

    // lifted index
    _readDataVar(LIFTED_INDEX, _liftedIndexData, var);
    _readVar(LIFTED_INDEX_OUTLIER_PIXEL_COUNT, _liftedIndexData.outlierPixelCount);
    _readVar(MIN_LIFTED_INDEX, _liftedIndexData.minValue);
    _readVar(MAX_LIFTED_INDEX, _liftedIndexData.maxValue);
    _readVar(MEAN_LIFTED_INDEX, _liftedIndexData.meanValue);
    _readVar(STD_DEV_LIFTED_INDEX, _liftedIndexData.stdDevValue);

    // Total Totals Index
    _readDataVar(TOTAL_TOTALS, _totalTotalsData, var);
    _readVar(TOTAL_TOTALS_INDEX_OUTLIER_PIXEL_COUNT, _totalTotalsData.outlierPixelCount);
    _readVar(MIN_TOTAL_TOTALS_INDEX, _totalTotalsData.minValue);
    _readVar(MAX_TOTAL_TOTALS_INDEX, _totalTotalsData.maxValue);
    _readVar(MEAN_TOTAL_TOTALS_INDEX, _totalTotalsData.meanValue);
    _readVar(STD_DEV_TOTAL_TOTALS_INDEX, _totalTotalsData.stdDevValue);

    // Showalter Index
    _readDataVar(SHOWALTER_INDEX, _showalterIndexData, var);
    _readVar(SHOWALTER_INDEX_OUTLIER_PIXEL_COUNT, _showalterIndexData.outlierPixelCount);
    _readVar(MIN_SHOWALTER_INDEX, _showalterIndexData.minValue);
    _readVar(MAX_SHOWALTER_INDEX, _showalterIndexData.maxValue);
    _readVar(MEAN_SHOWALTER_INDEX, _showalterIndexData.meanValue);
    _readVar(STD_DEV_SHOWALTER_INDEX, _showalterIndexData.stdDevValue);

    // K-Index
    _readDataVar(K_INDEX, _kIndexData, var);
    _readVar(K_INDEX_OUTLIER_PIXEL_COUNT, _kIndexData.outlierPixelCount);
    _readVar(MIN_K_INDEX, _kIndexData.minValue);
    _readVar(MAX_K_INDEX, _kIndexData.maxValue);
    _readVar(MEAN_K_INDEX, _kIndexData.meanValue);
    _readVar(STD_DEV_K_INDEX, _kIndexData.stdDevValue);


    // create list (map) of containers
    _outDataFields[LIFTED_INDEX] = &_liftedIndexData;
    _outDataFields[CAPE] = &_capeData;
    _outDataFields[TOTAL_TOTALS] = &_totalTotalsData;
    _outDataFields[SHOWALTER_INDEX] = &_showalterIndexData;
    _outDataFields[K_INDEX] = &_kIndexData;

    
  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

}


/*********************************************************************
 * _readQcDataVar()
 */

void DerivedStabilityIndicesProduct::_readQcDataVar(const std::string& name, QcDataContainer& qc_data)
{
  static const string methodName = _className + "::" + __FUNCTION__;


  try {
    // use name to get particular attributtes
    NcxxVar var;
    GoesRProduct::_readQcDataVar(name, qc_data,var);

    if (name == DQF_OVERALL) {
      OverallQcDataContainer* qcData = static_cast<OverallQcDataContainer*>(&qc_data);
      var.getAtt(PERCENT_GOOD_QUALITY_QF).getValues(&(qcData->percentGoodQuality));
      var.getAtt(PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_RETRIEVAL_LZA_THRESHOLD_EXCEEDED_QF).getValues(&(qcData->percentInvalidDueToNotGeolocatedOrRetrievalLZAThresholdExceeded));
      var.getAtt(PERCENT_DEGRADED_DUE_TO_QUANTITATIVE_LZA_THRESHOLD_EXCEEDED_QF).getValues(&(qcData->percentDegradedDueToLatitudeThresholdExceeded));
      var.getAtt(PERCENT_DEGRADED_DUE_TO_QUANTITATIVE_LZA_THRESHOLD_EXCEEDED_QF).getValues(&(qcData->percentDegradedDueToQuantitativeLZAThresholdExceeded));
      var.getAtt(PERCENT_INVALID_DUE_TO_INSUFFICIENT_CLEAR_PIXELS_IN_FIELD_OF_REGARD_QF).getValues(&(qcData->percentInvalidDueToInsufficientClearPixelsInFieldOfRegard));
      var.getAtt(PERCENT_INVALID_DUE_TO_MISSING_NWP_DATA_QF).getValues(&(qcData->percentInvalidDueToMissingNWPData));
      var.getAtt(PERCENT_INVALID_DUE_TO_MISSING_L1B_DATA_OR_FATAL_PROCESSING_ERROR_QF).getValues(&(qcData->percentInvalidDueToMissingL1bDataOrFatalProcessingError));
      var.getAtt(PERCENT_INVALID_DUE_TO_BAD_NWP_SURFACE_PRESSURE_INDEX_QF).getValues(&(qcData->percentInvalidDueToBadNWPSurfacePressureIndex));
      var.getAtt(PERCENT_INVALID_DUE_TO_INDETERMINATE_LAND_SURFACE_EMISSIVITY_QF).getValues(&(qcData->percentInvalidDueToIndeterminateLandSurfaceEmissivity));
      var.getAtt(PERCENT_INVALID_DUE_TO_BAD_TPW_SIGMA_PRESSURE_LEVEL_INDEX_QF).getValues(&(qcData->percentInvalidDueToBadTPWSigmaPressureLevelIndex));
      var.getAtt(PERCENT_INVALID_DUE_TO_OCCURRENCE_OF_NOT_A_NUMBER_QF).getValues(&(qcData->percentInvalidDueToOccurrenceOfNotANumber));
    }
    else if (name == DQF_RETRIEVAL) {
      RetreivalQcDataContainer* qcData = static_cast<RetreivalQcDataContainer*>(&qc_data);
      var.getAtt(PERCENT_GOOD_RETRIEVAL_QF).getValues(&(qcData->percentGoodRetrieval));
      var.getAtt(PERCENT_NONCONVERGENT_RETRIEVAL_QF).getValues(&(qcData->percentNonconvergentRetrieval));
      var.getAtt(PERCENT_BRIGHTNESS_TEMP_RESIDUAL_EXCEEDS_THRESHOLD_QF).getValues(&(qcData->percentBrightnessTempResidualExceedsThreshold));
      var.getAtt(PERCENT_INCOMPLETE_CONVERGENCE_OF_RETRIEVAL_QF).getValues(&(qcData->percentIncompleteConvergenceOfRetrieval));
      var.getAtt(PERCENT_UNREALISTIC_RETRIEVED_VALUE_QF).getValues(&(qcData->percentUnrealisticRetrievedValue));
      var.getAtt(PERCENT_INVALID_RADIATIVE_TRANSFER_MODEL_BRIGHTNESS_TEMP_VALUE_QF).getValues(&(qcData->percentInvalidRadiativeTransferModelBrightnessTempValue));
    }
    else { // must be DQF_SKINTEMP
      SkintempQcDataContainer *qcData = static_cast<SkintempQcDataContainer*>(&qc_data);
      var.getAtt(PERCENT_GOOD_FIRST_GUESS_SKIN_TEMP_QF).getValues(&(qcData->percentGoodFirstGuessSkinTemp));
      var.getAtt(PERCENT_FIRST_GUESS_SKIN_TEMP_EXCEEDS_UPPER_THRESHOLD_QF).getValues(&(qcData->percentFirstGuessSkinTempExceeds_upperThreshold));
      var.getAtt(PERCENT_FIRST_GUESS_SKIN_TEMP_EXCEEDS_LOWER_THRESHOLD_QF).getValues(&(qcData->percentFirstGuessSkinTempExceedsLowerThreshold));
    }
    
  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    cerr << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
}


/////////////////////////////////////////////////////////////////////////////////////
// string constant instantiation
//
const char* DerivedStabilityIndicesProduct::LIFTED_INDEX = "LI";
const char* DerivedStabilityIndicesProduct::CAPE = "CAPE";
const char* DerivedStabilityIndicesProduct::TOTAL_TOTALS = "TT";
const char* DerivedStabilityIndicesProduct::SHOWALTER_INDEX = "SI";
const char* DerivedStabilityIndicesProduct::K_INDEX = "KI";
const char* DerivedStabilityIndicesProduct::FINAL_AIR_PRESSURE = "final_air_pressure";
const char* DerivedStabilityIndicesProduct::CAPE_OUTLIER_PIXEL_COUNT = "CAPE_outlier_pixel_count";
const char* DerivedStabilityIndicesProduct::LIFTED_INDEX_OUTLIER_PIXEL_COUNT =
  "lifted_index_outlier_pixel_count";
const char* DerivedStabilityIndicesProduct::K_INDEX_OUTLIER_PIXEL_COUNT = "k_index_outlier_pixel_count";
const char* DerivedStabilityIndicesProduct::SHOWALTER_INDEX_OUTLIER_PIXEL_COUNT =
  "showalter_index_outlier_pixel_count";
const char* DerivedStabilityIndicesProduct::TOTAL_TOTALS_INDEX_OUTLIER_PIXEL_COUNT =
  "total_totals_index_outlier_pixel_count";
const char* DerivedStabilityIndicesProduct::MIN_CAPE = "minimum_CAPE";
const char* DerivedStabilityIndicesProduct::MAX_CAPE = "maximum_CAPE";
const char* DerivedStabilityIndicesProduct::MEAN_CAPE = "mean_CAPE";
const char* DerivedStabilityIndicesProduct::STD_DEV_CAPE = "standard_deviation_CAPE";
const char* DerivedStabilityIndicesProduct::MIN_LIFTED_INDEX = "minimum_lifted_index";
const char* DerivedStabilityIndicesProduct::MAX_LIFTED_INDEX = "maximum_lifted_index";
const char* DerivedStabilityIndicesProduct::MEAN_LIFTED_INDEX = "mean_lifted_index";
const char* DerivedStabilityIndicesProduct::STD_DEV_LIFTED_INDEX = "standard_deviation_lifted_index";
const char* DerivedStabilityIndicesProduct::MIN_SHOWALTER_INDEX = "minimum_showalter_index";
const char* DerivedStabilityIndicesProduct::MAX_SHOWALTER_INDEX = "maximum_showalter_index";
const char* DerivedStabilityIndicesProduct::MEAN_SHOWALTER_INDEX = "mean_showalter_index";
const char* DerivedStabilityIndicesProduct::STD_DEV_SHOWALTER_INDEX =
  "standard_deviation_showalter_index";
const char* DerivedStabilityIndicesProduct::MIN_TOTAL_TOTALS_INDEX = "minimum_total_totals_index";
const char* DerivedStabilityIndicesProduct::MAX_TOTAL_TOTALS_INDEX = "maximum_total_totals_index";
const char* DerivedStabilityIndicesProduct::MEAN_TOTAL_TOTALS_INDEX = "mean_total_totals_index";
const char* DerivedStabilityIndicesProduct::STD_DEV_TOTAL_TOTALS_INDEX =
  "standard_deviation_total_totals_index";
const char* DerivedStabilityIndicesProduct::MIN_K_INDEX = "minimum_k_index";
const char* DerivedStabilityIndicesProduct::MAX_K_INDEX = "maximum_k_index";
const char* DerivedStabilityIndicesProduct::MEAN_K_INDEX = "mean_k_index";
const char* DerivedStabilityIndicesProduct::STD_DEV_K_INDEX = "standard_deviation_k_index";
   const char* DerivedStabilityIndicesProduct::PERCENT_GOOD_QUALITY_QF = "percent_good_quality_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_RETRIEVAL_LZA_THRESHOLD_EXCEEDED_QF = "percent_invalid_due_to_not_geolocated_or_retrieval_LZA_threshold_exceeded_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_DEGRADED_DUE_TO_LATITUDE_THRESHOLD_EXCEEDED_QF = "percent_degraded_due_to_latitude_threshold_exceeded_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_DEGRADED_DUE_TO_QUANTITATIVE_LZA_THRESHOLD_EXCEEDED_QF = "percent_degraded_due_to_quantitative_LZA_threshold_exceeded_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_INVALID_DUE_TO_INSUFFICIENT_CLEAR_PIXELS_IN_FIELD_OF_REGARD_QF = "percent_invalid_due_to_insufficient_clear_pixels_in_field_of_regard_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_INVALID_DUE_TO_MISSING_NWP_DATA_QF = "percent_invalid_due_to_missing_NWP_data_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_INVALID_DUE_TO_MISSING_L1B_DATA_OR_FATAL_PROCESSING_ERROR_QF = "percent_invalid_due_to_missing_L1b_data_or_fatal_processing_error_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_INVALID_DUE_TO_BAD_NWP_SURFACE_PRESSURE_INDEX_QF = "percent_invalid_due_to_bad_NWP_surface_pressure_index_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_INVALID_DUE_TO_INDETERMINATE_LAND_SURFACE_EMISSIVITY_QF = "percent_invalid_due_to_indeterminate_land_surface_emissivity_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_INVALID_DUE_TO_BAD_TPW_SIGMA_PRESSURE_LEVEL_INDEX_QF = "percent_invalid_due_to_bad_TPW_sigma_pressure_level_index_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_INVALID_DUE_TO_OCCURRENCE_OF_NOT_A_NUMBER_QF = "percent_invalid_due_to_occurrence_of_not_a_number_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_GOOD_RETRIEVAL_QF = "percent_good_retrieval_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_NONCONVERGENT_RETRIEVAL_QF = "percent_nonconvergent_retrieval_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_BRIGHTNESS_TEMP_RESIDUAL_EXCEEDS_THRESHOLD_QF = "percent_brightness_temp_residual_exceeds_threshold_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_INCOMPLETE_CONVERGENCE_OF_RETRIEVAL_QF = "percent_incomplete_convergence_of_retrieval_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_UNREALISTIC_RETRIEVED_VALUE_QF = "percent_unrealistic_retrieved_value_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_INVALID_RADIATIVE_TRANSFER_MODEL_BRIGHTNESS_TEMP_VALUE_QF = "percent_invalid_radiative_transfer_model_brightness_temp_value_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_GOOD_FIRST_GUESS_SKIN_TEMP_QF = "percent_good_first_guess_skin_temp_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_FIRST_GUESS_SKIN_TEMP_EXCEEDS_UPPER_THRESHOLD_QF = "percent_first_guess_skin_temp_exceeds_upper_threshold_qf";
   const char* DerivedStabilityIndicesProduct::PERCENT_FIRST_GUESS_SKIN_TEMP_EXCEEDS_LOWER_THRESHOLD_QF = "percent_first_guess_skin_temp_exceeds_lower_threshold_qf";

