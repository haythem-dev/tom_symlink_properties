#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERHEADER_PTR_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERHEADER_PTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace tender
{
	class ITenderHeader;
	typedef boost::shared_ptr< ITenderHeader > ITenderHeaderPtr;
}
}
}

#endif // GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERHEADER_PTR_H
