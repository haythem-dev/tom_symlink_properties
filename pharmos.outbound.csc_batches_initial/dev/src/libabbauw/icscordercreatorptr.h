#ifndef GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERCREATORPTR_H
#define GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERCREATORPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace cscorder
{

    class ICSCOrderCreator;
    typedef boost::shared_ptr< ICSCOrderCreator > ICSCOrderCreatorPtr;
	
} // end namespace cscorder
} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERCREATORPTR_H
