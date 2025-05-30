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
 *
 * @file GoesRProduct.hh
 *
 * @class GoesRProduct
 *
 * This is a base class for reading and processing a GOES-R level-2 
 * product contained in an NetCDF file.
 *  
 * @author G. Cunning
 *
 * @date 10 May 2019
 *
 */

#ifndef GOESRPRODUCT_HH
#define GOESRPRODUCT_HH

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <Ncxx/Ncxx.hh>


/** 
 * @class GoesRProduct
 */

class GoesRProduct
{
 public:

  // header string for logging messages
  static const std::string ERROR_STR;
  static const std::string WARN_STR;
  static const std::string INFO_STR;

  static const double MISSING_DATA_VALUE;
  static const double BAD_DATA_VALUE;
  static const int DATA_QUALITY_GOOD;

  static const std::string LEVEL_1B_NAME_PATTERN;
  static const std::string LEVEL_2_NAME_PATTERN;
  
  static const char* DATA_QUALITY_FLAG;
  static const char* DQF_OVERALL;
  static const char* DQF_RETRIEVAL;
  static const char* DQF_SKINTEMP;

  class DataContainer {
  public:
    double ncMissingVal;
    std::vector< float > values;
    std::vector< int > validRange;
    float scaleFactor;
    float offset;
    std::string units;
    std::string longName;
    std::string standardName;
    std::string unsignedVal;
    std::string resolution;
    std::string coordinates;
    std::string gridMapping;
    std::string cellMethods;
    int outlierPixelCount;
    double minValue;
    double maxValue;
    double meanValue;
    double stdDevValue;
 };
  
  class CatDataContainer {
  public:
    int ncMissingVal;
    std::vector< int > values;
    std::map< int, std::string > categories;
    std::map< int, std::string > meanings;
    std::vector< int > validRange;
    std::string units;
    std::string longName;
    std::string standardName;
    std::string unsignedVal;
    std::string resolution;
    std::string coordinates;
    std::string gridMapping;
    std::string cellMethods;
  };
    
  class QcDataContainer {
  public:
    int ncMissingVal;
    std::vector< int > values;
    std::map< int, std::string > flags;
    std::map< int, std::pair<std::string, int > > flagsAndMasks;
    std::vector< int > validRange;
    std::string units;
    std::string longName;
    std::string standardName;
    std::string unsignedVal;
    std::string resolution;
    std::string coordinates;
    std::string gridMapping;
    std::string cellMethods;
  };


  GoesRProduct();
  GoesRProduct(const GoesRProduct &from);
  virtual ~GoesRProduct();
  
  virtual void clearAndReset();
  virtual bool applyQcTests() = 0;
  bool filterPath(const string &scene, const string &product, const std::string &path);
  void printInfo();
  bool latLon2XY(double lat, double lon, size_t& col_num, size_t& line_num);
  bool readFile();

  inline bool isOkay() { return _okay; }

  inline void addToAuxStr(const std::string& in) { _auxiliaryInfo += in; }
  
  inline size_t getNumX() const { return _numX; };
  inline size_t getNumY() const { return _numY; };
  inline std::string getErrStr() const { return _errStr; }
  inline time_t getBeginTime() const { return _beginTime; }
  inline time_t getMidpointTime() const { return _midpointTime; }
  inline time_t getEndTime() const { return _endTime; }
  inline double getSatSubpointLon() const { return _nominalSatSubpointLon; }
  inline double getSatSubpointLat() const { return _nominalSatSubpointLat; }
  inline double getSatHeight() const { return _nominalSatHeight; }
  
  inline void setFilePath(const std::string &file_path) { _filePath = file_path; }
  inline void setFileNamePattern(const std::string &pattern) { _namePattern = pattern; }

  inline void setDebugLevel(int level)
  {
    if((level >= static_cast<int>(GoesRProduct::DEBUG_OFF)) &&
       (level <= static_cast<int>(GoesRProduct::DEBUG_VERBOSE))) {
      _debugLevel = static_cast<debug_type_t>(level);
     }
    else {
      cerr << "Unknown debug level. setting level to DEBUG_OFF" << endl;
      _debugLevel = GoesRProduct::DEBUG_OFF;
    }
  }

