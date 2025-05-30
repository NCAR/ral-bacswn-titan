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
 * @file CloudTopPressureProduct.cc
 *
 * @class CloudTopPressureProduct
 *
 * CloudTopPressureProduct is a derived class for the cloud top pressure
 * Level-2 GOES-R product, based on GoesRProduct class.
 *  
 * @date 15 May 2019
 *
 */



#include <GoesRNc/CloudTopPressureProduct.hh>

using namespace std;


/*********************************************************************
 * Constructor
 */

CloudTopPressureProduct::CloudTopPressureProduct()
{
}

CloudTopPressureProduct::CloudTopPressureProduct(const CloudTopPressureProduct &from)
{
}

/*********************************************************************
 * Destructor
 */

CloudTopPressureProduct::~CloudTopPressureProduct()
{
}

bool CloudTopPressureProduct::applyQcTests()
{
 static const string methodName = _className + "::" + __FUNCTION__;

  for(size_t i = 0; i < _numX*_numY; i++) {

    if(_qcDataContainer.values[i] == DATA_QUALITY_GOOD) { 
      continue;
    }

    _dataContainer.values[i] = BAD_DATA_VALUE;
    _badPixelCount++;
  }

  return true;
}

void CloudTopPressureProduct::clearAndReset()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::clearAndReset();
}



/**********************************************************************
 *              Private Member Functions                              *
 **********************************************************************/

void CloudTopPressureProduct::_printVars(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printVars(prefix, strarg);  
}


void CloudTopPressureProduct::_printAttrs(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printAttrs(prefix, strarg);
}


void CloudTopPressureProduct::_printDims(const std::string &prefix, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  GoesRProduct::_printDims(prefix, strarg);   
}


void CloudTopPressureProduct::_printQC(const std::string &prefix, const std::string &strarg)
{
  
}

void CloudTopPressureProduct::_readDimensions()
{
  
}
 
void CloudTopPressureProduct::_readCoordinateVars()
{
  GoesRProduct::_readCoordinateVars();
}


  
void CloudTopPressureProduct::_readQualityControlVars()
{

}
  
void CloudTopPressureProduct::_readConstantsVars()
{

}
  
void CloudTopPressureProduct::_readDataVars()
{

}

/////////////////////////////////////////////////////////////////////////////////////
// string constant instantiation
//
const char* CloudTopPressureProduct::CLOUD_TOP_PRESSURE = "PRES";
const char* CloudTopPressureProduct::MIN_CLOUD_TOP_PRESSURE = "minimum_cloud_top_pressure";
const char* CloudTopPressureProduct::MAX_CLOUD_TOP_PRESSURE = "maximum_cloud_top_pressure";
const char* CloudTopPressureProduct::MEAN_CLOUD_TOP_PRESSURE = "mean_cloud_top_pressure";
const char* CloudTopPressureProduct::STD_DEV_CLOUD_TOP_PRESSURE = "std_dev_cloud_top_pressure";
