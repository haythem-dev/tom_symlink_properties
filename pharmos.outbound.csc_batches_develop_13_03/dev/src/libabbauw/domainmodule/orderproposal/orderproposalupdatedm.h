#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALUPDATEDM_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALUPDATEDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "iorderproposal.h"

#include "iorderproposalptr.h"
#include "ipurchasebatchorderposupdaterptr.h"

#include <libutil/classinfo.h>
#include <libutil/util.h>
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
namespace orderProposal
{
    class OrderProposalUpdateDM : public IOrderProposal
	{
        public:
            static const libutil::misc::ClassInfo& getClassInfo();

	    public:
		    OrderProposalUpdateDM();
		    ~OrderProposalUpdateDM();

            void                                                injectOrderProposal( orderProposal::IOrderProposalPtr );
            void                                                injectPurchaseBatchOrderPosUpdater( purchaseBatchOrder::IPurchaseBatchOrderPosUpdaterPtr );
			void                                                injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr );

            // from IOrderProposalUpdater interface
            basar::db::aspect::AccessorPropertyTable_YIterator  get() const;

            void                                                resetSearchYit();
			basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit();

			void                                                findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator );
            bool                                                isEmpty() const;

            void                                                setOrderNo( basar::Int32 );

            void                                                save();
            void                                                erase();

	    private:
            // forbidden
		    OrderProposalUpdateDM( const OrderProposalUpdateDM& );
		    OrderProposalUpdateDM& operator = ( const OrderProposalUpdateDM& );

            const log4cplus::Logger&                            getLogger() const;
            purchaseBatchOrder::IPurchaseBatchOrderPosUpdaterPtr getPurchaseBatchOrderPosUpdater() const;
            orderProposal::IOrderProposalPtr                    getOrderProposal() const;

        private:
            const log4cplus::Logger&                            m_Logger;

            purchaseBatchOrder::IPurchaseBatchOrderPosUpdaterPtr m_PurchaseBatchOrderPosUpdater;
            orderProposal::IOrderProposalPtr                    m_OrderProposal;
			libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr m_TransactionFactory;
	};

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALUPDATEDM_H
