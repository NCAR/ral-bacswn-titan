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
/////////////////////////////////////////////////////////////
// TriangMeshContourAlg.hh
//
// Class implementing a triangular mesh contour algorithm.
//
// Nancy Rehak, RAP, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// November 2000
//
///////////////////////////////////////////////////////////////

#ifndef TriangMeshContourAlg_H
#define TriangMeshContourAlg_H

#include <iostream>

#include <contour/Contour.hh>
#include <contour/ContourAlg.hh>


//class TriangMeshContourAlg : public ContourAlg
class TriangMeshContourAlg
{
  
public:

  ////////////////////////////
  // Initialization methods //
  ////////////////////////////

  // constructor

  TriangMeshContourAlg(const bool debug_flag = false);

  // destructor
  
  ~TriangMeshContourAlg();

  Contour *generateContour(float **d,
			   int ilb,
			   int iub,
			   int jlb,
			   int jub,
			   double *x,
			   double *y,
			   int nc,
			   double *z);
  
protected:
  
private:

  /////////////////////
  // Private members //
  /////////////////////


  /////////////////////
  // Private methods //
  /////////////////////

};

#endif
