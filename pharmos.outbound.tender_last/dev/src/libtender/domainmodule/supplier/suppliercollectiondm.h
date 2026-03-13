#ifndef GUARD_LIBTENDER_DOMMOD_SUPPLIER_SUPPLIERCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_SUPPLIER_SUPPLIERCOLLECTIONDM_H

#include "isuppliercollectiondm.h"

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace libtender
{
namespace domMod
{
namespace supplier
{

class SupplierCollectionDM : public ISupplierCollectionDM
{
public:
    static const libutil::misc::ClassInfo& getClassInfo();

    SupplierCollectionDM();
    ~SupplierCollectionDM();

    void injectSupplierAccessor( libutil::infrastructure::accessor::IAccessorPtr );

    virtual void                                                findByPattern(basar::db::aspect::AccessorPropertyTable_YIterator );
    virtual void                                                findBySupplierNo( basar::db::aspect::AccessorPropertyTable_YIterator );

    virtual basar::db::aspect::AccessorPropertyTableRef         get() const;
    virtual void                                                resetSearchYit();
    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;
    void                                                        setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit );
    virtual bool                                                isEmpty() const;

private:
    /////////////////////////////////////////////////////////////////////////
    // private member methods
    /////////////////////////////////////////////////////////////////////////
    SupplierCollectionDM( const SupplierCollectionDM & );
    SupplierCollectionDM & operator = ( const SupplierCollectionDM & );

    libutil::domMod::SearchYIteratorPtr             getSearchYIterator() const;
    libutil::infrastructure::accessor::IAccessorPtr getSupplierCollectionAccessor() const;
    const log4cplus::Logger &                       getLogger() const;

    /////////////////////////////////////////////////////////////////////////
    // private member variables
    /////////////////////////////////////////////////////////////////////////
    const log4cplus::Logger                                 m_Logger;

    mutable libutil::domMod::SearchYIteratorPtr		        m_SearchYIterator;
    libutil::infrastructure::accessor::IAccessorPtr         m_Accessor;
};

} // end namespace supplier
} // end namespace domMod
} // end namespace libtender

#endif // end GUARD_LIBTENDER_DOMMOD_SUPPLIER_SUPPLIERCOLLECTIONDM_H
