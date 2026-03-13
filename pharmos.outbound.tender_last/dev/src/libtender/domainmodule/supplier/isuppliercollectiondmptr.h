#ifndef GUARD_LIBTENDER_DOMMOD_CUSTOMER_ISUPPLIERCOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_CUSTOMER_ISUPPLIERCOLLECTIONDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace supplier
{
    class ISupplierCollectionDM;
    typedef boost::shared_ptr< ISupplierCollectionDM > ISupplierCollectionDMPtr;

} //end namespace supplier
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_CUSTOMER_ISUPPLIERCOLLECTIONDMPTR_H