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
 * @file Template4.34.hh
 * @author Jason Craig
 * @date   Nov 2023
 */

#ifndef _GRIB2_TEMPLATE_4_PT_34 
#define _GRIB2_TEMPLATE_4_PT_34

#include <dataport/port_types.h>
#include <grib2/ProdDefTemp.hh>
#include <grib2/constants.h>

using namespace std;

namespace Grib2 {

/** 
 * @class Template4_pt_34
 *
 * A derived Product Definition Template class for synthetic satellite from ensemble products.
 *
 * See http://www.nco.ncep.noaa.gov/pmb/docs/grib2/grib2_temp4-34.shtml
 *
 * @note The pack and unpack routines are static methods in Grib2::GribSection
 */
class Template4_pt_34: public ProdDefTemp {

public:
  /**
   * @class BandInfo
   *
   * Information about each of the bands in the satellite.
   *
   * See http://www.nco.ncep.noaa.gov/pmb/docs/grib2/grib2_temp4-34.shtml
   */

  class BandInfo
  {
  public:
    /** @brief Unpack a Product Definition Template 
     *  @param[in] projPtr Pointer to start of template
     *  @return Either GRIB_SUCCESS or GRIB_FAILURE */
    int unpack (ui08 *projPtr);

    /** @brief Pack up this Product Definition Template
     *  @param[in] projPtr Pointer to start of location to pack template
     *  @return Either GRIB_SUCCESS or GRIB_FAILURE */
    int pack (ui08 *projPtr);

    /** @brief Print to stream/file all information for this template */
    void print (FILE *stream, const size_t band_num) const;

  private :

    /** @brief Satellite series of band nb (code table defined by originating/generating centre) */
    si32 _satelliteSeries1;
    /** @brief Satellite series of band nb (code table defined by originating/generating centre) */
    si32 _satelliteSeries2;
    /** @brief Instrument types of band nb (code table defined by originating/generating centre)  */
    si32 _instrumentType;
    /** @brief Scale factor of central wave number of band nb */
    si32 _centralWaveNumScaleFactor;
    /** @brief Scaled value of central wave number of band nb (units: m-1) */
    si32 _centralWaveNumScaled;
  };
  
    
  /**
   * @brief Default constructor for use in passing object to GribFile.addField()
   *
   * @note After creation.. all public values variables below should be set
   * before passing this object to GribFile. 
   */
  Template4_pt_34();

  /**
   * @brief This constructor is used internally to the grib2 library when reading
   * a grib2 file.
   */
  Template4_pt_34(Grib2Record::Grib2Sections_t sectionsPtr);

  virtual ~Template4_pt_34();

  /** @brief Unpack a Product Definition Template 
   *  @param[in] projPtr Pointer to start of template
   *  @return Either GRIB_SUCCESS or GRIB_FAILURE */
  int unpack (ui08 *projPtr);

  /** @brief Pack up this Product Definition Template
   *  @param[in] projPtr Pointer to start of location to pack template
   *  @return Either GRIB_SUCCESS or GRIB_FAILURE */
  int pack (ui08 *projPtr);

  /** @brief Print to stream/file all information for this template */
  void print (FILE *) const;

  /** @brief Get the record summary for this grib record 
   *  @param[out] summary Record summary struct */
  void getRecSummary (Grib2Record::rec_summary_t *summary);


  /** @brief Get the forecast time for this grib record 
   *  @return Forecast lead time in seconds */
  long int getForecastTime() const;
  

  /** @brief Get the size of the packed derived template class. 
   *  @return Size of packed template in bytes */ 
  virtual si32 getTemplateSize() { return TEMPLATE4_PT_34_BASE_SIZE + (_numBands * BAND_INFO_SIZE) + (_numTimeIntervals * 12); };

  /** @brief Type of generating process */
  si32 _processType;      
  /** @brief Background generating process identifier */
  si32 _backgrdProcessId;   
  /** @brief Hours of observational data cutoff after reference time */
  si32 _hoursObsDataCutoff;      
  /** @brief Minutes of observational data cutoff after reference time */
  si32 _minutesObsDataCutoff;    
  /** @brief Indicator of unit of time range  */
  si32 _timeRangeUnit;           
  /** @brief In units defined by _timeUintRange */
  si32 _forecastTime;  
  /** @brief Number of contributing spectral bands (NB) */
  ui32 _numBands;
  /** @brief Repeating information defining each band */
  vector< BandInfo > _bands;
  /** @brief Type of ensemble forecast (see Code table 4.6) */
  si32 _ensembleType;            
  /** @brief Perturbation number */
  si32 _perturbationNum;         
  /** @brief Number of forecasts in ensemble */
  si32 _numForecasts;   
  /** @brief Year of end of overall time interval */
  si32 _year;                    
  /** @brief Month of end of overall time interval */
  si32 _month;                   
  /** @brief Day of end of overall time interval */
  si32 _day;                     
  /** @brief Hour of end of overall time interval */
  si32 _hour;                    
  /** @brief Minute of end of overall time interval */
  si32 _minute;                  
  /** @brief Second of end of overall time interval */
  si32 _second;                  
  /** @brief Number of time range specifications describing
   * the time intervals used to calculate the statistically processed field */
  si32 _numTimeIntervals;
  /** @brief Total number of data values missing in the statistical process */
  si64 _numMissingVals;
  /** @brief 1 or more repeating intervals based on _numTimeIntervals.
   * See Grib2::ProdDefTemp::interval_t */
  vector <interval_t> _interval;

protected:

private: 
  
  static const si32 TEMPLATE4_PT_34_BASE_SIZE;

  static const si32 BAND_INFO_SIZE;

};

} // namespace Grib2

#endif

