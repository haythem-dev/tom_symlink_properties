    //-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author     Ivan Fildishev
 *  \date       17.11.2023
 *  \version    00.00.00.01 first version
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// include section
//-----------------------------------------------------------------------------------------------//
#include "useradminmailconfiguration.h"
#include "classinfo_macros.h"
#include "loggerpool.h"
#include <libbasarlogin_manager.h>
#include "useradminparametergetter.h"
#include <sstream>
#include <iomanip>
#include "useradminfailureexception.h"

//-----------------------------------------------------------------------------------------------//
// class definition section
//-----------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace userAdmin
{

UserAdminMailConfiguration::UserAdminMailConfiguration( libBatch::userAdmin::UserMailConfiguration& userMailConfiguration )
:  m_Logger( LoggerPool::LoggerUserAdmin ), m_UserMailConfiguration(userMailConfiguration)
{
    METHODNAME_DEF( UserAdminMailConfiguration, UserAdminMailConfiguration )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

UserAdminMailConfiguration::~UserAdminMailConfiguration()
{
}

void UserAdminMailConfiguration::configure()
{
    METHODNAME_DEF( UserAdminMailConfiguration, configure )
	BLOG_TRACE_METHOD( m_Logger, fun );

    UserAdminParameterGetter userAdminParameterGetter;
        
	// Read Mail configuration from UsrAdmin
    if( false == userAdminParameterGetter.getParameterValue(m_UserMailConfiguration.m_AppName, m_UserMailConfiguration.m_MailPortName, m_UserMailConfiguration.m_MailPort))
    {
                                                             
        std::stringstream ss; 
        ss << "ERROR: No parameter value found for: Mail Port";
        throw libBatch::userAdmin::UserAdminFailureException( basar::ExceptInfo(fun, ss.str().c_str(), __FILE__, __LINE__) );        
    }

    if (false == userAdminParameterGetter.getParameterValue(m_UserMailConfiguration.m_AppName, m_UserMailConfiguration.m_MailServerName, m_UserMailConfiguration.m_MailServer))
    {

        std::stringstream ss;
            ss << "ERROR: No parameter value found for: Mail Server";
            throw libBatch::userAdmin::UserAdminFailureException(basar::ExceptInfo(fun, ss.str().c_str(), __FILE__, __LINE__));
    }

    if (false == userAdminParameterGetter.getParameterValue(m_UserMailConfiguration.m_AppName, m_UserMailConfiguration.m_MailSenderName, m_UserMailConfiguration.m_MailSender))
    {

        std::stringstream ss;
            ss << "ERROR: No parameter value found for: Mail Sender";
            throw libBatch::userAdmin::UserAdminFailureException(basar::ExceptInfo(fun, ss.str().c_str(), __FILE__, __LINE__));
    }
	
    readMailRecipients();
    logUserAdminMailConfiguration();
}

void UserAdminMailConfiguration::readMailRecipients()
{
    basar::I18nString	key;
    basar::I18nString	value;
    basar::Int16		index = 1;
    bool				found = false;
    UserAdminParameterGetter userAdminParameterGetter;

    //// TODO RBV: ONLY FOR TESTPURPOSE
    //m_UserMailConfiguration.m_MailRecepients.push_back("r.brahm-vogelsanger@externals.phoenixgroup.eu");
    //return;

    do
    {
        key.format("%s_%d", m_UserMailConfiguration.m_MailRecepientName.c_str(), index);
        found = userAdminParameterGetter.getParameterValue(m_UserMailConfiguration.m_AppName, key, value);
        if (true == found)
        {
            m_UserMailConfiguration.m_MailRecepients.push_back(value);
        }
        index++;
    } while (found == true);
}

void UserAdminMailConfiguration::logUserAdminMailConfiguration() const
{
    METHODNAME_DEF( UserAdminMailConfiguration, logUserAdminMailConfiguration)
	BLOG_TRACE_METHOD( m_Logger, fun );

    if( m_Logger.isEnabledFor( log4cplus::INFO_LOG_LEVEL ) ){
        std::stringstream ss;
        ss  << "Successfully received mail configuration from user admin: " << "Mail Port: " << m_UserMailConfiguration.m_MailPort << "Mail Server: " 
            << m_UserMailConfiguration.m_MailServer << "Mail Sender: " << m_UserMailConfiguration.m_MailSender;
        m_Logger.forcedLog( log4cplus::INFO_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
    }
}

libBatch::userAdmin::UserMailConfiguration& UserAdminMailConfiguration::getUserMailConfiguration()
{
    METHODNAME_DEF( UserAdminMailConfiguration, getuserAdminMailConfiguration)
	BLOG_TRACE_METHOD( m_Logger, fun );

    return m_UserMailConfiguration;
}

} // end namespace userAdmin
} // end namespace libBatch
