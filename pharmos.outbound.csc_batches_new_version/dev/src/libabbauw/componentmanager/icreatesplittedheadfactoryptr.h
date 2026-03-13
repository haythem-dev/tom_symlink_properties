#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ICREATESPLITTEDHEADFACTORYPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ICREATESPLITTEDHEADFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace componentManager 
{
    class ICreateSplittedHeadFactory;
    typedef boost::shared_ptr< ICreateSplittedHeadFactory > ICreateSplittedHeadFactoryPtr;
	
} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_COMPONENTMANAGER_ICREATESPLITTEDHEADFACTORYPTR_H
