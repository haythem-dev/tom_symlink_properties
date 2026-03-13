//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      Domain-module for pubatchorderpos table.
 *              Revised: injects IPurchaseDBParameterProxy so that
 *              PURCHASE_DB / PURCHASE_DBSRV are set on the iterator
 *              before every accessor execute() call (mirrors tender pattern).
 *  \author Hendrik Polzin 
 *  \date 15.08.2013
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_PU_BATCH_ORDER_POS_DM_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_PU_BATCH_ORDER_POS_DM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#include "pubatchorderposinitparams.h"
#include "igetpubatchorderpos.h"
#include "isetpubatchorderpos.h"
#include "ipurchasedbparameterproxyptr.h"
#include <libutil/classinfo.h>
#include <libutil/misc/componentinitialization.hpp>

using basar::db::aspect::AccessorInstanceRef;
//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
namespace OrderProposalTransfer
{
    //-------------------------------------------------------------------------------------------------//
// class declaration
    //-------------------------------------------------------------------------------------------------//
class PuBatchOrderPosDM : public IPuBatchOrderPositionWriter,
                          public IPuBatchOrderPositionFinder,
                               public libutil::misc::ComponentInitialization< PuBatchOrderPosInitParams, PuBatchOrderPosDM >
	{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
        public:
            static const libutil::misc::ClassInfo& getClassInfo();

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
	    public:
    		explicit PuBatchOrderPosDM( const basar::db::aspect::ConnectionRef );
		    ~PuBatchOrderPosDM();

    		// --- IPuBatchOrderPositionFinder / IPuBatchOrderPositionWriter interface ---
			virtual basar::db::aspect::AccessorPropertyTableRef getTable( ) const;
			virtual void savePuBatchOrderPositions( basar::db::aspect::AccessorPropertyTable_YIterator it );
			virtual const basar::db::aspect::AccessorPropertyTableRef findPuBatchOrderPositions();

			virtual void commitTransaction();	
			virtual void beginTransaction();
			virtual void rollbackTransaction();
			virtual bool isInTransaction() const;

    		// --- Proxy injection (must be called before doInit / first use) ---
    		void injectPurchaseDBParameterProxy( abbaUW::infrastructure::db::IPurchaseDBParameterProxyPtr proxy );

        protected:
	            void doInit( const PuBatchOrderPosInitParams& );
	            void doShutdown();

	    private:
		    // non-copyable
		    PuBatchOrderPosDM( const PuBatchOrderPosDM& );
		    PuBatchOrderPosDM& operator=( const PuBatchOrderPosDM& );
		
		    // Helper: stamps PURCHASE_DB / PURCHASE_DBSRV onto the given iterator
		    // from the injected proxy before the accessor executes SQL.
		    void setPurchaseDBProperties( basar::db::aspect::AccessorPropertyTable_YIterator& it ) const;

    //----------------------------------------------------------------------------------------------//
		    // private member variables
    //----------------------------------------------------------------------------------------------//
            basar::db::aspect::ConnectionRef m_Connection;
			basar::db::aspect::AccessorInstanceRef m_Accessor;
			PuBatchOrderPosInitParams              m_InitParams;  
    abbaUW::infrastructure::db::IPurchaseDBParameterProxyPtr   m_PurchaseDBParameterProxy;
	};

//-------------------------------------------------------------------------------------------------//

} // namespace OrderProposalTransfer
} // namespace domMod

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_PU_BATCH_ORDER_POS_DM_H
