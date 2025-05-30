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
 * @file TotalPrecipitableWaterProduct.cc
 *
 * @class TotalPrecipitableWaterProduct
 *
 * TotalPrecipitableWaterProduct is a derived class for the total 
 * precipitable water Level-2 GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/TotalPrecipitableWaterProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

TotalPrecipitableWaterProduct::TotalPrecipitableWaterProduct()
{
  _badPixelCount = 0;
  _passBadCountTest = true;
}

TotalPrecipitableWaterProduct::TotalPrecipitableWaterProduct(const TotalPrecipitableWaterProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

TotalPrecipitableWaterProduct::~TotalPrecipitableWaterProduct()
{
}

bool TotalPrecipitableWaterProduct::applyQcTests()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  bool testRecord = true;
  
  for(size_t i = 0; i < _numX*_numY; i++) {

    if(_outQcDataFields[DQF_OVERALL]->values[i] == DATA_QUALITY_GOOD) { 
      continue;
    }

    if(_checkQualityField == true) {
      
      _dataContainer.values[i] = BAD_DATA_VALUE;
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

void TotalPrecipitableWaterProduct::clearAndReset()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::clearAndReset();

  _badPixelCount = 0;
  _passBadCountTest = true;
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void TotalPrecipitableWaterProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printVars(prefix, strarg);  
  cout << prefix << "Total Precipitable Water Vars: " << endl;
  cout << prefix << "   " << MIN_TOTAL_PRECIPITABLE_WATER << ":  " << _dataContainer.minValue << endl;
  cout << prefix << "   " << MAX_TOTAL_PRECIPITABLE_WATER << ":  " << _dataContainer.maxValue << endl;
  cout << prefix << "   " << MEAN_TOTAL_PRECIPITABLE_WATER << ":  " << _dataContainer.meanValue << endl;
  cout << prefix << "   " << STD_DEV_TOTAL_PRECIPITABLE_WATER << ":  " << _dataContainer.stdDevValue << endl;
  cout << prefix << "   " << OUTLIER_PIXEL_COUNT << ":  " << _outlierPixelCount << endl;
  cout << prefix << "   " << TOTAL_ATTEMPTED_RETRIEVALS << ":  " << _totalAttemptedRetrievals << endl;

  cout << prefix << "   " << MEAN_OBS_MODELED_DIFF_SOUNDING_EMISSIVE_BANDS << ":  " << endl;
  cout << prefix;
  for(size_t i = 0; i < _meanObsModeledDiffSoundingEmissiveBands.size(); i++) {
    cout << "   " << _meanObsModeledDiffSoundingEmissiveBands[i];
  }
  cout << endl;
  
  cout << prefix << "   " << STD_DEV_OBS_MODELED_DIFF_SOUNDING_EMISSIVE_BANDS << ":  " << endl;
  cout << prefix;
  for(size_t i = 0; i < _stdDevObsModeledDiffSoundingEmissiveBands.size(); i++) {
    cout << "   " << _stdDevObsModeledDiffSoundingEmissiveBands[i];
  }
  cout << endl;
  
  cout << prefix << "   " << SOUNDING_EMISSIVE_WAVELENGTHS << ":  " << endl;
  cout << prefix;
  for(size_t i = 0; i < _soundingEmissiveWavelengths.size(); i++) {
    cout << "   " << _soundingEmissiveWavelengths[i];
  }
  cout << endl;
  
  cout << prefix << "   " << SOUNDING_EMISSIVE_BAND_IDS << ":  " << endl;
  cout << prefix;
  for(size_t i = 0; i < _soundingEmissiveIDs.size(); i++) {
    cout << "   " << _soundingEmissiveIDs[i];
  }
  cout << endl;
}


void TotalPrecipitableWaterProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printAttrs(prefix, strarg);
  
  _printDataAttrs(prefix, "Total Precipitable Water", _dataContainer);

  _printQcDataAttrs(prefix, "Overall Quality", _overallDataQuality);
  _printQcDataAttrs(prefix, "Retrieval Quality", _retrievalDataQuality);
  _printQcDataAttrs(prefix, "Skin Temperature Quality", _skintempDataQuality);
}


void TotalPrecipitableWaterProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printDims(prefix, strarg);
  
  cout << prefix << "Angle info" << endl;
  cout << prefix << "   " << RETRIEVAL_LOCAL_ZENITH_ANGLE <<  ":  " << _retrievalLocalZenithAngle << endl;
  cout << prefix << "   " << QUANTITATIVE_LOCAL_ZENITH_ANGLE <<  ":  " << _quantitativeLocalZenithAngle << endl;
  
  cout << prefix << "   " << RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS <<  ":  " <<  _retrievalLocalZenithAngleBounds[0] << ",  " << _retrievalLocalZenithAngleBounds[1] << endl;
  cout << prefix << "   " << QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS <<  ":  " << _quantitativeLocalZenithAngleBounds[0] << ",  " << _quantitativeLocalZenithAngleBounds[1] << endl;

  cout << prefix << "   " << SOLAR_ZENITH_ANGLE <<  ":  " << _solarZenithAngle << endl;
  cout << prefix << "   " << LATITUDE <<  ":  " << _latitude << endl;
  cout << prefix << "   " << SOLAR_ZENITH_ANGLE_BOUNDS <<  ":  " << _solarZenithAngleBounds[0] << ",  " << _solarZenithAngleBounds[1] << endl;
  cout << prefix << "   " << LATITUDE_BOUNDS <<  ":  " << _latitudeBounds[0] << ",  " << _latitudeBounds[1] << endl;
}


