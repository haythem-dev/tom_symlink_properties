//-----------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      declaration of database login class
 *  \author     Bischof Bjoern
 *  \date       31.12.2010
 *  \version    00.00.00.01 first version
 */ 
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// include section
//-----------------------------------------------------------------------------------------------//
#include "useradminparametergetter.h"
#include <libbasar_definitions.h>
#include <libbasarcmnutil.h>
#include "classinfo_macros.h"
#include "loggerpool.h"
#include <libbasarlogin_manager.h>


//-----------------------------------------------------------------------------------------------//
// class definition section
//-----------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace userAdmin
{

UserAdminParameterGetter::UserAdminParameterGetter()
: m_Logger( LoggerPool::LoggerUserAdmin )
{
    METHODNAME_DEF( UserAdminParameterGetter, UserAdminParameterGetter )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

UserAdminParameterGetter::~UserAdminParameterGetter()
{
    METHODNAME_DEF( UserAdminParameterGetter, ~UserAdminParameterGetter )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

bool UserAdminParameterGetter::getParameterValue( const basar::I18nString& parameterName, basar::I18nString& value ) const
{
    METHODNAME_DEF( UserAdminParameterGetter, getParameterValue )
	BLOG_TRACE_METHOD( m_Logger, fun );

    return basar::login::Manager::getInstance().getParaValue( parameterName, value );
}

bool UserAdminParameterGetter::getParameterValue( const basar::I18nString& parameter1, const basar::I18nString& parameterName, basar::I18nString& value ) const
{
    METHODNAME_DEF( UserAdminParameterGetter, getParameterValue )
	BLOG_TRACE_METHOD( m_Logger, fun );

    return basar::login::Manager::getInstance().getParaValue( parameter1, parameterName, value );
}

bool UserAdminParameterGetter::getParameterValue( const basar::Int16 parameter1, const basar::I18nString& parameterName, basar::I18nString& value ) const
{
    METHODNAME_DEF( UserAdminParameterGetter, getParameterValue )
	BLOG_TRACE_METHOD( m_Logger, fun );

    basar::I18nString para1; para1.format( "%d", parameter1 );
    return basar::login::Manager::getInstance().getParaValue( para1, parameterName, value );
}

bool UserAdminParameterGetter::getParameterValue( const basar::I18nString& parameter1,    const basar::I18nString& parameter2, 
                                                        const basar::I18nString& parameterName, basar::I18nString& value ) const
{
    METHODNAME_DEF( UserAdminParameterGetter, getParameterValue )
	BLOG_TRACE_METHOD( m_Logger, fun );

    return basar::login::Manager::getInstance().getParaValue( parameter1, parameter2, parameterName, value );
}

} // end namespace userAdmin
} // end namespace libBatch
