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
 * @file LandSurfaceTemperatureProduct.cc
 *
 * @class LandSurfaceTemperatureProduct
 *
 * LandSurfaceTemperatureProduct is a derived class for the land surface
 * temperature Level-2 GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/LandSurfaceTemperatureProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

LandSurfaceTemperatureProduct::LandSurfaceTemperatureProduct()
{
}

LandSurfaceTemperatureProduct::LandSurfaceTemperatureProduct(const LandSurfaceTemperatureProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

LandSurfaceTemperatureProduct::~LandSurfaceTemperatureProduct()
{
}

bool LandSurfaceTemperatureProduct::applyQcTests()
{
  return true;
}

void LandSurfaceTemperatureProduct::clearAndReset()
{
  GoesRProduct::clearAndReset();
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void LandSurfaceTemperatureProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printVars(prefix, strarg);  
}


void LandSurfaceTemperatureProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printAttrs(prefix, strarg);
}


void LandSurfaceTemperatureProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printDims(prefix, strarg);   
}


void LandSurfaceTemperatureProduct::_printQC(const std::string &prefix, const std::string &strarg)
{
  
}

void LandSurfaceTemperatureProduct::_readDimensions()
{
  
}
 
void LandSurfaceTemperatureProduct::_readCoordinateVars()
{
  GoesRProduct::_readCoordinateVars();
}


void LandSurfaceTemperatureProduct::_readQualityControlVars()
{

}
  
void LandSurfaceTemperatureProduct::_readConstantsVars()
{
  
}
  
void LandSurfaceTemperatureProduct::_readDataVars()
{
  
}

/////////////////////////////////////////////////////////////////////////////////////
// string constant instantiation
//
const char* LandSurfaceTemperatureProduct::LAND_SURFACE_TEMP = "LST";
const char* LandSurfaceTemperatureProduct::TOTAL_PIXELS_WHERE_LST_IS_RETRIEVED =
  "total_pixels_where_lst_is_retrieved";
const char* LandSurfaceTemperatureProduct::NUMBER_GOOD_RETRIEVALS = "number_good_retrievals";
const char* LandSurfaceTemperatureProduct::MIN_LAND_SURFACE_TEMP = "min_lst";
const char* LandSurfaceTemperatureProduct::MAX_LAND_SURFACE_TEMP = "max_lst";
const char* LandSurfaceTemperatureProduct::MEAN_LAND_SURFACE_TEMP = "mean_lst";
const char* LandSurfaceTemperatureProduct::STD_DEV_LAND_SURFACE_TEMP = "standard_deviation_lst";
