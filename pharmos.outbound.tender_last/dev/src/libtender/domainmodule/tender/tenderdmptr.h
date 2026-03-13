#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDERDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDERDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace tender
{
	class TenderDM;
	typedef boost::shared_ptr< TenderDM > TenderDMPtr;
}
}
}

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_TENDERDMPTR_H
