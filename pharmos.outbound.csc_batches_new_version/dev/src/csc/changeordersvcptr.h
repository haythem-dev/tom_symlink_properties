#ifndef GUARD_CSC_BATCHES_VIEWCONN_CHANGEORDERSVCPTR_H
#define GUARD_CSC_BATCHES_VIEWCONN_CHANGEORDERSVCPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace viewConn
    {
        class ChangeOrdersVC;
        typedef boost::shared_ptr< ChangeOrdersVC > ChangeOrdersVCPtr;
    }
}

#endif // GUARD_CSC_BATCHES_VIEWCONN_CHANGEORDERSVCPTR_H
