#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROGRESS_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROGRESS_H

#include "itenderprogressptr.h"

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

namespace libtender
{
namespace domMod
{
namespace tender
{
class ITenderProgress
{
public:
	virtual ~ITenderProgress() {}
    virtual void findArticleQuantityByPattern( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	virtual void findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	virtual void findByTender( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
    virtual void findTenderProgressForOMGCalcultion( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
    virtual void save( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
    virtual basar::db::aspect::AccessorPropertyTableRef         get() const          = 0;
    virtual void                                                resetSearchYit()     = 0;
    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const = 0;
    //virtual ITenderProgressPtr                                  getTender( const basar::db::aspect::AccessorPropertyTable_YIterator ) const = 0;
    virtual bool                                                isEmpty() const = 0;
};

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROGRESS_H
