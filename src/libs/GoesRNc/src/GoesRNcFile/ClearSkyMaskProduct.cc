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
 * @file ClearSkyMaskProduct.cc
 *
 * @class ClearSkyMaskProduct
 *
 * ClearSkyMaskProduct is a derived class for the clear sky mask Level-2
 * GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/ClearSkyMaskProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

ClearSkyMaskProduct::ClearSkyMaskProduct()
{
  _badPixelCount = 0;
  _passBadCountTest = true;
  _isCatData = true;
}

ClearSkyMaskProduct::ClearSkyMaskProduct(const ClearSkyMaskProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

ClearSkyMaskProduct::~ClearSkyMaskProduct()
{
  
}

bool ClearSkyMaskProduct::applyQcTests()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  bool testRecord = true;
  
  for(size_t i = 0; i < _numX*_numY; i++) {

    if(_qcDataContainer.values[i] == DATA_QUALITY_GOOD) { 
      continue;
    }

    if(_checkQualityField == true) {
      
      _catDataContainer.values[i] = BAD_DATA_VALUE;
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

void ClearSkyMaskProduct::clearAndReset()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::clearAndReset();

  _badPixelCount = 0;
  _passBadCountTest = true;
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void ClearSkyMaskProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printVars(prefix, strarg);  

  cout << prefix << "Clear Sky Mask Vars: " << endl;
  cout << prefix << "   " << TOTAL_NUMBER_OF_CLOUD_MASK_POINTS << ":  " << _totalNumberOfCloudMaskPoints << endl;
  cout << prefix << "   " << NUMBER_OF_CLEAR_PIXELS << ":  " << _numberOfClearPixels << endl;
  cout << prefix << "   " << NUMBER_OF_PROBABLY_CLEAR_PIXELS << ":  " << _numberOfProbablyClearPixels << endl;
  cout << prefix << "   " << NUMBER_OF_PROBABLY_CLOUDY_PIXELS << ":  " <<  _numberOfProbablyCloudyPixels << endl;
  cout << prefix << "   " << NUMBER_OF_CLOUDY_PIXELS << ":  " << _numberOfCloudyPixels << endl;
  cout << prefix << "   " << PERCENT_CLEAR_PIXELS << ":  " << _percentClearPixels << endl;
  cout << prefix << "   " << PERCENT_PROBABLY_CLEAR_PIXELS << ":  " <<  _percentProbablyClearPixels << endl;
  cout << prefix << "   " << PERCENT_PROBABLY_CLOUDY_PIXELS << ":  " << _percentProbablyCloudyPixels << endl;
  cout << prefix << "   " << PERCENT_CLOUDY_PIXELS << ":  " << _percentCloudyPixels << endl;
  cout << prefix << "   " << PERCENT_TERMINATOR_PIXELS << ":  " << _percentTerminatorPixels << endl;

}


void ClearSkyMaskProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printAttrs(prefix, strarg);

  _printCatDataAttrs(prefix, "Clear Sky Mask", _catDataContainer);

  _printQcDataAttrs(prefix, "Clear Sky Mask Quality", _qcDataContainer, true);

  cout << prefix << "   " << PERCENT_GOOD_QUALITY_QF << ":  " << _percentGoodQualityQF << endl;
  cout << prefix << "   " << PERCENT_BAD_QUALITY_QF << ":  " << _percentBadQualityQF << endl;
  cout << prefix << "   " << PERCENT_SPACE_QF << ":  " << _percentSpaceQF << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_QUALITY_QF << ":  " << _percentDegradedQualityQF << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_ALGORITHM_NON_EXECUTION_QF << ":  " << _percentInvalidDueToNotGeolocatedOrAlgorithmNonExecutionQF << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF << ":  " << _percentDegradedDueToLZAThresholdExceededQF << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_INPUT_BAND_14_BRIGHTNESS_TEMPERATURE_QF << ":  " << _percentInvalidDueToBadOrMissingInputBand14BrightnessTemperatureQF << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_DUE_TO_BAD_INPUT_BAND_7_PIXEL_QF << ":  " << _percentDegradedDueToBadInputBand7PixelQF << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_DUE_TO_FAILED_BAND_2_TESTS_QF << ":  " << _percentDegradedDueToFailed_band2TestsQF << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_DUE_TO_OTHER_BAD_BANDS_QF << ":  " << _percentDegradedDueToOtherBadBandsQF << endl;

}


void ClearSkyMaskProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printDims(prefix, strarg);

    cout << prefix << "   " << BAND_DIM <<  ":  " << _numBands << endl;

    cout << prefix << "Angle info" << endl;
    cout << prefix << "   " << RETRIEVAL_LOCAL_ZENITH_ANGLE <<  ":  " << _retrievalLocalZenithAngle << endl;
    cout << prefix << "   " << QUANTITATIVE_LOCAL_ZENITH_ANGLE <<  ":  " << _quantitativeLocalZenithAngle << endl;
    cout << prefix << "   " << RETRIEVAL_SOLAR_ZENITH_ANGLE <<  ":  " << _retrievalSolarZenithAngle << endl;
    cout << prefix << "   " << TWILIGHT_SOLAR_ZENITH_ANGLE <<  ":  " << _twilightSolarZenithAngle << endl;
    cout << prefix << "   " << RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS <<  ":  " <<  _retrievalLocalZenithAngleBounds[0] << ",  " << _retrievalLocalZenithAngleBounds[1] << endl;
    cout << prefix << "   " << QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS <<  ":  " << _quantitativeLocalZenithAngleBounds[0] << ",  " << _quantitativeLocalZenithAngleBounds[1] << endl;
    cout << prefix << "   " << RETRIEVAL_SOLAR_ZENITH_ANGLE_BOUNDS <<  ":  " << _retrievalSolarZenithAngleBounds[0] << ",  " << _retrievalSolarZenithAngleBounds[1] << endl;
    cout << prefix << "   " << TWILIGHT_SOLAR_ZENITH_ANGLE_BOUNDS <<  ":  " << _twilightSolarZenithAngleBounds[0] << ",  " <<_twilightSolarZenithAngleBounds[1] << endl;

 
}


