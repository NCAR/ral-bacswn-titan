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
 * @file UnevenLead.hh 
 * @brief Support for unevenly spaced lead times
 * @class UnevenLead
 * @brief Support for unevenly spaced lead times
 */

#ifndef UnevenLead_H
#define UnevenLead_H

#include <vector>

//----------------------------------------------------------------
class UnevenLead
{
public:
  /**
   * Empty constructor
   */
  inline UnevenLead(void) : _isSet(false) {}


  /**
   * Pass in lead seconds
   *
   * @param[in] leadSeconds  The Lead times (seconds)
   */
   inline UnevenLead(const std::vector<int> &leadSeconds) :
     _leadSeconds(leadSeconds), _isSet(!leadSeconds.empty()) {}

  /**
   * Destructor
   */
  inline virtual ~UnevenLead(void) {}


  inline bool isSet(void) const {return _isSet;}

  inline int num(void) const {return (int)_leadSeconds.size();}

   int leadIndex(int leadSeconds) const;
   bool hasAll(const std::vector<int> lt) const;

   int& operator[](size_t i)  {return _leadSeconds[i];}
   const int& operator[](size_t i)  const {return _leadSeconds[i];}

   std::vector<double> leadHours(void) const;
protected:
private:

  std::vector<int> _leadSeconds; /**< Lead times, seconds */
  bool _isSet;
};

#endif
