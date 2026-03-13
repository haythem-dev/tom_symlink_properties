//----------------------------------------------------------------------------
/*! \file
 *  \brief
 *  \author Bischof Bjoern
 *  \date   27.07.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_TENDERORDERCREATOR_COMPONENTMANAGER_IUSECASEGETTER_H
#define GUARD_TENDERORDERCREATOR_COMPONENTMANAGER_IUSECASEGETTER_H


//------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------//
#include <libutil/irunnableptr.h>

//------------------------------------------------------------------------------//
// forward declaration
//------------------------------------------------------------------------------//

//------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------//
namespace tenderordercreator
{
    namespace componentManager
    {
        //------------------------------------------------------------------------------//
        // class IUseCaseGetter
        //------------------------------------------------------------------------------//
        class IUseCaseGetter
        {
            public:
                virtual ~IUseCaseGetter(){};

                virtual libutil::useCase::IRunnablePtr getMainUC() = 0;
        };
    }
}

#endif
