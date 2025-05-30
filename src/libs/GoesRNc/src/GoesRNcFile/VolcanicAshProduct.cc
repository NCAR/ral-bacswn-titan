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
 * @file VolcanicAshProduct.cc
 *
 * @class VolcanicAshProduct
 *
 * VolcanicAshProduct is a derived class for the volcanic ash Level-2
 * GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/VolcanicAshProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

VolcanicAshProduct::VolcanicAshProduct()
{
}

VolcanicAshProduct::VolcanicAshProduct(const VolcanicAshProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

VolcanicAshProduct::~VolcanicAshProduct()
{
}

bool VolcanicAshProduct::applyQcTests()
{

  return true;
}

void VolcanicAshProduct::clearAndReset()
{
  GoesRProduct::clearAndReset();
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void VolcanicAshProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printVars(prefix, strarg);  
}


void VolcanicAshProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printAttrs(prefix, strarg);
}


void VolcanicAshProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  GoesRProduct::_printDims(prefix, strarg);   
}


void VolcanicAshProduct::_printQC(const std::string &prefix, const std::string &strarg)
{
  
}

void VolcanicAshProduct::_readDimensions()
{
  
}
 
void VolcanicAshProduct::_readCoordinateVars()
{
  GoesRProduct::_readCoordinateVars();
}

void VolcanicAshProduct::_readQualityControlVars()
{
  
}
  
void VolcanicAshProduct::_readConstantsVars()
{
  
}
  
void VolcanicAshProduct::_readDataVars()
{
  
}

/////////////////////////////////////////////////////////////////////////////////////
// string constant instantiation
//
