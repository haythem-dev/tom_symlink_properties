#================================================================================
#   liblockmanager makefile
#   13.07.2015
#   v 1.0.0
#================================================================================

#--------------------------------------------------------------------------------
# build artifact 
#--------------------------------------------------------------------------------

TARGET         = liblockmanager.a

#--------------------------------------------------------------------------------
# include common library versions and library depends 
#--------------------------------------------------------------------------------

VARS_FILE      = ../makevars.inc
include $(VARS_FILE)

#---------------------------------------------------------------------------------
# source directories
#---------------------------------------------------------------------------------

# sources (seperated with blank if more than one directory)
DIR_SRCS       = .

# headers (seperated with blank if more than one directory)
DIR_INCS       = .

#--------------------------------------------------------------------------------
# libraries 
#--------------------------------------------------------------------------------

LIBS_STATIC    =

LIBS_SHARED    =

# additional library dirs to add with -L like -L$(LIB_DIR)
ADD_LIB_DIRS   =

# additional compiler options like -pthread
ADD_COMP_OPTS  =

# additional linker options like -lpthread
ADD_LINK_OPTS  = 

ENABLE_LAZYLOADING=
ENABLE_BIG_TOC = 1

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
INCS           = $(BOOST_INC) $(BASAR_INC) $(LOG4CPLUS_INC)

#--------------------------------------------------------------------------------
# build rules includes
#--------------------------------------------------------------------------------

BUILD_RULES_FILE = $(MARATHON_DIR)/make/makefile.inc
include $(BUILD_RULES_FILE)
