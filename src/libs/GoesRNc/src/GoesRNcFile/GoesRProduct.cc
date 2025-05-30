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
 * @file GoesRProduct.cc
 *
 * @class GoesRProduct
 *
 * This is a base class for reading and processing a GOES-R level-2 
 * product contained in an NetCDF file.
 * 
 *  
 * @author G. Cunning
 *
 * @date 10 May 2019
 *
 */

#include <cmath>
#include <string>

#include <rapmath/math_macros.h>
#include <toolsa/DateTime.hh>

#include <GoesRNc/GoesRProduct.hh>

using namespace std;


// start of epoch for GOES-R is 2000-01-01 12:00:00, the J2000 epoch
// value determined by "date --date='2000-01-01 12:00:00 UTC' +%s"
const time_t GoesRProduct::J2000_EPOCH_START = 946728000;

const double GoesRProduct::MISSING_DATA_VALUE = 999999.0;
const double GoesRProduct::BAD_DATA_VALUE = 888888.0;
const int GoesRProduct::DATA_QUALITY_GOOD = 0;
const string GoesRProduct::_className = "GoesRProduct";
const string GoesRProduct::LEVEL_1B_NAME_PATTERN = "OR_ABI-L1b-";
const string GoesRProduct::LEVEL_2_NAME_PATTERN = "OR_ABI-L2-";
const string GoesRProduct::ERROR_STR = "ERROR - ";
const string GoesRProduct::WARN_STR = "WARNING - ";
const string GoesRProduct::INFO_STR = "INFO - ";

/*********************************************************************
 * Constructor
 */

GoesRProduct::GoesRProduct()
{
  _okay = true;
  _isCatData = false;
}

/*********************************************************************
 * Destructor
 */

GoesRProduct::~GoesRProduct()
{
  
}

/*********************************************************************
 * clearAndReset()
 */

void GoesRProduct::clearAndReset()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  _filePath = "";
  _badPixelCount = 0;
  _passBadCountTest = true;
  _dataContainer.values.clear();
  _catDataContainer.values.clear();
  _qcDataContainer.values.clear();
  for(auto it = _outDataFields.begin(); it != _outDataFields.end(); it++) {
    it->second->values.clear();
  }
  for(auto it = _outCatDataFields.begin(); it != _outCatDataFields.end(); it++) {
    it->second->values.clear();
  }
  for(auto it = _outQcDataFields.begin(); it != _outQcDataFields.end(); it++) {
    it->second->values.clear();
  }
  _auxiliaryInfo.clear();
  
}

/*********************************************************************
 * filterPath()
 */
bool GoesRProduct::filterPath(const string &scene, const string &product,
				   const string &path)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  // locate start of filename
  size_t start = path.find(_namePattern);
  if((start == string::npos) && !_namePattern.empty()) {
    return false;
  }
  start += _namePattern.size();

  // find end bound for product/scene substr
  size_t end = path.find("-", start);
  if(end == string::npos) {
    return false;
  }

  string info = path.substr(start, end-start);
  size_t idx = info.size() - 1;

  // check if the scene is meso 1 or 2, which are defined M1 or M2
  size_t offset = 1;
  if(isdigit(info[idx]) != 0) {
    offset = 2;
  }
  
  string fileScene = info.substr(info.size()-offset,offset);
  string fileProduct = info.substr(0,info.size()-offset);
  
  if(!scene.empty() && (fileScene.find(scene) == string::npos)) {
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << ERROR_STR << methodName << endl;
      cerr << "   failed to match scene substring" << scene << endl;
    }
    return false;
  }
  
  if(!product.empty() && (fileProduct.find(product) == string::npos)) {
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << ERROR_STR << methodName << endl;
      cerr << "   failed to match product substring " << product << endl;
    }
    return false;
  }
  
  return true;
}

/*********************************************************************
 * latLon2XY()
 */

bool GoesRProduct::latLon2XY(double lat, double lon,
			     size_t& x_idx,  size_t& y_idx)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  // 0.0174533
  double c_lat = atan(_invRadiusRatio2*tan(lat*DEG_TO_RAD));
  double cos_clat = cos(c_lat);

  double rc = _semiMinorAxis/sqrt(1.0 - pow(_ecc*cos_clat, 2.0));
      
  double del_lon_angle = (lon - _projectionOriginLongitude)*DEG_TO_RAD;

  double sx =  _H - (rc*cos_clat*cos(del_lon_angle));
  double sy = -rc*cos_clat*sin(del_lon_angle);
  double sz = rc*sin(c_lat);
      
  // // check that point is on disk of the earth
  if((_H*(_H - sx)) < (sy*sy + _radiusRatio2*sz*sz)) {
    x_idx = 0;
    y_idx = 0;
    return false;
  }

  double rl = sqrt((sx*sx + sy*sy + sz*sz));
  double xx = asin((-sy/rl));
  double yy = atan((sz/sx));
  
  x_idx = round((xx - _xImageBounds[0])/_dxRad);
  y_idx = round((_yImageBounds[0] - yy)/_dyRad);

  if( (y_idx < 0) || (y_idx >= _numY) ||
      (x_idx < 0) || (x_idx > _numX)) {
     x_idx = 0;
     y_idx = 0;
     return false;
   }
  
  return true;
}


/*********************************************************************
 * printInfo()
 */

void GoesRProduct::printInfo()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  cout << INFO_STR << " File Contents Information: " << endl;
  cout << INFO_STR << "   path: " << _filePath << endl;
  _printDims(INFO_STR);
  _printVars(INFO_STR);
  _printAttrs(INFO_STR);
  _printQC(INFO_STR);
  _printAuxiliary(INFO_STR);
}

/*********************************************************************
 * readFile()
 */

bool GoesRProduct::readFile()
{
  static const string methodName = _className + "::" + __FUNCTION__;

  // open file and read contents
  if (_filePath.empty() == true) {
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << ERROR_STR << methodName << endl;
      cerr << "   empty file path." << endl;
    }
    return false;
  }

  try {
    
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      string msg = INFO_STR + methodName + "--   Reading from " + _filePath;
      cerr << msg << endl;
    }
    _file.open(_filePath, NcxxFile::read);

    _readGlobalAttributes();
    _readDimensions();
    _readVariables();
    _file.close();

  } catch (NcxxException& e) {
    cerr << ERROR_STR << methodName << endl;
    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << "  " << e.what() << endl;
    }
    else {
      cerr << "Failed to read file" << endl;
    }
    return false;
  }
   

  return true;
}



/*********************************************************************
 * _printVars()
 */

