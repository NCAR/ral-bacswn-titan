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
 * @file RadianceProduct.hh
 *
 * @class RadianceProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef RADIANCE_PRODUCT_HH
#define RADIANCE_PRODUCT_HH

#include <string>

#include <GoesRNc/GoesRProduct.hh>

/** 
 * @class RadianceProduct
 */

class RadianceProduct : public GoesRProduct
{

public:

  static const int EMISSIVE_BAND_START;
  static const double MIN_RADIANCE;
  
  static const char* RADIANCE;

  RadianceProduct();
  RadianceProduct(const RadianceProduct &from);
  ~RadianceProduct();
  
  void clearAndReset();
  bool applyQcTests();

  double toBrightnessTemp(double rad);
  double toAlbedo(double rad);

  inline int getBandID() { return _bandID; }
  
  inline void setHeatPipeVars(bool hv) { _heatPipeVars = hv; }

  inline void setCheckQualityField(bool run) { _checkQualityField = run; }

  inline void setCheckFlagList(size_t n, const int* q_list)
  { for(size_t i = 0; i < n; i++) { _checkQualityList.push_back(q_list[i]); } }

  inline void setBadCountTest(bool run, int threshold)
  { _badPixel.run = run; _badPixel.threshold = threshold; }

  inline void setMissingPixelTest(bool run, int threshold)
  { _missingPixel.run = run; _missingPixel.threshold = threshold; }

  inline void setSaturatedPixelTest(bool run, int threshold)
  { _saturatedPixel.run = run; _saturatedPixel.threshold = threshold; }

  inline void setUndersaturatedPixelTest(bool run, int threshold)
  { _undersaturatedPixel.run = run; _undersaturatedPixel.threshold = threshold; }

  inline void setFocalPlaneTempCountTest(bool run, int threshold)
  { _focalPlaneTempCount.run = run; _focalPlaneTempCount.threshold = threshold; }

  inline void setClipNegativeRadiance(bool run) { _clipNegativeRadiance = run; }

  inline void setFocalPlaneMaxTempTest(bool run, double val)
  { _focalPlaneMaxTemp.run = run; _focalPlaneMaxTemp.threshold = val; }
    
  
protected:
  


private:

  typedef enum {
    QF_CONDITIONALLY_USABLE      = 1,
    QF_OUT_OF_RANGE              = 2,
    QF_NO_VALUE_PIXEL            = 3,
    QF_EXCEEDED_FOCAL_PLANE_TEMP = 4
  } quality_flag_t;

  int _sensorBitDepth;

  double _percentGoodPixels;
  double _percentCondUsabePixels;
  double _percentOutOfRangePixels;
  double _percentNoValuePixels;
  double _percentFocalPlaneTempExceedPixels;
  
  bool _heatPipeVars;
  
  size_t _numStarLooks;

  bool _yawFlipFlag;
  int _bandID;
  double _bandWavelength;

  // variables for converting radiances to relfectances and
  // brightness temperatures

  double _eSun; 
  double _kappa0; 
  double _planckFk1; 
  double _planckFk2; 
  double _planckBc1; 
  double _planckBc2; 

  // quality control variables
  
  int _validPixelCount;
  int _missingPixelCount;
  int _saturatedPixelCount;
  int _undersaturatedPixelCount;
  int _focalPlaneTempThreshExceedCount;
  double _maxFocalPlaneTemp;
  double _focalPlaneTemperatureThresholdIncr;
  double _focalPlaneTemperatureThresholdDecr;
  double _percentUncorrectableL0Errors;
  std::vector< time_t > _timeStarLook;
  std::vector< float > _bandWavelengthStarLook;
  std::vector< int > _starID;

  // QC test variables
  bool _checkQualityField;
  std::vector< int > _checkQualityList;
  QcCountTestParams _badPixel;
  QcCountTestParams _missingPixel;
  QcCountTestParams _saturatedPixel;
  QcCountTestParams _undersaturatedPixel;
  QcCountTestParams _focalPlaneTempCount;
  QcCountTestParams _focalPlaneMaxTemp;
  
  bool _clipNegativeRadiance;
  
  int _badPixelCount;
  bool _passMissingPixelTest;
  bool _passSaturatedPixelTest;
  bool _passUndersaturatedPixelTest;
  bool _passFocalPlaneCountTest;
  bool _passFocalPlaneMaxTempTest;
  bool _passBadCountTest;

  // Radiance specific Dimensions
  static const char* NUM_STAR_LOOKS;
  static const char* YAW_FLIP_FLAG;
  static const char* BAND_ID;
  static const char* BAND_WAVELENGTH;
  static const char* ESUN;
  static const char* KAPPA0;
  static const char* PLANCK_FK1;
  static const char* PLANCK_FK2;
  static const char* PLANCK_BC1;
  static const char* PLANCK_BC2;
  static const char* VALID_PIXEL_COUNT;
  static const char* MISSING_PIXEL_COUNT;
  static const char* SATURATED_PIXEL_COUNT;
  static const char* UNDERSATURATED_PIXEL_COUNT;
  static const char* FOCAL_PLANE_TEMPERATURE_THRESHOLD_EXCEEDED_COUNT;
  static const char* MIN_RADIANCE_VALUE_OF_VALID_PIXELS;
  static const char* MAX_RADIANCE_VALUE_OF_VALID_PIXELS;
  static const char* MEAN_RADIANCE_VALUE_OF_VALID_PIXELS;
  static const char* STD_DEV_RADIANCE_VALUE_OF_VALID_PIXELS;
  static const char* MAXIMUM_FOCAL_PLANE_TEMPERATURE;
  static const char* FOCAL_PLANE_TEMPERATURE_THRESHOLD_INCREASING;
  static const char* FOCAL_PLANE_TEMPERATURE_THRESHOLD_DECREASING;
  static const char* EARTH_SUN_DISTANCE_ANOMALY_IN_AU;
  static const char* T_STAR_LOOK;
  static const char* BAND_WAVELENGTH_STAR_LOOK;
  static const char* STAR_ID;
  static const char* PERCENT_UNCORRECTABLE_L0_ERRORS;
  
  // Variable Attributes
  static const char* SENSOR_BAND_BIT_DEPTH;
  static const char* NUMBER_OF_QF_VALUES;
  static const char* PERCENT_GOOD_PIXEL_QF;
  static const char* PERCENT_CONDITIONALLY_USABLE_PIXEL_QF;
  static const char* PERCENT_OUT_OF_RANGE_PIXEL_QF;
  static const char* PERCENT_NO_VALUE_PIXEL_QF;
  static const char* PERCENT_FOCAL_PLANE_TEMPERATURE_THRESHOLD_EXCEEDED_QF;
  static const char* SWEEP_ANGLE_AXIS;
  
  void _printVars(const std::string &prefix, const std::string &strarg = "");
  void _printAttrs(const std::string &prefix, const std::string &strarg = "");
  void _printDims(const std::string &prefix, const std::string &strarg = "");
  void _printQC(const std::string &prefix, const std::string &strarg = "");

  void _readDimensions();
  void _readCoordinateVars();
  void _readQualityControlVars();
  void _readDataVars();
  void _readConstantsVars(); 




};




#endif // RADIANCE_PRODUCT_HH
