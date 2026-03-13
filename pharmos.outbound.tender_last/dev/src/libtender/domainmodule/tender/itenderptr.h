#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPTR_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace tender
{
	class ITender;
	typedef boost::shared_ptr< ITender > ITenderPtr;
}
}
}

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPTR_H
