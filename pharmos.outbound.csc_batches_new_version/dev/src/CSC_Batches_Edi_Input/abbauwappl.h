#ifndef GUARD_ABBAUW_APPL_H
#define GUARD_ABBAUW_APPL_H

#include <libutil/flag.h>

// appl. spec.
#include "libbasarcmnutil_pattern.h"
#include "appsessionptr.h"
#include "infrastructure/mail/mailalertptr.h"

namespace log4cplus 
{
	class Logger;
}

namespace abbaUW
{
    class AppInitializer;	
}

namespace abbaUW
{
	class ABBAUWApplBase
	{
	
	public:
		//static const basar::I18nString s_APPL_NAME;
	
	private:
	    friend class basar::cmnutil::Singleton<ABBAUWApplBase>; //!< singleton as friend for construction!
	
	public:
		virtual ~ABBAUWApplBase();
	
	    void init( int argc, char *argv[] );
		bool run();
	    void shutdown();
	
	private:
		ABBAUWApplBase();
		ABBAUWApplBase( const ABBAUWApplBase& );
		ABBAUWApplBase& operator =( const ABBAUWApplBase& );
	     
	    void initSession( const AppInitializer& );
	    void checkCountryCodeValidity();
	    void checkForBranchnosValidity();
	    void startSession();
	    void endSession();
	    
		log4cplus::Logger&	m_Logger;
		AppSessionPtr		m_AppSession;
	    libutil::misc::Flag	m_InitFlag;
		abbaUW::infrastructure::mail::MailAlertPtr m_MailAlert;
	};

	class ABBAUWAppl : public basar::cmnutil::Singleton<ABBAUWApplBase>
	{
		friend class dummy;	//!< prevents pedantic compiler warning
	
	private:
		ABBAUWAppl();
		ABBAUWAppl( const ABBAUWAppl& );
		ABBAUWAppl& operator =( const ABBAUWAppl& );
	};

}	// namespace abbaUW

#endif	// GUARD_ABBAUW_APPL_H
