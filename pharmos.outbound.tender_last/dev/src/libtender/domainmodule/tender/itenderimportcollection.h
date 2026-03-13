#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_I_TENDER_IMPORT_COLLECTION_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_I_TENDER_IMPORT_COLLECTION_H

#include "tenderimportstruct.h"
#include "itenderptr.h"

namespace libtender
{
namespace domMod
{
namespace tender
{
class ITenderImportCollection
{
public:
	virtual ~ITenderImportCollection() {}

	virtual basar::Int32 getTenderCount() const = 0;
	virtual basar::Int32 getStartingTenderNo() const = 0;
	virtual void addPosition( const libtender::domMod::tender::TenderImportStruct & values ) = 0;
	virtual void save(bool importing = false) = 0;
	virtual void clear() = 0;
	virtual bool isValidID( const basar::Int32 id ) const = 0;
	virtual basar::Int32 getTenderID( const basar::Int32 id ) const = 0;
	virtual ITenderPtr getTender( const basar::Int32 id ) const = 0;
};

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_TENDER_I_TENDER_IMPORT_COLLECTION_H