void GoesRProduct::_printVars(const std::string &prefix,
			      const std::string &strarg /* = "" */)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  cout << prefix << "Variable info:" << endl;
  cout << prefix << "Time:" << endl;
  cout << prefix << "   begin:  " << DateTime::str(_beginTime) << endl;
  cout << prefix << "   mid point:  " << DateTime::str(_midpointTime) << endl;
  cout << prefix << "   end:  " << DateTime::str(_endTime) << endl;
  cout << prefix << "Projection:" << endl;
  cout << prefix << "   " << GRID_MAPPING_NAME << ":  " << _gridMappingName << endl;
  cout << prefix << "   " << PERSPECTIVE_POINT_HEIGHT << ":  "
       << _perspectivePointHeight << endl;
  cout << prefix << "   " << SEMI_MAJOR_AXIS << ":  " << _semiMajorAxis << endl;
  cout << prefix << "   " << SEMI_MINOR_AXIS << ":  " << _semiMinorAxis << endl;
  cout << prefix << "   " << INVERSE_FLATTENING << ":  "
       << _inverseFlattening << endl;
  cout << prefix << "   " << LATITUDE_OF_PROJECTION_ORIGIN << ":  "
       << _projectionOriginLatitude << endl;
  cout << prefix << "   " << LONGITUDE_OF_PROJECTION_ORIGIN << ":  "
       << _projectionOriginLongitude << endl;
  cout << prefix << "   " << NOMINAL_SATELLITE_SUBPOINT_LAT << ":  "
       << _nominalSatSubpointLat << endl;
  cout << prefix << "   " << NOMINAL_SATELLITE_SUBPOINT_LON << ":  "
       << _nominalSatSubpointLon << endl;
  cout << prefix << "   " << NOMINAL_SATELLITE_HEIGHT << ":  "
       << _nominalSatHeight << endl;
  cout << prefix << "   " << X_IMAGE << ":  " << _xImageCenter << endl;
  cout << prefix << "   " << Y_IMAGE << ":  " << _yImageCenter << endl;
  cout << prefix << "   " << GEOSPATIAL_WESTBOUND_LONGITUDE << ":  "
       << _westLongitude << endl;
  cout << prefix << "   " << GEOSPATIAL_NORTHBOUND_LATITUDE << ":  "
       << _northLatitude << endl;
  cout << prefix << "   " << GEOSPATIAL_EASTBOUND_LONGITUDE << ":  "
       << _eastLongitude << endl;
  cout << prefix << "   " << GEOSPATIAL_SOUTHBOUND_LATITUDE << ":  "
       << _southLatitude << endl;
  cout << prefix << "   " << GEOSPATIAL_LAT_CENTER << ":  " << _centerLatitude << endl;
  cout << prefix << "   " << GEOSPATIAL_LON_CENTER << ":  " << _centerLongitude << endl;
  cout << prefix << "   " << GEOSPATIAL_LAT_NADIR << ":  " << _nadirLatitude << endl;
  cout << prefix << "   " << GEOSPATIAL_LON_NADIR << ":  " << _nadirLongitude << endl;
  cout << prefix << "   " << X_IMAGE_BOUNDS << " west:  " << _xImageBounds[0] << endl;
  cout << prefix << "   " << X_IMAGE_BOUNDS << " west:  " << _xImageBounds[1] << endl;
  cout << prefix << "   " << Y_IMAGE_BOUNDS << " north:  " << _yImageBounds[0] << endl;
  cout << prefix << "   " << Y_IMAGE_BOUNDS << " south:  " << _yImageBounds[1] << endl;


}

/*********************************************************************
 * _printAttrs()
 */

void GoesRProduct::_printAuxiliary(const std::string &prefix,
				   const std::string &strarg /* = "" */)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  cout << prefix << "Auxiliary Info:" << endl;
  cout << prefix << "   " << _auxiliaryInfo << endl;

}
/*********************************************************************
 * _printAttrs()
 */

void GoesRProduct::_printAttrs(const std::string &prefix,
				 const std::string &strarg /* = "" */)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  cout << prefix << "Attributes: " << endl;
  cout << prefix << "Global attributes: " << endl;
  cout << prefix << "   " << NAMING_AUTHORITY << ":  "
       << _globalAtts.namingAuthority << endl;
  cout << prefix << "   " << CONVENTIONS << ":  "
       << _globalAtts.conventions << endl;
  cout << prefix << "   " << METADATA_CONVENTIONS << ":  "
       << _globalAtts.metadataConventions << endl;
  cout << prefix << "   " << STANDARD_NAME_VOCABULARY << ":  "
       << _globalAtts.standardNameVocabulary << endl;
  cout << prefix << "   " << INSTITUTION << ":  "
       << _globalAtts.institution << endl;
  cout << prefix << "   " << PROJECT << ":  "
       << _globalAtts.project << endl;
  cout << prefix << "   " << PRODUCTION_SITE << ":  "
       << _globalAtts.productionSite << endl;
  cout << prefix << "   " << PRODUCTION_ENVIRONMENT << ":  "
       << _globalAtts.productionEnvironment << endl;
  cout << prefix << "   " << SPATIAL_RESOLUTION << ":  "
       << _globalAtts.spatialResolution << endl;
  cout << prefix << "   " << ORBITAL_SLOT << ":  "
       << _globalAtts.orbitalSlot << endl;
  cout << prefix << "   " << PLATFORM_ID << ":  "
       << _globalAtts.platformID << endl;
  cout << prefix << "   " << INSTRUMENT_TYPE << ":  "
       << _globalAtts.instrumentType << endl;
  cout << prefix << "   " << INSTRUMENT_ID << ":  "
       << _globalAtts.instrumentID << endl;
  cout << prefix << "   " << SUMMARY << ":  "
       << _globalAtts.summary << endl;
  cout << prefix << "   " << KEYWORDS << ":  "
       << _globalAtts.keywords << endl;
  cout << prefix << "   " << KEYWORDS_VOCABULARY << ":  "
       << _globalAtts.keywordsVocabulary << endl;
  cout << prefix << "   " << ISO_SERIES_METADATA_ID << ":  "
       << _globalAtts.isoSeriesMetadataID << endl;
  cout << prefix << "   " << LICENSE << ":  "
       << _globalAtts.license << endl;
  cout << prefix << "   " << PROCESSING_LEVEL << ":  "
       << _globalAtts.processingLevel << endl;
  cout << prefix << "   " << CDM_DATA_TYPE << ":  "
       << _globalAtts.cdmDataType << endl;
  cout << prefix << "   " << DATASET_NAME << ":  "
       << _globalAtts.datasetName << endl;
  cout << prefix << "   " << PRODUCTION_DATA_SOURCE << ":  "
       << _globalAtts.productionDataSource << endl;
  cout << prefix << "   " << TIMELINE_ID << ":  "
       << _globalAtts.timelineID << endl;
  cout << prefix << "   " << DATE_CREATED << ":  "
       << _globalAtts.dateCreated << endl;
  cout << prefix << "   " << TIME_COVERAGE_START << ":  "
       << _globalAtts.timeCoverageStart << endl;
  cout << prefix << "   " << TIME_COVERAGE_END << ":  "
       << _globalAtts.timeCoverageEnd << endl;
  cout << prefix << "   " << SCENE_ID << ":  "
       << _globalAtts.sceneID << endl;
  cout << prefix << "   " << TITLE << ":  "
       << _globalAtts.title << endl;
}

/*********************************************************************
 * _printDims()
 */

void GoesRProduct::_printDims(const string &prefix,
				 const string &strarg /* = "" */)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  cout << prefix << "Grid dimensions info:" << endl;
  cout << prefix << "   " << X_DIM <<  " numpts:  " << _numX  << endl;
  cout << prefix << "   " << X_DIM <<  " dx:  " << _dxRad  << endl;
  cout << prefix << "   " << Y_DIM <<  " numpts:  " << _numY  << endl;
  cout << prefix << "   " << Y_DIM <<  " dy:  " << _dyRad  << endl;
}


/*********************************************************************
 * _printDataCategories()
 */

void GoesRProduct::_printDataCategories(const map< int, string > &cats,
					const string &prefix,
					const string &strarg /* = "" */)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  cout << prefix << "Data categories info:" << endl;
  cout << prefix << "    value      category" << endl;
  for(auto it = cats.begin(); it != cats.end(); it++) {
    cout << prefix << "      " << it->first << "           " << it->second << endl;
  }

}


/*********************************************************************
 * _printDataMeanings()
 */

void GoesRProduct::_printDataMeanings(const map< int, string > &meanings,
				      const string &prefix,
				      const string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  cout << prefix << "Data meainings info:" << endl;
  cout << prefix << "    value      meaning" << endl;
  for(auto it = meanings.begin(); it != meanings.end(); it++) {
    cout << prefix << "      " << it->first << "           " << it->second << endl;
  }

}


/*********************************************************************
 * _printQualityFlags()
 */

void GoesRProduct::_printQualityFlags(const map< int, string > &flags,
				      const string &prefix,
				      const string &strarg /* = "" */)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  cout << prefix << "Data quality flag info:" << endl;
  cout << prefix << "    value      flag meaning" << endl;
  for(auto it = flags.begin(); it != flags.end(); it++) {
    cout << prefix << "      " << it->first << "            " << it->second << endl;
  }
}

