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
 * @file CloudOpticalDepthProduct.cc
 *
 * @class CloudOpticalDepthProduct
 *
 * CloudOpticalDepthProduct is a derived class for the cloud optical
 * depth Level-2 GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/CloudOpticalDepthProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

CloudOpticalDepthProduct::CloudOpticalDepthProduct()
{
  _badPixelCount = 0;
  _passBadCountTest = true;
}

CloudOpticalDepthProduct::CloudOpticalDepthProduct(const CloudOpticalDepthProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

CloudOpticalDepthProduct::~CloudOpticalDepthProduct()
{
}

bool CloudOpticalDepthProduct::applyQcTests()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  bool testRecord = true;
  
  for(size_t i = 0; i < _numX*_numY; i++) {

    if(_qcDataContainer.values[i] == DATA_QUALITY_GOOD) { 
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

void CloudOpticalDepthProduct::clearAndReset()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::clearAndReset();

  _badPixelCount = 0;
  _passBadCountTest = true;
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void CloudOpticalDepthProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printVars(prefix, strarg);  
   cout << prefix << "Cloud Optical Depth Vars: " << endl;
   cout << prefix << "   " << MIN_COD_DAY << ":  " << _minimuimCODDay << endl;
   cout << prefix << "   " << MAX_COD_DAY << ":  " << _maximuimCODDay << endl;
   cout << prefix << "   " << MEAN_COD_DAY << ":  " << _meanCODDay << endl;
   cout << prefix << "   " << STD_DEV_COD_DAY << ":  " << _stdDevCODDay << endl;
   cout << prefix << "   " << MIN_COD_NIGHT << ":  " << _minimuimCODNight << endl;
   cout << prefix << "   " << MAX_COD_NIGHT << ":  " << _maximuimCODNight << endl;
   cout << prefix << "   " << MEAN_COD_NIGHT << ":  " << _meanCODNight << endl;
   cout << prefix << "   " << STD_DEV_COD_NIGHT << ":  " << _stdDevCODNight << endl;
   cout << prefix << "   " << COD_PRODUCT_WAVELENGTH << ":  " << _codProductWavelength << endl;
   cout << prefix << "   " << OUTLIER_COD_DAY << ":  " << _outlierCODDay << endl;
   cout << prefix << "   " << OUTLIER_COD_NIGHT << ":  " << _outlierCODNight << endl;
}


void CloudOpticalDepthProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printAttrs(prefix, strarg);
  _printDataAttrs(prefix, "Cloud Optical Depth", _dataContainer);
  _printQcDataAttrs(prefix, "Cloud Optical Depth Quality", _qcDataContainer, true);

  cout << prefix << "   " << PERCENT_DAY_ALGORITHM_PIXEL_QF << ":  " << _percentDayAlgorithmPixelQf << endl;
  cout << prefix << "   " << PERCENT_NIGHT_ALGORITHM_PIXEL_QF << ":  " << _percentNightAlgorithmPixelQf << endl;
  cout << prefix << "   " << PERCENT_GOOD_QUALITY_QF << ":  " << _percentGoodQualityQf << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_QUALITY_DUE_TO_SNOW_OR_SEA_ICE_QF << ":  " << _percentDegradedQualityDueToSnowOrSeaIceQf << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_QUALITY_DUE_TO_TWILIGHT_QF << ":  " << _percentDegradedQualityDueToTwilightQf << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_CLEAR_CONDITIONS_QF << ":  " << _percentInvalidDueToClearConditionsQf << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_LZA_THRESHOLD_EXCEEDED_QF << ":  " << _percentInvalidDueLZAThresholdExceededQf << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF << ":  " << _percentDegradedDueToLZAThresholdExceededQf << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_QF << ":  " << _percentInvalidDueToNotGeolocatedQf << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_MISSING_OR_BAD_INPUT_DATA_QF << ":  " << _percentInvalidDueToMissingOrBadInputDataQf << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_DUE_TO_NONCONVERGENCE_QF << ":  " << _percentDegradedDueToNonconvergenceQf << endl;
  
}


void CloudOpticalDepthProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
   static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printDims(prefix, strarg);   

  cout << prefix << "Angle info" << endl;
  cout << prefix << "   " << DAY_RETRIEVAL_LOCAL_ZENITH_ANGLE <<  ":  " << _dayRetrievalLocalZenithAngle << endl;
  cout << prefix << "   " << NIGHT_RETRIEVAL_LOCAL_ZENITH_ANGLE <<  ":  " << _nightRetrievalLocalZenithAngle << endl;
  cout << prefix << "   " << QUANTITATIVE_LOCAL_ZENITH_ANGLE <<  ":  " << _quantitativeLocalZenithAngle << endl;
  
  cout << prefix << "   " << DAY_RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS <<  ":  " <<  _dayRetrievalLocalZenithAngleBounds[0] << ",  " << _dayRetrievalLocalZenithAngleBounds[1] << endl;
  cout << prefix << "   " << NIGHT_RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS <<  ":  " <<  _nightRetrievalLocalZenithAngleBounds[0] << ",  " << _nightRetrievalLocalZenithAngleBounds[1] << endl;
  cout << prefix << "   " << QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS <<  ":  " << _quantitativeLocalZenithAngleBounds[0] << ",  " << _quantitativeLocalZenithAngleBounds[1] << endl;

  cout << prefix << "   " << DAY_SOLAR_ZENITH_ANGLE <<  ":  " << _daySolarZenithAngle << endl;
  cout << prefix << "   " << NIGHT_SOLAR_ZENITH_ANGLE <<  ":  " << _nightSolarZenithAngle << endl;
  cout << prefix << "   " << TWILIGHT_SOLAR_ZENITH_ANGLE <<  ":  " << _twilightSolarZenithAngle << endl;
  cout << prefix << "   " << DAY_ALGORITHM_SOLAR_ZENITH_ANGLE <<  ":  " << _dayAlgorithmSolarZenithAngle << endl;
  cout << prefix << "   " << NIGHT_ALGORITHM_SOLAR_ZENITH_ANGLE <<  ":  " << _nightAlgorithmSolarZenithAngle << endl;
  cout << prefix << "   " << DAY_SOLAR_ZENITH_ANGLE_BOUNDS <<  ":  " << _daySolarZenithAngleBounds[0] << ",  " << _daySolarZenithAngleBounds[1] << endl;
  cout << prefix << "   " << NIGHT_SOLAR_ZENITH_ANGLE_BOUNDS <<  ":  " << _nightSolarZenithAngleBounds[0] << ",  " << _nightSolarZenithAngleBounds[1] << endl;
  cout << prefix << "   " << TWILIGHT_SOLAR_ZENITH_ANGLE_BOUNDS <<  ":  " << _twilightSolarZenithAngleBounds[0] << ",  " << _twilightSolarZenithAngleBounds[1] << endl;
  cout << prefix << "   " << DAY_ALGORITHM_SOLAR_ZENITH_ANGLE_BOUNDS <<  ":  " << _dayAlgorithmSolarZenithAngleBounds[0] << ",  " << _dayAlgorithmSolarZenithAngleBounds[1] << endl;
  cout << prefix << "   " << NIGHT_ALGORITHM_SOLAR_ZENITH_ANGLE_BOUNDS <<  ":  " << _nightAlgorithmSolarZenithAngleBounds[0] << ",  " << _nightAlgorithmSolarZenithAngleBounds[1] << endl;
}


