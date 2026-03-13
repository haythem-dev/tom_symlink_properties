#ifndef GUARD_LIBTENDER_DOMMOD_CUSTOMER_IPARTNERCOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_CUSTOMER_IPARTNERCOLLECTIONDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace partner
{
	class IPartnerCollectionDM;
	typedef boost::shared_ptr< IPartnerCollectionDM > IPartnerCollectionDMPtr;

} //end namespace partner
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_CUSTOMER_IPARTNERCOLLECTIONDMPTR_H