/*********************************************************************
 * _printQualityFlagsAndMAsks()
 */

void GoesRProduct::_printQualityFlagsAndMasks(const map< int, pair<string, int > > &fam,
					      const string &prefix,
					      const string &strarg /* = "" */)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  cout << prefix << "Data quality flag info:" << endl;
  cout << prefix << "    value      mask               meaning" << endl;
  for(auto it = fam.begin(); it != fam.end(); it++) {
    cout << prefix << "      " << it->first << "         " << it->second.second
	 << "       " << it->second.first << endl;
  }


}


void GoesRProduct::_printCatDataAttrs(const string & prefix, const string & label, const CatDataContainer& data,
				      bool print_meaning /* = false */,
				      const std::string &strarg /*= "" */)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  cout << prefix << label << " Attributes: " << endl;
  cout << prefix << "   " << LONG_NAME << ":  " << data.longName << endl;
  cout << prefix << "   " << STANDARD_NAME << ":  " << data.standardName << endl;
  cout << prefix << "   " << FILL_VALUE << ":  " << data.ncMissingVal << endl;
  cout << prefix << "   " << UNITS << ":  " << data.units << endl;
  cout << prefix << "   " << VALID_RANGE << ":  " << data.validRange[0] << ",   "
       << data.validRange[1] << endl;
  cout << prefix << "   " << RESOLUTION << ":  " << data.resolution << endl;
  cout << prefix << "   " << COORDINATES << ":  " << data.coordinates << endl;
  cout << prefix << "   " << GRID_MAPPING << ":  " << data.gridMapping << endl;
  cout << prefix << "   " << CELL_METHODS << ":  " << data.cellMethods << endl;

  _printDataCategories(data.categories, prefix, strarg);
  if (print_meaning == true) {
    _printDataMeanings(data.meanings, prefix, strarg);
  }
}


void GoesRProduct::_printDataAttrs(const string & prefix, const string & label, const DataContainer& data,
				   const std::string &strarg /*= "" */)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  cout << prefix << label << " Attributes: " << endl;
  cout << prefix << "   " << LONG_NAME << ":  " << data.longName << endl;
  cout << prefix << "   " << STANDARD_NAME << ":  " << data.standardName << endl;
  cout << prefix << "   " << FILL_VALUE << ":  " << data.ncMissingVal << endl;
  cout << prefix << "   " << SCALE_FACTOR << ":  " << data.scaleFactor << endl;
  cout << prefix << "   " << ADD_OFFSET << ":  " << data.offset << endl;
  cout << prefix << "   " << UNITS << ":  " << data.units << endl;
  cout << prefix << "   " << VALID_RANGE << ":  " << data.validRange[0] << ",   " <<
    data.validRange[1] << endl;
  cout << prefix << "   " << RESOLUTION << ":  " << data.resolution << endl;
  cout << prefix << "   " << COORDINATES << ":  " << data.coordinates << endl;
  cout << prefix << "   " << GRID_MAPPING << ":  " << data.gridMapping << endl;
  cout << prefix << "   " << CELL_METHODS << ":  " << data.cellMethods << endl;
}

void GoesRProduct::_printQcDataAttrs(const string & prefix, const string & label, const QcDataContainer& data,
				     bool print_masks, const std::string &strarg)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  cout << prefix << label << " Attributes: " << endl;
  cout << prefix << "   " << LONG_NAME << ":  " << data.longName << endl;
  cout << prefix << "   " << STANDARD_NAME << ":  " << data.standardName << endl;
  cout << prefix << "   " << FILL_VALUE << ":  " << data.ncMissingVal << endl;
  cout << prefix << "   " << UNITS << ":  " << data.units << endl;
  cout << prefix << "   " << VALID_RANGE << ":  " << data.validRange[0] << ",   " <<
    data.validRange[1] << endl;
  cout << prefix << "   " << COORDINATES << ":  " << data.coordinates << endl;
  cout << prefix << "   " << GRID_MAPPING << ":  " << data.gridMapping << endl;
  cout << prefix << "   " << CELL_METHODS << ":  " << data.cellMethods << endl;

  if (print_masks == false) {
    _printQualityFlags(data.flags, prefix, strarg);
  }
  else {
    _printQualityFlagsAndMasks(data.flagsAndMasks, prefix, strarg);
  }
}

/*********************************************************************
 * _readGlobalAttributes()
 *
 * For the most part the global attributes only have informational 
 * value. There are a few that must be in the file and are used in 
 * subsequesnt processing. They are:
 *  SCENE_ID, TITLE, ...
 *
 * For these attributes the method will return false.
 *
 */

void GoesRProduct::_readGlobalAttributes()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  try {
    _file.readGlobAttr(NAMING_AUTHORITY, _globalAtts.namingAuthority);
    _file.readGlobAttr(CONVENTIONS, _globalAtts.conventions);
    _file.readGlobAttr(METADATA_CONVENTIONS, _globalAtts.metadataConventions);
    _file.readGlobAttr(STANDARD_NAME_VOCABULARY, _globalAtts.standardNameVocabulary);
    _file.readGlobAttr(INSTITUTION, _globalAtts.institution);
    _file.readGlobAttr(PROJECT, _globalAtts.project);
    _file.readGlobAttr(PRODUCTION_SITE, _globalAtts.productionSite);
    _file.readGlobAttr(PRODUCTION_ENVIRONMENT, _globalAtts.productionEnvironment);
    _file.readGlobAttr(SPATIAL_RESOLUTION, _globalAtts.spatialResolution);
    _file.readGlobAttr(ORBITAL_SLOT, _globalAtts.orbitalSlot);
    _file.readGlobAttr(PLATFORM_ID, _globalAtts.platformID);
    _file.readGlobAttr(INSTRUMENT_TYPE, _globalAtts.instrumentType);
    _file.readGlobAttr(INSTRUMENT_ID, _globalAtts.instrumentID);
    _file.readGlobAttr(SUMMARY, _globalAtts.summary);
    _file.readGlobAttr(KEYWORDS, _globalAtts.keywords);
    _file.readGlobAttr(KEYWORDS_VOCABULARY, _globalAtts.keywordsVocabulary);
    _file.readGlobAttr(ISO_SERIES_METADATA_ID, _globalAtts.isoSeriesMetadataID);
    _file.readGlobAttr(LICENSE, _globalAtts.license);
    _file.readGlobAttr(PROCESSING_LEVEL, _globalAtts.processingLevel);
    _file.readGlobAttr(CDM_DATA_TYPE, _globalAtts.cdmDataType);
    _file.readGlobAttr(DATASET_NAME, _globalAtts.datasetName);
    _file.readGlobAttr(PRODUCTION_DATA_SOURCE, _globalAtts.productionDataSource);
    _file.readGlobAttr(TIMELINE_ID, _globalAtts.timelineID);
    _file.readGlobAttr(DATE_CREATED, _globalAtts.dateCreated);
    _file.readGlobAttr(TIME_COVERAGE_START, _globalAtts.timeCoverageStart);
    _file.readGlobAttr(TIME_COVERAGE_END, _globalAtts.timeCoverageEnd);
    _file.readGlobAttr(TITLE, _globalAtts.title);
  } catch (NcxxException& e) {
    string errStr = e.what();
    size_t ePos = errStr.find("ERROR");
    while (ePos != string::npos){
      errStr.replace(ePos,std::string("ERROR").length(), "WARNING");
      ePos = errStr.find("ERROR");
    }

    if(_debugLevel == GoesRProduct::DEBUG_VERBOSE) {
      cerr << errStr << endl;
      cerr << WARN_STR  << methodName << endl;
      cerr << "  didn't find global attribute, " << errStr << endl;
    }
  }

  try {
    _file.readGlobAttr(SCENE_ID, _globalAtts.sceneID);
    _setScanType(_globalAtts.sceneID); 
  } catch (NcxxException e) {
    NcxxErrStr err;
    ostringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  didn't find " << SCENE_ID << " in global attributes" << endl;
    err.addErrStr(info.str());
    err.addErrStr("  exception: ", e.what());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

}


