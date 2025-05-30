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
 * @file CloudParticleSizeProduct.cc
 *
 * @class CloudParticleSizeProduct
 *
 * CloudParticleSizeProduct is a derived class for the cloud particle
 * size Level-2 GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/CloudParticleSizeProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

CloudParticleSizeProduct::CloudParticleSizeProduct()
{
  _badPixelCount = 0;
  _passBadCountTest = true;
}

CloudParticleSizeProduct::CloudParticleSizeProduct(const CloudParticleSizeProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

CloudParticleSizeProduct::~CloudParticleSizeProduct()
{
}

bool CloudParticleSizeProduct::applyQcTests()
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

void CloudParticleSizeProduct::clearAndReset()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::clearAndReset();

  _badPixelCount = 0;
  _passBadCountTest = true;
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void CloudParticleSizeProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printVars(prefix, strarg);  
   cout << prefix << "Cloud Particle Size Vars: " << endl;
   cout << prefix << "   " << MIN_PSD_DAY << ":  " << _minimuimPSDDay << endl;
   cout << prefix << "   " << MAX_PSD_DAY << ":  " << _maximuimPSDDay << endl;
   cout << prefix << "   " << MEAN_PSD_DAY << ":  " << _meanPSDDay << endl;
   cout << prefix << "   " << STD_DEV_PSD_DAY << ":  " << _stdDevPSDDay << endl;
   cout << prefix << "   " << MIN_PSD_NIGHT << ":  " << _minimuimPSDNight << endl;
   cout << prefix << "   " << MAX_PSD_NIGHT << ":  " << _maximuimPSDNight << endl;
   cout << prefix << "   " << MEAN_PSD_NIGHT << ":  " << _meanPSDNight << endl;
   cout << prefix << "   " << STD_DEV_PSD_NIGHT << ":  " << _stdDevPSDNight << endl;
   cout << prefix << "   " << OUTLIER_PSD_DAY << ":  " << _outlierPSDDay << endl;
   cout << prefix << "   " << OUTLIER_PSD_NIGHT << ":  " << _outlierPSDNight << endl;
}


void CloudParticleSizeProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printAttrs(prefix, strarg);

  _printDataAttrs(prefix, "Cloud Particle Size", _dataContainer);

  _printQcDataAttrs(prefix, "Cloud Particle Size Quality", _qcDataContainer, true);
  
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


void CloudParticleSizeProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printDims(prefix, strarg);   

  cout << prefix << "   " << BAND_DIM <<  ":  " << _numBands << endl;

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


void CloudParticleSizeProduct::_printQC(const std::string &prefix, const std::string &strarg)
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

void CloudParticleSizeProduct::_readDimensions()
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
 
void CloudParticleSizeProduct::_readCoordinateVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_readCoordinateVars();
}


void CloudParticleSizeProduct::_readQualityControlVars()
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
  
void CloudParticleSizeProduct::_readDataVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    NcxxVar var;
    _readDataVar(CLOUD_PARTICLE_SIZE, _dataContainer, var);

    _readVar(MIN_PSD_DAY, _minimuimPSDDay);
    _readVar(MAX_PSD_DAY, _maximuimPSDDay);
    _readVar(MEAN_PSD_DAY, _meanPSDDay);
    _readVar(STD_DEV_PSD_DAY, _stdDevPSDDay);
    _readVar(MIN_PSD_NIGHT, _minimuimPSDNight);
    _readVar(MAX_PSD_NIGHT, _maximuimPSDNight);
    _readVar(MEAN_PSD_NIGHT, _meanPSDNight);
    _readVar(STD_DEV_PSD_NIGHT, _stdDevPSDNight);
    _readVar(OUTLIER_PSD_DAY, _outlierPSDDay);
    _readVar(OUTLIER_PSD_NIGHT, _outlierPSDNight);

    // create list (map) of containers
    _outDataFields[CLOUD_PARTICLE_SIZE] = &_dataContainer;
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
const char* CloudParticleSizeProduct::CLOUD_PARTICLE_SIZE = "PSD";
const char* CloudParticleSizeProduct::OUTLIER_PSD_DAY = "outlier_PSD_day";
const char* CloudParticleSizeProduct::OUTLIER_PSD_NIGHT = "outlier_PSD_night";
const char* CloudParticleSizeProduct::MIN_PSD_DAY = "minimum_PSD_day";
const char* CloudParticleSizeProduct::MAX_PSD_DAY = "maximum_PSD_day";
const char* CloudParticleSizeProduct::MEAN_PSD_DAY = "mean_PSD_day";
const char* CloudParticleSizeProduct::STD_DEV_PSD_DAY = "std_dev_PSD_day";
const char* CloudParticleSizeProduct::MIN_PSD_NIGHT = "minimum_PSD_night";
const char* CloudParticleSizeProduct::MAX_PSD_NIGHT = "maximum_PSD_night";
const char* CloudParticleSizeProduct::MEAN_PSD_NIGHT = "mean_PSD_night";
const char* CloudParticleSizeProduct::STD_DEV_PSD_NIGHT = "std_dev_PSD_night";
const char* CloudParticleSizeProduct::NUMBER_OF_QF_VALUES = " number_of_qf_values";
const char* CloudParticleSizeProduct::PERCENT_DAY_ALGORITHM_PIXEL_QF = "percent_day_algorithm_pixel_qf";
const char* CloudParticleSizeProduct::PERCENT_NIGHT_ALGORITHM_PIXEL_QF = "percent_night_algorithm_pixel_qf";
const char* CloudParticleSizeProduct::PERCENT_GOOD_QUALITY_QF = "percent_good_quality_qf";
const char* CloudParticleSizeProduct::PERCENT_DEGRADED_QUALITY_DUE_TO_SNOW_OR_SEA_ICE_QF = "percent_degraded_quality_due_to_snow_or_sea_ice_qf";
const char* CloudParticleSizeProduct::PERCENT_DEGRADED_QUALITY_DUE_TO_TWILIGHT_QF = "percent_degraded_quality_due_to_twilight_qf";
const char* CloudParticleSizeProduct::PERCENT_INVALID_DUE_TO_CLEAR_CONDITIONS_QF = "percent_invalid_due_to_clear_conditions_qf";
const char* CloudParticleSizeProduct::PERCENT_INVALID_DUE_LZA_THRESHOLD_EXCEEDED_QF = "percent_invalid_due_LZA_threshold_exceeded_qf";
const char* CloudParticleSizeProduct::PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF = "percent_degraded_due_to_LZA_threshold_exceeded_qf";
const char* CloudParticleSizeProduct::PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_QF = "percent_invalid_due_to_not_geolocated_qf";
const char* CloudParticleSizeProduct::PERCENT_INVALID_DUE_TO_MISSING_OR_BAD_INPUT_DATA_QF = "percent_invalid_due_to_missing_or_bad_input_data_qf";
const char* CloudParticleSizeProduct::PERCENT_DEGRADED_DUE_TO_NONCONVERGENCE_QF = "percent_degraded_due_to_nonconvergence_qf";
