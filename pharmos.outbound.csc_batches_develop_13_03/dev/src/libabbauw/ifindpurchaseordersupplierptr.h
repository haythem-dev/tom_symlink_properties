#ifndef GUARD_LIBABBAUW_DOMMOD_PURCHASEORDERSUPPLIERDM_IFINDPURCHASEORDERSUPPLIERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_PURCHASEORDERSUPPLIERDM_IFINDPURCHASEORDERSUPPLIERPTR_H

#include <boost/shared_ptr.hpp>
namespace libabbauw
{
namespace domMod 
{
    namespace purchaseordersupplierdm
    {
        class IFindPurchaseOrderSupplier;
		typedef boost::shared_ptr< IFindPurchaseOrderSupplier >IFindPurchaseOrderSupplierPtr;
    }
}
}
#endif //GUARD_LIBABBAUW_DOMMOD_PURCHASEORDERSUPPLIERDM_IFINDPURCHASEORDERSUPPLIERPTR_H
