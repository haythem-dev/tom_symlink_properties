#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPOSITIONCOLLECTION_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPOSITIONCOLLECTION_H

#include <libbasar_definitions.h>
#include <libutil/accessor.h>
#include <string>
#include <domainmodule/tender/tenderstateenum.h>


namespace basar
{
namespace db
{
namespace aspect
{
	class AccessorInstanceRef;
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
class ITenderPositionCollection
{
public:
	virtual ~ITenderPositionCollection() {}

	virtual void findByKeyFR( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	virtual void findByKeyRS( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	virtual void findByKeyBG( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	virtual void findByKeyCH( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

	virtual basar::db::aspect::AccessorPropertyTable_YIterator addEmptyPosition() = 0;
	virtual void deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit ) = 0;

	virtual basar::db::aspect::AccessorPropertyTableRef get() = 0;
	virtual const basar::db::aspect::AccessorPropertyTableRef get() const = 0;
	virtual basar::db::aspect::AccessorPropertyTable_YIterator getByArticleCode( const basar::VarString articleCode ) = 0;

	virtual void setForeignKey( const basar::Int32 ) = 0;
	virtual void setState(const TenderStateEnum tenderState) = 0;
	virtual void setNameOfUser( const std::string ) = 0;
	virtual void save() = 0;
	virtual bool isEmpty() const = 0;
	virtual void validate() const = 0;
	virtual bool isValid() const = 0;

};

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPOSITIONCOLLECTION_H
