#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSITIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSITIONDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "itransferorderposition.h"

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>
#include <libutil/util.h>

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{
    class TransferOrderPositionDM : public ITransferOrderPosition
    {
    public:
        static const libutil::misc::ClassInfo& getClassInfo();

    public:
        TransferOrderPositionDM();
        ~TransferOrderPositionDM();

        //void                                                        injectTransferOrderPositionAccessor( libutil::infrastructure::accessor::IAccessorPtr );
        void                                                        set( basar::db::aspect::AccessorPropertyTable_YIterator );

        // from ITransferOrderPosition interface
        basar::db::aspect::AccessorPropertyTable_YIterator          get()     const;

        /*
        void                                                        resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator          getSearchYit();
        void                                                        findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator );
        */

        bool                                                        isEmpty() const;
        bool                                                        hasOrderProposalSet()    const;
        bool                                                        isOrderProposalAllowed() const;

    private:
        // forbidden
        TransferOrderPositionDM( const TransferOrderPositionDM& );
        TransferOrderPositionDM& operator = ( const TransferOrderPositionDM& );

        //libutil::infrastructure::accessor::IAccessorPtr             getAccessor() const;
        //libutil::domMod::SearchYIteratorPtr                         getSearchYIterator();
        const log4cplus::Logger&                                    getLogger() const;

        //bool                                                        isChanged() const;

    private:
        const log4cplus::Logger&                                    m_Logger;
        //libutil::domMod::SearchYIteratorPtr                         m_SearchYIterator;
        //libutil::infrastructure::accessor::IAccessorPtr             m_Accessor;
        basar::db::aspect::AccessorPropertyTable_YIterator          m_YitPos;
    };

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSITIONDM_H
