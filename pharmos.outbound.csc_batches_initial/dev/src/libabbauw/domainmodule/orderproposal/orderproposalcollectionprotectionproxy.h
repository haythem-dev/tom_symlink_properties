#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCOLLECTIONPROTECTIONPROXY_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCOLLECTIONPROTECTIONPROXY_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "domainmodule/orderproposal/iorderproposalcollectionprotectionproxy.h"
#include "domainmodule/orderproposal/iorderproposalcollectionptr.h"

#include <libutil/classinfo.h>
#include <map>

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

namespace log4cplus
{
    class Logger;
}

namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    class OrderProposalCollectionProtectionProxy : public IOrderProposalCollectionProtectionProxy
	{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
        public:
            static const libutil::misc::ClassInfo&              getClassInfo();

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
	    public:
		    OrderProposalCollectionProtectionProxy();
		    ~OrderProposalCollectionProtectionProxy();

            void                                                injectOrderProposalCollection( IOrderProposalCollectionPtr );

            // from IOrderProposalCollectionProtectionProxy
            void                                                passLockGuard( basar::Int32 orderNo, libLockManager::ILockGuardPtr );

            // from IOrderProposalCollection interface
            void                                                findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator );

            void                                                resetSearchYit();
			basar::db::aspect::AccessorPropertyTable_YIterator	getSearchYit();

            basar::db::aspect::AccessorPropertyTableRef         get() const;

            basar::db::aspect::AccessorPropertyTableRef         get();

            basar::db::aspect::AccessorPropertyTable_YIterator  addNewItem();
            void                                                removeItem( basar::db::aspect::AccessorPropertyTable_YIterator );
            void                                                save();
            
	    private:
		    OrderProposalCollectionProtectionProxy( const OrderProposalCollectionProtectionProxy& );
		    OrderProposalCollectionProtectionProxy& operator = (const OrderProposalCollectionProtectionProxy& );

            const log4cplus::Logger&                            getLogger() const;
            IOrderProposalCollectionPtr                         getOrderProposalCollection() const;

            bool                                                hasOrder( basar::Int32 orderNo );
            void                                                releaseLock( basar::Int32 orderNo );

        private:
            const log4cplus::Logger&                            m_Logger;
            IOrderProposalCollectionPtr                         m_OrderProposalCollection;

            typedef std::map< basar::Int32, libLockManager::ILockGuardPtr >  LockGuardCollection;
            LockGuardCollection                                 m_LockGuardCollection;
	};
} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCOLLECTIONPROTECTIONPROXY_H
