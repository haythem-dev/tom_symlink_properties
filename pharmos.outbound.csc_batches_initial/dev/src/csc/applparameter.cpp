#include "applparameter.h"

#include "definitions_uadm.h"
#include <libutil/util.h>
#include <libutil/log4cplus.h>
#include <libutil/dbconnection.h>

#include <libbasarlogin_exceptions.h> 

namespace CSC_Batches
{
namespace infrastructure
{
namespace parameter
{
	ApplParameter::ApplParameter()
	: m_Logger( libutil::LoggerPool::getLoggerLibUtilUserAdmin() )
	{
		METHODNAME_DEF( ApplParameter, ApplParameter )
		BLOG_TRACE_METHOD( m_Logger, fun );
	}

	ApplParameter::~ApplParameter()
	{
		METHODNAME_DEF( ApplParameter, ~ApplParameter )
		BLOG_TRACE_METHOD( m_Logger, fun );
	}

    void ApplParameter::injectParameterGateway( libutil::infrastructure::parameter::IParameterGatewayPtr parameterGateway )
    {
        METHODNAME_DEF( ApplParameter, injectParameterGateway )
		BLOG_TRACE_METHOD( m_Logger, fun );

        m_ParameterGateway = parameterGateway;
    }

	bool ApplParameter::getParameterValue( const basar::I18nString& parameterName, basar::I18nString& value ) const
	{
		METHODNAME_DEF( ApplParameter, getParameterValue )
		BLOG_TRACE_METHOD( m_Logger, fun );

        return m_ParameterGateway->getParameterValue( getParameterName( parameterName ), value );
	}
            
    basar::I18nString ApplParameter::getParameterName( const basar::I18nString& parameterName ) const
    {
        METHODNAME_DEF( ApplParameter, getParameterValue )
		BLOG_TRACE_METHOD( m_Logger, fun );

        // replace DBSRV_PARAMETERNAME/DB_PARAMETERNAME - keep anything else
        basar::I18nString parameterNameMapped = parameterName;
        if( libutil::infrastructure::db::dbConnection::DBSRV_PARAMETERNAME == parameterName )
        {
            parameterNameMapped = UADM_HOST;
        } 
        else if( libutil::infrastructure::db::dbConnection::DB_PARAMETERNAME == parameterName )
        {
            parameterNameMapped = UADM_DB;
        }

        return parameterNameMapped;
    }

    basar::I18nString ApplParameter::getUserGroup( const basar::I18nString& branchNo, const basar::I18nString& userName ) const
    {
        METHODNAME_DEF( ApplParameter, getUserGroup )
		BLOG_TRACE_METHOD( m_Logger, fun );

        // get user-specific GROUP parameter
        basar::I18nString group;
        if( ! m_ParameterGateway->getParameterValue( branchNo, userName, UADM_GROUP, group ) )
        {
            // no user-specific GROUP parameter found (for individual testing!) => take branch-specific group
            m_ParameterGateway->getParameterValue( branchNo, UADM_GROUP, group );
        }

        if( group.empty() )
        {
            std::stringstream ss;
		    ss << "There is no general, no branch-global and no user-branchno group name parameter "
			    << "'" << UADM_GROUP << "' in user administration."; 

		    basar::cmnutil::ExceptionInfoStruct sInfo( fun, ss.str().c_str(), __FILE__, __LINE__ );
    					
		    throw basar::login::NoTextFoundException( sInfo );
        }

        return group;
    }

	bool ApplParameter::getParameterValue( const basar::I18nString& parameter1, const basar::I18nString& parameterName, basar::I18nString& value ) const
	{
		METHODNAME_DEF( ApplParameter, getParameterValue )
		BLOG_TRACE_METHOD( m_Logger, fun );

        return m_ParameterGateway->getParameterValue( parameter1, getParameterName(parameterName), value );
	}

	bool ApplParameter::getParameterValue( const basar::Int16 parameter1, const basar::I18nString& parameterName, basar::I18nString& value ) const
	{
		METHODNAME_DEF( ApplParameter, getParameterValue )
		BLOG_TRACE_METHOD( m_Logger, fun );

        return m_ParameterGateway->getParameterValue( parameter1, getParameterName( parameterName ), value );
	}

	bool ApplParameter::getParameterValue( const basar::I18nString& parameter1,    const basar::I18nString& parameter2, 
                                         const basar::I18nString& parameterName, basar::I18nString& value ) const
	{
		METHODNAME_DEF( ApplParameter, getParameterValue )
		BLOG_TRACE_METHOD( m_Logger, fun );

        const basar::I18nString& branchNo = parameter1;
        const basar::I18nString& userName = parameter2;

        return m_ParameterGateway->getParameterValue( getUserGroup( branchNo, userName ), getParameterName( parameterName ), value );
	}

} // end namespace parameter
} // end namespace infrastructure
} // end namespace CSC_Batches
