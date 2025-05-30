######################################################################
#
# Makefile for GoesRNc/GoesRNcFile
#
# Gary Cunning, RAL, NCAR, Boulder, Co, USA, 80307
#
# Sept 2017
#
######################################################################

include $(RAP_MAKE_INC_DIR)/rap_make_macros

TARGET_FILE = ../libGoesRNc.a

LOC_INCLUDES = -I../include $(NETCDF4_INCS)
LOC_CFLAGS =


#
# file lists
#

HDRS = \
	../include/GoesRNc/AerosolDetectionProduct.hh \
	../include/GoesRNc/AerosolOpticalDepthProduct.hh \
	../include/GoesRNc/ClearSkyMaskProduct.hh \
	../include/GoesRNc/CloudOpticalDepthProduct.hh \
	../include/GoesRNc/CloudParticleSizeProduct.hh \
	../include/GoesRNc/CloudTopHeightProduct.hh \
	../include/GoesRNc/CloudTopPhaseProduct.hh \
	../include/GoesRNc/CloudTopPressureProduct.hh \
	../include/GoesRNc/CloudTopTemperatureProduct.hh \
	../include/GoesRNc/DerivedStabilityIndicesProduct.hh \
	../include/GoesRNc/FireCharacterizationProduct.hh \
	../include/GoesRNc/GoesRProduct.hh \
	../include/GoesRNc/LandSurfaceTemperatureProduct.hh \
	../include/GoesRNc/LegacyVerticalMoistureProfileProduct.hh \
	../include/GoesRNc/LegacyVerticalTemperatureProfileProduct.hh \
	../include/GoesRNc/Level2Products.hh \
	../include/GoesRNc/RadianceProduct.hh \
	../include/GoesRNc/RainfallRateQpeProduct.hh \
	../include/GoesRNc/SeaSurfaceTemperatureProduct.hh \
	../include/GoesRNc/TotalPrecipitableWaterProduct.hh \
	../include/GoesRNc/VolcanicAshProduct.hh


CPPC_SRCS = \
	AerosolDetectionProduct.cc \
	AerosolOpticalDepthProduct.cc \
	ClearSkyMaskProduct.cc \
	CloudOpticalDepthProduct.cc \
	CloudParticleSizeProduct.cc \
	CloudTopHeightProduct.cc \
	CloudTopPhaseProduct.cc \
	CloudTopPressureProduct.cc \
	CloudTopTemperatureProduct.cc \
	DerivedStabilityIndicesProduct.cc \
	FireCharacterizationProduct.cc \
	GoesRProduct.cc \
	LandSurfaceTemperatureProduct.cc \
	LegacyVerticalMoistureProfileProduct.cc \
	LegacyVerticalTemperatureProfileProduct.cc \
	RadianceProduct.cc \
	RainfallRateQpeProduct.cc \
	SeaSurfaceTemperatureProduct.cc \
	TotalPrecipitableWaterProduct.cc \
	VolcanicAshProduct.cc

#
# general targets
#

include $(RAP_MAKE_INC_DIR)/rap_make_lib_module_targets

#
# local targets
#

depend: depend_generic

# DO NOT DELETE THIS LINE -- make depend depends on it.
