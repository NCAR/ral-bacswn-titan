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
///////////////////////////////////////////////////
// Template7_pt_61 Simple Packing Format with logarithm pre-processing
//
// Jason Craig,  Nov 2023
//
//////////////////////////////////////////////////

#include <cmath>
#include <stdlib.h>

#include <grib2/Template7.61.hh>
#include <grib2/DS.hh>
#include <grib2/GDS.hh>
#include <grib2/DRS.hh>
#include <grib2/DataRepTemp.hh>
#include <grib2/Template5.0.hh>

using namespace std;

namespace Grib2 {

Template7_pt_61::Template7_pt_61(Grib2Record::Grib2Sections_t sectionsPtr)
: DataTemp(sectionsPtr)
{

}


Template7_pt_61::~Template7_pt_61 () 
{
  if(_pdata)
    delete[] _pdata;
}

void Template7_pt_61::print(FILE *stream) const
{
  si32 gridSz = _sectionsPtr.gds->getNumDataPoints();
  fprintf(stream, "DS length: %d\n", gridSz);
  if(_data) {
    for (int i = 0; i < gridSz; i++) {
      fprintf(stream, "%f ", _data[i]);
    }
    fprintf(stream, "\n");
  }
}

// Currently not support packing this type
int Template7_pt_61::pack (fl32 *dataPtr)
{
  return GRIB_FAILURE;
}


int Template7_pt_61::unpack (ui08 *dataPtr) 
{
  si32 gridSz = _sectionsPtr.drs->getNumPackedDataPoints();
  DataRepTemp::data_representation_t drsConstants = _sectionsPtr.drs->getDrsConstants();
  fl32 *outputData = new fl32[gridSz];
  si32  j;
  fl32 bscale, dscale, reference, b;

  bscale = pow(2.0, drsConstants.binaryScaleFactor);
  dscale = pow(10.0, -drsConstants.decimalScaleFactor);
  reference = drsConstants.referenceValue;

  if(drsConstants.templateNumber == 61) {
    Template5_pt_61 *template5_61 = (Template5_pt_61 *) _sectionsPtr.drs->getDrsTemplate();
    b = template5_61->getB();
  } else {
    b = 0.0;
  }
  si32 *tmp_data = new si32 [gridSz];

//
//  if drsConstants.numberOfBits equals 0, we have a constant field where the reference value
//  is the data value at each gridpoint
//
  if (drsConstants.numberOfBits != 0) {
    DS::gbits (dataPtr, tmp_data, 0, drsConstants.numberOfBits, 0, gridSz);
    for (j = 0; j < gridSz; j++) {
      outputData[j] = exp((((fl32) tmp_data[j] * bscale) 
			   + reference) * dscale) - b;
    }
  }
  else {
    for (j=0; j < gridSz; j++) {
      outputData[j] = exp(reference * dscale) - b;
    }
  }
  delete [] tmp_data;
  
  _applyBitMapUnpack(outputData);
  
  return GRIB_SUCCESS;
}

} // namespace Grib2

