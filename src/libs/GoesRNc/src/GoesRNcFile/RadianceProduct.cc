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
 * @file RadianceProduct.cc
 *
 * @class RadianceProduct
 *
 * RadianceProduct is a derived class for the 16 imager channels
 * GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */


#include <cmath>

#include <GoesRNc/RadianceProduct.hh>

using namespace std;

const int RadianceProduct::EMISSIVE_BAND_START = 7;
const double RadianceProduct::MIN_RADIANCE = 0.00001;

/*********************************************************************
 * Constructor
 */

RadianceProduct::RadianceProduct()
{
  _badPixelCount = 0;
  _passMissingPixelTest = true;
  _passSaturatedPixelTest = true;
  _passUndersaturatedPixelTest = true;
  _passFocalPlaneCountTest = true;
  _passFocalPlaneMaxTempTest = true;
  _passBadCountTest = true;
 
}

RadianceProduct::RadianceProduct(const RadianceProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

RadianceProduct::~RadianceProduct()
{
  
}

bool RadianceProduct::applyQcTests()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  bool testRecord = true;

  size_t numPts = _numX*_numY;

  cerr << GoesRProduct::INFO_STR << methodName << endl;
  cerr << "    Applying QC tests" << endl;

  // check thresholds from parameter file
  if ((_missingPixel.run == true) &&
      (100.0*(_missingPixelCount/numPts) > _missingPixel.threshold)) {
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << ERROR_STR << methodName << endl;
      cerr << "  Missing pixel count exceeded threshold." << endl;
    }
    _passMissingPixelTest = false;
    testRecord = false;
  }

  if ((_saturatedPixel.run == true) &&
      (100.0*(_saturatedPixelCount/numPts) > _saturatedPixel.threshold)) {
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << ERROR_STR << methodName << endl;
      cerr << "  Saturated pixel count exceeded threshold." << endl;
    }
    _passSaturatedPixelTest = false;
    testRecord = false;
  }

  if ((_undersaturatedPixel.run == true) &&
      (100.0*(_undersaturatedPixelCount/numPts) > _undersaturatedPixel.threshold)) {
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << ERROR_STR << methodName << endl;
      cerr << "  Undersaturated pixel count exceeded threshold." << endl;
    }
    _passUndersaturatedPixelTest = false;
    testRecord = false;
  }

  if ((_focalPlaneTempCount.run == true) &&
      (100.0*(_focalPlaneTempThreshExceedCount/numPts) > _focalPlaneTempCount.threshold)) {
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << ERROR_STR << methodName << endl;
      cerr << "  Focal plane temperature count exceeded threshold." << endl;
    }
    _passFocalPlaneCountTest = false;
    testRecord = false;
  }

  if ((_focalPlaneMaxTemp.run == true) &&
      (_maxFocalPlaneTemp > _focalPlaneMaxTemp.threshold)) {
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << ERROR_STR << methodName << endl;
      cerr << "  Focal plane max temperature exceeded threshold." << endl;
    }
    _passFocalPlaneMaxTempTest = false;
    testRecord = false;
  }


  // compare against DQF flags as specified in the parameter file
  for(size_t i = 0; i < numPts; i++) {

    if(_qcDataContainer.values[i] == DATA_QUALITY_GOOD) {

      if (_clipNegativeRadiance == true) {
        _dataContainer.values[i] = max(0.0f, _dataContainer.values[i]);
      }

      continue;
    }

    if(_checkQualityField == true) {

      for(size_t j = 0; j < _checkQualityList.size(); j++) {
        if(_qcDataContainer.values[i] == _checkQualityList[j]) {
          _dataContainer.values[i] = BAD_DATA_VALUE;
          _badPixelCount++;
          break;
        }
      }

    }
  }

  if ((_badPixel.run == true) &&
      (100.0*(_badPixelCount/numPts) > _badPixel.threshold)) {
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

void RadianceProduct::clearAndReset()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::clearAndReset();

  _badPixelCount = 0;
  _passMissingPixelTest = true;
  _passSaturatedPixelTest = true;
  _passUndersaturatedPixelTest = true;
  _passFocalPlaneCountTest = true;
  _passFocalPlaneMaxTempTest = true;
  _passBadCountTest = true;

}