  virtual void setCheckQualityField(bool run) {};
  virtual void setBadCountTest(bool run, int threshold) {};
  //  virtual void _setCheckQualityField(bool run) = 0;
  //virtual void _setBadCountTest(bool run, int threshold) = 0;

  const DataContainer& getDataContainer() { return _dataContainer; }
  const std::map<std::string, DataContainer*>& getDataFields() { return _outDataFields; }
  const DataContainer* getDataFieldByID(const std::string& id) { return _outDataFields.at(id); }

  inline bool isCatData() {  return _isCatData; }
  const CatDataContainer& getCatDataContainer() { return _catDataContainer; }
  const std::map<std::string, CatDataContainer*>& getCatDataFields() {return _outCatDataFields; }
  const CatDataContainer* getCatDataFieldByID(const std::string& id) { return _outCatDataFields.at(id); }

  const QcDataContainer& getQcDataContainer() { return _qcDataContainer; }
  const std::map<std::string, QcDataContainer*>& getQcDataFields() { return _outQcDataFields; }
  const QcDataContainer* getQcDataFieldByID(const std::string& id) { return _outQcDataFields.at(id); }

  
  
protected:
  
  typedef enum {
    DEBUG_OFF = 0,
    DEBUG_NORM = 1,
    DEBUG_VERBOSE = 2
  } debug_type_t;

  typedef enum {
    PRODUCT_LEVEL1B_RADIANCES,
    PRODUCT_AEROSOL_DETECTION,
    PRODUCT_AEROSOL_OPTICAL_DEPTH,
    PRODUCT_CLOUD_TOP_PHASE,
    PRODUCT_CLOUD_TOP_HEIGHT,
    PRODUCT_CLOUD_TOP_TEMPERATURE,
    PRODUCT_CLOUD_TOP_PRESSURE,
    PRODUCT_DERIVED_STABILITY_INDICES,
    PRODUCT_TOTAL_PRECIPITABLE_WATER,
    PRODUCT_CLEAR_SKY_MASK,
    PRODUCT_FIRE_CHARACTERIZATION,
    PRODUCT_LAND_SURFACE_TEMPERATURE,
    PRODUCT_CLOUD_OPTICAL_DEPTH,
    PRODUCT_CLOUD_PARTICLE_SIZE,
    PRODUCT_DERIVED_MOTION_WINDS,
    PRODUCT_GLOBAL_LIGHTNING,
    PRODUCT_LEGACY_VERTICAL_MOISTURE_PROFILE,
    PRODUCT_LEGACY_VERTICAL_TEMPERATURE_PROFILE,
    PRODUCT_RAINFALL_RATE_QPE,
    PRODUCT_SEA_SURFACE_TEMPERATURE ,
    PRODUCT_VOLCANIC_ASH
  } product_type_t;
   
  typedef enum {
    SCAN_FULL_DISK,
    SCAN_CONUS,
    SCAN_MESOSCALE
  } scan_type_t;

  // string content of global attributes
  class GlobalAttributes {
  public:
    std::string namingAuthority;
    std::string conventions;
    std::string metadataConventions;
    std::string standardNameVocabulary;
    std::string institution;
    std::string project;
    std::string productionSite;
    std::string productionEnvironment;
    std::string spatialResolution;
    std::string orbitalSlot;
    std::string platformID;
    std::string instrumentType;
    std::string sceneID;
    std::string instrumentID;
    std::string datasetName;
    std::string isoSeriesMetadataID;
    std::string title;
    std::string summary;
    std::string keywords;
    std::string keywordsVocabulary;
    std::string license;
    std::string processingLevel;
    std::string dateCreated;
    std::string cdmDataType;
    std::string timelineID;
    std::string timeCoverageStart;
    std::string timeCoverageEnd;
    std::string productionDataSource;
  };


  class QcCountTestParams
  {
  public:
    bool run;
    int threshold;
  };

  bool _isCatData;
  std::map<std::string, DataContainer*> _outDataFields;
  std::map<std::string, CatDataContainer*> _outCatDataFields;
  std::map<std::string, QcDataContainer*> _outQcDataFields;
  
  DataContainer _dataContainer;
  CatDataContainer _catDataContainer;
  QcDataContainer _qcDataContainer;

  int _badPixelCount;
  bool _passMissingPixelTest;
  bool _passBadCountTest;
  static const std::string _className;
  debug_type_t _debugLevel;	    

  size_t _numBands;					 

  std::string _namePattern;
  std::string _filePath;
  bool _okay;
  NcxxFile _file;
  GlobalAttributes _globalAtts;
  product_type_t _productType;
  scan_type_t _scanType;
  std::string _errStr; 
  size_t _numX;
  size_t _numY;
  std::vector< double > _xCoord;
  std::vector< double > _yCoord;
  double _dxRad;
  double _dyRad;

  time_t _beginTime;
  time_t _midpointTime;
  time_t _endTime;
  
  //  GoesImagerProjection _goesImagerProjection;
  double _nominalSatSubpointLat; 
  double _nominalSatSubpointLon;
  
  double _nominalSatHeight;
  double _westLongitude;
  double _northLatitude;
  double _eastLongitude;
  double _southLatitude;
  double _centerLongitude;
  double _centerLatitude;
  double _nadirLongitude;
  double _nadirLatitude;
  double _xImageCenter;
  double _yImageCenter;
  std::vector< double > _xImageBounds;
  std::vector< double > _yImageBounds;

  std::string _gridMappingName;
  double _perspectivePointHeight;
  double _semiMajorAxis;
  double _semiMinorAxis;
  double _inverseFlattening;
  double _projectionOriginLongitude;
  double _projectionOriginLatitude;
  double _ecc;
  double _radiusRatio2;
  double _invRadiusRatio2;
  double _H;
  int _validPixelCount;
  int _missingPixelCount;
  double _earthSunDistAnomalyAU;
  std::string _inputAbiL0Data;
  std::string _l1bProcessingParamVersion;
  std::string _algorithmVersion;
  std::string _algoProductVersion;

  int _cloudPixels;
  int _outlierPixels;

  double _retrievalLocalZenithAngle;
  double _quantitativeLocalZenithAngle;
  std::vector< double > _retrievalLocalZenithAngleBounds;
  std::vector< double > _quantitativeLocalZenithAngleBounds;

  double _localZenithAngle;
  std::vector< double > _localZenithAngleBounds;
  double _solarZenithAngle;
  std::vector< double > _solarZenithAngleBounds;

  double _dayRetrievalLocalZenithAngle;
  double _nightRetrievalLocalZenithAngle;
  std::vector< double > _dayRetrievalLocalZenithAngleBounds;
  std::vector< double > _nightRetrievalLocalZenithAngleBounds;
  
  double _daySolarZenithAngle;
  double _nightSolarZenithAngle; 
  double _twilightSolarZenithAngle;
  double _dayAlgorithmSolarZenithAngle;
  double _nightAlgorithmSolarZenithAngle;
  std::vector< double > _daySolarZenithAngleBounds;
  std::vector< double > _nightSolarZenithAngleBounds;
  std::vector< double > _twilightSolarZenithAngleBounds;
  std::vector< double > _dayAlgorithmSolarZenithAngleBounds;
  std::vector< double > _nightAlgorithmSolarZenithAngleBounds;

  double _latitude;
  std::vector< double > _latitudeBounds;
  std::string _auxiliaryInfo;


  // make a copy of all variables for the data quality variable attributes
  
  
  virtual void _printVars(const std::string &prefix, const std::string &strarg = "");
  virtual void _printAttrs(const std::string &prefix, const std::string &strarg = "");
  virtual void _printDims(const std::string &prefix, const std::string &strarg = "");
  virtual void _printQC(const std::string &prefix, const std::string &strarg = "") = 0;
  virtual void _printDataCategories(const std::map< int, std::string > &cats, const std::string &prefix,
				    const std::string &strarg = "");
  virtual void _printDataMeanings(const std::map< int, std::string > &meanings, const std::string &prefix,
				    const std::string &strarg = "");
  virtual void _printQualityFlags(const std::map< int, std::string > &flags, const std::string &prefix,
				  const std::string &strarg = "");
  virtual void _printQualityFlagsAndMasks(const std::map< int, std::pair<std::string, int > > &fam,
					  const std::string &prefix, const std::string &strarg = "");
  virtual void _printAuxiliary(const std::string &prefix, const std::string &strarg = "");
  void _printCatDataAttrs(const std::string &prefix, const std::string &label, const CatDataContainer& data,
			  bool print_meaning = false, const std::string &strarg = "");
  void _printDataAttrs(const std::string &prefix, const std::string &label, const DataContainer& data, const std::string &strarg = "");
  void _printQcDataAttrs(const std::string &prefix, const std::string &label, const QcDataContainer& data, bool print_masks = false,
			 const std::string &strarg = "");

