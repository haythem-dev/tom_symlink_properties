#ifndef GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDERMAINTENANCEVCPTR_H
#define GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDERMAINTENANCEVCPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace viewConn
    {
        class IBatchOrderMaintenanceVC;
        typedef boost::shared_ptr< IBatchOrderMaintenanceVC > IBatchOrderMaintenanceVCPtr;
    }
}

#endif // GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDERMAINTENANCEVCPTR_H