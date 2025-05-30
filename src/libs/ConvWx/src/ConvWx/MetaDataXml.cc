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
 * @file MetaDataXml.cc
 */

//----------------------------------------------------------------
#include <cstring>
#include <string>
#include <cstdio>
#include <toolsa/LogStream.hh>
#include <ConvWx/MetaDataXml.hh>
#include <ConvWx/Xml.hh>
#include <ConvWx/InterfaceLL.hh>
#include <ConvWx/ConvWxConstants.hh>
#include <ConvWx/ParmFcst.hh>
using std::string;

//----------------------------------------------------------------
const string MetaDataXml::pModelInputTag = "model_input_forecast";
const string MetaDataXml::pModelLatencyHoursTag = "model_latency_hours";
const string MetaDataXml::pModelLatencySecondsTag = "model_latency_seconds";
const string MetaDataXml::pUvInputTag = "UV_input";
const string MetaDataXml::pUvLatencyHoursTag = "UV_latency_hours";
const string MetaDataXml::pUvLatencySecondsTag = "UV_latency_seconds";
const string MetaDataXml::pCalibrateFcstTag = "Calib_input_forecast";
const string MetaDataXml::pCalibrateObsTag = "Calib_input_obs";
const string MetaDataXml::pModelDataTag = "model_forecast";
const string MetaDataXml::pExtrapDataTag = "extrap_forecast";
const string MetaDataXml::pSkillWithPcTag = "skill_with_pc";
const string MetaDataXml::pSkillWithoutPcTag = "skill_without_pc";
const string MetaDataXml::pPcWeightTag = "pc_weight";
const string MetaDataXml::pDaysInClimoTag ="days_in_climo";
const string MetaDataXml::pFreeformTag = "freeform";

//----------------------------------------------------------------
static void sComputeLatencySeconds(double latencyHours,
                                   int &latencySeconds)
{
   if (latencyHours == convWx::BAD_LATENCY_HOURS)
   {
      LOG(ERROR) <<  "Cannot compute latency seconds from hours, no hours";
      latencySeconds = convWx::BAD_LATENCY_SECONDS;
   }
   else
   {
      ParmFcst::convertToSeconds(latencyHours, latencySeconds);
   }
}

//----------------------------------------------------------------
/**
 * @return true if inputs are same path except for the suffix.
 * This is typically when one is .mdv the other .nc
 * @param[in] path1
 * @param[in] path2
 */
static bool sOnlySuffixDifferent(const std::string &path1,
				 const std::string &path2)
{
  // create substrings up to '.' in each
  size_t i0 = path1.find(".");
  size_t i1 = path2.find(".");
  if (i0 == string::npos || i1 == string::npos)
  {
    LOG(WARNING) << "Expected file name with suffix in strings "
		 << path1 << " " <<  path2;
    return false;
  }
  string substr0 = path1.substr(0, i0);
  string substr1 = path2.substr(0, i1);
  return substr0 == substr1;
}

//----------------------------------------------------------------
/**
 * Merge input forecast state into existing forecast state
 * @param[in] name  Debug name
 * @param[in] inHas  True if new thing is true
 * @param[in] in  The new name
 * @param[in] inLatencyH  The new latency hours
 * @param[in] inLatencyS  The new latency seconds
 * @param[in,out] has  Value to merge into
 * @param[in,out] current  old name
 * @param[in,out] latencyH  Value to merge into
 * @param[in,out] latencyS  Value to merge into
 */
static void sMergeFcst(const string &name, bool inHas,
		       const string &in, double inLatencyH,
                       int inLatencyS,
                       bool &has, string &current, double &latencyH,
                       int &latencyS)
{
  if (inHas)
  {
    if (has)
    {
      LOG(WARNING) << "overwriting " << name << ":" << current << "/" << latencyH
		   << " with " << in << "/" << inLatencyH;
      LOG(WARNING) << "overwriting " << name << ":" << current << "/" << latencyS
		   << " with " << in << "/" << inLatencyS;
    }
    has = true;
    current = in;
    latencyH = inLatencyH;
    latencyS = inLatencyS;
  }
}

//----------------------------------------------------------------
/**
 * Add input forecast state into existing forecast state
 * @param[in] in  The new name
 * @param[in] inLatencyH  The new latency hours
 * @param[in] inLatencyS  The new latency seconds
 * @param[in,out] has  Value to merge into
 * @param[in,out] current  old name
 * @param[in,out] latencyH  Value to merge into
 * @param[in,out] latencyS  Value to merge into
 */
static void sAddFcst(const string &in, double inLatencyH,
                     int inLatencyS, 
		     bool &has, string &current, double &latencyH,
                     int &latencyS)
{
  if (has)
  {
    LOG(WARNING) <<  "adding " << in << "/" << inLatencyH << " to existing state " << current << "/" << latencyH;
    LOG(WARNING) <<  "adding " << in << "/" << inLatencyS << " to existing state " << current << "/" << latencyS;
  }
  has = true;
  current = in;
  latencyH = inLatencyH;
  latencyS = inLatencyS;
}

