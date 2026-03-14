#ifndef GUARD_CSC_BATCHES_USECASE_USERSETTINGS_UC_H
#define GUARD_CSC_BATCHES_USECASE_USERSETTINGS_UC_H

#include <libutil/irunnable.h>

#include <libbasarappl_eventmacros.h>
#include "libbasarappl_systemeventmanager.h"

#include "iusersettingsvcptr.h"
#include "libabbauw/iparameterdmptr.h"

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace appl
    {
        struct EventReturnStruct;
    }
}
    

namespace CSC_Batches
{
namespace useCase 
{

	//-------------------------------------------------------------------------------------------------//
	// class declaration
	//-------------------------------------------------------------------------------------------------//
	class UserSettingsUC : public libutil::useCase::IRunnable
	{
	public:
		UserSettingsUC();
		~UserSettingsUC();

		SYSTEM_EVENT_HANDLER_DECLARATION( SaveSettingsRequested );
		     
		void injectVC( viewConn::IUserSettingsVCPtr );
		void injectParameter( libabbauw::domMod::parameter::IParameterDMPtr );

        // from IRunnable interface
		void run();
		
	private:
		UserSettingsUC( const UserSettingsUC& uc );
		UserSettingsUC& operator= ( const UserSettingsUC& uc );
		
		void registerEventHandlers( const bool forSave );

	private:
		viewConn::IUserSettingsVCPtr                                                    m_UserSettingsVC;
		libabbauw::domMod::parameter::IParameterDMPtr                                 m_Parameter;
	};
	
}// namespace useCase
}// namespace
#endif // GUARD_CSC_BATCHES_USECASE_USERSETTINGS_UC_H
