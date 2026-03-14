#ifndef GUARD_CSC_BATCHES_USECASE_TENDERCREATOR_UC_PTR_H
#define GUARD_CSC_BATCHES_USECASE_TENDERCREATOR_UC_PTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace useCase
    {
        class TenderCreatorUC;
        typedef boost::shared_ptr<TenderCreatorUC> TenderCreatorUCPtr;
    }
}

#endif // end GUARD_CSC_BATCHES_USECASE_TENDERCREATOR_UC_PTR_H
