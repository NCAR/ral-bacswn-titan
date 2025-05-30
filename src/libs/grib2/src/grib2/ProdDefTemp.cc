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
///////////////////////////////////////////////////////////
// ProdDefTemp.cc : implementation for the ProdDefTemp class 
//                  This is an abstract base class to allow
//                  objects to represent various product 
//                  definition templates
//       
//
///////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>
#include <map>

#include <grib2/ProdDefTemp.hh>

using namespace std;

namespace Grib2 {

/////////////////////////////////////////////////////////////////////////
//
// The following tables have been maintained from NCEP's web page at:
// http://www.nco.ncep.noaa.gov/pmb/docs/grib2/grib2_doc.shtml
//
//////////////////////////////////////////////////////////////////////////

/*                    GRIB2 Code Table 4.5                       */
/*                FIXED SURFACE TYPES AND UNITS                  */
const ProdDefTemp::_GRIB2SurfTable ProdDefTemp::_surface[] = {
   /* 1 */ {"SFC", "Ground or water surface", "-"},
   /* 2 */ {"CBL", "Cloud base level", "-"},
   /* 3 */ {"CTL", "Level of cloud tops", "-"},
   /* 4 */ {"0DEG", "Level of 0 degree C isotherm", "-"},
   /* 5 */ {"ADCL", "Level of adiabatic condensation lifted from the surface", "-"},
   /* 6 */ {"MWSL", "Maximum wind level", "-"},
   /* 7 */ {"TRO", "Tropopause", "-"},
   /* 8 */ {"NTAT", "Nominal top of atmosphere", "-"},
   /* 9 */ {"SEAB", "Sea bottom", "-"},
   /* 10 */ {"EATM", "Entire Atmosphere", "-"},
   /* 11 */ {"CBB", "Cumulonimbus Base", "m"},
   /* 12 */ {"CBT", "Cumulonimbus Top", "m"},
   /* 13 */ {"", "Lowest level where vertically integrated cloud cover exceeds the specified percentage (cloud base for a given percentage cloud cover)", "%"},
   /* 14 */ {"LFC", "Level of free convection", "-"},
   /* 15 */ {"CCL", "Convection condensation level", "-"},
   /* 16 */ {"LNB", "Level of neutral buoyancy or equilibrium", "-"},
   /* 17 */ {"MUDL", "Departure level of the most unstable parcel of air", "-"},
   /* 18 */ {"", "Departure level of a mixed layer parcel of air with specified layer depth", "Pa"},
   /* 19 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 20 */ {"TMPL", "Isothermal level", "K"},
   /* 21 */ {"", "Lowest level where mass density exceeds the specified value", "kg m-3"},
   /* 22 */ {"", "Highest level where mass density exceeds the specified value", "kg m-3"},
   /* 23 */ {"", "Lowest level where air concentration exceeds the specified value", "Bq m-3"},
   /* 24 */ {"", "Highest level where air concentration exceeds the specified value", "Bq m-3"},
   /* 25 */ {"", "Highest level where radar reflectivity exceeds the specified value", "dBZ"},
   /* 100 */ {"ISBL", "Isobaric surface", "Pa"},
   /* 101 */ {"MSL", "Mean sea level", "-"},
   /* 102 */ {"GPML", "Specific altitude above mean sea level", "m"},
   /* 103 */ {"HTGL", "Specified height level above ground", "m"},
   /* 104 */ {"SIGL", "Sigma level", "sigma value"},
   /* 105 */ {"HYBL", "Hybrid level", ""},
   /* 106 */ {"DBLL", "Depth below land surface", "m"},
   /* 107 */ {"THEL", "Isentropic (theta) level", "K"},
   /* 108 */ {"SPDL", "Level at specified pressure difference from ground to level", "Pa"},
   /* 109 */ {"PVL", "Potential vorticity surface", "K m^2 kg^-1 s^-1"},
   /* 110 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 111 */ {"ETAL", "Eta level", ""},
   /* 112 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 113 */ {"LOGHYB", "Logarithmic Hybrid Coordinate", "-"},
   /* 114 */ {"SNOWL", "Snow Level", "Numeric"},
   /* 115 */ {"SIGHL", "Sigma Height Level", "-"},
   /* 116 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 117 */ {"MLD", "Mixed layer depth", "m"}, 
   /* 118 */ {"HYBH", "Hybrid Height Level", "-"},
   /* 119 */ {"HYBP", "Hybrid Pressure Level", "-"},
   /* 120 */ {"PREST", "Pressure Thickness", "Pa"},  // REMOVED
   /* 150 */ {"", "Generalized Vertical Height Coordinate", "-"},
   /* 151 */ {"SOILL", "Soil level", "-"},
   /* 160 */ {"DBSL", "Depth below sea level", "m"},
   /* 161 */ {"", "Depth Below Water Surface", "m"},
   /* 162 */ {"", "Lake or River Bottom", "-"},
   /* 163 */ {"", "Bottom Of Sediment Layer", "-"},
   /* 164 */ {"", "Bottom Of Thermally Active Sediment Layer", "-"},
   /* 165 */ {"", "Bottom Of Sediment Layer Penetrated By Thermal Wave", "-"},
   /* 166 */ {"", "Mixing Layer", "-"},
   /* 167 */ {"", "Bottom of Root Zone", "-"},
   /* 168 */ {"", "Ocean Model Level", "-"},
   /* 169 */ {"", "Ocean level defined by water density", "kg m-3"},
   /* 170 */ {"", "Ocean level defined by water potential temperature", "K"},
   /* 171 */ {"", "Ocean level defined by vertical eddy diffusivity difference from near-surface to level", "m^2 s^-1"},
   /* 172 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 173 */ {"UNKNOWN", "Unknown Surface Type", ""}, 
   /* 174 */ {"", "Top Surface of Ice on Sea, Lake or River", "-"},
   /* 175 */ {"", "Top Surface of Ice, under Snow, on Sea, Lake or River", "-"},
   /* 176 */ {"", "Bottom Surface (underside) Ice on Sea, Lake or River", "-"},
   /* 177 */ {"", "Deep Soil (of indefinite depth)", "-"},
   /* 178 */ {"UNKNOWN", "Unknown Surface Type", ""}, 
   /* 179 */ {"", "Top Surface of Glacier Ice and Inland Ice", "-"},
   /* 180 */ {"", "Deep Inland or Glacier Ice (of indefinite depth)", "-"},
   /* 181 */ {"", "Grid Tile Land Fraction as a Model Surface", "-"},
   /* 182 */ {"", "Grid Tile Water Fraction as a Model Surface", "-"},
   /* 183 */ {"", "Grid Tile Ice Fraction on Sea, Lake or River as a Model Surface", "-"},
   /* 184 */ {"", "Grid Tile Glacier Ice and Inland Ice Fraction as a Model Surface", "-"},
   /* 200 */ {"EATM", "Entire atmosphere (considered as a single layer)", "-"},
   /* 201 */ {"EOCN", "Entire ocean (considered as a single layer)", "-"},
   /* 202 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 203 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 204 */ {"HTFL", "Highest tropospheric freezing level", "-"},
   /* 205 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 206 */ {"GCBL", "Grid scale cloud bottom level", "-"},
   /* 207 */ {"GCTL", "Grid scale cloud top level", "-"},
   /* 208 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 209 */ {"BCBL", "Boundary layer cloud bottom level", "-"},
   /* 210 */ {"BCTL", "Boundary layer cloud top level", "-"},
   /* 211 */ {"BCY", "Boundary layer cloud layer", "-"},
   /* 212 */ {"LCBL", "Low cloud bottom level", "-"},
   /* 213 */ {"LCTL", "Low cloud top level", "-"},
   /* 214 */ {"LCY", "Low cloud layer", "-"},
   /* 215 */ {"CCEIL", "Cloud ceiling", "-"},
   /* 216 */ {"EFFLT", "Effective Layer Top Level", "m"},
   /* 217 */ {"EFFLB", "Effective Layer Bottom Level", "m"},
   /* 218 */ {"EFFL", "Effective Layer", "m"},
   /* 219 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 220 */ {"PBL", "Planetary Boundary Layer", "-"},
   /* 221 */ {"LBHYBL", "Layer Between Two Hybrid Levels", "-"},
   /* 222 */ {"MCBL", "Middle cloud bottom level", "-"},
   /* 223 */ {"MCTL", "Middle cloud top level", "-"},
   /* 224 */ {"MCY", "Middle cloud layer", "-"},
   /* 232 */ {"HCBL", "High cloud bottom level", "-"},
   /* 233 */ {"HCTL", "High cloud top level", "-"},
   /* 234 */ {"HCY", "High cloud layer", "-"},
   /* 235 */ {"OITL", "Ocean Isotherm Level (1/10 deg C)", "C"},
   /* 236 */ {"OLYR", "Layer between two depths below ocean surface", "-"},	
   /* 237 */ {"OBML", "Bottom of Ocean Mixed Layer", "m"},
   /* 238 */ {"OBIL", "Bottom of Ocean Isothermal Layer", "m"},
   /* 239 */ {"S26CY", "Layer Ocean Surface and 26C Ocean Isothermal Level", "-"},
   /* 240 */ {"OMXL", "Ocean Mixed Layer", "-"},
   /* 241 */ {"OSEQD", "Ordered Sequence of Data", "-"},
   /* 242 */ {"CCBL", "Convective cloud bottom level", "-"},
   /* 243 */ {"CCTL", "Convective cloud top level", "-"},
   /* 244 */ {"CCY", "Convective cloud layer", "-"},
   /* 245 */ {"LLTW", "Lowest level of the wet bulb zero", "-"},
   /* 246 */ {"MTHE", "Maximum equivalent potential temperature level", "-"},
   /* 247 */ {"EHLT", "Equilibrium level", "-"},
   /* 248 */ {"SCBL", "Shallow convective cloud bottom level", "-"},
   /* 249 */ {"SCTL", "Shallow convective cloud top level", "-"},
   /* 250 */ {"UNKNOWN", "Unknown Surface Type", ""},
   /* 251 */ {"DCBL", "Deep convective cloud bottom level", "-"},
   /* 252 */ {"DCTL", "Deep convective cloud top level", "-"},
   /* 253 */ {"LBLSW", "Lowest bottom level of supercooled liquid water layer", "-"},
   /* 254 */ {"HTLSW", "Highest top level of supercooled liquid water layer", "-"},
   /* 255 */ {"MISSING", "", ""},
};


/*                       GRIB2 Code Table 4.2                    */
/* PARAMETER NUMBER BY PRODUCT DISCIPLINE AND PARAMETER CATEGORY */

// Note on comment style:  4.2 : 1.2
// Fields are unique to a discipline number a parameter category and parameter number.
// Discipline Number is octet 7 in section 0 (IS, Indicator Section).
// Parameter Category is octet 10 in section 4 (PDS, Product Definition Section).
// Parameter Number is octet 11 in section 4 (PDS, Product Definition Section).
//
// Numbers greater than 191 are for local use only, 
// these require checking the orginating/generating center and subcenter
// found in the IS, Identification Section, and using the appropriate local use table.
//
// For example the Temperature (TMP) field is 4.2 : 0.0 : 0
// 4.2 for Code Table 4.2
// 0.0 for Discipline 0, Category 0
// 0 for Parameter Number 0  (the index into the meteoTemp array)

/* Known Disciplines:
   0  Meteorological Products
   1  Hydrological Products
   2  Land Surface Products
   3  Space Products
   10 Oceanographic Products  */

/* Known Categories, by Discipline:
  Discipline 0, Meteorological products:
    Category 0 	 Temperature
    Category 1 	 Moisture
    Category 2 	 Momentum
    Category 3 	 Mass
    Category 4 	 Short wave radiation
    Category 5 	 Long wave radiation
    Category 6 	 Cloud
    Category 7 	 Thermodynamic stability indices
    Category 8   Kinematic stability indicies
    Category 9   Temperature probabilities 
    Category 10  Moisture probabilities 
    Category 11  Momentum probabilities
    Category 12  Mass probabilities 
    Category 13  Aerosols
    Category 14  Trace gases (e.g. Ozone, CO2)
    Category 15  Radar
    Category 18  Nuclear/radiology
    Category 19  Physical atmospheric properties
    Category 20-189  Reserved For Future Use - Unknown 
    Category 190 CCITT IA5 string
    Category 191 Miscellaneous
    Category 192-254 Local Use
    Category 255 Missing 

  Discipline 1, Hydrologic products
    Category 0 	Hydrology basic products
    Category 1 	Hydrology probabilities
    Category 2-191  Reserved For Future Use - Unknown
    Category 192-254 Local Use
    Category 255 Missing 

  Discipline 2, Land Surface products
    Category 0 	Vegetation/Biomass
    Category 3  Soil products
    Category 4-191  Reserved For Future Use - Unknown
    Category 192-254  Local Use
    Category 255 Missing 

  Discipline 3, Satellite Remote Sensing products
    Category 0 	Image format products
    Category 1  Quantitative products
    Category 2  Cloud Properties products
    Category 3  Flight Rules Conditions products
    Category 4  Volcanic Ash products
    Category 5  Sea-Surface Temperature products
    Category 6  Solar Radiation products
    Category 7-191  Reserved For Future Use - Unknown 
    Category 192-254 Local Use
    Category 255 Missing 

  Discipline 10, Oceanographic products
    Category 0 	Waves
    Category 1  Currents
    Category 2  Ice
    Category 3  Surface properties
    Category 4  Sub-surface properties
    Category 5-190  Reserved For Future Use - Unknown 
    Category 191 Miscellaneous
    Category 192-254 Local Use
    Category 255 Missing 
*/

/* GRIB2 Code table 4.2 : 0.0 */
/* Meteorological products, temperature category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoTemp[33] = {
   /* 0 */ {"TMP", "Temperature", "K"},
   /* 1 */ {"VTMP", "Virtual temperature", "K"},
   /* 2 */ {"POT", "Potential temperature", "K"},
   /* 3 */ {"EPOT", "Pseudo-adiabatic potential temperature", "K"},
   /* 4 */ {"TMAX", "Maximum Temperature", "K"},
   /* 5 */ {"TMIN", "Minimum Temperature", "K"},
   /* 6 */ {"DPT", "Dew point temperature", "K"},
   /* 7 */ {"DEPR", "Dew point depression", "K"},
   /* 8 */ {"LAPR", "Lapse rate", "K/m"},
   /* 9 */ {"TMPA", "Temperature anomaly", "K"},
   /* 10 */ {"LHTFL", "Latent heat net flux", "W m^-2"},
   /* 11 */ {"SHTFL", "Sensible heat net flux", "W m^-2"},
   /* 12 */ {"HEATX", "Heat index", "K"},
   /* 13 */ {"WCF", "Wind chill factor", "K"},
   /* 14 */ {"MINDPD", "Minimum dew point depression", "K"},
   /* 15 */ {"VPTMP", "Virtual potential temperature", "K"},
   /* 16 */ {"SNOHF", "Snow phase change heat flux", "W m^-2"},
   /* 17 */ {"SKINT", "Skin Temperature", "K"},
   /* 18 */ {"SNOT", "Snow Temperature", "K"},
   /* 19 */ {"TTCHT", "Turbulent Transfer Coefficient for Heat", "numeric"}, 
   /* 20 */ {"TDCHT", "Turbulent Diffusion Coefficient for Heat", "m^2 s^-1"},
   /* 21 */ {"APTMP", "Apparent Temperature", "K"},
   /* 22 */ {"TTSWR", "Temperature Tendency due to Short-Wave Radiation", "K s^-1"},
   /* 23 */ {"TTLWR", "Temperature Tendency due to Long-Wave Radiation", "K s^-1"},
   /* 24 */ {"TTSWRCS", "Temperature Tendency due to Short-Wave Radiation, Clear Sky", "K s^-1"},
   /* 25 */ {"TTLWRCS", "Temperature Tendency due to Long-Wave Radiation, Clear Sky", "K s^-1"},
   /* 26 */ {"TTPARM", "Temperature Tendency due to parameterizations", "K s^-1"},
   /* 27 */ {"WETBT", "Wet Bulb Temperature", "K"},
   /* 28 */ {"UCTMP", "Unbalanced Component of Temperature", "K"},
   /* 29 */ {"TMPADV", "Temperature Advection", "K s-1"},
   /* 30 */ {"LHFLXE", "Latent Heat Net Flux Due to Evaporation", "W m^-2"},
   /* 31 */ {"LHFLXS", "Latent Heat Net Flux Due to Sublimation", "W m^-2"},
   /* 32 */ {"WETBPT", "Wet-Bulb Potential Temperature", "K"},
};
  
/* GRIB2 Code table 4.2 : 0.1 */
/* Meteorological products, moisture category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoMoist[151] = {
   /* 0 */ {"SPFH",   "Specific humidity", "kg kg^-1"},
   /* 1 */ {"RH",     "Relative Humidity", "%"},
   /* 2 */ {"MIXR",   "Humidity mixing ratio", "kg kg^-1"},
   /* 3 */ {"PWAT",   "Precipitable water", "kg m^-2"},
   /* 4 */ {"VAPP",   "Vapor Pressure", "Pa"},
   /* 5 */ {"SATD",   "Saturation deficit", "Pa"},
   /* 6 */ {"EVP",    "Evaporation", "kg m^-2"},
   /* 7 */ {"PRATE",  "Precipitation rate", "kg m^-2 s^-1"},
   /* 8 */ {"APCP",   "Total precipitation", "kg m^-2"},
   /* 9 */ {"NCPCP",  "Large scale precipitation", "kg m^-2"},
   /* 10 */ {"ACPCP", "Convective precipitation", "kg m^-2"},
   /* 11 */ {"SNOD",  "Snow depth", "m"},
   /* 12 */ {"SRWEQ", "Snowfall rate water equivalent", "kg m^-2 s^-1"},
   /* 13 */ {"WEASD", "Water equivalent of accumulated snow depth","kg m^-2"},
   /* 14 */ {"SNOC",  "Convective snow", "kg m^-2"},
   /* 15 */ {"SNOL",  "Large scale snow", "kg m^-2"},
   /* 16 */ {"SNOM",  "Snow melt", "kg m^-2"},
   /* 17 */ {"SNOAGE","Snow age", "day"},
   /* 18 */ {"ABSH",    "Absolute humidity", "kg m^-3"},
   /* 19 */ {"PTYPE", "Precipitation type", "categorical"}, //(1 Rain, 2 Thunderstorm, "
             //"3 Freezing Rain, 4 Mixed/ice, 5 snow, 255 missing)
   /* 20 */ {"ILIQW",   "Integrated liquid water", "kg m^-2"},
   /* 21 */ {"COND",  "Condensate", "kg kg^-1"},
   /* 22 */ {"CLWMR", "Cloud Water Mixing Ratio", "kg kg^-1"},
   /* 23 */ {"ICMR",  "Ice water mixing ratio", "kg kg^-1"},
   /* 24 */ {"RWMR",  "Rain Water Mixing Ratio", "kg kg^-1"},
   /* 25 */ {"SNMR",  "Snow Water Mixing Ratio", "kg kg^-1"},
   /* 26 */ {"MCONV",   "Horizontal moisture convergence", "kg kg^-1 s^-1"},
   /* 27 */ {"MAXRH", "Maximum relative humidity", "%"},
   /* 28 */ {"MAXAH", "Maximum absolute humidity", "kg m^-3"},
   /* 29 */ {"ASNOW", "Total snowfall", "m"},
   /* 30 */ {"PWCAT", "Precipitable water category", "-"},
   /* 31 */ {"HAIL",  "Hail", "m"},
   /* 32 */ {"GRLE", "Graupel (snow pellets)", "kg kg^-1"},
   /* 33 */ {"CRAIN", "Categorical Rain", "categorical"}, //(0 no, 1 yes)
   /* 34 */ {"CFRZR", "Categorical Freezing Rain", "categorical"}, //(0 no, 1 yes)
   /* 35 */ {"CICEP", "Categorical Ice Pellets", "categorical"}, //(0 no, 1 yes)
   /* 36 */ {"CSNOW", "Categorical Snow", "categorical"}, //(0 no, 1 yes)
   /* 37 */ {"CPRAT", "Convective Precipitation Rate", "kg m^-2 s^-1"},
   /* 38 */ {"MCONV", "Horizontal Moisture Divergence", "kg kg^-1 s^-1"},
   /* 39 */ {"CPOFP", "Percent frozen precipitation", "%"},
   /* 40 */ {"PEVAP", "Potential Evaporation", "kg m^-2"},
   /* 41 */ {"PEVPR", "Potential Evaporation Rate", "W m^-2"},
   /* 42 */ {"SNOWC", "Snow Cover", "%"},
   /* 43 */ {"FRAIN", "Rain Fraction of Total Liquid Water", "proportion"},
   /* 44 */ {"RIME",  "Rime Factor", "Numeric"},
   /* 45 */ {"TCOLR", "Total Column Integrated Rain", "kg m^-2"},
   /* 46 */ {"TCOLS", "Total Column Integrated Snow", "kg m^-2"},
   /* 47 */ {"LSWP", "Large Scale Water Precipitation (Non-Convective)", "kg m^-2"},
   /* 48 */ {"CWP", "Convective Water Precipitation", "kg m^-2"},
   /* 49 */ {"TWATP", "Total Water Precipitation", "kg m^-2"},
   /* 50 */ {"TSNOWP", "Total Snow Precipitation", "kg m^-2"},
   /* 51 */ {"TCWAT", "Total Column Water (Vertically integrated total water"
	     " (vapour+cloud water/ice)", "kg m^-2"},
   /* 52 */ {"TPRATE", "Total Precipitation Rate", "kg m^-2 s^-1"},
   /* 53 */ {"TSRWE", "Total Snowfall Rate Water Equivalent", "kg m^-2 s^-1"},
   /* 54 */ {"LSPRATE", "Large Scale Precipitation Rate", "kg m^-2 s^-1"},
   /* 55 */ {"CSRWE", "Convective Snowfall Rate Water Equivalent", "kg m^-2 s^-1"},
   /* 56 */ {"LSSRWE", "Large Scale Snowfall Rate Water Equivalent", "kg m^-2 s^-1"},
   /* 57 */ {"TSRATE", "Total Snowfall Rate", "m s^-1"},
   /* 58 */ {"CSRATE", "Convective Snowfall Rate", "m s^-1"},
   /* 59 */ {"LSSRATE", "Large Scale Snowfall Rate", "m s^-1"},
   /* 60 */ {"SDWE", "Snow Depth Water Equivalent", "kg m^-2"},
   /* 61 */ {"SDEN", "Snow Density", "kg m^-3"},
   /* 62 */ {"SEVAP", "Snow Evaporation", "kg m^-2"},
   /* 63 */ {"UNKNOWN-0_1_63", "Unknown Parameter", "-"},
   /* 64 */ {"TCIWV", "Total Column Integrated Water Vapour", "kg m^-2"},
   /* 65 */ {"RPRATE", "Rain Precipitation Rate", "kg m^-2 s^-1"},
   /* 66 */ {"SPRATE", "Snow Precipitation Rate", "kg m^-2 s^-1"},
   /* 67 */ {"FPRATE", "Freezing Rain Precipitation Rate", "kg m^-2 s^-1"},
   /* 68 */ {"IPPRATE", "Ice Pellets Precipitation Rate", "kg m^-2 s^-1"},
   /* 69 */ {"TCOLW", "Total Column Integrate Cloud Water", "kg m^-2"},
   /* 70 */ {"TCOLI", "Total Column Integrate Cloud Ice", "kg m^-2"},
   /* 71 */ {"HAILMXR", "Hail Mixing Ratio", "kg kg^-1"},
   /* 72 */ {"TCOLH", "Total Column Integrate Hail", "kg m^-2"},
   /* 73 */ {"HAILPR", "Hail Prepitation Rate", "kg m^-2 s^-1"},
   /* 74 */ {"TCOLG", "Total Column Integrate Graupel", "kg m^-2"},
   /* 75 */ {"GPRATE", "Graupel (Snow Pellets) Prepitation Rate", "kg m^-2 s^-1"},
   /* 76 */ {"CRRATE", "Convective Rain Rate", "kg m^-2 s^-1"},
   /* 77 */ {"LSRRATE", "Large Scale Rain Rate", "kg m^-2 s^-1"},
   /* 78 */ {"TCOLWA", "Total Column Integrate Water", "kg m^-2"},
   /* 79 */ {"EVARATE", "Evaporation Rate", "kg m^-2 s^-1"},
   /* 80 */ {"TOTCON", "Total Condensate", "kg kg^-1"},
   /* 81 */ {"TCICON", "Total Column-Integrate Condensate", "kg m^-2"},
   /* 82 */ {"CIMIXR", "Cloud Ice Mixing Ratio", "kg kg^-1"},
   /* 83 */ {"SCLLWC", "Specific Cloud Liquid Water Content", "kg kg^-1"},
   /* 84 */ {"SCLIWC", "Specific Cloud Ice Water Content", "kg kg^-1"},
   /* 85 */ {"SRAINW", "Specific Rain Water Content", "kg kg^-1"},
   /* 86 */ {"SSNOWW", "Specific Snow Water Content", "kg kg^-1"},
   /* 87 */ {"STRPRATE", "Stratiform Precipitation Rate", "kg m^-2 s^-1"},
   /* 88 */ {"CATCP", "Categorical Convective Precipitation", "numeric"},
   /* 89 */ {"UNKNOWN-0_1_89", "Unknown Parameter", "-"},
   /* 90 */ {"TKMFLX", "Total Kinematic Moisture Flux", "kg kg^-1 m s^-1"},
   /* 91 */ {"UKMFLX", "U-component (zonal) Kinematic Moisture Flux", "kg kg^-1 m s^-1"},
   /* 92 */ {"VKMFLX", "V-component (meridional) Kinematic Moisture Flux", "kg kg^-1 m s^-1"},
   /* 93 */ {"RHWATER", "Relative Humidity With Respect to Water","%"},
   /* 94 */ {"RHICE", "Relative Humidity With Respect to Ice", "%"},
   /* 95 */ {"FZPRATE", "Freezing or Frozen Precipitation Rate", "kg m^-2 s^-1"},
   /* 96 */ {"MASSDR", "Mass Density of Rain", "kg m^-3"},
   /* 97 */ {"MASSDS", "Mass Density of Snow", "kg m^-3"},
   /* 98 */ {"MASSDG", "Mass Density of Graupel", "kg m^-3"},
   /* 99 */ {"MASSDH", "Mass Density of Hail", "kg m^-3"},
   /* 100 */ {"SPNCR", "Specific Number Concentration of Rain", "kg^-1"},
   /* 101 */ {"SPNCS", "Specific Number Concentration of Snow", "kg^-1"},
   /* 102 */ {"SPNCG", "Specific Number Concentration of Graupel", "kg^-1"},
   /* 103 */ {"SPNCH", "Specific Number Concentration of Hail", "kg^-1"},
   /* 104 */ {"NUMDR", "Number Density of Rain", "m^-3"},
   /* 105 */ {"NUMDS", "Number Density of Snow", "m^-3"},
   /* 106 */ {"NUMDG", "Number Density of Graupel", "m^-3"},
   /* 107 */ {"NUMDH", " Number Density of Hail", "m^-3"},
   /* 108 */ {"SHTPRM", "Specific Humidity Tendency due to Parameterizations", "kg kg^-1 s^-1"},
   /* 109 */ {"MDLWHVA", "Mass Density of Liquid Water Coating on Hail Expressed as Mass of Liquid Water per Unit Volume of Air", "kg m^-3"},
   /* 110 */ {"SMLWHMA", "Specific Mass of Liquid Water Coating on Hail Expressed as Mass of Liquid Water per Unit Mass of Moist Air", "kg kg^-1"},
   /* 111 */ {"MMLWHDA", "Mass Mixing Ratio of Liquid Water Coating on Hail Expressed as Mass of Liquid Water per Unit Mass of Dry Air", "kg kg^-1"},
   /* 112 */ {"MDLWGVA", "Mass Density of Liquid Water Coating on Graupel Expressed as Mass of Liquid Water per Unit Volume of Air", "kg m^-3"},
   /* 113 */ {"SMLWGMA", "Specific Mass of Liquid Water Coating on Graupel Expressed as Mass of Liquid Water per Unit Mass of Moist Air", "kg kg^-1"},
   /* 114 */ {"MMLWGDA", "Mass Mixing Ratio of Liquid Water Coating on Graupel Expressed as Mass of Liquid Water per Unit Mass of Dry Air", "kg kg^-1"},
   /* 115 */ {"MDLWSVA", "Mass Density of Liquid Water Coating on Snow Expressed as Mass of Liquid Water per Unit Volume of Air", "kg m^-3"},
   /* 116 */ {"SMLWSMA", "Specific Mass of Liquid Water Coating on Snow Expressed as Mass of Liquid Water per Unit Mass of Moist Air", "kg kg^-1"},
   /* 117 */ {"MMLWSDA", "Mass Mixing Ratio of Liquid Water Coating on Snow Expressed as Mass of Liquid Water per Unit Mass of Dry Air", "kg kg^-1"},
   /* 118 */ {"UNCSH", "Unbalanced Component of Specific Humidity", "kg kg-1"},
   /* 119 */ {"UCSCLW", "Unbalanced Component of Specific Cloud Liquid Water content", "kg kg-1"},
   /* 120 */ {"UCSCIW", "Unbalanced Component of Specific Cloud Ice Water content", "kg kg-1"},
   /* 121 */ {"FSNOWC", "Fraction of Snow Cover", "Proportion"},
   /* 122 */ {"PIIDX", "Precipitation intensity index", "numeric"},
   /* 123 */ {"DPTYPE", "Dominant precipitation type", "numeric"},
   /* 124 */ {"PSHOW", "Presence of blowing snow", "numeric"},
   /* 125 */ {"PBSNOW", "Presence of blowing snow", "numeric"},
   /* 126 */ {"PBLIZZ", "Presence of blizzard", "numeric"},
   /* 127 */ {"ICEP", "Ice pellets precipitation rate", "m s^-1"},
   /* 128 */ {"TSPRATE", "Total solid precipitation rate", "kg m^-2 s^-1"},
   /* 129 */ {"EFRCWAT", "Effective Radius of Cloud Water", "m"},
   /* 130 */ {"EFRRAIN", "Effective Radius of Rain", "m"},
   /* 131 */ {"EFRCICE", "Effective Radius of Cloud Ice", "m"},
   /* 132 */ {"EFRSNOW", "Effective Radius of Snow", "m"},
   /* 133 */ {"EFRGRL",  "Effective Radius of Graupel", "m"},
   /* 134 */ {"EFRHAIL", "Effective Radius of Hail", "m"},
   /* 135 */ {"EFRSLC",  "Effective Radius of Subgrid Liquid Clouds", "m"},
   /* 136 */ {"EFRSICEC","Effective Radius of Subgrid ice Clouds", "m"},
   /* 137 */ {"EFARRAIN", "Effective Aspect Ratio of Rain", "-"},
   /* 138 */ {"EFARCICE", "Effective Aspect Ratio of Cloud Ice", "-"},
   /* 139 */ {"EFARSNOW", "Effective Aspect Ratio of Snow", "-"},
   /* 140 */ {"EFARGRL",  "Effective Aspect Ratio of Graupel", "-"},
   /* 141 */ {"EFARHAIL", "Effective Aspect Ratio of Hail", "-"},
   /* 142 */ {"EFARSIC",  "Effective Aspect Ratio of Subgrid Ice Clouds", "-"},
   /* 143 */ {"PERATE",   "Potential evaporation rate", "kg m^-2 s^-1"},
   /* 144 */ {"SRWATERC", "Specific ran water content (convective)", "kg kg^-1"},
   /* 145 */ {"SSNOWWC",  "Specific snow water content (convective)", "kg kg^-1"},
   /* 146 */ {"CICEPR",   "Cloud ice precipitation rate", "kg m^-2 s^-1"},
   /* 147 */ {"CHPRECIP", "Character of precipitation", "categorical"}, //(0, None, 1 Showers, 
             //2 Intermittent, 3 Continuous, 255 missing)
   /* 148 */ {"SNOWERAT", "Snow evaporation rate", "kg m^-2 s^-1"},
   /* 149 */ {"CWATERMR", "Cloud water mixing ratio", "kg kg^-1"},
   /* 150 */ {"CWVMF",    "Column integrated eastward water vapour mass flux", "kg m^-1 s^-1"},
};


/* GRIB2 Code table 4.2 : 0.2 */
/* Meteorological products, momentum category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoMoment[49] = {
   /* 0 */ {"WDIR", "Wind direction (from which blowing)", "deg true"},
   /* 1 */ {"WIND", "Wind speed", "m s^-1"},
   /* 2 */ {"UGRD", "u-component of wind", "m s^-1"},
   /* 3 */ {"VGRD", "v-component of wind", "m s^-1"},
   /* 4 */ {"STRM", "Stream function", "m^2 s^-1"},
   /* 5 */ {"VPOT", "Velocity potential", "m^2 s^-1"},
   /* 6 */ {"MNTSF", "Montgomery stream function", "m^2 s^-2"},
   /* 7 */ {"SGCVV", "Sigma coordinate vertical velocity", "1 s^-1"},
   /* 8 */ {"VVEL", "Vertical velocity (pressure)", "Pa s^-1"},
   /* 9 */ {"DZDT", "Vertical velocity (geometric)", "m s^-1"},
   /* 10 */ {"ABSV", "Absolute vorticity", "1 s^-1"},
   /* 11 */ {"ABSD", "Absolute divergence", "1 s^-1"},
   /* 12 */ {"RELV", "Relative vorticity", "1 s^-1"},
   /* 13 */ {"RELD", "Relative divergence", "1 s^-1"},
   /* 14 */ {"PVORT", "Potential vorticity", "K m^2 kg^-1 s^-1"},
   /* 15 */ {"VUCSH", "Vertical u-component shear", "1 s^-1"},
   /* 16 */ {"VVCSH", "Vertical v-component shear", "1 s^-1"},
   /* 17 */ {"UFLX", "Momentum flux, u component", "N m^-2"},
   /* 18 */ {"VFLX", "Momentum flux, v component", "N m^-2"},
   /* 19 */ {"WMIXE", "Wind mixing energy", "J"},
   /* 20 */ {"BLYDP", "Boundary layer dissipation", "W m^-2"},
   /* 21 */ {"MAXGUST", "Maximum wind speed", "m s^-1"},
   /* 22 */ {"GUST", "Wind speed (gust)", "m s^-1"},
   /* 23 */ {"UGUST", "u-component of wind (gust)", "m s^-1"},
   /* 24 */ {"VGUST", "v-component of wind (gust)", "m s^-1"},
   /* 25 */ {"VWSH", "Vertical speed sheer", "s^-1"},
   /* 26 */ {"MFLX", "Horizontal Momentum Flux", "N m^-2"},
   /* 27 */ {"USTM", "U-Component Storm Motion", "m s^-1"},
   /* 28 */ {"VSTM", "V-Component Storm Motion", "m s^-1"},
   /* 29 */ {"CD", "Drag Coefficient", "numeric"},
   /* 30 */ {"FRICV", "Frictional Velocity", "m s^-1"},
   /* 31 */ {"TDCMOM", "Turbulent Diffusion Coefficient for Momentum", "m^2 s^-1"},
   /* 32 */ {"ETACVV", "Eta Coordinate Vertical Velocity", "s^-1"},
   /* 33 */ {"WINDF", "Wind Fetch", "m"},
   /* 34 */ {"NWIND", "Normal Wind Component", "m s^-1"},
   /* 35 */ {"TWIND", "Tangential Wind Component", "m s^-1"},
   /* 36 */ {"AFRWE", "Amplitude Function for Rossby Wave Envelope for Meridional Wind", "m s^-1"},
   /* 37 */ {"NTSS", "Northward Turbulent Surface Stress", "N m^-2 s"},
   /* 38 */ {"ETSS", "Eastward Turbulent Surface Stress", "N m^-2 s"},
   /* 39 */ {"EWTPARM", "Eastward Wind Tendency Due to Parameterizations", "m s^-2"},
   /* 40 */ {"NWTPARM", "Northward Wind Tendency Due to Parameterizations", "m s^-2"},
   /* 41 */ {"UGWIND", "U-Component of Geostrophic Wind", "m s^-1"},
   /* 42 */ {"VGWIND", "V-Component of Geostrophic Wind", "m s^-1"},
   /* 43 */ {"GEOWD", "Geostrophic Wind Direction", "degree true"},
   /* 44 */ {"GEOWS", "Geostrophic Wind Speed", "m s^-1"},
   /* 45 */ {"UNDIV", "Unbalanced Component of Divergence", "s-1"},
   /* 46 */ {"VORTADV", "Vorticity Advection", "s-2"},
   /* 47 */ {"SFRHEAT", "Surface roughness for heat", "m"},
   /* 48 */ {"SFRMOIST", "Surface roughness for moisture", "m"},
};

/* GRIB2 Code table 4.2 : 0.3 */
/* Meteorological products, mass category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoMass[35] = {
   /* 0 */ {"PRES", "Pressure", "Pa"},
   /* 1 */ {"PRMSL", "Pressure reduced to MSL", "Pa"},
   /* 2 */ {"PTEND", "Pressure tendency", "Pa s^-1"},
   /* 3 */ {"ICAHT", "ICAO Standard Atmosphere Reference Height", "m"},
   /* 4 */ {"GP", "Geopotential", "m^2 s^-2"},
   /* 5 */ {"HGT", "Geopotential height", "m"},
   /* 6 */ {"DIST", "Geometric Height", "m"},
   /* 7 */ {"HSTDV", "Standard deviation of height", "m"},
   /* 8 */ {"PRESA", "Pressure anomaly", "Pa"},
   /* 9 */ {"GPA", "Geopotential height anomally", "m"},
   /* 10 */ {"DEN", "Density", "kg m^-3"},
   /* 11 */ {"ALTS", "Altimeter setting", "Pa"},
   /* 12 */ {"THICK", "Thickness", "m"},
   /* 13 */ {"PRESALT", "Pressure altitude", "m"},
   /* 14 */ {"DENALT", "Density altitude", "m"},
   /* 15 */ {"5WAVH", "5-Wave Geopotential Height", "m"},
   /* 16 */ {"U-GWD", "Zonal Flux of Gravity Wave Stress", "N m^-2"},
   /* 17 */ {"V-GWD", "Meridional Flux of Gravity Wave Stress", "N m^-2"},
   /* 18 */ {"HPBL", "Planetary Boundary Layer Height", "m"},
   /* 19 */ {"5WAVA", "5-Wave Geopotential Height Anomaly", "m"},
   /* 20 */ {"SDSGSO", "Standard Deviation Of Sub-Grid Scale Orography", "m"},
   /* 21 */ {"AOSGSO", "Angle Of Sub-Grid Scale Orography", "Rad"},
   /* 22 */ {"SSGSO", "Slope Of Sub-Grid Scale Orography", "numeric"},
   /* 23 */ {"GSGSO", "Gravity Of Sub-Grid Scale Orography", "W m^-2"},
   /* 24 */ {"ASGSO", "Anisotropy Of Sub-Grid Scale Orography", "numeric"},
   /* 25 */ {"NLPRES", "Natural Logarithm of Pressure in Pa", "numeric"},
   /* 26 */ {"EXPRES", "Exner Pressure", "numeric"},
   /* 27 */ {"UMFLX", "Updraught Mass Flux", "kg m^-2 s^-1"},
   /* 28 */ {"DMFLX", "Downdraught Mass Flux", "kg m^-2 s^-1"},
   /* 29 */ {"UDRATE", "Updraught Detrainment Rate", "kg m^-3 s^-1"},
   /* 30 */ {"DDRATE", "Downdraught Detrainment Rate", "kg m^-3 s^-1"},
   /* 31 */ {"UCLSPRS", "Unbalanced Component of Logarithm of Surface Pressure", "-"},
   /* 32 */ {"SWATERVP", "Saturation water vapour pressure", "Pa"},
   /* 33 */ {"GAMSL", "Geometric altitude above mean sea level", "m"},
   /* 34 */ {"GHAGRD", "Geometric height above ground level", "m"},
};

