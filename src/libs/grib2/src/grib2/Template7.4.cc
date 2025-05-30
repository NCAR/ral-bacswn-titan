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
// Template7_pt_4 IEEE Floats
//
// Jason Craig,  Nov 2023
//
//////////////////////////////////////////////////

#include <cmath>
#include <stdlib.h>

#include <grib2/Template7.0.hh>
#include <grib2/DS.hh>
#include <grib2/GDS.hh>
#include <grib2/DRS.hh>
#include <grib2/DataRepTemp.hh>
#include <grib2/Template5.0.hh>

using namespace std;

namespace Grib2 {

Template7_pt_4::Template7_pt_4(Grib2Record::Grib2Sections_t sectionsPtr)
: DataTemp(sectionsPtr)
{

}


Template7_pt_4::~Template7_pt_4 () 
{
  if(_pdata)
    delete[] _pdata;
}

void Template7_pt_4::print(FILE *stream) const
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

int Template7_pt_4::pack (fl32 *dataPtr)
{

  fl32 *pdataPtr = _applyBitMapPack(dataPtr);
  si32 gridSz = _sectionsPtr.drs->getNumPackedDataPoints();
  DataRepTemp::data_representation_t drsConstants = _sectionsPtr.drs->getDrsConstants();
  si32 tmp;

  if(_pdata)
    delete[] _pdata;
  _pdata = new fl32[gridSz];

  for (int j = 0;j<gridSz;j++) {
    tmp = GribSection::mkIeee(dataPtr[j]);
    GribSection::_pkUnsigned4(tmp, (ui08 *)&(_pdata[j]));
  }

  if(pdataPtr != dataPtr)
    delete [] pdataPtr;

  return GRIB_SUCCESS;
}


int Template7_pt_4::unpack (ui08 *dataPtr) 
{
  si32 gridSz = _sectionsPtr.drs->getNumPackedDataPoints();
  DataRepTemp::data_representation_t drsConstants = _sectionsPtr.drs->getDrsConstants();
  fl32 *outputData = new fl32[gridSz];
  si32 precision = drsConstants.numberOfBits;
  si32 tmp;
 

//
//  Only 32 Bit Precision currently implemented
  if (precision == 1) {
    for (int j = 0; j < gridSz; j++) {
      tmp = GribSection::_upkUnsigned4 (dataPtr[(j*4)+0], dataPtr[(j*4)+1], dataPtr[(j*4)+2], dataPtr[(j*4)+3]);
      outputData[j] = GribSection::rdIeee(tmp);
    }
  }
  else {
    cerr << "ERROR: DRS IEEE Floating point precision > 32bit not implemented!" << endl;
    for (int j=0; j < gridSz; j++) {
      outputData[j] = -9999.0;
    }
  }

  _applyBitMapUnpack(outputData);
  
  return GRIB_SUCCESS;
}

} // namespace Grib2