void ClearSkyMaskProduct::_printQC(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  cout << prefix << "QC test info" << endl;
  
  cout << prefix << "  Bad count test" << endl << prefix;
  if(_badPixel.run == true) {
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
    cout << "      No test" << endl;
  }
    
}


void ClearSkyMaskProduct::_readDimensions()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  try {
    _readVar(RETRIEVAL_LOCAL_ZENITH_ANGLE, _retrievalLocalZenithAngle);
    _readVar(QUANTITATIVE_LOCAL_ZENITH_ANGLE, _quantitativeLocalZenithAngle);
    _readVar(RETRIEVAL_SOLAR_ZENITH_ANGLE, _retrievalSolarZenithAngle);
    _readVar(TWILIGHT_SOLAR_ZENITH_ANGLE, _twilightSolarZenithAngle);
    _readArray(NUMBER_OF_LZA_BOUNDS, RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS,
    	       _retrievalLocalZenithAngleBounds);
    _readArray(NUMBER_OF_LZA_BOUNDS, QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS,
    	       _quantitativeLocalZenithAngleBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, RETRIEVAL_SOLAR_ZENITH_ANGLE_BOUNDS,
	       _retrievalSolarZenithAngleBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, TWILIGHT_SOLAR_ZENITH_ANGLE_BOUNDS,
	       _twilightSolarZenithAngleBounds);

    NcxxDim dim = _file.getDim(BAND_DIM);
    if(dim.isNull() == true) {
      NcxxErrStr err;
      stringstream  info;
      info << ERROR_STR << methodName << endl;
      info << "  dimension is missing, name: " << BAND_DIM << endl;
      err.addErrStr(info.str());
      throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));

    }
    _numBands = dim.getSize();    


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
 
void ClearSkyMaskProduct::_readCoordinateVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_readCoordinateVars();
}