/* GRIB2 Code table 4.2 : 0.4 */
/* Meteorological products, short-wave radiation category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoShortRadiate[55] = {
   /* 0 */ {"NSWRS", "Net short-wave radiation flux (surface)", "W m^-2"},
   /* 1 */ {"NSWRT", "Net short-wave radiation flux (top of atmosphere)", "W m^-2"},
   /* 2 */ {"SWAVR", "Short wave radiation flux", "W m^-2"},
   /* 3 */ {"GRAD", "Global radiation flux", "W m^-2"},
   /* 4 */ {"BRTMP", "Brightness temperature", "K"},
   /* 5 */ {"LWRAD", "Radiance (with respect to wave number)", "W m^-1 sr^-1"},
   /* 6 */ {"SWRAD", "Radiance (with respect to wave length)", "W m^-3 sr^-1"},
   /* 7 */ {"DSWRF", "Downward Short-Wave Rad. Flux", "W m^-2"},
   /* 8 */ {"USWRF", "Upward Short-Wave Rad. Flux", "W m^-2"},
   /* 9 */ {"NSWRF", "Net Short Wave Radiation Flux", "W m^-2"},
   /* 10 */ {"PHOTAR", "Photosynthetically Active Radiation", "W m^-2"},
   /* 11 */ {"NSWRFCS", "Net Short-Wave Radiation Flux, Clear Sky", "W m^-2"},
   /* 12 */ {"DWUVR", "Downward UV Radiation", "W m^-2"},
   /* 13 */ {"UNKNOWN-0_4_13", "Unknown Parameter", "-"},
   /* 14 */ {"UNKNOWN-0_4_14", "Unknown Parameter", "-"},
   /* 15 */ {"UNKNOWN-0_4_15", "Unknown Parameter", "-"},
   /* 16 */ {"UNKNOWN-0_4_16", "Unknown Parameter", "-"},
   /* 17 */ {"UNKNOWN-0_4_17", "Unknown Parameter", "-"},
   /* 18 */ {"UNKNOWN-0_4_18", "Unknown Parameter", "-"},
   /* 19 */ {"UNKNOWN-0_4_19", "Unknown Parameter", "-"},
   /* 20 */ {"UNKNOWN-0_4_20", "Unknown Parameter", "-"},
   /* 21 */ {"UNKNOWN-0_4_21", "Unknown Parameter", "-"},
   /* 22 */ {"UNKNOWN-0_4_22", "Unknown Parameter", "-"},
   /* 23 */ {"UNKNOWN-0_4_23", "Unknown Parameter", "-"},
   /* 24 */ {"UNKNOWN-0_4_24", "Unknown Parameter", "-"},
   /* 25 */ {"UNKNOWN-0_4_25", "Unknown Parameter", "-"},
   /* 26 */ {"UNKNOWN-0_4_26", "Unknown Parameter", "-"},
   /* 27 */ {"UNKNOWN-0_4_27", "Unknown Parameter", "-"},
   /* 28 */ {"UNKNOWN-0_4_28", "Unknown Parameter", "-"},
   /* 29 */ {"UNKNOWN-0_4_29", "Unknown Parameter", "-"},
   /* 30 */ {"UNKNOWN-0_4_30", "Unknown Parameter", "-"},
   /* 31 */ {"UNKNOWN-0_4_31", "Unknown Parameter", "-"},
   /* 32 */ {"UNKNOWN-0_4_32", "Unknown Parameter", "-"},
   /* 33 */ {"UNKNOWN-0_4_33", "Unknown Parameter", "-"},
   /* 34 */ {"UNKNOWN-0_4_34", "Unknown Parameter", "-"},
   /* 35 */ {"UNKNOWN-0_4_35", "Unknown Parameter", "-"},
   /* 36 */ {"UNKNOWN-0_4_36", "Unknown Parameter", "-"},
   /* 37 */ {"UNKNOWN-0_4_37", "Unknown Parameter", "-"},
   /* 38 */ {"UNKNOWN-0_4_38", "Unknown Parameter", "-"},
   /* 39 */ {"UNKNOWN-0_4_39", "Unknown Parameter", "-"},
   /* 40 */ {"UNKNOWN-0_4_40", "Unknown Parameter", "-"},
   /* 41 */ {"UNKNOWN-0_4_41", "Unknown Parameter", "-"},
   /* 42 */ {"UNKNOWN-0_4_42", "Unknown Parameter", "-"},
   /* 43 */ {"UNKNOWN-0_4_43", "Unknown Parameter", "-"},
   /* 44 */ {"UNKNOWN-0_4_44", "Unknown Parameter", "-"},
   /* 45 */ {"UNKNOWN-0_4_45", "Unknown Parameter", "-"},
   /* 46 */ {"UNKNOWN-0_4_46", "Unknown Parameter", "-"},
   /* 47 */ {"UNKNOWN-0_4_47", "Unknown Parameter", "-"},
   /* 48 */ {"UNKNOWN-0_4_48", "Unknown Parameter", "-"},
   /* 49 */ {"UNKNOWN-0_4_49", "Unknown Parameter", "-"},
   /* 50 */ {"UVIUCS", "UV Index (Under Clear Sky)", "numeric"},
   /* 51 */ {"UVI", "UV Index", "J m^-2"},
   /* 52 */ {"DSWRFCS", "Downward Short-Wave Radiation Flux, Clear Sky", "W m^-2"},
   /* 53 */ {"USWRFCS", "Upward Short-Wave Radiation Flux, Clear Sky", "W m^-2"},
   /* 54 */ {"DSWRFLX", "Direct normal short-wave radiation flux", "W m^-2"},
};

/* GRIB2 Code table 4.2 : 0.5 */
/* Meteorological products, long-wave radiation category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoLongRadiate[9] = {
   /* 0 */ {"NLWRS", "Net long wave radiation flux (surface)", "W m^-2"},
   /* 1 */ {"NLWRT", "Net long wave radiation flux (top of atmosphere)", "W m^-2"},
   /* 2 */ {"LWAVR", "Long wave radiation flux", "W m^-2"},
   /* 3 */ {"DLWRF", "Downward Long-Wave Rad. Flux", "W m^-2"},
   /* 4 */ {"ULWRF", "Upward Long-Wave Rad. Flux", "W m^-2"},
   /* 5 */ {"NLWRF", "Net Long-Wave Radiation Flux", "W m^-2"},
   /* 6 */ {"NLWRCS", "Net Long-Wave Radiation Flux, Clear Sky", "W m^-2"},
   /* 7 */ {"BRTEMP", "Brightness Temperature", "K"},
   /* 8 */ {"DLWRFCS", "Downward Long-Wave Radiation Flux, Clear Sky", "W m^-2"},
};

/* GRIB2 Code table 4.2 : 0.6 */
/* Meteorological products, cloud category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoCloud[52] = {
   /* 0 */ {"CICE", "Cloud Ice", "kg m^-2"},
   /* 1 */ {"TCDC", "Total cloud cover", "%"},
   /* 2 */ {"CDCON", "Convective cloud cover", "%"},
   /* 3 */ {"LCDC", "Low cloud cover", "%"},
   /* 4 */ {"MCDC", "Medium cloud cover", "%"},
   /* 5 */ {"HCDC", "High cloud cover", "%"},
   /* 6 */ {"CWAT", "Cloud water", "kg m^-2"},
   /* 7 */ {"CDCA", "Cloud amount", "%"},
   /* 8 */ {"CDCT", "Cloud type", "categorical"},  //"(0 clear, 1 Cumulonimbus, 2 Stratus, "
            //"3 Stratocumulus, 4 Cumulus, 5 Altostratus, 6 Nimbostratus, "
            //"7 Altocumulus, 8 Cirrostratus, 9 Cirrocumulus, 10 Cirrus, "
            //"11 Cumulonimbus (fog), 12 Stratus (fog), 13 Stratocumulus (fog),"
            //" 14 Cumulus (fog), 15 Altostratus (fog), 16 Nimbostratus (fog), "
            //"17 Altocumulus (fog), 18 Cirrostratus (fog), "
            //"19 Cirrocumulus (fog), 20 Cirrus (fog), 191 unknown, "
            //"255 missing)"},
   /* 9 */ {"TMAXT", "Thunderstorm maximum tops", "m"},
   /* 10 */ {"THUNC", "Thunderstorm coverage", "categorical"}, //"(0 none, 1 isolated (1%-2%), "
             //"2 few (3%-15%), 3 scattered (16%-45%), 4 numerous (> 45%), "
             //"255 missing)"},
   /* 11 */ {"CDCB", "Cloud base", "m"},
   /* 12 */ {"CDCT", "Cloud top", "m"},
   /* 13 */ {"CEIL", "Ceiling", "m"},
   /* 14 */ {"CDLYR", "Non-Convective Cloud Cover", "%"},
   /* 15 */ {"CWORK", "Cloud Work Function", "J kg^-1"},
   /* 16 */ {"CUEFI", "Convective Cloud Efficiency", "proportion"},
   /* 17 */ {"TCOND", "Total Condensate", "kg kg^-1"},
   /* 18 */ {"TCOLW", "Total Column-Integrated Cloud Water", "kg m^-2"},
   /* 19 */ {"TCOLI", "Total Column-Integrated Cloud Ice", "kg m^-2"},
   /* 20 */ {"TCOLC", "Total Column-Integrated Condensate", "kg m^-2"},
   /* 21 */ {"FICE", "Ice fraction of total condensate", "proportion"},
   /* 22 */ {"CDCC", "Cloud Cover", "%"},
   /* 23 */ {"CDCIMR", "Cloud Ice Mixing Ratio", "kg kg^-1"},
   /* 24 */ {"SUNS", "Sunshine", "numeric"},
   /* 25 */ {"CBHE", "Horizontal Extent of Cumulonimbus (CB)", "%"},
   /* 26 */ {"HCONCB", "Height of Convective Cloud Base", "m"},
   /* 27 */ {"HCONCT", "Height of Convective Cloud Top", "m"},
   /* 28 */ {"NCONCD", "Number Concentration of Cloud Droplets", "kg^-1"},
   /* 29 */ {"NCCICE", "Number Concentration of Cloud Ice", "kg^-1"},
   /* 30 */ {"NDENCD", "Number Density of Cloud Droplets", "m^-3"},
   /* 31 */ {"NDCICE", "Number Density of Cloud Ice", "m^-3"},
   /* 32 */ {"FRACCC", "Fraction of Cloud Cover", "numeric"},
   /* 33 */ {"SUNSD", "Sunshine Duration", "s"},
   /* 34 */ {"SLWTC", "Surface Long Wave Effective Total Cloudiness", "numeric"},
   /* 35 */ {"SSWTC", "Surface Short Wave Effective Total Cloudiness", "numeric"},
   /* 36 */ {"FSTRPC", "Fraction of Stratiform Precipitation Cover", "proportion"},
   /* 37 */ {"FCONPC", "Fraction of Convective Precipitation Cover", "proportion"},
   /* 38 */ {"MASSDCD", "Mass Density of Cloud Droplets", "kg m^-3"},
   /* 39 */ {"MASSDCI", "Mass Density of Cloud Ice", "kg m^-3"},
   /* 40 */ {"MDCCWD", "Mass Density of Convective Cloud Water Droplets", "kg m^-3"},
   /* 41 */ {"UNKNOWN-0_6_41", "Unknown Parameter", "-"},
   /* 42 */ {"UNKNOWN-0_6_42", "Unknown Parameter", "-"},
   /* 43 */ {"UNKNOWN-0_6_43", "Unknown Parameter", "-"},
   /* 44 */ {"UNKNOWN-0_6_44", "Unknown Parameter", "-"},
   /* 45 */ {"UNKNOWN-0_6_45", "Unknown Parameter", "-"},
   /* 46 */ {"UNKNOWN-0_6_46", "Unknown Parameter", "-"},
   /* 47 */ {"VFRCWD", "Volume Fraction of Cloud Water Droplets", "Numeric"},
   /* 48 */ {"VFRCICE", "Volume Fraction of Cloud Ice Particles", "Numeric"},
   /* 49 */ {"VFRCIW", "Volume Fraction of Cloud (Ice and/or Water)", "Numeric"},
   /* 50 */ {"FOG", "Fog", "%"},
   /* 51 */ {"SUNFRAC", "Sunshine Duration Fraction", "Proportion"},
};

/* GRIB2 Code table 4.2 : 0.7 */
/* Meteorological products, Thermodynamic Stability category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoStability[21] = {
   /* 0 */ {"PLI", "Parcel lifted index (to 500 hPa)", "K"},
   /* 1 */ {"BLI", "Best lifted index (to 500 hPa)", "K"},
   /* 2 */ {"KX", "K index", "K"},
   /* 3 */ {"KOX", "KO index", "K"},
   /* 4 */ {"TOTALX", "Total totals index", "K"},
   /* 5 */ {"SX", "Sweat index", "numeric"},
   /* 6 */ {"CAPE", "Convective Available Potential Energy", "J kg^-1"},
   /* 7 */ {"CIN", "Convective Inhibition", "J kg^-1"},
   /* 8 */ {"HLCY", "Storm Relative Helicity", "J kg^-1"},
   /* 9 */ {"EHLX", "Energy helicity index", "numeric"},
   /* 10 */ {"LFTX", "Surface Lifted Index", "K"},
   /* 11 */ {"4LFTX", "Best (4 layer) Lifted Index", "K"},
   /* 12 */ {"RI", "Richardson Number", "numeric"},
   /* 13 */ {"SHWINX", "Showalter Index", "K"},
   /* 14 */ {"UNKNOWN-0_7_14", "Unknown Parameter", "-"},
   /* 15 */ {"UPHL", "Updraft Helicity", "m^2 s^-2"},
   /* 16 */ {"BLKRN", "Bulk Richardson Number", "numeric"},
   /* 17 */ {"GRDRN", "Gradient Richardson Number", "numeric"},
   /* 18 */ {"FLXRN", "Flux Richardson Number", "numeric"},
   /* 19 */ {"CONAPES", "Convective Available Potential Energy Shear", "m2 s-2"},
   /* 20 */ {"TIIDEX", "Thunderstorm intensity index", "categorical"},//(0, None, 1 Weak, 
             //2 Moderate, 3 Severe, 255 missing)
};

/* GRIB2 Code table 4.2 : 0.8 */
/* Meteorological products, Kinematic stability category */

/* GRIB2 Code table 4.2 : 0.9 */
/* Meteorological products, Temperature probabilities category */

/* GRIB2 Code table 4.2 : 0.10 */
/* Meteorological products, Moisture probabilities category */

/* GRIB2 Code table 4.2 : 0.11 */
/* Meteorological products, Momentum probabilities category */

/* GRIB2 Code table 4.2 : 0.12 */
/* Meteorological products, Mass probabilities category */

/* GRIB2 Code table 4.2 : 0.13 */
/* Meteorological products, Aerosols category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoAerosols[1] = {
   /* 0 */ {"AEROT", "Aerosol type", "categorical"}, //"(0 Aerosol not present, 1 Aerosol present, "
            //"255 missing)"}
};

/* GRIB2 Code table 4.2 : 0.14 */
/* Meteorological products, Trace Gases category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoGases[3] = {
  /* 0 */ {"TOZNE", "Total ozone", "Dobson"},
  /* 1 */ {"O3MR", "Ozone Mixing Ratio", "kg kg^-1"},
  /* 2 */ {"TCIOZ", "Total Column Integrated Ozone", "Dobson"},
};

/* GRIB2 Code table 4.2 : 0.15 */
/* Meteorological products, Radar category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoRadar[17] = {
   /* 0 */ {"BSWID", "Base spectrum width", "m s^-1"},
   /* 1 */ {"BREF", "Base reflectivity", "dB"},
   /* 2 */ {"BRVEL", "Base radial velocity", "m s^-1"},
   /* 3 */ {"VERIL", "Vertically-integrated liquid", "kg m^-2"},
   /* 4 */ {"LMAXBR", "Layer-maximum base reflectivity", "dB"},
   /* 5 */ {"PREC", "Precipitation", "kg m^-2"},
   /* 6 */ {"RDSP1", "Radar spectra (1)", "-"},
   /* 7 */ {"RDSP2", "Radar spectra (2)", "-"},
   /* 8 */ {"RDSP3", "Radar spectra (3)", "-"},
   /* 9 */ {"RFCD", "Reflectivity of Cloud Droplets", "dB"},
   /* 10 */ {"RFCI", "Reflectivity of Cloud Ice", "dB"},
   /* 11 */ {"RFSNOW", "Reflectivity of Snow", "dB"},
   /* 12 */ {"RFRAIN", "Reflectivity of Rain", "dB"},
   /* 13 */ {"RFGRPL", "Reflectivity of Graupel", "dB"},
   /* 14 */ {"RFHAIL", "Reflectivity of Hail", "dB"},
   /* 15 */ {"HSR", "Hybrid Scan Reflectivity", "dB"},
   /* 16 */ {"HSRHT", "Hybrid Scan Reflectivity Height", "m"},
};

/* GRIB2 Code table 4.2 : 0.16 */
/* Meteorological products, Forecast Radar Imagery category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoRadarForecast[6] = {
   /* 0 */ {"REFZR", "Equivalent radar reflectivity factor for rain", "m m^6 m^-3"},
   /* 1 */ {"REFZI", "Equivalent radar reflectivity factor for snow", "m m^6 m^-3"},
   /* 2 */ {"REFZC", "Equivalent radar reflectivity factor for parameterized convection", "m m^6 m^-3"},
   /* 3 */ {"RETOP", "Echo Top", "m"},
   /* 4 */ {"REFD", "Reflectivity", "dB"},
   /* 5 */ {"REFC", "Composite reflectivity", "dB"}
};

/* GRIB2 Code table 4.2 : 0.17 */
/* Meteorological products, Electrodynamics category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoElectro[6] = {
  /* 0 */ {"LTNGSD", "Lightning Strike Density", "m^-2 s^-1"},
  /* 1 */ {"LTPINX", "Lightning Potential Index", "J kg^-1"},
  /* 2 */ {"CDGDLTFD", "Cloud-to-Ground Lightning Flash Density", "km-2 day-1"},
  /* 3 */ {"CDCDLTFD", "Cloud-to-Cloud Lightning Flash Density", "km-2 day-1"},
  /* 4 */ {"TLGTFD", "Total Lightning Flash Density", "km-2 day-1"},
  /* 5 */ {"SLNGPIDX", "Subgrid-scale lightning potential index", "J kg^-1"},
};

/* GRIB2 Code table 4.2 : 0.18 */
/* Meteorological products, Nuclear/Radiology category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoNuclear[19] = {
   /* 0 */ {"ACCES", "Air concentration of Caesium 137", "Bq m^-3"},
   /* 1 */ {"ACIOD", "Air concentration of Iodine 131", "Bq m^-3"},
   /* 2 */ {"ACRADP ", "Air concentration of radioactive pollutant", "Bq m^-3"},
   /* 3 */ {"GDCES", "Ground deposition of Caesium 137", "Bq m^-2"},
   /* 4 */ {"GDIOD", "Ground deposition of Iodine 131", "Bq m^-2"},
   /* 5 */ {"GDRADP", "Ground deposition of radioactive pollutant", "Bq m^-2"},
   /* 6 */ {"TIACCP", "Time-integrated air concentration of caesium pollutant",
            "Bq s m^-3"},
   /* 7 */ {"TIACIP", "Time-integrated air concentration of iodine pollutant",
            "Bq s m^-3"},
   /* 8 */ {"TIACRP", "Time-integrated air concentration of radioactive pollutant",
            "(Bq s m^-3"}, 
   /* 9 */ {"UNKNOWN-0_18_9", "Unknown Parameter", "-"},
   /* 10 */ {"AIRCON", "Air Concentration", "Bq m^-3"},
   /* 11 */ {"WETDEP", "Wet Deposition", "Bq m^-2"},
   /* 12 */ {"DRYDEP", "Dry Deposition", "Bq m^-2"},
   /* 13 */ {"TOTLWD", "Total Deposition (Wet + Dry)", "Bq m^-2"},
   /* 14 */ {"SACON", "Specific Activity Concentration", "Bq kg^-1"},
   /* 15 */ {"MAXACON", "Maximum of Air Concentration in Layer", "Bq m^-3"},
   /* 16 */ {"HMXACON", "Height of Maximum of Air Concentration", "m"},
   /* 17 */ {"CIAIRC", "Column-Integrated Air Concentration", "Bq m-2"},
   /* 18 */ {"CAACL", "Column-Averaged Air Concentration in Layer", "Bq m-3"},
};

/* GRIB2 Code table 4.2 : 0.19 */
/* Meteorological products, Physical Atmospheric category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoAtmos[51] = {
   /* 0 */ {"VIS", "Visibility", "m"},
   /* 1 */ {"ALBDO", "Albedo", "%"},
   /* 2 */ {"TSTM", "Thunderstorm", "%"},
   /* 3 */ {"MIXHT", "Mixed layer depth", "m"},
   /* 4 */ {"VOLASH", "Volcanic ash", "catagorical"},
   /* 5 */ {"ICIT", "Icing top", "m"},
   /* 6 */ {"ICIB", "Icing base", "m"},
   /* 7 */ {"ICI", "Icing", "categorical"},
   /* 8 */ {"TURBT", "Turbulance top", "m"},
   /* 9 */ {"TURBB", "Turbulence base", "m"},
   /* 10 */ {"TURB", "Turbulance", "catagorical"},
   /* 11 */ {"TKE", "Turbulent Kinetic Energy", "J kg^-1"},
   /* 12 */ {"PBLREG", "Planetary boundary layer regime", "catagorical"},
   /* 13 */ {"CONTI", "Contrail intensity", "catagorical"},
   /* 14 */ {"CONTET", "Contrail engine type", "catagorical"},
   /* 15 */ {"CONTT", "Contrail top", "m"},
   /* 16 */ {"CONTB", "Contrail base", "m"},
   /* 17 */ {"MXSALB", "Maximum Snow Albedo", "%"},
   /* 18 */ {"SNFALB", "Snow-Free Albedo", "%"},
   /* 19 */ {"SALBD", "Snow Albedo", "%"},
   /* 20 */ {"ICIP", "Icing", "%"},
   /* 21 */ {"CTP", "In-Cloud Turbulence", "%"},
   /* 22 */ {"CAT", "Clear Air Turbulence", "%"},
   /* 23 */ {"SLDP", "Supercooled Large Droplet Probability", "%"},
   /* 24 */ {"CONTKE", "onvective Turbulent Kinetic Energy", "J kg^-1"},
   /* 25 */ {"WIWW", "Weather", "numeric"},
   /* 26 */ {"CONVO", "Convective Outlook", "numeric"},
   /* 27 */ {"ICESC", "Icing Scenario", "numeric"},
   /* 28 */ {"MWTURB", "Mountain Wave Turbulence", "m^2/3 s^-1"},
   /* 29 */ {"CATEDR", "Clear Air Turbulence", "m^2/3 s^-1"},
   /* 30 */ {"EDPARM", "Eddy Dissipation Parameter", "m^2/3 s^-1"},
   /* 31 */ {"MXEDPRM", "Maximum of Eddy Dissipation Parameter in Layer", "m^2/3 s^-1"},
   /* 32 */ {"HIFREL", "Highest Freezing Level", "m"},
   /* 33 */ {"VISLFOG", "Visibility Through Liquid Fog", "m"},
   /* 34 */ {"VISIFOG", "Visibility Through Ice Fog", "m"},
   /* 35 */ {"VISBSN", "Visibility Through Blowing Snow", "m"},
   /* 36 */ {"PSNOWS", "Presence of Snow Squalls", "categorical"}, //(0 no, 1 yes)
   /* 37 */ {"ICESEV", "Icing Severity", "categorical"}, //(0 None, 1 Trace,
       // 2 Light, 3 Moderate, 4 severe )
   /* 38 */ {"SKYIDX", "Sky transparency index", "categorical"},//(0 Worst, 1 Very Poor,
       // 2 Poor, 3 Average, 4 Good, 5 Excellent )
   /* 39 */ {"SEEINDEX", "Seeing index", "categorical"},//(0 Worst, 1 Very Poor,
       // 2 Poor, 3 Average, 4 Good, 5 Excellent )
   /* 40 */ {"SNOWLVL", "Snow level", "m"},
   /* 41 */ {"DBHEIGHT", "Duct base height", "m"},
   /* 42 */ {"TLBHEIGHT", "Trapping layer base height", "m"},
   /* 43 */ {"TLTHEIGHT", "Trapping layer top height", "m"},
   /* 44 */ {"MEANVGRTL", "Mean vertical gradient of refractivity inside trapping layer", "m^-1"},
   /* 45 */ {"MINVGRTL", "Minimum vertical gradient of refactivity inside trapping layer", "m^-1"},
   /* 46 */ {"NETRADFLUX", "Net radiation flux", "W m^-2"},
   /* 47 */ {"GLIRRTS", "Global irradiance of tilted surfaces", "W m^-2"},
   /* 48 */ {"PCONTT", "Top of persistent contrails", "m"},
   /* 49 */ {"PCONTB", "Base of persistent contrails", "m"},
   /* 50 */ {"CITEDR", "Convectively-induced Turbulence", "m^2/3 s^-1"},

};

/* GRIB2 Code table 4.2 : 0.20 */
/* Meteorological products, Atmospheric Chemical Constituents category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoChem[113] = {
   /* 0 */ {"MASSDEN", "Mass Density (Concentration)", "kg m^-3"},
   /* 1 */ {"COLMD", "Column-Integrated Mass Density", "kg m^-2"},
   /* 2 */ {"MASSMR", "Mass Mixing Ratio (Mass Fraction in Air)", "kg kg^-1"},
   /* 3 */ {"AEMFLX", "Atmosphere Emission Mass Flux", "kg m^-2 s^-1"},
   /* 4 */ {"ANPMFLX", "Atmosphere Net Production Mass Flux", "kg m^-2 s^-1"},
   /* 5 */ {"ANPEMFLX", "Atmosphere Net Production And Emision Mass Flux", "kg m^-2 s^-1"},
   /* 6 */ {"SDDMFLX", "Surface Dry Deposition Mass Flux", "kg m^-2 s^-1"},
   /* 7 */ {"SWDMFLX", "Surface Wet Deposition Mass Flux", "kg m^-2 s^-1"},
   /* 8 */ {"AREMFLX", "Atmosphere Re-Emission Mass Flux", "kg m^-2 s^-1"},
   /* 9 */ {"WLSMFLX", "Wet Deposition by Large-Scale Precipitation Mass Flux", "kg m^-2 s^-1"},
   /* 10 */ {"WDCPMFLX", "Wet Deposition by Convective Precipitation Mass Flux", "kg m^-2 s^-1"},
   /* 11 */ {"SEDMFLX", "Sedimentation Mass Flux", "kg m^-2 s^-1"},
   /* 12 */ {"DDMFLX", "Dry Deposition Mass Flux", "kg m^-2 s^-1"},
   /* 13 */ {"TRANHH", "Transfer From Hydrophobic to Hydrophilic", "kg kg^-1 s^-1"},
   /* 14 */ {"TRSDS", "Transfer From SO2 (Sulphur Dioxide) to SO4 (Sulphate)", "kg kg^-1 s^-1"},
   /* 15 */ {"DDVEL", "Dry Deposition Velocity", "m s^-1"},
   /* 16 */ {"MSSRDRYA", "Mass mixing ratio with respect to dry air", "kg kg^-1"}, 
   /* 17 */ {"MSSRWETA", "Mass mixing ratio with respect to wet air", "kg kg^-1"}, 
   /* 18 */ {"UNKNOWN-0_20_18", "Unknown Parameter", "-"},
   /* 19 */ {"UNKNOWN-0_20_19", "Unknown Parameter", "-"},
   /* 20 */ {"UNKNOWN-0_20_20", "Unknown Parameter", "-"},
   /* 21 */ {"UNKNOWN-0_20_21", "Unknown Parameter", "-"},
   /* 22 */ {"UNKNOWN-0_20_22", "Unknown Parameter", "-"},
   /* 23 */ {"UNKNOWN-0_20_23", "Unknown Parameter", "-"},
   /* 24 */ {"UNKNOWN-0_20_24", "Unknown Parameter", "-"},
   /* 25 */ {"UNKNOWN-0_20_25", "Unknown Parameter", "-"},
   /* 26 */ {"UNKNOWN-0_20_26", "Unknown Parameter", "-"},
   /* 27 */ {"UNKNOWN-0_20_27", "Unknown Parameter", "-"},
   /* 28 */ {"UNKNOWN-0_20_28", "Unknown Parameter", "-"},
   /* 29 */ {"UNKNOWN-0_20_29", "Unknown Parameter", "-"},
   /* 30 */ {"UNKNOWN-0_20_30", "Unknown Parameter", "-"},
   /* 31 */ {"UNKNOWN-0_20_31", "Unknown Parameter", "-"},
   /* 32 */ {"UNKNOWN-0_20_32", "Unknown Parameter", "-"},
   /* 33 */ {"UNKNOWN-0_20_33", "Unknown Parameter", "-"},
   /* 34 */ {"UNKNOWN-0_20_34", "Unknown Parameter", "-"},
   /* 35 */ {"UNKNOWN-0_20_35", "Unknown Parameter", "-"},
   /* 36 */ {"UNKNOWN-0_20_36", "Unknown Parameter", "-"},
   /* 37 */ {"UNKNOWN-0_20_37", "Unknown Parameter", "-"},
   /* 38 */ {"UNKNOWN-0_20_38", "Unknown Parameter", "-"},
   /* 39 */ {"UNKNOWN-0_20_39", "Unknown Parameter", "-"},
   /* 40 */ {"UNKNOWN-0_20_40", "Unknown Parameter", "-"},
   /* 41 */ {"UNKNOWN-0_20_41", "Unknown Parameter", "-"},
   /* 42 */ {"UNKNOWN-0_20_42", "Unknown Parameter", "-"},
   /* 43 */ {"UNKNOWN-0_20_43", "Unknown Parameter", "-"},
   /* 44 */ {"UNKNOWN-0_20_44", "Unknown Parameter", "-"},
   /* 45 */ {"UNKNOWN-0_20_45", "Unknown Parameter", "-"},
   /* 46 */ {"UNKNOWN-0_20_46", "Unknown Parameter", "-"},
   /* 47 */ {"UNKNOWN-0_20_47", "Unknown Parameter", "-"},
   /* 48 */ {"UNKNOWN-0_20_48", "Unknown Parameter", "-"},
   /* 49 */ {"UNKNOWN-0_20_49", "Unknown Parameter", "-"},
   /* 50 */ {"AIA", "Amount in Atmosphere", "mol"},
   /* 51 */ {"CONAIR", "Concentration In Air", "mol m^-3"},
   /* 52 */ {"VMXR", "Volume Mixing Ratio (Fraction in Air)", "mol mol^-1"},
   /* 53 */ {"CGPRC", "Chemical Gross Production Rate of Concentration", "mol m^-3 s^-1"},
   /* 54 */ {"CGDRC", "Chemical Gross Destruction Rate of Concentration", "mol m^-3 s^-1"},
   /* 55 */ {"SFLUX", "Surface Flux", "mol m^-2 s^-1"},
   /* 56 */ {"COAIA", "Changes Of Amount in Atmosphere (See Note 1)", "mol s^-1"},
   /* 57 */ {"TYABA", "Total Yearly Average Burden of The Atmosphere", "mol"},
   /* 58 */ {"TYAAL", "Total Yearly Average Atmospheric Loss (See Note 1)", "mol s^-1"},
   /* 59 */ {"ANCON", "Aerosol Number Concentration", "m^-3"},
   /* 60 */ {"ASNCON", "Aerosol Specific Number Concentration", "kg-1"},
   /* 61 */ {"MXMASSD", "Maximum of Mass Density", "kg m-3"},
   /* 62 */ {"HGTMD", "Height of Mass Density", "m"},
   /* 63 */ {"CAVEMDL", "Column-Averaged Mass Density in Layer", "kg m-3"},
   /* 64 */ {"MOLRDRYA", "Mole fraction with respect to dry air", "mol mol-1"},
   /* 65 */ {"MOLRWETA", "Mole fraction with respect to wet air", "mol mol-1"},
   /* 66 */ {"CINCLDSP", "Column-integrated in-cloud scavenging rate by precipitation", "kg m-2 s-1"},
   /* 67 */ {"CBLCLDSP", "Column-integrated below-cloud scavenging rate by precipitation", "kg m-2 s-1"},
   /* 68 */ {"CIRELREP", "Column-integrated release rate from evaporating precipitation", "kg m-2 s-1"},
   /* 69 */ {"CINCSLSP", "Column-integrated in-cloud scavenging rate by large-scale precipitation", "kg m-2 s-1"},
   /* 70 */ {"CBECSLSP", "Column-integrated below-cloud scavenging rate by large-scale precipitation", "kg m-2 s-1"},
   /* 71 */ {"CRERELSP", "Column-integrated release rate from evaporating large-scale precipitation", "kg m-2 s-1"},
   /* 72 */ {"CINCSRCP", "Column-integrated in-cloud scavenging rate by convective precipitation", "kg m-2 s-1"},
   /* 73 */ {"CBLCSRCP", "Column-integrated below-cloud scavenging rate by convective precipitation", "kg m-2 s-1"},
   /* 74 */ {"CIRERECP", "Column-integrated release rate from evaporating convective precipitation", "kg m-2 s-1"},
   /* 75 */ {"WFIREFLX", "Wildfire flux", "kg m-2 s-1"},
   /* 76 */ {"EMISFLX", "Emission Rate", "kg kg-1 s-1"},
   /* 77 */ {"SFCEFLX", "Surface Emission flux", "kg m-2 s-1"},
   /* 78 */ {"UNKNOWN-0_20_78", "Unknown Parameter", "-"},
   /* 79 */ {"UNKNOWN-0_20_79", "Unknown Parameter", "-"},
   /* 80 */ {"UNKNOWN-0_20_80", "Unknown Parameter", "-"},
   /* 81 */ {"UNKNOWN-0_20_81", "Unknown Parameter", "-"},
   /* 82 */ {"UNKNOWN-0_20_82", "Unknown Parameter", "-"},
   /* 83 */ {"UNKNOWN-0_20_83", "Unknown Parameter", "-"},
   /* 84 */ {"UNKNOWN-0_20_84", "Unknown Parameter", "-"},
   /* 85 */ {"UNKNOWN-0_20_85", "Unknown Parameter", "-"},
   /* 86 */ {"UNKNOWN-0_20_86", "Unknown Parameter", "-"},
   /* 87 */ {"UNKNOWN-0_20_87", "Unknown Parameter", "-"},
   /* 88 */ {"UNKNOWN-0_20_88", "Unknown Parameter", "-"},
   /* 89 */ {"UNKNOWN-0_20_89", "Unknown Parameter", "-"},
   /* 90 */ {"UNKNOWN-0_20_90", "Unknown Parameter", "-"},
   /* 91 */ {"UNKNOWN-0_20_91", "Unknown Parameter", "-"},
   /* 92 */ {"UNKNOWN-0_20_92", "Unknown Parameter", "-"},
   /* 93 */ {"UNKNOWN-0_20_93", "Unknown Parameter", "-"},
   /* 94 */ {"UNKNOWN-0_20_94", "Unknown Parameter", "-"},
   /* 95 */ {"UNKNOWN-0_20_95", "Unknown Parameter", "-"},
   /* 96 */ {"UNKNOWN-0_20_96", "Unknown Parameter", "-"},
   /* 97 */ {"UNKNOWN-0_20_97", "Unknown Parameter", "-"},
   /* 98 */ {"UNKNOWN-0_20_98", "Unknown Parameter", "-"},
   /* 99 */ {"UNKNOWN-0_20_99", "Unknown Parameter", "-"},
   /* 100 */ {"SADEN", "Surface Area Density (Aerosol)", "m^-1"},
   /* 101 */ {"ATMTK", "Atmosphere Optical Thickness", "m"},
   /* 102 */ {"AOTK", "Aerosol Optical Thickness", "numeric"},
   /* 103 */ {"SSALBK", "Single Scattering Albedo", "numeric"},
   /* 104 */ {"ASYSFK", "Asymmetry Factor", "numeric"},
   /* 105 */ {"AECOEF", "Aerosol Extinction Coefficient", "m^-1"},
   /* 106 */ {"AACOEF", "Aerosol Absorption Coefficient", "m^-1"},
   /* 107 */ {"ALBSAT", "Aerosol Lidar Backscatter from Satellite", "m^-1 sr-1"},
   /* 108 */ {"ALBGRD", "Aerosol Lidar Backscatter from the Ground", "m^-1 sr-1"},
   /* 109 */ {"ALESAT", "Aerosol Lidar Extinction from Satellite", "m^-1"},
   /* 110 */ {"ALEGRD", "Aerosol Lidar Extinction from the Ground", "m^-1"},
   /* 111 */ {"ANGSTEXP", "Angstrom Exponent", "numeric"},
   /* 112 */ {"SCTAOTK", "Scattering Aerosol Optical Thickness", "numeric"},
};

/* GRIB2 Code table 4.2 : 0.190 */
/* Meteorological products, ASCII category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoText[1] = {
   /* 0 */ {"TEXT", "Arbitrary text string", "CCITTIA5"}   /* No Official Abbrev */
};

/* GRIB2 Code table 4.2 : 0.191 */
/* Meteorological products, Miscellaneous category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_meteoMisc[4] = {
  /* 0 */ {"TSEC", "Seconds prior to initial reference time", "s"},
  /* 1 */ {"GEOLAT", "Geographical Latitude", "degrees"},
  /* 2 */ {"GEOLON", "Geographical Longitude", "degrees"},
  /* 3 */ {"DSLOBS", "Days Since Last Observation", "days"},
};

