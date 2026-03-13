#ifndef GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERRELEASERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERRELEASERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace cscorder
{

    class ICSCOrderReleaser;
    typedef boost::shared_ptr< ICSCOrderReleaser > ICSCOrderReleaserPtr;
	
} // end namespace cscorder
} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERRELEASERPTR_H
