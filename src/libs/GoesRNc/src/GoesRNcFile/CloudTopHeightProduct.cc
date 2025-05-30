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
 * @file CloudTopHeightProduct.cc
 *
 * @class CloudTopHeightProduct
 *
 * CloudTopHeightProduct is a derived class for the cloud top height
 * Level-2 GOES-R product, based on GoesRProduct class.
 *  
 * @date 16 May 2019
 *
 */



#include <GoesRNc/CloudTopHeightProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

CloudTopHeightProduct::CloudTopHeightProduct()
{
  _badPixelCount = 0;
  _passBadCountTest = true;
}

CloudTopHeightProduct::CloudTopHeightProduct(const CloudTopHeightProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

CloudTopHeightProduct::~CloudTopHeightProduct()
{
}

bool CloudTopHeightProduct::applyQcTests()
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

void CloudTopHeightProduct::clearAndReset()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::clearAndReset();

  _badPixelCount = 0;
  _passBadCountTest = true;
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void CloudTopHeightProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printVars(prefix, strarg);

  cout << prefix << "Cloud Top Height Vars: " << endl;
  cout << prefix << "   " << MIN_CLOUD_TOP_HEIGHT << ":  " << _dataContainer.minValue << endl;
  cout << prefix << "   " << MAX_CLOUD_TOP_HEIGHT << ":  " << _dataContainer.maxValue << endl;
  cout << prefix << "   " << MEAN_CLOUD_TOP_HEIGHT << ":  " << _dataContainer.meanValue << endl;
  cout << prefix << "   " << STD_DEV_CLOUD_TOP_HEIGHT << ":  " << _dataContainer.stdDevValue << endl;
  cout << prefix << "   " << OUTLIER_PIXELS << ":  " << _outlierPixels << endl;
  cout << prefix << "   " << CLOUD_PIXELS << ":  " << _cloudPixels << endl;
}


void CloudTopHeightProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printAttrs(prefix, strarg);

  _printDataAttrs(prefix, "Cloud Top Height", _dataContainer);
  _printQcDataAttrs(prefix, "Cloud Top Height Quality", _qcDataContainer);

  cout << prefix << "   " << PERCENT_GOOD_QUALITY_QF << ":  " << _percentGoodQualityQF << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_QF << ":  " << _percentInvalidDueToNotGeolocatedQf << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF << ":  " << _percentInvalidDueToLZAThresholdExceededQf << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_BRIGHTNESS_TEMP_DATA_QF << ":  " << _percentInvalidDueToBadOrMissingBrightnessTempDataQf << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_CLEAR_OR_PROBABLY_CLEAR_SKY_QF << ":  " << _percentInvalidDueToClearOrProbablyClearSkyQf << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_UNKNOWN_CLOUD_TYPE_QF << ":  " << _percentInvalidDueToUnknownCloudTypeQf << endl;
  cout << prefix << "   " << PERCENT_INVALID_DUE_TO_NONCONVERGENT_RETRIEVAL_QF << ":  " << _percentInvalidDueToNonconvergentRetrievalQf << endl;

}


void CloudTopHeightProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printDims(prefix, strarg);

  cout << prefix << "   " << LOCAL_ZENITH_ANGLE_BOUNDS <<  ":  " <<  _localZenithAngleBounds[0] << ",  " << _localZenithAngleBounds[1] << endl;
  cout << prefix << "   " << SOLAR_ZENITH_ANGLE_BOUNDS <<  ":  " << _solarZenithAngleBounds[0] << ",  " << _solarZenithAngleBounds[1] << endl;
}


void CloudTopHeightProduct::_printQC(const std::string &prefix, const std::string &strarg)
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

void CloudTopHeightProduct::_readDimensions()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    _readVar(LOCAL_ZENITH_ANGLE, _localZenithAngle);
    _readVar(SOLAR_ZENITH_ANGLE, _solarZenithAngle);
    _readArray(NUMBER_OF_LZA_BOUNDS, LOCAL_ZENITH_ANGLE_BOUNDS,
               _localZenithAngleBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, SOLAR_ZENITH_ANGLE_BOUNDS,
               _solarZenithAngleBounds);
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