/* GRIB2 Code table 4.2 : 1.0 */
/* Hydrological products, Basic Hydrology category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_hydroBasic[17] = {
   /* 0 */ {"FFLDG", "Flash flood guidance", "kg m^-2"},
   /* 1 */ {"FFLDRO", "Flash flood runoff", "kg m^-2"},
   /* 2 */ {"RSSC", "Remotely sensed snow cover", "categorical"}, //"(50 no-snow/no-cloud, "
            //"100 Clouds, 250 Snow, 255 missing)"},
   /* 3 */ {"ESCT", "Elevation of snow covered terrain", "categorical"}, //"(0-90 elevation in "
            //"increments of 100m, 254 clouds, 255 missing)"},
   /* 4 */ {"SWEPON", "Snow water equivalent percent of normal", "%"},
   /* 5 */ {"BGRUN", "Baseflow-Groundwater Runoff", "kg m^-2"},
   /* 6 */ {"SSRUN", "Storm Surface Runoff", "kg m^-2"},
   /* 7 */ {"DISRS", "Discharge from Rivers or Streams", "m^3 s^-1"},
   /* 8 */ {"GWUPS", "Group Water Upper Storage", "kg m^-2"},
   /* 9 */ {"GWLOWS", "Group Water Lower Storage", "kg m^-2"},
   /* 10 */ {"SFLORC", "Side Flow into River Channel", "m^3 s^-1 m^-1"},
   /* 11 */ {"RVERSW", "River Storage of Water", "m^3"},
   /* 12 */ {"FLDPSW", "Flood Plain Storage of Water", "m^3"},
   /* 13 */ {"DEPWSS", "Depth of Water on Soil Surface", "kg m^-2"},
   /* 14 */ {"UPAPCP", "Upstream Accumulated Precipitation", "kg m^-2"},
   /* 15 */ {"UPASM", "Upstream Accumulated Snow Melt", "kg m^-2"},
   /* 16 */ {"PERRATE", "Percolation Rate", "kg m^-2 s^-1"},
};

/* GRIB2 Code table 4.2 : 1.1 */
/* Hydrological products, Hydrology Inland Water and Sediment Properties category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_hydroProb[3] = {
   /* 0 */ {"CPPOP", "Conditional percent precipitation amount fractile for an overall period", "kg m^-2"},
   /* 1 */ {"PPOSP", "Percent precipitation in a sub-period of an overall period", "%"},
   /* 2 */ {"POP", "Probability of 0.01 inch of precipitation (POP)", "%"},
};

/* GRIB2 Code table 4.2 : 1.2 */
/* Hydrological products, Hydrology Probabilities category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_hydroWaterSediment[15] = {
   /* 0 */ {"WDPTHIL", "Water Depth", "m"},
   /* 1 */ {"WTMPIL", "Water Temperature", "K"},
   /* 2 */ {"WFRACT", "Water Fraction", "proportion"},
   /* 3 */ {"SEDTK", "Sediment Thickness", "m"},
   /* 4 */ {"SEDTMP", "Sediment Temperature", "K"},
   /* 5 */ {"ICTKIL", "Ice Thickness", "m"},
   /* 6 */ {"ICETIL", "Ice Temperature", "K"},
   /* 7 */ {"ICECIL", "Ice Cover", "proportion"},
   /* 8 */ {"LANDIL", "Land Cover (0=water, 1=land)", "proportion"},
   /* 9 */ {"SFSAL", "Shape Factor with Respect to Salinity Profile", "-"},
   /* 10 */ {"SFTMP", "Shape Factor with Respect to Temperature Profile in Thermocline", "-"},
   /* 11 */ {"ACWSR", "Attenuation Coefficient of Water with Respect to Solar Radiation", "m^-1"},
   /* 12 */ {"SALTIL", "Salinity", "kg kg^-1"},
   /* 13 */ {"CSAFC", "Cross Sectional Area of Flow in Channel", "m^2"},
   /* 14 */ {"LNDSNOWT", "Snow temperature", "K"},
};

/* GRIB2 Code table 4.2 : 2.0 */
/* Land Surface products, Vegetation/Biomass category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_landVeg[62] = {
   /* 0 */ {"LAND", "Land cover (0=sea, 1=land)", "proportion"},
   /* 1 */ {"SFCR", "Surface roughness", "m"},
   /* 2 */ {"TSOIL", "Soil temperature", "K"},
   /* 3 */ {"SOILM", "Soil moisture content", "kg m^-2"},
   /* 4 */ {"VEG", "Vegetation", "%"},
   /* 5 */ {"WATR", "Water runoff", "kg m^-2"},
   /* 6 */ {"EVAPT", "Evapotranspiration", "1 kg^-2 s^-1"},
   /* 7 */ {"MTERH", "Model terrain height", "m"},
   /* 8 */ {"LANDU", "Land use", "categorical"},  //"(1 Urban land, 2 agriculture, 3 Range Land, "
            //"4 Deciduous forest, 5 Coniferous forest, 6 Forest/wetland, "
            //"7 Water, 8 Wetlands, 9 Desert, 10 Tundra, 11 Ice, "
            //"12 Tropical forest, 13 Savannah)"},
   /* 9 */ {"SOILW", "Volumetric Soil Moisture Content", "proportion"},
   /* 10 */ {"GFLUX", "Ground Heat Flux", "W m^-2"},
   /* 11 */ {"MSTAV", "Moisture Availability", "%"},
   /* 12 */ {"SFEXC", "Exchange Coefficient", "kg m^-2 s^-1"},
   /* 13 */ {"CNWAT", "Plant Canopy Surface Water", "kg m^-2"},
   /* 14 */ {"BMIXL", "Blackadar's Mixing Length Scale", "m"},
   /* 15 */ {"CCOND", "Canopy Conductance", "m s^-1"},
   /* 16 */ {"RSMIN", "Minimal Stomatal Resistance", "s m^-1"},
   /* 17 */ {"WILT", "Wilting Point", "proportion"},
   /* 18 */ {"RCS", "Solar parameter in canopy conductance", "proportion"},
   /* 19 */ {"RCT", "Temperature parameter in canopy conductance", "proportion"},
   /* 20 */ {"RCSOL", "Soil moisture parameter in canopy conductance", "proportion"},
   /* 21 */ {"RCQ", "Humidity parameter in canopy conductance", "proportion"},
   /* 22 */ {"SOILM", "Soil Moisture", "kg m^-3"},
   /* 23 */ {"CISOILW", "Column-Integrated Soil Water", "kg m^-2"},
   /* 24 */ {"HFLUX", "Heat Flux", "W m^-2"},
   /* 25 */ {"VSOILM", "Volumetric Soil Moisture", "m^-3 m^-3"},
   /* 26 */ {"WILT", "Wilting Point", "kg m^-3"},
   /* 27 */ {"VWILTM", "Volumetric Wilting Moisture", "m^-3 m^-3"},
   /* 28 */ {"LEAINX", "Leaf Area Index", "numeric"},
   /* 29 */ {"EVGFC", "Evergreen Forest Cover", "proportion"},
   /* 30 */ {"DECFC", "Deciduous Forest Cover", "proportion"},
   /* 31 */ {"NDVINX", "Normalized Differential Vegetation Index (NDVI)", "numeric"},
   /* 32 */ {"RDVEG", "Root Depth of Vegetation", "m"},
   /* 33 */ {"WROD", "Water Runoff and Drainage", "kg m^-2"},
   /* 34 */ {"SFCWRO", "Surface Water Runoff", "kg m^-2"},
   /* 35 */ {"TCLASS", "Tile Class", "numeric"},
   /* 36 */ {"TFRCT", "Tile Fraction", "proportion"},
   /* 37 */ {"TPERCT", "Tile Percentage", "%"},
   /* 38 */ {"SOILVIC", "Soil Volumetric Ice Content", "m3 m^-3"},
   /* 39 */ {"EVAPTRAT", "Evapotranspiration Rate", "kg m-2 s-1"},
   /* 40 */ {"PERATE", "Potential Evapotranspiration Rate", "kg m-2 s-1"},
   /* 41 */ {"SMRATE", "Snow Melt Rate", "kg m-2 s-1"},
   /* 42 */ {"WRDRATE", "Water Runoff and Drainage Rate", "kg m-2 s-1"},
   /* 43 */ {"DRAINDIR", "Drainage direction", "categorical"}, // (0 Reserved, 1 South-West, 
      // 2 South, 3 South-East, 4 West, 5 No-Direction, 6 East, 7 North-West, 8 North,
      // 9 North-East, 255 missing)
   /* 44 */ {"UPSAREA", "Upstream Area", "m^2"},
   /* 45 */ {"WETCOV", "Wetland Cover", "Proportion"},
   /* 46 */ {"WETTYPE", "Wetland Type", "categorical"}, // (0 Reserved, 1 Bog, 2 Drained,
      // 3 Ren, 4 Floodplain, 5 Mangrove, 6 Mash, 7 Rice, 8 Riverine, 9 Salt Marsh, 
      // 10 Swamp, 11 Upland, 12 Wet tundra, 255 missing)
   /* 47 */ {"IRRCOV", "Irrigation Cover", "Proportion"},
   /* 48 */ {"CROPCOV", "C4 Crop Cover", "Proportion"},
   /* 49 */ {"GRASSCOV", "C4 Grass Cover", "Proportion"},
   /* 50 */ {"SKINRC", "Skin Resovoir Content", "kg m^-2"},
   /* 51 */ {"SURFRATE", "Surface Runoff Rate", "kg m-2 s-1"},
   /* 52 */ {"SUBSRATE", "Subsurface Runoff Rate", "kg m-2 s-1"},
   /* 53 */ {"LOVEGCOV", "Low-Vegetation Cover", "Proportion"},
   /* 54 */ {"HIVEGCOV", "High-Vegetation Cover", "Proportion"},
   /* 55 */ {"LAILO", "Leaf Area Index (Low-Vegetation)", "m^2 m^-2"},
   /* 56 */ {"LAIHI", "Leaf Area Index (High-Vegetation)", "m^2 m^-2"},
   /* 57 */ {"TYPLOVEG", "Type of Low-Vegetation", "Categorical"},
   /* 58 */ {"TYPHIVEG", "Type of High-Vegetation", "Categorical"},
   /* 59 */ {"NECOFLUX", "Net Ecosystem Exchange Flux", "kg^-2 s^-1"},
   /* 60 */ {"GROSSFLUX", "Gross Primary Production Flux", "kg^-2 s^-1"},
   /* 61 */ {"ECORFLUX", "Ecosystem Respiration Flux", "kg^-2 s^-1"},
};

/* GRIB2 Code table 4.2 : 2.1 */
/* Land Surface products, Agri/aquacultural special  category */

/* GRIB2 Code table 4.2 : 2.2 */
/* Land Surface products, Transportation related category */

/* GRIB2 Code table 4.2 : 2.3 */
/* Land Surface products, Soil category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_landSoil[30] = {
   /* 0 */ {"SOTYP", "Soil type", "categorical"}, //"(1 Sand, 2 Loamy sand, 3 Sandy loam, "
            //"4 Silt loam, 5 Organic (redefined), 6 Sandy clay loam, "
            //"7 Silt clay loam, 8 Clay loam, 9 Sandy clay, 10 Silty clay, "
            //"11 Clay)"},
   /* 1 */ {"UPLST", "Upper layer soil temperature", "K"},
   /* 2 */ {"UPLSM", "Upper layer soil moisture", "kg m^-3"},
   /* 3 */ {"LOWLSM", "Lower layer soil moisture", "kg m^-3"},
   /* 4 */ {"BOTLST", "Bottom layer soil temperature", "K"},
   /* 5 */ {"SOILL", "Liquid Volumetric Soil Moisture (non-frozen)", "proportion"},
   /* 6 */ {"RLYRS", "Number of Soil Layers in Root Zone", "numeric"},
   /* 7 */ {"SMREF", "Transpiration Stress-onset (soil moisture)", "proportion"},
   /* 8 */ {"SMDRY", "Direct Evaporation Cease (soil moisture)", "proportion"},
   /* 9 */ {"POROS", "Soil Porosity", "proportion"},
   /* 10 */ {"LIQVSM", "Liquid Volumetric Soil Moisture (Non-Frozen)", "m^3 m^-3"},
   /* 11 */ {"VOLTSO", "Volumetric Transpiration Stree-Onset(Soil Moisture)", "m^3 m^-3"},
   /* 12 */ {"TRANSO", "Transpiration Stree-Onset(Soil Moisture)", "kg m^-3"},
   /* 13 */ {"VOLDEC", "Volumetric Direct Evaporation Cease(Soil Moisture)", "m^3 m^-3"},
   /* 14 */ {"DIREC", "Direct Evaporation Cease(Soil Moisture)", "kg m^-3"},
   /* 15 */ {"SOILP", "Soil Porosity", "m^3 m^-3"},
   /* 16 */ {"VSOSM", "Volumetric Saturation Of Soil Moisture", "m^3 m^-3"},
   /* 17 */ {"SATOSM", "Saturation Of Soil Moisture", "m^3 m^-3"},
   /* 18 */ {"SOILTMP", "Soil Temperature", "K"},
   /* 19 */ {"SOILMOI", "Soil Moisture", "kg m^-3"},
   /* 20 */ {"CISOILM", "Column-Integrated Soil Moisture", "kg m^-2"},
   /* 21 */ {"SOILICE", "Soil Ice", "kg m^-3"},
   /* 22 */ {"CISICE", "Column-Integrated Soil Ice", "kg m^-2"},
   /* 23 */ {"LWSNOWP", "Liquid Water in Snow Pack", "kg m^-2"},
   /* 24 */ {"FRSTINX", "Frost Index", "kg day^-1"},
   /* 25 */ {"SNWDEB", "Snow Depth at Elevation Bands", "kg m^-2"},
   /* 26 */ {"SHFLX", "Soil Heat Flux", "W m^-2"},
   /* 27 */ {"SOILDEP", "Soil Depth", "m"},
   /* 28 */ {"SNOWTMP", "Snow Temperature", "K"},
   /* 29 */ {"ICETEMP", "Ice Temperature", "K"},
};

/* GRIB2 Code table 4.2 : 2.4 */
/* Land Surface products, Fire Weather category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_landFire[27] = {
  /* 0 */ {"FIREOLK", "Fire Outlook", "numeric"},
  /* 1 */ {"FIREODT", "Fire Outlook Due to Dry Thunderstorm", "numeric"},
  /* 2 */ {"HINDEX", "Haines Index", "numeric"},
  /* 3 */ {"FBAREA", "Fire Burned Area", "%"},
  /* 4 */ {"FOSINDX", "Fosberg Index", "numeric"},
  /* 5 */ {"FWINX", "Fire Weath Index", "numeric"},
  /* 6 */ {"FFMCODE", "Fine Fuel Moisture Code", "numeric"},
  /* 7 */ {"DUFMCODE", "Duff Moisture Code", "numeric"},
  /* 8 */ {"DRTCODE", "Drought Code", "numeric"},
  /* 9 */ {"INFSINX", "Initial Fire Spread Index", "numeric"},
  /* 10 */ {"FBUPINX" "Fire Build Up Index", "numeric"},
  /* 11 */ {"FDSRTE", "Fire Daily Severity Rating", "numeric"},
  /* 12 */ {"KRIDX", "Keetch-Byram Drought Index", "numeric"},
  /* 13 */ {"DRFACT", "Drought Factor (as defined by the Australian forest service)", "numeric"},
  /* 14 */ {"RATESPRD", "Rate of Spread (as defined by the Australian forest service)", "m s^-1"},
  /* 15 */ {"FIREDIDX", "Fire Danger index (as defined by the Australian forest service)", "numeric"},
  /* 16 */ {"SPRDCOMP", "Spread component (as defined by the USFS Fire Danger Rating System)", "numeric"},
  /* 17 */ {"BURNIDX", "Burning Index (as defined by the Australian forest service)", "numeric"},
  /* 18 */ {"IGNCOMP", "Ignition Component (as defined by the Australian forest service)", "%"},
  /* 19 */ {"ENRELCOM", "Energy Release Component (as defined by the Australian forest service)", "J m^-2"},
  /* 20 */ {"BURNAREA", "Burning Area", "%"},
  /* 21 */ {"BURNABAREA", "Burnable Area", "%"},
  /* 22 */ {"UNBURNAREA", "Unburnable Area", "%"},
  /* 23 */ {"FUELLOAD", "Fuel Load", "kg m^-2"},
  /* 24 */ {"COMBCO", "Combustion Completness", "%"},
  /* 25 */ {"FUELMC", "Fuel Moisture Content", "kg kg^-1"},
  /* 26 */ {"WFIREPOT", "Wildfire Potential (as defined by NOAA Global Systems Lab)", "numeric"},
};

/* GRIB2 Code table 4.2 : 2.5 */
/* Land Surface products, Glaciers and Inland Ice category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_landIce[2] = {
  /* 0 */ {"GLACCOV", "Glacier Cover", "Proportion"},
  /* 1 */ {"GLACTMP", "Glacier Temperature", "K"},
};

/* GRIB2 Code table 4.2 : 3.0 */
/* Satellite Remote Sensing products, Image Format category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_satelliteImage[10] = {
   /* 0 */ {"SRAD", "Scaled radiance", "numeric"},
   /* 1 */ {"SALBDO", "Scaled albedo", "numeric"},
   /* 2 */ {"SBRTMP", "Scaled brightness temperature", "numeric"},
   /* 3 */ {"SPWAT", "Scaled precipitable water", "numeric"},
   /* 4 */ {"SLIFTX", "Scaled lifted index", "numeric"},
   /* 5 */ {"SCTOPP", "Scaled cloud top pressure", "numeric"},
   /* 6 */ {"SKINTMP", "Scaled skin temperature", "numeric"},
   /* 7 */ {"CLOUDMASK", "Cloud mask", "categorical"}, //"(0 clear over water, 1 clear over land, "
            //"2 cloud, 3 No data)"},
   /* 8 */ {"PIXST", "Pixel scene type", "index"},
   /* 9 */ {"FIREDI", "Fire Detection Indicator", "index"},
};

/* GRIB2 Code table 4.2 : 3.1 */
/* Satellite Remote Sensing products, Quantitative category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_satelliteQuantitative[100] = {
   /* 0 */ {"ESTP", "Estimated precipitation", "kg m^-2"},
   /* 1 */ {"IRRATE", "Instantaneous Rain Rate", "kg m^-2 s^-1"},
   /* 2 */ {"CTOPH", "Cloud Top Height", "m"},
   /* 3 */ {"CTOPHQI", "Cloud Top Height Quality Indicator", "index"},
   /* 4 */ {"ESTUGRD", "Estimated u-Component of Wind", "m^-1"},
   /* 5 */ {"ESTVGRD", "Estimated v-Component of Wind", "m^-1"},
   /* 6 */ {"NPIXU", "Number Of Pixels Used", "numeric"},
   /* 7 */ {"SOLZA", "Solar Zenith Angle", "degrees"},
   /* 8 */ {"RAZA", "Relative Azimuth Angle", "degrees"},
   /* 9 */ {"RFL06", "Reflectance in 0.6 Micron Channel", "%"},
   /* 10 */ {"RFL08", "Reflectance in 0.8 Micron Channel", "%"},
   /* 11 */ {"RFL16", "Reflectance in 1.6 Micron Channel", "%"},
   /* 12 */ {"RFL39", "Reflectance in 3.9 Micron Channel", "%"},
   /* 13 */ {"ATMDIV", "Atmospheric Divergence", "s^-1"},
   /* 14 */ {"CBTMP", "Cloudy Brightness Temperature", "K"},
   /* 15 */ {"CSBTMP", "Clear Sky Brightness Temperature", "K"},
   /* 16 */ {"CLDRAD", "Cloudy Radiance (with respect to wave number)", "W m^-1 sr^-1"},
   /* 17 */ {"CSKYRAD", "Clear Sky Radiance (with respect to wave number)", "W m^-1 sr^-1"},
   /* 18 */ {"UNKNOWN-3_1_18", "Unknown Parameter", "-"},
   /* 19 */ {"WINDS", "Wind Speed", "m s^-1"},
   /* 20 */ {"AOT06", "Aerosol Optical Thickness at 0.635 µm", "-"},
   /* 21 */ {"AOT08", "Aerosol Optical Thickness at 0.810 µm", "-"},
   /* 22 */ {"AOT16", "Aerosol Optical Thickness at 1.640 µm", "-"},
   /* 23 */ {"ANGCOE", "Angstrom Coefficient", "-"},
   /* 24 */ {"UNKNOWN-3_1_24", "Unknown Parameter", "-"},
   /* 25 */ {"UNKNOWN-3_1_25", "Unknown Parameter", "-"},
   /* 26 */ {"UNKNOWN-3_1_26", "Unknown Parameter", "-"},
   /* 27 */ {"BRFLF", "Bidirectional Reflecance Factor", "numeric"},
   /* 28 */ {"SPBRT", "Brightness Temperture", "K"},
   /* 29 */ {"SRAD", "Scaled Radiance", "numeric"},
   /* 30 */ {"RFL04", "Reflectance in 0.4 Micron Channel", "%"},
   /* 31 */ {"UNKNOWN-3_1_31", "Unknown Parameter", "-"},
   /* 32 */ {"UNKNOWN-3_1_32", "Unknown Parameter", "-"},
   /* 33 */ {"UNKNOWN-3_1_33", "Unknown Parameter", "-"},
   /* 34 */ {"UNKNOWN-3_1_34", "Unknown Parameter", "-"},
   /* 35 */ {"UNKNOWN-3_1_35", "Unknown Parameter", "-"},
   /* 36 */ {"UNKNOWN-3_1_36", "Unknown Parameter", "-"},
   /* 37 */ {"UNKNOWN-3_1_37", "Unknown Parameter", "-"},
   /* 38 */ {"UNKNOWN-3_1_38", "Unknown Parameter", "-"},
   /* 39 */ {"UNKNOWN-3_1_39", "Unknown Parameter", "-"},
   /* 40 */ {"UNKNOWN-3_1_40", "Unknown Parameter", "-"},
   /* 41 */ {"UNKNOWN-3_1_41", "Unknown Parameter", "-"},
   /* 42 */ {"UNKNOWN-3_1_42", "Unknown Parameter", "-"},
   /* 43 */ {"UNKNOWN-3_1_43", "Unknown Parameter", "-"},
   /* 44 */ {"UNKNOWN-3_1_44", "Unknown Parameter", "-"},
   /* 45 */ {"UNKNOWN-3_1_45", "Unknown Parameter", "-"},
   /* 46 */ {"UNKNOWN-3_1_46", "Unknown Parameter", "-"},
   /* 47 */ {"UNKNOWN-3_1_47", "Unknown Parameter", "-"},
   /* 48 */ {"UNKNOWN-3_1_48", "Unknown Parameter", "-"},
   /* 49 */ {"UNKNOWN-3_1_49", "Unknown Parameter", "-"},
   /* 50 */ {"UNKNOWN-3_1_50", "Unknown Parameter", "-"},
   /* 51 */ {"UNKNOWN-3_1_51", "Unknown Parameter", "-"},
   /* 52 */ {"UNKNOWN-3_1_52", "Unknown Parameter", "-"},
   /* 53 */ {"UNKNOWN-3_1_53", "Unknown Parameter", "-"},
   /* 54 */ {"UNKNOWN-3_1_54", "Unknown Parameter", "-"},
   /* 55 */ {"UNKNOWN-3_1_55", "Unknown Parameter", "-"},
   /* 56 */ {"UNKNOWN-3_1_56", "Unknown Parameter", "-"},
   /* 57 */ {"UNKNOWN-3_1_57", "Unknown Parameter", "-"},
   /* 58 */ {"UNKNOWN-3_1_58", "Unknown Parameter", "-"},
   /* 59 */ {"UNKNOWN-3_1_59", "Unknown Parameter", "-"},
   /* 60 */ {"UNKNOWN-3_1_60", "Unknown Parameter", "-"},
   /* 61 */ {"UNKNOWN-3_1_61", "Unknown Parameter", "-"},
   /* 62 */ {"UNKNOWN-3_1_62", "Unknown Parameter", "-"},
   /* 63 */ {"UNKNOWN-3_1_63", "Unknown Parameter", "-"},
   /* 64 */ {"UNKNOWN-3_1_64", "Unknown Parameter", "-"},
   /* 65 */ {"UNKNOWN-3_1_65", "Unknown Parameter", "-"},
   /* 66 */ {"UNKNOWN-3_1_66", "Unknown Parameter", "-"},
   /* 67 */ {"UNKNOWN-3_1_67", "Unknown Parameter", "-"},
   /* 68 */ {"UNKNOWN-3_1_68", "Unknown Parameter", "-"},
   /* 69 */ {"UNKNOWN-3_1_69", "Unknown Parameter", "-"},
   /* 70 */ {"UNKNOWN-3_1_70", "Unknown Parameter", "-"},
   /* 71 */ {"UNKNOWN-3_1_71", "Unknown Parameter", "-"},
   /* 72 */ {"UNKNOWN-3_1_72", "Unknown Parameter", "-"},
   /* 73 */ {"UNKNOWN-3_1_73", "Unknown Parameter", "-"},
   /* 74 */ {"UNKNOWN-3_1_74", "Unknown Parameter", "-"},
   /* 75 */ {"UNKNOWN-3_1_75", "Unknown Parameter", "-"},
   /* 76 */ {"UNKNOWN-3_1_76", "Unknown Parameter", "-"},
   /* 77 */ {"UNKNOWN-3_1_77", "Unknown Parameter", "-"},
   /* 78 */ {"UNKNOWN-3_1_78", "Unknown Parameter", "-"},
   /* 79 */ {"UNKNOWN-3_1_79", "Unknown Parameter", "-"},
   /* 80 */ {"UNKNOWN-3_1_80", "Unknown Parameter", "-"},
   /* 81 */ {"UNKNOWN-3_1_81", "Unknown Parameter", "-"},
   /* 82 */ {"UNKNOWN-3_1_82", "Unknown Parameter", "-"},
   /* 83 */ {"UNKNOWN-3_1_83", "Unknown Parameter", "-"},
   /* 84 */ {"UNKNOWN-3_1_84", "Unknown Parameter", "-"},
   /* 85 */ {"UNKNOWN-3_1_85", "Unknown Parameter", "-"},
   /* 86 */ {"UNKNOWN-3_1_86", "Unknown Parameter", "-"},
   /* 87 */ {"UNKNOWN-3_1_87", "Unknown Parameter", "-"},
   /* 88 */ {"UNKNOWN-3_1_88", "Unknown Parameter", "-"},
   /* 89 */ {"UNKNOWN-3_1_89", "Unknown Parameter", "-"},
   /* 90 */ {"UNKNOWN-3_1_90", "Unknown Parameter", "-"},
   /* 91 */ {"UNKNOWN-3_1_91", "Unknown Parameter", "-"},
   /* 92 */ {"UNKNOWN-3_1_92", "Unknown Parameter", "-"},
   /* 93 */ {"UNKNOWN-3_1_93", "Unknown Parameter", "-"},
   /* 94 */ {"UNKNOWN-3_1_94", "Unknown Parameter", "-"},
   /* 95 */ {"UNKNOWN-3_1_95", "Unknown Parameter", "-"},
   /* 96 */ {"UNKNOWN-3_1_96", "Unknown Parameter", "-"},
   /* 97 */ {"UNKNOWN-3_1_97", "Unknown Parameter", "-"},
   /* 98 */ {"CCMPEMRR", "Correlation coefficient between MPE rain rates and microwave", "numeric"},
   /* 99 */ {"SDMPEMRR", "Standard deviation between MPE rain rates and microwave", "numeric"},
};

/* GRIB2 Code table 4.2 : 3.2 */
/* Satellite Remote Sensing products, Cloud Properties category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_satelliteCloud[31] = {
   /* 0 */ {"CSKPROB", "Clear Sky Probability", "%"},
   /* 1 */ {"CTOPTMP", "Cloud Top Temperature", "K"},
   /* 2 */ {"CTOPPRES", "Cloud Top Pressure", "Pa"},
   /* 3 */ {"CLDTYPE", "Cloud Type", "categorical"},
   /* 4 */ {"CLDPHAS", "Cloud Phase", "categorical"},
   /* 5 */ {"CLDODEP", "Cloud Optical Depth", "numeric"},
   /* 6 */ {"CLDPER", "Cloud Partical Effective Radius", "m"},
   /* 7 */ {"CLDLWP", "Cloud Liquid Water Path", "kg m-2"},
   /* 8 */ {"CLDIWP", "Cloud Ice Water Path", "kg m -2"},
   /* 9 */ {"CLDALB", "Cloud Albedo", "numeric"},
   /* 10 */ {"CLDEMISS", "Cloud Emissivity", "numeric"},
   /* 11 */ {"EAODR", "Effective Absorption Optical Depth Ratio", "numeric"},
   /* 12 */ {"UNKNOWN-3_2_12", "Unknown Parameter", "-"},
   /* 13 */ {"UNKNOWN-3_2_13", "Unknown Parameter", "-"},
   /* 14 */ {"UNKNOWN-3_2_14", "Unknown Parameter", "-"},
   /* 15 */ {"UNKNOWN-3_2_15", "Unknown Parameter", "-"},
   /* 16 */ {"UNKNOWN-3_2_16", "Unknown Parameter", "-"},
   /* 17 */ {"UNKNOWN-3_2_17", "Unknown Parameter", "-"},
   /* 18 */ {"UNKNOWN-3_2_18", "Unknown Parameter", "-"},
   /* 19 */ {"UNKNOWN-3_2_19", "Unknown Parameter", "-"},
   /* 20 */ {"UNKNOWN-3_2_20", "Unknown Parameter", "-"},
   /* 21 */ {"UNKNOWN-3_2_21", "Unknown Parameter", "-"},
   /* 22 */ {"UNKNOWN-3_2_22", "Unknown Parameter", "-"},
   /* 23 */ {"UNKNOWN-3_2_23", "Unknown Parameter", "-"},
   /* 24 */ {"UNKNOWN-3_2_24", "Unknown Parameter", "-"},
   /* 25 */ {"UNKNOWN-3_2_25", "Unknown Parameter", "-"},
   /* 26 */ {"UNKNOWN-3_2_26", "Unknown Parameter", "-"},
   /* 27 */ {"UNKNOWN-3_2_27", "Unknown Parameter", "-"},
   /* 28 */ {"UNKNOWN-3_2_28", "Unknown Parameter", "-"},
   /* 29 */ {"UNKNOWN-3_2_29", "Unknown Parameter", "-"},
   /* 30 */ {"MEACST", "Measurement Cost", "numeric"},
};

/* GRIB2 Code table 4.2 : 3.3 */
/* Satellite Remote Sensing products, Flight Rules Conditions category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_satelliteFlight[3] = {
   /* 0 */ {"PBMVFRC", "Probability of Encountering Marginal Visual Flight Rules Conditions", "%"},
   /* 1 */ {"PBLIFRC", "Probability of Encountering Low Instrument Flight Rules Conditions", "%"},
   /* 2 */ {"PBINFRC", "Probability of Encountering Instrument Flight Rules Conditions", "%"},
};

/* GRIB2 Code table 4.2 : 3.4 */
/* Satellite Remote Sensing products, Volcanic Ash category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_satelliteAsh[9] = {
   /* 0 */ {"VOLAPROB", "Volcanic Ash Probability", "%"},
   /* 1 */ {"VOLACDTT", "Volcanic Ash Cloud Top Temperature", "K"},
   /* 2 */ {"VOLACDTP", "Volcanic Ash Cloud Top Pressure", "Pa"},
   /* 3 */ {"VOLACDTH", "Volcanic Ash Cloud Top Height", "m"},
   /* 4 */ {"VOLACDEM", "Volcanic Ash Cloud Emissity", "numeric"},
   /* 5 */ {"VOLAEADR", "Volcanic Ash Effective Absorption Depth Ratio", "numeric"},
   /* 6 */ {"VOLACDOD", "Volcanic Ash Cloud Optical Depth", "numeric"},
   /* 7 */ {"VOLACDEN", "Volcanic Ash Column Density", "kg m-2"},
   /* 8 */ {"VOLAPER",  "Volcanic Ash Particle Effective Radius", "m"},
};

/* GRIB2 Code table 4.2 : 3.5 */
/* Satellite Remote Sensing products, Sea-Surface Temperature category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_satelliteSeaTemp[6] = {
   /* 0 */ {"ISSTMP", "Interface Sea-Surface Temperature", "K"},
   /* 1 */ {"SKSSTMP", "Skin Sea-Surface Temperature", "K"},
   /* 2 */ {"SSKSSTMP", "Sub-Skin Sea-Surface Temperature", "K"},
   /* 3 */ {"FDNSSTMP", "Foundation Sea-Surface Temperature", "K"},
   /* 4 */ {"EBSSTSTD", "Estimated bias between Sea-Surface Temperature and Standard", "K"},
   /* 5 */ {"EBSDSSTS", "Estimated bias Standard Deviation between Sea-Surface Temperature and Standard", "K"},
};

/* GRIB2 Code table 4.2 : 3.6 */
/* Satellite Remote Sensing products, Solar Radiation category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_satelliteSolar[6] = {
   /* 0 */ {"GSOLIRR", "Global Solar Irradiance", "W m-2"},
   /* 1 */ {"GSOLEXP", "Global Solar Exposure", "J m-2"},
   /* 2 */ {"DIRSOLIR", "Direct Solar Irradiance", "W m-2"},
   /* 3 */ {"DIRSOLEX", "Direct Solar Exposure", "J m-2"},
   /* 4 */ {"DIFSOLIR", "Diffuse Solar Irradiance", "W m-2"},
   /* 5 */ {"DIFSOLEX", "Diffuse Solar Exposure", "J m-2"},
};

/* GRIB2 Code table 4.2 : 4.0 */
/* Space Weather products, Temperature category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceTemp[6] = {
   /* 0 */ {"TMPSWP", "Temperature", "K"},
   /* 1 */ {"ELECTMP", "Electron Temperature", "K"},
   /* 2 */ {"PROTTMP", "Proton Temperature", "K"},
   /* 3 */ {"IONTMP", "Ion Temperature", "K"},
   /* 4 */ {"PRATMP", "Parallel Temperature", "K"},
   /* 5 */ {"PRPTMP", "Perpendicular Temperature", "K"},
};

/* GRIB2 Code table 4.2 : 4.1 */
/* Space Weather products, Momentum category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceMomentum[4] = {
   /* 0 */ {"SPEED", "Velocity Magnitude (Speed)", "m s-1"},
   /* 1 */ {"VEL1", "1st Vector Component of Velocity", "m s-1"},
   /* 2 */ {"VEL2", "2nd Vector Component of Velocity", "m s-1"},
   /* 3 */ {"VEL3", "3rd Vector Component of Velocity", "m s-1"},
};

/* GRIB2 Code table 4.2 : 4.2 */
/* Space Weather products, Charged Particle Mass and Number category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceChargedParticle[14] = {
   /* 0 */ {"PLSMDEN", "Particle Number Density", "m-3"},
   /* 1 */ {"ELCDEN", "Electron Density", "m-3"},
   /* 2 */ {"PROTDEN", "Proton Density", "m-3"},
   /* 3 */ {"IONDEN", "Ion Density", "m-3"},
   /* 4 */ {"VTEC", "Vertical Electron Content", "m-2"},
   /* 5 */ {"ABSFRQ", "HF Absorption Frequency", "Hz"},
   /* 6 */ {"ABSRB", "HF Absorption", "dB"},
   /* 7 */ {"SPRDF", "Spread F", "m"},
   /* 8 */ {"HPRIMF", "hF", "m"},
   /* 9 */ {"CRTFRQ", "Critical Frequency", "Hz"},
   /* 10 */ {"MAXUFZ", "Maximal Usable Frequency (MUF)", "Hz"},
   /* 11 */ {"PEAKH", "Peak Height (hm)", "m"},
   /* 12 */ {"PEAKDEN", "Peak Density", "m^-3"},
   /* 13 */ {"EQSLABT", "Equivalent Slab Thickness (tau)", "km"},
};

/* GRIB2 Code table 4.2 : 4.3 */
/* Space Weather products, Electric and Magnetic Fields category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceFields[8] = {
   /* 0 */ {"BTOT", "Magnetic Field Magnitude", "T"},
   /* 1 */ {"BVEC1", "1st Vector Component of Magnetic Field", "T"},
   /* 2 */ {"BVEC2", "2nd Vector Component of Magnetic Field", "T"},
   /* 3 */ {"BVEC3", "3rd Vector Component of Magnetic Field", "T"},
   /* 4 */ {"ETOT", "Electric Field Magnitude", "V m-1"},
   /* 5 */ {"EVEC1", "1st Vector Component of Electric Field", "V m-1"},
   /* 6 */ {"EVEC2", "2nd Vector Component of Electric Field", "V m-1"},
   /* 7 */ {"EVEC3", "3rd Vector Component of Electric Field", "V m-1"},
};

/* GRIB2 Code table 4.2 : 4.4 */
/* Space Weather products, Energetic Particles category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceEnergeticParticle[7] = {
   /* 0 */ {"DIFPFLUX", "Proton Flux (Differential)", "(m2 s sr eV)-1"},
   /* 1 */ {"INTPFLUX", "Proton Flux (Integral)", "(m2 s sr)-1"},
   /* 2 */ {"DIFEFLUX", "Electron Flux (Differential)", "(m2 s sr eV)-1"},
   /* 3 */ {"INTEFLUX", "Electron Flux (Integral)", "(m2 s sr)-1"},
   /* 4 */ {"DIFIFLUX", "Heavy Ion Flux (Differential)", "(m2 s sr eV / nuc)-1"},
   /* 5 */ {"INTIFLUX", "Heavy Ion Flux (Integral)", "(m2 s sr)-1"},
   /* 6 */ {"NTRNFLUX", "Cosmic Ray Neutron Flux", "h-1"},
};

/* GRIB2 Code table 4.2 : 4.5 */
/* Space Weather products, Waves category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceWaves[4] = {
   /* 0 */ {"AMPL", "Amplitude", "rad"},
   /* 1 */ {"PHASE", "Phase", "rad"},
   /* 2 */ {"FREQ", "Frequency", "Hz"},
   /* 3 */ {"WAVELGTH", "Wavelength", "m"},
};

/* GRIB2 Code table 4.2 : 4.6 */
/* Space Weather products, Solar Electromagnetic Emissions category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceSolarEmissions[7] = {
   /* 0 */ {"TSI", "Integrated Solar Irradiance", "W m-2"},
   /* 1 */ {"XLONG", "Solar X-ray Flux (XRS Long)", "W m-2"},
   /* 2 */ {"XSHRT", "Solar X-ray Flux (XRS Short)", "W m-2"},
   /* 3 */ {"EUVIRR", "Solar EUV Irradiance", "W m-2"},
   /* 4 */ {"SPECIRR", "Solar Spectral Irradiance", "W m-2 nm-1"},
   /* 5 */ {"F107", "F10.7", "W m-2 Hz-1"},
   /* 6 */ {"SOLRF", "Solar Radio Emissions", "W m-2 Hz-1"},
};

/* GRIB2 Code table 4.2 : 4.7 */
/* Space Weather products, Terrestial Electromagnetic Emissions category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceTerrEmissions[4] = {
   /* 0 */ {"LMBINT", "Limb Intensity", "m-2 s-1"},
   /* 1 */ {"DSKINT", "Disk Intensity", "m-2 s-1"},
   /* 2 */ {"DSKDAY", "Disk Intensity Day", "m-2 s-1"},
   /* 3 */ {"DSKNGT", "Disk Intensity Night", "m-2 s-1"},
};

/* GRIB2 Code table 4.2 : 4.8 */
/* Space Weather products, Imagery category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceImagery[9] = {
   /* 0 */ {"XRAYRAD", "X-Ray Radiance", "W sr-1 m-2"},
   /* 1 */ {"EUVRAD", "EUV Radiance", "W sr-1 m-2"},
   /* 2 */ {"HARAD", "H-Alpha Radiance", "W sr-1 m-2"},
   /* 3 */ {"WHTRAD", "White Light Radiance", "W sr-1 m-2"},
   /* 4 */ {"CAIIRAD", "CAII-K Radiance", "W sr-1 m-2"},
   /* 5 */ {"WHTCOR", "White Light Coronagraph Radiance", "W sr-1 m-2"},
   /* 6 */ {"HELCOR", "Heliospheric Radiance", "W sr-1 m-2"},
   /* 7 */ {"MASK", "Thematic Mask", "numeric"},
   /* 8 */ {"SICFL", "Solar Induced Chlorophyll Flourscence", ""},
};

