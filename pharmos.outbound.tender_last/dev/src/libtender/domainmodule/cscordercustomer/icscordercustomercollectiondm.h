#ifndef GUARD_LIBTENDER_DOMMOD_CSCORDERCUSTOMER_ICSCORDERCUSTOMERCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_CSCORDERCUSTOMER_ICSCORDERCUSTOMERCOLLECTIONDM_H

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
        namespace cscordercustomer
        {

            class ICscOrderCustomerCollectionDM
            {
                public:
                    virtual ~ICscOrderCustomerCollectionDM(){};
                    
                    virtual void                                                findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
                    
                    virtual basar::db::aspect::AccessorPropertyTableRef         get() const          = 0;
                    virtual void                                                resetSearchYit()     = 0;
                    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const = 0;
                    virtual bool                                                isEmpty() const = 0;
            };

        } // end namespace scheduledDelivery
    }     // end namespace domMod
}         // end namespace libtender

#endif //GUARD_LIBTENDER_DOMMOD_CSCORDERCUSTOMER_ICSCORDERCUSTOMERCOLLECTIONDM_H