  void _readVariables();
  void _readGlobalAttributes();
  void _readTimeVars();
  void _readProjectionVars();
  virtual void _readCoordinateVars();
  virtual void _readConstantsVars() = 0;  
  virtual void _readDimensions() = 0;
  virtual void _readQualityControlVars() = 0;
  virtual void _readDataVars() = 0;
  
  void _setProductType(const std::string &product_type);
  void _setScanType(const std::string &scan_id);
  void _addErrStr(std::string prefix, std::string strarg = "", bool cr = true);
  void _readVar(const char *name, int8_t &var);
  void _readVar(const char *name, uint8_t &var);
  void _readVar(const char *name, short &var);
  void _readVar(const char *name, int &var);
  void _readVar(const char *name, float &var);
  void _readVar(const char *name, double &var);
  void _readVar(const char *name, std::string &var);
  void _readArray(const char *dim_name, const char *var_name,
		      std::vector< double > &vals);
  void _readArray(const char *dim_name, const char *var_name,
		      std::vector< float > &vals);
  void _readArray(const char *dim_name, const char *var_name,
		      std::vector< int > &vals);
  void _readArray(const char *dim_name, const char *var_name,
		      std::vector< short > &vals);
  void _readArray(const char *dim_name, const char *var_name,
		      std::vector< int8_t > &vals);
  void _readArray(const char *dim_name, const char *var_name,
		      std::vector< uint8_t > &vals);
  void _readArray(const char *dim_name, const char *var_name,
		      std::vector< std::string > &vals);

  void _readDataCategories(const NcxxVar &var,
			   std::map< int, std::string > &out);

  void _readFlags(const NcxxVar &var,
		  std::map< int, std::string > &out);

  void _readFlagsAndMasks(const NcxxVar &var,
			 std::map< int, std::pair<std::string, int > > &out);

  virtual void _readDataVar(const std::string& name, DataContainer& data, NcxxVar& var);

  virtual void _readCatDataVar(const std::string& name, CatDataContainer& cat_data, NcxxVar& var);

  virtual void _readQcDataVar(const std::string& name, QcDataContainer& qc_data, NcxxVar& var,
			      bool read_masks = false);

  // start of epoch for GOES-R is 2000-01-01 12:00:00, the J2000 epoch
  static const time_t J2000_EPOCH_START;

  // Level-1 and Level-2 product Dimensions
  static const char* X_DIM;
  static const char* Y_DIM;
  static const char* NUMBER_OF_TIME_BOUNDS;
  static const char* BAND_DIM;
  static const char* NUMBER_OF_IMAGE_BOUNDS;

  // Level-2 specific Dimensions
  static const char* NUMBER_OF_LZA_BOUNDS;
  static const char* NUMBER_OF_SZA_BOUNDS;
  static const char* NUMBER_OF_SUNGLINT_ANGLE_BOUNDS;
  static const char* LAND_SENSOR_BANDS;
  static const char* SEA_SENSOR_BANDS;
  static const char* LATITUDE_BANDS;
  static const char* NUMBER_OF_LATITUDE_BAND_BOUNDS;
  static const char* NUM_AREA_TYPES;
  static const char* MAX_AREA_TYPE_LEN;
  static const char* NUMBER_OF_LAT_BOUNDS;
  static const char* SOUNDING_EMISSIVE_BANDS;
  static const char* RTM_BT_COMPARISON_BANDS;
  