/* GRIB2 Code table 4.2 : 4.9 */
/* Space Weather products, Ion-Neutral Coupling category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceCoupling[3] = {
   /* 0 */ {"SIGPED", "Pedersen Conductivity", "S m-1"},
   /* 1 */ {"SIGHAL", "Hall Conductivity", "S m-1"},
   /* 2 */ {"SIGPAR", "Parallel Conductivity", "S m-1"},
};

/* GRIB2 Code table 4.2 : 4.10 */
/* Space Weather products, Indices category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_spaceIndices[8] = {
   /* 0 */ {"SCINIDX", "Scintillation Index (sigma phi)", "rad"},
   /* 1 */ {"SCIDEXS4", "Scintillation Index S4", "Numeric"},
   /* 2 */ {"ROTIDX", "Rate of Change of TEC Index (ROTI)", "TECU/min"},
   /* 3 */ {"DIDXSG", "Disturbance of Ionosphere Index Spatial Gradient (DIXSG)", "Numeric"},
   /* 4 */ {"AATRATE", "Along Arc TEC Rate( AATR)", "TECU/min"},
   /* 5 */ {"KP", "Kp", "numeric"},
   /* 6 */ {"EDISSTIX", "Equatorial Disturbance Storm Time Index (Dst)", "nT"},
   /* 7 */ {"AURELEC", "Auroral Electroject (AE)", "nT"},
};

/* GRIB2 Code table 4.2 : 10.0 */
/* Oceanographic products, Waves category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_oceanWaves[93] = {
   /* 0 */ {"WVSP1", "Wave spectra (1)", "-"},
   /* 1 */ {"WVSP2", "Wave spectra (2)", "-"},
   /* 2 */ {"WVSP3", "Wave spectra (3)", "-"},
   /* 3 */ {"HTSGW", "Significant height of combined wind waves and swell", "m"},
   /* 4 */ {"WVDIR", "Direction of wind waves", "degree true"},
   /* 5 */ {"WVHGT", "Significant height of wind waves", "m"},
   /* 6 */ {"WVPER", "Mean period of wind waves", "s"},
   /* 7 */ {"SWDIR", "Direction of swell waves", "degree true"},
   /* 8 */ {"SWELL", "Significant height of swell waves", "m"},
   /* 9 */ {"SWPER", "Mean period of swell waves", "s"},
   /* 10 */ {"DIRPW", "Primary wave direction", "degree true"},
   /* 11 */ {"PERPW", "Primary wave mean period", "s"},
   /* 12 */ {"DIRSW", "Secondary wave direction", "degree true"},
   /* 13 */ {"PERSW", "Secondary wave mean period", "s"},
   /* 14 */ {"WWSDIR", "Direction of Combined Wind Waves and Swell", "degree true"},
   /* 15 */ {"MWSPER", "Mean Period of Combined Wind Waves and Swell", "s"},
   /* 16 */ {"CDWW", "Coefficient of Drag With Waves", "-"},
   /* 17 */ {"FRICV", "Friction Velocity", "m s^-1"},
   /* 18 */ {"WSTR", "Wave Stress", "N m^-2"},
   /* 19 */ {"NWSTR", "Normalised Waves Stress", "-"},
   /* 20 */ {"MSSW", "Mean Square Slope of Waves", "-"},
   /* 21 */ {"USSD", "U-component Surface Stokes Drift", "m s^-1"},
   /* 22 */ {"VSSD", "V-component Surface Stokes Drift", "m s^-1"},
   /* 23 */ {"PMAXWH", "Period of Maximum Individual Wave Height", "s"},
   /* 24 */ {"MAXWH", "Maximum Individual Wave Height", "m"},
   /* 25 */ {"IMWF", "Inverse Mean Wave Frequency", "s"},
   /* 26 */ {"IMFWW", "Inverse Mean Frequency of The Wind Waves", "s"},
   /* 27 */ {"IMFTSW", "Inverse Mean Frequency of The Total Swell", "s"},
   /* 28 */ {"MZWPER", "Mean Zero-Crossing Wave Period", "s"},
   /* 29 */ {"MZPWW", "Mean Zero-Crossing Period of The Wind Waves", "s"},
   /* 30 */ {"MZPTSW", "Mean Zero-Crossing Period of The Total Swell", "s"},
   /* 31 */ {"WDIRW", "Wave Directional Width", "-"},
   /* 32 */ {"DIRWWW", "Directional Width of The Wind Waves", "-"},
   /* 33 */ {"DIRWTS", "Directional Width of The Total Swell", "-"},
   /* 34 */ {"PWPER", "Peak Wave Period", "s"},
   /* 35 */ {"PPERWW", "Peak Period of The Wind Waves", "s"},
   /* 36 */ {"PPERTS", "Peak Period of The Total Swell", "s"},
   /* 37 */ {"ALTWH", "Altimeter Wave Height", "m"},
   /* 38 */ {"ALCWH", "Altimeter Corrected Wave Height", "m"},
   /* 39 */ {"ALRRC", "Altimeter Range Relative Correction", "-"},
   /* 40 */ {"MNWSOW", "10 Metre Neutral Wind Speed Over Waves", "m s^-1"},
   /* 41 */ {"MWDIRW", "10 Metre Wind Direction Over Waves", "degree true"},
   /* 42 */ {"WESP", "Wave Engery Spectrum", "m^-2 s rad^-1"},
   /* 43 */ {"KSSEW", "Kurtosis of The Sea Surface Elevation Due to Waves", "-"},
   /* 44 */ {"BENINX", "Benjamin-Feir Index", "-"},
   /* 45 */ {"SPFTR", "Spectral Peakedness Factor", "s^-1"},
   /* 46 */ {"PWAVEDIR", "Peak wave direction", "o"},
   /* 47 */ {"SWHFSWEL", "Significant wave height of first swell partition", "m"},
   /* 48 */ {"SWHSSWEL", "Significant wave height of second swell partition", "m"},
   /* 49 */ {"SWHTSWEL", "Significant wave height of third swell partition", "m"},
   /* 50 */ {"MWPFSWEL", "Mean wave period of first swell partition", "s"},
   /* 51 */ {"MWPSSWEL", "Mean wave period of second swell partition", "s"},
   /* 52 */ {"MWPTSWEL", "Mean wave period of third swell partition", "s"},
   /* 53 */ {"MWDFSWEL", "Mean wave direction of first swell partition", "o"},
   /* 54 */ {"MWDSSWEL", "Mean wave direction of second swell partition", "o"},
   /* 55 */ {"MWDTSWEL", "Mean wave direction of third swell partition", "o"},
   /* 56 */ {"WDWFSWEL", "Mean directional width of first swell partition", "-"},
   /* 57 */ {"WDWSSWEL", "Mean directional width of second swell partition", "-"},
   /* 58 */ {"WDWTSWEL", "Mean directional width of third swell partition", "-"},
   /* 59 */ {"MFWFSWEL", "Wave Frequency width of first swell partition", "-"},
   /* 60 */ {"MFWSSWEL", "Wave Frequency width of second swell partition", "-"},
   /* 61 */ {"MFWTSWEL", "Wave Frequency width of third swell partition", "-"},
   /* 62 */ {"WAVEFREW", "Wave frequency width", "-"},
   /* 63 */ {"FREWWW", "Frequency width of wind waves", "-"},
   /* 64 */ {"FREWTSW", "Frequency width of total swell", "-"},
   /* 65 */ {"PWPFSPAR", "Peak Wave Period of First Swell Parition", "s"},
   /* 66 */ {"PWPSSPAR", "Peak Wave Period of Second Swell Parition", "s"},
   /* 67 */ {"PWPTSPAR", "Peak Wave Period of Third Swell Parition", "s"},
   /* 68 */ {"PWDFSPAR", "Peak Wave Direction of First Swell Partition", "degree true"},
   /* 69 */ {"PWDSSPAR", "Peak Wave Direction of Second Swell Partition", "degree true"},
   /* 70 */ {"PWDTSPAR", "Peak Wave Direction of Third Swell Partition", "degree true"},
   /* 71 */ {"PDWWAVE", "Peak Direction of Wind Waves", "degree true"},
   /* 72 */ {"PDTSWELL", "Peak Direction of Total Swell", "degree true"},
   /* 73 */ {"WCAPFRAC", "Whitecap Fraction", "fraction"},
   /* 74 */ {"MDTSWEL", "Mean Direction of Total Swell", "degree"},
   /* 75 */ {"MDWWAVE", "Mean Direction of Wind Waves", "degree"},
   /* 76 */ {"CHNCK", "Charnock", "numeric"},
   /* 77 */ {"WAVESPSK", "Wave Spectral Skewness", "numeric"},
   /* 78 */ {"WAVEFMAG", "Wave Energy Flux Magnitude", "numeric"},
   /* 79 */ {"WAVEFDIR", "Wave Energy Flux Mean Direction", "numeric"},
   /* 80 */ {"RWAVEAFW", "Raio of Wave Angular and Frequency width", "numeric"},
   /* 81 */ {"FCVOCEAN", "Free Convective Velocity over the Oceans", "m s^-1"},
   /* 82 */ {"AIRDENOC", "Air Density over the Oceans", "kg m^-3"},
   /* 83 */ {"NEFW", "Normalized Energy Flux into Waves", "numeric"},
   /* 84 */ {"NSOCEAN", "Normalized Stress into Ocean", "numeric"},
   /* 85 */ {"NEFOCEAN", "Normalized Energy Flux into Ocean", "numeric"},
   /* 86 */ {"SEWAVE", "Surface Elevation Variance due to Waves", "m^2 s rad^-1"},
   /* 87 */ {"WAVEMSLC", "Wave Induced Mean Se Level Correction", "m"},
   /* 88 */ {"SPECWI", "Spectral Width Index", "numeric"},
   /* 89 */ {"EFWS", "Number of Events in Freak Wave Statistics", "numeric"},
   /* 90 */ {"USMFO", "U-Component of Surface Momentum Flux into Ocean", "N m^-2"},
   /* 91 */ {"VSMFO", "V-Component of Surface Momentum Flux into Ocean", "N m^-2"},
   /* 92 */ {"WAVETEFO", "Wave Turbulent Energy Flux into Ocean", "W m^-2"},
};

/* GRIB2 Code table 4.2 : 10.1 */
/* Oceanographic products, Currents category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_oceanCurrents[7] = {
   /* 0 */ {"DIRC", "Current direction", "degree true"},
   /* 1 */ {"SPC", "Current speed", "m s^-1"},
   /* 2 */ {"UOGRD", "u-component of current", "m s^-1"},
   /* 3 */ {"VOGRD", "v-component of current", "m s^-1"},
   /* 4 */ {"RIPCOP", "Rip Current Occurrence Probability", "%"},
   /* 5 */ {"EASTCUR", "Eastward Current", "m s^-1"},
   /* 6 */ {"NRTHCUR", "Northward Current", "m s^-1"},
};

/* GRIB2 Code table 4.2 : 10.2 */
/* Oceanographic products, Ice category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_oceanIce[24] = {
   /* 0 */ {"ICEC", "Ice cover", "proportion"},
   /* 1 */ {"ICETK", "Ice thinkness", "m"},
   /* 2 */ {"DICED", "Direction of ice drift", "degree true"},
   /* 3 */ {"SICED", "Speed of ice drift", "m s^-1"},
   /* 4 */ {"UICE", "u-component of ice drift", "m s^-1"},
   /* 5 */ {"VICE", "v-component of ice drift", "m s^-1"},
   /* 6 */ {"ICEG", "Ice growth rate", "m s^-1"},
   /* 7 */ {"ICED", "Ice divergence", "s^-1"},
   /* 8 */ { "ICETMP", "ce Temperatur", "K"},
   /* 9 */ { "ICEPRS", "Ice Internal Pressure", "Pa m"},
   /* 10 */ { "ZVCICEP", "Zonal Vector Component of Vertically Integrated Ice Internal Pressure", "Pa m"},
   /* 11 */ { "MVCICEP", "Meridional Vector Component of Vertically Integrated Ice Internal Pressure", "Pa m"},
   /* 12 */ { "CICES", "Compressive Ice Strength", "N m^-1"},
   /* 13 */ { "SNOWTSI", "Snow Temperature (over sea ice)", "K"},
   /* 14 */ { "ALBDOIC", "Albeco", "numeric"},
   /* 15 */ { "SICEVOL", "Sea Ice Volume per Unit Area", "m^3 m^-2"},
   /* 16 */ { "SNVOLSI", "Snow Volume Over Sea Ice per Unit Area", "m^3 m^-2"},
   /* 17 */ { "SICEHC", "Sea Ice Heat Content", "J m^-2"},
   /* 18 */ { "SNCEHC", "Snow over Sea Ice Heat Content", "J m^-2"},
   /* 19 */ { "ICEFTHCK", "Ice Freeboard Thickness", "m"},
   /* 20 */ { "ICEMPF", "Ice Melt Pond Fraction", "fraction"},
   /* 21 */ { "ICEMPD", "Ice Melt Pond Depth", "m"},
   /* 22 */ { "ICEMPV", "Ice Melt Pond Volume per Unit Area", "m^3 m^-2"},
   /* 23 */ { "SIFTP", "Sea Ice Fraction Tendency due to Parameterization", "s^-1"},
};

/* GRIB2 Code table 4.2 : 10.3 */
/* Oceanographic products, Surface Properties category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_oceanSurface[21] = {
   /* 0 */ {"WTMP", "Water temperature", "K"},
   /* 1 */ {"DSLM", "Deviation of sea level from mean", "m"},
   /* 2 */ {"CH","Heat Exchange Coefficient", "-"},
   /* 3 */ {"PRACTSAL","Practical salinity", "numeric"},
   /* 4 */ {"DWHFLUX","Downward Heat Flux", "W m^-2"},
   /* 5 */ {"EASTWSS","Eastward Surface Stress", "N m^-2"},
   /* 6 */ {"NORTHWSS","Northward surface stress", "N m^-2"},
   /* 7 */ {"XCOMPSS","x-component Surface Stress", "N m^-2"},
   /* 8 */ {"YCOMPSS","y-component Surface Stress", "N m^-2"},
   /* 9 */ {"THERCSSH","Thremosteric Change in Sea Surface Height", "m"},
   /* 10 */ {"HALOCSSH","Halosteric Change in Sea Surface Height", "m"},
   /* 11 */ {"STERCSSH","Steric Change in Sea Surface Height", "m"},
   /* 12 */ {"SEASFLUX","Sea Salt Flux", "kg m^-2 s^-1"},
   /* 13 */ {"NETUPWFLUX","Net upward water flux", "kg m^-2 s^-1"},
   /* 14 */ {"ESURFWVEL","Eastward surface water velocity", "m s^-1"},
   /* 15 */ {"NSURFWVEL","Northward surface water velocity", "m s^-1"},
   /* 16 */ {"XSURFWVEL","x-component of surface water velocity", "m s^-1"},
   /* 17 */ {"YSURFWVEL","y-component of surface water velocity", "m s^-1"},
   /* 18 */ {"HFLUXCOR","Heat flux correction", "W m^-2"},
   /* 19 */ {"SSHGTPARM","Sea surface height tendency due to parameterization", "m s^-1"},
   /* 20 */ {"DSLIBARCOR","Deviation of sea level mean with inverse barometer correction", "m"},
};

/* GRIB2 Code table 4.2 : 10.4 */
/* Oceanographic products, Sub-Surface Properties category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_oceanSubSurface[42] = {
   /* 0 */ {"MTHD", "Main thermocline depth", "m"},
   /* 1 */ {"MTHA", "Main thermocline anomaly", "m"},
   /* 2 */ {"TTHDP", "Transient thermocline depth", "m"},
   /* 3 */ {"SALTY", "Salinity", "kg kg^-1"},
   /* 4 */ {"OVHD", "Ocean Vertical Heat Diffusivity", "m^2 s^-1"},
   /* 5 */ {"OVSD", "Ocean Vertical Salt Diffusivity", "m^2 s^-1"},
   /* 6 */ {"OVMD", "Ocean Vertical Momentum Diffusivity", "m^2 s^-1"},
   /* 7 */ {"BATHY", "athymetry", "m"},
   /* 8 */ {"UNKNOWN-10_4_8", "Unknown Parameter", "-"},
   /* 9 */ {"UNKNOWN-10_4_9", "Unknown Parameter", "-"},
   /* 10 */ {"UNKNOWN-10_4_10", "Unknown Parameter", "-"},
   /* 11 */ {"SFSALP", "Shape Factor With Respect To Salinity Profile", "-"},
   /* 12 */ {"SFTMPP", "Shape Factor With Respect To Temperature Profile In Thermocline", "-"},
   /* 13 */ {"ACWSRD", "Attenuation Coefficient Of Water With Respect to Solar Radiation", "m^-1"},
   /* 14 */ {"WDEPTH", "Water Depth", "m"},
   /* 15 */ {"WTMPSS", "Water Temperature", "K"},
   /* 16 */ {"WATERDEN", "Water Density (rho)", "kg m-3"},
   /* 17 */ {"WATDENA", "Water Density Anomaly (sigma)", "kg m-3"},
   /* 18 */ {"WATPTEMP", "Water potential temperature (theta)", "K"},
   /* 19 */ {"WATPDEN", "Water potential density (rho theta)", "kg m-3"},
   /* 20 */ {"WATPDENA", "Water potential density anomaly (sigma theta)", "kg m-3"},
   /* 21 */ {"PRTSAL", "Practical salinity", "psu"},
   /* 22 */ {"WCHEATC", "Water Column-integrated Heat Content", "J m^-2"},
   /* 23 */ {"EASTWVEL", "Eastward Water Velocity", "m s^-1"},
   /* 24 */ {"NRTHWVEL", "Northward Water Velocity", "m s^-1"},
   /* 25 */ {"XCOMPWV", "X-Component Water Velocity", "m s^-1"},
   /* 26 */ {"YCOMPWV", "Y-Component Water Velocity", "m s^-1"},
   /* 27 */ {"UPWWVEL", "Upward Water Velocity", "m s^-1"},
   /* 28 */ {"VEDDYDIF", "Vertical Eddy Diffusivity", "m^2 s^-1"},
   /* 29 */ {"BPEH", "Bottom Pressure Equivalent Height", "m"},
   /* 30 */ {"FWFSW", "Fresh Water Flux into Sea Water from Rivers", "kg m^-2 s^-1"},
   /* 31 */ {"FWFC", "Fresh Water Flux Correction", "kg m^-2 s^-1"},
   /* 32 */ {"VSFSW", "Virtual Salt Flux into Sea Water", "g kg m^-2 s^-1"},
   /* 33 */ {"VSFC", "Virtual Salt Flux Correction", "g kg m^-2 s^-1"},
   /* 34 */ {"SWTTNR", "Sea Water Temperature Tendency due to Newtonian Relaxation", "K s^-1"},
   /* 35 */ {"SWSTNR", "Sea Water Salinity Tendency due to Newtonian Relaxation", "g kg m^-2 s^-1"},
   /* 36 */ {"SWTTP", "Sea Water Temperature Tendency due to Parameterization", "K s^-1"},
   /* 37 */ {"SWSTP", "Sea Water Salinity Tendency due to Parameterization", "g kg m^-2 s^-1"},
   /* 38 */ {"ESWVP", "Eastward Sea Water Velocity Due to Parameterization", "m^-2 s^-1"},
   /* 39 */ {"NSWVP", "Northward Sea Water Velocity Due to Parameterization", "m^-2 s^-1"},
   /* 40 */ {"SWTTBC", "Sea Water Temperature Tendency Due to Direct Bias Correction", "K s^-1"},
   /* 41 */ {"SWSTBC", "Sea Water Salinity due to Direct Bias Correction", "g kg m^-2 s^-1"},
};

/* GRIB2 Code table 4.2 : 10.191 */
/* Meteorological products, Miscellaneous category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_oceanMisc[5] = {
  /* 0 */ {"TSEC", "Seconds prior to initial reference time", "s"},
  /* 1 */ {"MOSF", "Meridional Overturning Stream Function", "m^3 s^-1"},
  /* 2 */ {"UNKNOWN-10_191_2", "Unknown Parameter", "-"},
  /* 3 */ {"DSLOBSO", "Days Since Last Observation", "days"},
  /* 4 */ {"BARDSF", "Barotropic Stream Function", "m^3 s^-1"},
};

/* GRIB2 Code table 4.2 : 20.0 */
/* Health and Socioeconomic impacts, Health Indicators category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_healthIndicators[9] = {
  /* 0 */ {"UTHCIDX", "Universal Thermal Climate Index", "K"},
  /* 1 */ {"MEANRTMP", "Mean Radiant Temperature", "K"},
  /* 2 */ {"WTBGTMP", "Wet-bulb Globe Temperature", "K"},
  /* 3 */ {"GLOBETMP", "Globe Temperature", "K"},
  /* 4 */ {"HUMIDX", "Humidex", "K"},
  /* 5 */ {"EFFTEMP", "Effective Temperature", "K"},
  /* 6 */ {"NOREFTMP", "Normal Effective Temperature", "K"},
  /* 7 */ {"STDEFTMP", "Standard Effective Temperature", "K"},
  /* 8 */ {"PEQUTMP", "Physiological Equivalent Temperature", "K"},
};

/* GRIB2 Code table 4.2 : 20.1 */
/* Health and Socioeconomic impacts, Epidemiology category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_healthEpidemiology[10] = {
  /* 0 */ {"MALACASE", "Malaria Cases", "fraction"},
  /* 1 */ {"MACPRATE", "Malaria Circumporozoite Protein Rate", "fraction"},
  /* 2 */ {"PFEIRATE", "Plasmodium Falciparum Entomological Inoculation Rate", "Bites per day per person"},
  /* 3 */ {"HBRATEAV", "Human Bite Rate by Anopheles Vectors", "Bites per day per person"},
  /* 4 */ {"MALAIMM", "Malaria Immunity", "fraction"},
  /* 5 */ {"FALPRATE", "Falciparum Parasite Rates", "fraction"},
  /* 6 */ {"DFPRATIO", "Detectable Falciparum Parasite Ratio (after day 10)", "fraction"},
  /* 7 */ {"AVHRATIO", "Anopheles Vector to Host Ratio", "fraction"},
  /* 8 */ {"AVECTNUM", "Anopheles Vector Number", "number m^-2"},
  /* 9 */ {"FMALVRH", "Fraction of Malarial Vector Reproductive Habitat", "fraction"},
};

/* GRIB2 Code table 4.2 : 20.2 */
/* Health and Socioeconomic impacts, Population Density category */
const ProdDefTemp::_GRIB2ParmTable ProdDefTemp::_healthPopDensity[1] = {
  /* 0 */ {"POPDEN", "Population Density", "Person m^-2"},
};

/* NCAR/RAL Local Use Table, Center 60, SubCenter 0? */
/* 
   This is a list of known NCAR/RAL created products that are
   being created and sent via grib2 to other institutions.
   Only create a new variable here if the variable cannot be
   found in any of the standard tables above.
*/
const ProdDefTemp::_GRIB2LocalTable ProdDefTemp::_NCAR_RALlocalTable[] = {
  {0, 19, 205, "FLGHT", "Flight Category", "none"},
  {0, 19, 206, "CICEL", "Confidence - Ceiling", "-"},
  {0, 19, 207, "CIVIS", "Confidence - Visibility", "-"},
  {0, 19, 208, "CIFLT", "Confidence - Flight Category", "-"},
  {0, 19, 217, "SLDICE", "Supercooled Large Droplet Icing", "categorical"},
  {0, 19, 219, "TPFI", "Turbulence Potential Forecast Index", "-"},
  {0, 19, 233, "ICPRB", "Icing probability", "-"},
  {0, 19, 234, "ICSEV", "Icing severity", "-"},
};


