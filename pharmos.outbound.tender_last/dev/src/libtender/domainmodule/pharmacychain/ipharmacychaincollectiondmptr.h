#ifndef GUARD_LIBTENDER_DOMMOD_CUSTOMER_IPHARMACYCHAINCOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_CUSTOMER_IPHARMACYCHAINCOLLECTIONDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace pharmacychain
{
	class IPharmacyChainCollectionDM;
	typedef boost::shared_ptr< IPharmacyChainCollectionDM > IPharmacyChainCollectionDMPtr;

} //end namespace pharmacychain
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_CUSTOMER_IPHARMACYCHAINCOLLECTIONDMPTR_H