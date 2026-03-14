#ifndef GUARD_CSC_BATCHES_USECASE_TRANSFERORDERCREATORUCPTR_H
#define GUARD_CSC_BATCHES_USECASE_TRANSFERORDERCREATORUCPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace useCase
    {
        class TransferOrderCreatorUC;
        typedef boost::shared_ptr< TransferOrderCreatorUC > TransferOrderCreatorUCPtr;
    }
}

#endif // GUARD_CSC_BATCHES_USECASE_TRANSFERORDERCREATORUCPTR_H
