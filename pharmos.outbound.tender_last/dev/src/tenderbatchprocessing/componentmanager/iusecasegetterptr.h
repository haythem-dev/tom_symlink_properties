//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief
 *  \author Bischof Bjoern
 *  \date   27.07.2013 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_TENDERBATCHPROCESSING_COMPONENTMANAGER_IUSECASEGETTERPTR_H
#define GUARD_TENDERBATCHPROCESSING_COMPONENTMANAGER_IUSECASEGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderbatchprocessing
{
    namespace componentManager
    {
        class IUseCaseGetter;
        typedef boost::shared_ptr< IUseCaseGetter > IUseCaseGetterPtr;
    }
}

#endif 