void ClearSkyMaskProduct::_readQualityControlVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;


  // Put in some code to switch attribute names as they have changed over time.
  //
  // It appears that the names were changed to remove '.' and '-' from the attribute
  // names. This was probably done to meet some convention. The names
  // can't be converted to variables in some languages.
  //
  // a second modification took place where the quality attributes changed, swo the code below is even more muddled
  //
  NcxxVar var;

  try {
    
    _readQcDataVar(DATA_QUALITY_FLAG, _qcDataContainer, var);

      // create list (map) of containers
    _outQcDataFields[DATA_QUALITY_FLAG] = &_qcDataContainer;
   
    var.getAtt(PERCENT_GOOD_QUALITY_QF).getValues(&_percentGoodQualityQF);

    try {
      var.getAtt(PERCENT_BAD_QUALITY_QF).getValues(&_percentBadQualityQF);
      var.getAtt(PERCENT_SPACE_QF).getValues(&_percentSpaceQF);
      var.getAtt(PERCENT_DEGRADED_QUALITY_QF).getValues(&_percentDegradedQualityQF);
    }
    catch (NcxxException& e) {
      try {
        var.getAtt(PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF).getValues(&_percentDegradedDueToLZAThresholdExceededQF);
        var.getAtt(PERCENT_DEGRADED_DUE_TO_OTHER_BAD_BANDS_QF).getValues(&_percentDegradedDueToOtherBadBandsQF);
        var.getAtt(PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_ALGORITHM_NON_EXECUTION_QF).getValues(&_percentInvalidDueToNotGeolocatedOrAlgorithmNonExecutionQF);
        var.getAtt(PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_INPUT_BAND_14_BRIGHTNESS_TEMPERATURE_QF).getValues(&_percentInvalidDueToBadOrMissingInputBand14BrightnessTemperatureQF);
        var.getAtt(PERCENT_DEGRADED_DUE_TO_BAD_INPUT_BAND_7_PIXEL_QF).getValues(&_percentDegradedDueToBadInputBand7PixelQF);
        var.getAtt(PERCENT_DEGRADED_DUE_TO_FAILED_BAND_2_TESTS_QF).getValues(&_percentDegradedDueToFailed_band2TestsQF);
      }
      catch (NcxxException& e) {
        var.getAtt(PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_ALGORITHM_NON_M_EXECUTION_QF).getValues(&_percentInvalidDueToNotGeolocatedOrAlgorithmNonExecutionQF);
        var.getAtt(PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_INPUT_BAND_1119UM_BRIGHTNESS_TEMPERATURE_QF).getValues(&_percentInvalidDueToBadOrMissingInputBand14BrightnessTemperatureQF);
        var.getAtt(PERCENT_DEGRADED_DUE_TO_BAD_INPUT_BAND_389UM_PIXEL_QF).getValues(&_percentDegradedDueToBadInputBand7PixelQF);
        var.getAtt(PERCENT_DEGRADED_DUE_TO_FAILED_BAND_064UM_TESTS_QF).getValues(&_percentDegradedDueToFailed_band2TestsQF);
      }
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
  
void ClearSkyMaskProduct::_readDataVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    NcxxVar var; 
    _readCatDataVar(CLEAR_SKY_MASK, _catDataContainer, var);

    var.getAtt(CLEAR_PIXEL_DEFINITION).getValues(_clearPixelDefinition);
    var.getAtt(PROBABLY_CLEAR_PIXEL_DEFINITION).getValues(_probablyClearPixelDefinition);
    var.getAtt(PROBABLY_CLOUDY_PIXEL_DEFINITION).getValues(_probablyCloudyPixelDefinition);
    var.getAtt(CLOUDY_PIXEL_DEFINITION).getValues(_cloudyPixelDefinition);
    
    // create list (map) of containers
    _outCatDataFields[CLEAR_SKY_MASK] = &_catDataContainer;
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

/////////////////////////////////////////////////////////////////////////////////////
// string constant instantiation
//
const char* ClearSkyMaskProduct::CLEAR_SKY_MASK = "BCM";
const char* ClearSkyMaskProduct::TOTAL_NUMBER_OF_CLOUD_MASK_POINTS =
  "total_number_of_cloud_mask_points";
const char* ClearSkyMaskProduct::NUMBER_OF_CLEAR_PIXELS = "number_of_clear_pixels";
const char* ClearSkyMaskProduct::NUMBER_OF_PROBABLY_CLEAR_PIXELS =
  "number_of_probably_clear_pixels";
const char* ClearSkyMaskProduct::NUMBER_OF_PROBABLY_CLOUDY_PIXELS =
  "number_of_probably_cloudy_pixels";
const char* ClearSkyMaskProduct::NUMBER_OF_CLOUDY_PIXELS = "number_of_cloudy_pixels";
const char* ClearSkyMaskProduct::PERCENT_CLEAR_PIXELS = "percent_clear_pixels";
const char* ClearSkyMaskProduct::PERCENT_PROBABLY_CLEAR_PIXELS =
  "percent_probably_clear_pixels";
const char* ClearSkyMaskProduct::PERCENT_PROBABLY_CLOUDY_PIXELS = "percent_probably_cloudy_pixels";
const char* ClearSkyMaskProduct::PERCENT_CLOUDY_PIXELS = "percent_cloudy_pixels";
const char* ClearSkyMaskProduct::PERCENT_TERMINATOR_PIXELS = "percent_terminator_pixels";
const char* ClearSkyMaskProduct::CLEAR_PIXEL_DEFINITION = "clear_pixel_definition";
const char* ClearSkyMaskProduct::PROBABLY_CLEAR_PIXEL_DEFINITION = "probably_clear_pixel_definition";
const char* ClearSkyMaskProduct::PROBABLY_CLOUDY_PIXEL_DEFINITION = "probably_cloudy_pixel_definition";
const char* ClearSkyMaskProduct::CLOUDY_PIXEL_DEFINITION = "cloudy_pixel_definition";
const char* ClearSkyMaskProduct::PERCENT_GOOD_QUALITY_QF = "percent_good_quality_qf";
const char* ClearSkyMaskProduct::PERCENT_BAD_QUALITY_QF = "percent_bad_quality_qf";
const char* ClearSkyMaskProduct::PERCENT_SPACE_QF = "percent_space_qf";
const char* ClearSkyMaskProduct::PERCENT_DEGRADED_QUALITY_QF = "percent_degraded_quality_qf";
const char* ClearSkyMaskProduct::PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_ALGORITHM_NON_EXECUTION_QF =
    "percent_invalid_due_to_not_geolocated_or_algorithm_non_execution_qf";
const char* ClearSkyMaskProduct::PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF =
  "percent_degraded_due_to_LZA_threshold_exceeded_qf";
const char* ClearSkyMaskProduct::PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_INPUT_BAND_14_BRIGHTNESS_TEMPERATURE_QF =
  "percent_invalid_due_to_bad_or_missing_input_band_14_brightness_temperature_qf";
const char* ClearSkyMaskProduct::PERCENT_DEGRADED_DUE_TO_BAD_INPUT_BAND_7_PIXEL_QF =
  "percent_degraded_due_to_bad_input_band_7_pixel_qf";
const char* ClearSkyMaskProduct::PERCENT_DEGRADED_DUE_TO_FAILED_BAND_2_TESTS_QF =
  "percent_degraded_due_to_failed_band_2_tests_qf";
const char* ClearSkyMaskProduct::PERCENT_DEGRADED_DUE_TO_OTHER_BAD_BANDS_QF =
  "percent_degraded_due_to_other_bad_bands_qf";
const char* ClearSkyMaskProduct::MIN_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_ALL_SKY =
  "min_obs_modeled_diff_RTM_BT_comparison_bands_all_sky";
const char* ClearSkyMaskProduct::MAX_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_ALL_SKY =
  "max_obs_modeled_diff_RTM_BT_comparison_bands_all_sky";
const char* ClearSkyMaskProduct::MEAN_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_ALL_SKY =
  "mean_obs_modeled_diff_RTM_BT_comparison_bands_all_sky";
const char* ClearSkyMaskProduct::STD_DEV_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_ALL_SKY =
  "std_dev_obs_modeled_diff_RTM_BT_comparison_bands_all_sky";
const char* ClearSkyMaskProduct::MIN_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_CLEAR_SKY =
  "min_obs_modeled_diff_RTM_BT_comparison_bands_clear_sky";
const char* ClearSkyMaskProduct::MAX_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_CLEAR_SKY =
  "max_obs_modeled_diff_RTM_BT_comparison_bands_clear_sky";
const char* ClearSkyMaskProduct::MEAN_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_CLEAR_SKY =
  "mean_obs_modeled_diff_RTM_BT_comparison_bands_clear_sky";
const char* ClearSkyMaskProduct::STD_DEV_OBS_MODELED_DIFF_RTM_BT_COMPARISON_BANDS_CLEAR_SKY =
  "std_dev_obs_modeled_diff_RTM_BT_comparison_bands_clear_sky";
const char* ClearSkyMaskProduct::RTM_BT_COMPARISON_WAVELENGTHS =
  "RTM_BT_comparison_wavelengths";
const char* ClearSkyMaskProduct::RTM_BT_COMPARISON_BAND_IDS =
  "RTM_BT_comparison_band_ids";

const char* ClearSkyMaskProduct::PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_OR_ALGORITHM_NON_M_EXECUTION_QF =
  "percent_invalid_due_to_not_geolocated_or_algorithm_non-execution_qf";
const char* ClearSkyMaskProduct::PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_INPUT_BAND_1119UM_BRIGHTNESS_TEMPERATURE_QF =
  "percent_invalid_due_to_bad_or_missing_input_11.19um_brightness_temperature_qf";
const char* ClearSkyMaskProduct::PERCENT_DEGRADED_DUE_TO_BAD_INPUT_BAND_389UM_PIXEL_QF =
  "percent_degraded_due_to_bad_input_3.89um_pixel_qf";
const char* ClearSkyMaskProduct::PERCENT_DEGRADED_DUE_TO_FAILED_BAND_064UM_TESTS_QF =
  "percent_degraded_due_to_failed_0.64_um_tests_qf";
