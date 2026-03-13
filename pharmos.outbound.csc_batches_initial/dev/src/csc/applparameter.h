#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_PARAMETER_APPLPARAMETER_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_PARAMETER_APPLPARAMETER_H

#include <libbasar_definitions.h>
#include <libutil/parameter.h>

namespace log4cplus 
{
    class Logger;
}

namespace CSC_Batches
{
namespace infrastructure
{
namespace parameter
{
	class ApplParameter : public libutil::infrastructure::parameter::IParameterGateway
	{
	public:
		ApplParameter();
		~ApplParameter();

        void injectParameterGateway( libutil::infrastructure::parameter::IParameterGatewayPtr );

		virtual bool  getParameterValue(  const basar::I18nString& parameterName,  basar::I18nString& value ) const;
		virtual bool  getParameterValue(  const basar::I18nString& parameter1,     const basar::I18nString& parameterName, 
												basar::I18nString& value ) const;
		virtual bool  getParameterValue(  const basar::Int16 parameter1,           const basar::I18nString& parameterName, 
												basar::I18nString& value ) const;
		virtual bool  getParameterValue(  const basar::I18nString& parameter1,     const basar::I18nString& parameter2, 
												const basar::I18nString& parameterName,  basar::I18nString& value ) const;
	private:
		ApplParameter( const ApplParameter& );
		ApplParameter& operator = ( const ApplParameter& );

        basar::I18nString getParameterName( const basar::I18nString& ) const;
        basar::I18nString getUserGroup( const basar::I18nString& branchNo, const basar::I18nString& userName ) const;

	    const log4cplus::Logger&                                    m_Logger;
        libutil::infrastructure::parameter::IParameterGatewayPtr    m_ParameterGateway;
	};

} // end namespace parameter
} // end namespace infrastructure
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_PARAMETER_APPLPARAMETER_H