void CloudOpticalDepthProduct::_printQC(const std::string &prefix, const std::string &strarg)
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

void CloudOpticalDepthProduct::_readDimensions()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    _readVar(DAY_RETRIEVAL_LOCAL_ZENITH_ANGLE, _dayRetrievalLocalZenithAngle);
    _readVar(NIGHT_RETRIEVAL_LOCAL_ZENITH_ANGLE, _nightRetrievalLocalZenithAngle);
    _readVar(QUANTITATIVE_LOCAL_ZENITH_ANGLE, _quantitativeLocalZenithAngle);
    _readVar(DAY_SOLAR_ZENITH_ANGLE, _daySolarZenithAngle);
    _readVar(NIGHT_SOLAR_ZENITH_ANGLE, _nightSolarZenithAngle);
    _readVar(TWILIGHT_SOLAR_ZENITH_ANGLE, _twilightSolarZenithAngle);
    _readVar(DAY_ALGORITHM_SOLAR_ZENITH_ANGLE, _dayAlgorithmSolarZenithAngle);
    _readVar(NIGHT_ALGORITHM_SOLAR_ZENITH_ANGLE, _nightAlgorithmSolarZenithAngle);
    
    _readArray(NUMBER_OF_LZA_BOUNDS, DAY_RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS, _dayRetrievalLocalZenithAngleBounds);
    _readArray(NUMBER_OF_LZA_BOUNDS, NIGHT_RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS, _nightRetrievalLocalZenithAngleBounds);
    _readArray(NUMBER_OF_LZA_BOUNDS, QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS, _quantitativeLocalZenithAngleBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, DAY_SOLAR_ZENITH_ANGLE_BOUNDS, _daySolarZenithAngleBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, NIGHT_SOLAR_ZENITH_ANGLE_BOUNDS, _nightSolarZenithAngleBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, TWILIGHT_SOLAR_ZENITH_ANGLE_BOUNDS, _twilightSolarZenithAngleBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, DAY_ALGORITHM_SOLAR_ZENITH_ANGLE_BOUNDS, _dayAlgorithmSolarZenithAngleBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, NIGHT_ALGORITHM_SOLAR_ZENITH_ANGLE_BOUNDS, _nightAlgorithmSolarZenithAngleBounds);
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
 
