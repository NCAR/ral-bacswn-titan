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
 * @file Template5.0.hh
 * @author Jason Craig
 * @date   Nov 2023
 */

#ifndef _GRIB2_TEMPLATE_5_PT_4 
#define _GRIB2_TEMPLATE_5_PT_4

#include <grib2/DataRepTemp.hh>
#include <cstdio>

using namespace std;

namespace Grib2 {

/** 
 * @class Template5_pt_4
 *
 * Grid point data - Simple Packing 
 *
 */
class Template5_pt_4: public DataRepTemp {

public:

  /** @brief Constructor for use in passing to GribFile.addField()
   *  @param[in] decimalScaleFactor Floating point precision to use in simple packing
   *  @param[in] precision Requested Precision,
   * 1 = 32-Bit, 2 = 64-Bit, 3 = 128-Bit */
  Template5_pt_4(si32 precision = 1);

  /** 
   * @brief Internal constructor used during grib2 decoding
   *
   * @param[in] sectionsPtr Struct containing pointers to all
   * sections that come before this one.
   */
  Template5_pt_4(Grib2Record::Grib2Sections_t sectionsPtr);

  virtual ~Template5_pt_4();

  /** @brief Unpack a Data Representation Template 
   *  @param[in] projPtr Pointer to start of template
   *  @return Either GRIB_SUCCESS or GRIB_FAILURE */
  int unpack (ui08 *projPtr);

  /** @brief Pack up this Data Representation Template
   *  @param[in] projPtr Pointer to start of location to pack template
   *  @return Either GRIB_SUCCESS or GRIB_FAILURE */
  int pack (ui08 *projPtr);

  /** @brief Print to stream/file all information for this template */
  void print (FILE *) const;

  /** @brief Get the size of the packed template */
  virtual si32 getTemplateSize() { return TEMPLATE5_PT_4_SIZE; };


protected:


private: 
    static const si32 TEMPLATE5_PT_4_SIZE;

};

} // namespace Grib2

#endif

