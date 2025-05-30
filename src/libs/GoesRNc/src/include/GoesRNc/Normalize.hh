//////////////////////////////////////////////////////////////////////////
//
// Header: Normalize
//
// Author: G M Cunning
//
// Date:	April 12, 2021
//
// Description:	This class is responsible for calculating a normalized albedo.
//              The calculation is separated out into a class, so it can be shared
//              with the GoesRnetCDF2MDv and SatDerive
//
//


# ifndef    NORMALIZE_H
# define    NORMALIZE_H

// C++ include files
#include <string>

// System/RAP include files
#include <toolsa/os_config.h>
#include <dataport/port_types.h>


class Normalize {

public:

  ////////////////////
  // public members //
  ////////////////////

  ////////////////////
  // public methods //
  ////////////////////

  // use default constructor and destructor
  Normalize();
  Normalize(int method, double max_sza, double max_albedo);

  // initialize -- setup the object
  // inputs:
  //  method: normalize method; one of norm_albedo_method_t
  //  max_sza: maximum solar zenith angle threshold in degrees
  //  max_albedo: maximum albedo threshold as a percentage
  // output: none
  void init(int method, double max_sza, double max_albedo);

  // compute the noramlized albedo
  // inputs:
  //  albedo: albedo value (0.0 to 1.0)
  //  sza: solar zenith angle in degrees
  // output:
  //  normalized albedo
  double compute(double albedo, double sza);


protected:

  ///////////////////////
  // protected members //
  ///////////////////////


  ///////////////////////
  // protected methods //
  ///////////////////////

private:

  ///////////////////
  // private types //
  ///////////////////

  typedef enum {
    COS_SOL_ZEN = 0,
    SQRT_COS_SOL_ZEN = 1
  } norm_albedo_method_t;


  /////////////////////
  // private members //
  /////////////////////

  static const std::string _className;

  norm_albedo_method_t _method;

  // minimum cosine of solar zenith angle. needed to stop divide-by-zero error
  double _minCosSza;

  double _maxAlbedo;

  bool _debug;

  /////////////////////
  // private methods //
  /////////////////////

};



# endif     /* NROMALIZE_H */