void CloudOpticalDepthProduct::_readCoordinateVars()
{
   static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_readCoordinateVars();
}


void CloudOpticalDepthProduct::_readQualityControlVars()
{
   static const string methodName = _className + "::" + __FUNCTION__;


  try {
    NcxxVar var;
    _readQcDataVar(DATA_QUALITY_FLAG, _qcDataContainer, var, true);

    var.getAtt(PERCENT_DAY_ALGORITHM_PIXEL_QF).getValues(&_percentDayAlgorithmPixelQf);
    var.getAtt(PERCENT_NIGHT_ALGORITHM_PIXEL_QF).getValues(&_percentNightAlgorithmPixelQf);
    var.getAtt(PERCENT_GOOD_QUALITY_QF).getValues(&_percentGoodQualityQf);
    var.getAtt(PERCENT_DEGRADED_QUALITY_DUE_TO_SNOW_OR_SEA_ICE_QF).getValues(&_percentDegradedQualityDueToSnowOrSeaIceQf);
    var.getAtt(PERCENT_DEGRADED_QUALITY_DUE_TO_TWILIGHT_QF).getValues(&_percentDegradedQualityDueToTwilightQf);
    var.getAtt(PERCENT_INVALID_DUE_TO_CLEAR_CONDITIONS_QF).getValues(&_percentInvalidDueToClearConditionsQf);
    var.getAtt(PERCENT_INVALID_DUE_LZA_THRESHOLD_EXCEEDED_QF).getValues(&_percentInvalidDueLZAThresholdExceededQf);
    var.getAtt(PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF).getValues(&_percentDegradedDueToLZAThresholdExceededQf);
    var.getAtt(PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_QF).getValues(&_percentInvalidDueToNotGeolocatedQf);
    var.getAtt(PERCENT_INVALID_DUE_TO_MISSING_OR_BAD_INPUT_DATA_QF).getValues(&_percentInvalidDueToMissingOrBadInputDataQf);
    var.getAtt(PERCENT_DEGRADED_DUE_TO_NONCONVERGENCE_QF).getValues(&_percentDegradedDueToNonconvergenceQf);

     // create list (map) of containers
    _outQcDataFields[DATA_QUALITY_FLAG] = &_qcDataContainer;
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
  
void CloudOpticalDepthProduct::_readDataVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    NcxxVar var;
    _readDataVar(CLOUD_OPTICAL_DEPTH, _dataContainer, var);

    _readVar(MIN_COD_DAY, _minimuimCODDay);
    _readVar(MAX_COD_DAY, _maximuimCODDay);
    _readVar(MEAN_COD_DAY, _meanCODDay);
    _readVar(STD_DEV_COD_DAY, _stdDevCODDay);
    _readVar(MIN_COD_NIGHT, _minimuimCODNight);
    _readVar(MAX_COD_NIGHT, _maximuimCODNight);
    _readVar(MEAN_COD_NIGHT, _meanCODNight);
    _readVar(STD_DEV_COD_NIGHT, _stdDevCODNight);
    _readVar(COD_PRODUCT_WAVELENGTH, _codProductWavelength);
    _readVar(OUTLIER_COD_DAY, _outlierCODDay);
    _readVar(OUTLIER_COD_NIGHT, _outlierCODNight);

    // create list (map) of containers
    _outDataFields[CLOUD_OPTICAL_DEPTH] = &_dataContainer;
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
const char* CloudOpticalDepthProduct::CLOUD_OPTICAL_DEPTH = "COD";
const char* CloudOpticalDepthProduct::MIN_COD_DAY = "minimum_COD_day";
const char* CloudOpticalDepthProduct::MAX_COD_DAY = "maximum_COD_day";
const char* CloudOpticalDepthProduct::MEAN_COD_DAY = "mean_COD_day";
const char* CloudOpticalDepthProduct::STD_DEV_COD_DAY = "std_dev_COD_day";
const char* CloudOpticalDepthProduct::MIN_COD_NIGHT = "minimum_COD_night";
const char* CloudOpticalDepthProduct::MAX_COD_NIGHT = "maximum_COD_night";
const char* CloudOpticalDepthProduct::MEAN_COD_NIGHT = "mean_COD_night";
const char* CloudOpticalDepthProduct::STD_DEV_COD_NIGHT = "std_dev_COD_night";
const char* CloudOpticalDepthProduct::COD_PRODUCT_WAVELENGTH = "cod_product_wavelength";
const char* CloudOpticalDepthProduct::OUTLIER_COD_DAY = "outlier_COD_day";
const char* CloudOpticalDepthProduct::OUTLIER_COD_NIGHT = "outlier_COD_night";
const char* CloudOpticalDepthProduct::NUMBER_OF_QF_VALUES = " number_of_qf_values";
const char* CloudOpticalDepthProduct::PERCENT_DAY_ALGORITHM_PIXEL_QF = "percent_day_algorithm_pixel_qf";
const char* CloudOpticalDepthProduct::PERCENT_NIGHT_ALGORITHM_PIXEL_QF = "percent_night_algorithm_pixel_qf";
const char* CloudOpticalDepthProduct::PERCENT_GOOD_QUALITY_QF = "percent_good_quality_qf";
const char* CloudOpticalDepthProduct::PERCENT_DEGRADED_QUALITY_DUE_TO_SNOW_OR_SEA_ICE_QF = "percent_degraded_quality_due_to_snow_or_sea_ice_qf";
const char* CloudOpticalDepthProduct::PERCENT_DEGRADED_QUALITY_DUE_TO_TWILIGHT_QF = "percent_degraded_quality_due_to_twilight_qf";
const char* CloudOpticalDepthProduct::PERCENT_INVALID_DUE_TO_CLEAR_CONDITIONS_QF = "percent_invalid_due_to_clear_conditions_qf";
const char* CloudOpticalDepthProduct::PERCENT_INVALID_DUE_LZA_THRESHOLD_EXCEEDED_QF = "percent_invalid_due_LZA_threshold_exceeded_qf";
const char* CloudOpticalDepthProduct::PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF = "percent_degraded_due_to_LZA_threshold_exceeded_qf";
const char* CloudOpticalDepthProduct::PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_QF = "percent_invalid_due_to_not_geolocated_qf";
const char* CloudOpticalDepthProduct::PERCENT_INVALID_DUE_TO_MISSING_OR_BAD_INPUT_DATA_QF = "percent_invalid_due_to_missing_or_bad_input_data_qf";
const char* CloudOpticalDepthProduct::PERCENT_DEGRADED_DUE_TO_NONCONVERGENCE_QF = "percent_degraded_due_to_nonconvergence_qf";