  // Level-1 and Level-2 product Global Attributes
  static const char* NAMING_AUTHORITY;
  static const char* CONVENTIONS;
  static const char* METADATA_CONVENTIONS;
  static const char* STANDARD_NAME_VOCABULARY;
  static const char* INSTITUTION;
  static const char* PROJECT;
  static const char* PRODUCTION_SITE;
  static const char* PRODUCTION_ENVIRONMENT;
  static const char* SPATIAL_RESOLUTION;
  static const char* ORBITAL_SLOT;
  static const char* PLATFORM_ID;
  static const char* INSTRUMENT_TYPE;
  static const char* SCENE_ID;
  static const char* INSTRUMENT_ID;
  static const char* TITLE;
  static const char* SUMMARY;
  static const char* KEYWORDS;
  static const char* KEYWORDS_VOCABULARY;
  static const char* ISO_SERIES_METADATA_ID;
  static const char* LICENSE;
  static const char* PROCESSING_LEVEL;
  static const char* CDM_DATA_TYPE;
  static const char* DATASET_NAME;
  static const char* PRODUCTION_DATA_SOURCE;
  static const char* TIMELINE_ID;
  static const char* DATE_CREATED;
  static const char* TIME_COVERAGE_START;
  static const char* TIME_COVERAGE_END;
  

  // Level-1 and Level-2 Product Variables
  static const char* X_COORD;
  static const char* Y_COORD;
  static const char* TIME;
  static const char* TIME_BOUNDS;
  static const char* GOES_IMAGER_PROJECTION;
  static const char* X_IMAGE;
  static const char* Y_IMAGE;
  static const char* X_IMAGE_BOUNDS;
  static const char* Y_IMAGE_BOUNDS;
  static const char* NOMINAL_SATELLITE_SUBPOINT_LAT;
  static const char* NOMINAL_SATELLITE_SUBPOINT_LON;
  static const char* NOMINAL_SATELLITE_HEIGHT;
  static const char* GEOSPATIAL_LAT_LON_EXTENT;
  static const char* ALGORITHM_DYNAMIC_INPUT_DATA_CONTAINER;
  static const char* PROCESSING_PARM_VERSION_CONTAINER;
  static const char* ALGORITHM_PRODUCT_VERSION_CONTAINER;

  // Level-2 Product Shared Variables
  static const char* RETRIEVAL_LOCAL_ZENITH_ANGLE;
  static const char* QUANTITATIVE_LOCAL_ZENITH_ANGLE;
  static const char* RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS;
  static const char* QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS;
  static const char* RETRIEVAL_SOLAR_ZENITH_ANGLE;
  static const char* QUANTITATIVE_SOLAR_ZENITH_ANGLE;
  static const char* RETRIEVAL_SOLAR_ZENITH_ANGLE_BOUNDS;
  static const char* QUANTITATIVE_SOLAR_ZENITH_ANGLE_BOUNDS;
  static const char* SOLAR_ZENITH_ANGLE;
  static const char* SOLAR_ZENITH_ANGLE_BOUNDS;
  static const char* LOCAL_ZENITH_ANGLE;
  static const char* LOCAL_ZENITH_ANGLE_BOUNDS;
  static const char* TWILIGHT_SOLAR_ZENITH_ANGLE;
  static const char* TWILIGHT_SOLAR_ZENITH_ANGLE_BOUNDS;
  static const char* SUNGLINT_ANGLE;
  static const char* SUNGLINT_ANGLE_BOUNDS;
  static const char* DAY_RETRIEVAL_LOCAL_ZENITH_ANGLE;
  static const char* NIGHT_RETRIEVAL_LOCAL_ZENITH_ANGLE;
  static const char* DAY_RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS;
  static const char* NIGHT_RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS;
  static const char* DAY_SOLAR_ZENITH_ANGLE;
  static const char* NIGHT_SOLAR_ZENITH_ANGLE;
  static const char* DAY_ALGORITHM_SOLAR_ZENITH_ANGLE;
  static const char* NIGHT_ALGORITHM_SOLAR_ZENITH_ANGLE;
  static const char* DAY_SOLAR_ZENITH_ANGLE_BOUNDS;
  static const char* NIGHT_SOLAR_ZENITH_ANGLE_BOUNDS;
  static const char* DAY_ALGORITHM_SOLAR_ZENITH_ANGLE_BOUNDS;
  static const char* NIGHT_ALGORITHM_SOLAR_ZENITH_ANGLE_BOUNDS;
  static const char* DATA_QUALITY_FLAG_OVERALL;
  static const char* DATA_QUALITY_FLAG_RETRIEVAL;
  static const char* DATA_QUALITY_FLAG_SKINTEMP;
  static const char* PERCENT_UNCORRECTABLE_GRB_ERRORS;
  static const char* OUTLIER_PIXELS;
  static const char* CLOUD_PIXELS;
  static const char* OUTLIER_PIXEL_COUNT;
  static const char* DAY_CLOUD_PIXELS;
  static const char* NIGHT_CLOUD_PIXELS;
  static const char* PERCENT_DAY_PIXEL;
  static const char* PERCENT_NIGHT_PIXELS;
  static const char* PERCENT_TERMINATOR_PIXELS;
  static const char* LATITUDE;
  static const char* LATITUDE_BOUNDS;

