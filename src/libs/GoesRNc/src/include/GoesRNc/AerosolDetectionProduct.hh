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
 * @file AerosolDetectionProduct.hh
 *
 * @class AerosolDetectionProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef AEROSOL_DETECTION_PRODUCT_HH
#define AEROSOL_DETECTION_PRODUCT_HH

#include <string>

#include "GoesRProduct.hh"

/** 
 * @class AerosolDetectionProduct
 */

class AerosolDetectionProduct : public GoesRProduct
{

public:


  AerosolDetectionProduct();
  AerosolDetectionProduct(const AerosolDetectionProduct &from);
  ~AerosolDetectionProduct();
  
  void clearAndReset();
  bool applyQcTests();
  
  
  static const char* AEROSOL;
  static const char* SMOKE;
  static const char* DUST;

protected:
  


private:

  void _printVars(const std::string &prefix, const std::string &strarg = "");
  void _printAttrs(const std::string &prefix, const std::string &strarg = "");
  void _printDims(const std::string &prefix, const std::string &strarg = "");
  void _printQC(const std::string &prefix, const std::string &strarg = "");
  void _readDimensions();
  void _readCoordinateVars();
  void _readConstantsVars();
  void _readQualityControlVars();
  void _readDataVars();

  static const char* NUMBER_GOOD_LZA_PIXELS;
  static const char* NUMBER_GOOD_SZA_PIXELS;
  static const char* NUMBER_OF_GOOD_SMOKE_RETRIEVALS;
  static const char* NUMBER_OF_GOOD_DUST_RETRIEVALS;
  static const char* NUMBER_OF_GOOD_RETRIEVALS_WHERE_SMOKE_DETECTED;
  static const char* NUMBER_OF_GOOD_RETRIEVALS_WHERE_DUST_DETECTED;

};




#endif // AEROSOL_DETECTION_PRODUCT_HH
