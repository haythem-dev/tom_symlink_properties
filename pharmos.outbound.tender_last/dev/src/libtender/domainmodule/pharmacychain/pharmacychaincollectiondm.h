#ifndef GUARD_LIBTENDER_DOMMOD_PARTNER_PHARMACYCHAINCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_PARTNER_PHARMACYCHAINCOLLECTIONDM_H

#include "ipharmacychaincollectiondm.h"

#include <componentmanager/itenderconnectedfactoryptr.h>

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace libtender
{
namespace domMod
{
namespace pharmacychain
{

class PharmacyChainCollectionDM : public IPharmacyChainCollectionDM
{
public:
	static const libutil::misc::ClassInfo& getClassInfo();

	PharmacyChainCollectionDM();
	~PharmacyChainCollectionDM();

	void injectPharmacyChainAccessor( libutil::infrastructure::accessor::IAccessorPtr );

	virtual void                                                findByPattern(basar::db::aspect::AccessorPropertyTable_YIterator );
	virtual void                                                findByPharmacyChainNo( basar::db::aspect::AccessorPropertyTable_YIterator );
	virtual void												findCustomerGroupIDByPharmacyChain( basar::db::aspect::AccessorPropertyTable_YIterator );

	virtual basar::db::aspect::AccessorPropertyTableRef         get() const;
	virtual void                                                resetSearchYit();
	virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;
	void                                                        setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit );
	virtual bool                                                isEmpty() const;

private:
	/////////////////////////////////////////////////////////////////////////
    // private member methods
    /////////////////////////////////////////////////////////////////////////
	PharmacyChainCollectionDM( const PharmacyChainCollectionDM & );
	PharmacyChainCollectionDM & operator = ( const PharmacyChainCollectionDM & );

	libutil::domMod::SearchYIteratorPtr		        getSearchYIterator() const;
	libutil::infrastructure::accessor::IAccessorPtr getPharmacyChainCollectionAccessor() const;
	const log4cplus::Logger &				        getLogger() const;

    /////////////////////////////////////////////////////////////////////////
    // private member variables
    /////////////////////////////////////////////////////////////////////////
	const log4cplus::Logger 				                m_Logger;
	
	mutable libutil::domMod::SearchYIteratorPtr		        m_SearchYIterator;
	libutil::infrastructure::accessor::IAccessorPtr         m_Accessor;
};

} // end namespace pharmacychain
} // end namespace domMod
} // end namespace libtender

#endif // end GUARD_LIBTENDER_DOMMOD_PARTNER_PHARMACYCHAINCOLLECTIONDM_H
