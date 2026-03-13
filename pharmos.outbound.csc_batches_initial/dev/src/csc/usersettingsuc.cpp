//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "usersettingsuc.h"

#include "iusersettingsvc.h"
#include "libabbauw/iparameterdm.h"

#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libutil/usecase.h>
#include <libutil/util.h>

namespace CSC_Batches
{
namespace useCase
{

    BEGIN_HANDLERS_REGISTRATION( UserSettingsUC )
        SYSTEM_EVENT_HANDLER_REGISTRATION( UserSettingsUC, SaveSettingsRequested    )
    END_HANDLERS_REGISTRATION()

	UserSettingsUC::UserSettingsUC() 
	{
        METHODNAME_DEF( UserSettingsUC, UserSettingsUC );
	    BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );
	}

	UserSettingsUC::~UserSettingsUC()
	{
        METHODNAME_DEF( UserSettingsUC, ~UserSettingsUC );
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );
	}

	SYSTEM_EVENT_HANDLER_DEFINITION( UserSettingsUC, SaveSettingsRequested )
	{
		METHODNAME_DEF( UserSettingsUC, SaveSettingsRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
		yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result( HANDLER_ERROR );
        
		try {
			m_UserSettingsVC->matchTo();
			m_Parameter->save();
			m_UserSettingsVC->shutdown();
			
			result = HANDLER_OK;
		} catch( basar::Exception& e ){
            m_UserSettingsVC->error( e.what() );
            result.message = e.what();
        }

		return result;
	}

	void UserSettingsUC::injectVC( viewConn::IUserSettingsVCPtr userSettingsVC )
	{
		m_UserSettingsVC = userSettingsVC;
	}

	void UserSettingsUC::injectParameter( libabbauw::domMod::parameter::IParameterDMPtr parameter )
	{
		m_Parameter = parameter;
	}

	void UserSettingsUC::run()
	{
		METHODNAME_DEF( UserSettingsUC, run )
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );

        registerEventHandlers( true );
        
		m_Parameter->load();
		
		// first check all parameters and reset wrong ones (with user notification)
		if( m_Parameter->setInvalidParametersToDefault() )
			m_Parameter->save();

		m_UserSettingsVC->show();

		m_UserSettingsVC->matchFrom();
        m_UserSettingsVC->waitForEvents();
        
        registerEventHandlers( false );
	}

}// namespace useCase
}//namespace CSC_Batches