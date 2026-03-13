#================================================================================
#   libabbauw makefile
#   07.07.2015
#   v 1.0.0
#================================================================================

#--------------------------------------------------------------------------------
# build artifact 
#--------------------------------------------------------------------------------

TARGET         = libabbauw.a

#--------------------------------------------------------------------------------
# include common library versions and library depends 
#--------------------------------------------------------------------------------

VARS_FILE      = ../makevars.inc
include $(VARS_FILE)

#---------------------------------------------------------------------------------
# source directories
#---------------------------------------------------------------------------------

# sources (seperated with blank if more than one directory)
DIR_SRCS       = . loggerpool infrastructure/accessor/* domainmodule/* \
                 componentmanager exceptions

# headers (seperated with blank if more than one directory)
DIR_INCS       = . loggerpool infrastructure/accessor/* domainmodule/* \
                 componentmanager exceptions

#--------------------------------------------------------------------------------
# libraries 
#--------------------------------------------------------------------------------

LIBS_STATIC    =

LIBS_SHARED    =

# additional library dirs to add with -L like -L$(LIB_DIR)
ADD_LIB_DIRS   =

# additional compiler options like -pthread
ADD_COMP_OPTS  = -pedantic -Wno-long-long # -Werror

# additional linker options like -lpthread
ADD_LINK_OPTS  = 

ENABLE_LAZYLOADING=

#--------------------------------------------------------------------------------
# defines
#--------------------------------------------------------------------------------

COMMON_DEFINES =

C_DEFINES      =
ESQL_DEFINES   = -DESQL_C -DZENTRAL
CPP_DEFINES    =

#--------------------------------------------------------------------------------
# header
#--------------------------------------------------------------------------------

# include paths
INCS           = -I. \
                 $(BOOST_INC) $(BASAR_INC) $(LOG4CPLUS_INC) $(QT_INC) \
                 $(LIBUTIL_INC) $(LIBLOCKMANAGER_INC)

#--------------------------------------------------------------------------------
# build rules includes
#--------------------------------------------------------------------------------

BUILD_RULES_FILE = $(MARATHON_DIR)/make/makefile.inc
include $(BUILD_RULES_FILE)
