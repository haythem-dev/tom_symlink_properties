 #================================================================================
#   common libs and headers
#================================================================================

#--------------------------------------------------------------------------------
# base directories
#--------------------------------------------------------------------------------

### marathon base directory
MARATHON_DIR          = $(DEVLIB_PATH)/marathon
BASE_DIR             := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
EXT_DIR              := $(abspath $(BASE_DIR)../ext)

### informix base directory
INFORMIX_DIR          = $(INFORMIXDIR)

#--------------------------------------------------------------------------------
# library versions
#--------------------------------------------------------------------------------
ARITHDEC_VERSION      = 1.1.0.1
SSUK_VERSION          = ssuk-01.13.02.00
#BOOST_VERSION         = 1.41.0_2
CXXTOOLS_VERSION      = 1.1.0.3
KSCCLIENT_VERSION     = 01.02.03.00
KSCCLIENTWRAPPER_VERSION = 01.00.00.09
HDATUM_VERSION        = 1.1.0.1
LIBABBAUW_VERSION     =
LIBBATCH_VERSION      =
LIBAMCQUOTA_VERSION   =
LIBCSCBATCH_VERSION   =
LIBCALLPLAN_VERSION   = 1.1.1.4
LIBLOCKMANAGER_VERSION=
LIBTPLD_VERSION       = 1.1.0.2
LIBUMK_VERSION        = 1.1.0.1
LIBUTIL_VERSION       = 00.04.06.00_ssuk-01.13.02.00
LOCKMAN_VERSION       = 1.1.0.2
#LOG4CPLUS_VERSION     = 1.1.2_1
PPUNIXC_VERSION       = 1.1.0.5
PXDB_VERSION          = 1.1.0.1
PXVERBUND_VERSION     = 1.0.2.0
#QT_VERSION            = 4.4.3_2
TCACCESS_VERSION      = 8.0.11.4
TCAWRAP_VERSION       = 1.1.0.2
VOODOO_VERSION        = 01.13.01.01

#--------------------------------------------------------------------------------
# library directories
#--------------------------------------------------------------------------------
### static library dirs
LIBABBAUW_DIR         = ../libabbauw
LIBBATCH_DIR          = ../libbatch
LIBCSCBATCH_DIR       = ../libcscbatch
LIBLOCKMANAGER_DIR    = ../lockman
LIBUTIL_DIR           = $(EXT_DIR)/libutil

#CXXTOOLS_DIR          = $(MARATHON_DIR)/lib/cxxtools/$(CXXTOOLS_VERSION)
#PXVERBUND_DIR         = $(MARATHON_DIR)/orderentry/pxverbund/$(PXVERBUND_VERSION)
#LOCKMAN_DIR           = $(MARATHON_DIR)/orderentry/lockman/$(LOCKMAN_VERSION)
#PXDB_DIR              = $(MARATHON_DIR)/orderentry/pxdb/$(PXDB_VERSION)

#ARITHDEC_DIR          = $(MARATHON_DIR)/lib/arithdec/$(ARITHDEC_VERSION)
PPUNIXC_DIR           = $(EXT_DIR)/pplib

#TCAWRAP_DIR           = $(MARATHON_DIR)/lib/tcawrap/$(TCAWRAP_VERSION)

### shared library dirs
SSUK_DIR              = $(EXT_DIR)/ssuk
BASAR_DIR             = $(SSUK_DIR)/basar
TCACCESS_DIR          = $(MARATHON_DIR)/lib/tcaccess/$(TCACCESS_VERSION)
LOG4CPLUS_DIR         = $(EXT_DIR)/log4cplus

#BOOST_DIR             = $(MARATHON_DIR)/lib/boost/$(BOOST_VERSION)
BOOST_DIR             = $(EXT_DIR)/boost
#QT_DIR                = $(MARATHON_DIR)/lib/qt/$(QT_VERSION)
QT_DIR                = $(EXT_DIR)/qt5

KSCCLIENT_DIR         = $(EXT_DIR)/kscclient
KSCCLIENTWRAPPER_DIR  = $(EXT_DIR)/kscclientwrapper
#HDATUM_DIR            = $(MARATHON_DIR)/lib/hdatum/$(HDATUM_VERSION)

#LIBCALLPLAN_DIR       = $(MARATHON_DIR)/orderentry/libcallplan/$(LIBCALLPLAN_VERSION)
#LIBTPLD_DIR           = $(MARATHON_DIR)/orderentry/libtpld/$(LIBTPLD_VERSION)
#LIBUMK_DIR            = $(MARATHON_DIR)/orderentry/libumk/$(LIBUMK_VERSION)

#VOODOO_DIR            = $(MARATHON_DIR)/lib/servclnt/$(VOODOO_VERSION)

