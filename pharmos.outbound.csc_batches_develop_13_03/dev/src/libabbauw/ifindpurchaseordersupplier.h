#ifndef GUARD_LIBABBAUW_DOMMOD_PURCHASEORDERSUPPLIERDM_IFINDPURCHASEORDERSUPPLIER_H
#define GUARD_LIBABBAUW_DOMMOD_PURCHASEORDERSUPPLIERDM_IFINDPURCHASEORDERSUPPLIER_H

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTableRef;
			class AccessorPropertyTable_YIterator;
        }
    }
}

//----------------------------------------------------------------------------//
// class declaration section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace purchaseordersupplierdm
{
    class IFindPurchaseOrderSupplier
    {
        public:
            virtual ~IFindPurchaseOrderSupplier() {}

            virtual bool                                                isEmpty() const                                                        = 0;
			virtual basar::db::aspect::AccessorPropertyTable_YIterator  get()                                                                  = 0;
            virtual basar::db::aspect::AccessorPropertyTable_YIterator  get() const                                                            = 0;
			virtual void			                                    findPurchaseOrderSupplierById( basar::Int32 purchaseOrderSupplierNo )  = 0;
            virtual void                                                clear()                                                                = 0;

			//virtual void                                                resetSearchYit()                                                       = 0;
            //virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit()                                                         = 0;
    };
} 
} 
}

#endif // GUARD_LIBABBAUW_DOMMOD_PURCHASEORDERSUPPLIERDM_IFINDPURCHASEORDERSUPPLIER_H