/* NCEP Local Use Table, Center 7, SubCenter 0 */
/* 
   This list is from NCEPs listing of their local use variables
   at http://www.nco.ncep.noaa.gov/pmb/docs/grib2/grib2_table4-1.shtml
*/
const ProdDefTemp::_GRIB2LocalTable ProdDefTemp::_NCEPlocalTable[] = {

  /* 4.2 : 0.0 Meteorological products, temperature category */
  {0, 0, 192, "SNOHF", "Snow Phase Change Heat Flux", "W m^-2"},    /* also 4.2 : 0.0 : 16  */
  {0, 0, 193, "TTRAD", "Temperature tendency by all radiation", "K s^-1"},
  {0, 0, 194, "REV",   "Relative Error Variance", "-"},
  {0, 0, 195, "LRGHR", "Large Scale Condensate Heat rate", "K s^-1"},
  {0, 0, 196, "CNVHR", "Deep Convective Heat rate", "K s^-1"},
  {0, 0, 197, "THFLX", "Total Downward Heat Flux at Surface", "W m^-2"},
  {0, 0, 198, "TTDIA", "Temperature Tendency By All Physics", "K s^-1"},
  {0, 0, 199, "TTPHY", "Temperature Tendency By Non-radiation Physics", "K s^-1"},
  {0, 0, 200, "TSD1D", "Standard Dev. of IR Temp. over 1x1 deg. area", "K"},
  {0, 0, 201, "SHAHR", "Shallow Convective Heating rate", "K s^-1"},
  {0, 0, 202, "VDFHR", "Vertical Diffusion Heating rate", "K s^-1"},
  {0, 0, 203, "THZ0", "Potential temperature at top of viscous sublayer", "K"},
  {0, 0, 204, "TCHP", "Tropical Cyclone Heat Potential", "J m^-2 K^-1"},
  {0, 0, 205, "ELMELT", "Effective Layer (EL) Temperature", "C"},
  {0, 0, 206, "WETGLBT", "Wet Bulb Globe Temperature", "K"},

  /* 4.2 : 0.1  Meteorological products, moisture category */
  {0, 1, 192, "CRAIN", "Categorical Rain", "categorical"},            /* also 4.2 : 0.1 : 33  */
  {0, 1, 193, "CFRZR", "Categorical Freezing Rain", "categorical"},   /* also 4.2 : 0.1 : 34  */
  {0, 1, 194, "CICEP", "Categorical Ice Pellets", "categorical"},     /* also 4.2 : 0.1 : 35  */
  {0, 1, 195, "CSNOW", "Categorical Snow", "categorical"},            /* also 4.2 : 0.1 : 36  */
  {0, 1, 196, "CPRAT", "Convective Precipitation Rate", "kg s m^-2"},/* also 4.2 : 0.1 : 37  */
  {0, 1, 197, "MCONV", "Horizontal Moisture Divergence", "kg s kg^-1"},  /* also 4.2 : 0.1 : 38  */
  {0, 1, 198, "MINRH", "Minimum Relative Humidity", "%"},
  {0, 1, 199, "PEVAP", "Potential Evaporation", "kg m^-2"},            /* also 4.2 : 0.1 : 40  */
  {0, 1, 200, "PEVPR", "Potential Evaporation Rate", "W m^-2"},        /* also 4.2 : 0.1 : 41  */
  {0, 1, 201, "SNOWC", "Snow Cover", "%"},                              /* also 4.2 : 0.1 : 42  */
  {0, 1, 202, "FRAIN", "Rain Fraction of Total Liquid Water", ""},      /* also 4.2 : 0.1 : 43  */
  {0, 1, 203, "FRIME", "Rime Factor", "-"},                             /* also 4.2 : 0.1 : 44  */
  {0, 1, 204, "TCOLR", "Total Column Integrated Rain", "kg m^-2"},     /* also 4.2 : 0.1 : 45  */
  {0, 1, 205, "TCOLS", "Total Column Integrated Snow", "kg m^-2"},     /* also 4.2 : 0.1 : 46  */
  {0, 1, 206, "TIPD", "Total Icing Potential Diagnostic", "-"},
  {0, 1, 207, "NCIP", "Number concentration for ice particles", "-"},
  {0, 1, 208, "SNOT", "Snow temperature", "K"},
  {0, 1, 209, "TCLSW", "Total column-integrated supercooled liquid water", "kg m^-2"},
  {0, 1, 210, "TCOLM", "Total column-integrated melting ice", "kg m^-2"},
  {0, 1, 211, "EMNP", "Evaporation - Precipitation", "cm day^-1"},
  {0, 1, 212, "SBSNO", "Sublimation (evaporation from snow)", "W m^-2"},
  {0, 1, 213, "CNVMR", "Deep Convective Moistening Rate", "kg kg^-1 s^-1"},
  {0, 1, 214, "SHAMR", "Shallow Convective Moistening Rate", "kg kg^-1 s^-1"},
  {0, 1, 215, "VDFMR", "Vertical Diffusion Moistening Rate", "kg kg^-1 s^-1"},
  {0, 1, 216, "CONDP", "Condensation Pressure of Parcali Lifted From Indicate Surface", "Pa"},
  {0, 1, 217, "LRGMR", "Large scale moistening rate", "kg kg^-1 s^-1"},
  {0, 1, 218, "QZ0", "Specific humidity at top of viscous sublayer", "kg kg^-1"},
  {0, 1, 219, "QMAX", "Maximum specific humidity at 2m", "kg kg^-1"},
  {0, 1, 220, "QMIN", "Minimum specific humidity at 2m", "kg kg^-1"},
  {0, 1, 221, "ARAIN", "Liquid precipitation (rainfall)", "kg m^-2"},
  {0, 1, 222, "SNOWT", "Snow temperature, depth-avg", "K"},
  {0, 1, 223, "APCPN", "Total precipitation (nearest grid point)", "kg m^-2"},
  {0, 1, 224, "ACPCPN", "Convective precipitation (nearest grid point)", "kg m^-2"},
  {0, 1, 225, "FRZR", "Freezing Rain", "kg m^-2"},
  {0, 1, 226, "PWTHER", "Predominant Weather", "numeric"},
  {0, 1, 227, "FROZR", "Frozen Rain", "kg m^-2"},
  {0, 1, 228, "FICEAC", "Flat Ice Accumulation", "kg m^-2"},
  {0, 1, 229, "LICEAC", "Line Ice Accumulation", "kg m^-2"},
  {0, 1, 230, "SLACC", "Sleet Accumulation", "kg m^-2"},
  {0, 1, 231, "PPINDX", "Precipitation Potential Index", "%"},
  {0, 1, 232, "PROBCIP", "Probability Cloud Ice Present", "%"},
  {0, 1, 233, "SNOWLR", "Snow Liquid Ratio", "kg kg^-1"},
  {0, 1, 234, "PCPDUR", "Precipitation Duration", "hour"},
  {0, 1, 235, "CLLMR", "Cloud Liquid Mixing Ratio", "kg kg^-1"},
  {0, 1, 241, "TSNOW", "Total Snow", "kg m^-2"},
  {0, 1, 242, "RHPW", "Relative Humidity with Respect to Precipitable Water", "%"},
  {0, 1, 245, "MAXVIG", "Hourly Maximum of Column Vertical Integrated Graupel on Entier Atmosphere", "kg m^-2"},

  /* 4.2 : 0.2 Meteorological products, momentum category */
  {0, 2, 192, "VWSH", "Vertical speed sheer", "1 s^-1"},             /* also 4.2 : 0.2 : 25  */
  {0, 2, 193, "MFLX", "Horizontal Momentum Flux", "N m^-2"},     /* also 4.2 : 0.2 : 26  */
  {0, 2, 194, "USTM", "U-Component Storm Motion", "m s^-1"},         /* also 4.2 : 0.2 : 27  */
  {0, 2, 195, "VSTM", "V-Component Storm Motion", "m s^-1"},         /* also 4.2 : 0.2 : 28  */
  {0, 2, 196, "CD", "Drag Coefficient", "-"},                     /* also 4.2 : 0.2 : 29  */
  {0, 2, 197, "FRICV", "Frictional Velocity", "m s^-1"},             /* also 4.2 : 0.2 : 30  */
  {0, 2, 198, "LAUV", "Latitude of U Wind Component of Velocity", "degrees"},
  {0, 2, 199, "LOUV", "Latitude of U Wind Component of Velocity", "degrees"},
  {0, 2, 200, "LAVV", "Latitude of V Wind Component of Velocity", "degrees"},
  {0, 2, 201, "LOUV", "Longitude of V Wind Component of Velocity", "degrees"},
  {0, 2, 202, "LAPP", "Latitude of Presure Point", "degrees"},
  {0, 2, 203, "LOPP", "Longitude of Presure Point", "degrees"},
  {0, 2, 204, "VEDH", "Vertical Eddy Diffusivity Heat exchange", "m^2 s^-1"},
  {0, 2, 205, "COVMZ", "Covariance between Meridional and Zonal Components of the wind","m^2 s^-2"},
  {0, 2, 206, "COVTZ", "Covariance between Temperature and Zonal Components of the wind", "K m s^-1"},
  {0, 2, 207, "COVTM", "Covariance between Temperature and Meridional Components of the wind", "K m s^-1"},
  {0, 2, 208, "VDFUA", "Vertical Diffusion Zonal Acceleration", "m s^-2"},
  {0, 2, 209, "VDFVA", "Vertical Diffusion Meridional Acceleration", "m s^-2"},
  {0, 2, 210, "GWDU", "Gravity wave drag zonal acceleration", "m s^-2"},
  {0, 2, 211, "GWDV", "Gravity wave drag meridional acceleration", "m s^-2"},
  {0, 2, 212, "CNVU", "Convective zonal momentum mixing acceleration", "m s^-2"},
  {0, 2, 213, "CNVV", "Convective meridional momentum mixing acceleration", "m s^-2"},
  {0, 2, 214, "WTEND", "Tendency of vertical velocity", "m s^-2"},
  {0, 2, 215, "OMGALF", "Omega (Dp/Dt) divide by density", "K"},
  {0, 2, 216, "CNGWDU", "Convective Gravity wave drag zonal acceleration", "m s^-2"},
  {0, 2, 217, "CNGWDV", "Convective Gravity wave drag meridional acceleration", "m s^-2"},
  {0, 2, 218, "LMV", "Velocity Point Model Surface", "-"},
  {0, 2, 219, "PVMWW", "Potential Vorticity (Mass-Weighted)", "1 s^-1 m^-1"},
  {0, 2, 220, "MAXUVV", "Hourly Maximum of Upward Vertical Velocity in the lowest 400hPa", "m s^-1"},
  {0, 2, 221, "MAXDVV", "Hourly Maximum of Downward Vertical Velocity in the lowest 400hPa", "m s^-1"},
  {0, 2, 222, "MAXUW", "U Component of Hourly Maximum 10m Wind Speed", "m s^-1"},
  {0, 2, 223, "MAXVW", "V Component of Hourly Maximum 10m Wind Speed", "m s^-1"},
  {0, 2, 224, "VRATE", "Ventilation Rate", "m^2 s^-1"},
  {0, 2, 225, "TRWSPD", "Transport Wind Speed", "m s-1       "},
  {0, 2, 226, "TRWDIR", "Transport Wind Direction", "Deg     "},
  {0, 2, 227, "TOA10", "Earliest Reasonable Arrival Time (10% exceedance)", "s      "},
  {0, 2, 228, "TOA50", "Most Likely Arrival Time (50% exceedance)", "s      "},
  {0, 2, 229, "TOD50", "Most Likely Departure Time (50% exceedance)", "s    "},
  {0, 2, 230, "TOD90", "Latest Reasonable Departure Time (90% exceedance)", "s      "},
  {0, 2, 231, "TPWDIR", "Tropical Wind Direction", "°"},
  {0, 2, 232, "TPWSPD", "Tropical Wind Speed", "m s-1"},
  {0, 2, 233, "ESHR", "Inflow Based (ESFC) to 50% EL Shear magnitude", "kt"},
  {0, 2, 234, "UESH", "U Component Inflow Based to 50% EL Shear Vector", "kt"},
  {0, 2, 235, "VESH", "V Component Inflow Based to 50% EL Shear Vector", "kt"},
  {0, 2, 236, "UEID", "U Component Bunkers Effective Right Motion", "kt"},
  {0, 2, 237, "VEID", "V Component Bunkers Effective Right Motion", "kt"},

  /* 4.2 : 0.3 Meteorological products, mass category */
  {0, 3, 192, "MSLET", "Mean Sea Level Pressure (Eta Reduction)", "Pa"},
  {0, 3, 193, "5WAVH", "5-Wave Geopotential Height", "m"},                 /* also 4.2 : 0.3 : 15  */
  {0, 3, 194, "U-GWD", "Zonal Flux of Gravity Wave Stress", "N m^-2"},      /* also 4.2 : 0.3 : 16  */
  {0, 3, 195, "V-GWD", "Meridional Flux of Gravity Wave Stress", "N m^-2"}, /* also 4.2 : 0.3 : 17  */
  {0, 3, 196, "HPBL", "Planetary Boundary Layer Height", "m"},               /* also 4.2 : 0.3 : 18  */
  {0, 3, 197, "5WAVA", "5-Wave Geopotential Height Anomaly", "m"},         /* also 4.2 : 0.3 : 19  */
  {0, 3, 198, "MSLMA", "MSLP (MAPS System Reduction)", "Pa"},
  {0, 3, 199, "TSLSA", "3-hr pressure tendency (Std. Atmos. Reduction)", "Pa s^-1"},
  {0, 3, 200, "PLPL", "Pressure of level from which parcel was lifted", "Pa"},
  {0, 3, 201, "LPSX", "X-gradient of Log Pressure", "1 m^-1"},
  {0, 3, 202, "LPSY", "Y-gradient of Log Pressure", "1 m^-1"},
  {0, 3, 203, "HGTX", "X-gradient of Height", "1 m^-1"},
  {0, 3, 204, "HGTY", "Y-gradient of Height", "1 m^-1"},
  {0, 3, 205, "LAYTH", "Layer Thickness", "m"},
  {0, 3, 206, "NLGSP", "Natural Log of Surface Pressure", "ln(kPa)"},
  {0, 3, 207, "CNVUMF", "Convective updraft mass flux", "kg m^-2 s^-1"},
  {0, 3, 208, "CNVDMF", "Convective downdraft mass flux", "kg m^-2 s^-1"},
  {0, 3, 209, "CNVDEMF", "Convective detrainment mass flux", "kg m^-2 s^-1"},
  {0, 3, 210, "LMH", "Mass Point Model Surface", "-"},
  {0, 3, 211, "HGTN", "Geopotential Height (nearest grid point)", "m"},
  {0, 3, 212, "PRESN", "Pressure (nearest grid point)", "Pa"},
  {0, 3, 213, "ORCONV", "Orographic Convexity", "-"},
  {0, 3, 214, "ORASW", "Orographic Asymmetry, W Component", "-"},
  {0, 3, 215, "ORASS", "Orographic Asymmetry, S Component", "-"},
  {0, 3, 216, "ORASSW", "Orographic Asymmetry, SW Component", "-"},
  {0, 3, 217, "ORASNW", "Orographic Asymmetry, NW Component", "-"},	
  {0, 3, 218, "ORLSW", "Orographic Length Scale, W Component", "-"},	
  {0, 3, 219, "ORLSS", "Orographic Length Scale, S Component", "-"},
  {0, 3, 220, "ORLSSW", "Orographic Length Scale, SW Component", "-"},
  {0, 3, 221, "ORLSNW", "Orographic Length Scale, NW Component", "-"},
  {0, 3, 222, "EFSH", "Effective Surface Height", "m"},

  /* 4.2 : 0.4 Meteorological products, short-wave radiation category */
  {0, 4, 192, "DSWRF", "Downward Short-Wave Rad. Flux", "W m^-2"},  /* also 4.2 : 0.4 : 7  */
  {0, 4, 193, "USWRF", "Upward Short-Wave Rad. Flux", "W m^-2"},    /* also 4.2 : 0.4 : 8  */
  {0, 4, 194, "DUVB",  "UV-B downward solar flux", "W m^-2"},
  {0, 4, 195, "CDUVB", "Clear sky UV-B downward solar flu", "W m^-2"},
  {0, 4, 196, "CSDSF", "Clear Sky Downward Solar Flux", "W m^-2"},
  {0, 4, 197, "SWHR", "Solar Radiative Heating Rate", "K s^-1"},
  {0, 4, 198, "CSUSF", "Clear Sky Upward Solar Flux", "W m^-2"},
  {0, 4, 199, "CFNSF", "Cloud Forcing Net Solar Flux", "W m^-2"},
  {0, 4, 200, "VBDSF", "Visible Beam Downward Solar Flux", "W m^-2"},
  {0, 4, 201, "VDDSF", "Visible Diffuse Downward Solar Flux", "W m^-2"},
  {0, 4, 202, "NBDSF", "Near IR Beam Downward Solar Flux", "W m^-2"},
  {0, 4, 203, "NDDSF", "Near IR Diffuse Downward Solar Flux", "W m^-2"},
  {0, 4, 204, "DTRF", "Downward Total radiation Flux", "W m^-2"},
  {0, 4, 205, "UTRF", "Upward Total radiation Flux", "W m^-2"},

  /* 4.2 : 0.5 Meteorological products, long-wave radiation category */
  {0, 5, 192, "DLWRF", "Downward Long-Wave Rad. Flux", "W m^-2"},  /* also 4.2 : 0.5 : 3  */
  {0, 5, 193, "ULWRF", "Upward Long-Wave Rad. Flux", "W m^-2"},    /* also 4.2 : 0.5 : 4  */
  {0, 5, 194, "LWHRF", "Long-Wave Radiative Heating Rate", "W m^-2"},
  {0, 5, 195, "CSULF", "Clear Sky Upward Long Wave Flux", "W m^-2"},
  {0, 5, 196, "CSDLF", "Clear Sky Downward Long Wave Flux", "W m^-2"},
  {0, 5, 197, "CFNLF", "Cloud Forcing Net Long Wave Flux", "W m^-2"},

  /* 4.2 : 0.6 Meteorological products, cloud category */
  {0, 6, 192, "CDLYR", "Non-Convective Cloud Cover", "%"},
  {0, 6, 193, "CWORK", "Cloud Work Function", "J/kg"},
  {0, 6, 194, "CUEFI", "Convective Cloud Efficiency", "-"},
  {0, 6, 195, "TCOND", "Total Condensate", "kg kg^-1"},
  {0, 6, 196, "TCOLW", "Total Column-Integrated Cloud Water", "kg m^-2"},
  {0, 6, 197, "TCOLI", "Total Column-Integrated Cloud Ice", "kg m^-2"},
  {0, 6, 198, "TCOLC", "Total Column-Integrated Condensate", "kg m^-2"},
  {0, 6, 199, "FICE", "Ice fraction of total condensate", "-"},
  {0, 6, 200, "MFLUX", "Convective Cloud Mass Flux", "Pa s^-1"},
  {0, 6, 201, "SUNSD", "Sunshine Duration", "s"},

  /* 4.2 : 0.7 Meteorological products, Thermodynamic Stability category */
  {0, 7, 192, "LFTX", "Surface Lifted Index", "K"},         /* also 4.2 : 0.7 : 10  */
  {0, 7, 193, "4LFTX", "Best (4 layer) Lifted Index", "K"}, /* also 4.2 : 0.7 : 11  */
  {0, 7, 194, "RI", "Richardson Number", "-"},              /* also 4.2 : 0.7 : 12  */
  {0, 7, 195, "CWDI", "Convective Weather Detection Index", "-"},
  {0, 7, 196, "UVI", "Ultra Violet Index", "J m^-2"},
  {0, 7, 197, "UPHL", "Updraft Helicity", "m^2 s^-2"},
  {0, 7, 198, "LAI", "Leaf Area Index", "-"},
  {0, 7, 199, "MXUPHL", "Hourly Maximum of Updraft Helicity over Layer 2km to 5 km AGL", "m^2 s^-2"},
  {0, 7, 200, "MNUPHL", "Hourly Minimum of Updraft Helicity", "m2 s-2"},
  {0, 7, 201, "BNEGELAY", "Bourgoiun Negative Energy Layer (surface to freezing level)", "J kg-1"},
  {0, 7, 202, "BPOSELAY", "Bourgoiun Positive Energy Layer (2k ft AGL to 400 hPa)", "J kg-1"},
  {0, 7, 203, "DCAPE", "Downdraft CAPE", "J kg-1"},
  {0, 7, 204, "EFHL", "Effective Storm Relative Helicity", "m2 s-2"},
  {0, 7, 205, "ESP", "Enhanced Stretching Potential", "Numeric"},
  {0, 7, 206, "CANGLE", "Critical Angle", "Degree"},
  {0, 7, 207, "E3KH", "Effective Surface Helicity", "m2 s-2"},
  {0, 7, 208, "STPC", "Significant Tornado Parameter with CIN-Effective Layer", "numeric"},
  {0, 7, 209, "SIGH", "Significant Hail Parameter", "numeric"},
  {0, 7, 210, "SCCP", "Supercell Composite Parameter-Effective Layer", "numeric"},
  {0, 7, 211, "SIGT", "Significant Tornado Parameter-Fixed Layer", "numeric"},
  {0, 7, 212, "MLFC", "Mixed Layer (100 mb) Virtual LFC", "numeric"},

  /* 4.2 : 0.13 Meteorological products, Aerosols category */
  {0, 13, 192, "PMTC", "Particulate matter (coarse)", "µg m^-3"},
  {0, 13, 193, "PMTF", "Particulate matter (fine)", "µg m^-3"},
  {0, 13, 194, "LPMTF", "Particulate matter (fine)", "log10(µg m^-3)"},
  {0, 13, 195, "LIPMF", "Integrated column particulate matter (fine)", "log10(µg m^-3)"},

  /* 4.2 : 0.14 Meteorological products, Trace Gases category */
  {0, 14, 192, "O3MR", "Ozone Mixing Ratio", "kg kg^-1"},  /* also 4.2 : 0.14 : 1  */
  {0, 14, 193, "OZCON", "Ozone Concentration", "PPB"},
  {0, 14, 194, "OZCAT", "Categorical Ozone Concentration", "unknown"},
  {0, 14, 195, "VDFOZ", "Ozone vertical diffusion", "kg kg^-1 s^-1"},
  {0, 14, 196, "POZ", "Ozone production", "kg kg^-1 s^-1"},
  {0, 14, 197, "TOZ", "Ozone tendency", "kg kg^-1 s^-1"},
  {0, 14, 198, "POZT", "Ozone production from temperature term", "kg kg^-1 s^-1"},
  {0, 14, 199, "POZO", "Ozone production from col ozone term", "kg kg^-1 s^-1"},
  {0, 14, 200, "OZMAX1", "Ozone Daily Max from 1-hour Average", "ppbV"},
  {0, 14, 201, "OZMAX8", "Ozone Daily Max from 8-hour Average", "ppbV"},
  {0, 14, 202, "PDMAX1", "PM 2.5 Daily Max from 1-hour Average", "ug m^-3"},
  {0, 14, 203, "PDMAX24", "PM 2.5 Daily Max from 24-hour Average", "ug m^-3"},
  {0, 14, 204, "ALD2", "Acetaldehyde & Higher Aldehydes", "ppbV"},

  /* 4.2 : 0.16 Meteorological products, Forecast Radar Imagery category */
  {0, 16, 192, "REFZR", "Equivalent radar reflectivity factor for rain", "m m^6 m^-3"},
  {0, 16, 193, "REFZI", "Equivalent radar reflectivity factor for snow", "m m^6 m^-3"},
  {0, 16, 194, "REFZC", "Equivalent radar reflectivity factor for parameterized convection", "m m^6 m^-3"},
  {0, 16, 195, "REFD", "Reflectivity", "dB"},
  {0, 16, 196, "REFC", "Composite radar reflectivity", "dB"},
  {0, 16, 197, "RETOP", "Echo Top", "m"},
  {0, 16, 198, "MAXREF", "Hourly Maximum of Simulated Reflectivity at 1 km AGL", "dB"},

  /* 4.2 : 0.17 Meteorological products, Electrodynamics category */
  {0, 17, 192, "LTNG", "Lightning", "-"},

  /* 4.2 : 0.19 Meteorological products, Physical Atmospheric category */
  {0, 19, 192, "MXSALB", "Maximum Snow Albedo", "%"},       /* also 4.2 : 0.19 : 17  */
  {0, 19, 193, "SNFALB", "Snow-Free Albedo", "%"},          /* also 4.2 : 0.19 : 18  */
  {0, 19, 194, "SlightRisk", "Slight risk convective outlook", "categorical"},
  {0, 19, 195, "ModerateRisk", "Moderate risk convective outlook", "categorical"},
  {0, 19, 196, "HighRisk", "High risk convective outlook", "categorical"},
  {0, 19, 197, "TornadoProb", "Tornado probability", "%"},
  {0, 19, 198, "HailProb", "Hail probability", "%"},
  {0, 19, 199, "WindProb", "Wind probability", "%"},
  {0, 19, 200, "SigTornadoProb", "Significant Tornado probability", "%"},
  {0, 19, 201, "SigHailProb", "Significant Hail probability", "%"},
  {0, 19, 202, "SigWindProb", "Significant Wind probability", "%"},
  {0, 19, 203, "CatThunder", "Categorical Thunderstorm", "categorical"},
  {0, 19, 204, "MIXLY", "Number of mixed layers next to surface", "integer"},
  {0, 19, 205, "FLGHT", "Flight Category", "none"},
  {0, 19, 206, "CICEL", "Confidence - Ceiling", "-"},
  {0, 19, 207, "CIVIS", "Confidence - Visibility", "-"},
  {0, 19, 208, "CIFLT", "Confidence - Flight Category", "-"},
  {0, 19, 209, "LAVNI", "Low-Level aviation interest", "-"},
  {0, 19, 210, "HAVNI", "High-Level aviation interest", "-"},
  {0, 19, 211, "SBSALB", "Visible, Black Sky Albedo", "%"},
  {0, 19, 212, "SWSALB", "Visible, White Sky Albedo", "%"},
  {0, 19, 213, "NBSALB", "Near IR, Black Sky Albedo", "%"},
  {0, 19, 214, "NWSALB", "Near IR, White Sky Albedo", "%"},
  {0, 19, 215, "PRSVR", "Total Probability of Severe Thunderstorms (Days 2,3)", "%"},
  {0, 19, 216, "PRSIGSVR", "Total Probability of Extreme Severe Thunderstorms (Days 2,3)", "%"},
  {0, 19, 217, "SLDICE", "Supercooled Large Droplet Icing", "categorical"},
  {0, 19, 218, "EPSR", "Radiative emissivity", "-"},
  {0, 19, 219, "TPFI", "Turbulence Potential Forecast Index", "-"},
  {0, 19, 220, "SVRTS", "Categorical Severe Thunderstorm", "categorical"},
  {0, 19, 221, "PROCON", "Probability of Convection", "%"},
  {0, 19, 222, "CONVP", "Convection Potential", "categorical"},
  /*   These have been taken out of the NCEP local tables
  {0, 19, 217, "MEIP", "Mean Icing Potential", "-"},
  {0, 19, 218, "MAIP", "Maximum Icing Potential", "-"},
  {0, 19, 219, "MECTP", "Mean in-Cloud Turbulence Potential", "-"},
  {0, 19, 220, "MACTP", "Max in-Cloud Turbulence Potential", "-"},
  {0, 19, 221, "MECAT", "Mean Cloud Air Turbulence Potentia", "-"},
  {0, 19, 222, "MACAT", "Maximum Cloud Air Turbulence Potentia", "-"},
  {0, 19, 223, "CBHE", "Cumulonimbus Horizontal Extent", "%"},
  {0, 19, 224, "PCBB", "Pressure at Cumulonimbus Base", "Pa"},
  {0, 19, 225, "PCBT", "Pressure at Cumulonimbus Top", "Pa"},
  {0, 19, 226, "PECBB", "Pressure at Embedded Cumulonimbus Base", "Pa"},
  {0, 19, 227, "PECBT", "Pressure at Embedded Cumulonimbus Top", "Pa"},
  {0, 19, 228, "HCBB", "ICAO Height at Cumulonimbus Base", "m"},
  {0, 19, 229, "HCBT", "ICAO Height at Cumulonimbus Top", "m"},
  {0, 19, 230, "HECBB", "ICAO Height at Embedded Cumulonimbus Base", "m"},
  {0, 19, 231, "HECBT", "ICAO Height at Embedded Cumulonimbus Top", "m"},
  */
  {0, 19, 232, "VAFTD", "Volcanic Ash Forecast Transport and Dispersion", "log10(kg m^-3)"},
  {0, 19, 233, "ICPRB", "Icing probability", "-"},
  {0, 19, 234, "ICSEV", "Icing severity", "-"},
  {0, 19, 235, "JFWPRB", "Joint Fire Weather Probability", "%"},
  {0, 19, 236, "SNOWLVL", "Snow Level", "m"},
  {0, 19, 237, "DRYTPROB", "Dry Thunderstorm Probability", "%"},
  {0, 19, 238, "ELLINX", "Ellrod Index", "-"},
  {0, 19, 239, "CWASP", "Craven-Wiedenfeld Aggregate Severe Parameter", "Numeric"},

  /* 4.2 : 0.191 Meteorological products, Miscellaneous category */
  {0, 191, 192, "NLAT", "Latitude (-90 to 90)", "degrees"},
  {0, 191, 193, "ELON", "East Longitude (0 to 360)", "degrees"},
  {0, 191, 194, "TSEC", "Seconds prior to initial reference time", "sec"},  /* also 4.2 : 0.191 : 1  */
  {0, 191, 195, "MLYNO", "Model Layer number (From bottom up)", "-"},
  {0, 191, 196, "NLATN", "Latitude (nearest neighbor) (-90 to +90)", "degrees"},
  {0, 191, 197, "ELONN", "East Longitude (nearest neighbor) (0 - 360)", "degrees"},

  /* 4.2 : 0.192 Meteorological products, Covariance category */
  {0, 192, 1, "COVZM", "Covariance between zonal and meridional components of the wind. Defined as [uv]-[u][v]", "m^2 s^-2"},
  {0, 192, 2, "COVTZ", "Covariance between izonal component of the wind and temperature. Defined as [uT]-[u][T]", "K m s^-1"},
  {0, 192, 3, "COVTM", "Covariance between meridional component of the wind and temperature. Defined as [vT]-[v][T]", "K m s^-1"},
  {0, 192, 4, "COVTW", "Covariance between temperature and vertical component of the wind. Defined as [wT]-[w][T]", "K m s^-1"},
  {0, 192, 5, "COVZZ", "Covariance between zonal and zonal components of the wind. Defined as [uu]-[u][u]", "m^2 s^-2"},
  {0, 192, 6, "COVMM", "Covariance between meridional and meridional components of the wind. Defined as [vv]-[v][v]", "m^2 s^-2"},
  {0, 192, 7, "COVQZ", "Covariance between specific humidity and zonal components of the wind. Defined as [uq]-[u][q]", "kg kg^-1 m s^-1"},
  {0, 192, 8, "COVQM", "Covariance between specific humidity and meridional components of the wind. Defined as [vq]-[v][q]", "kg kg^-1 m s^-1"},
  {0, 192, 9, "COVTVV", "Covariance between temperature and vertical components of the wind. Defined as [\u03a9T]-[\u03a9][T]", "K Pa s^-1"},
  {0, 192, 10, "COVQVV", "ovariance between specific humidity and vertical components of the wind. Defined as [\u03a9q]-[\u03a9][q]", "kg kg^-1 Pa s^-1"},
  {0, 192, 11, "COVPSPS", "Covariance between surface pressure and surface pressure. Defined as [Psfc]-[Psfc][Psfc]", "Pa Pa"},
  {0, 192, 12, "COVQQ", "Covariance between specific humidity and specific humidy. Defined as [qq]-[q][q]", "kg kg^-1 kg kg^-1"},
  {0, 192, 13, "COVVVVV", "Covariance between vertical and vertical components of the wind. Defined as [\u03a9\u03a9]-[\u03a9][\u03a9]", "Pa^2 s^-2"},
  {0, 192, 14, "COVTT", "Covariance between temperature and temperature. Defined as [TT]-[T][T]", "K K"},

  /* 4.2 : 1.0 Hydrological products, Basic Hydrology category */
  {1, 0, 192, "BGRUN", "Baseflow-Groundwater Runoff", "kg m^-2"}, /* also 4.2 : 1.0 : 5  */
  {1, 0, 193, "SSRUN", "Storm Surface Runoff", "kg m^-2"},        /* also 4.2 : 1.0 : 6  */

  /* 4.2 : 1.1 Hydrological products, Hydrology Probabilities category */
  {1, 1, 192, "PropFreezPre", "Probability of Freezing Precipitation", "%"},
  {1, 1, 193, "PropFrozenPre", "Probability of Frozen Precipitation", "%"},	
  {1, 1, 194, "PPFFG", "Probability of precipitation exceeding flash flood guidance values", "%"},
  {1, 1, 195, "CWR", "Probability of Wetting Rain, exceeding in 0.10 in a given time period", "%"},	
  {1, 1, 196, "QPFARI", "Binary Probability of precipitation exceeding average recurrence intervals (ARI)", "%"}, // (0 No, 1 Yes)	
  {1, 1, 197, "QPFFFG", "Binary Probability of precipitation exceeding flash flood guidance values", "%"}, // (0 No, 1 Yes)	

  /* 4.2 : 2.0 Land Surface products, Vegetation/Biomass category */
  {2, 0, 192, "SOILW", "Volumetric Soil Moisture Content", "fraction"},             /* also 4.2 : 2.0 : 9  */
  {2, 0, 193, "GFLUX", "Ground Heat Flux", "W m^-2"},                              /* also 4.2 : 2.0 : 10  */
  {2, 0, 194, "MSTAV", "Moisture Availability", "%"},                               /* also 4.2 : 2.0 : 11  */
  {2, 0, 195, "SFEXC", "Exchange Coefficient", "kg m^-3 m s^-1"},                  /* also 4.2 : 2.0 : 12  */
  {2, 0, 196, "CNWAT", "Plant Canopy Surface Water", "kg m^-2"},                   /* also 4.2 : 2.0 : 13  */
  {2, 0, 197, "BMIXL", "Blackadar's Mixing Length Scale", "m"},                     /* also 4.2 : 2.0 : 14  */
  {2, 0, 198, "VGTYP", "Vegetation Type", "categorical"},
  {2, 0, 199, "CCOND", "Canopy Conductance", "m s^-1"},                                /* also 4.2 : 2.0 : 15  */
  {2, 0, 200, "RSMIN", "Minimal Stomatal Resistance", "s m^-1"},                       /* also 4.2 : 2.0 : 16  */
  {2, 0, 201, "WILT", "Wilting Point", "fraction"},                                 /* also 4.2 : 2.0 : 17  */
  {2, 0, 202, "RCS", "Solar parameter in canopy conductance", "fraction"},          /* also 4.2 : 2.0 : 18  */
  {2, 0, 203, "RCT", "Temperature parameter in canopy conductance", "fraction"},    /* also 4.2 : 2.0 : 19  */
  {2, 0, 204, "RCQ", "Humidity parameter in canopy conductance", "fraction"},       /* also 4.2 : 2.0 : 21  */
  {2, 0, 205, "RCSOL", "Soil moisture parameter in canopy conductance", "fraction"},/* also 4.2 : 2.0 : 20  */
  {2, 0, 206, "RDRIP", "Rate of water dropping from canopy to ground", "-"},
  {2, 0, 207, "ICWAT", "Ice-free water surface", "%"},
  {2, 0, 208, "AKHS", "Surface exchange coefficients for T and Q divided by delta z", "m s^-1"},
  {2, 0, 209, "AKMS", "Surface exchange coefficients for U and V divided by delta z", "m s^-1"},
  {2, 0, 210, "VEGT", "Vegetation canopy temperature", "K"},
  {2, 0, 211, "SSTOR", "Surface water storage", "Kg m^-2"},
  {2, 0, 212, "LSOIL", "Liquid soil moisture content (non-frozen)", "Kg m^-2"},
  {2, 0, 213, "EWATR", "Open water evaporation (standing water)", "W m^-2"},
  {2, 0, 214, "GWREC", "Groundwater recharge", "kg m^-2"},
  {2, 0, 215, "QREC", "Flood plain recharge", "kg m^-2"},
  {2, 0, 216, "SFCRH", "Roughness length for heat", "m"},
  {2, 0, 217, "NDVI", "Normalized Difference Vegetation Index", "-"},
  {2, 0, 218, "LANDN", "Land-sea coverage (nearest neighbor) [land=1,sea=0]", "categorical"},
  {2, 0, 219, "AMIXL", "Asymptotic mixing length scale", "m"},
  {2, 0, 220, "WVINC", "Water vapor added by precip assimilation", "kg m^-2"},
  {2, 0, 221, "WCINC", "Water condensate added by precip assimilation", "kg m^-2"},
  {2, 0, 222, "WVCONV", "Water Vapor Flux Convergance (Vertical Int)", "kg m^-2"},
  {2, 0, 223, "WCCONV", "Water Condensate Flux Convergance (Vertical Int)", "kg m^-2"},
  {2, 0, 224, "WVUFLX", "Water Vapor Zonal Flux (Vertical Int)", "kg m^-2"},
  {2, 0, 225, "WVVFLX", "Water Vapor Meridional Flux (Vertical Int)", "kg m^-2"},
  {2, 0, 226, "WCUFLX", "Water Condensate Zonal Flux (Vertical Int)", "kg m^-2"},
  {2, 0, 227, "WCVFLX", "Water Condensate Meridional Flux (Vertical Int)", "kg m^-2"},
  {2, 0, 228, "ACOND", "Aerodynamic conductance", "m s^-1"},
  {2, 0, 229, "EVCW", "Canopy water evaporation", "W m^-2"},
  {2, 0, 230, "TRANS", "Transpiration", "W m^-2"},
  {2, 0, 231, "VEGMIN", "Seasonally Minimum Green Vegetation Fraction (over 1-year period)", "%"},
  {2, 0, 232, "VEGMAX", "Seasonally Maximum Green Vegetation Fraction (over 1-year period)", "%"},
  {2, 0, 233, "LANDFRC", "Land Fraction", "Fraction"},
  {2, 0, 234, "LAKEFRC", "Lake Fraction", "Fraction"},
  {2, 0, 235, "PAHFLX", "Precipitation Advected Head Flux", "W m^-2"},
  {2, 0, 236, "WATERSA", "Water Storage in Aquifer", "Kg m^-2"},
  {2, 0, 237, "EIWATER", "Evaporation of Intercepted Water", "Kg m^-2"},
  {2, 0, 238, "PLANTTR", "Plant Transpiration", "Kg m^-2"},
  {2, 0, 239, "SOILSE", "Soil Surface Evaporation", "Kg m^-2"},

  /* 4.2 : 2.1  Land Surface products, Agricultural category */
  {2, 1, 192, "CANL", "Cold Advisory for Newborn Livestock", "numeric"},

  /* 4.2 : 2.3  Land Surface products, Soil category */
  {2, 3, 192, "SOILL", "Liquid Volumetric Soil Moisture", "fraction"},            /* also 4.2 : 2.3 : 5 */
  {2, 3, 193, "RLYRS", "Number of Soil Layers in Root Zone", "-"},                /* also 4.2 : 2.3 : 6 */
  {2, 3, 194, "SLTYP", "Surface Slope Type", "index"},
  {2, 3, 195, "SMREF", "Transpiration Stress-onset (soil moisture)", "fraction"}, /* also 4.2 : 2.3 : 7 */
  {2, 3, 196, "SMDRY", "Direct Evaporation Cease (soil moisture)", "fraction"},   /* also 4.2 : 2.3 : 8 */
  {2, 3, 197, "POROS", "Soil Porosity", "fraction"},                              /* also 4.2 : 2.3 : 9 */
  {2, 3, 198, "EVBS", "Direct evaporation from bare soil", "W m^-2"},
  {2, 3, 199, "LSPA", "Land Surface Precipitation Accumulation", "kg m^-2"},
  {2, 3, 200, "BARET", "Bare soil surface skin temperature", "K"},
  {2, 3, 201, "AVSFT", "Average surface skin temperature", "K"},
  {2, 3, 202, "RADT", "Effective radiative skin temperature", "K"},
  {2, 3, 203, "FLDCP", "Field Capacity", "fraction"},
  {2, 3, 204, "MSTAV", "Soil Moisture Availability In The Top Soil Layer", "%"},

  /* 4.2 : 3.1 Satellite Remote Sensing products, Quantitative category */
  {3, 1, 192, "USCT", "Scatterometer Estimated U Wind", "-"},
  {3, 1, 193, "VSCT", "Scatterometer Estimated V Wind", "-"},
  {3, 1, 194, "SWQI", "Scatterometer Wind Quality","-"},

  /* 4.2 : 3.192 Satellite Remote Sensing products, Forecast Satellite Imagery */
  {3, 192, 0, "SBT122", "Simulated Brightness Temperature for GOES 12, Channel 2", "K"},
  {3, 192, 1, "SBT123", "Simulated Brightness Temperature for GOES 12, Channel 3", "K"},
  {3, 192, 2, "SBT124", "Simulated Brightness Temperature for GOES 12, Channel 4", "K"},
  {3, 192, 3, "SBT125", "Simulated Brightness Temperature for GOES 12, Channel 5", "K"},
  {3, 192, 4, "SBC123", "Simulated Brightness Counts for GOES 12, Channel 3", "byte"},
  {3, 192, 5, "SBC124", "Simulated Brightness Counts for GOES 12, Channel 4", "byte"},
  {3, 192, 6, "SBT112", "Simulated Brightness Temperature for GOES 11, Channel 2", "K"},
  {3, 192, 7, "SBT113", "Simulated Brightness Temperature for GOES 11, Channel 3", "K"},
  {3, 192, 8, "SBT114", "Simulated Brightness Temperature for GOES 11, Channel 4", "K"},
  {3, 192, 9, "SBT115", "Simulated Brightness Temperature for GOES 11, Channel 5", "K"},
  {3, 192, 10, "AMSRE9", "Simulated Brightness Temperature for AMSRE on Aqua, Channel 9", "K"},
  {3, 192, 11, "AMSRE10", "Simulated Brightness Temperature for AMSRE on Aqua, Channel 10", "K"},
  {3, 192, 12, "AMSRE11", "Simulated Brightness Temperature for AMSRE on Aqua, Channel 11", "K"},
  {3, 192, 13, "AMSRE12", "Simulated Brightness Temperature for AMSRE on Aqua, Channel 12", "K"},
  {3, 192, 14, "SRFA161", "Simulated Reflectance Factor for ABI GOES-16, Band-1", "-"},
  {3, 192, 15, "SRFA162", "Simulated Reflectance Factor for ABI GOES-16, Band-2", "-"},
  {3, 192, 16, "SRFA163", "Simulated Reflectance Factor for ABI GOES-16, Band-3", "-"},
  {3, 192, 17, "SRFA164", "Simulated Reflectance Factor for ABI GOES-16, Band-4", "-"},
  {3, 192, 18, "SRFA165", "Simulated Reflectance Factor for ABI GOES-16, Band-5", "-"},
  {3, 192, 19, "SRFA166", "Simulated Reflectance Factor for ABI GOES-16, Band-6", "-"},
  {3, 192, 20, "SBTA167", "Simulated Brightness Temperature for ABI GOES-16, Band-7", "K"},
  {3, 192, 21, "SBTA168", "Simulated Brightness Temperature for ABI GOES-16, Band-8", "K"},
  {3, 192, 22, "SBTA169", "Simulated Brightness Temperature for ABI GOES-16, Band-9", "K"},
  {3, 192, 23, "SBTA1610", "imulated Brightness Temperature for ABI GOES-16, Band-10", "K"},
  {3, 192, 24, "SBTA1611", "Simulated Brightness Temperature for ABI GOES-16, Band-11", "K"},
  {3, 192, 25, "SBTA1612", "Simulated Brightness Temperature for ABI GOES-16, Band-12", "K"},
  {3, 192, 26, "SBTA1613", "Simulated Brightness Temperature for ABI GOES-16, Band-13", "K"},
  {3, 192, 27, "SBTA1614", "Simulated Brightness Temperature for ABI GOES-16, Band-14", "K"},
  {3, 192, 28, "SBTA1615", "Simulated Brightness Temperature for ABI GOES-16, Band-15", "K"},
  {3, 192, 29, "SBTA1616", "Simulated Brightness Temperature for ABI GOES-16, Band-16", "K"},
  {3, 192, 30, "SRFA171", "Simulated Reflectance Factor for ABI GOES-17, Band-1", "-"},
  {3, 192, 31, "SRFA172", "Simulated Reflectance Factor for ABI GOES-17, Band-2", "-"},
  {3, 192, 32, "SRFA173", "Simulated Reflectance Factor for ABI GOES-17, Band-3", "-"},
  {3, 192, 33, "SRFA174", "Simulated Reflectance Factor for ABI GOES-17, Band-4", "-"},
  {3, 192, 34, "SRFA175", "Simulated Reflectance Factor for ABI GOES-17, Band-5", "-"},
  {3, 192, 35, "SRFA176", "Simulated Reflectance Factor for ABI GOES-17, Band-6", "-"},
  {3, 192, 36, "SBTA177", "Simulated Brightness Temperature for ABI GOES-17, Band-7", "K"},
  {3, 192, 37, "SBTA178", "Simulated Brightness Temperature for ABI GOES-17, Band-8", "K"},
  {3, 192, 38, "SBTA179", "Simulated Brightness Temperature for ABI GOES-17, Band-9", "K"},
  {3, 192, 39, "SBTA1710", "Simulated Brightness Temperature for ABI GOES-17, Band-10", "K"},
  {3, 192, 40, "SBTA1711", "Simulated Brightness Temperature for ABI GOES-17, Band-11", "K"},
  {3, 192, 41, "SBTA1712", "Simulated Brightness Temperature for ABI GOES-17, Band-12", "K"},
  {3, 192, 42, "SBTA1713", "Simulated Brightness Temperature for ABI GOES-17, Band-13", "K"},
  {3, 192, 43, "SBTA1714", "Simulated Brightness Temperature for ABI GOES-17, Band-14", "K"},
  {3, 192, 44, "SBTA1715", "Simulated Brightness Temperature for ABI GOES-17, Band-15", "K"},
  {3, 192, 45, "SBTA1716", "Simulated Brightness Temperature for ABI GOES-17, Band-16", "K"},

  {3, 192, 46, "SRFAGR1", "Simulated Reflectance Factor for nadir ABI GOES-R, Band-1", "-"},
  {3, 192, 47, "SRFAGR2", "Simulated Reflectance Factor for nadir ABI GOES-R, Band-2", "-"},
  {3, 192, 48, "SRFAGR3", "Simulated Reflectance Factor for nadir ABI GOES-R, Band-3", "-"},
  {3, 192, 49, "SRFAGR4", "Simulated Reflectance Factor for nadir ABI GOES-R, Band-4", "-"},
  {3, 192, 50, "SRFAGR5", "Simulated Reflectance Factor for nadir ABI GOES-R, Band-5", "-"},
  {3, 192, 51, "SRFAGR6", "Simulated Reflectance Factor for nadir ABI GOES-R, Band-6", "-"},
  {3, 192, 52, "SBTAGR7", "Simulated Brightness Temperature for nadir ABI GOES-R, Band-7", "K"},
  {3, 192, 53, "SBTAGR8", "Simulated Brightness Temperature for nadir ABI GOES-R, Band-8", "K"},
  {3, 192, 54, "SBTAGR9", "Simulated Brightness Temperature for nadir ABI GOES-R, Band-9", "K"},
  {3, 192, 55, "SBTAGR10", "Simulated Brightness Temperature for nadir ABI GOES-R, Band-10", "K"},
  {3, 192, 56, "SBTAGR11", "Simulated Brightness Temperature for nadir ABI GOES-R, Band-11", "K"},
  {3, 192, 57, "SBTAGR12", "Simulated Brightness Temperature for nadir ABI GOES-R, Band-12", "K"},
  {3, 192, 58, "SBTAGR13", "Simulated Brightness Temperature for nadir ABI GOES-R, Band-13", "K"},
  {3, 192, 59, "SBTAGR14", "Simulated Brightness Temperature for nadir ABI GOES-R, Band-14", "K"},
  {3, 192, 60, "SBTAGR15", "Simulated Brightness Temperature for nadir ABI GOES-R, Band-15", "K"},
  {3, 192, 61, "SBTAGR16", "Simulated Brightness Temperature for nadir ABI GOES-R, Band-16", "K"},
  {3, 192, 62, "SSMS1715", "Simulated Brightness Temperature for SSMIS-F17, Channel 15", "K"},
  {3, 192, 63, "SSMS1716", "Simulated Brightness Temperature for SSMIS-F17, Channel 16", "K"},
  {3, 192, 64, "SSMS1717", "Simulated Brightness Temperature for SSMIS-F17, Channel 17", "K"},
  {3, 192, 65, "SSMS1718", "Simulated Brightness Temperature for SSMIS-F17, Channel 18", "K"},
  {3, 192, 66, "SBTAHI7", "Simulated Brightness Temperature for Himawari-8, Band-7", "K"},
  {3, 192, 67, "SBTAHI8", "Simulated Brightness Temperature for Himawari-8, Band-8", "K"},
  {3, 192, 68, "SBTAHI9", "Simulated Brightness Temperature for Himawari-8, Band-9", "K"},
  {3, 192, 69, "SBTAHI10", "Simulated Brightness Temperature for Himawari-8, Band-10", "K"},
  {3, 192, 70, "SBTAHI11", "Simulated Brightness Temperature for Himawari-8, Band-11", "K"},
  {3, 192, 71, "SBTAHI12", "Simulated Brightness Temperature for Himawari-8, Band-12", "K"},
  {3, 192, 72, "SBTAHI13", "Simulated Brightness Temperature for Himawari-8, Band-13", "K"},
  {3, 192, 73, "SBTAHI14", "Simulated Brightness Temperature for Himawari-8, Band-14", "K"},
  {3, 192, 74, "SBTAHI15", "Simulated Brightness Temperature for Himawari-8, Band-15", "K"},
  {3, 192, 75, "SBTAHI16", "Simulated Brightness Temperature for Himawari-8, Band-16", "K"},
  {3, 192, 76, "SBTA187", "Simulated Brightness Temperature for ABI GOES-18, Band-7", "K"},
  {3, 192, 77, "SBTA188", "Simulated Brightness Temperature for ABI GOES-18, Band-8", "K"},
  {3, 192, 78, "SBTA189", "Simulated Brightness Temperature for ABI GOES-18, Band-9", "K"},
  {3, 192, 79, "SBTA1810", "Simulated Brightness Temperature for ABI GOES-18, Band-10", "K"},
  {3, 192, 80, "SBTA1811", "Simulated Brightness Temperature for ABI GOES-18, Band-11", "K"},
  {3, 192, 81, "SBTA1812", "Simulated Brightness Temperature for ABI GOES-18, Band-12", "K"},
  {3, 192, 82, "SBTA1813", "Simulated Brightness Temperature for ABI GOES-18, Band-13", "K"},
  {3, 192, 83, "SBTA1814", "Simulated Brightness Temperature for ABI GOES-18, Band-14", "K"},
  {3, 192, 84, "SBTA1815", "Simulated Brightness Temperature for ABI GOES-18, Band-15", "K"},
  {3, 192, 85, "SBTA1816", "Simulated Brightness Temperature for ABI GOES-18, Band-16", "K"},

  /* 4.2 : 10.0 Oceanographic products, Waves category */
  {10, 0, 192, "WSTP", "Wave Steepness", "-"},
  {10, 0, 193, "WLENG", "Wave Length", "-"},

  /* 4.2 : 10.1 Oceanographic products, Currents category */
  {10, 1, 192, "OMLU", "Ocean Mixed Layer U Velocity", "m s^-1"},
  {10, 1, 193, "OMLV", "Ocean Mixed Layer V Velocity", "m s^-1"},
  {10, 1, 194, "UBARO", "Barotropic U velocity", "m s^-1"},
  {10, 1, 195, "VBARO", "Barotropic V velocity", "m s^-1"},

  /* 4.2 : 10.3 Oceanographic products, Surface Properties category */
  {10, 3, 192, "SURGE", "Hurricane Storm Surge", "m"},
  {10, 3, 193, "ETSRG", "Extra Tropical Storm Surge", "m"},
  {10, 3, 194, "ELEV", "Ocean Surface Elevation Relative to Geoid", "m"},
  {10, 3, 195, "SSHG", "Sea Surface Height Relative to Geoid", "m"},
  {10, 3, 196, "P2OMLT", "Ocean Mixed Layer Potential Density (Reference 2000m)", "kg m^-3"},
  {10, 3, 197, "AOHFLX", "Net Air-Ocean Heat Flux", "W m^-2"},
  {10, 3, 198, "ASHFL", "Assimilative Heat Flux", "W m^-2"},
  {10, 3, 199, "SSTT", "Surface Temperature Trend", "degree day^-1"},
  {10, 3, 200, "SSST", "Surface Salinity Trend", "psu day^-1"},
  {10, 3, 201, "KENG", "Kinetic Energy", "J kg^-1"},
  {10, 3, 202, "SLTFL", "Salt Flux", "kg m^-2 s^-1"},
  {10, 3, 203, "LCH", "Heat Exchange Coefficient", "-"},
  {10, 3, 204, "FRZSPR", "Freezing Spray", "-"},
  {10, 3, 205, "TWLWAV", "Total Water Level Accounting for Tide, Wind and Waves", "m"},
  {10, 3, 206, "RUNUP", "Total Water Level Increase due to Waves", "m"},
  {10, 3, 207, "SETUP", "Mean Increase in Water Level due to Waves", "m"},
  {10, 3, 208, "SWASH", "Time-varying Increase in Water Level due to Waves", "m"},
  {10, 3, 209, "TWLDT", "Total Water Level Above Dune Toe", "m"},
  {10, 3, 210, "TWLDC", "Total Water Level Above Dune Crest", "m"},
  {10, 3, 242, "TCSRG20", "20% Tropical Cyclone Storm Surge Exceedanc", "m"},
  {10, 3, 243, "TCSRG30", "30% Tropical Cyclone Storm Surge Exceedanc", "m"},
  {10, 3, 244, "TCSRG40", "40% Tropical Cyclone Storm Surge Exceedanc", "m"},
  {10, 3, 245, "TCSRG50", "50% Tropical Cyclone Storm Surge Exceedanc", "m"},
  {10, 3, 246, "TCSRG60", "60% Tropical Cyclone Storm Surge Exceedanc", "m"},
  {10, 3, 247, "TCSRG70", "70% Tropical Cyclone Storm Surge Exceedanc", "m"},
  {10, 3, 248, "TCSRG80", "80% Tropical Cyclone Storm Surge Exceedanc", "m"},
  {10, 3, 249, "TCSRG90", "90% Tropical Cyclone Storm Surge Exceedanc", "m"},
  {10, 3, 250, "ETCWL", "Extra Tropical Storm Surge Combined Surge and Tide", "m"},
  {10, 3, 251, "TIDE", "Tide", "m"},
  {10, 3, 252, "EROSNP", "Erosion Occurrence Probability", "%"},
  {10, 3, 253, "OWASHP", "Overwash Occurrence Probability", "%"},

  /* 4.2 : 10.4 Oceanographic products, Sub-Surface Properties category) */
  {10, 4, 192, "WTMPC", "3-D Temperature", "degrees c"},
  {10, 4, 193, "SALIN", "3-D Salinity", "-"},
  {10, 4, 194, "BKENG", "Barotropic Kinectic Energy", "J kg^-1"},
  {10, 4, 195, "DBSS", "Geometric Depth Below Sea Surface", "m"},
  {10, 4, 196, "INTFD", "Interface Depths", "m"},
  {10, 4, 197, "OHC", "Ocean Heat Content", "J m^-2"},

};

