#ifndef GUARD_LIBABBAUW_DOMMOD_PURCHASEBATCHORDER_PURCHASEBATCHORDERPOSDM_H
#define GUARD_LIBABBAUW_DOMMOD_PURCHASEBATCHORDER_PURCHASEBATCHORDERPOSDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ipurchasebatchorderposupdater.h"

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>
#include <libutil/util.h>

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
namespace purchaseBatchOrder
{
    class PurchaseBatchOrderPosDM : public IPurchaseBatchOrderPosUpdater
	{
        public:
            static const libutil::misc::ClassInfo& getClassInfo();

	    public:
		    PurchaseBatchOrderPosDM();
		    ~PurchaseBatchOrderPosDM();

            void                                                injectPurchaseBatchOrderPosAccessor( libutil::infrastructure::accessor::IAccessorPtr );

            // from IPurchaseBatchOrderPosUpdater interface
            void                                                resetSearchYit();
			basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit();

			void                                                findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator );
            bool                                                isEmpty() const;

            void                                                setOrderNo( basar::Int32 );

            void                                                save();
            void                                                erase();

	    private:
            // forbidden
		    PurchaseBatchOrderPosDM( const PurchaseBatchOrderPosDM& );
		    PurchaseBatchOrderPosDM& operator = ( const PurchaseBatchOrderPosDM& );

            libutil::infrastructure::accessor::IAccessorPtr     getAccessor() const;
            libutil::domMod::SearchYIteratorPtr                 getSearchYIterator();
            const log4cplus::Logger&                            getLogger() const;

            basar::db::aspect::AccessorPropertyTable_YIterator  get();
            basar::db::aspect::AccessorPropertyTable_YIterator  get() const;

			void                                                checkErasable() const;

        private:
            const log4cplus::Logger&                            m_Logger;
            libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;
            libutil::infrastructure::accessor::IAccessorPtr     m_Accessor;
	};
} // end namespace purchaseBatchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_PURCHASEBATCHORDER_PURCHASEBATCHORDERPOSDM_H
