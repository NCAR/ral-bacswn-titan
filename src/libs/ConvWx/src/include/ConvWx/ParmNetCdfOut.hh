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
 * @file ParmNetCdfOut.hh
 * @brief Parameters for NetCDF output
 * @class ParmNetCdfOut
 * @brief Parameters for NetCDF output
 */

# ifndef    PARM_NETCDF_OUT_HH
# define    PARM_NETCDF_OUT_HH

#include <string>

//---------------------------------------------------------------
class ParmNetCdfOut
{
public:

  typedef enum {
    DATA_PACK_NONE, DATA_PACK_BYTE, DATA_PACK_SHORT
  } DataPack_t;

  typedef enum {
    CLASSIC, NC64BIT, NETCDF4, NETCDF4_CLASSIC
  } NcFormat_t;

  // typedef enum {
  //   FILE_TYPE_CF, // basic CF
  //   FILE_TYPE_CF_RADIAL, // New CF Radial format
  //   FILE_TYPE_DORADE, // NCAR Dorade format
  //   FILE_TYPE_UF // universal format
  // } RadialFileType_t;

  typedef struct
  {
    std::string mdv_field_name;
    std::string ncf_field_name;
    std::string ncf_standard_name;
    std::string ncf_long_name;
    std::string ncf_units;
    bool do_linear_transform;
    double linear_multiplier;
    double linear_const;
    DataPack_t packed_data_type;
  } NcfData_t;

  /**
   * default constructor. values are not set
   */
  ParmNetCdfOut(void);

  /**
   * Constructor with all values passed in
   */
  ParmNetCdfOut(
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
		// const RadialFileType_t adial_file_type,
		bool force_ldata_as_valid,
		double earth_radius_km);

  /**
   * Destructor
   */
  virtual ~ParmNetCdfOut(void);

  /**
   * Debug print
   */
  void print(void) const;

  //  std::string _conventions;  /**< Global attribute */
  std::string _institution;  /**< Global attribute */
  std::string _references;  /**< Global attribute */
  // std::string _source;  /**< Global attribute */
  // std::string _title;  /**< Global attribute */
  std::string _comment;  /**< Global attribute */
  std::vector<NcfData_t> _mdv_ncf_fields;  /**< Field list */
  bool _compress_data;  /**< Option to compress data, only for NETCDF4 and NETCDF4_CLASSIC */
  int _compression_level;  /**< Compression level from 1 to 9 with 9 being the greates. */
  std::string _baseName;  /**< Base name of netCDF files */
  bool _write_to_day_dir; /**< Option to add day dir into the output file path */
  bool _output_as_forecast; /**< If true, the final output dir will be 'output_dir/g_hhmmss */

  /**
   * If true the output filename uses the basename, followed by ISO 8601 timestamp convention
   */
  bool _use_iso8601_filename_convention;
  
  /**
   * If true the output filename uses the basename, followed by a Thredds Data Server
   * (TDS) compliant timestamp convention.  The TDS doesn't like ':', '-' in name 
   * (basename.20080522_140000.nc
   */
  bool _use_TDS_filename_convention;

  /**
   * If true the _output_filename is used, otherwise it is created with the baename and a timestamp
   * User specifies the output file name"
   */
  bool _use_output_filename;

  std::string _output_filename; /**< User-specified output filename */
  std::string _output_file_prefix;  /**< User-specified output file prefix, comes before basename */
  std::string _output_file_suffix; /**< User-specified output file suffix, comes before .nc (automatic) */

  /**
   * NetCDF file format\n
   * CLASSIC = netCDF classic format\n
   * NC64BIT = netCDF 64-bit offset format\n
   * NETCDF4 =  netCDF4 using HDF5 format\n
   * NETCDF4_CLASSIC netCDF4 using HDF5\n"
   */
  NcFormat_t _file_format;

  /**
   * If true latitude and longitude arrays of each grid point are output\n
   * The CF convention requires that these arrays are present in the netCDF file; however, the information is redundant since the lat and lon arrays could be constructed using the other projection and grid information required with a gridded data field
   */
  bool _output_latlon_arrays;

  /**
   * If true start_time and end_time are are output. Start and end times may provide redundant information in the Mdv file or may not be relevant to the output data
   */
  bool _output_start_end_times;


  /**
   * Option to output non-CF compliant MDV attributes. If true, MDV attributes which are not CF compliant will be output. This will facilitate the translation of the data back into MDV with the minimal loss of information.
   */
  bool _output_mdv_attributes;

  /**
   * Option to output non-CF compliant MDV chunks.
   * If true, MDV chunks will be included as byte binary variables.
   */
  bool _output_mdv_chunks;

  // RadialFileType_t _radial_file_type;  /**< Output format for polar radar data */

  /**
   * Option to force output ldata info file to use valid time.
   * If true, the ldata info time fields will be generated using the centroid time.  If false, it will determine how to create the time fields based on the collection type and forecast time.
   */
  bool _force_ldata_as_valid;

  /**
   * Optional earth radius setting to use.  If negative the param has no effect
   */
  double _earth_radius_km;

protected:
private:  
};

# endif