 // Variable Attributes
  static const char* SCALE_FACTOR;
  static const char* ADD_OFFSET;
  static const char* UNITS;
  static const char* AXIS;
  static const char* LONG_NAME;
  static const char* STANDARD_NAME;
  static const char* FILL_VALUE;
  static const char* UNSIGNED;
  static const char* SENSOR_BAND_BIT_DEPTH;
  static const char* VALID_RANGE;
  static const char* RESOLUTION;
  static const char* COORDINATES;
  static const char* GRID_MAPPING;
  static const char* CELL_METHODS;
  static const char* ANCILLARY_VARIABLES;
  static const char* FLAG_VALUES;
  static const char* FLAG_MEANINGS;
  static const char* FLAG_MASKS;
  static const char* NUMBER_OF_QF_VALUES;
  static const char* BOUNDS;
  static const char* GRID_MAPPING_NAME;
  static const char* PERSPECTIVE_POINT_HEIGHT;
  static const char* SEMI_MAJOR_AXIS;
  static const char* SEMI_MINOR_AXIS;
  static const char* INVERSE_FLATTENING;
  static const char* LATITUDE_OF_PROJECTION_ORIGIN;
  static const char* LONGITUDE_OF_PROJECTION_ORIGIN;
  static const char* SWEEP_ANGLE_AXIS;
  static const char* GEOSPATIAL_WESTBOUND_LONGITUDE;
  static const char* GEOSPATIAL_NORTHBOUND_LATITUDE;
  static const char* GEOSPATIAL_EASTBOUND_LONGITUDE;
  static const char* GEOSPATIAL_SOUTHBOUND_LATITUDE;
  static const char* GEOSPATIAL_LAT_CENTER;
  static const char* GEOSPATIAL_LON_CENTER;
  static const char* GEOSPATIAL_LAT_NADIR;
  static const char* GEOSPATIAL_LON_NADIR;
  static const char* GEOSPATIAL_LAT_UNITS;
  static const char* GEOSPATIAL_LON_UNITS;
  static const char* INPUT_ABI_L0_DATA;
  static const char* L1B_PROCESSING_PARM_VERSION;
  static const char* ALGORITHM_VERSION;
  static const char* PRODUCT_VERSION;
  
  // strings to identify product type
  static const char* TITLE_LEVEL1B_RADIANCES;
  static const char* TITLE_AEROSOL_DETECTION;
  static const char* TITLE_AEROSOL_OPTICAL_DEPTH;
  static const char* TITLE_CLOUD_TOP_PHASE;
  static const char* TITLE_CLOUD_TOP_HEIGHT;
  static const char* TITLE_CLOUD_TOP_TEMPERATURE;
  static const char* TITLE_CLOUD_TOP_PRESSURE;
  static const char* TITLE_DERIVED_STABILITY_INDICES;
  static const char* TITLE_TOTAL_PRECIPITABLE_WATER;
  static const char* TITLE_CLEAR_SKY_MASK;
  static const char* TITLE_FIRE_CHARACTERIZATION;
  static const char* TITLE_LAND_SURFACE_TEMPERATURE;
  static const char* TITLE_CLOUD_OPTICAL_DEPTH;
  static const char* TITLE_CLOUD_PARTICLE_SIZE;
  static const char* TITLE_DERIVED_MOTION_WINDS;
  static const char* TITLE_GLOBAL_LIGHTNING;

  // strings to identify type of scan
  static const char* FULL_DISK_SCAN_NAME;
  static const char* CONUS_SCAN_NAME;
  static const char* MESOSCALE_SCAN_NAME;


private:


};




#endif // GOESRPRODUCT_HH
