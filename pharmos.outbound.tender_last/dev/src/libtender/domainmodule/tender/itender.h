#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDER_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDER_H

#include "itenderpositioncollectionptr.h"
#include "domainmodule/customerlist/icustomerlistdmptr.h"
#include <domainmodule/tender/tenderstateenum.h>

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
class ITender
{
public:
	virtual ~ITender() {}

	virtual basar::db::aspect::AccessorPropertyTable_YIterator getHeader() = 0;
	virtual ITenderPositionCollectionPtr getPositionCollection() = 0;
	virtual libtender::domMod::customerlist::ICustomerListDMPtr getCustomerList() = 0;

	virtual basar::db::aspect::AccessorPropertyTable_YIterator addEmptyPosition() = 0;
	virtual void deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit ) = 0;

	virtual void findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	virtual void save(bool importing = false) = 0;
	virtual void deleteTender() = 0;
	virtual void setTenderState( const TenderStateEnum state ) = 0;
	virtual TenderStateEnum getTenderState() const = 0;
};

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_ITENDER_H
