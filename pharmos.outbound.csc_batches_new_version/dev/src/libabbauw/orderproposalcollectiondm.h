#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCOLLECTIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCOLLECTIONDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "orderproposalcollectioninitparams.h"
#include "iorderproposalcollectionfinder.h"

#include "dmbase.h"	
#include <misc/componentinitialization.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
            class ConnectionRef;
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
    class OrderProposalCollectionDM : public libutil::misc::ComponentInitialization< OrderProposalCollectionInitParams, OrderProposalCollectionDM >,
                                      public IOrderProposalCollectionFinder,
                                      private DMBase
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
		    OrderProposalCollectionDM( const basar::db::aspect::ConnectionRef );
		    ~OrderProposalCollectionDM();

            // from IOrderProposalCollectionFinder interface
            void                                                findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator );

            void                                                resetSearchYit();
			basar::db::aspect::AccessorPropertyTable_YIterator	getSearchYit();

			basar::db::aspect::AccessorPropertyTableRef         get();
            basar::db::aspect::AccessorPropertyTableRef         get() const;

        protected:
	        void                                                doInit( const OrderProposalCollectionInitParams& );
	        void                                                doShutdown();

            AccessorExecuteParameterization						getFindByPatternExecuteParameters() const;
            basar::VarString									getSearchYIteratorAttributeNames()  const;

	    private:
		    OrderProposalCollectionDM( const OrderProposalCollectionDM& );
		    OrderProposalCollectionDM& operator = (const OrderProposalCollectionDM& );
            
            OrderProposalCollectionInitParams                    m_InitParams;
	};
} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCOLLECTIONDM_H