void CloudTopHeightProduct::_readCoordinateVars()
{
  GoesRProduct::_readCoordinateVars();
}


void CloudTopHeightProduct::_readQualityControlVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    NcxxVar var;
    _readQcDataVar(DATA_QUALITY_FLAG, _qcDataContainer, var);

    var.getAtt(PERCENT_GOOD_QUALITY_QF).getValues(&_percentGoodQualityQF);
    var.getAtt(PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_QF).getValues(&_percentInvalidDueToNotGeolocatedQf);
    var.getAtt(PERCENT_INVALID_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF).getValues(&_percentInvalidDueToLZAThresholdExceededQf);
    var.getAtt(PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_BRIGHTNESS_TEMP_DATA_QF).getValues(&_percentInvalidDueToBadOrMissingBrightnessTempDataQf);
    var.getAtt(PERCENT_INVALID_DUE_TO_CLEAR_OR_PROBABLY_CLEAR_SKY_QF).getValues(&_percentInvalidDueToClearOrProbablyClearSkyQf);
    var.getAtt(PERCENT_INVALID_DUE_TO_UNKNOWN_CLOUD_TYPE_QF).getValues(&_percentInvalidDueToUnknownCloudTypeQf);
    var.getAtt(PERCENT_INVALID_DUE_TO_NONCONVERGENT_RETRIEVAL_QF).getValues(&_percentInvalidDueToNonconvergentRetrievalQf);

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

void CloudTopHeightProduct::_readDataVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    NcxxVar var;
    _readDataVar(CLOUD_TOP_HEIGHT, _dataContainer, var);

    _readVar(CLOUD_PIXELS, _cloudPixels);
    _readVar(OUTLIER_PIXELS, _outlierPixels);
    _readVar(MIN_CLOUD_TOP_HEIGHT, _dataContainer.minValue);
    _readVar(MAX_CLOUD_TOP_HEIGHT, _dataContainer.maxValue);
    _readVar(MEAN_CLOUD_TOP_HEIGHT, _dataContainer.meanValue);
    _readVar(STD_DEV_CLOUD_TOP_HEIGHT, _dataContainer.stdDevValue);

    // create list (map) of containers
    _outDataFields[CLOUD_TOP_HEIGHT] = &_dataContainer;
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
const char* CloudTopHeightProduct::CLOUD_TOP_HEIGHT = "HT";
const char* CloudTopHeightProduct::MIN_CLOUD_TOP_HEIGHT = "minimum_cloud_top_height";
const char* CloudTopHeightProduct::MAX_CLOUD_TOP_HEIGHT = "maximum_cloud_top_height";
const char* CloudTopHeightProduct::MEAN_CLOUD_TOP_HEIGHT = "mean_cloud_top_height";
const char* CloudTopHeightProduct::STD_DEV_CLOUD_TOP_HEIGHT = "std_dev_cloud_top_height";
const char* CloudTopHeightProduct::PERCENT_GOOD_QUALITY_QF = "percent_good_quality_qf";
const char* CloudTopHeightProduct::PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_QF = "percent_invalid_due_to_not_geolocated_qf";
const char* CloudTopHeightProduct::PERCENT_INVALID_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF = "percent_invalid_due_to_LZA_threshold_exceeded_qf";
const char* CloudTopHeightProduct::PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_BRIGHTNESS_TEMP_DATA_QF = "percent_invalid_due_to_bad_or_missing_brightness_temp_data_qf";
const char* CloudTopHeightProduct::PERCENT_INVALID_DUE_TO_CLEAR_OR_PROBABLY_CLEAR_SKY_QF = "percent_invalid_due_to_clear_or_probably_clear_sky_qf";
const char* CloudTopHeightProduct::PERCENT_INVALID_DUE_TO_UNKNOWN_CLOUD_TYPE_QF = "percent_invalid_due_to_unknown_cloud_type_qf";
const char* CloudTopHeightProduct::PERCENT_INVALID_DUE_TO_NONCONVERGENT_RETRIEVAL_QF = "percent_invalid_due_to_nonconvergent_retrieval_qf";


