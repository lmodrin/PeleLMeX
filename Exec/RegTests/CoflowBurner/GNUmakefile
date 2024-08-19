
# AMReX
DIM             = 2
DEBUG           = FALSE
PRECISION       = DOUBLE
VERBOSE         = FALSE
TINY_PROFILE    = FALSE

# Compilation
COMP            = gnu
USE_MPI         = TRUE
USE_OMP         = FALSE
USE_CUDA        = FALSE
USE_HIP         = FALSE

# PeleLMeX
USE_EFIELD      = FALSE
USE_EFIELD      = TRUE

# PelePhysics
Chemistry_Model = methaneIons_diRenzo
Eos_Model	 = Fuego
Transport_Model = Simple

PELE_HOME ?= ../../..
include $(PELE_HOME)/Exec/Make.PeleLMeX
