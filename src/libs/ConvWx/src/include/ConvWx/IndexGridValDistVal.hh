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
 * @file IndexGridValDistVal.hh
 * @brief IndexGridValDistVal holds grid index, data value at the grid index,
 *                            distance to grid index with data >= threshold.
 * @class IndexGridValDistVal
 * @brief IndexGridValDistVal holds grid index, data value at the grid index,
 *                            distance to grid index with data >= threshold.
 */

#ifndef INDEXGRIDVALDISTVAL_HH
#define INDEXGRIDVALDISTVAL_HH

#include <utility> 

class  IndexGridValDistVal{

public:

  /** 
   * Constructor
   * @param[in]  index  Array index of grid point
   * @param[in]  gridVal  Data value at grid index
   */
  inline IndexGridValDistVal(const double index, const double gridVal,
                             const double distVal, const int nonZeroNeighborCount)
  {
    pIndex = index; 
    pGridVal = gridVal;
    pDistVal = distVal;
    pNonZeroNeighborCount = nonZeroNeighborCount;
  }
  

  /**
   * Default destructor  
   */
  inline virtual ~IndexGridValDistVal(void) {} 

  /** 
   *  @return pIndex  Model grid array index 
   */
  inline double getIndex(void)const {return pIndex;}

  /** 
   * @return pGridVal  Data value at model grid index 
   */
  inline double getValue(void)const {return pGridVal;}

  /** 
   * @return pDistVal  Data value at model grid index 
   */
  inline double getDist(void)const {return pDistVal;}

  /** 
   * @return pDistVal  Data value at model grid index 
   */
  inline double getNonZeroNeighborCount(void)const {return pNonZeroNeighborCount;}



  /** 
   * @param[in] a  Reference to an IndexGridVal object
   * @return true if pGridVal is less than a.pGridVal,
   *         true if pGridVal == input arguments pGridVal and 
   *         pDistVal < a.pDistVal. Else return false.
   */
  inline bool operator <(const IndexGridValDistVal& a) const
  {
    //return pGridVal < a.pGridVal;
    if ( pGridVal != 0 &&  a.pGridVal != 0)
    {
       return (pGridVal < a.pGridVal);
    }
    else if (pDistVal != a.pDistVal)
    {
       return (pDistVal > a.pDistVal);
    }
    else
    {
       return (pNonZeroNeighborCount < a.pNonZeroNeighborCount);
    }
    
  }


protected: 

private:

  /** 
   * Array index of grid point.
   */
  double pIndex;

  /** 
   * Data value at grid index.
   */
  double pGridVal;

  /** 
   * Distance value at grid index.
   */
  double pDistVal;

  /** 
   * Neighbors with non zero values at grid index.
   */
  double pNonZeroNeighborCount;
};

#endif

