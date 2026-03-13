#ifndef GUARD_LIBTENDER_DOMMOD_CUSTOMER_SUPPLIERCOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_CUSTOMER_SUPPLIERCOLLECTIONDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace supplier
{
    class SupplierCollectionDM;
    typedef boost::shared_ptr< SupplierCollectionDM > SupplierCollectionDMPtr;

} //end namespace supplier
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_CUSTOMER_SUPPLIERCOLLECTIONDMPTR_H