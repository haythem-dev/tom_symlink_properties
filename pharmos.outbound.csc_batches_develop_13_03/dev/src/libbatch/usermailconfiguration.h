//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Ivan Fildishev
 *  \date       17.11.2023
 *  \version    00.00.00.01 first version
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBATCH_USER_MAIL_CONFIGURATION_H
#define GUARD_LIBBATCH_USER_MAIL_CONFIGURATION_H

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
    struct UserMailConfiguration
    {
        UserMailConfiguration() :
        m_AppName(""),
        m_MailSenderName("MAIL_SENDER"),
        m_MailServerName("MAIL_SERVER"),
        m_MailPortName("MAIL_PORT"),
        m_MailRecepientName("MAIL_RECIPIENT"),
		m_MailSender( "" ), 
        m_MailServer( "" ),
		m_MailPort( "")
        {
		}

        UserMailConfiguration(const UserMailConfiguration& m) :
            m_AppName(m.m_AppName),
            m_MailSenderName(m.m_MailSenderName),
            m_MailServerName(m.m_MailServerName),
            m_MailPortName(m.m_MailPortName),
            m_MailRecepientName(m.m_MailRecepientName),
            m_MailSender(m.m_MailSender),
            m_MailServer(m.m_MailServer),
            m_MailPort(m.m_MailPort)
        {
        }

        std::ostream& toStream( std::ostream& strm = std::cout ) const
        {
            strm << "User Mail Configuration: Mail Sender: <" << m_MailSender << ">, ";
			strm << "Mail Server: <" << m_MailServer << ">, " << "Mail Port: <" << m_MailPort << ">";                
            return strm;
        }

        basar::I18nString   m_AppName;

        basar::I18nString   m_MailSenderName;
        basar::I18nString   m_MailServerName;
        basar::I18nString   m_MailPortName;
        basar::I18nString   m_MailRecepientName;

        basar::I18nString   m_MailSender;
        basar::I18nString   m_MailServer;
        basar::I18nString   m_MailPort ;

        basar::cmnutil::CollBString m_MailRecepients;
    };

} // end namespace userAdmin
} // end namespace libBatch

#endif // end GUARD_LIBBATCH_USER_MAIL_CONFIGURATION_H
