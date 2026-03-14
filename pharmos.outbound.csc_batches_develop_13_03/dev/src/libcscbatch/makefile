#================================================================================
#   libcscbatch makefile
#   27.02.2013
#   v 1.0.0
#================================================================================

#--------------------------------------------------------------------------------
# build artifact 
#--------------------------------------------------------------------------------

TARGET         = libcscbatch.a

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

# sources
DIR_SRCS     = .

# headers
DIR_INCS     = .


#--------------------------------------------------------------------------------
# libraries 
#--------------------------------------------------------------------------------

LIBS_STATIC    =

LIBS_SHARED    =

LIB_DIRS       =

# additional linker options like -library=stlport4 or -z lazyload
ADD_LINK_OPTS  =

# additional compiler options like -library=stlport4 (xcode=pic32 supersedes KPIC)
ADD_COMP_OPTS  = -pedantic -Wno-long-long -Werror

#--------------------------------------------------------------------------------
# defines
#--------------------------------------------------------------------------------

COMMON_DEFINES =

C_DEFINES      =
ESQL_DEFINES   =
CPP_DEFINES    =

#--------------------------------------------------------------------------------
# header
#--------------------------------------------------------------------------------

# include paths
INCS           = \
                 $(PPUNIXC_INC) \
                 $(KSCCLIENT_INC) \
                 $(KSCCLIENTWRAPPER_INC) \
                 $(BASAR_INC) \
                 $(BOOST_INC) \
                 $(LOG4CPLUS_INC) \
                 -I../ \
                 $(QT_INC)
#TODO FN remove Qt dependency in unix batches!!!
#TODO FN remove dependency from libabbauw!!!

#--------------------------------------------------------------------------------
# build rules includes
#--------------------------------------------------------------------------------

BUILD_RULES_FILE = $(MARATHON_DIR)/make/makefile.inc
include $(BUILD_RULES_FILE)