void TotalPrecipitableWaterProduct::_printQC(const std::string &prefix, const std::string &strarg)
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

void TotalPrecipitableWaterProduct::_readDimensions()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  try {
    _readVar(RETRIEVAL_LOCAL_ZENITH_ANGLE, _retrievalLocalZenithAngle);
    _readVar(QUANTITATIVE_LOCAL_ZENITH_ANGLE, _quantitativeLocalZenithAngle);
    _readVar(SOLAR_ZENITH_ANGLE, _solarZenithAngle);
    _readVar(LATITUDE, _latitude);
    _readArray(NUMBER_OF_LZA_BOUNDS, RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS, _retrievalLocalZenithAngleBounds);
    _readArray(NUMBER_OF_LZA_BOUNDS, QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS, _quantitativeLocalZenithAngleBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, SOLAR_ZENITH_ANGLE_BOUNDS, _solarZenithAngleBounds);
    _readArray(NUMBER_OF_LAT_BOUNDS, LATITUDE_BOUNDS,_latitudeBounds);
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
 
void TotalPrecipitableWaterProduct::_readCoordinateVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_readCoordinateVars();
}

void TotalPrecipitableWaterProduct::_readQualityControlVars()
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
  
void TotalPrecipitableWaterProduct::_readDataVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    NcxxVar var;
    _readDataVar(TOTAL_PRECIPITABLE_WATER, _dataContainer, var);
    
    _readVar(MIN_TOTAL_PRECIPITABLE_WATER, _dataContainer.minValue);
    _readVar(MAX_TOTAL_PRECIPITABLE_WATER, _dataContainer.maxValue);
    _readVar(MEAN_TOTAL_PRECIPITABLE_WATER, _dataContainer.meanValue);
    _readVar(STD_DEV_TOTAL_PRECIPITABLE_WATER, _dataContainer.stdDevValue);
    _readVar(OUTLIER_PIXEL_COUNT, _outlierPixelCount);
    _readVar(TOTAL_ATTEMPTED_RETRIEVALS, _totalAttemptedRetrievals);
    _readArray(SOUNDING_EMISSIVE_BANDS, MEAN_OBS_MODELED_DIFF_SOUNDING_EMISSIVE_BANDS, _meanObsModeledDiffSoundingEmissiveBands);
    _readArray(SOUNDING_EMISSIVE_BANDS, STD_DEV_OBS_MODELED_DIFF_SOUNDING_EMISSIVE_BANDS, _stdDevObsModeledDiffSoundingEmissiveBands);
    _readArray(SOUNDING_EMISSIVE_BANDS, SOUNDING_EMISSIVE_WAVELENGTHS, _soundingEmissiveWavelengths);
    _readArray(SOUNDING_EMISSIVE_BANDS, SOUNDING_EMISSIVE_BAND_IDS, _soundingEmissiveIDs);

    // create list (map) of containers
    _outDataFields[TOTAL_PRECIPITABLE_WATER] = &_dataContainer;
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


void TotalPrecipitableWaterProduct::_readQcDataVar(const std::string& name, QcDataContainer& qc_data)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    NcxxVar var;
    GoesRProduct::_readQcDataVar(name, qc_data, var);

    // use name to get particular attributtes

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
const char* TotalPrecipitableWaterProduct::TOTAL_PRECIPITABLE_WATER = "TPW";
const char* TotalPrecipitableWaterProduct::MIN_TOTAL_PRECIPITABLE_WATER =
  "minimum_total_precipitable_water";
const char* TotalPrecipitableWaterProduct::MAX_TOTAL_PRECIPITABLE_WATER =
  "maximum_total_precipitable_water";
const char* TotalPrecipitableWaterProduct::MEAN_TOTAL_PRECIPITABLE_WATER =
  "mean_total_precipitable_water";