/* SPC (Storm Prediction Center) Local Use Table, Center 7, SubCenter 9 or 255 */

const ProdDefTemp::_GRIB2LocalTable ProdDefTemp::_SPClocalTable[] = {

  {0, 19, 205, "MUCP", "Most Unstable Parcel Cape", "J kg^-1"},
  {0, 19, 206, "MUCN", "Most Unstable Parcel CIN", "J kg^-1"},
  {0, 19, 207, "M1CP", "Mean Layer Parcel Cape", "J kg^-1"},
  {0, 19, 208, "SBCP", "Surface Based Parcel Cape", "J kg^-1"},
  {0, 19, 209, "ML3K", "Mean Layer Cape 0-3km", "J kg^-1"},
  {0, 19, 210, "DNCP", "Downdraft Cape", "J kg^-1"},
  {0, 19, 211, "NCAP", "Normalized Cape", "m s^-2"},
  {0, 19, 212, "MLCH", "Mean Layer parcel LCL Height", "m"},
  {0, 19, 213, "MLFH", "Mean Layer parcel LFC Height", "m"},
  {0, 19, 214, "MFCN", "Moisture Flux Convergence", "g kg^-1"},
  {0, 19, 215, "INPW", "Integrated Precipitable Water", "in"},
  {0, 19, 216, "FRNT", "Surface Frontogenesis", "K"},
  {0, 19, 217, "U6SV", "0-6km Shear U comp", "m s^-1"},
  {0, 19, 218, "V6SV", "0-6km Shear V comp", "m s^-1"},
  {0, 19, 219, "U8SV", "0-8km Shear U comp", "m s^-1"},
  {0, 19, 220, "V8SV", "0-8km Shear V comp", "m s^-1"},
  {0, 19, 221, "PVOR", "H4-H25 Potential Vorticity Advection", "m s^-1"},
  {0, 19, 222, "LLLR", "Low Level Lapse Rate", "K km^-1"},
  {0, 19, 223, "TL75", "H7-H5 Temp Difference", "K"},
  {0, 19, 224, "M1CN", "Mean Layer Parcel CIN", "J kg^-1"},
  {0, 19, 225, "SBCN", "Surface Based Parcel CIN", "J kg^-1"},
  {0, 19, 226, "MLPH", "Lifted Parcel Height (Mean Layer Parcel)", "m"},
  {0, 19, 227, "MCAV", "Deep Moisture Flux Convergence", "g kg^-1"},
  {0, 19, 228, "FCRH", "Mean RH of LCL-LFC Layer", "%"},
  {0, 19, 229, "ESHR", "Effective Shear Magnitude", "M s^-1"},
  {0, 19, 230, "ALT2", "2hr Altimeter Change", "in 2hr^-1"},
  {0, 19, 231, "EDCP", "Derecho Composite Index", "m"},
};

/* Candian Meteorological Service - Montreal - Local Use Table, Center 54, SubCenter 2 */
/* 
   This list matches NCEP's but is only products we have seen from MSC
*/
const ProdDefTemp::_GRIB2LocalTable ProdDefTemp::_MSC_MONTREAL_localTable[] = {
  /* 4.2 : 0.4 Meteorological products, short-wave radiation category */
  {0, 4, 192, "DSWRF", "Downward Short-Wave Rad. Flux", "W m^-2"},        /* also 4.2 : 0.4 : 7  */
  {0, 4, 193, "USWRF", "Upward Short-Wave Radiation Flux", "W m^-2"},     /* also 4.2 : 0.4 : 8  */

  /* 4.2 : 0.5 Meteorological products, long-wave radiation category */
  {0, 5, 192, "DLWRF", "Downward Long-Wave Rad. Flux", "W m^-2"},         /* also 4.2 : 0.5 : 3  */
  {0, 5, 193, "ULWRF", "Upward Long-Wave Rad. Flux", "W m^-2"},           /* also 4.2 : 0.5 : 4  */
};

/* NOAA Forecast Systems Lab Local Use Table, Center 59, SubCenter 0 */
/* 
   This list matches NCEP's but is only products we have seen from FSL
*/
const ProdDefTemp::_GRIB2LocalTable ProdDefTemp::_NOAA_FSLlocalTable[] = {

  /* 4.2 : 0.0 Meteorological products, temperature category */
  {0, 0, 192, "SNOHF", "Snow Phase Change Heat Flux", "W m^-2"},    /* also 4.2 : 0.0 : 16  */
  {0, 0, 195, "LRGHR", "Large Scale Condensate Heat rate", "K s^-1"},

  /* 4.2 : 0.1  Meteorological products, moisture category */
  {0, 1, 192, "CRAIN", "Categorical Rain", "categorical"},            /* also 4.2 : 0.1 : 33  */
  {0, 1, 193, "CFRZR", "Categorical Freezing Rain", "categorical"},   /* also 4.2 : 0.1 : 34  */
  {0, 1, 194, "CICEP", "Categorical Ice Pellets", "categorical"},     /* also 4.2 : 0.1 : 35  */
  {0, 1, 195, "CSNOW", "Categorical Snow", "categorical"},            /* also 4.2 : 0.1 : 36  */
  {0, 1, 196, "CPRAT", "Convective Precipitation Rate", "kg s m^-2"},/* also 4.2 : 0.1 : 37  */
  {0, 1, 198, "MINRH", "Minimum Relative Humidity", "%"},
  {0, 1, 201, "SNOWC", "Snow Cover", "%"},                              /* also 4.2 : 0.1 : 42  */
  {0, 1, 207, "NCIP", "Number concentration for ice particles", "-"},
  {0, 1, 208, "SNOT", "Snow temperature", "K"},

  /* 4.2 : 0.3 Meteorological products, mass category */
  {0, 2, 192, "VWSH", "Vertical speed sheer", "s^-1"},                      /* also 4.2 : 0.2 : 25  */
  {0, 2, 194, "USTM", "U-Component Storm Motion", "m s^-1"},                  /* also 4.2 : 0.2 : 27  */
  {0, 2, 195, "VSTM", "V-Component Storm Motion", "m s^-1"},                  /* also 4.2 : 0.2 : 28  */
  {0, 2, 197, "FRICV", "Frictional Velocity", "m s^-1"},                      /* also 4.2 : 0.2 : 30  */

  /* 4.2 : 0.3 Meteorological products, mass category */
  {0, 3, 192, "MSLET", "Mean Sea Level Pressure (Eta Reduction)", "Pa"},
  {0, 3, 193, "ECHOT", "Echo top height of 18dBZ surface", "m"},           /* does not match NCEPs */
  {0, 3, 194, "VIL",   "Vertically integrated liquid,", "kg m^-2,"},        /* does not match NCEPs */
  {0, 3, 195, "RDVIL", "Radar-derived vertically integrated liquid", "kg m^-2,"}, /* does not match NCEPs */
  {0, 3, 196, "HPBL", "Planetary Boundary Layer Height", "m"},             /* also 4.2 : 0.3 : 18  */
  {0, 3, 197, "5WAVA", "5-Wave Geopotential Height Anomaly", "m"},       /* also 4.2 : 0.3 : 19  */
  {0, 3, 198, "MSLMA", "MSLP (MAPS System Reduction)", "Pa"},
  {0, 3, 200, "PLPL", "Pressure of level from which parcel was lifted", "Pa"},
  {0, 3, 206, "NLGSP", "Natural Log of Surface Pressure", "ln(kPa)"},

  /* 4.2 : 0.4 Meteorological products, short-wave radiation category */
  {0, 4, 192, "DSWRF", "Downward Short-Wave Rad. Flux", "W m^-2"},        /* also 4.2 : 0.4 : 7  */
  {0, 4, 193, "USWRF", "Upward Short-Wave Radiation Flux", "W m^-2"},     /* also 4.2 : 0.4 : 8  */

  /* 4.2 : 0.5 Meteorological products, long-wave radiation category */
  {0, 5, 192, "DLWRF", "Downward Long-Wave Rad. Flux", "W m^-2"},         /* also 4.2 : 0.5 : 3  */
  {0, 5, 193, "ULWRF", "Upward Long-Wave Rad. Flux", "W m^-2"},           /* also 4.2 : 0.5 : 4  */

  /* 4.2 : 0.7 Meteorological products, Thermodynamic Stability category */
  {0, 7, 192, "LFTX", "Surface Lifted Index", "K"},                        /* also 4.2 : 0.7 : 10  */
  {0, 7, 193, "4LFTX", "Best (4 layer) Lifted Index", "K"},                /* also 4.2 : 0.7 : 11  */

  /* 4.2 : 0.16 Meteorological products, Forecast Radar Imagery category */
  {0, 16, 195, "REFD", "Derived radar reflectivity", "dB"},
  {0, 16, 196, "REFC", "Maximum/Composite radar reflectivity", "dB"},

  /* 4.2 : 0.17 Meteorological products, Electrodynamics category */
  {0, 17, 192, "LTNG", "Lightning", "-"},

  /* 4.2 : 1.0 Hydrological products, Basic Hydrology category */
  {1, 0, 192, "BGRUN", "Baseflow-Groundwater Runoff", "kg m^-2"}, /* also 4.2 : 1.0 : 5  */
  {1, 0, 193, "SSRUN", "Storm Surface Runoff", "kg m^-2"},        /* also 4.2 : 1.0 : 6  */

  /* 4.2 : 2.0 Land Surface products, Vegetation/Biomass category */
  {2, 0, 192, "SOILW", "Volumetric Soil Moisture Content", "fraction"},    /* also 4.2 : 2.0 : 9  */
  {2, 0, 193, "GFLUX", "Ground Heat Flux", "W m^-2"},                     /* also 4.2 : 2.0 : 10  */
  {2, 0, 194, "MSTAV", "Moisture Availability", "%"},                      /* also 4.2 : 2.0 : 11  */
  {2, 0, 196, "CNWAT", "Plant Canopy Surface Water", "kg m^-2"},          /* also 4.2 : 2.0 : 13  */
  {2, 0, 198, "VGTYP", "Vegetation Type", "categorical"},
  {2, 0, 229, "EVCW", "Canopy water evaporation", "W m^-2"},
  {2, 0, 230, "TRANS", "Transpiration", "W m^-2"},

  /* 4.2 : 2.3  Land Surface products, Soil category */
  {2, 3, 198, "EVBS", "Direct evaporation from bare soil", "W m^-2"},

  /* 4.2 : 3.192 Satellite Remote Sensing products, Forecast Satellite Imagery */
  {3, 192, 2, "SBT124", "Simulated Brightness Temperature for GOES 12, Channel 4", "K"},

  /* 4.2 : 10.3 Oceanographic products, Surface Properties category */
  {10, 3, 192, "SURGE", "Storm Surge", "m"},
};


/* MRMS Local Use Table, Center 161, SubCenter 0, Discipline 209 */
/* 
   This list is from NOAAs listing of MRMS operation local use variables
   at http://www.nssl.noaa.gov/projects/mrms/operational/tables.php

*/
const ProdDefTemp::_GRIB2LocalTable ProdDefTemp::_MRMSlocalTable[] = {
  {209,2,0, "NLDN1min", "CG Lightning Density 1-min - NLDN", "flashes km^-2 min^-1"},
  {209,2,1, "NLDN5min", "CG Lightning Density 5-min - NLDN", "flashes km^-2 min^-1"},
  {209,2,2, "NLDN15min", "CG Lightning Density 15-min - NLDN", "flashes km^-2 min^-1"},
  {209,2,3, "NLDN30min", "CG Lightning Density 30-min - NLDN", "flashes km^-2 min^-1"},
  {209,2,4, "NLDNProbability30min", "Lightning Probability 0-30 minutes - NLDN", "%"},
  {209,3,0, "MergedAzShear0to2kmAGL", "Azimuth Shear 0-2km AGL", "0.001 s^-1"},
  {209,3,1, "MergedAzShear3to6kmAGL", "Azimuth Shear 3-6km AGL", "0.001 s^-1"},
  {209,3,2, "RotationTrack30min", "Rotation Track 0-2km AGL 30-min", "0.001 s^-1"},
  {209,3,3, "RotationTrack60min", "Rotation Track 0-2km AGL 60-min", "0.001 s^-1"},
  {209,3,4, "RotationTrack120min", "Rotation Track 0-2km AGL 120-min", "0.001 s^-1"},
  {209,3,5, "RotationTrack240min", "Rotation Track 0-2km AGL 240-min", "0.001 s^-1"},
  {209,3,6, "RotationTrack360min", "Rotation Track 0-2km AGL 360-min", "0.001 s^-1"},
  {209,3,7, "RotationTrack1440min", "Rotation Track 0-2km AGL 1440-min", "0.001 s^-1"},
  {209,3,14, "RotationTrackML30min", "Rotation Track 0-3km AGL 30-min", "0.001 s^-1"},
  {209,3,15, "RotationTrackML60min", "Rotation Track 0-3km AGL 60-min", "0.001 s^-1"},
  {209,3,16, "RotationTrackML120min", "Rotation Track 0-3km AGL 120-min", "0.001 s^-1"},
  {209,3,17, "RotationTrackML240min", "Rotation Track 0-3km AGL 240-min", "0.001 s^-1"},
  {209,3,18, "RotationTrackML360min", "Rotation Track 0-3km AGL 360-min", "0.001 s^-1"},
  {209,3,19, "RotationTrackML1440min", "Rotation Track 0-3km AGL 1440-min", "0.001 s^-1"},
  {209,3,26, "SHI", "Severe Hail Index", "index"},
  {209,3,27, "POSH", "Prob of Severe Hail", "%"},
  {209,3,28, "MESH", "Maximum Estimated Size of Hail (MESH)", "mm"},
  {209,3,29, "MESHMax30min", "MESH Hail Swath 30-min", "mm"},
  {209,3,30, "MESHMax60min", "MESH Hail Swath 60-min", "mm"},
  {209,3,31, "MESHMax120min", "MESH Hail Swath 120-min", "mm"},
  {209,3,32, "MESHMax240min", "MESH Hail Swath 240-min", "mm"},
  {209,3,33, "MESHMax360min", "MESH Hail Swath 360-min", "mm"},
  {209,3,34, "MESHMax1440min", "MESH Hail Swath 1440-min", "mm"},
  {209,3,41, "VIL", "Vertically Integrated Liquid", "kg m^-2"},
  {209,3,42, "VILD", "Vertically Integrated Liquid Density", "g m^-3"},
  {209,3,43, "VII", "Vertically Integrated Ice", "kg m^-2"},
  {209,3,44, "ETP18", "Echo Top - 18 dBZ", "km MSL"},
  {209,3,45, "ETP30", "Echo Top - 30 dBZ", "km MSL"},
  {209,3,46, "ETP50", "Echo Top - 50 dBZ", "km MSL"},
  {209,3,47, "ETP60", "Echo Top - 60 dBZ", "km MSL"},
  {209,3,48, "H50AboveM20C", "Thickness [50 dBZ top - (-20C)]", "km"},
  {209,3,49, "H50Above0C", "Thickness [50 dBZ top - 0C]", "km"},
  {209,3,50, "H60AboveM20C", "Thickness [60 dBZ top - (-20C)]", "km"},
  {209,3,51, "H60Above0C", "Thickness [60 dBZ top - 0C]", "km"},
  {209,3,52, "Reflectivity0C", "Isothermal Reflectivity at 0C", "dBZ"},
  {209,3,53, "ReflectivityM5C", "Isothermal Reflectivity at -5C", "dBZ"},
  {209,3,54, "ReflectivityM10C", "Isothermal Reflectivity at -10C", "dBZ"},
  {209,3,55, "ReflectivityM15C", "Isothermal Reflectivity at -15C", "dBZ"},
  {209,3,56, "ReflectivityM20C", "Isothermal Reflectivity at -20C", "dBZ"},
  {209,3,57, "ReflectivityAtLowestAltitude", "ReflectivityAtLowestAltitude", "dBZ"},
  {209,3,58, "MergedReflectivityAtLowestAltitude", "Non Quality Controlled Reflectivity At Lowest Altitude", "dBZ"},
  {209,4,0, "IRband4", "Infrared (E/W blend)", "K"},
  {209,4,1, "Visible", "Visible (E/W blend)", "non-dim"},
  {209,4,2, "WaterVapor", "Water Vapor (E/W blend)", "K"},
  {209,4,3, "CloudCover", "Cloud Cover", "K"},
  {209,6,0, "PCPFLAG", "Surface Precipitation Type (Convective 6, Stratiform 1-2 10, Tropical 91-96, Hail 7, Snow 3-4)", "categorical"},
  {209,6,1, "PRATE", "Radar Precipitation Rate", "mm hr^-1"},
  {209,6,2, "RadarOnlyQPE01H", "Radar precipitation accumulation 1-hour", "mm"},
  {209,6,3, "RadarOnlyQPE03H", "Radar precipitation accumulation 3-hour", "mm"},
  {209,6,4, "RadarOnlyQPE06H", "Radar precipitation accumulation 6-hour", "mm"},
  {209,6,5, "RadarOnlyQPE12H", "Radar precipitation accumulation 12-hour", "mm"},
  {209,6,6, "RadarOnlyQPE24H", "Radar precipitation accumulation 24-hour", "mm"},
  {209,6,7, "RadarOnlyQPE48H", "Radar precipitation accumulation 48-hour", "mm"},
  {209,6,8, "RadarOnlyQPE72H", "Radar precipitation accumulation 72-hour", "mm"},
  {209,6,9, "GaugeCorrQPE01H", "Local gauge bias corrected radar precipitation accumulation 1-hour", "mm"},
  {209,6,10, "GaugeCorrQPE03H", "Local gauge bias corrected radar precipitation accumulation 3-hour", "mm"},
  {209,6,11, "GaugeCorrQPE06H", "Local gauge bias corrected radar precipitation accumulation 6-hour", "mm"},
  {209,6,12, "GaugeCorrQPE12H", "Local gauge bias corrected radar precipitation accumulation 12-hour", "mm"},
  {209,6,13, "GaugeCorrQPE24H", "Local gauge bias corrected radar precipitation accumulation 24-hour", "mm"},
  {209,6,14, "GaugeCorrQPE48H", "Local gauge bias corrected radar precipitation accumulation 48-hour", "mm"},
  {209,6,15, "GaugeCorrQPE72H", "Local gauge bias corrected radar precipitation accumulation 72-hour", "mm"},
  {209,6,16, "GaugeOnlyQPE01H", "Gauge only precipitation accumulation 1-hour", "mm"},
  {209,6,17, "GaugeOnlyQPE03H", "Gauge only precipitation accumulation 3-hour", "mm"},
  {209,6,18, "GaugeOnlyQPE06H", "Gauge only precipitation accumulation 6-hour", "mm"},
  {209,6,19, "GaugeOnlyQPE12H", "Gauge only precipitation accumulation 12-hour", "mm"},
  {209,6,20, "GaugeOnlyQPE24H", "Gauge only precipitation accumulation 24-hour", "mm"},
  {209,6,21, "GaugeOnlyQPE48H", "Gauge only precipitation accumulation 48-hour", "mm"},
  {209,6,22, "GaugeOnlyQPE72H", "Gauge only precipitation accumulation 72-hour", "mm"},
  {209,6,23, "MountainMapperQPE01H", "Mountain Mapper precipitation accumulation 1-hour", "mm"},
  {209,6,24, "MountainMapperQPE03H", "Mountain Mapper precipitation accumulation 3-hour", "mm"},
  {209,6,25, "MountainMapperQPE06H", "Mountain Mapper precipitation accumulation 6-hour", "mm"},
  {209,6,26, "MountainMapperQPE12H", "Mountain Mapper precipitation accumulation 12-hour", "mm"},
  {209,6,27, "MountainMapperQPE24H", "Mountain Mapper precipitation accumulation 24-hour", "mm"},
  {209,6,28, "MountainMapperQPE48H", "Mountain Mapper precipitation accumulation 48-hour", "mm"},
  {209,6,29, "MountainMapperQPE72H", "Mountain Mapper precipitation accumulation 72-hour", "mm"},
  {209,7,0, "ModelSurfaceTemp", "Model Surface temperature [RAP 13km]", "C"},
  {209,7,1, "ModelWetBulbTemp", "Model Surface wet bulb temperature [RAP 13km]", "C"},
  {209,7,2, "WarmRainProbability", "Probability of warm rain [RAP 13km derived]", "%"},
  {209,7,3, "ModelHeight0C", "Model Freezing Level Height [RAP 13km]", "m MSL"},
  {209,7,4, "BrightBandTopHeight", "Brightband Top Radar [RAP 13km derived]", "m AGL"},
  {209,7,5, "BrightBandBottomHeight", "Brightband Bottom Radar [RAP 13km derived]", "m AGL"},
  {209,8,0, "RadarQualityIndex", "Radar Quality Index", "non-dim"},
  {209,8,1, "GaugeInflIndex01H", "Gauge Influence Index for 1-hour QPE", "non-dim"},
  {209,8,2, "GaugeInflIndex03H", "Gauge Influence Index for 3-hour QPE", "non-dim"},
  {209,8,3, "GaugeInflIndex06H", "Gauge Influence Index for 6-hour QPE", "non-dim"},
  {209,8,4, "GaugeInflIndex12H", "Gauge Influence Index for 12-hour QPE", "non-dim"},
  {209,8,5, "GaugeInflIndex24H", "Gauge Influence Index for 24-hour QPE", "non-dim"},
  {209,8,6, "GaugeInflIndex48H", "Gauge Influence Index for 48-hour QPE", "non-dim"},
  {209,8,7, "GaugeInflIndex72H", "Gauge Influence Index for 72-hour QPE", "non-dim"},
  {209,8,8, "SHSR", "Seamless Hybrid Scan Reflectivity with VPR correction", "dBZ"},
  {209,8,9, "SHSRH", "Height of Seamless Hybrid Scan Reflectivity", "km AGL"},
  {209,9,0, "MREF", "WSR-88D 3D Reflectivty Mosaic", "dBZ"},
  {209,9,1, "MREF", "All Radar 3D Reflectivty Mosaic", "dBZ"},
  {209,10,0, "CREF", "Composite Reflectivity Mosaic (optimal method)", "dBZ"},
  {209,10,1, "CREFH", "Height of Composite Reflectivity Mosaic (optimal method)", "m MSL"},
  {209,10,2, "LCREF", "Low-Level Composite Reflectivity Mosaic (0-4km)", "dBZ"},
  {209,10,3, "LCREFH", "Height of Low-Level Composite Reflectivity Mosaic (0-4km)", "m MSL"},
  {209,10,4, "LCR_LOW", "Layer Composite Reflectivity Mosaic 0-24kft (low altitude)", "dBZ"},
  {209,10,5, "LCR_HIGH", "Layer Composite Reflectivity Mosaic 24-60 kft (highest altitude)", "dBZ"},
  {209,10,6, "LCR_SUSER", "Layer Composite Reflectivity Mosaic 33-60 kft (super high altitude)", "dBZ"},
  {209,10,7, "CREFMAX", "Composite Reflectivity Hourly Maximum", "dBZ"},
  {209,10,8, "CREFM10C", "Maximum Reflectivity at -10 deg C height and above", "dBZ"},
  {209,11,0, "BASE_REFL", "Mosaic Base Reflectivity (optimal method)", "dBZ"},
  {209,11,1, "CREF", "UnQcd Composite Reflectivity Mosaic (max ref)", "dBZ"},
  {209,11,2, "CREF", "Composite Reflectivity Mosaic (max ref)", "dBZ"}
};

/* COSMO - Local Use Table, Center 250, SubCenter 2 */
const ProdDefTemp::_GRIB2LocalTable ProdDefTemp::_COSMO_localTable[] = {
  {192, 0, 0,   "TDEF", "Total Deformation", "s^-1"},
  {0, 1, 197,   "TUHFLX", "Total Upward Heat Flux", "W m^-2"},
  {0, 191, 207, "BLH", "Boundary Layer Height", "m"},
  {2, 0, 203,   "IMOL", "Inverse of Monin-Obukov length", "-"},
  {0, 2, 207,   "USTAR", "U-star", "-"},
  {2, 0, 208,   "VOROG", "Variance of Orography", "-"},
  {2, 0, 206,   "RHEIGHT", "Roughness Height", "-"},
};

/* MISSING - Local Use Table, Center 255, SubCenter 255 */
const ProdDefTemp::_GRIB2LocalTable ProdDefTemp::_MISSING_localTable[] = {
  {0, 1, 197,   "TUHFLX", "Total Upward Heat Flux", "W m^-2"},
  {0, 2, 207,   "USTAR", "U-star", "-"},
  {0, 191, 207, "BLH", "Boundary Layer Height", "m"},
  {2, 0, 206,   "RHEIGHT", "Roughness Height", "-"},
  {2, 0, 203,   "IMOL", "Inverse of Monin-Obukov length", "-"},
  {2, 0, 208,   "VOROG", "Variance of Orography", "-"},
  {192, 0, 0,    "TDEF", "Total Deformation", "s^-1"},
  {192, 0, 10,   "LOGTGRAD", "", "K m^-1"},
  {192, 0, 12,   "LOGVORTGRAD", "", "m^-1 s^-1"},
  {192, 0, 14,   "LOGDELSQW", "", "m^-1 s^-1"},
  {192, 0, 15,   "CNDHEAT", "", "K s^-1"},
  {192, 0, 16,   "CNDHEATI", "", "K s^-1"},
  {192, 0, 20,   "LOGTKE", "", "m^2 s^-2"},
  {192, 1, 13,   "NEGRITEND", "", "-"},
  {192, 3, 1,    "VWS", "Vertical Wind Speed", "s^-1"},
  {192, 3, 2,    "LOGINVRI", "", "-"},
  {192, 3, 4,    "LOGINVRITW", "", "-"},
  {192, 3, 9,    "LOGIAWIND", "", "m s^-1"},
  {192, 3, 27,   "LOGNCSU2", "", "s^-3"},
  {192, 3, 28,   "LOGPVGRAD", "", "PVU km^-1"},
  {192, 3, 33,   "LOGLHFK", "", "m^1/2 s^-1/2"},
  {192, 3, 37,   "LOGF2D", "", "K m^-1 s^-1"},
  {192, 3, 41,   "LOGEDRLL", "", "m^2/3 s^-1"},
  {192, 3, 42,   "LOGEDRAVG", "", "m^2/3 s^-1"},
  {192, 3, 44,   "LOGSIGWAVG", "", "m^2/3 s^-1"},
  {192, 3, 45,   "LOGCTSQ", "", "m^2/3 s^-1"},
  {192, 3, 46,   "LOGMSEDR", "", "m^2/3 s^-1"},
  {192, 3, 47,   "LOGPBLEDR", "", "m^2/3 s^-1"},
};

/**************************************************************************
 * Constructor.
 */

ProdDefTemp::ProdDefTemp() 
{
  _parameterLongName = NULL;
  _parameterName = NULL;
  _parameterUnits = NULL;
  _processID = 255;
  _newCenterFound = 0;
}

ProdDefTemp::ProdDefTemp(Grib2Record::Grib2Sections_t sectionsPtr) 
{
  setSectionsPtr(sectionsPtr);
  _parameterLongName = NULL;
  _parameterName = NULL;
  _parameterUnits = NULL;
  _processID = 255;
  _newCenterFound = 0;
}

/**************************************************************************
 * Destructor
 */

ProdDefTemp::~ProdDefTemp() 
{
  if(_parameterLongName != NULL)
    delete _parameterLongName;
  if(_parameterName != NULL)
    delete _parameterName;
  if(_parameterUnits != NULL)
    delete _parameterUnits;

  _parameterLongName = NULL;
  _parameterName = NULL;
  _parameterUnits = NULL;
}

void ProdDefTemp::setSectionsPtr(Grib2Record::Grib2Sections_t sectionsPtr)
{
  _sectionsPtr = sectionsPtr;
  _centerId = _sectionsPtr.ids->getCenterId();
  _subCenterId = _sectionsPtr.ids->getSubCenterId();
  _disciplineNum = _sectionsPtr.is->getDisciplineNum();
}

void ProdDefTemp::setParamStrings () 
{
  if(_parameterLongName != NULL)
    delete _parameterLongName;
  if(_parameterName != NULL)
    delete _parameterName;
  if(_parameterUnits != NULL)
    delete _parameterUnits;

  _parameterLongName = NULL;
  _parameterName = NULL;
  _parameterUnits = NULL;

  if (_paramNumber >= 255 || _parameterCategory >= 255) 
  {
     _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
     _parameterLongName = new string ("Unknown Parameter");
     _parameterUnits = new string ("-");

  // Parameter categories 192 through 254 are Local Use Only
  // This is constant among the various parameter Disciplines
  } else if (_paramNumber >= 192 || _parameterCategory >= 192 || _disciplineNum >= 192) 
  {

     // override defaults with local values where appropriate
     switch (_centerId) {
        // NCEP local tables
        case 7:
	  // Sub-center values for Center 7, US National Centers for Environmental Prediction 
	  //1	NCEP Re-Analysis Project
	  //2	NCEP Ensemble Products
	  //3	NCEP Central Operations
	  //4	Environmental Modeling Center
	  //5	Hydrometeorological Prediction Center
	  //6	Marine Prediction Center
	  //7	Climate Prediction Center
	  //8	Aviation Weather Center
	  //9	Storm Prediction Center
	  //10	National Hurricane Prediction Center
	  //11	NWS Techniques Development Laboratory
	  //12	NESDIS Office of Research and Applications
	  //13	Federal Aviation Administration
	  //14	NWS Meteorological Development Laboratory
	  //15	North American Regional Reanalysis Project
	  //16	Space Weather Prediction Center
          if (_subCenterId == 0 || _subCenterId == 1 ||_subCenterId == 2 || _subCenterId == 3) {
            // set local parameter information

            for (ui32 i = 0; i < _NCEPlocalTable_numElements; i++ ) {

               if ((_NCEPlocalTable[i].prodDiscipline == _disciplineNum) && 
                         (_parameterCategory == _NCEPlocalTable[i].category) &&
                         (_paramNumber == _NCEPlocalTable[i].paramNumber)) {
                    _parameterName = new string (_NCEPlocalTable[i].name);   
                    _parameterLongName = new string (_NCEPlocalTable[i].comment);
                    _parameterUnits = new string (_NCEPlocalTable[i].unit);
                    break;
               }
            } // for
          }  // NCEP / Environmental Modeling Center (EMC), for now same as basic NCEP local tables
	  else if (_subCenterId == 4) {
            for (ui32 i = 0; i < _NCEPlocalTable_numElements; i++ ) {

               if ((_NCEPlocalTable[i].prodDiscipline == _disciplineNum) && 
                         (_parameterCategory == _NCEPlocalTable[i].category) &&
                         (_paramNumber == _NCEPlocalTable[i].paramNumber)) {
                    _parameterName = new string (_NCEPlocalTable[i].name);   
                    _parameterLongName = new string (_NCEPlocalTable[i].comment);
                    _parameterUnits = new string (_NCEPlocalTable[i].unit);
                    break;
               }
            } // for
          }  // NCEP / Aviation Weather Center (AWC) local tables, for now same as basic NCEP local tables
	  else if (_subCenterId == 8) {
            for (ui32 i = 0; i < _NCEPlocalTable_numElements; i++ ) {

               if ((_NCEPlocalTable[i].prodDiscipline == _disciplineNum) && 
                         (_parameterCategory == _NCEPlocalTable[i].category) &&
                         (_paramNumber == _NCEPlocalTable[i].paramNumber)) {
                    _parameterName = new string (_NCEPlocalTable[i].name);   
                    _parameterLongName = new string (_NCEPlocalTable[i].comment);
                    _parameterUnits = new string (_NCEPlocalTable[i].unit);
                    break;
               }
            } // for
	  } // NCEP / MDL (Meteorological Developmental Lab) local tables, for now same as basic NCEP local tables
	  else if (_subCenterId == 14) {
            for (ui32 i = 0; i < _NCEPlocalTable_numElements; i++ ) {

               if ((_NCEPlocalTable[i].prodDiscipline == _disciplineNum) && 
                         (_parameterCategory == _NCEPlocalTable[i].category) &&
                         (_paramNumber == _NCEPlocalTable[i].paramNumber)) {
                    _parameterName = new string (_NCEPlocalTable[i].name);   
                    _parameterLongName = new string (_NCEPlocalTable[i].comment);
                    _parameterUnits = new string (_NCEPlocalTable[i].unit);
                    break;
               }
            } // for
	  } // SPC (Storm Prediction Center) local tables
	  else if (_subCenterId == 9 || _subCenterId == 255) {
            for (ui32 i = 0; i < _SPClocalTable_numElements; i++ ) {

               if ((_SPClocalTable[i].prodDiscipline == _disciplineNum) && 
                         (_parameterCategory == _SPClocalTable[i].category) &&
                         (_paramNumber == _SPClocalTable[i].paramNumber)) {
                    _parameterName = new string (_SPClocalTable[i].name.c_str());   
                    _parameterLongName = new string (_SPClocalTable[i].comment.c_str());
                    _parameterUnits = new string (_SPClocalTable[i].unit.c_str());
                    break;
               }
            } // for
	  }
          else {
	    if(_newCenterFound == 0) {
	      cerr << "ERROR: ProdDefTemp::setParamStrings()" << endl;
	      cerr << "New sub-center found for NCEP, cannot decode local parameter numbers" << endl;
	    }
	    _newCenterFound = 1;
	  }
          break;

	// NWS Local Tables
        case 8:  
	  if(_newCenterFound == 0) {
	    cerr << "ProdDefTemp::setParamStrings()" << endl;
	    cerr << "NWS Telecomunications gateway local parameter tables not implemented" << endl;
	  }
	  _newCenterFound = 1;
          break;

       // Meteorological Service of Canada
       case 54:
	  // Montreal
          if (_subCenterId == 2 || _subCenterId == 0) {
            for (ui32 i = 0; i < _MSC_MONTREAL_localTable_numElements; i++ ) {

               if ((_MSC_MONTREAL_localTable[i].prodDiscipline == _disciplineNum) && 
                         (_parameterCategory == _MSC_MONTREAL_localTable[i].category) &&
                         (_paramNumber == _MSC_MONTREAL_localTable[i].paramNumber)) {
                    _parameterName = new string (_MSC_MONTREAL_localTable[i].name);   
                    _parameterLongName = new string (_MSC_MONTREAL_localTable[i].comment);
                    _parameterUnits = new string (_MSC_MONTREAL_localTable[i].unit);
                    break;
               }
            } // for
	    if(_parameterName == NULL) {
	      for (ui32 i = 0; i < _NCEPlocalTable_numElements; i++ ) {
		if ((_NCEPlocalTable[i].prodDiscipline == _disciplineNum) && 
		    (_parameterCategory == _NCEPlocalTable[i].category) &&
		    (_paramNumber == _NCEPlocalTable[i].paramNumber)) {
		  _parameterName = new string (_NCEPlocalTable[i].name);   
		  _parameterLongName = new string (_NCEPlocalTable[i].comment);
		  _parameterUnits = new string (_NCEPlocalTable[i].unit);
		  break;
		}
	      } // for
	    }

          }
          else {
	    if(_newCenterFound == 0) {
	      cerr << "ERROR: ProdDefTemp::setParamStrings()" << endl;
	      cerr << "New sub-center found for NOAA FSL, cannot decode local parameter numbers" << endl;
	    }
	    _newCenterFound = 1;
	  }
          break;



	// NOAA Forecast Systems Lab
        case 59:
	  // Global Systems Division (GSD)
          if (_subCenterId == 0) {
            for (ui32 i = 0; i < _NOAA_FSLlocalTable_numElements; i++ ) {

               if ((_NOAA_FSLlocalTable[i].prodDiscipline == _disciplineNum) && 
                         (_parameterCategory == _NOAA_FSLlocalTable[i].category) &&
                         (_paramNumber == _NOAA_FSLlocalTable[i].paramNumber)) {
                    _parameterName = new string (_NOAA_FSLlocalTable[i].name);   
                    _parameterLongName = new string (_NOAA_FSLlocalTable[i].comment);
                    _parameterUnits = new string (_NOAA_FSLlocalTable[i].unit);
                    break;
               }
            } // for
	    if(_parameterName == NULL) {
	      for (ui32 i = 0; i < _NCEPlocalTable_numElements; i++ ) {
		if ((_NCEPlocalTable[i].prodDiscipline == _disciplineNum) && 
		    (_parameterCategory == _NCEPlocalTable[i].category) &&
		    (_paramNumber == _NCEPlocalTable[i].paramNumber)) {
		  _parameterName = new string (_NCEPlocalTable[i].name);   
		  _parameterLongName = new string (_NCEPlocalTable[i].comment);
		  _parameterUnits = new string (_NCEPlocalTable[i].unit);
		  break;
		}
	      } // for
	    }

          }
          else {
	    if(_newCenterFound == 0) {
	      cerr << "ERROR: ProdDefTemp::setParamStrings()" << endl;
	      cerr << "New sub-center found for NOAA FSL, cannot decode local parameter numbers" << endl;
	    }
	    _newCenterFound = 1;
	  }
          break;

        // NCAR local tables
        case 60:
	  // Research Aplications Laboratory local table
          if (_subCenterId == 0) {
            for (ui32 i = 0; i < _NCAR_RALlocalTable_numElements; i++ ) {

               if ((_NCAR_RALlocalTable[i].prodDiscipline == _disciplineNum) && 
                         (_parameterCategory == _NCAR_RALlocalTable[i].category) &&
                         (_paramNumber == _NCAR_RALlocalTable[i].paramNumber)) {
                    _parameterName = new string (_NCAR_RALlocalTable[i].name);   
                    _parameterLongName = new string (_NCAR_RALlocalTable[i].comment);
                    _parameterUnits = new string (_NCAR_RALlocalTable[i].unit);
                    break;
               }
            } // for
          }
          else {
	    if(_newCenterFound == 0) {
	      cerr << "ERROR: ProdDefTemp::setParamStrings()" << endl;
	      cerr << "New sub-center found for NCAR, cannot decode local parameter numbers" << endl;
	    }
	    _newCenterFound = 1;
	  }
          break;

        // US NOAA Office of Oceanic and Atmospheric Research
        case 161:
	  // Subcenter: 0 (National Severe Storms Lab)
	  // Operational MRMS GRIB2 Tables
	  if(_subCenterId == 0) {
	    for (ui32 i = 0; i < _MRMSlocalTable_numElements; i++ ) {
	      
	      if ((_MRMSlocalTable[i].prodDiscipline == _disciplineNum) && 
		  (_parameterCategory == _MRMSlocalTable[i].category) &&
		  (_paramNumber == _MRMSlocalTable[i].paramNumber)) {
		_parameterName = new string (_MRMSlocalTable[i].name);   
		_parameterLongName = new string (_MRMSlocalTable[i].comment);
		_parameterUnits = new string (_MRMSlocalTable[i].unit);
		break;
	      }
	    } // for
	  }
	  break;
	  
        // COSMO local tables
        case 250:
	  // COnsortium for Small scall MOdelling (COSMO) local table
          if (_subCenterId == 2) {
            for (ui32 i = 0; i < _COSMO_localTable_numElements; i++ ) {

               if ((_COSMO_localTable[i].prodDiscipline == _disciplineNum) && 
                         (_parameterCategory == _COSMO_localTable[i].category) &&
                         (_paramNumber == _COSMO_localTable[i].paramNumber)) {
                    _parameterName = new string (_COSMO_localTable[i].name);   
                    _parameterLongName = new string (_COSMO_localTable[i].comment);
                    _parameterUnits = new string (_COSMO_localTable[i].unit);
                    break;
               }
            } // for
          }
          else {
	    if(_newCenterFound == 0) {
	      cerr << "ERROR: ProdDefTemp::setParamStrings()" << endl;
	      cerr << "New sub-center found for COSMO, cannot decode local parameter numbers" << endl;
	    }
	    _newCenterFound = 1;
	  }
          break;

       // Catch-All for Missing Local Use Table Usage
       case 255:
          if (_subCenterId == 255) {
            for (ui32 i = 0; i < _MISSING_localTable_numElements; i++ ) {

               if ((_MISSING_localTable[i].prodDiscipline == _disciplineNum) && 
                         (_parameterCategory == _MISSING_localTable[i].category) &&
                         (_paramNumber == _MISSING_localTable[i].paramNumber)) {
                    _parameterName = new string (_MISSING_localTable[i].name);   
                    _parameterLongName = new string (_MISSING_localTable[i].comment);
                    _parameterUnits = new string (_MISSING_localTable[i].unit);
                    break;
               }
            } // for
          }
          break;

      default:
	if(_newCenterFound == 0) {
	  cerr << "ERROR: ProdDefTemp::setParamStrings()" << endl;
	  cerr << "New center ID found, cannot decode local parameter numbers" << endl;
	}
	_newCenterFound = 1;
	break;
     }  // switch


     if(_parameterName == NULL) {
       // set default values
       _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
       _parameterLongName = new string ("Unknown Paramater");
       _parameterUnits = new string ("-");
     }

  }
  else 
  {
    switch (_disciplineNum)
    {
    case 0:
      // Product Discipline 0, Meteorological products

      switch (_parameterCategory) 
      {
      case 0:
        // GRIB2 Code table 4.2 : 0.0
 	// parameterCategory is temperature
        if (_paramNumber < (si32) sizeof (_meteoTemp) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_meteoTemp[_paramNumber].name);
	  _parameterLongName = new string (_meteoTemp[_paramNumber].comment);
	  _parameterUnits = new string (_meteoTemp[_paramNumber].unit);
        }
        break;

      case 1:
        // GRIB2 Code table 4.2 : 0.1 
 	// parameterCategory is Moisture
        if (_paramNumber < (si32) sizeof (_meteoMoist) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoMoist[_paramNumber].name);
          _parameterLongName = new string (_meteoMoist[_paramNumber].comment);
          _parameterUnits = new string (_meteoMoist[_paramNumber].unit);
        }
        break;

      case 2:
        // GRIB2 Code table 4.2 : 0.2         
 	// parameterCategory is Momentum
        if (_paramNumber < (si32) sizeof (_meteoMoment) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoMoment[_paramNumber].name);
          _parameterLongName = new string (_meteoMoment[_paramNumber].comment);
          _parameterUnits = new string (_meteoMoment[_paramNumber].unit);
        }
        break;

     case 3:
        // GRIB2 Code table 4.2 : 0.3
 	// parameterCategory is Mass
        if (_paramNumber < (si32) sizeof (_meteoMass) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoMass[_paramNumber].name);
          _parameterLongName = new string (_meteoMass[_paramNumber].comment);
          _parameterUnits = new string (_meteoMass[_paramNumber].unit);
        }
        break;

     case 4:
        // GRIB2 Code table 4.2 : 0.4
 	// parameterCategory is Short-wave Radiation
        if (_paramNumber < (si32) sizeof (_meteoShortRadiate) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoShortRadiate[_paramNumber].name);
          _parameterLongName =new string (_meteoShortRadiate[_paramNumber].comment);
          _parameterUnits = new string (_meteoShortRadiate[_paramNumber].unit);
        }
        break;

     case 5:
        // GRIB2 Code table 4.2 : 0.5
 	// parameterCategory is Long-wave Radiation
        if (_paramNumber < (si32) sizeof (_meteoLongRadiate) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoLongRadiate[_paramNumber].name);
          _parameterLongName =new string (_meteoLongRadiate[_paramNumber].comment);
          _parameterUnits = new string (_meteoLongRadiate[_paramNumber].unit);
        }
        break;

     case 6:
        // GRIB2 Code table 4.2 : 0.6 
 	// parameterCategory is Cloud
        if (_paramNumber < (si32) sizeof (_meteoCloud) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoCloud[_paramNumber].name);
          _parameterLongName =new string (_meteoCloud[_paramNumber].comment);
          _parameterUnits = new string (_meteoCloud[_paramNumber].unit);
        }
        break;

     case 7:
        // GRIB2 Code table 4.2 : 0.7
 	// parameterCategory is Thermodynamic Stability indices
        if (_paramNumber < (si32) sizeof (_meteoStability) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoStability[_paramNumber].name);
          _parameterLongName =new string (_meteoStability[_paramNumber].comment);
          _parameterUnits = new string (_meteoStability[_paramNumber].unit);
        }
        break;

