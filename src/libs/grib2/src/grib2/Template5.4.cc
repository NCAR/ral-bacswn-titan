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
// Template5_pt_4 IEEE Floating Point Data
//
// Jason Craig,  Nov 2023
//
//////////////////////////////////////////////////

#include <grib2/Template5.4.hh>
#include <grib2/DRS.hh>
#include <grib2/GDS.hh>

using namespace std;

namespace Grib2 {

const si32 Template5_pt_4::TEMPLATE5_PT_4_SIZE = 12;

Template5_pt_4::Template5_pt_4(si32 precision)
: DataRepTemp()
{
  _dataRepresentation.templateNumber = 4;
  _dataRepresentation.numberOfBits = precision;
  _dataRepresentation.decimalScaleFactor = 0;
  _dataRepresentation.binaryScaleFactor = 0;
  _dataRepresentation.origFieldTypes = 0;
}

Template5_pt_4::Template5_pt_4(Grib2Record::Grib2Sections_t sectionsPtr)
: DataRepTemp(sectionsPtr)
{
  _dataRepresentation.templateNumber = 4;
}


Template5_pt_4::~Template5_pt_4 () {

}


int Template5_pt_4::pack (ui08 *templatePtr)
{
  templatePtr[0] = (ui08) _dataRepresentation.numberOfBits;

  return GRIB_SUCCESS;
}

int Template5_pt_4::unpack (ui08 *templatePtr) 
{
  si32 tmp;

  _dataRepresentation.templateNumber = 4;

  // Precision (see Code Table 5.7) 
  _dataRepresentation.numberOfBits = (si32) templatePtr[0]; 

  _dataRepresentation.decimalScaleFactor = 0;
  _dataRepresentation.binaryScaleFactor = 0;
  _dataRepresentation.origFieldTypes = 0;

  return( GRIB_SUCCESS );
}


void Template5_pt_4::print(FILE *stream) const
{
  fprintf(stream, "Grid Point Data, IEEE Floating Point Data: \n");

  fprintf(stream, "Precision: ");
  switch (_dataRepresentation.numberOfBits) {
     case 1:
 	fprintf(stream, "32 bit\n");
        break;
     case 2:
 	fprintf(stream, "64 bit\n");
        break;
     case 3:
 	fprintf(stream, "128 bit\n");
        break;
     case 255:
        fprintf(stream, "Missing\n");
        break;
     default:
       fprintf(stream, "Reserved for local use\n");
  }

}

} // namespace Grib2