//----------------------------------------------------------------
MetaDataXml::MetaDataXml(void) :
  // don't have any thing
  pHasPcType(false),
  pPcType(PcType::UNKNOWN),
  pHasBlendType(false),
  pBlendType(BlendingType::UNKNOWN),
  pHasModelInput(false),
  pModelInput(),
  pModelLatencyHours(0),
  pModelLatencySeconds(0),
  pHasUvInput(false),
  pUvInput(),
  pUvInputLatencyHours(0),
  pUvInputLatencySeconds(0),
  pHasCalibrate(false),
  pCalibrateFcst(),
  pCalibrateObs(),
  pHasModelData(false),
  pModelData(),
  pHasExtrapData(false),
  pExtrapData(),
  pHasSkill(false),
  pSkillWithPc(0),
  pSkillWithoutPc(0),
  pHasPcWeight(false),
  pPcWeight(0),
  pHasClimo(false),
  pDaysInClimo(0),
  pHasUvUnits(false),
  pUvUnits(),
  pFreeformXml()
{
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
MetaDataXml::MetaDataXml(const string &xmlStr)
{
  pHasPcType = PcType::parseXmlOptional(xmlStr, pPcType);
  pHasBlendType = BlendingType::parseXmlOptional(xmlStr, pBlendType);
  pHasModelInput = Xml::readXmlString(xmlStr, pModelInputTag, false,
				      pModelInput);
  if (pHasModelInput)
  {
    if (!Xml::readXmlDouble(xmlStr, pModelLatencyHoursTag, true,
			    pModelLatencyHours))
    {
      LOG(ERROR) <<  "got tag " << pModelInputTag << " without tag " 
		 << pModelLatencyHoursTag;
      pModelLatencyHours = convWx::BAD_LATENCY_HOURS;
    }
    if (!Xml::readXmlInt(xmlStr, pModelLatencySecondsTag, false,
			    pModelLatencySeconds))
    {
      // for backwards compatibility, don't need this, can compute
      // on the fly using the hours
      sComputeLatencySeconds(pModelLatencyHours, pModelLatencySeconds);
    }
  }

  pHasUvInput = Xml::readXmlString(xmlStr, pUvInputTag, false,
				   pUvInput);
  if (pHasUvInput)
  {
    if (!Xml::readXmlDouble(xmlStr, pUvLatencyHoursTag, true,
			    pUvInputLatencyHours))
    {
      LOG(ERROR) <<  "got tag " << pUvInputTag << " without tag " 
		 << pUvLatencyHoursTag;
      pUvInputLatencyHours = convWx::BAD_LATENCY_HOURS;
    }
    if (!Xml::readXmlInt(xmlStr, pUvLatencySecondsTag, false,
                         pUvInputLatencySeconds))
    {
      // for backwards compatibility, don't need this, can compute
      // on the fly using the hours
      sComputeLatencySeconds(pUvInputLatencyHours, pUvInputLatencySeconds);
    }
  }

  pHasCalibrate = Xml::readXmlString(xmlStr, pCalibrateFcstTag, false,
				     pCalibrateFcst);
  if (pHasCalibrate)
  {
    if (!Xml::readXmlString(xmlStr, pCalibrateObsTag, false,
			    pCalibrateObs))
    {
      LOG(ERROR) << "got tag " << pCalibrateFcstTag << " without tag "
		 << pCalibrateObsTag;
      pCalibrateObs = "unknown";
    }
  }

  pHasModelData = Xml::readXmlString(xmlStr, pModelDataTag, false, pModelData);

  pHasExtrapData = Xml::readXmlString(xmlStr, pExtrapDataTag, false,
				      pExtrapData);
  pHasClimo = Xml::readXmlDouble(xmlStr, pDaysInClimoTag, false, pDaysInClimo);
  pHasSkill = Xml::readXmlDouble(xmlStr, pSkillWithPcTag, false,
				 pSkillWithPc);
  if (pHasSkill)
  {
    if (!Xml::readXmlDouble(xmlStr, pSkillWithoutPcTag, true,
			    pSkillWithoutPc))
    {
      LOG(ERROR) << "got tag " << pSkillWithPcTag << " but no "
		 << pSkillWithoutPcTag;
      pHasSkill = false;
    }
  }
  pHasPcWeight = Xml::readXmlDouble(xmlStr, pPcWeightTag, false, pPcWeight);

  pHasUvUnits = UvUnits::parseXmlOptional(xmlStr, pUvUnits);
  Xml::readXmlString(xmlStr, pFreeformTag, false, pFreeformXml);
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
MetaDataXml::MetaDataXml(const PcType::Pc_t phaseCorrectType) :
  // don't have any thing
  pHasPcType(true),
  pPcType(phaseCorrectType),
  pHasBlendType(false),
  pBlendType(BlendingType::UNKNOWN),
  pHasModelInput(false),
  pModelInput(),
  pModelLatencyHours(0),
  pModelLatencySeconds(0),
  pHasUvInput(false),
  pUvInput(),
  pUvInputLatencyHours(0),
  pUvInputLatencySeconds(0),
  pHasCalibrate(false),
  pCalibrateFcst(),
  pCalibrateObs(),
  pHasModelData(false),
  pModelData(),
  pHasExtrapData(false),
  pExtrapData(),
  pHasSkill(false),
  pSkillWithPc(0),
  pSkillWithoutPc(0),
  pHasPcWeight(false),
  pPcWeight(0),
  pHasClimo(false),
  pDaysInClimo(0),
  pHasUvUnits(false),
  pUvUnits(),
  pFreeformXml()
{
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
MetaDataXml::~MetaDataXml()
{
}

//----------------------------------------------------------------
bool MetaDataXml::operator==(const MetaDataXml &m) const
{
  // cheat for now look only at this string.
  return (pXmlContent == m.pXmlContent);
}

//----------------------------------------------------------------
bool MetaDataXml::similar(const MetaDataXml &m) const
{
  string s = "";
  if (!pPcSame(m, s))
  {
    return false;
  }
  if (!pBlendingSame(m, s))
  {
    return false;
  }
  if (!pModelInputSame(m, s))
  {
    return false;
  }
  if (!pUvInputSame(m, s))
  {
    return false;
  }
  // these are obsolete so no check anymore
  //bool pHasUvAdvect; 
  //bool pHasUvBackgnd;

  if (!pCalibrateSame(m, s))
  {
    return false;
  }

  if (!pModelSame(m, s))
  {
    return false;
  }
  if (!pExtrapSame(m, s))
  {
    return false;
  }

  if (!pSkillSame(m, s))
  {
    return false;
  }

  if (!pPcWeightSame(m, s))
  {
    return false;
  }

  if (!pClimoSame(m, s, false))
  {
    return false;
  }

  if (!pUvUnitsSame(m, s))
  {
    return false;
  }

  if (!pFreeformSame(m, s))
  {
    return false;
  }

  return true;
}

//----------------------------------------------------------------
string MetaDataXml::printDifferences(const MetaDataXml &m) const
{
  string s = "";
  pPcSame(m, s);
  pBlendingSame(m, s);
  pModelInputSame(m, s);
  pUvInputSame(m, s);
  pCalibrateSame(m, s);
  pModelSame(m, s);
  pExtrapSame(m, s);
  pSkillSame(m, s);
  pPcWeightSame(m, s);
  pClimoSame(m, s, true);
  pUvUnitsSame(m, s);
  pFreeformSame(m, s);
  return s;
}

//----------------------------------------------------------------
string MetaDataXml::printMajorDifferences(const MetaDataXml &m) const
{
  string s = "";
  pPcSame(m, s);
  pBlendingSame(m, s);
  pModelInputSame(m, s);
  pUvInputSame(m, s);
  pCalibrateSame(m, s);
  pModelSame(m, s);
  pExtrapSame(m, s);
  pSkillSame(m, s);
  pPcWeightSame(m, s);
  pClimoSame(m, s, false);
  pUvUnitsSame(m, s);
  pFreeformSame(m, s);
  return s;
}

//----------------------------------------------------------------
void MetaDataXml::printXmlString(void) const
{
  printf("%s\n", xmlString().c_str());
}

//----------------------------------------------------------------
string MetaDataXml::xmlString(void) const
{
  string ret = "";

  if (pHasModelInput)
  {
    ret += Xml::writeString(pModelInputTag, 0, pModelInput);
    ret += Xml::writeDouble(pModelLatencyHoursTag, 0, pModelLatencyHours,
			    "%.2lf");
    ret += Xml::writeInt(pModelLatencySecondsTag, 0, pModelLatencySeconds);
  }
  if (pHasPcType)
  {
    ret += PcType::xml(pPcType);
    ret += PcType::typeDescrXml();
  }
  if (pHasUvInput)
  {
    ret += Xml::writeString(pUvInputTag, 0,pUvInput);
    ret += Xml::writeDouble(pUvLatencyHoursTag, 0, pUvInputLatencyHours,
			    "%.2lf");
    ret += Xml::writeInt(pUvLatencySecondsTag, 0, pUvInputLatencySeconds);
  }
  if (pHasCalibrate)
  {
    ret += Xml::writeString(pCalibrateFcstTag, 0, pCalibrateFcst);
    ret += Xml::writeString(pCalibrateObsTag, 0, pCalibrateObs);
  }

  if (pHasBlendType)
  {
    ret += BlendingType::xml(pBlendType);
    ret += BlendingType::typeDescrXml();
  }
  if (pHasModelData)
  {
    ret += Xml::writeString(pModelDataTag, 0, pModelData);
  }
  if (pHasExtrapData)
  {
    ret += Xml::writeString(pExtrapDataTag, 0, pExtrapData);
  }
  if (pHasSkill)
  {
    ret += Xml::writeDouble(pSkillWithoutPcTag, 0, pSkillWithoutPc, "%010.6lf");
    ret += Xml::writeDouble(pSkillWithPcTag, 0, pSkillWithPc, "%010.6lf");
  }
  if (pHasPcWeight)
  {
    ret += Xml::writeDouble(pPcWeightTag, 0, pPcWeight, "%010.6lf");
  }
  if (pHasClimo)
  {
    ret += Xml::writeDouble(pDaysInClimoTag, 0, pDaysInClimo, "%010.6lf");
  }

  if (pHasUvUnits)
  {
    ret += pUvUnits.xml();
  }

  if (!pFreeformXml.empty())
  {
    ret += Xml::writeString(pFreeformTag, 0, pFreeformXml);
  }
  return ret;
}

//----------------------------------------------------------------
string MetaDataXml::oneLineString(void) const
{
  string ret = "";
  char buf[convWx::ARRAY_LEN_LONG];
  
  if (pHasModelInput)
  {
    sprintf(buf, "min:%s mlat:%.2lf(%d) ", pModelInput.c_str(),
	    pModelLatencyHours, pModelLatencySeconds);
    ret += buf;
  }
  if (pHasPcType)
  {
    sprintf(buf, "pc:%d ", static_cast<int>(pPcType));
    ret += buf;
  }
  if (pHasUvInput)
  {
    sprintf(buf, "uvin:%s uvlat:%.2lf(%d) ", pUvInput.c_str(),
	    pUvInputLatencyHours, pUvInputLatencySeconds);
    ret += buf;
  }
  if (pHasCalibrate)
  {
    ret += "calib ";
  }

  if (pHasBlendType)
  {
    sprintf(buf, "bt:%d ", static_cast<int>(pBlendType));
    ret += buf;
  }
  if (pHasModelData)
  {
    ret += "mod ";
  }
  if (pHasExtrapData)
  {
    ret += "ext ";
  }
  if (pHasSkill)
  {
    sprintf(buf, "skil:%.5lf/%.5lf", pSkillWithoutPc, pSkillWithPc);
    ret += buf;
  }
  if (pHasPcWeight)
  {
    sprintf(buf, "pcwt:%.5lf", pPcWeight);
    ret += buf;
  }
  if (pHasUvUnits)
  {
    ret += pUvUnits.briefDescr();
  }

  if (!pFreeformXml.empty())
  {
    ret += "freeform";
  }
  return ret;
}

//----------------------------------------------------------------
string MetaDataXml::debugDescription(void) const
{
  string ret = "";

  char buf[convWx::ARRAY_LEN_VERY_LONG];

  if (pHasModelInput)
  {
    ret += "model input:";
    ret += pModelInput;
    sprintf(buf, " latencyH%.2lf\n", pModelLatencyHours);
    sprintf(buf, " latencyS%d\n", pModelLatencySeconds);
    ret += buf;
  }

  if (pHasPcType)
  {
    sprintf(buf, "Pc type:%d\n", static_cast<int>(pPcType));
    ret += buf;
  }
  if (pHasUvInput)
  {
    ret += "UV input:";
    ret += pUvInput;
    sprintf(buf, " latencyH:%2lf\n", pUvInputLatencyHours);
    sprintf(buf, " latencyS:%d\n", pUvInputLatencySeconds);
  }
  if (pHasCalibrate)
  {
    ret += "Calib:fcst:";
    ret += pCalibrateFcst;
    ret += ",obs:";
    ret += pCalibrateObs;
    ret += "\n";
  }

  if (pHasBlendType)
  {
    sprintf(buf, "Blend type:%d\n", static_cast<int>(pBlendType));
    ret += buf;
  }
  if (pHasModelData)
  {
    ret += "model:";
    ret += pModelData;
  }
  if (pHasExtrapData)
  {
    ret += "etrap:";
    ret += pExtrapData;
  }
  if (pHasSkill)
  {
    sprintf(buf, "Skill nopc:%10.7lf withpc:%10.7lf\n", pSkillWithoutPc, 
	    pSkillWithPc);
    ret += buf;
  }
  if (pHasPcWeight)
  {
    sprintf(buf, "PcWt:%10.7lf\n", pPcWeight);
    ret += buf;
  }

  if (pHasUvUnits)
  {
    ret += pUvUnits.briefDescr();
    ret += "\n";
  }
  if (!pFreeformXml.empty())
  {
    ret += "\nFreeform:\n";
    ret += pFreeformXml.c_str();
  }
  return ret;
}

//----------------------------------------------------------------
void MetaDataXml::addInputUv(const string &uvPath,
			     double uvLatencyHours)
{
  int uvLatencySeconds;
  sComputeLatencySeconds(uvLatencyHours, uvLatencySeconds);
  sAddFcst(uvPath, uvLatencyHours, uvLatencySeconds, pHasUvInput,
           pUvInput, pUvInputLatencyHours, pUvInputLatencySeconds);
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addInputUvSeconds(const string &uvPath,
                                    int uvLatencySeconds)
{
  double uvLatencyHours = (double)uvLatencySeconds/convWx::DOUBLE_SECS_PER_HOUR;
  sAddFcst(uvPath, uvLatencyHours, uvLatencySeconds, pHasUvInput,
           pUvInput, pUvInputLatencyHours, pUvInputLatencySeconds);
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addCalibrate(const string &fcstPath, const string &obsPath)
{
  pHasCalibrate = true;
  pCalibrateFcst = fcstPath;
  pCalibrateObs = obsPath;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addModelInput(const string &path,
				double latencyHours)
{
  int latencySeconds;
  sComputeLatencySeconds(latencyHours, latencySeconds);
  sAddFcst(path, latencyHours, latencySeconds, pHasModelInput,
           pModelInput, pModelLatencyHours, pModelLatencySeconds);
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addModelInputSeconds(const string &path,
                                       int latencySeconds)
{
  double latencyHours = (double)latencySeconds/convWx::DOUBLE_SECS_PER_HOUR;
  sAddFcst(path, latencyHours, latencySeconds, pHasModelInput,
           pModelInput, pModelLatencyHours, pModelLatencySeconds);
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addModel(const string &path)
{
  if (pHasModelData)
  {
    if (path != pModelData)
    {
      LOG(WARNING) << "adding " << path << " to existing state " << pModelData;
    }
  }
  pHasModelData = true;
  pModelData = path;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addExtrap(const string &path)
{
  if (pHasExtrapData)
  {
    if (path != pExtrapData)
    {
      // special check for mdv that is now nc
      if (!sOnlySuffixDifferent(path, pExtrapData))
      {
	LOG(WARNING) << "adding " << path << " to existing state " << pExtrapData;
      }
    }
  }
  pHasExtrapData = true;
  pExtrapData = path;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::replaceExtrap(const string &path)
{
  pHasExtrapData = true;
  pExtrapData = path;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addBlending(const BlendingType::Blend_t blendingType)
{
  if (pHasBlendType)
  {
    LOG(WARNING) << "adding " << blendingType  << " to existing blend " << pBlendType;
  }
  pHasBlendType = true;
  pBlendType = blendingType;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addSkill(const double noPcSkill, const double yesPcSkill)
{
  if (pHasSkill)
  {
    LOG(WARNING) <<  "adding " << noPcSkill << "," << yesPcSkill
		 << "to existing state " << pSkillWithoutPc << "," << pSkillWithPc;
  }
  pHasSkill = true;
  pSkillWithoutPc = noPcSkill;
  pSkillWithPc = yesPcSkill;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addPcWeight(const double weight)
{
  if (pHasPcWeight)
  {
    LOG(WARNING) << "adding " << weight << " to existing state " << pPcWeight;
  }
  pHasPcWeight = true;
  pPcWeight = weight;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addDaysInClimo(const double daysInClimo)
{
  LOG(DEBUG) << "adding as days in climo: " << daysInClimo;
  pHasClimo = true;
  pDaysInClimo = daysInClimo;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::replaceModelInput(const string &path,
				    double latencyHours)
{
  pHasModelInput = true;
  pModelInput = path;
  pModelLatencyHours = latencyHours;
  sComputeLatencySeconds(latencyHours, pModelLatencySeconds);
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::replaceModelInputInt(const string &path,
                                       int latencySeconds)
{
  pHasModelInput = true;
  pModelInput = path;
  pModelLatencySeconds = latencySeconds;
  pModelLatencyHours = (double)pModelLatencySeconds/convWx::DOUBLE_SECS_PER_HOUR;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::replacePcType(const PcType::Pc_t pcType)
{
  pHasPcType = true;
  pPcType = pcType;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
PcType::Pc_t MetaDataXml::getPcType(void) const
{
  if (pHasPcType)
  {
    return pPcType;
  }
  else
  {
    return PcType::UNKNOWN;
  }
}

//----------------------------------------------------------------
string MetaDataXml::getPcDescr(void) const
{
  if (pHasPcType)
  {
    return PcType::correctionString(pPcType);
  }
  else
  {
    return PcType::correctionStringNone();
  }
}

//----------------------------------------------------------------
bool MetaDataXml::getModelLatencyHours(double &latency) const
{
  if (pHasModelInput)
  {
    latency = pModelLatencyHours;
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------
bool MetaDataXml::getModelLatencySeconds(int &latency) const
{
  if (pHasModelInput)
  {
    latency = pModelLatencySeconds;
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------
bool MetaDataXml::getInputUvLatencyHours(double &latency) const
{
  if (pHasUvInput)
  {
    latency = pUvInputLatencyHours;
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------
bool MetaDataXml::getInputUvLatencySeconds(int &latency) const
{
  if (pHasUvInput)
  {
    latency = pUvInputLatencySeconds;
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------
bool MetaDataXml::getSkill(double &noPcSkill, double &yesPcSkill) const
{
  if (pHasSkill)
  {
    noPcSkill = pSkillWithoutPc;
    yesPcSkill = pSkillWithPc;
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------
bool MetaDataXml::getPcWeight(double &weight) const
{
  if (pHasPcWeight)
  {
    weight = pPcWeight;
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------
bool MetaDataXml::getClimoDays(double &daysInAve) const
{
  if (pHasClimo)
  {
    daysInAve  = pDaysInClimo;
    return true;
  }
  else
  {
    return false;
  }
}


//----------------------------------------------------------------
void MetaDataXml::replaceUvUnits(const UvUnits &units)
{
  pHasUvUnits = true;
  pUvUnits = units;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
UvUnits MetaDataXml::getUvUnits(void) const
{
  if (pHasUvUnits)
  {
    return pUvUnits;
  }
  else
  {
    return UvUnits();
  }
}

//----------------------------------------------------------------
void MetaDataXml::merge(const MetaDataXml &mergeData)
{
  if (mergeData.pHasPcType)
  {
    if (pHasPcType)
    {
      LOG(WARNING) << "overwriting pc:" << pPcType << " with " << mergeData.pPcType;
    }
    pHasPcType = true;
    pPcType = mergeData.pPcType;
  }
  if (mergeData.pHasBlendType)
  {
    if (pHasBlendType)
    {
      LOG(WARNING) << "overwriting blend:" << pBlendType << " with " << mergeData.pBlendType;
    }
    pHasBlendType = true;
    pBlendType = mergeData.pBlendType;
  }

  sMergeFcst("model", mergeData.pHasModelInput, mergeData.pModelInput,
	     mergeData.pModelLatencyHours, mergeData.pModelLatencySeconds,
             pHasModelInput, pModelInput, pModelLatencyHours, pModelLatencySeconds);
  sMergeFcst("input uv", mergeData.pHasUvInput, mergeData.pUvInput,
	     mergeData.pUvInputLatencyHours, mergeData.pUvInputLatencySeconds,
             pHasUvInput, pUvInput, pUvInputLatencyHours, pUvInputLatencySeconds);
  if (mergeData.pHasCalibrate)
  {
    if (pHasCalibrate)
    {
      LOG(WARNING) << "overwriting calibrate:" << pCalibrateFcst << "," << pCalibrateObs << " with "
		   << mergeData.pCalibrateFcst << "," << mergeData.pCalibrateObs;
    }
    pHasCalibrate = true;
    pCalibrateFcst = mergeData.pCalibrateFcst;
    pCalibrateObs = mergeData.pCalibrateObs;
  }
  if (mergeData.pHasModelData)
  {
    if (pHasModelData)
    {
      LOG(WARNING) <<  "overwriting model:" << pModelData << " with " << mergeData.pModelData;
    }
    pHasModelData = true;
    pModelData = mergeData.pModelData;
  }
  if (mergeData.pHasExtrapData)
  {
    if (pHasExtrapData)
    {
      LOG(WARNING) <<  "overwriting extrap:" << pExtrapData << " with " << mergeData.pExtrapData;
    }
    pHasExtrapData = true;
    pExtrapData = mergeData.pExtrapData;
  }
  if (mergeData.pHasSkill)
  {
    if (pHasSkill)
    {
      LOG(WARNING) << "overwriting skill:" 
		   << pSkillWithoutPc << "/" << pSkillWithPc << " with "
		   << mergeData.pSkillWithoutPc << "/" <<  mergeData.pSkillWithPc;
    }
    pHasSkill = true;
    pSkillWithoutPc = mergeData.pSkillWithoutPc;
    pSkillWithPc = mergeData.pSkillWithPc;
  }
  if (mergeData.pHasPcWeight)
  {
    if (pHasPcWeight)
    {
      LOG(WARNING) <<  "overwriting Pcweight:" << pPcWeight << " with " << mergeData.pPcWeight;
    }
    pHasPcWeight = true;
    pPcWeight = mergeData.pPcWeight;
  }
  if (mergeData.pHasUvUnits)
  {
    if (pHasUvUnits)
    {
      LOG(WARNING) <<  "overwriting uvUnits:" << pUvUnits.briefDescr() << " with " << mergeData.pUvUnits.briefDescr();
    }
    pHasUvUnits = true;
    pUvUnits = mergeData.pUvUnits;
  }
  pMergeFreeform(mergeData);
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
void MetaDataXml::addFreeformXml(const string &xmlStr)
{
  pFreeformXml += xmlStr;
  pXmlContent = xmlString();
}

//----------------------------------------------------------------
bool MetaDataXml::checkInputUV(int &latencySeconds) const
{
  //double latency;
  // shouldn't this be getInputUvLatencySeconds??
  if (!getModelLatencySeconds(latencySeconds))
  {
    // It should be there so speak up if no
    LOG(ERROR) << "no model latency hours in metadata";
    return false;
  }

  // // Turn it into seconds for return
  // latencySeconds = static_cast<int>(latency*convWx::DOUBLE_SECS_PER_HOUR);
  return true;
}

//----------------------------------------------------------------
bool MetaDataXml::getModelDataPath(std::string &path) const
{
  if (pHasModelData)
  {
    path = pModelData;
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------
bool MetaDataXml::getModelGenLead(time_t &genTime, int &lt) const
{
  if (pHasModelData)
  {
    return InterfaceLL::dataPathParse(pModelData, genTime, lt);
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------
void MetaDataXml::pMergeFreeform(const MetaDataXml &mergeData)
{
  if (!pFreeformXml.empty())
  {
    if (!mergeData.pFreeformXml.empty())
    {
      pFreeformXml += "\n";
      pFreeformXml += mergeData.pFreeformXml;
    }
  }
  else
  {
    pFreeformXml = mergeData.pFreeformXml;
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pPcSame(const MetaDataXml &compareData, string &ret) const
{
  if (pHasPcType != compareData.pHasPcType)
  {
    ret += "  MetaDataXml::PcType is set in one of two\n";
    return false;
  }
  else
  {
    if (pHasPcType && pPcType != compareData.pPcType)
    {
      ret += "  MetaDataXml::PcType diff\n\t";
      ret += PcType::xml(pPcType);
      ret += "\n\t";
      ret += PcType::xml(compareData.pPcType);
      ret += "\n";
      return false;
    }
    else
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pBlendingSame(const MetaDataXml &compareData,
				string &ret) const
{
  if (pHasBlendType != compareData.pHasBlendType)
  {
    ret += "  MetaDataXml::BlendingType is set in one of two\n";
    return false;
  }
  else
  {
    if (pHasBlendType && pBlendType != compareData.pBlendType)
    {
      ret += "  MetaDataXml::PcType diff\n\t";
      ret += BlendingType::xml(pBlendType);
      ret += "\n\t";
      ret += BlendingType::xml(compareData.pBlendType);
      ret += "\n";
      return false;
    }
    else
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pModelInputSame(const MetaDataXml &compareData,
				  string &ret) const
{
  if (pHasModelInput != compareData.pHasModelInput)
  {
    ret += "  MetaDataXml::ModelInput is set in one of two\n";
    return false;
  }
  else
  {
    if (pHasModelInput &&
	(pModelInput != compareData.pModelInput ||
	 pModelLatencyHours != compareData.pModelLatencyHours ||
         pModelLatencySeconds != compareData.pModelLatencySeconds))
    {
      ret += "  MetaDataXml::ModelInput diff\n\t";
      ret += Xml::writeString(pModelInputTag, 0, pModelInput) + "\t";
      ret += Xml::writeString(pModelInputTag, 0, compareData.pModelInput)+"\t";
      ret += Xml::writeDouble(pModelLatencyHoursTag, 0, pModelLatencyHours,
			      "%.2lf") + "\t";
      ret += Xml::writeDouble(pModelLatencyHoursTag, 0,
			      compareData.pModelLatencyHours,"%.2lf") + "\t";
      ret += Xml::writeInt(pModelLatencySecondsTag, 0, pModelLatencySeconds) + "\t";
      ret += Xml::writeInt(pModelLatencySecondsTag, 0, compareData.pModelLatencySeconds);
      return false;
    }
    else
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pUvInputSame(const MetaDataXml &compareData,
			       string &ret) const
{
  if (pHasUvInput != compareData.pHasUvInput)
  {
    ret += "  MetaDataXml::UvInput is set in one of two\n";
    return false;
  }
  else
  {
    if (pHasUvInput &&
	(pUvInput != compareData.pUvInput ||
	 pUvInputLatencySeconds != compareData.pUvInputLatencySeconds ||
	 pUvInputLatencyHours != compareData.pUvInputLatencyHours))
    {
      ret += "  MetaDataXml::UvInput diff\n\t";
      ret += Xml::writeString(pUvInputTag, 0, pUvInput) + "\t";
      ret += Xml::writeString(pUvInputTag, 0, compareData.pUvInput) + "\t";
      ret += Xml::writeDouble(pUvLatencyHoursTag, 0, pUvInputLatencyHours,
			      "%.2lf") + "\t";
      ret += Xml::writeDouble(pUvLatencyHoursTag, 0,
			      compareData.pUvInputLatencyHours, "%.2lf") + "\t";
      ret += Xml::writeInt(pUvLatencySecondsTag, 0, pUvInputLatencySeconds) + "\t";
      ret += Xml::writeInt(pUvLatencyHoursTag, 0, compareData.pUvInputLatencySeconds);
      return false;
    }
    else
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pCalibrateSame(const MetaDataXml &compareData,
				 string &ret) const
{
  if (pHasCalibrate != compareData.pHasCalibrate)
  {
    ret += "  MetaDataXml::Calibrate is set in one of two\n";
    return false;
  }
  else
  {
    if (pHasCalibrate && (pCalibrateFcst != compareData.pCalibrateFcst ||
			  pCalibrateObs != compareData.pCalibrateObs))
    {
      ret += "  MetaDataXml::Calibrate diff\n\t";
      ret += Xml::writeString(pCalibrateFcstTag, 0, pCalibrateFcst);
      ret += "\t";
      ret += Xml::writeString(pCalibrateFcstTag, 0, compareData.pCalibrateFcst);
      ret += "\t";
      ret += Xml::writeString(pCalibrateObsTag, 0, pCalibrateObs);
      ret += "\t";
      ret += Xml::writeString(pCalibrateObsTag, 0, compareData.pCalibrateObs);
      return false;
    }
    else
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pModelSame(const MetaDataXml &compareData, string &ret) const
{
  if (pHasModelData != compareData.pHasModelData)
  {
    ret += "  MetaDataXml::Model is set in one of two\n";
    return false;
  }
  else
  {
    if (pHasModelData && (pModelData != compareData.pModelData))
    {
      ret += "  MetaDataXml::Model diff\n\t";
      ret += Xml::writeString(pModelDataTag, 0, pModelData) + "\t";
      ret += Xml::writeString(pModelDataTag, 0, compareData.pModelData);
      return false;
    }
    else
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pExtrapSame(const MetaDataXml &compareData, string &ret) const
{
  if (pHasExtrapData != compareData.pHasExtrapData)
  {
    ret += "  MetaDataXml::Extrap is set in one of two\n";
    return false;
  }
  else
  {
    if (pHasExtrapData && (pExtrapData != compareData.pExtrapData))
    {
      ret += "  MetaDataXml::Extrap diff\n\t";
      ret += Xml::writeString(pExtrapDataTag, 0, pExtrapData) + "\t";
      ret += Xml::writeString(pExtrapDataTag, 0, compareData.pExtrapData);
      return false;
    }
    else
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pSkillSame(const MetaDataXml &compareData, string &ret) const
{
  if (pHasSkill != compareData.pHasSkill)
  {
    ret += "  MetaDataXml::Skill set in one of two\n";
    return false;
  }
  else
  {
    if (pHasSkill && (pSkillWithoutPc != compareData.pSkillWithoutPc ||
		      pSkillWithPc != compareData.pSkillWithPc))
    {
      ret += "  MetaDataXml::Skill diff\n\t";
      ret += Xml::writeDouble(pSkillWithoutPcTag, 0, pSkillWithoutPc,
			      "%010.6lf") + "\t";
      ret += Xml::writeDouble(pSkillWithoutPcTag, 0,
			      compareData.pSkillWithoutPc, "%010.6lf") + "\t";
      ret += Xml::writeDouble(pSkillWithPcTag, 0, pSkillWithPc,
			      "%010.6lf") + "\t";
      ret += Xml::writeDouble(pSkillWithPcTag, 0, compareData.pSkillWithPc,
			      "%010.6lf");
      return false;
    }
    else
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pPcWeightSame(const MetaDataXml &compareData, string &ret) const
{
  if (pHasPcWeight != compareData.pHasPcWeight)
  {
    ret += "  MetaDataXml::PcWeight set in one of two\n";
    return false;
  }
  else
  {
    if (pHasPcWeight && (pPcWeight != compareData.pPcWeight))
    {
      ret += "  MetaDataXml::PcWeight diff\n\t";
      ret += Xml::writeDouble(pPcWeightTag, 0, pPcWeight, "%010.6lf") + "\t";
      ret += Xml::writeDouble(pPcWeightTag, 0, compareData.pPcWeight,
			      "%010.6lf") + "\t";
      return false;
    }
    else
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pClimoSame(const MetaDataXml &compareData, string &ret,
			     const bool full) const
{
  if (pHasClimo != compareData.pHasClimo)
  {
    ret += "  MetaDataXml::Climo set in one of two\n";
    return false;
  }
  else
  {
    if (full)
    {
      if (pHasClimo && (pDaysInClimo != compareData.pDaysInClimo))
      {
	ret += "  MetaDataXml::Climo diff\n\t";
	ret += Xml::writeDouble(pDaysInClimoTag, 0, pDaysInClimo,
				"%010.6lf") + "\t";
	ret += Xml::writeDouble(pDaysInClimoTag, 0, compareData.pDaysInClimo,
				"%010.6lf") + "\t";
	return false;
      }
    }
    return true;
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pUvUnitsSame(const MetaDataXml &compareData,
			       string &ret) const
{
  if (pHasUvUnits != compareData.pHasUvUnits)
  {
    ret += "  MetaDataXml::UvUnits set in one of two\n";
    return false;
  }
  else
  {
    if (pHasUvUnits && !(pUvUnits == compareData.pUvUnits))
    {
      ret += "  MetaDataXml::UvUnits diff\n\t";
      ret += pUvUnits.xml() + "\t";
      ret += compareData.pUvUnits.xml();
      return false;
    }
    else
    {
      return true;
    }
  }
}

//----------------------------------------------------------------
bool MetaDataXml::pFreeformSame(const MetaDataXml &compareData,
				string &ret) const
{
  if (pFreeformXml != compareData.pFreeformXml)
  {
    ret += "  MetaDataXml::Freeform xml not equal\n\t";
    ret += Xml::writeString(pFreeformTag, 0, pFreeformXml) + "\t";
    ret += Xml::writeString(pFreeformTag, 0, compareData.pFreeformXml);
    return false;
  }
  else
  {
    return true;
  }
}
