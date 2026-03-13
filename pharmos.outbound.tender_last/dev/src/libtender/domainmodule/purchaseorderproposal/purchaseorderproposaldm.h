#ifndef GUARD_LIBTENDER_DOMMOD_PURCHASEORDERPROPOSAL_PURCHASEORDERPROPOSALDM_H
#define GUARD_LIBTENDER_DOMMOD_PURCHASEORDERPROPOSAL_PURCHASEORDERPROPOSALDM_H

//-------------------------------------------------------------------------------------------------//

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/util.h>

#include "ipurchaseorderproposal.h"
#include <infrastructure/purchasedbconn/ipurchasedbparameterproxyptr.h>

//-------------------------------------------------------------------------------------------------//

namespace libtender {
namespace domMod {
namespace purchaseOrderProposal {

//-------------------------------------------------------------------------------------------------//

class PurchaseOrderProposalDM : public IPurchaseOrderProposal
{
public:
	PurchaseOrderProposalDM();
	~PurchaseOrderProposalDM();

	void injectPurchaseOrderProposalAccessor( libutil::infrastructure::accessor::IAccessorPtr );
	void injectPurchaseDBParameterProxy( infrastructure::db::IPurchaseDBParameterProxyPtr );

	virtual void setBranchNo(const basar::Int16);
	virtual void setArticleNo(const basar::Int32);
	virtual void setOrderProposalQuantity(const basar::Int32);
	virtual void setOrderNoBatch(const basar::Int32);
	virtual void setOrderTypeBatch(const basar::Int16);
	virtual void setExpectedGoodsinDate(const basar::Date);
	virtual void setSupplierNo(const basar::Int32);

	virtual basar::Int32 getOrderProposalNo() const;
	virtual basar::Int32 getPurchaseOrderNo() const;

	virtual void markForDelete();

	virtual void findByKey(const basar::db::aspect::AccessorPropertyTable_YIterator);
	virtual void resetSearchYit();
	virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit() const;

	virtual basar::db::aspect::AccessorPropertyTable_YIterator get();
	virtual const basar::db::aspect::AccessorPropertyTable_YIterator get() const;

	virtual basar::Int32 getPrimaryKey() const;

	virtual void save();
	virtual bool isEmpty() const;
	virtual void clear();

private:
	PurchaseOrderProposalDM(const PurchaseOrderProposalDM&);
	PurchaseOrderProposalDM& operator = (const PurchaseOrderProposalDM&);

	libutil::infrastructure::accessor::IAccessorPtr getAccessor() const;
	libutil::domMod::SearchYIteratorPtr				getSearchYIterator() const;
	const log4cplus::Logger&						getLogger() const;

	const log4cplus::Logger 						m_Logger;
	mutable libutil::domMod::SearchYIteratorPtr		m_SearchYIterator;
	libutil::infrastructure::accessor::IAccessorPtr	m_Accessor;
	infrastructure::db::IPurchaseDBParameterProxyPtr	m_PurchaseDBParameterProxy;
};

//-------------------------------------------------------------------------------------------------//

} // namespace purchaseOrderProposal
} // namespace domMod
} // namespace libtender

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_LIBTENDER_DOMMOD_PURCHASEORDERPROPOSAL_PURCHASEORDERPROPOSALDM_H

//-------------------------------------------------------------------------------------------------//
