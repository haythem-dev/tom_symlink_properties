#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROTOCOL_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROTOCOL_H

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
class ITenderProtocol
{
public:
	virtual ~ITenderProtocol() {}

	//virtual void findByTenderBG( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

    virtual basar::db::aspect::AccessorPropertyTableRef			get() const          = 0;
    virtual void                                                resetSearchYit()     = 0;
    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const = 0;
    virtual bool                                                isEmpty() const		 = 0;

	virtual void												saveProtocol() = 0;
};

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROTOCOL_H
