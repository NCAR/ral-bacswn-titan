// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// © University Corporation for Atmospheric Research (UCAR) 2009-2010. 
// All rights reserved.  The Government's right to use this data and/or 
// software (the "Work") is restricted, per the terms of Cooperative 
// Agreement (ATM (AGS)-0753581 10/1/08) between UCAR and the National 
// Science Foundation, to a "nonexclusive, nontransferable, irrevocable, 
// royalty-free license to exercise or have exercised for or on behalf of 
// the U.S. throughout the world all the exclusive rights provided by 
// copyrights.  Such license, however, does not include the right to sell 
// copies or phonorecords of the copyrighted works to the public."   The 
// Work is provided "AS IS" and without warranty of any kind.  UCAR 
// EXPRESSLY DISCLAIMS ALL OTHER WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE.  
//  
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
#include <toolsa/copyright.h>
/**
 * @file ParmNetCdfOut.cc
 * @brief Algorithm parameters all in one place
 */

//----------------------------------------------------------------
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>
#include <ConvWx/ParmNetCdfOut.hh>

//----------------------------------------------------------------
ParmNetCdfOut::ParmNetCdfOut(void) :
  // _conventions(""),
  _institution("UCAR"),
  _references(""),
  // _source(""),
  // _title(""),
  _comment(""),
  _mdv_ncf_fields(),
  _compress_data(true),
  _compression_level(9),
  _baseName("ncfdata"),
  _write_to_day_dir(false),
  _output_as_forecast(true),
  _use_iso8601_filename_convention(false),
  _use_TDS_filename_convention(false),
  _use_output_filename(false),
  _output_filename("outputfile.nc"),
  _output_file_prefix(""),
  _output_file_suffix(""),
  _file_format(CLASSIC),
  _output_latlon_arrays(true),
  _output_start_end_times(true),
   _output_mdv_attributes(false),
  _output_mdv_chunks(false),
  // _radial_file_type(FILE_TYPE_CF),
  _force_ldata_as_valid(false),
  _earth_radius_km(-1)
{
}

//----------------------------------------------------------------
ParmNetCdfOut::ParmNetCdfOut(
			     // const std::string &conventions,
			     const std::string &institution,
			     const std::string &references,
			     // const std::string &source,
			     // const std::string &title,
			     const std::string &comment,
			     const std::vector<NcfData_t> &mdv_ncf_fields,
			     bool compress_data,
			     int compression_level,
			     const std::string &baseName,
			     bool write_to_day_dir,
			     bool output_as_forecast,
			     bool use_iso8601_filename_convention,
			     bool use_TDS_filename_convention,
			     bool use_output_filename,
			     const std::string &output_filename,
			     const std::string &output_file_prefix,
			     const std::string &output_file_suffix,
			     const NcFormat_t file_format,
			     bool output_latlon_arrays,
			     bool output_start_end_times,
			     bool output_mdv_attributes,
			     bool output_mdv_chunks,
			     // const RadialFileType_t radial_file_type,
			     bool force_ldata_as_valid,
			     double earth_radius_km
			     ) :
  // _conventions(conventions),
  _institution(institution),
  _references(references),
  // _source(source),
  // _title(title),
  _comment(comment),
  _mdv_ncf_fields(mdv_ncf_fields),
  _compress_data(compress_data),
  _compression_level(compression_level),
  _baseName(baseName),
  _write_to_day_dir(write_to_day_dir),
  _output_as_forecast(output_as_forecast),
  _use_iso8601_filename_convention(use_iso8601_filename_convention),
  _use_TDS_filename_convention(use_TDS_filename_convention),
  _use_output_filename(use_output_filename),
  _output_filename(output_filename),
  _output_file_prefix(output_file_prefix),
  _output_file_suffix(output_file_suffix),
  _file_format(file_format),
  _output_latlon_arrays(output_latlon_arrays),
  _output_start_end_times(output_start_end_times),
   _output_mdv_attributes(output_mdv_attributes),
  _output_mdv_chunks(output_mdv_chunks),
  // _radial_file_type(radial_file_type),
  _force_ldata_as_valid(force_ldata_as_valid),
  _earth_radius_km(earth_radius_km)
{
}

//----------------------------------------------------------------
ParmNetCdfOut::~ParmNetCdfOut()
{
}

//----------------------------------------------------------------
void ParmNetCdfOut::print(void) const
{    
  printf("\tNetCdfOut:\n");
}

