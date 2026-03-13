#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERCOLLECTIONDM_H

//----------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------//
#include "itenderptr.h"

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
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

//----------------------------------------------------------------------------//
// class declaration section
//----------------------------------------------------------------------------//
namespace libtender
{
    namespace domMod
    {
        namespace tender
        {

            class ITenderCollectionDM
            {
                public:
                    virtual ~ITenderCollectionDM(){};

                    virtual void findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
                    virtual void findByEndDate( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
                    virtual void findSuccessorTender( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
                    virtual void findValidTenders( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
					virtual void findExpiredTenders( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
                    virtual basar::db::aspect::AccessorPropertyTableRef         get() const          = 0;
                    virtual void                                                resetSearchYit()     = 0;
                    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const = 0;
                    virtual ITenderPtr                                          getTender( const basar::db::aspect::AccessorPropertyTable_YIterator ) const = 0;
                    virtual bool                                                isEmpty() const = 0;
            };

        } // end namespace tender
    } // end namespace domMod
} // end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERCOLLECTION_H
