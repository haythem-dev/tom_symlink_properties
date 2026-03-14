#================================================================================
#   csc_batches_edi_input makefile
#   04.07.2013
#   v 1.0.0
#================================================================================

#--------------------------------------------------------------------------------
# build artifact 
#--------------------------------------------------------------------------------

TARGET         = csc_batches_edi_input.bin

# assures script call (Solaris/GNU make)
GITVERSIONINFO:sh = gitversioninfo.sh
GITVERSIONINFO    = $(shell gitversioninfo.sh >/dev/null 2>&1)

#--------------------------------------------------------------------------------
# include common library versions and library depends 
#--------------------------------------------------------------------------------

BASEDIR        = ..
VARS_FILE      = $(GITVERSIONINFO)$(BASEDIR)/makevars.inc
include $(VARS_FILE)

#---------------------------------------------------------------------------------
# source directories
#---------------------------------------------------------------------------------

# sources (seperated with blank if more than one directory)
DIR_SRCS       = . \
			   infrastructure/mail

# headers (seperated with blank if more than one directory)
DIR_INCS       = . \
               infrastructure/mail

#--------------------------------------------------------------------------------
# libraries 
#--------------------------------------------------------------------------------

LIBS_STATIC    = $(LIBBATCH) $(LIBUTIL_LIB)

LIBS_SHARED    = $(QT_CORE_SHARED_LIBS) $(BASAR_PROJECT_LIBS)

# additional library dirs to add with -L like -L$(LIB_DIR)
ADD_LIB_DIRS   =

# additional compiler options like -pthread
ADD_COMP_OPTS  =  -pedantic -Wno-long-long -Werror

# additional linker options like -lpthread
#ADD_LINK_OPTS  = -pthread -lpthread
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
INCS           = $(BOOST_INC) $(BASAR_INC) $(LOG4CPLUS_INC) $(QT_INC) \
                 $(LIBBATCH_INC) $(LIBUTIL_INC) $(LIBCSCBATCH_INC)

#--------------------------------------------------------------------------------
# build rules includes
#--------------------------------------------------------------------------------

BUILD_RULES_FILE = $(MARATHON_DIR)/make/makefile.inc
include $(BUILD_RULES_FILE)