double RadianceProduct::toAlbedo(double rad)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  double albedo = rad;

  if(rad != MISSING_DATA_VALUE) {

    albedo = rad * _kappa0; // use convert units to go to percent
  }
  
  return albedo;
}


double RadianceProduct::toBrightnessTemp(double rad)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  
  double brightTemp = BAD_DATA_VALUE;

  if ((min(fabs(rad), MIN_RADIANCE) >= MIN_RADIANCE) && (rad != MISSING_DATA_VALUE)) {

    brightTemp = (_planckFk2/log((_planckFk1/rad) + 1.0) - _planckBc1)/_planckBc2;

  }
  
  return brightTemp;
}




/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void RadianceProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printVars(prefix, strarg);  

  cout << prefix << "Radiance:" << endl;
  cout << prefix << "   " << BAND_ID << ":  " << _bandID << endl;
  cout << prefix << "   " << BAND_WAVELENGTH << ":  " << _bandWavelength << endl;
  cout << prefix << "   " << YAW_FLIP_FLAG << ":  " << _yawFlipFlag << endl;
  cout << prefix << "   " << MIN_RADIANCE_VALUE_OF_VALID_PIXELS << ":  "
       << _dataContainer.minValue << endl;
  cout << prefix << "   " << MAX_RADIANCE_VALUE_OF_VALID_PIXELS << ":  "
       << _dataContainer.maxValue << endl;
  cout << prefix << "   " << MEAN_RADIANCE_VALUE_OF_VALID_PIXELS << ":  "
       << _dataContainer.meanValue << endl;
  cout << prefix << "   " << STD_DEV_RADIANCE_VALUE_OF_VALID_PIXELS << ":  "
       << _dataContainer.stdDevValue << endl;

  cout << prefix << "Radiance constants:" << endl;
  cout << prefix << "   " << ESUN << ":  " << _eSun << endl;
  cout << prefix << "   " << KAPPA0 << ":  " << _kappa0 << endl;
  cout << prefix << "   " << PLANCK_FK1 << ":  " << _planckFk1 << endl;
  cout << prefix << "   " << PLANCK_FK2 << ":  " << _planckFk2 << endl;
  cout << prefix << "   " << PLANCK_BC1 << ":  " << _planckBc1 << endl;
  cout << prefix << "   " << PLANCK_BC2 << ":  " << _planckBc2 << endl;
  
  cout << prefix << "Quality control:" << endl;

  cout << prefix << "   " << VALID_PIXEL_COUNT << ":  " << _validPixelCount << endl;
  cout << prefix << "   " << MISSING_PIXEL_COUNT << ":  "
       << _missingPixelCount << endl;
  cout << prefix << "   " << SATURATED_PIXEL_COUNT << ":  "
       << _saturatedPixelCount << endl;
  cout << prefix << "   " << UNDERSATURATED_PIXEL_COUNT << ":  "
       << _undersaturatedPixelCount << endl;
  cout << prefix << "   " << FOCAL_PLANE_TEMPERATURE_THRESHOLD_EXCEEDED_COUNT
       << ":  " << _focalPlaneTempThreshExceedCount << endl;
  cout << prefix << "   " << MAXIMUM_FOCAL_PLANE_TEMPERATURE << ":  "
       << _maxFocalPlaneTemp << endl;
  cout << prefix << "   " << FOCAL_PLANE_TEMPERATURE_THRESHOLD_INCREASING << ":  "
       << _focalPlaneTemperatureThresholdIncr << endl;
  cout << prefix << "   " << FOCAL_PLANE_TEMPERATURE_THRESHOLD_DECREASING << ":  "
       << _focalPlaneTemperatureThresholdDecr << endl;

  cout << prefix << "   " << PERCENT_UNCORRECTABLE_L0_ERRORS << ":  "
       << _percentUncorrectableL0Errors << endl;
  cout << prefix << "   " << EARTH_SUN_DISTANCE_ANOMALY_IN_AU << ":  "
       << _earthSunDistAnomalyAU << endl;

  cout << prefix << "   " << INPUT_ABI_L0_DATA << ":  " << _inputAbiL0Data << endl;
  cout << prefix << "   " << L1B_PROCESSING_PARM_VERSION << ":  "
       << _l1bProcessingParamVersion << endl;
  cout << prefix << "   " << ALGORITHM_VERSION << ":  " << _algorithmVersion << endl;
  cout << prefix << "   " << PRODUCT_VERSION << ":  " << _algoProductVersion << endl;

}


void RadianceProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printAttrs(prefix, strarg);

  _printDataAttrs(prefix, "Radiance", _dataContainer);

  _printQcDataAttrs(prefix, "Radiance Quality", _qcDataContainer);

  cout << prefix << "   " << PERCENT_GOOD_PIXEL_QF << ":  " << _percentGoodPixels << endl;
  cout << prefix << "   " << PERCENT_CONDITIONALLY_USABLE_PIXEL_QF << ":  " << _percentCondUsabePixels << endl;
  cout << prefix << "   " << PERCENT_OUT_OF_RANGE_PIXEL_QF << ":  " << _percentOutOfRangePixels << endl;
  cout << prefix << "   " << PERCENT_NO_VALUE_PIXEL_QF << ":  " << _percentNoValuePixels << endl;
  cout << prefix << "   " << PERCENT_FOCAL_PLANE_TEMPERATURE_THRESHOLD_EXCEEDED_QF << ":  " << _percentFocalPlaneTempExceedPixels << endl;

}


void RadianceProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printDims(prefix, strarg);   
  cout << prefix << "   " << BAND_DIM <<  ":  " << _numBands << endl;
}

void RadianceProduct::_printQC(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  cout << prefix << "QC test info" << endl;
  cout << prefix << "  Missing pixel test:" << endl << prefix;
  if(_missingPixel.run == true) {
    if(_passMissingPixelTest == false) {
      cout << "      Failed ";
    }
    else {
      cout << "      Passed ";
    }
    cout << "test --  count: " << _missingPixelCount << "  threshold: "
	 << _missingPixel.threshold << endl;
  }
  else {
    cout << "      No test" << endl;
  }
  
  cout << prefix << "  Saturated pixel test" << endl << prefix;
  if(_saturatedPixel.run == true) {
    if(_passSaturatedPixelTest == false) {
      cout << "      Failed ";
    }
    else {
      cout << "      Passed ";
    }
    cout << "test --  count: " << _saturatedPixelCount << "  threshold: "
	 << _saturatedPixel.threshold << endl;
  }
  else {
    cout << "      No test" << endl;
  }


  cout << prefix << "  Undersaturated pixel test" << endl << prefix;
  if(_undersaturatedPixel.run == true) {
    if(_passUndersaturatedPixelTest == false) {
      cout << "      Failed ";
    }
    else {
      cout << "      Passed ";
    }
    cout << "test --  count: " << _undersaturatedPixelCount << "  threshold: "
	 << _undersaturatedPixel.threshold << endl;
  }
  else {
    cout << "      No test" << endl;
  }


  cout << prefix << "  Focal plane temperature count test" << endl << prefix;
  if(_focalPlaneTempCount.run == true) {
    if(_passFocalPlaneCountTest == false) {
      cout << "      Failed ";
    }
    else {
      cout << "      Passed ";
    }
    cout << "test --  count: " << _focalPlaneTempThreshExceedCount << "  threshold: "
	 << _focalPlaneTempCount.threshold << endl;
  }
  else {
    cout << "      No test" << endl;
  }

  cout << prefix << "  Focal plane maximum temperature test" << endl << prefix;
  if(_focalPlaneMaxTemp.run == true) {
    if(_passFocalPlaneMaxTempTest == false) {
      cout << "      Failed ";
    }
    else {
      cout << "      Passed ";
    }
    cout << "test --  temperature: " << _maxFocalPlaneTemp << "  threshold: "
	 << _focalPlaneMaxTemp.threshold << endl;
  }
  else {
    cout << "      No test" << endl;
  }

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
    
  if(_clipNegativeRadiance == true) {
    cout << prefix << "Clippied negative radiances at 0." << endl;
  }
  else {
    cout << prefix << "No clipping of radiance data." << endl;
  }

 
}


