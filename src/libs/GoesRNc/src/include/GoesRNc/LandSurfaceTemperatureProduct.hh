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
 * @file LandSurfaceTemperatureProduct.hh
 *
 * @class LandSurfaceTemperatureProduct
 *
 * 
 *  
 * @author G. Cunning
 *
 * @date 13 May 2019
 *
 */

#ifndef LAND_SURFACE_TEMPERATURE_PRODUCT_HH
#define LAND_SURFACE_TEMPERATURE_PRODUCT_HH

#include <string>

#include "GoesRProduct.hh"

/** 
 * @class LandSurfaceTemperatureProduct
 */

class LandSurfaceTemperatureProduct : public GoesRProduct
{

public:


  LandSurfaceTemperatureProduct();
  LandSurfaceTemperatureProduct(const LandSurfaceTemperatureProduct &from);
  ~LandSurfaceTemperatureProduct();
  
  void clearAndReset();
  bool applyQcTests();
  
  static const char* LAND_SURFACE_TEMP;
  
protected:
  


private:

  void _printVars(const std::string &prefix, const std::string &strarg = "");
  void _printAttrs(const std::string &prefix, const std::string &strarg = "");
  void _printDims(const std::string &prefix, const std::string &strarg = "");
  void _printQC(const std::string &prefix, const std::string &strarg = "");
  void _readVariables();
  void _readGlobalAttributes();
  void _readDimensions();
  void _readTimeVars();
  void _readCoordinateVars();
  void _readProjectionVars();
  void _readConstantsVars();
  void _readQualityControlVars();
  void _readDataVars();

  static const char* TOTAL_PIXELS_WHERE_LST_IS_RETRIEVED;
  static const char* NUMBER_GOOD_RETRIEVALS;
  static const char* MIN_LAND_SURFACE_TEMP;
  static const char* MAX_LAND_SURFACE_TEMP;
  static const char* MEAN_LAND_SURFACE_TEMP;
  static const char* STD_DEV_LAND_SURFACE_TEMP;

};




#endif // LAND_SURFACE_TEMPERATURE_PRODUCT_HH
