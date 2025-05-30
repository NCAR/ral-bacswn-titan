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
 * @file UnevenLead.cc
 */
#include <vector>
#include <algorithm>
#include <ConvWx/UnevenLead.hh>
#include <ConvWx/ConvWxConstants.hh>
#include <toolsa/LogStream.hh>

using std::vector;

//----------------------------------------------------------------
int UnevenLead::leadIndex(int leadSeconds) const
{  
   vector<int>::const_iterator i;
   i = find(_leadSeconds.begin(), _leadSeconds.end(), leadSeconds);
   if (i == _leadSeconds.end())
   {
      return convWx::BAD_INDEX;
   }
   else
   {
      return i - _leadSeconds.begin();
   }
}

//----------------------------------------------------------------
bool UnevenLead::hasAll(const std::vector<int> lt) const
{
   if (lt.size() < _leadSeconds.size())
   {
      return false;
   }
   for (size_t i=0; i<_leadSeconds.size(); ++i)
   {
      vector<int>::const_iterator j;
      j = find(lt.begin(), lt.end(), _leadSeconds[i]);
      if (j == _leadSeconds.end())
      {
         LOG(WARNING) << "Expect all the leads to be present, but "
                      << _leadSeconds[i] << " is missing";
         return false;
      }
   }
   return true;
}

//----------------------------------------------------------------
std::vector<double> UnevenLead::leadHours(void) const
{
   std::vector<double> lth;
   for (size_t i=0 ;i<_leadSeconds.size(); ++i)
   {
      lth.push_back(static_cast<double>(_leadSeconds[i])/convWx::DOUBLE_SECS_PER_HOUR);
   }
   return lth;
}
