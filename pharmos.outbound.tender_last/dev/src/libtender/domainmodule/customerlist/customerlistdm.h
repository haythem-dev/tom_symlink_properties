#ifndef GUARD_LIBTENDER_DOMMOD_CUSTOMER_CUSTOMERLISTDM_H
#define GUARD_LIBTENDER_DOMMOD_CUSTOMER_CUSTOMERLISTDM_H

#include "icustomerlistdm.h"

#include <componentmanager/itenderconnectedfactoryptr.h>

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace libtender
{
namespace domMod
{
namespace customerlist
{

class CustomerListDM : public ICustomerListDM
{
public:
	static const libutil::misc::ClassInfo& getClassInfo();

	CustomerListDM();
	~CustomerListDM();

	void injectCustomerAccessor( libutil::infrastructure::accessor::IAccessorPtr );

	virtual void                                                findByPattern(basar::db::aspect::AccessorPropertyTable_YIterator );
	virtual void                                                findByTenderNo(basar::db::aspect::AccessorPropertyTable_YIterator );

	virtual basar::db::aspect::AccessorPropertyTableRef			get();
	virtual const basar::db::aspect::AccessorPropertyTableRef	get() const;
	virtual void                                                resetSearchYit();
	virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;
	void                                                        setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit );
	virtual bool                                                isEmpty() const;

	virtual void												saveExistingCustomers();
	virtual void												saveDeleteCustomers();

	virtual void												setTenderNo( basar::Int32 tenderNo );

	virtual basar::db::aspect::AccessorPropertyTable_YIterator	addEmptyCustomer();

private:
	/////////////////////////////////////////////////////////////////////////
    // private member methods
    /////////////////////////////////////////////////////////////////////////
	CustomerListDM( const CustomerListDM & );
	CustomerListDM & operator = ( const CustomerListDM & );

	libutil::domMod::SearchYIteratorPtr		        getSearchYIterator() const;
	libutil::infrastructure::accessor::IAccessorPtr getCustomerCollectionAccessor() const;
	const log4cplus::Logger &				        getLogger() const;

	bool isSavable() const;
	bool isSavableForDelete() const;

    /////////////////////////////////////////////////////////////////////////
    // private member variables
    /////////////////////////////////////////////////////////////////////////
	const log4cplus::Logger 				                m_Logger;
	
	mutable libutil::domMod::SearchYIteratorPtr		        m_SearchYIterator;
	libutil::infrastructure::accessor::IAccessorPtr         m_Accessor;
};

} // end namespace customerlist
} // end namespace domMod
} // end namespace libtender

#endif // end GUARD_LIBTENDER_DOMMOD_CUSTOMER_CUSTOMERLISTDM_H
