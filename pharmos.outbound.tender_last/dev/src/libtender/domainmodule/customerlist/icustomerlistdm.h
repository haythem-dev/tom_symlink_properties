#ifndef GUARD_LIBTENDER_DOMMOD_CUSTOMER_ICUSTOMERLISTDM_H
#define GUARD_LIBTENDER_DOMMOD_CUSTOMER_ICUSTOMERLISTDM_H

#include <libbasarcmnutil.h>

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
namespace customerlist
{
class ICustomerListDM
{
public:
	virtual ~ICustomerListDM() {}

	virtual void                                                findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
	virtual void                                                findByTenderNo(basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

	virtual basar::db::aspect::AccessorPropertyTableRef         get()				 = 0;
	virtual const basar::db::aspect::AccessorPropertyTableRef	get() const			 = 0;
	virtual void                                                resetSearchYit()     = 0;
	virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const = 0;
	virtual void                                                setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit ) = 0;
	virtual bool                                                isEmpty() const      = 0;

	virtual void												saveExistingCustomers() = 0;
	virtual void												saveDeleteCustomers() = 0;

	virtual void												setTenderNo( basar::Int32 tenderNo ) = 0;

	virtual basar::db::aspect::AccessorPropertyTable_YIterator	addEmptyCustomer() = 0;
};

} //end namespace customerlist
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_CUSTOMER_ICUSTOMERLISTDM_H
