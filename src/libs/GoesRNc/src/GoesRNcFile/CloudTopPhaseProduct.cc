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
 * @file CloudTopPhaseProduct.cc
 *
 * @class CloudTopPhaseProduct
 *
 * CloudTopPhaseProduct is a derived class for the cloud top phase
 * Level-2 GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/CloudTopPhaseProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

CloudTopPhaseProduct::CloudTopPhaseProduct()
{
  _badPixelCount = 0;
  _passBadCountTest = true;
  _isCatData = true;
}

CloudTopPhaseProduct::CloudTopPhaseProduct(const CloudTopPhaseProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

CloudTopPhaseProduct::~CloudTopPhaseProduct()
{
}

bool CloudTopPhaseProduct::applyQcTests()
{
 static const string methodName = _className + "::" + __FUNCTION__;

  bool testRecord = true;
  
  for(size_t i = 0; i < _numX*_numY; i++) {

    if(_catDataContainer.values[i] == DATA_QUALITY_GOOD) { 
      continue;
    }

    if(_checkQualityField == true) {
      
      _qcDataContainer.values[i] = BAD_DATA_VALUE;
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

void CloudTopPhaseProduct::clearAndReset()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::clearAndReset();

  _badPixelCount = 0;
  _passBadCountTest = true;
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void CloudTopPhaseProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printVars(prefix, strarg);  

  cout << prefix << "Cloud Top Phase Vars: " << endl;
  cout << prefix << "   " << TOTAL_NUMBER_CLOUDY_PIXELS << ":  " << _totalNumberCloudyPixels << endl;
}


void CloudTopPhaseProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printAttrs(prefix, strarg);

  _printCatDataAttrs(prefix, "Cloud Top Phase", _catDataContainer);
  cout << prefix << "   " << NUMBER_OF_CLOUD_TOP_PHASE_CATEGORY_VALUES << ":  " << _numberOfCloudTopPhaseCategoryValues << endl;
  cout << prefix << "   " << PERCENT_CLEAR_SKY << ":  " << _percentClearSky << endl;
  cout << prefix << "   " << PERCENT_LIQUID_WATER << ":  " << _percentLiquidWater << endl;
  cout << prefix << "   " << PERCENT_SUPER_COOLED_LIQUID_WATER << ":  " << _percentSuperCooledLiquidWater << endl;
  cout << prefix << "   " << PERCENT_MIXED_PHASE << ":  " << _percentMixedPhase << endl;
  cout << prefix << "   " << PERCENT_ICE << ":  " << _percentIce << endl;
  cout << prefix << "   " << PERCENT_UNKNOWN << ":  " << _percentUnknown << endl;

  _printQcDataAttrs(prefix, "Cloud Top Phase Quality", _qcDataContainer, true);
  cout << prefix << "   " << NUMBER_OF_OVERALL_QF_VALUES << ":  " << _numberOfOverallQfValues << endl;
  cout << prefix << "   " << PERCENT_OVERALL_GOOD_QUALITY_QF << ":  " << _percentOverallGoodQualityQf << endl;
  cout << prefix << "   " << PERCENT_OVERALL_DEGRADED_QUALITY_QF << ":  " << _percentOverallDegradedQualityQf << endl;
  cout << prefix << "   " << NUMBER_OF_L1B_QF_VALUES << ":  " << _numberOfL1bQfValues << endl;
  cout << prefix << "   " << PERCENT_GOOD_QUALITY_L1B_DATA_QF << ":  " << _percentGoodQualityL1bDataQf << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_QUALITY_L1B_DATA_QF << ":  " << _percentDegradedQualityL1bDataQf << endl;
  cout << prefix << "   " << NUMBER_OF_BETA_RATIO_QF_VALUES << ":  " << _numberOfBetaRatioQfValues << endl;
  cout << prefix << "   " << PERCENT_GOOD_QUALITY_BETA_RATIO_QF << ":  " << _percentGoodQualityBetaRatioQf << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_QUALITY_BETA_RATIO_QF << ":  " << _percentDegradedQualityBetaRatioQf << endl;
  cout << prefix << "   " << NUMBER_OF_ICE_CLOUD_QF_VALUES << ":  " << _numberOfIceCloudQfValues << endl;
  cout << prefix << "   " << PERCENT_ICE_CLOUD_DETERMINATION_BASED_ON_STRONG_RADIATIVE_SIGNAL_QF << ":  " << _percentIceCloudDeterminationBasedOnStrongRadiativeSignalQf << endl;
  cout << prefix << "   " << PERCENT_ICE_CLOUD_DETERMINATION_BASED_ON_WEAK_RADIATIVE_SIGNAL_QF << ":  " << _percentIceCloudDeterminationBasedOnWeakRadiativeSignalQf << endl;
  cout << prefix << "   " << NUMBER_OF_SURFACE_EMISSIVITY_QF_VALUES << ":  " << _numberOfSurfaceEmissivityQfValues << endl;
  cout << prefix << "   " << PERCENT_GOOD_QUALITY_SURFACE_EMISSIVITY_QF << ":  " << _percentGoodQualitySurfaceEmissivityQf << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_QUALITY_SURFACE_EMISSIVITY_QF << ":  " << _percentDegradedQualitySurfaceEmissivityQf << endl;
  cout << prefix << "   " << NUMBER_OF_LZA_QF_VALUES << ":  " << _numberOfLZAQfValues << endl;
  cout << prefix << "   " << PERCENT_GOOD_WITHIN_LZA_THRESHOLD_QF << ":  " << _percentGoodWithinLZAThresholdQf << endl;
  cout << prefix << "   " << PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF << ":  " << _percentDegradedDueToLZAThresholdExceededQf << endl;
  
}


void CloudTopPhaseProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printDims(prefix, strarg);   

  cout << prefix << "   " << BAND_DIM <<  ":  " << _numBands << endl;

  cout << prefix << "Angle info" << endl;
  cout << prefix << "   " << RETRIEVAL_LOCAL_ZENITH_ANGLE <<  ":  " << _retrievalLocalZenithAngle << endl;
  cout << prefix << "   " << QUANTITATIVE_LOCAL_ZENITH_ANGLE <<  ":  " << _quantitativeLocalZenithAngle << endl;
  cout << prefix << "   " << SOLAR_ZENITH_ANGLE <<  ":  " << _solarZenithAngle << endl;
  cout << prefix << "   " << RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS <<  ":  " <<  _retrievalLocalZenithAngleBounds[0] << ",  " << _retrievalLocalZenithAngleBounds[1] << endl;
  cout << prefix << "   " << QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS <<  ":  " << _quantitativeLocalZenithAngleBounds[0] << ",  " << _quantitativeLocalZenithAngleBounds[1] << endl;
  cout << prefix << "   " << SOLAR_ZENITH_ANGLE_BOUNDS <<  ":  " << _solarZenithAngleBounds[0] << ",  " << _solarZenithAngleBounds[1] << endl;
}


void CloudTopPhaseProduct::_printQC(const std::string &prefix, const std::string &strarg)
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

void CloudTopPhaseProduct::_readDimensions()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  try {
    _readVar(RETRIEVAL_LOCAL_ZENITH_ANGLE, _retrievalLocalZenithAngle);
    _readVar(QUANTITATIVE_LOCAL_ZENITH_ANGLE, _quantitativeLocalZenithAngle);
    _readVar(SOLAR_ZENITH_ANGLE, _solarZenithAngle);
    _readArray(NUMBER_OF_LZA_BOUNDS, RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS,
    	       _retrievalLocalZenithAngleBounds);
    _readArray(NUMBER_OF_LZA_BOUNDS, QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS,
    	       _quantitativeLocalZenithAngleBounds);
    _readArray(NUMBER_OF_SZA_BOUNDS, SOLAR_ZENITH_ANGLE_BOUNDS,
    	       _solarZenithAngleBounds);

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
 
void CloudTopPhaseProduct::_readCoordinateVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_readCoordinateVars();
}


void CloudTopPhaseProduct::_readQualityControlVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  NcxxVar var;

  try {
    _readQcDataVar(DATA_QUALITY_FLAG, _qcDataContainer, var);

    var.getAtt(NUMBER_OF_OVERALL_QF_VALUES).getValues(&_numberOfOverallQfValues);
    var.getAtt(PERCENT_OVERALL_GOOD_QUALITY_QF).getValues(&_percentOverallGoodQualityQf);
    var.getAtt(PERCENT_OVERALL_DEGRADED_QUALITY_QF).getValues(&_percentOverallDegradedQualityQf);
    var.getAtt(NUMBER_OF_L1B_QF_VALUES).getValues(&_numberOfL1bQfValues);
    var.getAtt(PERCENT_GOOD_QUALITY_L1B_DATA_QF).getValues(&_percentGoodQualityL1bDataQf);
    var.getAtt(PERCENT_DEGRADED_QUALITY_L1B_DATA_QF).getValues(&_percentDegradedQualityL1bDataQf);
    var.getAtt(NUMBER_OF_BETA_RATIO_QF_VALUES).getValues(&_numberOfBetaRatioQfValues);
    var.getAtt(PERCENT_GOOD_QUALITY_BETA_RATIO_QF).getValues(&_percentGoodQualityBetaRatioQf);
    var.getAtt(PERCENT_DEGRADED_QUALITY_BETA_RATIO_QF).getValues(&_percentDegradedQualityBetaRatioQf);
    var.getAtt(NUMBER_OF_ICE_CLOUD_QF_VALUES).getValues(&_numberOfIceCloudQfValues);
    var.getAtt(PERCENT_ICE_CLOUD_DETERMINATION_BASED_ON_STRONG_RADIATIVE_SIGNAL_QF).getValues(
        &_percentIceCloudDeterminationBasedOnStrongRadiativeSignalQf);
    var.getAtt(PERCENT_ICE_CLOUD_DETERMINATION_BASED_ON_WEAK_RADIATIVE_SIGNAL_QF).getValues(
        &_percentIceCloudDeterminationBasedOnWeakRadiativeSignalQf);
    var.getAtt(NUMBER_OF_SURFACE_EMISSIVITY_QF_VALUES).getValues(&_numberOfSurfaceEmissivityQfValues);
    var.getAtt(PERCENT_GOOD_QUALITY_SURFACE_EMISSIVITY_QF).getValues(&_percentGoodQualitySurfaceEmissivityQf);
    var.getAtt(PERCENT_DEGRADED_QUALITY_SURFACE_EMISSIVITY_QF).getValues(&_percentDegradedQualitySurfaceEmissivityQf);

    // the attribute names changed. Try to have backward compatibility
    try {
      var.getAtt(NUMBER_OF_LZA_QF_VALUES).getValues(&_numberOfLZAQfValues);
      var.getAtt(PERCENT_GOOD_WITHIN_LZA_THRESHOLD_QF).getValues(&_percentGoodWithinLZAThresholdQf);
      var.getAtt(PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF).getValues(
          &_percentDegradedDueToLZAThresholdExceededQf);
    }
    catch (NcxxException &e) {
    var.getAtt(NUMBER_OF_SATELLITE_ZENITH_THRESHOLD_QF_VALUES).getValues(&_numberOfLZAQfValues);
    var.getAtt(PERCENT_GOOD_WITHIN_SATELLITE_ZENITH_THRESHOLD_QF).getValues(&_percentGoodWithinLZAThresholdQf);
    var.getAtt(PERCENT_DEGRADED_DUE_TO_SATELLITE_ZENITH_THRESHOLD_EXCEEDED_QF).getValues(
        &_percentDegradedDueToLZAThresholdExceededQf);
    }
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
  
void CloudTopPhaseProduct::_readDataVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  NcxxVar var;
  
  try {
    _readCatDataVar(CLOUD_TOP_PHASE, _catDataContainer, var);

    _readVar(TOTAL_NUMBER_CLOUDY_PIXELS, _totalNumberCloudyPixels);
    var.getAtt(NUMBER_OF_CLOUD_TOP_PHASE_CATEGORY_VALUES).getValues(&_numberOfCloudTopPhaseCategoryValues);
    var.getAtt(PERCENT_CLEAR_SKY).getValues(&_percentClearSky);
    var.getAtt(PERCENT_LIQUID_WATER).getValues(&_percentLiquidWater);
    var.getAtt(PERCENT_SUPER_COOLED_LIQUID_WATER).getValues(&_percentSuperCooledLiquidWater);
    var.getAtt(PERCENT_MIXED_PHASE).getValues(&_percentMixedPhase);
    var.getAtt(PERCENT_ICE).getValues(&_percentIce);
    var.getAtt(PERCENT_UNKNOWN).getValues(&_percentUnknown);

    // create list (map) of containers
    _outCatDataFields[CLOUD_TOP_PHASE] = &_catDataContainer;
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
const char* CloudTopPhaseProduct::CLOUD_TOP_PHASE = "Phase";
const char* CloudTopPhaseProduct::TOTAL_NUMBER_CLOUDY_PIXELS = "total_number_cloudy_pixels";
const char* CloudTopPhaseProduct::NUMBER_OF_CLOUD_TOP_PHASE_CATEGORY_VALUES = "number_of_cloud_top_phase_category_values";
const char* CloudTopPhaseProduct::PERCENT_CLEAR_SKY = "percent_clear_sky";
const char* CloudTopPhaseProduct::PERCENT_LIQUID_WATER = "percent_liquid_water";
const char* CloudTopPhaseProduct::PERCENT_SUPER_COOLED_LIQUID_WATER = "percent_super_cooled_liquid_water";
const char* CloudTopPhaseProduct::PERCENT_MIXED_PHASE = "percent_mixed_phase";
const char* CloudTopPhaseProduct::PERCENT_ICE = "percent_ice";
const char* CloudTopPhaseProduct::PERCENT_UNKNOWN = "percent_unknown";
const char* CloudTopPhaseProduct::NUMBER_OF_OVERALL_QF_VALUES = "number_of_overall_qf_values";
const char* CloudTopPhaseProduct::PERCENT_OVERALL_GOOD_QUALITY_QF = "percent_overall_good_quality_qf";
const char* CloudTopPhaseProduct::PERCENT_OVERALL_DEGRADED_QUALITY_QF = "percent_overall_degraded_quality_qf";
const char* CloudTopPhaseProduct::NUMBER_OF_L1B_QF_VALUES = "number_of_L1b_qf_values";
const char* CloudTopPhaseProduct::PERCENT_GOOD_QUALITY_L1B_DATA_QF = "percent_good_quality_L1b_data_qf";
const char* CloudTopPhaseProduct::PERCENT_DEGRADED_QUALITY_L1B_DATA_QF = "percent_degraded_quality_L1b_data_qf";
const char* CloudTopPhaseProduct::NUMBER_OF_BETA_RATIO_QF_VALUES = "number_of_beta_ratio_qf_values";
const char* CloudTopPhaseProduct::PERCENT_GOOD_QUALITY_BETA_RATIO_QF = "percent_good_quality_beta_ratio_qf";
const char* CloudTopPhaseProduct::PERCENT_DEGRADED_QUALITY_BETA_RATIO_QF = "percent_degraded_quality_beta_ratio_qf";
const char* CloudTopPhaseProduct::NUMBER_OF_ICE_CLOUD_QF_VALUES = "number_of_ice_cloud_qf_values";
const char* CloudTopPhaseProduct::PERCENT_ICE_CLOUD_DETERMINATION_BASED_ON_STRONG_RADIATIVE_SIGNAL_QF = "percent_ice_cloud_determination_based_on_strong_radiative_signal_qf";
const char* CloudTopPhaseProduct::PERCENT_ICE_CLOUD_DETERMINATION_BASED_ON_WEAK_RADIATIVE_SIGNAL_QF = "percent_ice_cloud_determination_based_on_weak_radiative_signal_qf";
const char* CloudTopPhaseProduct::NUMBER_OF_SURFACE_EMISSIVITY_QF_VALUES = "number_of_surface_emissivity_qf_values";
const char* CloudTopPhaseProduct::PERCENT_GOOD_QUALITY_SURFACE_EMISSIVITY_QF = "percent_good_quality_surface_emissivity_qf";
const char* CloudTopPhaseProduct::PERCENT_DEGRADED_QUALITY_SURFACE_EMISSIVITY_QF = "percent_degraded_quality_surface_emissivity_qf";
const char* CloudTopPhaseProduct::NUMBER_OF_LZA_QF_VALUES = "number_of_LZA_qf_values";
const char* CloudTopPhaseProduct::PERCENT_GOOD_WITHIN_LZA_THRESHOLD_QF = "percent_good_within_LZA_threshold_qf";
const char* CloudTopPhaseProduct::PERCENT_DEGRADED_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF = "percent_degraded_due_to_LZA_threshold_exceeded_qf";
const char* CloudTopPhaseProduct::NUMBER_OF_SATELLITE_ZENITH_THRESHOLD_QF_VALUES = "number_of_satellite_zenith_threshold_qf_values";
const char* CloudTopPhaseProduct::PERCENT_GOOD_WITHIN_SATELLITE_ZENITH_THRESHOLD_QF = "percent_good_within_satellite_zenith_threshold_qf";
const char* CloudTopPhaseProduct::PERCENT_DEGRADED_DUE_TO_SATELLITE_ZENITH_THRESHOLD_EXCEEDED_QF = "percent_degraded_due_to_satellite_zenith_threshold_exceeded_qf";
