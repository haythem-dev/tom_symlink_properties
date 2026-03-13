#================================================================================
#   common libs and headers
#================================================================================

#--------------------------------------------------------------------------------
#   base directories
#--------------------------------------------------------------------------------

### marathon base directory
MARATHON_DIR          = $(DEVLIB_PATH)/marathon

### informix base directory
INFORMIX_DIR          = $(INFORMIXDIR)

BASE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
EXT_DIR := $(abspath $(BASE_DIR)../ext)

#--------------------------------------------------------------------------------
#   library versions
#--------------------------------------------------------------------------------
TCACCESS_VERSION      = 8.0.11.4

#--------------------------------------------------------------------------------
#   library directories
#--------------------------------------------------------------------------------
SSUK_DIR              = $(EXT_DIR)/ssuk
BASAR_DIR             = $(SSUK_DIR)/basar
BOOST_DIR             = $(EXT_DIR)/boost
QT_DIR                = $(EXT_DIR)/qt5
LOG4CPLUS_DIR         = $(EXT_DIR)/log4cplus
TCACCESS_DIR          = $(MARATHON_DIR)/lib/tcaccess/$(TCACCESS_VERSION)
LIBUTIL_DIR           = $(EXT_DIR)/libutil
KSCCLIENT_DIR         = $(EXT_DIR)/kscclient
PPUNIXC_DIR           = $(EXT_DIR)/pplib
THRIFT_DIR            = $(EXT_DIR)/thrift
OPENSSL_DIR           = $(EXT_DIR)/openssl

LIBTENDER_DIR         = $(BASE_DIR)/libtender

#--------------------------------------------------------------------------------
#   include directories
#--------------------------------------------------------------------------------
BASAR_INC             = -I$(BASAR_DIR)/include
BOOST_INC             = -I$(BOOST_DIR)/include
INFORMIX_INC          = -I$(INFORMIX_DIR)/incl/esql
LIBUTIL_INC           = -I$(LIBUTIL_DIR)/include
LOG4CPLUS_INC         = -I$(LOG4CPLUS_DIR)/include
QT_INC                = -I$(QT_DIR)/include
TCACCESS_INC          = -I$(TCACCESS_DIR)/include
KSCCLIENT_INC         = -I$(KSCCLIENT_DIR)/include
PPUNIXC_INC           = -I$(PPUNIXC_DIR)/include
THRIFT_INC            = -I$(THRIFT_DIR)/include
OPENSSL_INC           = -I$(OPENSSL_DIR)/include

LIBTENDER_INC         = -I$(LIBTENDER_DIR)
GENCPP_INC            = -I$(LIBTENDER_DIR)/gen-cpp

#--------------------------------------------------------------------------------
#   static libraries
#--------------------------------------------------------------------------------
### others mostly local libraries
LIBTENDER_LIB         = $(LIBTENDER_DIR)/build/$(CONFVER)/libtender.a
### global libraries
LIBUTIL_LIB           = $(LIBUTIL_DIR)/lib/$(CONFVER)/libutil.a
KSCCLIENT_LIB         = $(KSCCLIENT_DIR)/lib/$(CONFVER)/kscclient.a
PPUNIXC_LIB           = $(PPUNIXC_DIR)/lib/$(CONFVER)/ppunixc.a

### ESQL libraries
ESQL_CHECKAPI_LIB     = $(INFORMIX_ESQL_LIB_DIR)/checkapi.o


THRIFT_LIB            = $(THRIFT_DIR)/libstatic/libthrift.a
THRIFTZ_LIB           = $(THRIFT_DIR)/libstatic/libthriftz.a 

LIBCRYPTO_LIB         = $(OPENSSL_DIR)/libstatic/libcrypto_a.a 
LIBSSL_LIB            = $(OPENSSL_DIR)/libstatic/libssl_a.a 

#--------------------------------------------------------------------------------
#   shared libraries
#--------------------------------------------------------------------------------
### directories for library paths

BASAR_LIB_DIR         = $(SSUK_DIR)/sharedlib/$(CONFVER)
TCACCESS_LIB_DIR      = $(TCACCESS_DIR)/lib/
LOG4CPLUS_LIB_DIR     = $(SSUK_DIR)/sharedlib/$(CONFVER)
BOOST_LIB_DIR         = $(SSUK_DIR)/sharedlib/$(CONFVER)
QT_LIB_DIR            = $(QT_DIR)/lib

INFORMIX_LIB_DIR      = $(INFORMIX_DIR)/lib
INFORMIX_ESQL_LIB_DIR = $(INFORMIX_LIB_DIR)/esql

THRIFT_LIB_DIR		  = $(THRIFT_DIR)/lib
OPENSSL_LIB_DIR		  = $(OPENSSL_DIR)/lib

### shared libs
BASAR_SHARED_LIBS     = -L$(BASAR_LIB_DIR) -lbasardebug -lbasarcmnutil \
                        -lbasarproperty  -lbasardbsql -lbasardbaspect \
                        -lbasarlogin -lbasarappl
												
BASAR_LIBS_SQL_CMNUTIL	= -L$(BASAR_LIB_DIR) $(LOG4CPLUS_SHARED_LIBS) $(BOOST_FS_SHARED_LIBS) \
                        -lbasarcmnutil -lbasardbsql -lpthread -pthread $(TCACCESS_SHARED_LIBS)

LOG4CPLUS_SHARED_LIBS = -L$(LOG4CPLUS_LIB_DIR) -llog4cplus

BOOST_SYSTEM_SHARED_LIBS = -L$(BOOST_LIB_DIR) -lboost_system -lboost_filesystem -lboost_thread -lboost_locale
BOOST_REGEX_SHARED_LIBS= -L$(BOOST_LIB_DIR) -lboost_regex
BOOST_PO_SHARED_LIBS  = -L$(BOOST_LIB_DIR) -lboost_program_options
BOOST_FS_SHARED_LIBS  = -L$(BOOST_LIB_DIR) -lboost_system -lboost_filesystem

QT_CORE_SHARED_LIBS   = -L$(QT_LIB_DIR) -lQt5Core
QT_XML_SHARED_LIBS    = -L$(QT_LIB_DIR) -lQt5Xml

TCACCESS_SHARED_LIBS  = -L$(TCACCESS_LIB_DIR) \
                        -lComHd -lChTN3270 -lChTcpIp -lSpecMap \
                        -lBml -lbmlApi -lBmlCh -lBmlUtils

BASAR_PROJECT_LIBS    = $(LOG4CPLUS_SHARED_LIBS) $(BOOST_FS_SHARED_LIBS) \
                        $(BASAR_SHARED_LIBS) -lpthread -pthread \
                        $(TCACCESS_SHARED_LIBS)

INFORMIX_SHARED_LIBS  = -L$(INFORMIX_LIB_DIR) -L$(INFORMIX_ESQL_LIB_DIR) \
                        -lifsql -lifasf -lifgen -lifos -lifgls -lifglx

SOCKET_SHARED_LIBS    = -lnsl
# remove next line when completely transitioned to AIX because this is Solaris specific
# on most unix based systems socket support is directly included in libc
SOCKET_SHARED_LIBS:sh = echo -lsocket -lnsl

VOODOO_SHARED_LIBS    = -L$(VOODOO_LIB_DIR) -lscsvoodoo

# modeline - do not remove
# vim: ff=unix ts=4 sw=4 noexpandtab syntax=make