const char* TotalPrecipitableWaterProduct::STD_DEV_TOTAL_PRECIPITABLE_WATER =
  "standard_deviation_total_precipitable_water";
const char* TotalPrecipitableWaterProduct::OUTLIER_PIXEL_COUNT = "total_attempted_retrievals";
const char* TotalPrecipitableWaterProduct::TOTAL_ATTEMPTED_RETRIEVALS = "total_attempted_retrievals";
const char* TotalPrecipitableWaterProduct::MEAN_OBS_MODELED_DIFF_SOUNDING_EMISSIVE_BANDS = "mean_obs_modeled_diff_sounding_emissive_bands";
const char* TotalPrecipitableWaterProduct::STD_DEV_OBS_MODELED_DIFF_SOUNDING_EMISSIVE_BANDS = "std_dev_obs_modeled_diff_sounding_emissive_bands";
const char* TotalPrecipitableWaterProduct::SOUNDING_EMISSIVE_WAVELENGTHS = "sounding_emissive_wavelengths";
const char* TotalPrecipitableWaterProduct::SOUNDING_EMISSIVE_BAND_IDS = "sounding_emissive_band_ids";
const char* TotalPrecipitableWaterProduct::PERCENT_GOOD_QUALITY_QF = "percent_good_quality_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_RETRIEVAL_LZA_THRESHOLD_EXCEEDED_QF = "percent_invalid_due_to_not_geolocated_or_retrieval_LZA_threshold_exceeded_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_DEGRADED_DUE_TO_LATITUDE_THRESHOLD_EXCEEDED_QF = "percent_degraded_due_to_latitude_threshold_exceeded_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_DEGRADED_DUE_TO_QUANTITATIVE_LZA_THRESHOLD_EXCEEDED_QF = "percent_degraded_due_to_quantitative_LZA_threshold_exceeded_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_INVALID_DUE_TO_INSUFFICIENT_CLEAR_PIXELS_IN_FIELD_OF_REGARD_QF = "percent_invalid_due_to_insufficient_clear_pixels_in_field_of_regard_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_INVALID_DUE_TO_MISSING_NWP_DATA_QF = "percent_invalid_due_to_missing_NWP_data_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_INVALID_DUE_TO_MISSING_L1B_DATA_OR_FATAL_PROCESSING_ERROR_QF = "percent_invalid_due_to_missing_L1b_data_or_fatal_processing_error_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_INVALID_DUE_TO_BAD_NWP_SURFACE_PRESSURE_INDEX_QF = "percent_invalid_due_to_bad_NWP_surface_pressure_index_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_INVALID_DUE_TO_INDETERMINATE_LAND_SURFACE_EMISSIVITY_QF = "percent_invalid_due_to_indeterminate_land_surface_emissivity_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_INVALID_DUE_TO_BAD_TPW_SIGMA_PRESSURE_LEVEL_INDEX_QF = "percent_invalid_due_to_bad_TPW_sigma_pressure_level_index_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_INVALID_DUE_TO_OCCURRENCE_OF_NOT_A_NUMBER_QF = "percent_invalid_due_to_occurrence_of_not_a_number_qf" ;
const char* TotalPrecipitableWaterProduct::PERCENT_GOOD_RETRIEVAL_QF = "percent_good_retrieval_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_NONCONVERGENT_RETRIEVAL_QF = "percent_nonconvergent_retrieval_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_BRIGHTNESS_TEMP_RESIDUAL_EXCEEDS_THRESHOLD_QF = "percent_brightness_temp_residual_exceeds_threshold_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_INCOMPLETE_CONVERGENCE_OF_RETRIEVAL_QF = "percent_incomplete_convergence_of_retrieval_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_UNREALISTIC_RETRIEVED_VALUE_QF = "percent_unrealistic_retrieved_value_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_INVALID_RADIATIVE_TRANSFER_MODEL_BRIGHTNESS_TEMP_VALUE_QF = "percent_invalid_radiative_transfer_model_brightness_temp_value_qf";
const char* TotalPrecipitableWaterProduct:: PERCENT_GOOD_FIRST_GUESS_SKIN_TEMP_QF = "percent_good_first_guess_skin_temp_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_FIRST_GUESS_SKIN_TEMP_EXCEEDS_UPPER_THRESHOLD_QF = "percent_first_guess_skin_temp_exceeds_upper_threshold_qf";
const char* TotalPrecipitableWaterProduct::PERCENT_FIRST_GUESS_SKIN_TEMP_EXCEEDS_LOWER_THRESHOLD_QF = "percent_first_guess_skin_temp_exceeds_lower_threshold_qf";