void RadianceProduct::_readDimensions()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  try {
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
  catch  (NcxxException e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }


}
 
void RadianceProduct::_readCoordinateVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_readCoordinateVars();
}

void RadianceProduct::_readConstantsVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    _readVar(ESUN, _eSun);
    _readVar(KAPPA0, _kappa0);
    _readVar(PLANCK_FK1, _planckFk1);
    _readVar(PLANCK_FK2, _planckFk2);
    _readVar(PLANCK_BC1, _planckBc1);
    _readVar(PLANCK_BC2, _planckBc2);
  }
  catch(NcxxException e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
}
  

void RadianceProduct::_readQualityControlVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    NcxxVar var;
    _readQcDataVar(DATA_QUALITY_FLAG, _qcDataContainer, var);
    
    var.getAtt(PERCENT_GOOD_PIXEL_QF).getValues(&_percentGoodPixels);
    var.getAtt(PERCENT_CONDITIONALLY_USABLE_PIXEL_QF).getValues(&_percentCondUsabePixels);
    var.getAtt(PERCENT_OUT_OF_RANGE_PIXEL_QF).getValues(&_percentOutOfRangePixels);
    var.getAtt(PERCENT_NO_VALUE_PIXEL_QF).getValues(&_percentNoValuePixels);
    if(_heatPipeVars == true) {
      var.getAtt(PERCENT_FOCAL_PLANE_TEMPERATURE_THRESHOLD_EXCEEDED_QF).getValues(&_percentFocalPlaneTempExceedPixels);
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
  
void RadianceProduct::_readDataVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {

    NcxxVar var;
    _readDataVar(RADIANCE, _dataContainer, var);
    var.getAtt(SENSOR_BAND_BIT_DEPTH).getValues(&_sensorBitDepth);

    _readVar(VALID_PIXEL_COUNT, _validPixelCount);
    _readVar(MISSING_PIXEL_COUNT, _missingPixelCount);
    _readVar(SATURATED_PIXEL_COUNT, _saturatedPixelCount);
    _readVar(UNDERSATURATED_PIXEL_COUNT, _undersaturatedPixelCount);
    _readVar(MIN_RADIANCE_VALUE_OF_VALID_PIXELS, _dataContainer.minValue);
    _readVar(MAX_RADIANCE_VALUE_OF_VALID_PIXELS, _dataContainer.maxValue);
    _readVar(MEAN_RADIANCE_VALUE_OF_VALID_PIXELS, _dataContainer.meanValue);
    _readVar(STD_DEV_RADIANCE_VALUE_OF_VALID_PIXELS, _dataContainer.stdDevValue);
    _readVar(PERCENT_UNCORRECTABLE_L0_ERRORS, _percentUncorrectableL0Errors);
    _readVar(EARTH_SUN_DISTANCE_ANOMALY_IN_AU, _earthSunDistAnomalyAU);
    _readVar(BAND_ID, _bandID);
    _readVar(BAND_WAVELENGTH, _bandWavelength);
    if(_heatPipeVars == true) {
      _readVar(MAXIMUM_FOCAL_PLANE_TEMPERATURE, _maxFocalPlaneTemp);
      _readVar(FOCAL_PLANE_TEMPERATURE_THRESHOLD_INCREASING, _focalPlaneTemperatureThresholdIncr);
      _readVar(FOCAL_PLANE_TEMPERATURE_THRESHOLD_DECREASING, _focalPlaneTemperatureThresholdDecr);
    }
    uint8_t val;
    _readVar(YAW_FLIP_FLAG, val);
    _yawFlipFlag = static_cast< bool >(val);
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

const char* RadianceProduct::RADIANCE = "Rad";
const char* RadianceProduct::YAW_FLIP_FLAG = "yaw_flip_flag";
const char* RadianceProduct::BAND_ID = "band_id";
const char* RadianceProduct::BAND_WAVELENGTH = "band_wavelength";
const char* RadianceProduct::ESUN = "esun";
const char* RadianceProduct::KAPPA0 = "kappa0";
const char* RadianceProduct::PLANCK_FK1 = "planck_fk1";
const char* RadianceProduct::PLANCK_FK2 = "planck_fk2";
const char* RadianceProduct::PLANCK_BC1 = "planck_bc1";
const char* RadianceProduct::PLANCK_BC2 = "planck_bc2";
const char* RadianceProduct::VALID_PIXEL_COUNT = "valid_pixel_count";
const char* RadianceProduct::MISSING_PIXEL_COUNT = "missing_pixel_count";
const char* RadianceProduct::SATURATED_PIXEL_COUNT = "saturated_pixel_count";
const char* RadianceProduct::UNDERSATURATED_PIXEL_COUNT =
  "undersaturated_pixel_count";
const char* RadianceProduct::FOCAL_PLANE_TEMPERATURE_THRESHOLD_EXCEEDED_COUNT = "focal_plane_temperature_threshold_exceeded_count";

const char* RadianceProduct::MIN_RADIANCE_VALUE_OF_VALID_PIXELS =
  "min_radiance_value_of_valid_pixels";
const char* RadianceProduct::MAX_RADIANCE_VALUE_OF_VALID_PIXELS =
  "max_radiance_value_of_valid_pixels";
const char* RadianceProduct::MEAN_RADIANCE_VALUE_OF_VALID_PIXELS =
  "mean_radiance_value_of_valid_pixels";
const char* RadianceProduct::STD_DEV_RADIANCE_VALUE_OF_VALID_PIXELS =
  "std_dev_radiance_value_of_valid_pixels";
const char* RadianceProduct::MAXIMUM_FOCAL_PLANE_TEMPERATURE = "maximum_focal_plane_temperature";
const char* RadianceProduct::FOCAL_PLANE_TEMPERATURE_THRESHOLD_INCREASING = "focal_plane_temperature_threshold_increasing";
const char* RadianceProduct::FOCAL_PLANE_TEMPERATURE_THRESHOLD_DECREASING = "focal_plane_temperature_threshold_decreasing";
const char* RadianceProduct::EARTH_SUN_DISTANCE_ANOMALY_IN_AU =
  "earth_sun_distance_anomaly_in_AU";
const char* RadianceProduct::T_STAR_LOOK = "t_star_look";
const char* RadianceProduct::BAND_WAVELENGTH_STAR_LOOK =
  "band_wavelength_star_look";
const char* RadianceProduct::STAR_ID = "star_id";
const char* RadianceProduct::PERCENT_UNCORRECTABLE_L0_ERRORS =
  "percent_uncorrectable_L0_errors";

// observation and data quality flag Variable Attributes
const char* RadianceProduct::SENSOR_BAND_BIT_DEPTH = "sensor_band_bit_depth";
const char* RadianceProduct::NUMBER_OF_QF_VALUES = "number_of_qf_values";
const char* RadianceProduct::PERCENT_GOOD_PIXEL_QF = "percent_good_pixel_qf";
const char* RadianceProduct::PERCENT_CONDITIONALLY_USABLE_PIXEL_QF =
  "percent_conditionally_usable_pixel_qf";
const char* RadianceProduct::PERCENT_OUT_OF_RANGE_PIXEL_QF =
  "percent_out_of_range_pixel_qf";
const char* RadianceProduct::PERCENT_NO_VALUE_PIXEL_QF =
  "percent_no_value_pixel_qf";
const char* RadianceProduct::PERCENT_FOCAL_PLANE_TEMPERATURE_THRESHOLD_EXCEEDED_QF =
  "percent_focal_plane_temperature_threshold_exceeded_qf";
const char* RadianceProduct::SWEEP_ANGLE_AXIS = "sweep_angle_axis";
