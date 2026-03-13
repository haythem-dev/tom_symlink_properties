#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERSPLITDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERSPLITDM_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "itransferordersplitdm.h"

#include "isplittingpolicyptr.h"
#include "componentmanager/icreatesplittedheadfactoryptr.h"
#include "itransferorderptr.h"
#include "domainmodule/orderproposal/iorderproposalptr.h"

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning(disable: 4512 )
#endif
#include <QtCore/QCoreApplication> // translation
#ifdef _MSC_VER
#pragma warning (pop)
#endif

#include <libutil/dbtransaction.h>
//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libutil
{
    namespace misc
    {
	    class ClassInfo;
    }
}

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{
	class TransferOrderSplitDM : public ITransferOrderSplitDM
    {
    Q_DECLARE_TR_FUNCTIONS( TransferOrderSplitDM )

    public:
        static const libutil::misc::ClassInfo&                            getClassInfo();

    public:
        TransferOrderSplitDM();
        ~TransferOrderSplitDM();

        void                                                              injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr );
        void                                                              injectCreateSplittedHeadFactory( componentManager::ICreateSplittedHeadFactoryPtr );
        void                                                              injectSplittingPolicy( splittingPolicy::ISplittingPolicyPtr );
        void                                                              injectOrderProposal( orderProposal::IOrderProposalPtr );
        
        // from ITransferOrderSplitDM interface
        transferOrder::ITransferOrderPtr                                  split( transferOrder::ITransferOrderPtr );
        bool                                                              isSplittable( transferOrder::ITransferOrderPtr );

    private:
        TransferOrderSplitDM ( const TransferOrderSplitDM& );
	    TransferOrderSplitDM operator = ( const TransferOrderSplitDM& );

        const log4cplus::Logger&                                          getLogger() const;
        void                                                              copyPositionToOrder( const basar::db::aspect::AccessorPropertyTable_YIterator, 
                                                                                               transferOrder::ITransferOrderPtr );
        void                                                              setReference( basar::db::aspect::AccessorPropertyTable_YIterator yitHead,
                                                                                        basar::Int32 orderNo );
        void                                                              setDeliveryText( basar::db::aspect::AccessorPropertyTable_YIterator yitPos );
        void                                                              resetPositionToZero( basar::db::aspect::AccessorPropertyTable_YIterator yitPos );
        
        void                                                              processOrderProposals( transferOrder::ITransferOrderPtr, basar::Int32 previousOrderNo );

    private:
        const log4cplus::Logger&                                          m_Logger;

        libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr m_DBTransactionFactory;
        componentManager::ICreateSplittedHeadFactoryPtr                   m_CreateSplittedHeadFactory;
        orderProposal::IOrderProposalPtr                                  m_OrderProposal;
        splittingPolicy::ISplittingPolicyPtr                              m_SplittingPolicy;
    };
	
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERSPLITDM_H
