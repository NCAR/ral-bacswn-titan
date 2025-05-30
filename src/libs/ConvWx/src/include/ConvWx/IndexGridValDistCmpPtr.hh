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
/**
 * @file IndexGridValCmpPtr.hh
 * @brief Class IndexGridValPtrCmp contains a comparison method for sorting 
 *        IndexGridValDistVal objects according to data value and distance value. 
 * @class IndexGridValPtrCmp
 * @brief IndexGridValPtrCmp contains a comparison method for sorting 
 *        IndexGridVal objects according to data value and distance value.
 */

#ifndef INDEX_GRID_VAL_DIST_PTR_CMP_HH
#define INDEX_GRID_VAL_DIST_PTR_CMP_HH

class IndexGridValDistVal;

class IndexGridValDistPtrCmp
{

public:
 
  /**
   * Default constructor 
   */
  inline IndexGridValDistPtrCmp(void) {}

  /**
   * Default destructor
   */  
  inline virtual ~IndexGridValDistPtrCmp(void) {} 

  /**
   * Comparison function for IndexGridValDistVal
   *
   * @param[in] lhs  Const pointer to an IndexGridValDistVal object
   * @param[in] rhs  Const pointer to an IndexGridValDistVal object
   *
   * @return true for successful comparison (equality)
   */
  inline bool operator()( const IndexGridValDistVal *lhs,
			  const IndexGridValDistVal *rhs) const { return *lhs < *rhs;}

protected:

private:

};

#endif
