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
/////////////////////////////////////////////////
// Template4_pt_33 Template for a satellite product.
//
// Jason Craig, Nov 2023
//
//////////////////////////////////////////////////

#include <iostream>
#include <cmath>

#include <grib2/Template4.33.hh>
#include <grib2/ProdDefTemp.hh>

using namespace std;

namespace Grib2 {

const si32 Template4_pt_33::TEMPLATE4_PT_33_BASE_SIZE = 26;
const si32 Template4_pt_33::BAND_INFO_SIZE = 11;

Template4_pt_33::Template4_pt_33()
: ProdDefTemp()
{

}

Template4_pt_33::Template4_pt_33(Grib2Record::Grib2Sections_t sectionsPtr)
: ProdDefTemp(sectionsPtr)
{

}


Template4_pt_33::~Template4_pt_33 () {


}


int Template4_pt_33::pack (ui08 *templatePtr) 
{
  templatePtr[0] = (ui08) _parameterCategory;

  templatePtr[1] = (ui08) _paramNumber;

  templatePtr[2] = (ui08) _processType;

  templatePtr[3] = (ui08) _backgrdProcessId;

  templatePtr[4] = (ui08) _processID;

  GribSection::_pkUnsigned2(_hoursObsDataCutoff, &(templatePtr[5]));

  templatePtr[7] = (ui08) _minutesObsDataCutoff;

  templatePtr[8] = (ui08) _timeRangeUnit;

  GribSection::_pkUnsigned4(_forecastTime, &(templatePtr[9]));

  templatePtr[13] = (ui08) _numBands;

  // Pack the repeating band information

  ui08 *band_info_ptr = &templatePtr[14];
  
  for (size_t i = 0; i < _numBands; ++i)
  {
    _bands[i].pack(band_info_ptr);
    band_info_ptr += BAND_INFO_SIZE;
  }

  band_info_ptr[0] = (ui08) _ensembleType;

  band_info_ptr[1] = (ui08) _perturbationNum;

  band_info_ptr[2] = (ui08) _numForecasts;

  return GRIB_SUCCESS;

}

int Template4_pt_33::unpack (ui08 *templatePtr) 
{


  _parameterCategory = (si32) templatePtr[0]; 
  
  _paramNumber = (si32) templatePtr[1]; 

  // set strings for parameter, longParameter and units
  setParamStrings();

  // Type of generating process
  _processType = (si32) templatePtr[2]; 

  // Background generating process identifier 
  _backgrdProcessId = (si32) templatePtr[3]; 

  // Analysis or forecast generating processes identifier 
  _processID = (si32) templatePtr[4]; 

  // Hours of observational data cutoff after reference time
  _hoursObsDataCutoff 
            = GribSection::_upkUnsigned2 (templatePtr[5], templatePtr[6]);

  // Minutes of observational data cutoff after reference time
  _minutesObsDataCutoff = (si32) templatePtr[7]; 

  // Indicator of unit of time range
  _timeRangeUnit = (si32) templatePtr[8]; 

  // Forecast Time, In units defined by _timeRangeUnit
  _forecastTime =
       GribSection::_upkUnsigned4 (templatePtr[9], templatePtr[10], templatePtr[11], templatePtr[12]);

  // Number of contributing spectral bands (NB)
  _numBands = (size_t) templatePtr[13];
  
  // Unpack the repeating band information

  ui08 *band_info_ptr = &templatePtr[14];
  
  for (size_t i = 0; i < _numBands; ++i)
  {
     BandInfo bandInfo;
     _bands.push_back(bandInfo);
    _bands[i].unpack(band_info_ptr);
    band_info_ptr += BAND_INFO_SIZE;
  }

  // Type of ensemble forecast (see Code table 4.6)
  _ensembleType = (si32) band_info_ptr[0];

  // Perturbation number
  _perturbationNum = (si32) band_info_ptr[1];

  // Number of forecasts in ensemble
  _numForecasts = (si32) band_info_ptr[2];

  return GRIB_SUCCESS;
}

void Template4_pt_33::getRecSummary (Grib2Record::rec_summary_t *summary) 
{

  summary->discipline = _disciplineNum;
  summary->category = _parameterCategory;
  summary->paramNumber = _paramNumber;
  summary->name.assign (_parameterName->c_str());
  summary->longName.assign (_parameterLongName->c_str());
  summary->units.assign (_parameterUnits->c_str());

  summary->forecastTime = _getTimeUnitName(_forecastTime, _timeRangeUnit);

  char ensemble[50];
  sprintf(ensemble, "Ensemble #%d", _perturbationNum);
  summary->additional.assign (ensemble);

  summary->levelType.assign("SPACE");
  summary->levelTypeLong.assign ("Satellite space");
  summary->levelUnits.assign ("");

  summary->levelVal = 0.0;
  summary->levelVal2 = 0.0;

  sprintf(ensemble, "_PERT%d", _perturbationNum);
  summary->name.append( ensemble );

  return;

}

long int Template4_pt_33::getForecastTime() const
{
   return( _getTimeUnits(_timeRangeUnit) * _forecastTime );
}

void Template4_pt_33::print(FILE *stream) const
{
  fprintf(stream, "Parameter Discipline: %d\n", _disciplineNum);
  fprintf(stream, "Parameter Category: %d\n", _parameterCategory);
  fprintf(stream, "Parameter Number: %d\n", _paramNumber);
  fprintf(stream, "Parameter name '%s' \n", _parameterName->c_str());
  fprintf(stream, "     long name '%s'\n", _parameterLongName->c_str());
  fprintf(stream, "         units '%s'\n", _parameterUnits->c_str());
  _printGeneratingProcessType(stream, _processType);
  fprintf(stream, "Background generating process identifier %d\n", _backgrdProcessId);
  fprintf(stream, "Generating process identifier: %s\n", getGeneratingProcess().c_str());
  fprintf(stream, "Hours of observational data cutoff after reference time %d\n", _hoursObsDataCutoff);
  fprintf(stream, "Minutes of observational data cutoff after reference time %d\n", _minutesObsDataCutoff);
  fprintf(stream, "Forecast time is %d ", _forecastTime);
  _printTimeUnits(stream, _timeRangeUnit);
  fprintf(stream, "Number of contributing spectral bands (NB) %d\n", _numBands);

  for (size_t i = 0; i < _numBands; ++i)
    _bands[i].print(stream, i);
  
  _printEnsembleForecastType(stream, _ensembleType);
   fprintf(stream, "Perturbation Number %d\n", _perturbationNum);
   fprintf(stream, "Number of forecasts in ensemble %d\n", _numForecasts);

  fprintf(stream,"\n\n");
}

//////////////////////////////////////////////////////////////
// BandInfo methods
//////////////////////////////////////////////////////////////

int Template4_pt_33::BandInfo::pack (ui08 *templatePtr) 
{
  GribSection::_pkUnsigned2(_satelliteSeries1, &(templatePtr[0]));
  
  GribSection::_pkUnsigned2(_satelliteSeries2, &(templatePtr[2]));
  
  GribSection::_pkUnsigned2(_instrumentType, &(templatePtr[4]));
  
  templatePtr[6] = (ui08) _centralWaveNumScaleFactor;
  
  GribSection::_pkUnsigned4(_centralWaveNumScaled, &(templatePtr[7]));
  
  return GRIB_SUCCESS;

}

int Template4_pt_33::BandInfo::unpack (ui08 *templatePtr) 
{

  _satelliteSeries1 = GribSection::_upkUnsigned2(templatePtr[0], templatePtr[1]);
  
  _satelliteSeries2 = GribSection::_upkUnsigned2(templatePtr[2], templatePtr[3]);
  
  _instrumentType = GribSection::_upkUnsigned2(templatePtr[4], templatePtr[5]);
  
  _centralWaveNumScaleFactor = (si32) templatePtr[6];
  
  _centralWaveNumScaled = GribSection::_upkUnsigned4(templatePtr[7], templatePtr[8],
						   templatePtr[9], templatePtr[10]);
  
  return GRIB_SUCCESS;
}

void Template4_pt_33::BandInfo::print(FILE *stream, const size_t band_num) const
{
  fprintf(stream, "  Band number %ld\n", band_num);
  fprintf(stream, "    Satellite series 1 is %d\n", _satelliteSeries1);
  fprintf(stream, "    Satellite series 2 is %d\n", _satelliteSeries2);
  fprintf(stream, "    Instrument type is %d\n", _instrumentType);
  fprintf(stream, "    Central wave number scale factor is %d\n", _centralWaveNumScaleFactor);
  fprintf(stream, "    Scaled central wave number is %d\n", _centralWaveNumScaled);
  fprintf(stream,"\n");
}

} // namespace Grib2

