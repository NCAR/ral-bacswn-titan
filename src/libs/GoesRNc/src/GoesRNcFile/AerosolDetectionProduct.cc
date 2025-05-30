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
 * @file AerosolDetectionProduct.cc
 *
 * @class AerosolDetectionProduct
 *
 * AerosolDetectionProduct is derived class for the aerosol detectoiom
 * Level-2 GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/AerosolDetectionProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

AerosolDetectionProduct::AerosolDetectionProduct()
{
}

AerosolDetectionProduct::AerosolDetectionProduct(const AerosolDetectionProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

AerosolDetectionProduct::~AerosolDetectionProduct()
{
}

bool AerosolDetectionProduct::applyQcTests()
{
  return true;
}

void AerosolDetectionProduct::clearAndReset()
{
  GoesRProduct::clearAndReset();
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void AerosolDetectionProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printVars(prefix, strarg);  
}


void AerosolDetectionProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printAttrs(prefix, strarg);
}


void AerosolDetectionProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printDims(prefix, strarg);   
}


void AerosolDetectionProduct::_printQC(const std::string &prefix, const std::string &strarg)
{

}
 

void AerosolDetectionProduct::_readDimensions()
{
 
}

void AerosolDetectionProduct::_readCoordinateVars()
{
  GoesRProduct::_readCoordinateVars();
}


void AerosolDetectionProduct::_readQualityControlVars()
{

}
  
void AerosolDetectionProduct::_readConstantsVars()
{

}
  
void AerosolDetectionProduct::_readDataVars()
{

}

/////////////////////////////////////////////////////////////////////////////////////
// string constant instantiation
//
const char* AerosolDetectionProduct::AEROSOL = "Aerosol";
const char* AerosolDetectionProduct::SMOKE = "Smoke";
const char* AerosolDetectionProduct::DUST = "Dust";
const char* AerosolDetectionProduct::NUMBER_GOOD_LZA_PIXELS = "number_good_LZA_pixels";
const char* AerosolDetectionProduct::NUMBER_GOOD_SZA_PIXELS = "number_good_SZA_pixels";
const char* AerosolDetectionProduct::NUMBER_OF_GOOD_SMOKE_RETRIEVALS =
  "number_of_good_smoke_retrievals";
const char* AerosolDetectionProduct::NUMBER_OF_GOOD_DUST_RETRIEVALS = "number_of_good_dust_retrievals";
const char* AerosolDetectionProduct::NUMBER_OF_GOOD_RETRIEVALS_WHERE_SMOKE_DETECTED =
  "number_of_good_retrievals_where_smoke_detected";
const char* AerosolDetectionProduct::NUMBER_OF_GOOD_RETRIEVALS_WHERE_DUST_DETECTED =
  "number_of_good_retrievals_where_dust_detected";
