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
// Template4_pt_45 Individual ensemble forecast,
//                control and perturbed, at a
//                horizontal level or in a
//                horizontal layer at a point in time
//                For Aerosol
//
// Jason Craig, Nov 2023
//
//////////////////////////////////////////////////

#include <iostream>
#include <cmath>

#include <grib2/Template4.45.hh>
#include <grib2/ProdDefTemp.hh>

using namespace std;

namespace Grib2 {

const si32 Template4_pt_45::TEMPLATE4_PT_45_SIZE = 50;

Template4_pt_45::Template4_pt_45()
: ProdDefTemp()
{

}

Template4_pt_45::Template4_pt_45(Grib2Record::Grib2Sections_t sectionsPtr)
: ProdDefTemp(sectionsPtr)
{

}


Template4_pt_45::~Template4_pt_45 () {


}


int Template4_pt_45::pack (ui08 *templatePtr) 
{
  setParamStrings();

  templatePtr[0] = (ui08) _parameterCategory;

  templatePtr[1] = (ui08) _paramNumber;

  GribSection::_pkUnsigned2(_chemical, &(templatePtr[2]));

  templatePtr[4] = (ui08) _sizeType;

  templatePtr[5] = (ui08) _scaleFactorFirstSize;

  GribSection::_pkUnsigned4(_scaleValFirstSize, &(templatePtr[6]));

  templatePtr[10] = (ui08) _scaleFactorSecondSize;

  GribSection::_pkUnsigned4(_scaleValSecondSize, &(templatePtr[11]));

  templatePtr[15] = (ui08) _processType;

  templatePtr[16] = (ui08) _backgrdProcessId;

  templatePtr[17] = (ui08) _processID;

  GribSection::_pkUnsigned2(_hoursObsDataCutoff, &(templatePtr[18]));

  templatePtr[20] = (ui08) _minutesObsDataCutoff;

  templatePtr[21] = (ui08) _timeRangeUnit;

  GribSection::_pkUnsigned4(_forecastTime, &(templatePtr[22]));

  templatePtr[26] = (ui08) _firstSurfaceType;

  templatePtr[27] = (ui08) _scaleFactorFirstSurface;

  GribSection::_pkUnsigned4(_scaleValFirstSurface, &(templatePtr[28]));

  templatePtr[32] = (ui08) _secondSurfaceType;

  templatePtr[33] = (ui08) _scaleFactorSecondSurface;

  GribSection::_pkUnsigned4(_scaleValSecondSurface, &(templatePtr[34]));

  templatePtr[38] = (ui08) _ensembleType;

  templatePtr[39] = (ui08) _perturbationNum;

  templatePtr[40] = (ui08) _numForecasts;

  return GRIB_SUCCESS;

}

int Template4_pt_45::unpack (ui08 *templatePtr) 
{

  _parameterCategory = (si32) templatePtr[0]; 
  
  _paramNumber = (si32) templatePtr[1]; 

  // set strings for parameter, longParameter and units
  setParamStrings();

  _chemical = GribSection::_upkUnsigned2 (templatePtr[2], templatePtr[3]);

  // Type of interval for first and second size
  _sizeType = (si32) templatePtr[4]; 

  // Scale factor of first size
  _scaleFactorFirstSize = (si32) templatePtr[5];

  // Scale value of first size
  _scaleValFirstSize =
       GribSection::_upkUnsigned4 (templatePtr[6], templatePtr[7], templatePtr[8], templatePtr[9]);

  // Scale factor of second size
  _scaleFactorSecondSize = (si32) templatePtr[10];

  // Scale value of second size
  _scaleValSecondSize =
       GribSection::_upkUnsigned4 (templatePtr[11], templatePtr[12], templatePtr[13], templatePtr[14]);

  // Type of generating process
  _processType = (si32) templatePtr[15]; 

  // Background generating process identifier 
  _backgrdProcessId = (si32) templatePtr[16]; 

  // Analysis or forecast generating processes identifier 
  _processID = (si32) templatePtr[17]; 

  // Hours of observational data cutoff after reference time
  _hoursObsDataCutoff 
            = GribSection::_upkUnsigned2 (templatePtr[18], templatePtr[19]);

  // Minutes of observational data cutoff after reference time
  _minutesObsDataCutoff = (si32) templatePtr[20]; 

  // Indicator of unit of time range
  _timeRangeUnit = (si32) templatePtr[21]; 

  // Forecast Time, In units defined by _timeRangeUnit
  _forecastTime =
       GribSection::_upkUnsigned4 (templatePtr[22], templatePtr[23], templatePtr[24], templatePtr[25]);

  // Type of first fixed surface
  _firstSurfaceType = (si32) templatePtr[26]; 

  // Scale factor of first fixed surface
  _scaleFactorFirstSurface = (si32) templatePtr[27];

  // Scale value of first fixed surface
  _scaleValFirstSurface =
       GribSection::_upkUnsigned4 (templatePtr[28], templatePtr[29], templatePtr[30], templatePtr[31]);

  // Type of second fixed surface
  _secondSurfaceType = (si32) templatePtr[32]; 

  // Scale factor of second fixed surface
  _scaleFactorSecondSurface = (si32) templatePtr[33];

  // Scale value of second fixed surface
  _scaleValSecondSurface =
       GribSection::_upkUnsigned4 (templatePtr[34], templatePtr[35], templatePtr[36], templatePtr[37]);

  // Type of ensemble forecast (see Code table 4.6)
  _ensembleType = (si32) templatePtr[38];

  // Perturbation number
  _perturbationNum = (si32) templatePtr[39];

  // Number of forecasts in ensemble
  _numForecasts = (si32) templatePtr[40];

  return (GRIB_SUCCESS);
}

void Template4_pt_45::getRecSummary (Grib2Record::rec_summary_t *summary) 
{

  int ndex, ndex2;
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

  ndex = _getSurfaceIndex(_firstSurfaceType);
  if ((ndex < 0)) {
    summary->levelType.assign("UNKNOWN");
    summary->levelTypeLong.assign ("unknown primary surface type");
    summary->levelUnits.assign ("");
  } else {
    summary->levelType.assign (_surface[ndex].name);
    summary->levelTypeLong.assign (_surface[ndex].comment);
    summary->levelUnits.assign (_surface[ndex].unit);
  }

  summary->levelVal = _scaleValFirstSurface;
  if(_scaleFactorFirstSurface > 0 && _scaleFactorFirstSurface < 127)
    summary->levelVal /= pow(10.0, _scaleFactorFirstSurface);
  if(_scaleFactorFirstSurface > 127 && _scaleFactorFirstSurface != 255)
    summary->levelVal *= pow(10.0, _scaleFactorFirstSurface & 127);

  ndex2 = _getSurfaceIndex(_secondSurfaceType);
  if (_secondSurfaceType == 255 || (ndex2 < 0))
    summary->levelVal2 = -999;
  else
    if(ndex2 != ndex) {
      summary->levelType.append ("-");
      summary->levelType.append (_surface[ndex2].name);
    } else {
      if(_secondSurfaceType == 105 || _secondSurfaceType == 104 || _secondSurfaceType == 107) {
	summary->levelType.append("_LAYER");
	summary->levelTypeLong.append(" layer");
      }
      summary->levelVal2 = _scaleValSecondSurface;
      if(_scaleFactorSecondSurface > 0 && _scaleFactorSecondSurface < 127)
	summary->levelVal2 /= pow(10.0, _scaleFactorSecondSurface);
      if(_scaleFactorSecondSurface > 127 && _scaleFactorSecondSurface != 255)
	summary->levelVal2 *= pow(10.0, _scaleFactorSecondSurface & 127);
    }

  summary->name.append( "_" );
  summary->name.append( _getAtmoChemicalName(_chemical) );
  summary->longName.append( " - " );
  summary->longName.append( _getAtmoChemical(_chemical) );

  sprintf(ensemble, "_PERT%d", _perturbationNum);
  summary->name.append( ensemble );

  return;
}

long int Template4_pt_45::getForecastTime() const
{
   return( _getTimeUnits(_timeRangeUnit) * _forecastTime );
}


void Template4_pt_45::print(FILE *stream) const
{
  int ndex;

  fprintf(stream, "Parameter Discipline: %d\n", _disciplineNum);
  fprintf(stream, "Parameter Category is %d\n", _parameterCategory);
  fprintf(stream, "Parameter Number is %d\n", _paramNumber);
  fprintf(stream, "Parameter name '%s' \n", _parameterName->c_str());
  fprintf(stream, "     long name '%s'\n", _parameterLongName->c_str());
  fprintf(stream, "         units '%s'\n", _parameterUnits->c_str());
  fprintf(stream, " Type of Atmospheric Chemical / Aerosol: %s\n", _getAtmoChemical(_chemical).c_str() );
  fprintf(stream, "  Type of Interval for first and second size: %d\n", _sizeType);
  fprintf(stream, "    Scale factor of first size %d\n", _scaleFactorFirstSize);
  fprintf(stream, "    Scale value of first size %d\n", _scaleValFirstSize);
  fprintf(stream, "    Scale factor of second size %d\n", _scaleFactorSecondSize);
  fprintf(stream, "    Scale value of second size %d\n", _scaleValSecondSize);
  fprintf(stream, "  Type of Interval for first and second size: \n");
  _printInterval(stream, _sizeType, _scaleFactorFirstSize, _scaleValFirstSize, _scaleFactorSecondSize, _scaleValSecondSize);
  _printGeneratingProcessType(stream, _processType);
  fprintf(stream, "Background generating process identifier %d\n", _backgrdProcessId);
  fprintf(stream, "Generating process identifier: %s\n", getGeneratingProcess().c_str());
  fprintf(stream, "Hours of observational data cutoff after reference time %d\n", _hoursObsDataCutoff);
  fprintf(stream, "Minutes of observational data cutoff after reference time %d\n", _minutesObsDataCutoff);
  fprintf(stream, "Forecast time is %d ", _forecastTime);
  _printTimeUnits(stream, _timeRangeUnit);
  
  fprintf(stream, "Type of first fixed surface is %d\n", _firstSurfaceType);
  ndex = _getSurfaceIndex(_firstSurfaceType);
  if ((ndex < 0) || (_firstSurfaceType == 255))
    fprintf (stream, "    unknown/missing primary surface type\n");
  else {
    fprintf(stream, "    Surface name '%s'\n", _surface[ndex].name.c_str());
    fprintf(stream, "       long name '%s'\n", _surface[ndex].comment.c_str());
    fprintf(stream, "           units '%s'\n", _surface[ndex].unit.c_str());
    fprintf(stream, "    Scale factor of first fixed surface %d\n", _scaleFactorFirstSurface);
    fprintf(stream, "    Scale value of first fixed surface %d\n", _scaleValFirstSurface);
  }
  fprintf(stream, "Type of second fixed surface %d\n", _secondSurfaceType);
  ndex = _getSurfaceIndex(_secondSurfaceType);
  if ((ndex < 0) || (_secondSurfaceType == 255))
    fprintf (stream, "    unknown/missing second surface type\n");
  else {
    fprintf(stream, "    Surface name '%s'\n", _surface[ndex].name.c_str());
    fprintf(stream, "       long name '%s'\n", _surface[ndex].comment.c_str());
    fprintf(stream, "           units '%s'\n", _surface[ndex].unit.c_str());
    fprintf(stream, "    Scale factor of second fixed surface %d\n", _scaleFactorSecondSurface);
    fprintf(stream, "    Scale value of second fixed surface %d\n", _scaleValSecondSurface);
  }
  _printEnsembleForecastType(stream, _ensembleType);
   fprintf(stream, "Perturbation Number %d\n", _perturbationNum);
   fprintf(stream, "Number of forecasts in ensemble %d\n", _numForecasts);
   fprintf(stream,"\n\n");
}

} // namespace Grib2

