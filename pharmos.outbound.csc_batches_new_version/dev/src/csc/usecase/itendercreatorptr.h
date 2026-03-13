#ifndef GUARD_CSCBATCHES_USECASE_ITENDERCREATORPTR_H
#define GUARD_CSCBATCHES_USECASE_ITENDERCREATORPTR_H

#include <boost/shared_ptr.hpp>
namespace CSC_Batches
{
    namespace useCase
    {
        class ITenderCreator;
        typedef boost::shared_ptr<ITenderCreator> ITenderCreatorPtr;
    }
}

#endif // GUARD_CSCBATCHES_USECASE_ITENDERCREATORPTR_H
