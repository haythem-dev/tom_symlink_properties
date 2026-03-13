//----------------------------------------------------------------------------
/*! \file
 *  \brief
 *  \author Bischof Bjoern
 *  \date   27.07.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_TENDERBATCHPROCESSING_COMPONENTMANAGER_IUSECASEGETTER_H
#define GUARD_TENDERBATCHPROCESSING_COMPONENTMANAGER_IUSECASEGETTER_H


//------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------//
#include <libutil/irunnableptr.h>
#include "usecase/iomgcalcbaseucptr.h"

//------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------//
namespace tenderbatchprocessing
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
                virtual libutil::useCase::IRunnablePtr getOMGCalcUC() = 0;
                virtual libutil::useCase::IRunnablePtr getOMGTransferUC() = 0;
                virtual tenderbatchprocessing::useCase::IOMGCalcBaseUCPtr getOMGCustomerCalcUC() = 0;
                virtual tenderbatchprocessing::useCase::IOMGCalcBaseUCPtr getOMGSupplierCalcUC() = 0;
        };
    }
}

#endif
