#ifndef GUARD_ABBAUW_APPL_H
#define GUARD_ABBAUW_APPL_H

#include <libutil/flag.h>

// appl. spec.
#include "libbasarcmnutil_pattern.h"
#include "appsessionptr.h"
#include "addtopubatchorderposuc.h"
#include "updateorderproposaluc.h"
#include "pubatchorderposdm.h"
#include "orderproposaldm.h"


//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//

namespace log4cplus 
{
	class Logger;
}

namespace abbaUW
{
    class AppInitializer;
}

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace abbaUW
{
//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
class ABBAUWApplBase
{

//-------------------------------------------------------------------------------------------------//
// static section
//-------------------------------------------------------------------------------------------------//
public:

private:
    //CLASSNAME_DECL

    //---------------------------------------------------------//
    // friends
    //---------------------------------------------------------//	
    friend class basar::cmnutil::Singleton<ABBAUWApplBase>; //!< singleton as friend for construction!

public:
	/*! \brief destructor \n no-throw */
	virtual ~ABBAUWApplBase();

    void                                                    init( int argc, char *argv[] );
	bool                                                    run();
    void				                                    shutdown();

private:
	ABBAUWApplBase();
	ABBAUWApplBase( const ABBAUWApplBase& );
	ABBAUWApplBase& operator =( const ABBAUWApplBase& );
     
    void 													initSession( const AppInitializer& );
    void 													checkCountryCodeValidity();
    void 													startSession();
    void 													endSession();
	boost::shared_ptr< domMod::OrderProposalTransfer::OrderProposalDM >			getOrderProposalDM();
	boost::shared_ptr< domMod::OrderProposalTransfer::PuBatchOrderPosDM >		getPuBatchOrderPosDM();
	void			shutdownOrderProposalDM();
	void		shutdownPuBatchOrderPosDM();
    
    //---------------------------------------------------------//
	// private member variables
	//---------------------------------------------------------//
	log4cplus::Logger&								        m_Logger;
	AppSessionPtr											m_AppSession;
    libutil::misc::Flag                                     m_InitFlag;
	boost::shared_ptr< domMod::OrderProposalTransfer::OrderProposalDM >         m_OrderProposalDM;
	boost::shared_ptr< domMod::OrderProposalTransfer::PuBatchOrderPosDM >         m_PuBatchOrderPosDM;

    
    //libirm::version::VersionManagementInfo					m_VersionManagementInfo;
};

//---------------------------------------------------------------------------------------//
/*! \brief  derived class from IRMApplBase.

	This is a final class. Singleton 
 *  \note  class is necessary for dll exporting Singleton\<IRMApplBase\>  */
class ABBAUWAppl : public basar::cmnutil::Singleton<ABBAUWApplBase>
{
	friend class dummy;	//!< prevents pedantic compiler warning

private:
	ABBAUWAppl();
	ABBAUWAppl( const ABBAUWAppl& );
	ABBAUWAppl& operator =( const ABBAUWAppl& );
};

//-------------------------------------------------------------------------------------------------//
}	// namespace 


#endif	// GUARD_ABBAUW_APPL_H