/*********************************************************************
 * _readVariables()
 */

void GoesRProduct::_readVariables()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  try {
    _readCoordinateVars();
    _readTimeVars();
    _readProjectionVars();
    _readConstantsVars();  
    _readQualityControlVars();
    _readDataVars();
  }
  catch (NcxxException e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  Cannot read all the variables." << endl;
    err.addErrStr(info.str());
    err.addErrStr("  exception: ", e.what());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

}

/*********************************************************************
 * _readCoordinateVars()
 */

void GoesRProduct::_readCoordinateVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  NcxxDim xDim;
  NcxxDim yDim;
  NcxxVar xVar;
  NcxxVar yVar;
  string xDimStr = X_DIM;
  string yDimStr = Y_DIM;
  _file.getCoordVar(xDimStr, xDim, xVar);
  _file.getCoordVar(yDimStr, yDim, yVar);

  if((xDim.isNull() == true) || (xVar.isNull() == true)) {
    NcxxErrStr err;
    string info = ERROR_STR + methodName;
    err.addErrStr(info);
    cerr << "  X coordinate is missing. dimension: " << X_DIM << " variable: "
	 << X_COORD << endl;
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  _numX = xDim.getSize();
  double scaleFactor;
  double offset;
  try{
    xVar.getAtt(SCALE_FACTOR).getValues(&scaleFactor);
    xVar.getAtt(ADD_OFFSET).getValues(&offset);
  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    err.addErrStr(info.str());
    err.addErrStr("  exception: ", e.what());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  // use absolute value of scale factor to get dx
  _dxRad = fabs(scaleFactor);
  
  short *xVals = new short[_numX];
  xVar.getVal(xVals);
  for (size_t i = 0; i < _numX; i++) {
    _xCoord.push_back((static_cast<double>(xVals[i])*scaleFactor + offset));
  }
  delete [] xVals;

  if((yDim.isNull() == true) || (yVar.isNull() == true)) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  Y coordinate is missing. dimension: " << Y_DIM << " variable: "
	 << Y_COORD << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  _numY = yDim.getSize();
  try{
    yVar.getAtt(SCALE_FACTOR).getValues(&scaleFactor);
    yVar.getAtt(ADD_OFFSET).getValues(&offset);
  }
  catch (NcxxException e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    err.addErrStr("  exception: ", e.what());
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  // use absolute value of scale factor to get dy
  _dyRad = fabs(scaleFactor);
  
  short *yVals = new short[_numY];
  yVar.getVal(yVals);
  for (size_t i = 0; i < _numY; i++) {
    _yCoord.push_back((static_cast<double>(yVals[i])*scaleFactor + offset));
  }
  delete [] yVals;
  	    
}


/*********************************************************************
 * _readTimeVars()
 */

void GoesRProduct::_readTimeVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  try {
    double val;
    _readVar(TIME, val);
    _midpointTime = J2000_EPOCH_START + static_cast< time_t >(round(val));
    vector< double > vals;
    _readArray(NUMBER_OF_TIME_BOUNDS, TIME_BOUNDS, vals);
    _endTime = J2000_EPOCH_START + static_cast< time_t >(round(vals[1]));
    _beginTime = J2000_EPOCH_START + static_cast< time_t >(round(vals[0]));
  }
  catch (NcxxException e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    err.addErrStr("  exception: ", e.what());
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

}
  
/*********************************************************************
 * _readProjectionVars()
 */

void GoesRProduct::_readProjectionVars()
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  NcxxVar varG = _file.getVar(GOES_IMAGER_PROJECTION);
  if(varG.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " <<  GOES_IMAGER_PROJECTION << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  NcxxVar varLL = _file.getVar(GEOSPATIAL_LAT_LON_EXTENT);
  if(varLL.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << GEOSPATIAL_LAT_LON_EXTENT  << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }


  try{
    varG.getAtt(GRID_MAPPING_NAME).getValues(_gridMappingName);
    varG.getAtt(PERSPECTIVE_POINT_HEIGHT).getValues(&_perspectivePointHeight);
    varG.getAtt(SEMI_MAJOR_AXIS).getValues(&_semiMajorAxis);
    varG.getAtt(SEMI_MINOR_AXIS).getValues(&_semiMinorAxis);
    varG.getAtt(INVERSE_FLATTENING).getValues(&_inverseFlattening);
    varG.getAtt(LATITUDE_OF_PROJECTION_ORIGIN).getValues(&_projectionOriginLatitude);
    varG.getAtt(LONGITUDE_OF_PROJECTION_ORIGIN).getValues(&_projectionOriginLongitude);

    _readVar(NOMINAL_SATELLITE_SUBPOINT_LAT, _nominalSatSubpointLat);
    _readVar(NOMINAL_SATELLITE_SUBPOINT_LON, _nominalSatSubpointLon);
    _readVar(NOMINAL_SATELLITE_HEIGHT, _nominalSatHeight);
    _readVar(X_IMAGE, _xImageCenter);
    _readVar(Y_IMAGE, _yImageCenter);   

    varLL.getAtt(GEOSPATIAL_WESTBOUND_LONGITUDE).getValues(&_westLongitude);
    varLL.getAtt(GEOSPATIAL_NORTHBOUND_LATITUDE).getValues(&_northLatitude);
    varLL.getAtt(GEOSPATIAL_EASTBOUND_LONGITUDE).getValues(&_eastLongitude);
    varLL.getAtt(GEOSPATIAL_SOUTHBOUND_LATITUDE).getValues(&_southLatitude);
    varLL.getAtt(GEOSPATIAL_LAT_CENTER).getValues(&_centerLatitude);
    varLL.getAtt(GEOSPATIAL_LON_CENTER).getValues(&_centerLongitude);
    varLL.getAtt(GEOSPATIAL_LAT_NADIR).getValues(&_nadirLatitude);
    varLL.getAtt(GEOSPATIAL_LON_NADIR).getValues(&_nadirLongitude);

    _readArray(NUMBER_OF_IMAGE_BOUNDS, X_IMAGE_BOUNDS, _xImageBounds);
    _readArray(NUMBER_OF_IMAGE_BOUNDS, Y_IMAGE_BOUNDS, _yImageBounds);
  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }


  // initialize coeffiecients for the projective geometry 
  double flatten = 1.0/_inverseFlattening;
  _ecc = sqrt(2.0*flatten - flatten*flatten);
  _radiusRatio2 = pow((_semiMajorAxis/_semiMinorAxis), 2.0);
  _invRadiusRatio2 = 1.0/_radiusRatio2;
  _H = _perspectivePointHeight +  _semiMajorAxis;

}
  

void GoesRProduct::_readVar(const char *name, int8_t &var)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar ncvar = _file.getVar(name);
  if(ncvar.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  ncvar.getVal(&var);
}
		       
void GoesRProduct::_readVar(const char *name, uint8_t &var)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar ncvar = _file.getVar(name);
  if(ncvar.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  ncvar.getVal(&var);
}
		       
void GoesRProduct::_readVar(const char *name, short &var)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar ncvar = _file.getVar(name);
  if(ncvar.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  ncvar.getVal(&var);
}

void GoesRProduct::_readVar(const char *name, int &var)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  NcxxVar ncvar = _file.getVar(name);
  if(ncvar.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  ncvar.getVal(&var);
}
		       
void GoesRProduct::_readVar(const char *name, float &var)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar ncvar = _file.getVar(name);
  if(ncvar.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  ncvar.getVal(&var);  
}
		       
void GoesRProduct::_readVar(const char *name, double &var)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar ncvar = _file.getVar(name);
  if(ncvar.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  ncvar.getVal(&var);
}
		       
void GoesRProduct::_readVar(const char *name, string &var)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar ncvar = _file.getVar(name);
  if(ncvar.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  ncvar.getVal(&var);
}
		       

void GoesRProduct::_readArray(const char *dim_name, const char *var_name,
				  vector< double > &vals)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar var = _file.getVar(var_name);
  if(var.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << var_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  NcxxDim dim = _file.getDim(dim_name); 
  if(dim.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  dimension is missing, name: " << dim_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  vals.clear();
  double *newVals = new double[dim.getSize()];
  var.getVal(newVals);

  for(size_t i = 0; i < dim.getSize(); i ++) {
    vals.push_back(newVals[i]);
  }
  delete [] newVals;
}

void GoesRProduct::_readArray(const char *dim_name, const char *var_name,
				  vector< float > &vals)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  NcxxVar var = _file.getVar(var_name);
  if(var.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << var_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  NcxxDim dim = _file.getDim(dim_name); 
  if(dim.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  dimension is missing, name: " << dim_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  vals.clear();
  float *newVals = new float[dim.getSize()];
  var.getVal(newVals);

  for(size_t i = 0; i < dim.getSize(); i ++) {
    vals.push_back(newVals[i]);
  }
  delete [] newVals;
}

void GoesRProduct::_readArray(const char *dim_name, const char *var_name,
				  vector< int > &vals)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar var = _file.getVar(var_name);
  if(var.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << var_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  NcxxDim dim = _file.getDim(dim_name); 
  if(dim.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  dimension is missing, name: " << dim_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  vals.clear();
  int *newVals = new int[dim.getSize()];
  var.getVal(newVals);

  for(size_t i = 0; i < dim.getSize(); i ++) {
    vals.push_back(newVals[i]);
  }
  delete [] newVals;
}

void GoesRProduct::_readArray(const char *dim_name, const char *var_name,
				  vector< short > &vals)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar var = _file.getVar(var_name);
  if(var.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << var_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  NcxxDim dim = _file.getDim(dim_name); 
  if(dim.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  dimension is missing, name: " << dim_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  vals.clear();
  short *newVals = new short[dim.getSize()];
  var.getVal(newVals);

  for(size_t i = 0; i < dim.getSize(); i ++) {
    vals.push_back(newVals[i]);
  }
  delete [] newVals;
}

void GoesRProduct::_readArray(const char *dim_name, const char *var_name,
				  vector< int8_t > &vals)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar var = _file.getVar(var_name);
  if(var.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << var_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  NcxxDim dim = _file.getDim(dim_name); 
  if(dim.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  dimension is missing, name: " << dim_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  vals.clear();
  int8_t *newVals = new int8_t[dim.getSize()];
  var.getVal(newVals);

  for(size_t i = 0; i < dim.getSize(); i ++) {
    vals.push_back(newVals[i]);
  }
  delete [] newVals;
}

void GoesRProduct::_readArray(const char *dim_name, const char *var_name,
				  vector< uint8_t > &vals)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar var = _file.getVar(var_name);
  if(var.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << var_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  NcxxDim dim = _file.getDim(dim_name); 
  if(dim.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  dimension is missing, name: " << dim_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  vals.clear();
  uint8_t *newVals = new uint8_t[dim.getSize()];
  var.getVal(newVals);

  for(size_t i = 0; i < dim.getSize(); i ++) {
    vals.push_back(newVals[i]);
  }
  delete [] newVals;
}

void GoesRProduct::_readArray(const char *dim_name, const char *var_name,
				  vector< string > &vals)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  NcxxVar var = _file.getVar(var_name);
  if(var.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << var_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  NcxxDim dim = _file.getDim(dim_name); 
  if(dim.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  dimension is missing, name: " << dim_name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  vals.clear();
  var.getVal(vals.data());
}


void GoesRProduct::_readDataCategories(const NcxxVar &var,
				       map< int, string > &out)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  try {
    
    string labelStr;
    vector< string > labels;
    string flag;
    size_t end = 0;
    size_t start = 0;
    char sep = ' ';

    var.getAtt(FLAG_MEANINGS).getValues(labelStr);      
    
    // split the dqLabels based on white space
    while((end = labelStr.find(sep, start)) != string::npos) {
      labels.push_back(labelStr.substr(start, end - start));
      start = end + 1;
    }
    // add last in string
    labels.push_back(labelStr.substr(start, labelStr.size() - start));

    vector<int> vals;
    var.getAtt(FLAG_VALUES).getValues(vals);

    for(size_t i = 0; i < labels.size(); i++) {
      out[static_cast<int>(vals[i])] = labels[i];
    }
    
  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    cerr << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
}


void GoesRProduct::_readFlags(const NcxxVar &var,
			      map< int, string > &out)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    
    string labelStr;
    vector< int > vals;
    vector< string > labels;
    string flag;
    size_t end = 0;
    size_t start = 0;
    char sep = ' ';

    var.getAtt(FLAG_MEANINGS).getValues(labelStr);
    var.getAtt(FLAG_VALUES).getValues(vals);

    // split the dqLabels based on white space
    while((end = labelStr.find(sep, start)) != string::npos) {
      labels.push_back(labelStr.substr(start, end - start));
      start = end + 1;
    }
    // add last in string
    labels.push_back(labelStr.substr(start, labelStr.size() - start));

    for(size_t i = 0; i < labels.size(); i++) {
      out[static_cast<int>(vals[i])] = labels[i];
    }
  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    cerr << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
}

void GoesRProduct::_readFlagsAndMasks(const NcxxVar &var,
				      map< int, pair<string, int > > &out)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  try {
    
    string labelStr;
    vector< int > vals;
    vector< int > masks;
    vector< string > labels;
    string flag;
    size_t end = 0;
    size_t start = 0;
    char sep = ' ';

    var.getAtt(FLAG_MEANINGS).getValues(labelStr);

    var.getAtt(FLAG_VALUES).getValues(vals);
    var.getAtt(FLAG_MASKS).getValues(masks);

    // split the dqLabels based on white space
    while((end = labelStr.find(sep, start)) != string::npos) {
      labels.push_back(labelStr.substr(start, end - start));
      start = end + 1;
    }
    // add last in string
    labels.push_back(labelStr.substr(start, labelStr.size() - start));
    
    for(size_t i = 0; i < labels.size(); i++) {
      out[static_cast<int>(vals[i])] = make_pair( labels[i], static_cast<int>(masks[i]));
    }
  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    cerr << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
}
  
/*********************************************************************
 * _readDataVar()
 */

void GoesRProduct::_readDataVar(const std::string& name, DataContainer& data, NcxxVar& var)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  var = _file.getVar(name);
  if(var.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

  try {
    var.getAtt(SCALE_FACTOR).getValues(&data.scaleFactor);
    var.getAtt(ADD_OFFSET).getValues(&data.offset);
    var.getAtt(LONG_NAME).getValues(data.longName);
    var.getAtt(STANDARD_NAME).getValues(data.standardName);
    var.getAtt(FILL_VALUE).getValues(&data.ncMissingVal);
    var.getAtt(UNITS).getValues(data.units);
    var.getAtt(VALID_RANGE).getValues(data.validRange);
    var.getAtt(RESOLUTION).getValues(data.resolution);
    var.getAtt(COORDINATES).getValues(data.coordinates);
    var.getAtt(GRID_MAPPING).getValues(data.gridMapping);
    var.getAtt(CELL_METHODS).getValues(data.cellMethods);

    size_t nPts = _numX*_numY;

    unsigned short usMiss = static_cast<unsigned short>(data.ncMissingVal);
    unsigned short usMin = static_cast<unsigned short>(data.validRange[0]);
    unsigned short usMax = static_cast<unsigned short>(data.validRange[1]);
    float flMiss = static_cast<float>(usMiss)*data.scaleFactor + data.offset;
    float flMin = static_cast<float>(usMin)*data.scaleFactor + data.offset;
    float flMax = static_cast<float>(usMax)*data.scaleFactor + data.offset;

    try {
      var.getAtt(UNSIGNED).getValues(data.unsignedVal);

      vector< short > svals;
      svals.assign(nPts, 0);
      var.getVal(svals.data());

      data.values.assign( nPts, MISSING_DATA_VALUE);

      for (size_t i = 0; i < nPts; i++) {
        unsigned short usVal = static_cast<unsigned short>(svals[i]);
        data.values[i] = static_cast<float>( usVal )*data.scaleFactor + data.offset;
        if (data.values[i] == flMiss) {
          data.values[i] = MISSING_DATA_VALUE;
        }
        else if (data.values[i] > flMax) {
          data.values[i] = flMax;
        }
        else if (data.values[i] < flMin) {
          data.values[i] = flMin;
        }

      }

    }
    catch (NcxxException& e) {
      vector< unsigned short  > svals;
      svals.assign(nPts, 0);
      var.getVal(svals.data());

      data.values.assign( nPts, MISSING_DATA_VALUE);
      
      for (size_t i = 0; i < nPts; i++) {
        unsigned short usVal = static_cast<unsigned short>(svals[i]);
        data.values[i] = static_cast<float>( usVal )*data.scaleFactor + data.offset;
        if (data.values[i] == flMiss) {
          data.values[i] = MISSING_DATA_VALUE;
        }
        else if (data.values[i] > flMax) {
          data.values[i] = flMax;
        }
        else if (data.values[i] < flMin) {
          data.values[i] = flMin;
        }

      }

    }

  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }


}

/*********************************************************************
 * _readDataVar()
 */

void  GoesRProduct::_readCatDataVar(const std::string& name, CatDataContainer& data, NcxxVar& var)
{
  static const string methodName = _className + "::" + __FUNCTION__;

  var = _file.getVar(name);
  if(var.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }


  try {
    var.getAtt(LONG_NAME).getValues(data.longName);
    var.getAtt(STANDARD_NAME).getValues(data.standardName);
    var.getAtt(FILL_VALUE).getValues(&data.ncMissingVal);
    var.getAtt(UNITS).getValues(data.units);
    var.getAtt(VALID_RANGE).getValues(data.validRange);
    var.getAtt(RESOLUTION).getValues(data.resolution);
    var.getAtt(COORDINATES).getValues(data.coordinates);
    var.getAtt(GRID_MAPPING).getValues(data.gridMapping);
    var.getAtt(CELL_METHODS).getValues(data.cellMethods);

    _readDataCategories(var, data.categories);

    data.values.assign( _numX*_numY, MISSING_DATA_VALUE);
    var.getVal(data.values.data());

  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

}

/*********************************************************************
 * _readQcDataVar()
 */

void GoesRProduct::_readQcDataVar(const std::string& name, QcDataContainer& qc_data,
				  NcxxVar& var, bool read_masks /* = false */ )
{
  static const string methodName = _className + "::" + __FUNCTION__;

  var  = _file.getVar(name);
  if(var.isNull() == true) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  cannot find variable, name: " << name << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }


  try {
    var.getAtt(LONG_NAME).getValues(qc_data.longName);
    var.getAtt(STANDARD_NAME).getValues(qc_data.standardName);
    var.getAtt(FILL_VALUE).getValues(&(qc_data.ncMissingVal));
    var.getAtt(UNITS).getValues(qc_data.units);
    var.getAtt(VALID_RANGE).getValues(qc_data.validRange);
    var.getAtt(COORDINATES).getValues(qc_data.coordinates);
    var.getAtt(GRID_MAPPING).getValues(qc_data.gridMapping);
    var.getAtt(CELL_METHODS).getValues(qc_data.cellMethods);

    if (read_masks == false) {
      _readFlags(var, qc_data.flags);
    }
    else {
      _readFlagsAndMasks(var, qc_data.flagsAndMasks);
    }
    
    qc_data.values.assign(_numX*_numY, 0);
    var.getVal(qc_data.values.data());
  }
  catch (NcxxException& e) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    cerr << "   exception: " << e.what() << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
}


/*********************************************************************
 * _setProductType()
 */

void GoesRProduct::_setProductType(const string &title)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  if(title == TITLE_LEVEL1B_RADIANCES) {
    _productType = PRODUCT_LEVEL1B_RADIANCES;
  }
  else if (title == TITLE_AEROSOL_DETECTION) {
    _productType = PRODUCT_AEROSOL_DETECTION;
  }
  else if (title == TITLE_AEROSOL_OPTICAL_DEPTH) {
    _productType = PRODUCT_AEROSOL_OPTICAL_DEPTH;
  }
  else if (title == TITLE_CLOUD_TOP_PHASE) {
    _productType = PRODUCT_CLOUD_TOP_PHASE;
  }
  else if (title == TITLE_CLOUD_TOP_HEIGHT) {
    _productType = PRODUCT_CLOUD_TOP_HEIGHT;
  }
  else if (title == TITLE_CLOUD_TOP_TEMPERATURE) {
    _productType = PRODUCT_CLOUD_TOP_TEMPERATURE;
  }
  else if (title == TITLE_CLOUD_TOP_PRESSURE) {
    _productType = PRODUCT_CLOUD_TOP_PRESSURE;
  }
  else if (title == TITLE_DERIVED_STABILITY_INDICES) {
    _productType = PRODUCT_DERIVED_STABILITY_INDICES;
  }
  else if (title == TITLE_TOTAL_PRECIPITABLE_WATER) {
    _productType = PRODUCT_TOTAL_PRECIPITABLE_WATER;
  }
  else if (title == TITLE_CLEAR_SKY_MASK) {
    _productType = PRODUCT_CLEAR_SKY_MASK;
  }
  else if (title == TITLE_FIRE_CHARACTERIZATION) {
    _productType = PRODUCT_FIRE_CHARACTERIZATION;
  }
  else if (title == TITLE_LAND_SURFACE_TEMPERATURE) {
    _productType = PRODUCT_LAND_SURFACE_TEMPERATURE;
  }
  else if (title == TITLE_CLOUD_OPTICAL_DEPTH) {
    _productType = PRODUCT_CLOUD_OPTICAL_DEPTH;
  }
  else if (title == TITLE_CLOUD_PARTICLE_SIZE) {
    _productType = PRODUCT_CLOUD_PARTICLE_SIZE;
  }
  else if (title == TITLE_DERIVED_MOTION_WINDS) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  Processing derived motion winds not implemented." << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  else if (title == TITLE_GLOBAL_LIGHTNING) {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  Processing global lightning not implemented." << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  else {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  Unknown product type" << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }
  
}

/*********************************************************************
 * _setScanType()
 */

void GoesRProduct::_setScanType(const string &scan_id)
{
  static const string methodName = _className + "::" + __FUNCTION__;
  
  if(scan_id == FULL_DISK_SCAN_NAME) {
    _scanType = SCAN_FULL_DISK;
  }
  else if (scan_id == CONUS_SCAN_NAME) {
    _scanType = SCAN_CONUS;
  }
  else if (scan_id == MESOSCALE_SCAN_NAME) {
    _scanType = SCAN_MESOSCALE;
  }
  else {
    NcxxErrStr err;
    stringstream  info;
    info << ERROR_STR << methodName << endl;
    info << "  Unknown scan ID." << endl;
    err.addErrStr(info.str());
    throw(NcxxException(err.getErrStr(), __FILE__, __LINE__));
  }

}




/////////////////////////////////////////////////////////////////////////////////////
// string constant instantiation
//


// Level-1 and Level-2 Product Dimensions
const char* GoesRProduct::X_DIM = "x";
const char* GoesRProduct::Y_DIM = "y";
const char* GoesRProduct::NUMBER_OF_TIME_BOUNDS = "number_of_time_bounds";
const char* GoesRProduct::BAND_DIM = "band";
const char* GoesRProduct::NUMBER_OF_IMAGE_BOUNDS = "number_of_image_bounds";

// Level-1 and Level-2 product Global Attributes
const char* GoesRProduct::NAMING_AUTHORITY = "naming_authority";
const char* GoesRProduct::CONVENTIONS = "Conventions";
const char* GoesRProduct::METADATA_CONVENTIONS = "Metadata_Conventions";
const char* GoesRProduct::STANDARD_NAME_VOCABULARY  = "standard_name_vocabulary";
const char* GoesRProduct::INSTITUTION = "institution";
const char* GoesRProduct::PROJECT = "project";
const char* GoesRProduct::PRODUCTION_SITE = "production_site";
const char* GoesRProduct::PRODUCTION_ENVIRONMENT = "production_environment";
const char* GoesRProduct::SPATIAL_RESOLUTION = "spatial_resolution";
const char* GoesRProduct::ORBITAL_SLOT = "orbital_slot";
const char* GoesRProduct::PLATFORM_ID = "platform_ID";
const char* GoesRProduct::INSTRUMENT_TYPE = "instrument_type";
const char* GoesRProduct::SCENE_ID = "scene_id";
const char* GoesRProduct::INSTRUMENT_ID = "instrument_ID";
const char* GoesRProduct::TITLE = "title";
const char* GoesRProduct::SUMMARY = "summary";
const char* GoesRProduct::KEYWORDS = "keywords";
const char* GoesRProduct::KEYWORDS_VOCABULARY = "keywords_vocabulary";
const char* GoesRProduct::ISO_SERIES_METADATA_ID = "iso_series_metadata_id";
const char* GoesRProduct::LICENSE = "license";
const char* GoesRProduct::PROCESSING_LEVEL = "processing_level";
const char* GoesRProduct::CDM_DATA_TYPE = "cdm_data_type";
const char* GoesRProduct::DATASET_NAME = "dataset_name";
const char* GoesRProduct::PRODUCTION_DATA_SOURCE = "production_data_source";
const char* GoesRProduct::TIMELINE_ID = "timeline_id";
const char* GoesRProduct::DATE_CREATED = "date_created";
const char* GoesRProduct::TIME_COVERAGE_START = "time_coverage_start";
const char* GoesRProduct::TIME_COVERAGE_END = "time_coverage_end";

// Level-1 and Level-2  Variables
const char* GoesRProduct::X_COORD = "x";
const char* GoesRProduct::Y_COORD = "y";
const char* GoesRProduct::DQF_OVERALL = "DQF_Overall";
const char* GoesRProduct::DQF_RETRIEVAL = "DQF_Retrieval";
const char* GoesRProduct::DQF_SKINTEMP = "DQF_SkinTemp";
const char* GoesRProduct::DATA_QUALITY_FLAG = "DQF";
const char* GoesRProduct::TIME = "t";
const char* GoesRProduct::TIME_BOUNDS = "time_bounds";
const char* GoesRProduct::GOES_IMAGER_PROJECTION =
  "goes_imager_projection";
const char* GoesRProduct::X_IMAGE = "x_image";
const char* GoesRProduct::Y_IMAGE = "y_image";
const char* GoesRProduct::X_IMAGE_BOUNDS = "x_image_bounds";
const char* GoesRProduct::Y_IMAGE_BOUNDS = "y_image_bounds";
const char* GoesRProduct::NOMINAL_SATELLITE_SUBPOINT_LAT =
  "nominal_satellite_subpoint_lat";
const char* GoesRProduct::NOMINAL_SATELLITE_SUBPOINT_LON =
  "nominal_satellite_subpoint_lon";
const char* GoesRProduct::NOMINAL_SATELLITE_HEIGHT =
  "nominal_satellite_height";
const char* GoesRProduct::GEOSPATIAL_LAT_LON_EXTENT =
  "geospatial_lat_lon_extent";
const char* GoesRProduct::ALGORITHM_DYNAMIC_INPUT_DATA_CONTAINER =
  "algorithm_dynamic_input_data_container";
const char* GoesRProduct::PROCESSING_PARM_VERSION_CONTAINER =
  "processing_parm_version_container";
const char* GoesRProduct::ALGORITHM_PRODUCT_VERSION_CONTAINER =
  "algorithm_product_version_container";

// Level-1 observation, Level-2 product, and data quality flag Variable Attributes
const char* GoesRProduct::SCALE_FACTOR = "scale_factor";
const char* GoesRProduct::ADD_OFFSET = "add_offset";
const char* GoesRProduct::UNITS = "units";
const char* GoesRProduct::AXIS = "axis";
const char* GoesRProduct::LONG_NAME = "long_name";
const char* GoesRProduct::STANDARD_NAME = "standard_name";
const char* GoesRProduct::FILL_VALUE = "_FillValue";
const char* GoesRProduct::UNSIGNED = "_Unsigned";
const char* GoesRProduct::SENSOR_BAND_BIT_DEPTH = "sensor_band_bit_depth";
const char* GoesRProduct::VALID_RANGE = "valid_range";
const char* GoesRProduct::RESOLUTION = "resolution";
const char* GoesRProduct::COORDINATES = "coordinates";
const char* GoesRProduct::GRID_MAPPING = "grid_mapping";
const char* GoesRProduct::CELL_METHODS = "cell_methods";
const char* GoesRProduct::ANCILLARY_VARIABLES = "ancillary_variables";
const char* GoesRProduct::FLAG_VALUES = "flag_values";
const char* GoesRProduct::FLAG_MEANINGS = "flag_meanings";
const char* GoesRProduct::FLAG_MASKS = "flag_masks";
const char* GoesRProduct::NUMBER_OF_QF_VALUES = "number_of_qf_values";
const char* GoesRProduct::BOUNDS = "bounds";
const char* GoesRProduct::GRID_MAPPING_NAME = "grid_mapping_name";
const char* GoesRProduct::PERSPECTIVE_POINT_HEIGHT = "perspective_point_height";
const char* GoesRProduct::SEMI_MAJOR_AXIS = "semi_major_axis";
const char* GoesRProduct::SEMI_MINOR_AXIS = "semi_minor_axis";
const char* GoesRProduct::INVERSE_FLATTENING = "inverse_flattening";
const char* GoesRProduct::LATITUDE_OF_PROJECTION_ORIGIN = "latitude_of_projection_origin";
const char* GoesRProduct::LONGITUDE_OF_PROJECTION_ORIGIN = "longitude_of_projection_origin";
const char* GoesRProduct::SWEEP_ANGLE_AXIS = "sweep_angle_axis";
const char* GoesRProduct::GEOSPATIAL_WESTBOUND_LONGITUDE = "geospatial_westbound_longitude";
const char* GoesRProduct::GEOSPATIAL_NORTHBOUND_LATITUDE = "geospatial_northbound_latitude";
const char* GoesRProduct::GEOSPATIAL_EASTBOUND_LONGITUDE = "geospatial_eastbound_longitude";
const char* GoesRProduct::GEOSPATIAL_SOUTHBOUND_LATITUDE = "geospatial_southbound_latitude";
const char* GoesRProduct::GEOSPATIAL_LAT_CENTER = "geospatial_lat_center";
const char* GoesRProduct::GEOSPATIAL_LON_CENTER = "geospatial_lon_center";
const char* GoesRProduct::GEOSPATIAL_LAT_NADIR = "geospatial_lat_nadir";
const char* GoesRProduct::GEOSPATIAL_LON_NADIR = "geospatial_lon_nadir";
const char* GoesRProduct::GEOSPATIAL_LAT_UNITS = "geospatial_lat_units";
const char* GoesRProduct::GEOSPATIAL_LON_UNITS = "geospatial_lon_units";
const char* GoesRProduct::INPUT_ABI_L0_DATA = "input_ABI_L0_data";
const char* GoesRProduct::L1B_PROCESSING_PARM_VERSION = "L1b_processing_parm_version";
const char* GoesRProduct::ALGORITHM_VERSION = "algorithm_version";
const char* GoesRProduct::PRODUCT_VERSION = "product_version";
const char* GoesRProduct::OUTLIER_PIXELS = "outlier_pixels";
const char* GoesRProduct::CLOUD_PIXELS = "cloud_pixels";
const char* GoesRProduct::LATITUDE = "latitude";
const char* GoesRProduct::LATITUDE_BOUNDS = "latitude_bounds";
const char* GoesRProduct::NUMBER_OF_LAT_BOUNDS = "number_of_lat_bounds";
const char* GoesRProduct::SOUNDING_EMISSIVE_BANDS = "sounding_emissive_bands";

// Title strings that are used to set the product type
const char* GoesRProduct::TITLE_LEVEL1B_RADIANCES = "ABI L1b Radiances";
const char* GoesRProduct::TITLE_AEROSOL_DETECTION = "ABI L2 Aerosol Detection";
const char* GoesRProduct::TITLE_AEROSOL_OPTICAL_DEPTH = "ABI L2 Aerosol Optical Depth";
const char* GoesRProduct::TITLE_CLOUD_TOP_PHASE = "ABI L2 Cloud Top Phase";
const char* GoesRProduct::TITLE_CLOUD_TOP_HEIGHT = "ABI L2 Cloud Top Height";
const char* GoesRProduct::TITLE_CLOUD_TOP_TEMPERATURE = "ABI L2 Cloud Top Temperature";
const char* GoesRProduct::TITLE_CLOUD_TOP_PRESSURE = "ABI L2 Cloud Top Pressure";
const char* GoesRProduct::TITLE_DERIVED_STABILITY_INDICES = "ABI L2 Derived Stability Indices";
const char* GoesRProduct::TITLE_TOTAL_PRECIPITABLE_WATER = "ABI L2 Total Precipitable Water";
const char* GoesRProduct::TITLE_CLEAR_SKY_MASK = "ABI L2 Clear Sky Mask";
const char* GoesRProduct::TITLE_FIRE_CHARACTERIZATION = "ABI L2 Fire - Hot Spot Characterization";
const char* GoesRProduct::TITLE_LAND_SURFACE_TEMPERATURE = "ABI L2 Land Surface (Skin) Temperature";
const char* GoesRProduct::TITLE_CLOUD_OPTICAL_DEPTH = "ABI L2 Cloud Optical Depth at 640 nm";
const char* GoesRProduct::TITLE_CLOUD_PARTICLE_SIZE = "ABI L2 Cloud Particle Size";
const char* GoesRProduct::TITLE_DERIVED_MOTION_WINDS = "ABI L2 Derived Motion Winds";
const char* GoesRProduct::TITLE_GLOBAL_LIGHTNING = "GLM L2 Lightning Detections: Events, Groups, and Flashes";

const char* GoesRProduct::RETRIEVAL_LOCAL_ZENITH_ANGLE =
  "retrieval_local_zenith_angle";
const char* GoesRProduct::QUANTITATIVE_LOCAL_ZENITH_ANGLE =
  "quantitative_local_zenith_angle";
const char* GoesRProduct::RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS =
  "retrieval_local_zenith_angle_bounds";
const char* GoesRProduct::QUANTITATIVE_LOCAL_ZENITH_ANGLE_BOUNDS =
  "quantitative_local_zenith_angle_bounds";
const char* GoesRProduct::RETRIEVAL_SOLAR_ZENITH_ANGLE =
  "retrieval_solar_zenith_angle";
const char* GoesRProduct::QUANTITATIVE_SOLAR_ZENITH_ANGLE =
  "quantitative_solar_zenith_angle";
const char* GoesRProduct::RETRIEVAL_SOLAR_ZENITH_ANGLE_BOUNDS =
  "retrieval_solar_zenith_angle_bounds";
const char* GoesRProduct::QUANTITATIVE_SOLAR_ZENITH_ANGLE_BOUNDS =
  "quantitative_solar_zenith_angle_bounds";
const char* GoesRProduct::SOLAR_ZENITH_ANGLE = "solar_zenith_angle";
const char* GoesRProduct::SOLAR_ZENITH_ANGLE_BOUNDS =
  "solar_zenith_angle_bounds";
const char* GoesRProduct::LOCAL_ZENITH_ANGLE = "local_zenith_angle";
const char* GoesRProduct::LOCAL_ZENITH_ANGLE_BOUNDS =
  "local_zenith_angle_bounds";
const char* GoesRProduct::TWILIGHT_SOLAR_ZENITH_ANGLE =
  "twilight_solar_zenith_angle";
const char* GoesRProduct::TWILIGHT_SOLAR_ZENITH_ANGLE_BOUNDS =
  "twilight_solar_zenith_angle_bounds";
const char* GoesRProduct::SUNGLINT_ANGLE = "sunglint_angle";
const char* GoesRProduct::SUNGLINT_ANGLE_BOUNDS = "sunglint_angle_bounds";
const char* GoesRProduct::DAY_RETRIEVAL_LOCAL_ZENITH_ANGLE =
  "day_retrieval_local_zenith_angle";
const char* GoesRProduct::NIGHT_RETRIEVAL_LOCAL_ZENITH_ANGLE =
  "night_retrieval_local_zenith_angle";
const char* GoesRProduct::DAY_RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS =
  "day_retrieval_local_zenith_angle_bounds";
const char* GoesRProduct::NIGHT_RETRIEVAL_LOCAL_ZENITH_ANGLE_BOUNDS =
  "night_retrieval_local_zenith_angle_bounds";
const char* GoesRProduct::DAY_SOLAR_ZENITH_ANGLE = "day_solar_zenith_angle";
const char* GoesRProduct::NIGHT_SOLAR_ZENITH_ANGLE = "night_solar_zenith_angle";
const char* GoesRProduct::DAY_ALGORITHM_SOLAR_ZENITH_ANGLE =
  "day_algorithm_solar_zenith_angle";
const char* GoesRProduct::NIGHT_ALGORITHM_SOLAR_ZENITH_ANGLE =
  "night_algorithm_solar_zenith_angle";
const char* GoesRProduct::DAY_SOLAR_ZENITH_ANGLE_BOUNDS =
  "day_solar_zenith_angle_bounds";
const char* GoesRProduct::NIGHT_SOLAR_ZENITH_ANGLE_BOUNDS =
  "night_solar_zenith_angle_bounds";
const char* GoesRProduct::DAY_ALGORITHM_SOLAR_ZENITH_ANGLE_BOUNDS =
  "day_algorithm_solar_zenith_angle_bounds";
const char* GoesRProduct::NIGHT_ALGORITHM_SOLAR_ZENITH_ANGLE_BOUNDS =
  "night_algorithm_solar_zenith_angle_bounds";
const char* GoesRProduct::NUMBER_OF_LZA_BOUNDS = "number_of_LZA_bounds";
const char* GoesRProduct::NUMBER_OF_SZA_BOUNDS = "number_of_SZA_bounds";
const char* GoesRProduct::NUMBER_OF_SUNGLINT_ANGLE_BOUNDS =
  "number_of_sunglint_angle_bounds";


// strings to identify type of scan
const char* GoesRProduct::FULL_DISK_SCAN_NAME = "Full Disk";
const char* GoesRProduct::CONUS_SCAN_NAME = "CONUS";
const char* GoesRProduct::MESOSCALE_SCAN_NAME = "Mesoscale";

