#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_STATE_COLLECTION_PTR_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_STATE_COLLECTION_PTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace tender
{
	class TenderStateCollection;
	typedef boost::shared_ptr< TenderStateCollection > TenderStateCollectionPtr;
}
}
}

#endif
