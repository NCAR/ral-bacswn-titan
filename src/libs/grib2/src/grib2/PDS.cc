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
//////////////////////////////////////////////////////
// PDS - Product Definition Section
//
// $Id: PDS.cc,v 1.19 2020/07/30 20:49:59 jcraig Exp $   
//
//////////////////////////////////////////////////////

#include <iostream>

#include <grib2/PDS.hh>

using namespace std;

namespace Grib2 {

PDS::PDS(Grib2Record::Grib2Sections_t sectionsPtr) :
  GribSection()
{
  _sectionNum = 4;
  _sectionsPtr = sectionsPtr;
  _sectionsPtr.pds = this;
  _coordinateValsize = 0;
  _prodDefinition = NULL;
}

PDS::PDS(Grib2Record::Grib2Sections_t sectionsPtr, si32 prodDefNum, ProdDefTemp *productTemplate) :
  GribSection()
{
  _sectionLen = 0;
  _sectionNum = 4;
  _sectionsPtr = sectionsPtr;
  _sectionsPtr.pds = this;
  _coordinateValsize = 0;
  _prodDefTempNum = prodDefNum;
   switch (_prodDefTempNum) {  
     case 0:
     case 1:
     case 2:
     case 5:
     case 6:
     case 7:
     case 8:
     case 9:
     case 10:
     case 11:
     case 12:
     case 15:
     case 30:
     case 31:
     case 32:
     case 33:
     case 34:
     case 35:
     case 40:
     case 41:
     case 42:
     case 43:
     case 44:
     case 45:
     case 46:
     case 47:
     case 48:
     case 49:
       break;
     default:
       _prodDefinition = NULL;
       cerr << "ERROR: PDS()" << endl;
       cerr << "Product definition template  - " << prodDefNum << " not implemented" << endl;
       return;
   }
  _prodDefinition = productTemplate;
  _prodDefinition->setSectionsPtr(_sectionsPtr);
  _sectionLen = _prodDefinition->getTemplateSize();
}

PDS::~PDS() 
{
  if (_prodDefinition != NULL)
    delete  _prodDefinition;
}

int PDS::unpack( ui08 *pdsPtr)
{
  // reinitialize variables 

   //
   // Length in bytes of this section
   //
   _sectionLen = _upkUnsigned4 (pdsPtr[0], pdsPtr[1], pdsPtr[2], pdsPtr[3]);

   //
   // Various identification numbers - See Grib docmumentation
   //
   _sectionNum = (si32) pdsPtr[4];

   if (_sectionNum != 4) {
     cerr << "ERROR: PDS::unpack()" << endl;
     cerr << "Detecting incorrect section number, should be 4 but found section "
           << _sectionNum << endl;
     return( GRIB_FAILURE );
   }

   _coordinateValsize = (si32) _upkUnsigned2 (pdsPtr[5], pdsPtr[6]);

   if(_coordinateValsize != 0) {
     cerr << "WARNING: PDS::unpack()" << endl;
     cerr << "Additional coordinate values are present, reading additional values is not implemented" << endl;
   }

   _prodDefTempNum = (si32) _upkUnsigned2 (pdsPtr[7], pdsPtr[8]);

   switch (_prodDefTempNum) {  
     case 0:
       _prodDefinition = new Template4_pt_0(_sectionsPtr);
       break;
     case 1:
       _prodDefinition = new Template4_pt_1(_sectionsPtr);
       break;
     case 2:
       _prodDefinition = new Template4_pt_2(_sectionsPtr);
       break;
     case 5:
       _prodDefinition = new Template4_pt_5(_sectionsPtr);
       break;
     case 6:
       _prodDefinition = new Template4_pt_6(_sectionsPtr);
       break;
     case 7:
       _prodDefinition = new Template4_pt_7(_sectionsPtr);
       break;
     case 8:
       _prodDefinition = new Template4_pt_8(_sectionsPtr);
       break;
     case 9:
       _prodDefinition = new Template4_pt_9(_sectionsPtr);
       break;
     case 10:
       _prodDefinition = new Template4_pt_10(_sectionsPtr);
       break;
     case 11:
       _prodDefinition = new Template4_pt_11(_sectionsPtr);
       break;
     case 12:
       _prodDefinition = new Template4_pt_12(_sectionsPtr);
       break;
     case 15:
       _prodDefinition = new Template4_pt_15(_sectionsPtr);
       break;
     case 30:
       _prodDefinition = new Template4_pt_30(_sectionsPtr);
       break;
     case 31:
       _prodDefinition = new Template4_pt_31(_sectionsPtr);
       break;
     case 32:
       _prodDefinition = new Template4_pt_32(_sectionsPtr);
       break;
     case 33:
       _prodDefinition = new Template4_pt_33(_sectionsPtr);
       break;
     case 34:
       _prodDefinition = new Template4_pt_34(_sectionsPtr);
       break;
     case 35:
       _prodDefinition = new Template4_pt_35(_sectionsPtr);
       break;
     case 40:
       _prodDefinition = new Template4_pt_40(_sectionsPtr);
       break;
     case 41:
       _prodDefinition = new Template4_pt_41(_sectionsPtr);
       break;
     case 42:
       _prodDefinition = new Template4_pt_42(_sectionsPtr);
       break;
     case 43:
       _prodDefinition = new Template4_pt_43(_sectionsPtr);
       break;
     case 44:
       _prodDefinition = new Template4_pt_44(_sectionsPtr);
       break;
     case 45:
       _prodDefinition = new Template4_pt_45(_sectionsPtr);
       break;
     case 46:
       _prodDefinition = new Template4_pt_46(_sectionsPtr);
       break;
     case 47:
       _prodDefinition = new Template4_pt_47(_sectionsPtr);
       break;
     case 48:
       _prodDefinition = new Template4_pt_48(_sectionsPtr);
       break;
     case 49:
       _prodDefinition = new Template4_pt_49(_sectionsPtr);
       break;
     default:
       cerr << "ERROR: PDS::unpack()" << endl;
       cerr << "Product definition template  - " << _prodDefTempNum << " not implemented" << endl;
       _prodDefinition = NULL;
       return (GRIB_FAILURE);
   }

   if(_prodDefinition)
     _prodDefinition->unpack(&pdsPtr[9]);

   return( GRIB_SUCCESS );
}

int PDS::pack(ui08 *pdsPtr) 
{
  _pkUnsigned4(_sectionLen, &(pdsPtr[0]));

  pdsPtr[4] = (ui08) _sectionNum;

  _pkUnsigned2(_coordinateValsize, &(pdsPtr[5]));

  _pkUnsigned2(_prodDefTempNum, &(pdsPtr[7]));

   switch (_prodDefTempNum) {  
     case 0:
     case 1:
     case 2:
     case 5:
     case 6:
     case 7:
     case 8:
     case 9:
     case 10:
     case 11:
     case 12:
     case 15:
     case 30:
     case 31:
     case 32:
     case 33:
     case 34:
     case 35:
     case 40:
     case 41:
     case 42:
     case 43:
     case 44:
     case 45:
     case 46:
     case 47:
     case 48:
     case 49:
       return _prodDefinition->pack(&pdsPtr[9]);

     default:
       cerr << "ERROR: PDS::unpack()" << endl;
       cerr << "Product definition template  - " << _prodDefTempNum << " not implemented" << endl;
       return (GRIB_FAILURE);
   }

}

void PDS::getRecSummary (Grib2Record::rec_summary_t *summary) {

  return _prodDefinition->getRecSummary(summary);
}

long int PDS::getForecastTime () const {

  return _prodDefinition->getForecastTime();
}

void PDS::print(FILE *stream) const
{
  fprintf(stream, "\n\n");
  fprintf(stream, "Product Definition Section:\n");
  fprintf(stream, "----------------------------------------------------\n");
  fprintf(stream, "PDS length %d\n", _sectionLen);
  fprintf(stream, "Number of coordinates values %d\n", _coordinateValsize);
  fprintf(stream, "Product Definition Number 4.%d:\n", _prodDefTempNum);
  switch (_prodDefTempNum) {
     case 0:
          fprintf(stream, "	Analysis or forecast at a horizontal level or in a horizontal layer at a point in time\n");
          break;
     case 1:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time\n");
          break;
     case 2:
          fprintf(stream, "	Derived forecast based on all ensemble members at a horizontal level or in a horizontal layer at a point in time\n");
          break;
     case 3:
          fprintf(stream, "	Derived forecasts based on a cluster of ensemble members over a rectangular\n");
          fprintf(stream, "	area at a horizontal level or in a horizontal layer at a point in time\n");
          break;
     case 4:
          fprintf(stream, "	Derived forecasts based on a cluster of ensemble members over a circular\n");
          fprintf(stream, "	area at a horizontal level or in a horizontal layer at a point in time\n");
          break;
     case 5:
          fprintf(stream, "	Probability forecasts at a horizontal level or in a horizontal layer at a point in time\n");
          break;
     case 6:
          fprintf(stream, "	Percentile forecasts at a horizontal level or in a horizontal layer at a point in time\n");
          break;
     case 7:
          fprintf(stream, "	Analysis or forecast error at a horizontal level or in a horizontal layer at a point in time\n");
          break;
     case 8:
          fprintf(stream, "	Average, accumulation, extreme values or other statistically processed value\n");
          fprintf(stream, "	at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval\n");
          break;
     case 9:
          fprintf(stream, "	Probability forecasts at a horizontal level or in a horizontal layer in a continuous\n");
          fprintf(stream, "	or non-continuous time interval.\n");
          break;
     case 10:
          fprintf(stream, "	Percentile forecasts at a horizontal level or in a horizontal layer in a continuous or\n");
          fprintf(stream, "	non-continuous time interval.");
          break;
     case 11:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a\n");
          fprintf(stream, "	horizontal layer, in a continuous or non-continuous time interval.\n");
          break;
     case 12:
          fprintf(stream, "	Derived forecasts based on all ensemble members at a horizontal level or in a\n");
          fprintf(stream, "	horizontal layer, in a continuous or non-continuous time interval.\n");
          break;
     case 13:
          fprintf(stream, "	Derived forecasts based on a cluster of ensemble members over a rectangular area at a horizontal\n");
          fprintf(stream, "	level or in a horizontal layer, in a continuous or non-continuous time interval.\n");
          break;
     case 14:
          fprintf(stream, "	Derived forecasts based on a cluster of ensemble members over a circular area at a horizontal\n");
          fprintf(stream, "	level or in a horizontal layer, in a continuous or non-continuous time interval.\n");
          break;
     case 15:
          fprintf(stream, "	Average, accumulation, extreme values or other statistically-processed values over a spatial area\n");
          fprintf(stream, "	at a horizontal level or in a horizontal layer at a point in time.  \n");
     case 20:
          fprintf(stream, "	Radar product\n");
          break;
     case 30:
          fprintf(stream, "	Satellite product, **DEPRECATED**\n");
          break;
     case 31:
          fprintf(stream, "	Satellite product\n");
          break;
     case 32:
	  fprintf(stream, "     Analysis or forecast at a horizontal level or in a horizontal layer at a point in time\n");
	  fprintf(stream, "     for simulate (synthetic) staellite data (see Template 4.32)\n");
	  break;
     case 33:
	  fprintf(stream, "     Individual Ensemble Forecast, control and perturbed, at a horizontal level or in a\n");
	  fprintf(stream, "     horizontal layer at a point in time for simulated (synthetic) satellite data\n");
	  break;
     case 34:
	  fprintf(stream, "     Individual Ensemble Forecast, control and perturbed, at a horizontal level or in a horizontal layer,\n");
	  fprintf(stream, "     in a continuous or non-continuous interval for simulated (synthetic) satellite data.\n");
          break;
     case 35:
	  fprintf(stream, "     Satellite product with or without associated quality values.\n");
          break;
     case 40:
	  fprintf(stream, "     Analysis or forecast at a horizontal level or in a horizontal layer at a point in time \n");
	  fprintf(stream, "     for atmospheric chemical constituents.\n");
          break;
     case 41:
	  fprintf(stream, "     Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer \n");
	  fprintf(stream, "     at a point in time for atmospheric chemical constituents.\n");
          break;
     case 42:
	  fprintf(stream, "     Average, accumulation, and/or extreme values or other statistically processed values at a horizontal level or \n");
	  fprintf(stream, "     in a horizontal layer in a continuous or non-continuous time interval for atmospheric chemical constituents.\n");
          break;
     case 43:
	  fprintf(stream, "     Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer, \n");
	  fprintf(stream, "     in a continuous or non-continuous time interval for atmospheric chemical constituents.\n");
          break;
     case 44:
	  fprintf(stream, "     Analysis or forecast at a horizontal level or in a horizontal layer at a point in time for aerosol.\n");
          break;
     case 45:
	  fprintf(stream, "     Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer, \n");
	  fprintf(stream, "     in a continuous or non-continuous time interval for aerosol.\n");
          break;
     case 46:
	  fprintf(stream, "     Average, accumulation, and/or extreme values or other statistically processed values at a horizontal level or \n");
	  fprintf(stream, "     in a horizontal layer in a continuous or non-continuous time interval for aerosol.\n");
          break;
     case 47:
	  fprintf(stream, "     Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer, \n");
	  fprintf(stream, "     in a continuous or non-continuous time interval for aerosol.\n");
          break;
     case 48:
	  fprintf(stream, "     Analysis or forecast at a horizontal level or in a horizontal layer at a point in time for aerosol.\n");
          break;
     case 49:
	  fprintf(stream, "     Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer, \n");
	  fprintf(stream, "     at a point in time for optical properties of aerosol.\n");
          break;
     case 51:
	  fprintf(stream, "     Categorical forecast at a horizontal level or in a horizontal layer at a point in time.\n");
          break;
     case 53:
	  fprintf(stream, "     Partitioned parameters at a horizontal level or horizontal layer at a point in time.\n");
          break;
     case 54:
	  fprintf(stream, "     Individual ensemble forecast, control and perturbed, at a horizontal level or in a \n");
	  fprintf(stream, "     horizontal layer at a point in time for partitioned parameters.\n");
          break;
     case 55:
          fprintf(stream, "	Spatio-temporal changing tiles at a horizontal level or horizontal layer at a point in time.\n");
       break;
     case 56:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time for spatio-temporal changing tile parameters (DEPRECATED).\n");
       break;
     case 57:
          fprintf(stream, "	Analysis or forecast at a horizontal level or in a horizontal layer at a point in time for atmospheric chemical constituents based on a distribution function.\n");
        break;
     case 58:
          fprintf(stream, "	Individual Ensemble Forecast, Control and Perturbed, at a horizontal level or in a horizontal layer at a point in time interval for Atmospheric Chemical Constituents based on a distribution function.\n");
        break;
     case 59:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time for spatio-temporal changing tile parameters (corrected version of template 4.56 - See Template 4.59)\n");
        break;
     case 60:
          fprintf(stream, "	Individual Ensemble Reforecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time.\n");
        break;
     case 61:
          fprintf(stream, "	Individual Ensemble Reforecast, control and perturbed, at a horizontal level or in a horizontal layer, in a continuous or non-continuous time interval.\n");
        break;
     case 62:
          fprintf(stream, "	Average, Accumulation and/or Extreme values or other Statistically-processed values at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for spatio-temporal changing tiles at a horizontal level or horizontal layer at a point in time.\n");
        break;
     case 63:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for spatio-temporal changing tiles.\n");
        break;
     case 67:
          fprintf(stream, "	Average, accumulation and/or extreme values or other statistically processed values at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for atmospheric chemical constituents based on a distribution function.\n");
        break;
     case 68:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for atmospheric chemical constituents based on a distribution function.\n");
        break;
     case 70:
          fprintf(stream, "	Post-processing analysis or forecast at a horizontal level or in a horizontal layer at a point in time.\n");
        break;
     case 71:
          fprintf(stream, "	Post-processing individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time.\n");
        break;
     case 72:
          fprintf(stream, "	Post-processing average, accumulation, extreme values or other statistically processed values at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval.\n");
        break;
     case 73:
          fprintf(stream, "	Post-processing individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer, in a continuous or non-continuous time interval.\n");
        break;
     case 76:
          fprintf(stream, "	Analysis or forecast at a horizontal level or in a horizontal layer at a point in time for atmospheric chemical constituents with source or sink.\n");
        break;
     case 77:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time for atmospheric chemical constituents with source or sink.\n");
        break;
     case 78:
          fprintf(stream, "	Average, accumulation, and/or extreme values or other statistically processed values at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for atmospheric chemical constituents with source or sink.\n");
        break;
     case 79:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for atmospheric chemical constituents with source or sink.\n");
        break;
     case 80:
          fprintf(stream, "	Analysis or forecast at a horizontal level or in a horizontal layer at a point in time for optical properties of aerosol with source or sink.\n");
        break;
     case 81:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time for optical properties of aerosol with source or sink.\n");
        break;
     case 82:
          fprintf(stream, "	Average, accumulation, and/or extreme values or other statistically processed values at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for aerosol with source or sink.\n");
        break;
     case 83:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for aerosol with source or sink.\n");
        break;
     case 84:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for aerosol with source or sink.\n");
        break;
     case 85:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for aerosol.\n");
        break;
     case 86:
          fprintf(stream, "	Quantile forecasts at a horizontal level or in a horizontal layer at a point in time.\n");
        break;
     case 87:
          fprintf(stream, "	Quantile forecasts at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval.\n");
        break;
     case 88:
          fprintf(stream, "	Analysis or forecast at a horizontal level or in a horizontal layer at a specified local time.\n");
        break;
     case 91:
          fprintf(stream, "	Categorical forecast at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval.\n");
        break;
     case 92:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a specified local time.\n");
        break;
     case 93:
          fprintf(stream, "	Post-processing analysis or forecast at a horizontal level or in a horizontal layer at a specified local time.\n");
        break;
     case 94:
          fprintf(stream, "	Post-processing individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a specified local time.\n");
        break;
     case 95:
          fprintf(stream, "	Average, accumulation, extreme values or other statiscally processed value at a horizontal level or in a horizontal layer at a specified local time.\n");
        break;
     case 96:
          fprintf(stream, "	Average, accumulation, extreme values or other statistically processed values of an individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a specified local time.\n");
        break;
     case 97:
          fprintf(stream, "	Average, accumulation, extreme values or other statistically processed values of post-processing analysis or forecast at a horizontal level or in a horizontal layer at a specified local time.\n");
        break;
     case 98:
          fprintf(stream, "	Average, accumulation, extreme values or other statistically processed values of a post-processing individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a specified local time.\n");
        break;
     case 99:
          fprintf(stream, "	Analysis or forecast at a horizontal level or in a horizontal layer at a point in time for wave 2D spectra with explicit list of frequencies and directions.\n");
        break;
     case 100:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time for wave 2D spectra with explicit list of frequencies and directions.\n");
        break;
     case 101:
          fprintf(stream, "	Analysis or forecast at a horizontal level or in a horizontal layer at a point in time for wave 2D spectra with frequencies and directions defined by formulae.\n");
        break;
     case 102:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time for wave 2D spectra with frequencies and directions defined by formulae.\n");
        break;
     case 103:
          fprintf(stream, "	Analysis or forecast at a horizontal level or in a horizontal layer at a point in time for waves selected by period range.\n");
        break;
     case 104:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time for waves selected by period range.\n");
        break;
     case 105:
          fprintf(stream, "	Anomalies, significance and other derived products from an analysis or forecast in relation to a reference period at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval.\n");
        break;
     case 106:
          fprintf(stream, "	Anomalies, significance and other derived products from an individual ensemble forecast, control and perturbed in relation to a reference period at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval.\n");
        break;
     case 107:
          fprintf(stream, "	Anomalies, significance and other derived products from derived forecasts based on all ensemble members in relation to a reference period at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval.\n");
        break;
     case 108:
          fprintf(stream, "	Analysis or forecast at a horizontal level or in a horizontal layer at a point in time for generic optical products.\n");
        break;
     case 109:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer at a point in time for generic optical products.\n");
        break;
     case 110:
          fprintf(stream, "	Average, accumulation, extreme values or other statistically processed values at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval for generic optical products.\n");
        break;
     case 111:
          fprintf(stream, "	Individual ensemble forecast, control and perturbed, at a horizontal level or in a horizontal layer, in a continuous or non-continuous interval for generic optical products.\n");
        break;
     case 112:
          fprintf(stream, "	Anomalies, significance and other derived products as probability forecasts in relation to a reference period at a horizontal level or in a horizontal layer in a continuous or non-continuous time interval.\n");
        break;
     case 254:
          fprintf(stream, "	CCITTIA5 character string\n");
          break;
     case 1000:
	  fprintf(stream, "	Cross-section of analysis and forecast at a point in time.\n");
          break;
     case 1001:
	  fprintf(stream, "	Cross-section of averaged or otherwise statistically processed analysis or forecast over a range of time.\n");
          break;
     case 1002:
	  fprintf(stream, "	Cross-section of analysis and forecast, averaged or otherwise statistically-processed over latitude or longitude.\n");
          break;
     case 1100:
	  fprintf(stream, "	Hovmoller-type grid with no averaging or other statistical processing.\n");
          break;
     case 1101:
	  fprintf(stream, "	Hovmoller-type grid with averaging or other statistical processing.\n");
          break;
     default:
          fprintf(stream, "	Reserved, Reserved for local use, or Missing \n");
          break;
  }

  if (_prodDefinition != NULL)
    _prodDefinition->print(stream);
  else {
    cerr << "ERROR: PDS::unpack()" << endl;
    cerr << "Product definition template  - " << _prodDefTempNum << " not implemented" << endl;
  }
}

} // namespace Grib2

