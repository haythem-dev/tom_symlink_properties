#================================================================================
#
#   libtender makefile
#   
#================================================================================

#--------------------------------------------------------------------------------
# build artifact 
#--------------------------------------------------------------------------------

TARGET         = libtender.a

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
                 componentmanager \
                 domainmodule/* \
                 exception \
                 infrastructure \
                 infrastructure/accessor/* \
                 infrastructure/purchasedbconn \
                 infrastructure/cscclient \
                 loggerpool \
				 gen-cpp 

# headers (seperated with blank if more than one directory)
DIR_INCS       = . $(DIR_SRCS)

#--------------------------------------------------------------------------------
# libraries 
#--------------------------------------------------------------------------------

LIBS_STATIC    =	$(KSCCLIENT_LIB) \
					$(PPUNIXC_LIB) \
					$(THRIFT_LIB)  \
					$(THRIFTZ_LIB) \
					$(LIBCRYPTO_LIB) \
					$(LIBSSL_LIB)

LIBS_SHARED    =

# additional library dirs to add with -L like -L$(LIB_DIR)
ADD_LIB_DIRS   =

# additional compiler options like -pthread
ADD_COMP_OPTS  = -pedantic -Wno-long-long -mminimal-toc

# additional linker options like -lptread
ADD_LINK_OPTS  =

#--------------------------------------------------------------------------------
# defines
#--------------------------------------------------------------------------------

COMMON_DEFINES =

C_DEFINES      =
ESQL_DEFINES   = -DESQL_C
CPP_DEFINES    =

#--------------------------------------------------------------------------------
# header
#--------------------------------------------------------------------------------

# include paths
INCS           = $(BASAR_INC) \
                 $(LOG4CPLUS_INC) \
                 $(BOOST_INC) \
                 $(QT_INC) \
                 $(LIBUTIL_INC) \
				 $(KSCCLIENT_INC) \
				 $(PPUNIXC_INC) \
				 $(THRIFT_INC) \
				 $(OPENSSL_INC)\
				 $(GENCPP_INC) \
				 -I.

#--------------------------------------------------------------------------------
# build rules includes
#--------------------------------------------------------------------------------

BUILD_RULES_FILE = $(MARATHON_DIR)/make/makefile.inc
include $(BUILD_RULES_FILE)

