//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief      Application controller (singleton) for CSC_Batches_OrderProposal.
 *              Revised: holds a PurchaseDBParameterProxy and passes it to
 *              PuBatchOrderPosDM so the remote dblink SQL resolves correctly.
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_APPL_H
#define GUARD_ABBAUW_APPL_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/flag.h>
#include "libbasarcmnutil_pattern.h"
#include "appsessionptr.h"
#include "addtopubatchorderposuc.h"
#include "updateorderproposaluc.h"
#include "pubatchorderposdm.h"
#include "orderproposaldm.h"

// Purchase DB proxy
#include "purchasedbparameterproxyptr.h"

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
public:

	virtual ~ABBAUWApplBase();

    void                                                    init( int argc, char *argv[] );
	bool                                                    run();
    void				                                    shutdown();

private:
    friend class basar::cmnutil::Singleton<ABBAUWApplBase>;

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

    // Proxy that supplies PURCHASE_DB / PURCHASE_DBSRV to PuBatchOrderPosDM
    abbaUW::infrastructure::db::PurchaseDBParameterProxyPtr                     m_PurchaseDBParameterProxy;
};

//-------------------------------------------------------------------------------------------------//
// Singleton derived class
//-------------------------------------------------------------------------------------------------//
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
