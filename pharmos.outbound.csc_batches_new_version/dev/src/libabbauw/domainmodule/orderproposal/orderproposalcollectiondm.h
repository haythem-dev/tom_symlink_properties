#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCOLLECTIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCOLLECTIONDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "domainmodule/orderproposal/iorderproposalcollection.h"
#include "domainmodule/orderproposal/iorderproposalptr.h"

#include "componentmanager/iorderproposalfactoryptr.h"

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

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
    class OrderProposalCollectionDM : public IOrderProposalCollection
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
		    OrderProposalCollectionDM();
		    ~OrderProposalCollectionDM();

            void                                                injectOrderProposalCollectionAccessor( libutil::infrastructure::accessor::IAccessorPtr );
            void                                                injectOrderProposalFactory( componentManager::IOrderProposalFactoryPtr );

            // from IOrderProposalCollectionFinder interface
            void                                                findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator );

            void                                                resetSearchYit();
			basar::db::aspect::AccessorPropertyTable_YIterator	getSearchYit();

            basar::db::aspect::AccessorPropertyTableRef         get() const;

            // from IOrderProposalCollection interface
            basar::db::aspect::AccessorPropertyTableRef         get();

            basar::db::aspect::AccessorPropertyTable_YIterator  addNewItem();
            void                                                removeItem( basar::db::aspect::AccessorPropertyTable_YIterator );
            void                                                save();
            
	    private:
		    OrderProposalCollectionDM( const OrderProposalCollectionDM& );
		    OrderProposalCollectionDM& operator = (const OrderProposalCollectionDM& );

            libutil::infrastructure::accessor::IAccessorPtr     getAccessor() const;
            libutil::domMod::SearchYIteratorPtr                 getSearchYIterator();
            const log4cplus::Logger&                            getLogger() const;

            componentManager::IOrderProposalFactoryPtr          getOrderProposalFactory() const;

			void												setDeliveryNote(basar::db::aspect::AccessorPropertyTableRef propTab);
            
        private:
            const log4cplus::Logger&                            m_Logger;
            libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;
            libutil::infrastructure::accessor::IAccessorPtr     m_Accessor;

            componentManager::IOrderProposalFactoryPtr          m_OrderProposalFactory;
	};
} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCOLLECTIONDM_H