#ifdef NOTNOW
     case 8:
        // GRIB2 Code table 4.2 : 0.8
 	// parameterCategory is Kinematic Stability indices
        break;
     case 9:
        // GRIB2 Code table 4.2 : 0.9 
 	// parameterCategory is Temperature Probabilities
        break;
     case 10:
        // GRIB2 Code table 4.2 : 0.10 
 	// parameterCategory is Moisture Probabilities
        break;
     case 11:
        // GRIB2 Code table 4.2 : 0.11
 	// parameterCategory is Momentum Probabilities
        break;
     case 12:
        // GRIB2 Code table 4.2 : 0.12
 	// parameterCategory is Mass Probabilities
        break;
#endif
     case 13:
        // GRIB2 Code table 4.2 : 0.13
 	// parameterCategory is Aerosols
        if (_paramNumber < (si32) sizeof (_meteoAerosols) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoAerosols[_paramNumber].name);
          _parameterLongName =new string (_meteoAerosols[_paramNumber].comment);
          _parameterUnits = new string (_meteoAerosols[_paramNumber].unit);
        }
        break;

     case 14:
        // GRIB2 Code table 4.2 : 0.14
 	// parameterCategory is Trace gases (e.g., ozone, CO2)
        if (_paramNumber < (si32) sizeof (_meteoGases) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoGases[_paramNumber].name);
          _parameterLongName =new string (_meteoGases[_paramNumber].comment);
          _parameterUnits = new string (_meteoGases[_paramNumber].unit);
        }
        break;

     case 15:
        // GRIB2 Code table 4.2 : 0.15
 	// parameterCategory is Radar
        if (_paramNumber < (si32) sizeof (_meteoRadar) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoRadar[_paramNumber].name);
          _parameterLongName =new string (_meteoRadar[_paramNumber].comment);
          _parameterUnits = new string (_meteoRadar[_paramNumber].unit);
        }
        break;

     case 16:
        // GRIB2 Code table 4.2 : 0.16
 	// parameterCategory is Forecast Radar Imagery
        if (_paramNumber < (si32) sizeof (_meteoRadarForecast) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoRadarForecast[_paramNumber].name);
          _parameterLongName =new string (_meteoRadarForecast[_paramNumber].comment);
          _parameterUnits = new string (_meteoRadarForecast[_paramNumber].unit);
        }
        break;

     case 17:
        // GRIB2 Code table 4.2 : 0.17
 	// parameterCategory is Electro-dynamics
        if (_paramNumber < (si32) sizeof (_meteoElectro) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoElectro[_paramNumber].name);
          _parameterLongName =new string (_meteoElectro[_paramNumber].comment);
          _parameterUnits = new string (_meteoElectro[_paramNumber].unit);
        }
        break;

     case 18:
        // GRIB2 Code table 4.2 : 0.18
 	// parameterCategory is Nuclear/radiology
        if (_paramNumber < (si32) sizeof (_meteoNuclear) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoNuclear[_paramNumber].name);
          _parameterLongName =new string (_meteoNuclear[_paramNumber].comment);
          _parameterUnits = new string (_meteoNuclear[_paramNumber].unit);
        }
        break;

     case 19:
        // GRIB2 Code table 4.2 : 0.19 
 	// parameterCategory is Physical atmospheric properties
        if (_paramNumber < (si32) sizeof (_meteoAtmos) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoAtmos[_paramNumber].name);
          _parameterLongName =new string (_meteoAtmos[_paramNumber].comment);
          _parameterUnits = new string (_meteoAtmos[_paramNumber].unit);
        }
        break;

     case 20:
        // GRIB2 Code table 4.2 : 0.20 
 	// parameterCategory is Atmospheric Chemical Constituents
        if (_paramNumber < (si32) sizeof (_meteoChem) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoChem[_paramNumber].name);
          _parameterLongName =new string (_meteoChem[_paramNumber].comment);
          _parameterUnits = new string (_meteoChem[_paramNumber].unit);
        }
        break;

     case 190:
        // GRIB2 Code table 4.2 : 0.190 
 	// parameterCategory is Arbitrary text string
        if (_paramNumber < (si32) sizeof (_meteoText) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoText[_paramNumber].name);
          _parameterLongName =new string (_meteoText[_paramNumber].comment);
          _parameterUnits = new string (_meteoText[_paramNumber].unit);
        }
        break;

     case 191:
        // GRIB2 Code table 4.2 : 0.191 
 	// parameterCategory is Miscellaneous
        if (_paramNumber < (si32) sizeof (_meteoMisc) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_meteoMisc[_paramNumber].name);
          _parameterLongName =new string (_meteoMisc[_paramNumber].comment);
          _parameterUnits = new string (_meteoMisc[_paramNumber].unit);
        }
        break;

      case 255:
 	// parameterCategory is Missing
        _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
        _parameterLongName =new string ("Unknown Parameter");
        _parameterUnits = new string ("-");	
        break;

      default:
        _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
        _parameterLongName =new string ("Unknown Parameter");
        _parameterUnits = new string ("-");
      } // end switch (parameterCategory) 

      break;   // end Discipline 0

    case 1:
      // Product Discipline 1, Hydrologic products

      switch (_parameterCategory) 
      {
      case 0:
        // GRIB2 Code table 4.2 : 1.0
 	// Hydrology basic products
        if (_paramNumber < (si32) sizeof (_hydroBasic) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_hydroBasic[_paramNumber].name);
	  _parameterLongName = new string (_hydroBasic[_paramNumber].comment);
	  _parameterUnits = new string (_hydroBasic[_paramNumber].unit);
        }
        break;

      case 1:
        // GRIB2 Code table 4.2 : 1.1 
 	// Hydrology probabilities
        if (_paramNumber < (si32) sizeof (_hydroProb) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_hydroProb[_paramNumber].name);
          _parameterLongName = new string (_hydroProb[_paramNumber].comment);
          _parameterUnits = new string (_hydroProb[_paramNumber].unit);
        }
        break;

      case 2:
        // GRIB2 Code table 4.2 : 1.2
 	// Hydrology Inland water and sediment properties
        if (_paramNumber < (si32) sizeof (_hydroWaterSediment) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_hydroWaterSediment[_paramNumber].name);
          _parameterLongName = new string (_hydroWaterSediment[_paramNumber].comment);
          _parameterUnits = new string (_hydroWaterSediment[_paramNumber].unit);
        }
        break;

      default:
        _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
        _parameterLongName =new string ("Unknown Parameter");
        _parameterUnits = new string ("-");
      } // end switch (parameterCategory) 
      break;

    case 2:
      // Product Discipline 2, Land Surface products

      switch (_parameterCategory) 
      {
      case 0:
        // GRIB2 Code table 4.2 : 2.0
 	// Vegetation/Biomass
        if (_paramNumber < (si32) sizeof (_landVeg) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_landVeg[_paramNumber].name);
	  _parameterLongName = new string (_landVeg[_paramNumber].comment);
	  _parameterUnits = new string (_landVeg[_paramNumber].unit);
        }
        break;

      case 3:
        // GRIB2 Code table 4.2 : 2.3 
 	// Soil products
        if (_paramNumber < (si32) sizeof (_landSoil) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_landSoil[_paramNumber].name);
          _parameterLongName = new string (_landSoil[_paramNumber].comment);
          _parameterUnits = new string (_landSoil[_paramNumber].unit);
        }
        break;

      case 4:
        // GRIB2 Code table 4.2 : 2.4 
 	// Fire Weather products
        if (_paramNumber < (si32) sizeof (_landFire) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_landFire[_paramNumber].name);
          _parameterLongName = new string (_landFire[_paramNumber].comment);
          _parameterUnits = new string (_landFire[_paramNumber].unit);
        }
        break;

      default:
        _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
        _parameterLongName =new string ("Unknown Parameter");
        _parameterUnits = new string ("-");
      } // end switch (parameterCategory) 
      break;

    case 3:
      // Product Discipline 3, Satellite Remote Sensing products

      switch (_parameterCategory) 
      {
      case 0:
        // GRIB2 Code table 4.2 : 3.0
 	// Image format products
        if (_paramNumber < (si32) sizeof (_satelliteImage) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_satelliteImage[_paramNumber].name);
	  _parameterLongName = new string (_satelliteImage[_paramNumber].comment);
	  _parameterUnits = new string (_satelliteImage[_paramNumber].unit);
        }
        break;

      case 1:
        // GRIB2 Code table 4.2 : 3.1 
 	// Quantitative products
        if (_paramNumber < (si32) sizeof (_satelliteQuantitative) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_satelliteQuantitative[_paramNumber].name);
          _parameterLongName = new string (_satelliteQuantitative[_paramNumber].comment);
          _parameterUnits = new string (_satelliteQuantitative[_paramNumber].unit);
        }
        break;

      case 2:
        // GRIB2 Code table 4.2 : 3.2
 	// Cloud products
        if (_paramNumber < (si32) sizeof (_satelliteCloud) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_satelliteCloud[_paramNumber].name);
          _parameterLongName = new string (_satelliteCloud[_paramNumber].comment);
          _parameterUnits = new string (_satelliteCloud[_paramNumber].unit);
        }
        break;

      case 3:
        // GRIB2 Code table 4.2 : 3.3
 	// Flight Rules Conditions products
        if (_paramNumber < (si32) sizeof (_satelliteFlight) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_satelliteFlight[_paramNumber].name);
          _parameterLongName = new string (_satelliteFlight[_paramNumber].comment);
          _parameterUnits = new string (_satelliteFlight[_paramNumber].unit);
        }
        break;

      case 4:
        // GRIB2 Code table 4.2 : 3.4
 	// Volcanic Ash products
        if (_paramNumber < (si32) sizeof (_satelliteAsh) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_satelliteAsh[_paramNumber].name);
          _parameterLongName = new string (_satelliteAsh[_paramNumber].comment);
          _parameterUnits = new string (_satelliteAsh[_paramNumber].unit);
        }
        break;

      case 5:
        // GRIB2 Code table 4.2 : 3.5
 	// Sea-Surface Temperature products
        if (_paramNumber < (si32) sizeof (_satelliteSeaTemp) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_satelliteSeaTemp[_paramNumber].name);
          _parameterLongName = new string (_satelliteSeaTemp[_paramNumber].comment);
          _parameterUnits = new string (_satelliteSeaTemp[_paramNumber].unit);
        }
        break;

      case 6:
        // GRIB2 Code table 4.2 : 3.6
 	// Solar Radiation products
        if (_paramNumber < (si32) sizeof (_satelliteSolar) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_satelliteSolar[_paramNumber].name);
          _parameterLongName = new string (_satelliteSolar[_paramNumber].comment);
          _parameterUnits = new string (_satelliteSolar[_paramNumber].unit);
        }
        break;

      default:
        _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
        _parameterLongName =new string ("Unknown Parameter");
        _parameterUnits = new string ("-");
      } // end switch (parameterCategory) 

      break;


    case 4:
      // Product Discipline 3, Space Weather products

      switch (_parameterCategory) 
      {
      case 0:
        // GRIB2 Code table 4.2 : 4.0
 	// Temperature
        if (_paramNumber < (si32) sizeof (_spaceTemp) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceTemp[_paramNumber].name);
	  _parameterLongName = new string (_spaceTemp[_paramNumber].comment);
	  _parameterUnits = new string (_spaceTemp[_paramNumber].unit);
        }
        break;

      case 1:
        // GRIB2 Code table 4.2 : 4.1
 	// Momentum
        if (_paramNumber < (si32) sizeof (_spaceMomentum) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceMomentum[_paramNumber].name);
	  _parameterLongName = new string (_spaceMomentum[_paramNumber].comment);
	  _parameterUnits = new string (_spaceMomentum[_paramNumber].unit);
        }
        break;

      case 2:
        // GRIB2 Code table 4.2 : 4.2
 	// Charged Particle Mass and Number
        if (_paramNumber < (si32) sizeof (_spaceChargedParticle) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceChargedParticle[_paramNumber].name);
	  _parameterLongName = new string (_spaceChargedParticle[_paramNumber].comment);
	  _parameterUnits = new string (_spaceChargedParticle[_paramNumber].unit);
        }
        break;

      case 3:
        // GRIB2 Code table 4.2 : 4.3
 	// Electric and Magnetic Fields
        if (_paramNumber < (si32) sizeof (_spaceFields) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceFields[_paramNumber].name);
	  _parameterLongName = new string (_spaceFields[_paramNumber].comment);
	  _parameterUnits = new string (_spaceFields[_paramNumber].unit);
        }
        break;

      case 4:
        // GRIB2 Code table 4.2 : 4.4
 	// Energetic Particles
        if (_paramNumber < (si32) sizeof (_spaceEnergeticParticle) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceEnergeticParticle[_paramNumber].name);
	  _parameterLongName = new string (_spaceEnergeticParticle[_paramNumber].comment);
	  _parameterUnits = new string (_spaceEnergeticParticle[_paramNumber].unit);
        }
        break;

      case 5:
        // GRIB2 Code table 4.2 : 4.4
 	// Space Waves
        if (_paramNumber < (si32) sizeof (_spaceWaves) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceWaves[_paramNumber].name);
	  _parameterLongName = new string (_spaceWaves[_paramNumber].comment);
	  _parameterUnits = new string (_spaceWaves[_paramNumber].unit);
        }
        break;

      case 6:
        // GRIB2 Code table 4.2 : 4.6
 	// Solar Electromagnetic Emissions
        if (_paramNumber < (si32) sizeof (_spaceSolarEmissions) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceSolarEmissions[_paramNumber].name);
	  _parameterLongName = new string (_spaceSolarEmissions[_paramNumber].comment);
	  _parameterUnits = new string (_spaceSolarEmissions[_paramNumber].unit);
        }
        break;

      case 7:
        // GRIB2 Code table 4.2 : 4.7
 	// Terrestial Electromagnetic Emissions
        if (_paramNumber < (si32) sizeof (_spaceTerrEmissions) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceTerrEmissions[_paramNumber].name);
	  _parameterLongName = new string (_spaceTerrEmissions[_paramNumber].comment);
	  _parameterUnits = new string (_spaceTerrEmissions[_paramNumber].unit);
        }
        break;

      case 8:
        // GRIB2 Code table 4.2 : 4.8
 	// Imagery
        if (_paramNumber < (si32) sizeof (_spaceImagery) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceImagery[_paramNumber].name);
	  _parameterLongName = new string (_spaceImagery[_paramNumber].comment);
	  _parameterUnits = new string (_spaceImagery[_paramNumber].unit);
        }
        break;

      case 9:
        // GRIB2 Code table 4.2 : 4.9
 	// Ion-Neutral Coupling
        if (_paramNumber < (si32) sizeof (_spaceCoupling) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceCoupling[_paramNumber].name);
	  _parameterLongName = new string (_spaceCoupling[_paramNumber].comment);
	  _parameterUnits = new string (_spaceCoupling[_paramNumber].unit);
        }
        break;

      case 10:
        // GRIB2 Code table 4.2 : 4.10
 	// Spasce Indices
        if (_paramNumber < (si32) sizeof (_spaceIndices) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_spaceIndices[_paramNumber].name);
	  _parameterLongName = new string (_spaceIndices[_paramNumber].comment);
	  _parameterUnits = new string (_spaceIndices[_paramNumber].unit);
        }
        break;

      default:
        _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
        _parameterLongName =new string ("Unknown Parameter");
        _parameterUnits = new string ("-");
      } // end switch (parameterCategory) 

      break;

    case 10:
      // Product Discipline 10, Oceanographic products

      switch (_parameterCategory) 
      {
      case 0:
        // GRIB2 Code table 4.2 : 10.0
 	// Waves
        if (_paramNumber < (si32) sizeof (_oceanWaves) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_oceanWaves[_paramNumber].name);
	  _parameterLongName = new string (_oceanWaves[_paramNumber].comment);
	  _parameterUnits = new string (_oceanWaves[_paramNumber].unit);
        }
        break;

      case 1:
        // GRIB2 Code table 4.2 : 10.1 
 	// Currents
        if (_paramNumber < (si32) sizeof (_oceanCurrents) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_oceanCurrents[_paramNumber].name);
          _parameterLongName = new string (_oceanCurrents[_paramNumber].comment);
          _parameterUnits = new string (_oceanCurrents[_paramNumber].unit);
        }
        break;

      case 2:
        // GRIB2 Code table 4.2 : 10.2
 	// Ice
        if (_paramNumber < (si32) sizeof (_oceanIce) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_oceanIce[_paramNumber].name);
	  _parameterLongName = new string (_oceanIce[_paramNumber].comment);
	  _parameterUnits = new string (_oceanIce[_paramNumber].unit);
        }
        break;

      case 3:
        // GRIB2 Code table 4.2 : 10.3 
 	// Surface properties
        if (_paramNumber < (si32) sizeof (_oceanSurface) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_oceanSurface[_paramNumber].name);
          _parameterLongName = new string (_oceanSurface[_paramNumber].comment);
          _parameterUnits = new string (_oceanSurface[_paramNumber].unit);
        }
        break;

      case 4:
        // GRIB2 Code table 4.2 : 10.4
 	// Sub-surface properties
        if (_paramNumber < (si32) sizeof (_oceanSubSurface) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_oceanSubSurface[_paramNumber].name);
          _parameterLongName = new string (_oceanSubSurface[_paramNumber].comment);
          _parameterUnits = new string (_oceanSubSurface[_paramNumber].unit);
        }
        break;

     case 191:
        // GRIB2 Code table 4.2 : 10.191 
 	//  Miscellaneous
        if (_paramNumber < (si32) sizeof (_oceanMisc) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_oceanMisc[_paramNumber].name);
          _parameterLongName =new string (_oceanMisc[_paramNumber].comment);
          _parameterUnits = new string (_oceanMisc[_paramNumber].unit);
        }
        break;

      default:
        _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
        _parameterLongName =new string ("Unknown Parameter");
        _parameterUnits = new string ("-");
      } // end switch (parameterCategory) 

      break;

    case 20:
      // Product Discipline 10, Health and Socioeconomic impacts products

      switch (_parameterCategory) 
      {
      case 0:
        // GRIB2 Code table 4.2 : 20.0
 	// Waves
        if (_paramNumber < (si32) sizeof (_healthIndicators) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_healthIndicators[_paramNumber].name);
	  _parameterLongName = new string (_healthIndicators[_paramNumber].comment);
	  _parameterUnits = new string (_healthIndicators[_paramNumber].unit);
        }
        break;

      case 1:
        // GRIB2 Code table 4.2 : 20.1 
 	// Currents
        if (_paramNumber < (si32) sizeof (_healthEpidemiology) / (si32)sizeof(_GRIB2ParmTable)) {
          _parameterName = new string (_healthEpidemiology[_paramNumber].name);
          _parameterLongName = new string (_healthEpidemiology[_paramNumber].comment);
          _parameterUnits = new string (_healthEpidemiology[_paramNumber].unit);
        }
        break;

      case 2:
        // GRIB2 Code table 4.2 : 20.2
 	// Ice
        if (_paramNumber < (si32) sizeof (_healthPopDensity) / (si32)sizeof(_GRIB2ParmTable)) {
	  _parameterName = new string (_healthPopDensity[_paramNumber].name);
	  _parameterLongName = new string (_healthPopDensity[_paramNumber].comment);
	  _parameterUnits = new string (_healthPopDensity[_paramNumber].unit);
        }
        break;

      default:
        _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
        _parameterLongName =new string ("Unknown Parameter");
        _parameterUnits = new string ("-");
      } // end switch (parameterCategory) 

      break;

      // Product Discipline 209, Reserved for Local Use
    case 209:

      // Center: 161 = US NOAA Office of Oceanic and Atmospheric Research
      // Subcenter: 0 (National Severe Storms Lab)
      // Operational MRMS GRIB2 Tables
      if(_centerId == 161 && _subCenterId == 0) {
	for (ui32 i = 0; i < _MRMSlocalTable_numElements; i++ ) {

	  if ((_MRMSlocalTable[i].prodDiscipline == _disciplineNum) && 
	      (_parameterCategory == _MRMSlocalTable[i].category) &&
	      (_paramNumber == _MRMSlocalTable[i].paramNumber)) {
	    _parameterName = new string (_MRMSlocalTable[i].name);   
	    _parameterLongName = new string (_MRMSlocalTable[i].comment);
	    _parameterUnits = new string (_MRMSlocalTable[i].unit);
	    break;
	  }
	} // for
      } else {
	_parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
	_parameterLongName = new string ("Unknown Paramater");
	_parameterUnits = new string ("-");
      }
      
      break;

    default:
      _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
      _parameterLongName =new string ("Unknown Paramater");
      _parameterUnits = new string ("-");      
    } // end switch (_disciplineNum)

    if(_parameterName == NULL) {
      _parameterName = new string ("UNKNOWN-"+to_string(_disciplineNum)+"_"+to_string(_parameterCategory)+"_"+to_string(_paramNumber));
      _parameterLongName = new string ("Unknown Paramater");
      _parameterUnits = new string ("-");
    } 

  } // end else

}

int ProdDefTemp::_getSurfaceIndex (int value) const
{
  if(value <= 25)                   /* 0 - 24 */
    return (value-1);
  if(value >= 100 && value <= 120)  /* 25 - 45 */
    return (value -100 + 25);
  if(value == 150)                  /* 46 */
    return (value - 150 + 46);
  if(value == 151)                  /* 47 */
    return (value - 151 + 47);
  if(value >= 160 && value <= 184)  /* 48 - 72 */
    return (value -160 + 48);
  if(value >= 200 && value <= 224)  /* 73 - 97 */
    return (value -200 + 73);
  if(value >= 232 && value <= 255)  /* 98 - 120 */
    return (value -232 + 98);
	    
  return (-1);
 
}

void ProdDefTemp::_printTimeIncrementType(FILE *stream, int timeIncrement) const
{
  fprintf(stream, "    Type of time increment between successive fields: \n");
  switch( timeIncrement ) {
  case 1:
    fprintf(stream, "        Successive times processed have same forecast time, start time of forecast is incremented.\n");
    break;
  case 2:
    fprintf(stream, "        Successive times processed have same start time of forecast, forecast time is incremented.\n");
    break;
  case 3:
    fprintf(stream, "        Successive times processed have start time of forecast incremented and forecast time decremented so that valid time remains constant.\n");
    break;
  case 4:
    fprintf(stream, "        Successive times processed have start time of forecast decremented and forecast time incremented so that valid time remains constant.\n");
    break;
  case 5:
    fprintf(stream, "        Floating subinterval of time between forecast time and end of overall time interval.\n");
    break;
  default:
    fprintf(stream, "        Unknown\n");
    break;
  }
}

//
// GRIB2 - CODE TABLE 4.3
// TYPE OF GENERATING PROCESS
void ProdDefTemp::_printGeneratingProcessType(FILE *stream, int processType) const
{
  fprintf(stream, "Type of generating process: ");
  switch( processType ) {
  case 0:
    fprintf(stream, "Analysis\n");
    break;
  case 1:
    fprintf(stream, "Initialization\n");
    break;
  case 2:
    fprintf(stream, "Forecast\n");
    break;
  case 3:
    fprintf(stream, "Bias Corrected Forecast\n");
    break;
  case 4:
    fprintf(stream, "Ensemble Forecast\n");
    break;
  case 5:
    fprintf(stream, "Probability Forecast\n");
    break;
  case 6:
    fprintf(stream, "Forecast Error\n");
    break;
  case 7:
    fprintf(stream, "Analysis Error\n");
    break;
  case 8:
    fprintf(stream, "Observation\n");
    break;
  case 9:
    fprintf(stream, "Climatological\n");
    break;
  case 10:
    fprintf(stream, "Probability-Weighted Forecast\n");
    break;
  case 11:
    fprintf(stream, "Bias-Corrected Ensemble Forecast\n");
    break;
  case 12:
    fprintf(stream, "Post-processed Analysis\n");
    break;
  case 13:
    fprintf(stream, "Post-processed Forecast\n");
    break;
  case 14:
    fprintf(stream, "Nowcast\n");
    break;
  case 15:
    fprintf(stream, "Hindcast\n");
    break;
  case 192:
    fprintf(stream, "Forecast Confidence Indicator\n");
    break;
  case 255:
    fprintf(stream, "Missing\n");
    break;
  default:
    fprintf(stream, "Unknown\n");
    break;
  }
}

//
// GRIB2 -CODE TABLE 4.4
// INDICATOR OF UNIT OF TIME RANGE
void ProdDefTemp::_printTimeUnits(FILE *stream, int timeUnits) const
{
  switch( timeUnits ) {
  case 0:
    fprintf(stream, "Minutes\n");
    break;	  
  case 1:
    fprintf(stream, "Hours\n");
    break;
  case 2:
    fprintf(stream, "Days\n");
    break;
  case 3:
    fprintf(stream, "Months\n");
    break;
  case 4:
    fprintf(stream, "Years\n");
    break;
  case 5:
    fprintf(stream, "Decades\n");
    break;
  case 6:
    fprintf(stream, "Normals (30 Years)\n"); 
    break;    
  case 7:
    fprintf(stream, "Centurys\n");
    break;
  case 10:
    fprintf(stream, "3 hour periods\n");
    break;
  case 11:
    fprintf(stream, "6 hours periods\n");
    break;
  case 12:
    fprintf(stream, "12 hours periods\n");
    break;
  case 13:	  
  case 254:
    fprintf(stream, "Seconds\n");
    break;
  default:
    fprintf(stream, "Unknown units\n");
    break;
  }
}

long int ProdDefTemp::_getTimeUnits( long int timeUnits) const
{
  switch( timeUnits ) {
  // Minutes
  case 0:
    return(60);
    break;
  // Hours    
  case 1:
    return(60*60);
    break;
  // Days
  case 2:
    return(24*60*60);
    break;
  // Time period of months poses a problem... variable days per month
  case 3:
    return(24*60*60);
    break;
  // Time period of years is set to 365 days which could be incorrect by one day (leap year).
  case 4:
    return((long int)24*60*60*(long int)365);
    break;
  // Decade can be off by one day as well (unknown number of leap years in decade).
  case 5:
    return((long int)24*60*60*(long int)3652);
    break;
  // Normal 30 years, hopefully never used.
  case 6:
    return((long int)24*60*60*(long int)10957);
    break;
  // Century
  case 7:
    return((long int)24*60*60*(long int)36524);
    break;
  // 3 hours
  case 10:
    return(3*60*60);
    break;
  // 6 hours    
  case 11:
    return(6*60*60);
    break;
  // 12 hours    
  case 12:
    return(12*60*60);
    break;
  // Seconds    
  case 13:
  case 254:
    return (1);
    break;
    
  default:
    return( -1 );
    break;
  }
}

string ProdDefTemp::_getTimeUnitName(int time, int timeUnits) const
{
  string timeName;
  switch( timeUnits ) {
  case 0:
    timeName = "Min";
    break;
  case 1:
    timeName = "Hr";
    break;    
  case 2:
    timeName = "Day";
    break;
  case 3:
    timeName = "Mon";
    break;
  case 4:
    timeName = "Yr";
    break;
  case 5:
    timeName = "Yr";
    time *= 10;
    break;
  case 6:
    timeName = "Yr";
    time *= 30;
    break;
  case 7:
    timeName = "Yr";
    time *= 100;
    break;
  case 10:
    timeName = "Hr";
    time *= 3;
    break;    
  case 11:
    timeName = "Hr";
    time *= 6;
    break;
  case 12:
    timeName = "Hr";
    time *= 12;
    break;
  case 13:
  case 254:
    timeName = "Sec";
    break;
  default:
    timeName = "Unk";
    break;
  }
  char buf[50];
  sprintf(buf, "%d", time);
  timeName = buf + timeName;
  return timeName;
}

//
// GRIB2 - CODE TABLE 4.6
// TYPE OF ENSEMBLE FORECAST
void ProdDefTemp::_printEnsembleForecastType(FILE *stream, int ensembleId) const
{
  fprintf(stream, "Type of ensemble forecast: ");
  switch( ensembleId ) {
  case 0:
    fprintf(stream, "Unperturbed High-Resolution Control Forecast\n");
    break;	  
  case 1:
    fprintf(stream, "Unperturbed Low-Resolution Control Forecast\n");
    break;
  case 2:
    fprintf(stream, "Negatively Perturbed Forecast\n");
    break;
  case 3:
    fprintf(stream, "Positively Perturbed Forecast\n");
    break;
  case 4:
    fprintf(stream, "Multi-Model Forecast\n");
    break;
  case 192:
    fprintf(stream, "Perturbed Ensemble Member\n");
    break;
  default:
    fprintf(stream, "Unknown Ensemble Type\n");
    break;
  }
}

//
// GRIB2 - CODE TABLE 4.7
// DERIVED FORECAST TYPE
void ProdDefTemp::_printDerivedForecastType(FILE *stream, int derivedId) const
{
  fprintf(stream, "Type of derived forecast: ");
  switch( derivedId ) {
  case 0:
    fprintf(stream, "Unweighted Mean of All Members\n");
    break;	  
  case 1:
    fprintf(stream, "Weighted Mean of All Members\n");
    break;
  case 2:
    fprintf(stream, "Standard Deviation with respect to Cluster Mean\n");
    break;
  case 3:
    fprintf(stream, "Standard Deviation with respect to Cluster Mean, Normalized\n");
    break;
  case 4:
    fprintf(stream, "Spread of All Members\n");
    break;
  case 5:
    fprintf(stream, "Large Anomaly Index of All Members\n");
    break;
  case 6:
    fprintf(stream, "Unweighted Mean of the Cluster Members\n");
    break;
  case 7:
    fprintf(stream, "Interquartile Range (Range between the 25th and 75th quantile)\n");
    break;
  case 8:
    fprintf(stream, "Minimum Of All Ensemble Members\n");
    break;
  case 9:
    fprintf(stream, "Maximum Of All Ensemble Members\n");
    break;
  case 192:
    fprintf(stream, "Unweighted Mode of All Members\n");
    break;
  case 193:
    fprintf(stream, "Percentile value (10%%) of All Members\n");
    break;
  case 194:
    fprintf(stream, "Percentile value (50%%) of All Members\n");
    break;
  case 195:
    fprintf(stream, "Percentile value (90%%) of All Members\n");
    break;
  case 196:
    fprintf(stream, "Statistically decided weights for each ensemble member\n");
    break;
  case 197:
    fprintf(stream, "Climate Percentile (percentile values from climate distribution)\n");
    break;
  default:
    fprintf(stream, "Unknown Derived Type\n");
    break;
  }
}

string ProdDefTemp::_getDerivedForecastType(int derivedId) const
{
  string ensemble;
  switch( derivedId ) {
  case 0:
    ensemble = "Ensemble Mean";
    break;	  
  case 1:
    ensemble = "Ensemble Weighted Mean";
    break;
  case 2:
    ensemble = "Ensemble Standard Deviation";
    break;
  case 3:
    ensemble = "Ensemble Standard Deviation Normalized";
    break;
  case 4:
    ensemble = "Ensemble Spread";
    break;
  case 5:
    ensemble = "Ensemble Large Anomaly";
    break;
  case 6:	  
    ensemble = "Ensemble Mean";
    break;
  case 7:	  
    ensemble = "Ensemble Interquartile Range";
    break;
  case 8:	  
    ensemble = "Ensemble Minimum";
    break;
  case 9:	  
    ensemble = "Ensemble Maximum";
    break;
  case 192:
    ensemble = "Unweighted Mode";
    break;
  case 193:
    ensemble = "Percentile value (10%)";
    break;
  case 194:
    ensemble = "Percentile value (50%)";
    break;
  case 195:
    ensemble = "Percentile value (90%)";
    break;
  case 196:
    ensemble = "Statistically decided weights for each ensemble member";
    break;
  case 197:
    ensemble = "Climate Percentile";
    break;
  default:
    ensemble = "";
    break;
  }
  return ensemble;
}

//
// GRIB2 - CODE TABLE 4.9
// PROBABILITY TYPE
void ProdDefTemp::_printProbabilityType(FILE *stream, int probId) const
{
  fprintf(stream, "Probability Type: \n");
  switch( probId ) {
  case 0:
    fprintf(stream, "        Probability of event below lower limit\n");
    break;
  case 1:
    fprintf(stream, "        Probability of event above upper limit\n");
    break;
  case 2:
    fprintf(stream, "        Probability of event between upper and lower limits (range includes lower limit but no the upper limit)\n");
    break;
  case 3:
    fprintf(stream, "        Probability of event above lower limit\n");
    break;
  case 4:
    fprintf(stream, "        Probability of event below upper limit\n");
    break;
  default:
    fprintf(stream, "        Unknown Probability type\n");
    break;
  }
}

