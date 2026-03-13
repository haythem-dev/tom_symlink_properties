#ifndef GUARD_LIBTENDER_DOMMOD_PURCHASEORDERPROPOSAL_IPURCHASEORDERPROPOSAL_H
#define GUARD_LIBTENDER_DOMMOD_PURCHASEORDERPROPOSAL_IPURCHASEORDERPROPOSAL_H

//-------------------------------------------------------------------------------------------------//

#include <libbasar_definitions.h>

//-------------------------------------------------------------------------------------------------//

namespace basar {
namespace db {
namespace aspect {
	class AccessorPropertyTable_YIterator;
}
}
}

//-------------------------------------------------------------------------------------------------//

namespace libtender	{
namespace domMod {
namespace purchaseOrderProposal	{

//-------------------------------------------------------------------------------------------------//

class IPurchaseOrderProposal
{
public:
	virtual ~IPurchaseOrderProposal() {}

	virtual void setBranchNo(const basar::Int16) = 0;
	virtual void setArticleNo(const basar::Int32) = 0;
	virtual void setOrderProposalQuantity(const basar::Int32) = 0;
	virtual void setOrderNoBatch(const basar::Int32) = 0;
	virtual void setOrderTypeBatch(const basar::Int16) = 0;
	virtual void setExpectedGoodsinDate(const basar::Date) = 0;
	virtual void setSupplierNo(const basar::Int32) = 0;

	virtual basar::Int32 getOrderProposalNo() const = 0;
	virtual basar::Int32 getPurchaseOrderNo() const = 0;

	virtual void markForDelete() = 0;

	virtual void findByKey ( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	virtual void resetSearchYit() = 0;
	virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit() const = 0;

	virtual basar::db::aspect::AccessorPropertyTable_YIterator get() = 0;
	virtual const basar::db::aspect::AccessorPropertyTable_YIterator get() const = 0; 

	virtual basar::Int32 getPrimaryKey() const = 0;

	virtual void save() = 0;
	virtual bool isEmpty() const = 0;
	virtual void clear() = 0;
};

//-------------------------------------------------------------------------------------------------//

} // namespace purchaseOrderProposal
} // namespace domMod
} // namespace libtender

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_LIBTENDER_DOMMOD_PURCHASEORDERPROPOSAL_IPURCHASEORDERPROPOSAL_H

//-------------------------------------------------------------------------------------------------//
