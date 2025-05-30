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
 * @file CloudTopHeightProduct.hh
 *
 * @class CloudTopHeightProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef CLOUD_TOP_HEIGHT_PRODUCT_HH
#define CLOUD_TOP_HEIGHT_PRODUCT_HH

#include <string>

#include "GoesRProduct.hh"

/** 
 * @class CloudTopHeightProduct
 */

class CloudTopHeightProduct : public GoesRProduct
{

public:


  CloudTopHeightProduct();
  CloudTopHeightProduct(const CloudTopHeightProduct &from);
  ~CloudTopHeightProduct();
  
  void clearAndReset();
  bool applyQcTests();
  
  inline void setCheckQualityField(bool run) { _checkQualityField = run; }
  inline void setBadCountTest(bool run, int threshold)
  { _badPixel.run = run; _badPixel.threshold = threshold; }

  static const char* CLOUD_TOP_HEIGHT;
  
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
  
  inline void _readConstantsVars() {}; // do nothing with virtual method

  double _percentGoodQualityQF;
  double _percentInvalidDueToNotGeolocatedQf;
  double _percentInvalidDueToLZAThresholdExceededQf;
  double _percentInvalidDueToBadOrMissingBrightnessTempDataQf;
  double _percentInvalidDueToClearOrProbablyClearSkyQf;
  double _percentInvalidDueToUnknownCloudTypeQf;
  double _percentInvalidDueToNonconvergentRetrievalQf;

  // QC test variables
  bool _checkQualityField;
  QcCountTestParams _badPixel;
  int _badPixelCount;
  bool _passBadCountTest;

  static const char* MIN_CLOUD_TOP_HEIGHT;
  static const char* MAX_CLOUD_TOP_HEIGHT;
  static const char* MEAN_CLOUD_TOP_HEIGHT;
  static const char* STD_DEV_CLOUD_TOP_HEIGHT;
  static const char* PERCENT_GOOD_QUALITY_QF;
  static const char* PERCENT_INVALID_DUE_TO_NOT_GEOLOCATED_QF;
  static const char* PERCENT_INVALID_DUE_TO_LZA_THRESHOLD_EXCEEDED_QF;
  static const char* PERCENT_INVALID_DUE_TO_BAD_OR_MISSING_BRIGHTNESS_TEMP_DATA_QF;
  static const char* PERCENT_INVALID_DUE_TO_CLEAR_OR_PROBABLY_CLEAR_SKY_QF;
  static const char* PERCENT_INVALID_DUE_TO_UNKNOWN_CLOUD_TYPE_QF;
  static const char* PERCENT_INVALID_DUE_TO_NONCONVERGENT_RETRIEVAL_QF;

}; 


#endif // CLOUD_TOP_HEIGHT_PRODUCT_HH