//
// GRIB2 - CODE TABLE 4.10
// TYPE OF STATISTICAL PROCESSING 
void ProdDefTemp::_printStatisticalProcess(FILE *stream, int processId) const
{
  fprintf(stream, "Statistical process: ");
  switch( processId ) {
  case 0:
    fprintf(stream, "Average\n");
    break;
  case 1:
    fprintf(stream, "Accumulation\n");
    break;
  case 2:
    fprintf(stream, "Maximum\n");
    break;
  case 3:
    fprintf(stream, "Minimum\n");
    break;
  case 4:
    fprintf(stream, "Difference (value at the end of the time range minus value at the beginning)\n");
    break;
  case 5:
    fprintf(stream, "Root Mean Square\n");
    break;
  case 6:
    fprintf(stream, "Standard Deviation\n");
    break;
  case 7:
    fprintf(stream, "Covariance (temporal variance)\n");
    break;
  case 8:
    fprintf(stream, "Difference (value at the beginning of the time range minus value at the end)\n");
    break;
  case 9:
    fprintf(stream, "Ratio\n");
    break;
  case 10:
     fprintf(stream, "Standardized Anomaly\n");
     break;
  case 11:
     fprintf(stream, "Summation\n");
     break;
  case 12:
     fprintf(stream, "Confidence Index\n");
     break;
  case 13:
     fprintf(stream, "Quality Indicator\n");
     break;
  case 192:
     fprintf(stream, "Climatological Mean Value\n");
     break;
    case 193:
     fprintf(stream, "Average of forecasts or initialized analyses\n");
     break;
    case 194:
     fprintf(stream, "Average of uninitialized analyses\n");
     break;
    case 195:
     fprintf(stream, "Average of forecast accumulations\n");
     break;
    case 196:
     fprintf(stream, "Average of successive forecast accumulations\n");
     break;
    case 197:
     fprintf(stream, "Average of forecast averages\n");
     break;
    case 198:
     fprintf(stream, "Average of successive forecast averages\n");
     break;
    case 199:
     fprintf(stream, "Climatological Average of N analyses, each a year apart\n");
     break;
    case 200:
     fprintf(stream, "Climatological Average of N forecasts, each a year apart\n");
     break;
    case 201:
     fprintf(stream, "Climatological Root Mean Square difference between N forecasts and their verifying analyses, each a year apart\n");
     break;
    case 202:
     fprintf(stream, "Climatological Standard Deviation of N forecasts from the mean of the same N forecasts, for forecasts one year apart\n");
     break;
    case 203:
     fprintf(stream, "Climatological Standard Deviation of N analyses from the mean of the same N analyses, for analyses one year apart\n");
     break;
    case 204:
     fprintf(stream, "Average of forecast accumulations at 6-hour intervals\n");
     break;
    case 205:
     fprintf(stream, "Average of forecast averages at 6-hour intervals\n");
     break;
    case 206:
     fprintf(stream, "Average of forecast accumulations at 12-hour intervals\n");
     break;
    case 207:
     fprintf(stream, "Average of forecast averages at 12-hour intervals\n");
     break;
  default:
    fprintf(stream, "Unknown Statistical Process\n");
    break;
  }
}

string ProdDefTemp::_getStatisticalProcess(int processId) const
{
  string process;
  switch( processId ) {
  case 0:
  case 193:
  case 194:
  case 195:
  case 196:
  case 197:
  case 198:
  case 199:
  case 200:
  case 204:
  case 205:
  case 206:
  case 207:
    process = "_AVG";
    break;
  case 1:  // Do not append Accumulation to var name
    process = "";
    break;
  case 2:
    process = "_MAX";
    break;
  case 3:
    process = "_MIN";
    break;
  case 4:
 case 8:
    process = "_DIFF";
    break;
  case 5:
 case 201:
    process = "_RMS";
    break;
  case 6:
 case 202:
 case 203:
    process = "_SDEV";
    break;
  case 7:
    process = "_COV";
    break;
  case 9:
    process = "_RATIO";
    break;
  case 10:
     process = "_ANOM";
     break;
  case 11:
     process = "_SUM";
     break;
  case 12:
     process = "_CONF";
     break;
  case 13:
     process = "_QUAL";
     break;
  case 192:
     process = "_MEAN";
     break;
  default:
    process = "";
    break;
  }
  return process;
}

//
// GRIB2 - CODE TABLE 4.15
// TYPE OF SPATIAL PROCESSING USED TO ARRIVE AT A GIVEN
// DATA VALUE FROM THE SOURCE DATA
void ProdDefTemp::_printSpatialProcess(FILE *stream, int processId) const
{
  fprintf(stream, "Type of spatial interpolation: \n");
  switch( processId ) {
  case 0:
    fprintf(stream, "      Data is calculated directly from the source grid with no interpolation\n");
    break;
  case 1:
    fprintf(stream, "      Bilinear interpolation using the 4 source grid grid-point values surrounding the nominal grid-point\n");
    break;
  case 2:
    fprintf(stream, "      Bicubic interpolation using the 4 source grid grid-point values surrounding the nominal grid-point\n");
    break;
  case 3:
    fprintf(stream, "      Using the value from the source grid grid-point which is nearest to the nominal grid-point\n");
    break;
  case 4:
    fprintf(stream, "      Budget interpolation using the 4 source grid grid-point values surrounding the nominal grid-point\n");
    break;
  case 5:
    fprintf(stream, "      Spectral interpolation using the 4 source grid grid-point values surrounding the nominal grid-point\n");
    break;
  case 6:
    fprintf(stream, "      Neighbor-budget interpolation using the 4 source grid grid-point values surrounding the nominal grid-point\n");
    break;
  default:
    fprintf(stream, "      Unknown Spatial Process\n");
    break;
  }
}

//
// ON388 - TABLE A
// Generating Process or Model from Originating Center 7 (US-NWS, NCEP)
string ProdDefTemp::getGeneratingProcess() const
{
  string process;
  if(_centerId != 7) {
    process = "Generating Process ID ";
    process += to_string(_processID);
    return process;
  }
  switch( _processID ) {
  case 02:
    process = "Ultra Violet Index Model";
    break;
  case 03:
    process = "NCEP/ARL Transport and Dispersion Model";
    break;
  case 04:
    process = "NCEP/ARL Smoke Model";
    break;
  case 05:
    process = "Satellite Derived Precipitation and temperatures, from IR";
    break;
  case 06:
    process = "NCEP/ARL Dust Model";
    break;
  case 10:
    process = "Global Wind-Wave Forecast Model";
    break;
  case 11:
    process = "Global Multi-Grid Wave Model (Static Grids)";
    break;
  case 12:
    process = "Probabilistic Storm Surge";
    break;
  case 13:
    process = "Hurricane Multi-Grid Wave Model";
    break;
  case 14:
    process = "Extra-tropical Storm Surge Atlantic Domain";
    break;
  case 15:
    process = "Nearshore Wave Prediction System (NWPS)";
    break;
  case 17:
    process = "Extra-tropical Storm Surge Pacific Domain";
    break;
  case 19:
    process = "Limited-area Fine Mesh (LFM) analysis";
    break;
  case 25:
    process = "Snow Cover Analysis";
    break;
  case 30:
    process = "Forecaster generated field";
    break;
  case 31:
    process = "Value added post processed field";
    break;
  case 42:
    process = "Global Optimum Interpolation Analysis (GOI) from GFS model";
    break;
  case 43:
    process = "Global Optimum Interpolation Analysis (GOI) from Final run ";
    break;
  case 44:
    process = "Sea Surface Temperature Analysis";
    break;
  case 45:
    process = "Coastal Ocean Circulation Model";
    break;
  case 46:
    process = "HYCOM - Global";
    break;
  case 47:
    process = "HYCOM - North Pacific basin";
    break;
  case 48:
    process = "HYCOM - North Atlantic basin";
    break;
  case 49:
    process = "Ozone Analysis from TIROS Observations ";
    break;
  case 52:
    process = "Ozone Analysis from Nimbus 7 Observations ";
    break;
  case 53:
    process = "LFM-Fourth Order Forecast Model";
    break;
  case 64:
    process = "Regional Optimum Interpolation Analysis (ROI)";
    break;
  case 68:
    process = "80 wave triangular, 18-layer Spectral model from GFS model";
    break;
  case 69:
    process = "80 wave triangular, 18 layer Spectral model from Medium Range Forecast run";
    break;
  case 70:
    process = "Quasi-Lagrangian Hurricane Model (QLM)";
    break;
  case 73:
    process = "Fog Forecast model - Ocean Prod. Center";
    break;
  case 74:
    process = "Gulf of Mexico Wind/Wave";
    break;
  case 75:
    process = "Gulf of Alaska Wind/Wave";
    break;
  case 76:
    process = "Bias corrected Medium Range Forecast";
    break;
  case 77:
    process = "126 wave triangular, 28 layer Spectral model from GFS model";
    break;
  case 78:
    process = "126 wave triangular, 28 layer Spectral model from Medium Range Forecast run";
    break;
  case 79:
    process = "Backup from the previous run";
    break;
  case 80:
    process = "62 wave triangular, 28 layer Spectral model from Medium Range Forecast run";
    break;
  case 81:
    process = "Analysis from GFS (Global Forecast System)";
    break;
  case 82:
    process = "Analysis from GDAS (Global Data Assimilation System)";
    break;
  case 83:
    process = "High Resolution Rapid Refresh (HRRR)";
    break;
  case 84:
    process = "MESO NAM Model";
    break;
  case 85:
    process = "Real Time Ocean Forecast System (RTOFS)";
    break;
  case 87:
    process = "CAC Ensemble Forecasts from Spectral (ENSMB)";
    break;
  case 88:
    process = "NOAA Wave Watch III (NWW3) Ocean Wave Model";
    break;
  case 89:
    process = "Non-hydrostatic Meso Model (NMM) (Currently 8 km)";
    break;
  case 90:
    process = "62 wave triangular, 28 layer spectral model extension of the Medium Range Forecast run";
    break;
  case 91:
    process = "62 wave triangular, 28 layer spectral model extension of the GFS model";
    break;
  case 92:
    process = "62 wave triangular, 28 layer spectral model run from the Medium Range Forecast final analysis";
    break;
  case 93:
    process = "62 wave triangular, 28 layer spectral model run from the T62 GDAS analysis of the Medium Range Forecast run";
    break;
  case 94:
    process = "T170/L42 Global Spectral Model from MRF run";
    break;
  case 95:
    process = "T126/L42 Global Spectral Model from MRF run";
    break;
  case 96:
    process = "Global Forecast System Model";
    break;
  case 98:
    process = "Climate Forecast System Model -- Atmospheric model (GFS) coupled to a multi level ocean model";
    break;
  case 105:
    process = "Rapid Refresh (RAP)";
    break;
  case 107:
    process = "Global Ensemble Forecast System (GEFS)";
    break;
  case 108:
    process = "LAMP";
    break;
  case 109:
    process = "RTMA (Real Time Mesoscale Analysis)";
    break;
  case 110:
    process = "NAM Model - 15km version";
    break;
  case 111:
    process = "NAM model, generic resolution (Used in SREF processing)";
    break;
  case 112:
    process = "WRF-NMM model, generic resolution (Used in various runs) NMM=Nondydrostatic Mesoscale Model (NCEP)";
    break;
  case 113:
    process = "Products from NCEP SREF processing";
    break;
  case 114:
    process = "NAEFS Products from joined NCEP, CMC global ensembles";
    break;
  case 115:
    process = "Downscaled GFS from NAM eXtension";
    break;
  case 116:
    process = "WRF-EM model, generic resolution (Used in various runs) EM - Eulerian Mass-core (NCAR - aka Advanced Research WRF)";
    break;
  case 117:
    process = "NEMS GFS Aerosol Component";
    break;
  case 118:
    process = "URMA (UnRestricted Mesoscale Analysis)";
    break;
  case 119:
    process = "WAM (Whole Atmosphere Model)";
    break;
  case 120:
    process = "Ice Concentration Analysis";
    break;
  case 121:
    process = "Western North Atlantic Regional Wave Model";
    break;
  case 122:
    process = "Alaska Waters Regional Wave Model";
    break;
  case 123:
    process = "North Atlantic Hurricane Wave Model";
    break;
  case 124:
    process = "Eastern North Pacific Regional Wave Model";
    break;
  case 125:
    process = "North Pacific Hurricane Wave Model";
    break;
  case 126:
    process = "Sea Ice Forecast Model";
    break;
  case 127:
    process = "Lake Ice Forecast Model";
    break;
  case 128:
    process = "Global Ocean Forecast Model";
    break;
  case 129:
    process = "Global Ocean Data Analysis System (GODAS)";
    break;
  case 130:
    process = "Merge of fields from the RUC, NAM, and Spectral Model ";
    break;
  case 131:
    process = "Great Lakes Wave Model";
    break;
  case 140:
    process = "North American Regional Reanalysis (NARR)";
    break;
  case 141:
    process = "Land Data Assimilation and Forecast System";
    break;
  case 150:
    process = "NWS River Forecast System (NWSRFS)";
    break;
  case 151:
    process = "NWS Flash Flood Guidance System (NWSFFGS)";
    break;
  case 152:
    process = "WSR-88D Stage II Precipitation Analysis";
    break;
  case 153:
    process = "WSR-88D Stage III Precipitation Analysis";
    break;
  case 180:
    process = "Quantitative Precipitation Forecast generated by NCEP";
    break;
  case 181:
    process = "River Forecast Center Quantitative Precipitation Forecast mosaic generated by NCEP";
    break;
  case 182:
    process = "River Forecast Center Quantitative Precipitation estimate mosaic generated by NCEP";
    break;
  case 183:
    process = "NDFD product generated by NCEP/HPC";
    break;
  case 184:
    process = "Climatological Calibrated Precipitation Analysis - CCPA";
    break;
  case 190:
    process = "National Convective Weather Diagnostic generated by NCEP/AWC";
    break;
  case 191:
    process = "Current Icing Potential automated product genterated by NCEP/AWC";
    break;
  case 192:
    process = "Analysis product from NCEP/AWC";
    break;
  case 193:
    process = "Forecast product from NCEP/AWC";
    break;
  case 195:
    process = "Climate Data Assimilation System 2 (CDAS2)";
    break;
  case 196:
    process = "Climate Data Assimilation System 2 (CDAS2) - used for regeneration runs";
    break;
  case 197:
    process = "Climate Data Assimilation System (CDAS)";
    break;
  case 198:
    process = "Climate Data Assimilation System (CDAS) - used for regeneration runs";
    break;
  case 199:
    process = "Climate Forecast System Reanalysis (CFSR) -- Atmospheric model (GFS) coupled to a multi level ocean, land and seaice model";
    break;
  case 200:
    process = "CPC Manual Forecast Product";
    break;
  case 201:
    process = "CPC Automated Product";
    break;
  case 210:
    process = "EPA Air Quality Forecast - Currently North East US domain";
    break;
  case 211:
    process = "EPA Air Quality Forecast - Currently Eastern US domain";
    break;
  case 215:
    process = "SPC Manual Forecast Product";
    break;
  case 220:
    process = "NCEP/OPC automated product";
    break;
  default:
    process = "Unknown Generating Process";
    break;
  }
  return process;
}

//
// GRIB2 - CODE TABLE 4.233 and 4.230
// Atmospheric Chemicals / Aerosols
string ProdDefTemp::_getAtmoChemical(int chemId) const
{
  string chemical;
  switch( chemId ) {
   case 0:
	chemical = "Ozone  O3";
	break;
   case 1:
	chemical = "Water Vapour  H2O";
	break;
   case 2:
	chemical = "Methane  CH4";
	break;
   case 3:
	chemical = "Carbon Dioxide  CO2";
	break;
   case 4:
	chemical = "Carbon Monoxide  CO";
	break;
   case 5:
	chemical = "Nitrogen Dioxide  NO2";
	break;
   case 6:
	chemical = "Nitrous Oxide  N2O";
	break;
   case 7:
	chemical = "Formaldehyde  HCHO";
	break;
   case 8:
	chemical = "Sulphur Dioxide  SO2";
	break;
   case 9:
	chemical = "Ammonia  NH3";
	break;
   case 10:
	chemical = "Ammonium  NH4+";
	break;
   case 11:
	chemical = "Nitrogen Monoxide  NO";
	break;
   case 12:
	chemical = "Atomic Oxygen  O";
	break;
   case 13:
	chemical = "Nitrate Radical  NO3";
	break;
   case 14:
	chemical = "Hydroperoxyl Radical  HO2";
	break;
   case 15:
	chemical = "Dinitrogen Pentoxide  H2O5";
	break;
   case 16:
	chemical = "Nitrous Acid  HONO";
	break;
   case 17:
	chemical = "Nitric Acid  HNO3";
	break;
   case 18:
	chemical = "Peroxynitric Acid  HO2NO2";
	break;
   case 19:
	chemical = "Hydrogen Peroxide  H2O2";
	break;
   case 20:
	chemical = "Molecular Hydrogen  H";
	break;
   case 21:
	chemical = "Atomic Nitrogen  N";
	break;
   case 22:
	chemical = "Sulphate SO42-";
	break;
   case 23:
	chemical = "Radon  Rn";
	break;
   case 24:
	chemical = "Elemental Mercury  Hg(O)";
	break;
   case 25:
	chemical = "Divalent Mercury  Hg2+";
	break;
   case 26:
	chemical = "Atomic Chlorine  Cl";
	break;
   case 27:
	chemical = "Chlorine Monoxide  ClO";
	break;
   case 28:
	chemical = "Dichlorine Peroxide  Cl2O2";
	break;
   case 29:
	chemical = "Hypochlorous Acid  HClO";
	break;
   case 30:
	chemical = "Chlorine Nitrate  ClONO2";
	break;
   case 31:
	chemical = "Chlorine Dioxide  ClO2";
	break;
   case 32:
	chemical = "Atomic Bromide  Br";
	break;
   case 33:
	chemical = "Bromine Monoxide  BrO";
	break;
   case 34:
	chemical = "Bromine Chloride  BrCl";
	break;
   case 35:
	chemical = "Hydrogen Bromide  HBr";
	break;
   case 36:
	chemical = "Hypobromous Acid  HBrO";
	break;
   case 37:
	chemical = "Bromine Nitrate  BrONO2";
	break;
   case 38:
	chemical = "Oxygen  O2";
	break;
   case 10000:
	chemical = "Hydroxyl Radical  OH";
	break;
   case 10001:
	chemical = "Methyl Peroxy Radical  CH3O2";
	break;
   case 10002:
	chemical = "Methyl Hydroperoxide  CH3O2H";
	break;
   case 10004:
	chemical = "Methanol  CH3OH";
	break;
   case 10005:
	chemical = "Formic Acid  CH3OOH";
	break;
   case 10006:
	chemical = "Hydrogen Cyanide  HCN";
	break;
   case 10007:
	chemical = "Aceto Nitrile  CH3CN";
	break;
   case 10008:
	chemical = "Ethane  C2H6";
	break;
   case 10009:
	chemical = "Ethene (= Ethylene)  C2H4";
	break;
   case 10010:
	chemical = "Ethyne (= Acetylene)  C2H2";
	break;
   case 10011:
	chemical = "Ethanol  C2H5OH";
	break;
   case 10012:
	chemical = "Acetic Acid  C2H5OOH";
	break;
   case 10013:
	chemical = "Peroxyacetyl Nitrate  CH3C(O)OONO2";
	break;
   case 10014:
	chemical = "Propane  C3H8";
	break;
   case 10015:
	chemical = "Propene  C3H6";
	break;
   case 10016:
	chemical = "Butanes  C4H10";
	break;
   case 10017:
	chemical = "Isoprene  C5H10";
	break;
   case 10018:
	chemical = "Alpha Pinene  C10H16";
	break;
   case 10019:
	chemical = "Beta Pinene  C10H16";
	break;
   case 10020:
	chemical = "Limonene  C10H16";
	break;
   case 10021:
	chemical = "Benzene  C6H6";
	break;
   case 10022:
	chemical = "Toluene  C7H8";
	break;
   case 10023:
	chemical = "Xylene  C8H10";
	break;
   case 10500:
	chemical = "Dimethyl Sulphide  CH3SCH3";
	break;
   case 20001:
	chemical = "Hydrogen Chloride  HCL";
	break;
   case 20002:
	chemical = "CFC-11";
	break;
   case 20003:
	chemical = "CFC-12";
	break;
   case 20004:
	chemical = "CFC-113";
	break;
   case 20005:
	chemical = "CFC-113a";
	break;
   case 20006:
	chemical = "CFC-114";
	break;
   case 20007:
	chemical = "CFC-115";
	break;
   case 20008:
	chemical = "HCFC-22";
	break;
   case 20009:
	chemical = "HCFC-141b";
	break;
   case 20010:
	chemical = "HCFC-142b";
	break;
   case 20011:
	chemical = "Halon-1202";
	break;
   case 20012:
	chemical = "Halon-1211";
	break;
   case 20013:
	chemical = "Halon-1301";
	break;
   case 20014:
	chemical = "Halon-2402";
	break;
   case 20015:
	chemical = "Methyl Chloride (HCC-40)";
	break;
   case 20016:
	chemical = "Carbon Tetrachloride (HCC-10)";
	break;
   case 20017:
	chemical = "HCC-140a  CH3CCl3";
	break;
   case 20018:
	chemical = "Methyl Bromide (HBC-40B1)";
	break;
   case 20019:
	chemical = "Hexachlorocyclohexane (HCH)";
	break;
   case 20020:
	chemical = "Alpha Hexachlorocyclohexane";
	break;
   case 20021:
	chemical = "Hexachlorobiphenyl (PCB-153)";
	break;
   case 30000:
	chemical = "Radioactive Pollutant (Tracer, defined by originating centre)";
	break;
   case 60000:
	chemical = "HOx Radical (OH+HO2)";
	break;
   case 60001:
	chemical = "Total Inorganic and Organic Peroxy Radicals (HO2+RO2)  RO2";
	break;
   case 60002:
	chemical = "Passive Ozone";
	break;
   case 60003:
	chemical = "NOx Expressed As Nitrogen  NOx";
	break;
   case 60004:
	chemical = "All Nitrogen Oxides (NOy) Expressed As Nitrogen  NOy";
	break;
   case 60005:
	chemical = "Total Inorganic Chlorine  Clx";
	break;
   case 60006:
	chemical = "Total Inorganic Bromine  Brx";
	break;
   case 60007:
	chemical = "Total Inorganic Chlorine Except HCl, ClONO2: ClOx";
	break;
   case 60008:
	chemical = "Total Inorganic Bromine Except Hbr, BrONO2:BrOx";
	break;
   case 60009:
	chemical = "Lumped Alkanes";
	break;
   case 60010:
	chemical = "Lumped Alkenes";
	break;
   case 60011:
	chemical = "Lumped Aromatic Coumpounds";
	break;
   case 60012:
	chemical = "Lumped Terpenes";
	break;
   case 60013:
	chemical = "Non-Methane Volatile Organic Compounds Expressed as Carbon  NMVOC";
	break;
   case 60014:
	chemical = "Anthropogenic Non-Methane Volatile Organic Compounds Expressed as Carbon  aNMVOC";
	break;
   case 60015:
	chemical = "Biogenic Non-Methane Volatile Organic Compounds Expressed as Carbon  bNMVOC";
	break;
   case 60016:
	chemical = "Lumped Oxygenated Hydrocarbons  OVOC";
	break;
   case 62000:
	chemical = "Total Aerosol";
	break;
   case 62001:
	chemical = "Dust Dry";
	break;
   case 62002:
	chemical = "water In Ambient";
	break;
   case 62003:
	chemical = "Ammonium Dry";
	break;
   case 62004:
	chemical = "Nitrate Dry";
	break;
   case 62005:
	chemical = "Nitric Acid Trihydrate";
	break;
   case 62006:
	chemical = "Sulphate Dry";
	break;
   case 62007:
	chemical = "Mercury Dry";
	break;
   case 62008:
	chemical = "Sea Salt Dry";
	break;
   case 62009:
	chemical = "Black Carbon Dry";
	break;
   case 62010:
	chemical = "Particulate Organic Matter Dry";
	break;
   case 62011:
	chemical = "Primary Particulate Organic Matter Dry";
	break;
   case 62012:
	chemical = "Secondary Particulate Organic Matter Dry";
	break;
   case 62013:
	chemical = "Black carbon hydrophilic dry";
	break;
   case 62014:
	chemical = "Black carbon hydrophobic dry";
	break;
   case 62015:
	chemical = "Particulate organic matter hydrophilic dry";
	break;
   case 62016:
	chemical = "Particulate organic matter hydrophobic dry";
	break;
   case 62017:
	chemical = "Nitrate hydrophilic dry";
	break;
   case 62018:
	chemical = "Nitrate hydrophobic dry";
	break;
   case 62020:
	chemical = "Smoke - high absorption";
	break;
   case 62021:
	chemical = "Smoke - low absorption";
	break;
   case 62022:
	chemical = "Aerosol - high absorption";
	break;
   case 62023:
	chemical = "Aerosol - low absorption";
	break;
   case 62025:
	chemical = "Volcanic ash";
	break;
   case 62036:
	chemical = "Brown Carbon Dry";
	break;
  default:
    chemical = "Unknown Chemical Type";
    break;
  }
  return chemical;
}

string ProdDefTemp::_getAtmoChemicalName(int chemId) const
{
  string chemName;
  switch( chemId ) {
   case 0:
        chemName = "03"; // Ozone
	break;
   case 1:
	chemName = "H2O"; // Water Vapour	
	break;
   case 2:
	chemName = "CH4"; // Methane	
	break;
   case 3:
	chemName = "CO2"; // Carbon Dioxide	
	break;
   case 4:
	chemName = "CO"; // Carbon Monoxide	
	break;
   case 5:
	chemName = "NO2"; // Nitrogen Dioxide	
	break;
   case 6:
	chemName = "N2O"; // Nitrous Oxide	
	break;
   case 7:
	chemName = "HCHO"; // Formaldehyde	
	break;
   case 8:
	chemName = "SO2"; // Sulphur Dioxide	
	break;
   case 9:
	chemName = "NH3"; // Ammonia	
	break;
   case 10:
	chemName = "NH4+"; // Ammonium	
	break;
   case 11:
	chemName = "NO"; // Nitrogen Monoxide	
	break;
   case 12:
	chemName = "O"; // Atomic Oxygen	
	break;
   case 13:
	chemName = "NO3"; // Nitrate Radical	
	break;
   case 14:
	chemName = "HO2"; // Hydroperoxyl Radical	
	break;
   case 15:
	chemName = "H2O5"; // Dinitrogen Pentoxide	
	break;
   case 16:
	chemName = "HONO"; // Nitrous Acid	
	break;
   case 17:
	chemName = "HNO3"; // Nitric Acid	
	break;
   case 18:
	chemName = "HO2NO2"; // Peroxynitric Acid	
	break;
   case 19:
	chemName = "H2O2"; // Hydrogen Peroxide	
	break;
   case 20:
	chemName = "H"; // Molecular Hydrogen	
	break;
   case 21:
	chemName = "N"; // Atomic Nitrogen	
	break;
   case 22:
	chemName = "SO42-"; // Sulphate 
	break;
   case 23:
	chemName = "Rn"; // Radon	
	break;
   case 24:
	chemName = "Hg(O)"; //Elemental Mercury	
	break;
   case 25:
	chemName = "Hg2+"; // Divalent Mercury	
	break;
   case 26:
	chemName = "Cl"; // Atomic Chlorine	
	break;
   case 27:
	chemName = "ClO"; // Chlorine Monoxide	
	break;
   case 28:
	chemName = "Cl2O2"; // Dichlorine Peroxide	
	break;
   case 29:
	chemName = "HClO"; // Hypochlorous Acid	
	break;
   case 30:
	chemName = "ClONO2"; // Chlorine Nitrate	
	break;
   case 31:
	chemName = "ClO2"; // Chlorine Dioxide	
	break;
   case 32:
	chemName = "Br"; // Atomic Bromide	
	break;
   case 33:
	chemName = "BrO"; // Bromine Monoxide	
	break;
   case 34:
	chemName = "BrCl"; // Bromine Chloride	
	break;
   case 35:
	chemName = "HBr"; // Hydrogen Bromide	
	break;
   case 36:
	chemName = "HBrO"; // Hypobromous Acid	
	break;
   case 37:
	chemName = "BrONO2"; // Bromine Nitrate	
	break;
   case 38:
	chemName = "O2"; // Oxygen	
	break;
   case 10000:
	chemName = "OH"; // Hydroxyl Radical	
	break;
   case 10001:
	chemName = "CH3O2"; // Methyl Peroxy Radical	
	break;
   case 10002:
	chemName = "CH3O2H"; // Methyl Hydroperoxide	
	break;
   case 10004:
	chemName = "CH3OH"; // Methanol	
	break;
   case 10005:
	chemName = "CH3OOH"; // Formic Acid	
	break;
   case 10006:
	chemName = "HCN"; // Hydrogen Cyanide	
	break;
   case 10007:
	chemName = "CH3CN"; // Aceto Nitrile	
	break;
   case 10008:
	chemName = "C2H6"; // Ethane	
	break;
   case 10009:
	chemName = "C2H4"; // Ethene (= Ethylene)	
	break;
   case 10010:
	chemName = "C2H2"; // Ethyne (= Acetylene)	
	break;
   case 10011:
	chemName = "C2H5OH"; // Ethanol	
	break;
   case 10012:
	chemName = "C2H5OOH"; // Acetic Acid	
	break;
   case 10013:
	chemName = "CH3C(O)OONO2"; // Peroxyacetyl Nitrate
	break;
   case 10014:
	chemName = "C3H8"; // Propane	
	break;
   case 10015:
	chemName = "C3H6"; // Propene	
	break;
   case 10016:
	chemName = "C4H10"; // Butanes	
	break;
   case 10017:
	chemName = "C5H10"; // Isoprene	
	break;
   case 10018:
	chemName = "C10H16"; // Alpha Pinene	
	break;
   case 10019:
	chemName = "C10H16"; // Beta Pinene	
	break;
   case 10020:
	chemName = "C10H16"; // Limonene	
	break;
   case 10021:
	chemName = "C6H6"; // Benzene	
	break;
   case 10022:
	chemName = "C7H8"; // Toluene	
	break;
   case 10023:
	chemName = "C8H10"; // Xylene	
	break;
   case 10500:
	chemName = "CH3SCH3"; // Dimethyl Sulphide	
	break;
   case 20001:
	chemName = "HCL"; // Hydrogen Chloride	
	break;
   case 20002:
	chemName = "CFC-11"; // CFC-11
	break;
   case 20003:
	chemName = "CFC-12	"; //CFC-12	
	break;
   case 20004:
	chemName = "CFC-113"; //CFC-113	
	break;
   case 20005:
	chemName = "CFC-113a"; //CFC-113a	
	break;
   case 20006:
	chemName = "CFC-114"; //CFC-114	
	break;
   case 20007:
	chemName = "CFC-115"; //CFC-115	
	break;
   case 20008:
	chemName = "HCFC-22"; //HCFC-22	
	break;
   case 20009:
	chemName = "HCFC-141b"; //HCFC-141b	
	break;
   case 20010:
	chemName = "HCFC-142b"; //HCFC-142b	
	break;
   case 20011:
	chemName = "Halon-1202"; //Halon-1202	
	break;
   case 20012:
	chemName = "Halon-1211"; //Halon-1211	
	break;
   case 20013:
	chemName = "Halon-1301"; //Halon-1301	
	break;
   case 20014:
	chemName = "Halon-2402"; //Halon-2402	
	break;
   case 20015:
	chemName = "HCC-40"; // Methyl Chloride (HCC-40)
	break;
   case 20016:
	chemName = "HCC-10"; // Carbon Tetrachloride (HCC-10)
	break;
   case 20017:
	chemName = "HCC-140a"; // HCC-140a	CH3CCl3
	break;
   case 20018:
	chemName = "HBC-40B1"; //Methyl Bromide (HBC-40B1)
	break;
   case 20019:
	chemName = "HCH"; // Hexachlorocyclohexane (HCH)
	break;
   case 20020:
	chemName = "AlphaHex";  // Alpha Hexachlorocyclohexane
	break;
   case 20021:
	chemName = "PCB-153"; //Hexachlorobiphenyl (PCB-153)
	break;
   case 30000:
	chemName = "RadPollutant"; //Radioactive Pollutant (Tracer, defined by originating centre)
	break;
   case 60000:
	chemName = "OH+HO2"; //HOx Radical (OH+HO2)
	break;
   case 60001:
	chemName = "HO2+RO2"; //Total Inorganic and Organic Peroxy Radicals (HO2+RO2)	RO2
	break;
   case 60002:
	chemName = "Ozone"; //Passive Ozone
	break;
   case 60003:
	chemName = "NOx"; //NOx Expressed As Nitrogen	NOx
	break;
   case 60004:
	chemName = "NOy"; //All Nitrogen Oxides (NOy) Expressed As Nitrogen	NOy
	break;
   case 60005:
	chemName = "Clx"; //Total Inorganic Chlorine	Clx
	break;
   case 60006:
	chemName = "Brx"; //Total Inorganic Bromine	Brx
	break;
   case 60007:
	chemName = "ClOx"; //Total Inorganic Chlorine Except HCl, ClONO2: ClOx
	break;
   case 60008:
	chemName = "BrOx"; //Total Inorganic Bromine Except Hbr, BrONO2:BrOx
	break;
   case 60009:
	chemName = "Alkanes"; //Lumped Alkanes
	break;
   case 60010:
	chemName = "Alkenes"; //Lumped Alkenes
	break;
   case 60011:
	chemName = "Aromatic"; //Lumped Aromatic Coumpounds
	break;
   case 60012:
	chemName = "Terpenes"; //Lumped Terpenes
	break;
   case 60013:
	chemName = "NMVOC"; //Non-Methane Volatile Organic Compounds Expressed as Carbon	NMVOC
	break;
   case 60014:
	chemName = "aNMVOC"; //Anthropogenic Non-Methane Volatile Organic Compounds Expressed as Carbon	aNMVOC
	break;
   case 60015:
	chemName = "bNMVOC"; //Biogenic Non-Methane Volatile Organic Compounds Expressed as Carbon	bNMVOC
	break;
   case 60016:
	chemName = "OVOC"; //Lumped Oxygenated Hydrocarbons	OVOC
	break;
   case 62000:
	chemName = "TotalAerosol"; //Total Aerosol
	break;
   case 62001:
	chemName = "Dust"; //Dust Dry
	break;
   case 62002:
	chemName = "AmbientWater"; //water In Ambient
	break;
   case 62003:
	chemName = "Ammonium"; //Ammonium Dry
	break;
   case 62004:
	chemName = "Nitrate"; //Nitrate Dry
	break;
   case 62005:
	chemName = "NitricAcidTrihydrate"; //Nitric Acid Trihydrate
	break;
   case 62006:
	chemName = "Sulphate"; //Sulphate Dry
	break;
   case 62007:
	chemName = "Mercury"; //Mercury Dry
	break;
   case 62008:
	chemName = "SeaSalt"; //Sea Salt Dry
	break;
   case 62009:
	chemName = "BlackCarbon"; //Black Carbon Dry
	break;
   case 62010:
	chemName = "OrganicMatter"; //Particulate Organic Matter Dry
	break;
   case 62011:
	chemName = "PrimaryOrganicMatter"; //Primary Particulate Organic Matter Dry
	break;
   case 62012:
	chemName = "SecondaryOrganicMatter"; //Secondary Particulate Organic Matter Dry
	break;
   case 62013:
	chemName = "BlackCarbonHydrophilic"; //Black carbon hydrophilic dry
	break;
   case 62014:
	chemName = "BlackCarbonHydrophobic"; //Black carbon hydrophobic dry
	break;
   case 62015:
	chemName = "OrganicMatterHydrophilic"; //Particulate organic matter hydrophilic dry
	break;
   case 62016:
	chemName = "OrganicMatterHydrophobic"; //Particulate organic matter hydrophobic dry
	break;
   case 62017:
	chemName = "NitrateHydrophilic"; //Nitrate hydrophilic dry
	break;
   case 62018:
	chemName = "NitrateHydrophobic"; //Nitrate hydrophobic dry
	break;
   case 62020:
	chemName = "SmokeHigh"; //Smoke - high absorption
	break;
   case 62021:
	chemName = "SmokeLow"; //Smoke - low absorption
	break;
   case 62022:
	chemName = "AerosolHigh"; //Aerosol - high absorption
	break;
   case 62023:
	chemName = "AerosolLow"; //Aerosol - low absorption
	break;
   case 62025:
	chemName = "VA"; //Volcanic ash
	break;
   case 62036:
	chemName = "BrownCarbonDry"; //Brown Carbon Dry
	break;
   default:
        chemName = "UNK"; //Unknown Chemical Type
      break;
  }
  return chemName;
}

void ProdDefTemp::_printInterval(FILE *stream, int sizeType, int scaleFirstSize, int valFirstSize, int scaleSecondSize, int valSecondSize) const
{

  fl32 floatFirstSize = valFirstSize;
  if(scaleFirstSize > 0 && scaleFirstSize < 127)
    floatFirstSize /= pow(10.0, scaleFirstSize);
  if(scaleFirstSize > 127 && scaleFirstSize != 255)
    floatFirstSize *= pow(10.0, scaleFirstSize & 127);

  fl32 floatSecondSize = valSecondSize;
  if(scaleSecondSize > 0 && scaleSecondSize < 127)
    floatSecondSize /= pow(10.0, scaleSecondSize);
  if(scaleSecondSize > 127 && scaleSecondSize != 255)
    floatSecondSize *= pow(10.0, scaleSecondSize & 127);

  switch( sizeType ) {
  case 0:
    fprintf(stream, "   Smaller than first limit\n");
    fprintf(stream, "     < %e\n", floatFirstSize);
    break;
  case 1:
    fprintf(stream, "   Greater than second limit\n");
    fprintf(stream, "     > %e\n", floatSecondSize);
    break;
  case 2:
    fprintf(stream, "   Between first and second limit.\n");
    fprintf(stream, "   The range include the first limit but not the second limit.\n");
    fprintf(stream, "     >= %e and  < %e\n", floatFirstSize, floatSecondSize);
    break;
  case 3:
    fprintf(stream, "   Greater than first limit\n");
    fprintf(stream, "     > %e\n", floatFirstSize);
    break;
  case 4:
    fprintf(stream, "   Smaller than the second limit\n");
    fprintf(stream, "     < %e\n", floatSecondSize);
    break;
  case 5:
    fprintf(stream, "   Smaller or equal to first limit\n");
    fprintf(stream, "     <= %e\n", floatFirstSize);
    break;
  case 6:
    fprintf(stream, "   Greater or equal to second limit\n");
    fprintf(stream, "     >= %e\n", floatSecondSize);
    break;
  case 7:
    fprintf(stream, "   Between first and second limit.\n");
    fprintf(stream, "   The range include the first limit and the second limit.\n");
    fprintf(stream, "     >= %e and  <= %e\n", floatFirstSize, floatSecondSize);
    break;
  case 8:
    fprintf(stream, "   Greater or equal to first limit\n");
    fprintf(stream, "     >= %e\n", floatFirstSize);
    break;
  case 9:
    fprintf(stream, "   Smaller or equal to second limit\n");
    fprintf(stream, "     <= %e\n", floatSecondSize);
    break;
  case 10:
    fprintf(stream, "   Between first and second limit.\n");
    fprintf(stream, "   The range include the seoncd limit but not the first limit.\n");
    fprintf(stream, "     > %e and  <= %e\n", floatFirstSize, floatSecondSize);
    break;
  case 11:
    fprintf(stream, "   Equal to the first limit\n");
    fprintf(stream, "     = %e\n", floatFirstSize);
    break;
  default:
    fprintf(stream, "   Unknown Type of Interval\n");
    break;
  }
}

} // namespace Grib2

