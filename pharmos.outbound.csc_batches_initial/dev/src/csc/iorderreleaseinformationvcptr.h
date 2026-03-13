#ifndef GUARD_CSCBATCHES_VIEWCONN_IORDERRELEASEINFORMATIONVCPTR_H
#define GUARD_CSCBATCHES_VIEWCONN_IORDERRELEASEINFORMATIONVCPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace viewConn
    {
        class IOrderReleaseInformationVC;
        typedef boost::shared_ptr< IOrderReleaseInformationVC > IOrderReleaseInformationVCPtr;
    }
}

#endif // GUARD_CSCBATCHES_VIEWCONN_IORDERRELEASEINFORMATIONVCPTR_H
