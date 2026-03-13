#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERHEADER_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERHEADER_H

#include <libbasar_definitions.h>
#include <domainmodule/tender/tenderstateenum.h>

namespace basar
{
namespace db
{
namespace aspect
{
	class AccessorPropertyTable_YIterator;
}
}
}

namespace libtender
{
namespace domMod
{
namespace tender
{

class ITenderHeader
{
public:
	virtual ~ITenderHeader() {}

	virtual void findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

	virtual basar::db::aspect::AccessorPropertyTable_YIterator get() = 0;
	virtual const basar::db::aspect::AccessorPropertyTable_YIterator get() const = 0;

	virtual void save() = 0;
	virtual bool isEmpty() const = 0;
	virtual void validate() const = 0;
	virtual bool isValid() const = 0;
	virtual TenderStateEnum getState() const = 0;
	virtual void setState( const TenderStateEnum state ) = 0;

	virtual basar::Int32 getPrimaryKey() const = 0;
};

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERHEADER_H