#--------------------------------------------------------------------------------
# include directories
#--------------------------------------------------------------------------------
ARITHDEC_INC          = -I$(ARITHDEC_DIR)/include
BASAR_INC             = -I$(BASAR_DIR)/include
BOOST_INC             = -I$(BOOST_DIR)/include
CXXTOOLS_INC          = -I$(CXXTOOLS_DIR)/include
KSCCLIENT_INC         = -I$(KSCCLIENT_DIR)/include
KSCCLIENTWRAPPER_INC  = -I$(KSCCLIENTWRAPPER_DIR)/include
HDATUM_INC            = -I$(HDATUM_DIR)/include
INFORMIX_INC          = -I$(INFORMIX_DIR)/incl/esql
LIBABBAUW_INC         = -I$(LIBABBAUW_DIR)
LIBBATCH_INC          = -I$(LIBBATCH_DIR)
LIBAMCQUOTA_INC       = -I$(LIBAMCQUOTA_DIR)
LIBCSCBATCH_INC       = -I$(LIBCSCBATCH_DIR)
LIBCALLPLAN_INC       = -I$(LIBCALLPLAN_DIR)/include
LIBLOCKMANAGER_INC    = -I$(LIBLOCKMANAGER_DIR)
LIBTPLD_INC           = -I$(LIBTPLD_DIR)/include
LIBUMK_INC            = -I$(LIBUMK_DIR)/include
LIBUTIL_INC           = -I$(LIBUTIL_DIR)/include
LOCKMAN_INC           = -I$(LOCKMAN_DIR)/include
LOG4CPLUS_INC         = -I$(LOG4CPLUS_DIR)/include
PPUNIXC_INC           = -I$(PPUNIXC_DIR)/include
PXDB_INC              = -I$(PXDB_DIR)/include
PXVERBUND_INC         = -I$(PXVERBUND_DIR)/include
TCACCESS_INC          = -I$(TCACCESS_DIR)/include
TCAWRAP_INC           = -I$(TCAWRAP_DIR)/include
QT_INC                = -I$(QT_DIR)/include
QT_CORE_INC           = -I$(QT_DIR)/include/QtCore
QT_XML_INC            = -I$(QT_DIR)/include/QtXml
VOODOO_INC            = -I$(VOODOO_DIR)/include

#--------------------------------------------------------------------------------
# static libraries
#--------------------------------------------------------------------------------
### others
LIBABBAUW             = $(LIBABBAUW_DIR)/build/$(CONFVER)/libabbauw.a
LIBBATCH              = $(LIBBATCH_DIR)/build/$(CONFVER)/libbatch.a
LIBAMCQUOTA           = $(LIBAMCQUOTA_DIR)/build/$(CONFVER)/libamcquota.a
LIBCSCBATCH           = $(LIBCSCBATCH_DIR)/build/$(CONFVER)/libcscbatch.a
LIBLOCKMANAGER        = $(LIBLOCKMANAGER_DIR)/build/$(CONFVER)/liblockmanager.a

CXXTOOLS_LIB          = $(CXXTOOLS_DIR)/lib/$(CONFVER)/cxxtools.a
PXVERBUND_LIB         = $(PXVERBUND_DIR)/lib/$(CONFVER)/pxserverd.a
LOCKMAN_LIB           = $(LOCKMAN_DIR)/lib/$(CONFVER)/lockman.a
PXDB_LIB              = $(PXDB_DIR)/lib/$(CONFVER)/sqldbxxKd.a

ARITHDEC++_LIB        = $(ARITHDEC_DIR)/lib/$(CONFVER)/arithdec++.a
ARITHDEC_LIB          = $(ARITHDEC_DIR)/lib/$(CONFVER)/arithdec.a

PPUNIXC_LIB           = $(PPUNIXC_DIR)/lib/$(CONFVER)/ppunixc.a

TCAWRAP_LIB           = $(TCAWRAP_DIR)/lib/$(CONFVER)/tcawrap.a

KSCCLIENT_LIB         = $(KSCCLIENT_DIR)/lib/$(CONFVER)/kscclient.a
KSCCLIENTWRAPPER_LIB  = $(KSCCLIENTWRAPPER_DIR)/lib/$(CONFVER)/kscclientwrapper.a
HDATUM_LIB            = $(HDATUM_DIR)/lib/$(CONFVER)/hdatum.a
LIBCALLPLAN           = $(LIBCALLPLAN_DIR)/lib/$(CONFVER)/libcallplan.a
LIBTPLD               = $(LIBTPLD_DIR)/lib/$(CONFVER)/libtpld.a
LIBUMK                = $(LIBUMK_DIR)/lib/$(CONFVER)/libumk.a
LIBUTIL_LIB           = $(LIBUTIL_DIR)/lib/$(CONFVER)/libutil.a

### ESQL libraries
ESQL_CHECKAPI_LIB     = $(INFORMIX_ESQL_LIB_DIR)/checkapi.o

#--------------------------------------------------------------------------------
# shared libraries
#--------------------------------------------------------------------------------
### directories for library paths

BASAR_LIB_DIR         = $(SSUK_DIR)/sharedlib/$(CONFVER)
TCACCESS_LIB_DIR      = $(TCACCESS_DIR)/lib/
LOG4CPLUS_LIB_DIR     = $(EXT_DIR)/lib/$(CONFVER)
BOOST_LIB_DIR         = $(EXT_DIR)/lib/$(CONFVER)
QT_LIB_DIR            = $(QT_DIR)/lib/

INFORMIX_LIB_DIR      = $(INFORMIX_DIR)/lib
INFORMIX_ESQL_LIB_DIR = $(INFORMIX_LIB_DIR)/esql

VOODOO_LIB_DIR        = $(VOODOO_DIR)/lib/$(CONFVER)

### shared libs
BASAR_SHARED_LIBS     = -L$(BASAR_LIB_DIR) -lbasardebug -lbasarcmnutil -lbasarproperty  -lbasardbsql -lbasardbaspect \
                        -lbasarlogin -lbasarappl

LOG4CPLUS_SHARED_LIBS = -L$(LOG4CPLUS_LIB_DIR) -llog4cplus

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
SOCKET_SHARED_LIBS:sh = echo -lsocket -lnsl

VOODOO_SHARED_LIBS    = -L$(VOODOO_LIB_DIR) -lscsvoodoo

# modeline - do not remove
# vim: ff=unix ts=4 sw=4 noexpandtab syntax=make
