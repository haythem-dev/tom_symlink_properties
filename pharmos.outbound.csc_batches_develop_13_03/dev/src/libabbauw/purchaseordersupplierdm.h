#ifndef GUARD_LIBABBAUW_DOMMOD_PURCHASEORDERSUPPLIERDM_PURCHASEORDERSUPPLIERDM_H
#define GUARD_LIBABBAUW_DOMMOD_PURCHASEORDERSUPPLIERDM_PURCHASEORDERSUPPLIERDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasardbaspect_connectionref.h>
#include <libbasardbaspect_accessor.h>
#include "dmbase.h"

#include "ifindpurchaseordersupplier.h"

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

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace purchaseordersupplierdm
{
    //-------------------------------------------------------------------------------------------------//
    // class declaration section
    //-------------------------------------------------------------------------------------------------//
    class PurchaseOrderSupplierDM : public IFindPurchaseOrderSupplier,
	                                private DMBase
	{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
        public:
            static const libutil::misc::ClassInfo& getClassInfo();

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
	    public:
		    PurchaseOrderSupplierDM( const basar::db::aspect::ConnectionRef );
		    ~PurchaseOrderSupplierDM();

            // defined in ifindpurchaseordersupplier interface
            bool                                                        isEmpty() const;
			void                                                        findPurchaseOrderSupplierById( basar::Int32 purchaseOrderSupplierNo );

	        basar::db::aspect::AccessorPropertyTable_YIterator          get();
            basar::db::aspect::AccessorPropertyTable_YIterator          get() const;
            void                                                        clear();

            void                                                        resetSearchYit();
	        basar::db::aspect::AccessorPropertyTable_YIterator          getSearchYit();

        protected:
            // from DMBase
            AccessorExecuteParameterization						        getFindByPatternExecuteParameters() const;
            basar::VarString									        getSearchYIteratorAttributeNames()  const;

	    private:
            /////////////////////////////////////////////////////////////////////////
		    // private member methods
		    /////////////////////////////////////////////////////////////////////////
		    PurchaseOrderSupplierDM(const PurchaseOrderSupplierDM& PurchaseOrderSupplierDM);
		    PurchaseOrderSupplierDM& operator= (const PurchaseOrderSupplierDM& PurchaseOrderSupplierDM);
            
		    /////////////////////////////////////////////////////////////////////////
		    // private member variables
		    /////////////////////////////////////////////////////////////////////////
	};
} // end namespace purchaseordersupplierdm
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_PURCHASEORDERSUPPLIERDM_PURCHASEORDERSUPPLIERDM_H
