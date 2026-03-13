#ifndef GUARD_LIBTENDER_DOMMOD_CSCORDERCUSTOMER_CSCORDERCUSTOMERCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_CSCORDERCUSTOMER_CSCORDERCUSTOMERCOLLECTIONDM_H

//-----------------------------------------------------------------------------------------

#include "icscordercustomercollectiondm.h"


#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace cscordercustomer {

//-----------------------------------------------------------------------------------------

class CscOrderCustomerCollectionDM : public ICscOrderCustomerCollectionDM
{
public:
    CscOrderCustomerCollectionDM();
    ~CscOrderCustomerCollectionDM();
    
    void                                                       injectCscOrderAccessor( libutil::infrastructure::accessor::IAccessorPtr );
    virtual void                                               findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator );
    virtual basar::db::aspect::AccessorPropertyTableRef        get() const;
    virtual void                                               resetSearchYit();
    virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit() const;
    virtual bool                                               isEmpty() const;
    
    private:
    CscOrderCustomerCollectionDM( const CscOrderCustomerCollectionDM& r );
    CscOrderCustomerCollectionDM& operator= ( const CscOrderCustomerCollectionDM& r );
    
    libutil::domMod::SearchYIteratorPtr             getSearchYIterator() const;
    libutil::infrastructure::accessor::IAccessorPtr getCscOrderAccessor() const;
    
    
    const log4cplus::Logger & getLogger() const;
    
    const log4cplus::Logger                         m_Logger;
    mutable libutil::domMod::SearchYIteratorPtr     m_SearchYIterator;
    libutil::infrastructure::accessor::IAccessorPtr m_Accessor;
};

//-----------------------------------------------------------------------------------------

} //end namespace cscordercustomer
} //end namespace domMod
} //end namespace libtender

//-----------------------------------------------------------------------------------------

#endif //end GUARD_LIBTENDER_DOMMOD_CSCORDERCUSTOMER_CSCORDERCUSTOMERCOLLECTIONDM_H

//-----------------------------------------------------------------------------------------
