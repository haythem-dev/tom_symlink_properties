#ifndef GUARD_CSC_BATCHES_VIEWCONN_ITRANSFERORDERCREATORVCPTR_H
#define GUARD_CSC_BATCHES_VIEWCONN_ITRANSFERORDERCREATORVCPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace viewConn
    {
        class ITransferOrderCreatorVC;
        typedef boost::shared_ptr< ITransferOrderCreatorVC > ITransferOrderCreatorVCPtr;
    }
}

#endif // GUARD_CSC_BATCHES_VIEWCONN_ITRANSFERORDERCREATORVCPTR_H
