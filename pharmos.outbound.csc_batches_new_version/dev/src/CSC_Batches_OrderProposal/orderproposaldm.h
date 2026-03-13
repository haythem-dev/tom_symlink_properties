#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_ORDER_PROPOSAL_DM_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_ORDER_PROPOSAL_DM_H

#include "orderproposalinitparams.h"
#include "igetorderproposal.h"
#include "isetorderproposal.h"

#include <libutil/classinfo.h>
#include <libutil/misc/componentinitialization.hpp>

namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class ConnectionRef;
			class AccessorInstanceRef;
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

using basar::db::aspect::AccessorInstanceRef;

namespace domMod {
namespace OrderProposalTransfer {

    class OrderProposalDM :  public IOrderProposalWriter, public IOrderProposalFinder, 
                             public libutil::misc::ComponentInitialization< OrderProposalInitParams, OrderProposalDM >
	{
        public:
            static const libutil::misc::ClassInfo& getClassInfo();

	    public:
		    OrderProposalDM( const basar::db::aspect::ConnectionRef );
		    ~OrderProposalDM();

			virtual basar::db::aspect::AccessorPropertyTableRef getTable() const; 
			virtual basar::db::aspect::AccessorPropertyTableRef findOrderProposals();
			virtual void updateOrderProposals( basar::db::aspect::AccessorPropertyTableRef ref );
			virtual void updateTransferTime( basar::db::aspect::AccessorPropertyTable_YIterator ref );
            virtual void updateOrderProposalCompletedFlag();

			virtual void beginTransaction();
			virtual void commitTransaction();
			virtual void rollbackTransaction();
			virtual bool isInTransaction() const;

        protected:
	            void doInit( const OrderProposalInitParams& );
	            void doShutdown();

	    private:
		    OrderProposalDM(const OrderProposalDM& OrderProposalDM);
		    OrderProposalDM& operator= (const OrderProposalDM& OrderProposalDM);

		    /////////////////////////////////////////////////////////////////////////
		    // private member variables
		    /////////////////////////////////////////////////////////////////////////
            OrderProposalInitParams                 m_InitParams;
            basar::db::aspect::ConnectionRef        m_Connection;
			basar::db::aspect::AccessorInstanceRef  m_Accessor;
			
	};

} // namespace 
} // end namespace 

#endif  
