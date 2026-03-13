#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALDM_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "iorderproposal.h"

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

namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    class OrderProposalDM : public IOrderProposal
	{
        public:
            static const libutil::misc::ClassInfo& getClassInfo();

	    public:
		    OrderProposalDM();
		    ~OrderProposalDM();

            void                                                injectOrderProposalAccessor( libutil::infrastructure::accessor::IAccessorPtr );

            // from IOrderProposal interface
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
		    OrderProposalDM( const OrderProposalDM& );
		    OrderProposalDM& operator = ( const OrderProposalDM& );

            libutil::infrastructure::accessor::IAccessorPtr     getAccessor() const;
            libutil::domMod::SearchYIteratorPtr                 getSearchYIterator();
            const log4cplus::Logger&                            getLogger() const;

            basar::db::aspect::AccessorPropertyTable_YIterator  get();

            void                                                clear();

        private:
            const log4cplus::Logger&                            m_Logger;
            libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;
            libutil::infrastructure::accessor::IAccessorPtr     m_Accessor;
	};
} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALDM_H
