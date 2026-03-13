#ifndef GUARD_LIBTENDER_DOMMOD_CUSTOMER_CUSTOMERCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_CUSTOMER_CUSTOMERCOLLECTIONDM_H

#include "icustomercollectiondm.h"

#include <componentmanager/itenderconnectedfactoryptr.h>

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace libtender
{
namespace domMod
{
namespace customer
{

class CustomerCollectionDM : public ICustomerCollectionDM
{
public:
    static const libutil::misc::ClassInfo& getClassInfo();
    
    CustomerCollectionDM();
    ~CustomerCollectionDM();
    
    void injectCustomerAccessor( libutil::infrastructure::accessor::IAccessorPtr );
    
    virtual void                                                findByPattern(basar::db::aspect::AccessorPropertyTable_YIterator );
    virtual void                                                findByCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator );
    virtual void                                                findCustomerByPharmacyGroupId( basar::db::aspect::AccessorPropertyTable_YIterator );
    
    
    virtual basar::db::aspect::AccessorPropertyTableRef         get() const;
    virtual void                                                resetSearchYit();
    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;
    void                                                        setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit );
    virtual bool                                                isEmpty() const;

private:
    /////////////////////////////////////////////////////////////////////////
    // private member methods
    /////////////////////////////////////////////////////////////////////////
    CustomerCollectionDM( const CustomerCollectionDM & );
    CustomerCollectionDM & operator = ( const CustomerCollectionDM & );
    
    libutil::domMod::SearchYIteratorPtr		        getSearchYIterator() const;
    libutil::infrastructure::accessor::IAccessorPtr getCustomerCollectionAccessor() const;
    const log4cplus::Logger &				        getLogger() const;
    
    /////////////////////////////////////////////////////////////////////////
    // private member variables
    /////////////////////////////////////////////////////////////////////////
    const log4cplus::Logger 				                m_Logger;
    
    mutable libutil::domMod::SearchYIteratorPtr		        m_SearchYIterator;
    libutil::infrastructure::accessor::IAccessorPtr         m_Accessor;
};

} // end namespace customer
} // end namespace domMod
} // end namespace libtender

#endif // end GUARD_LIBTENDER_DOMMOD_CUSTOMER_CUSTOMERCOLLECTIONDM_H
