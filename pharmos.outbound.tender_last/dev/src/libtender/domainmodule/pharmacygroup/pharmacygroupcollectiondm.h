#ifndef GUARD_LIBTENDER_DOMMOD_PHARMACYGROUPCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_PHARMACYGROUPCOLLECTIONDM_H

#include "ipharmacygroupcollectiondm.h"

#include <componentmanager/itenderconnectedfactoryptr.h>

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace libtender
{
namespace domMod
{
namespace pharmacygroup
{

class PharmacyGroupCollectionDM : public IPharmacyGroupCollectionDM
{
public:
    static const libutil::misc::ClassInfo& getClassInfo();
    
    PharmacyGroupCollectionDM();
    ~PharmacyGroupCollectionDM();
    
    void injectPharmacyGroupAccessor( libutil::infrastructure::accessor::IAccessorPtr );
    
    virtual void                                                findByPattern(basar::db::aspect::AccessorPropertyTable_YIterator );
    virtual void                                                findPharmacyGroupCustomerAssignmentByPattern(basar::db::aspect::AccessorPropertyTable_YIterator );
    virtual void                                                findByPharmacyGroupId( basar::db::aspect::AccessorPropertyTable_YIterator );
    virtual void                                                findByPseudoCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator );

    virtual basar::db::aspect::AccessorPropertyTableRef         get() const;
    virtual void                                                resetSearchYit();
    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;
    void                                                        setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit );
    virtual bool                                                isEmpty() const;

private:
    /////////////////////////////////////////////////////////////////////////
    // private member methods
    /////////////////////////////////////////////////////////////////////////
    PharmacyGroupCollectionDM( const PharmacyGroupCollectionDM & );
    PharmacyGroupCollectionDM & operator = ( const PharmacyGroupCollectionDM & );
    
    libutil::domMod::SearchYIteratorPtr             getSearchYIterator() const;
    libutil::infrastructure::accessor::IAccessorPtr getPharmacyGroupCollectionAccessor() const;
    const log4cplus::Logger &                       getLogger() const;
    
    /////////////////////////////////////////////////////////////////////////
    // private member variables
    /////////////////////////////////////////////////////////////////////////
    const log4cplus::Logger                                 m_Logger;
    
    mutable libutil::domMod::SearchYIteratorPtr             m_SearchYIterator;
    libutil::infrastructure::accessor::IAccessorPtr         m_Accessor;
};

} // end namespace pharmacychain
} // end namespace domMod
} // end namespace libtender

#endif // end GUARD_LIBTENDER_DOMMOD_PHARMACYGROUPCOLLECTIONDM_H
