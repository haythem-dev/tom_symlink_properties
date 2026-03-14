#================================================================================
#   cscbatchauto makefile
#   27.02.2013
#   v 1.0.0
#================================================================================

#--------------------------------------------------------------------------------
# build artifact 
#--------------------------------------------------------------------------------

TARGET         = cscbatchauto.bin

#--------------------------------------------------------------------------------
# include common library versions and library depends 
#--------------------------------------------------------------------------------

VARS_FILE      = ../makevars.inc
include $(VARS_FILE)

#---------------------------------------------------------------------------------
# source directories
#---------------------------------------------------------------------------------

# sources (separated with blank if more than one directory)
DIR_SRCS       = .

# headers (separated with blank if more than one directory)
DIR_INCS       = .

#--------------------------------------------------------------------------------
# libraries 
#--------------------------------------------------------------------------------

LIBS_STATIC    = $(LIBCSCBATCH) $(KSCCLIENT_LIB) $(KSCCLIENTWRAPPER_LIB) $(PPUNIXC_LIB) -liconv

LIBS_SHARED    = $(QT_CORE_SHARED_LIBS) $(BASAR_PROJECT_LIBS)

# additional library dirs to add with -L like -L$(LIB_DIR)
ADD_LIB_DIRS   =

# additional compiler options like -pthread
ADD_COMP_OPTS  = -pedantic -Wno-long-long -Werror

# additional linker options like -lpthread
ADD_LINK_OPTS  = -pthread -lpthread

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
INCS           = \
		$(BOOST_INC) \
		$(BASAR_INC) \
		$(LOG4CPLUS_INC) \
		$(QT_INC) \
		$(KSCCLIENTWRAPPER_INC) \
		$(LIBCSCBATCH_INC) \
		-I..

#--------------------------------------------------------------------------------
# build rules includes
#--------------------------------------------------------------------------------

BUILD_RULES_FILE = $(MARATHON_DIR)/make/makefile.inc
include $(BUILD_RULES_FILE)
