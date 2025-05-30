######################################################################
#
# Makefile for GoesRNc/Angles
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
	../include/GoesRNc/Angles.hh


CPPC_SRCS = \
	Angles.cc \

#
# general targets
#

include $(RAP_MAKE_INC_DIR)/rap_make_lib_module_targets

#
# local targets
#

depend: depend_generic

# DO NOT DELETE THIS LINE -- make depend depends on it.
