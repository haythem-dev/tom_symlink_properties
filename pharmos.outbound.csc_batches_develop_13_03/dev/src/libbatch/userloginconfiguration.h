//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Bischof Bjoern
 *  \date       27.12.2010
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBATCH_USER_ADMIN_LOGIN_CONFIGURATION_H
#define GUARD_LIBBATCH_USER_ADMIN_LOGIN_CONFIGURATION_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil.h>
#include <sstream>
#include <iomanip>

//--------------------------------------------------------------------------------------------------//
// namespace setcion
//--------------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace userAdmin
{
    struct UserLoginConfiguration
    {
        UserLoginConfiguration() :
        m_User( "" ),
		m_Passwd( "" ), 
        m_AppName( "" ),
		m_AreaID( -1 )
        {
		}

        std::ostream& toStream( std::ostream& strm = std::cout ) const
        {
            strm << "User Login Configuration: User: <" << m_User << ">, ";
			strm << "Application name: <" << m_AppName << ">, " << "Ared ID: <" << m_AreaID << ">";                
            return strm;
        }

        basar::I18nString m_User;
        basar::I18nString m_Passwd;
        basar::I18nString m_AppName;
        basar::Int16      m_AreaID;
    };

} // end namespace userAdmin
} // end namespace libBatch

#include "userloginconfiguration.inl"

#endif // end GUARD_LIBBATCH_USER_ADMIN_LOGIN_CONFIGURATION_H
