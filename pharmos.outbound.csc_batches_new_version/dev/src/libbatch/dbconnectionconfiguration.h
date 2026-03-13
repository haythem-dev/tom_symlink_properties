//--------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      declaration of abstract connection base class
 *  \author     Bischof Bjoern
 *  \date       27.01.2009
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBATCH_DBCONNECTION_CONFIGURATION_H
#define GUARD_LIBBATCH_DBCONNECTION_CONFIGURATION_H

//--------------------------------------------------------------------------------------------------//
//  include section
//--------------------------------------------------------------------------------------------------//
#include "dbconnectionconfiguration_definitions.h"
#include "libbatch_definitions.h"
#include <libbasarcmnutil.h> 
#include <sstream>
#include <iomanip>

//--------------------------------------------------------------------------------------------------//
//  class declaration section
//--------------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace dbConnection
{
    struct DBConnectionConfiguration
    {
        DBConnectionConfiguration()
        :m_AppName( STRING_UNSET ), m_CountryCode( STRING_UNSET ), m_DB( STRING_UNSET ), m_DBParameterName( DB_PARAMETERNAME ), 
         m_DBSrv( STRING_UNSET ),  m_DBSrvParameterName( DBSRV_PARAMETERNAME )
        {
        
        }

        bool isConfigured() const
        {
            return  !m_AppName.empty() &&
                    !m_CountryCode.empty() &&
                    !m_DB.empty() &&
                    !m_DBParameterName.empty() &&
                    !m_DB.empty() &&
                    !m_DBSrvParameterName.empty();
        }

        std::ostream& toStream( std::ostream& strm = std::cout ) const
        {
            strm << "DB Connection Configuration: " << "Appl. Name: <" << m_AppName << ">, " << "Country Code: <" << m_CountryCode << ">, ";
            strm << "DB parameter name: <" << m_DBParameterName << ">, ";
			strm << "DB: <" << m_DB << ">, " << "DB server parameter name: <" << m_DBSrvParameterName << ">, ";
			strm << "DB server: <" << m_DBSrv << ">";
            return strm;
        }
// appl. specific modifications (despite it is located in the lib 'libbatch')
        basar::I18nString m_AppName; // todo PK rename
        basar::I18nString m_CountryCode;
        basar::I18nString m_DB;
        basar::I18nString m_DBParameterName;
        basar::I18nString m_DBSrv;
        basar::I18nString m_DBSrvParameterName;
    };
   
} // end dbConnection
} // end namespace GUARD_LIBBATCH_CONNECTION_BASE_H

#include "dbconnectionconfiguration.inl"

#endif // GUARD_LIBBATCH_DBCONNECTION_CONFIGURATION_H
