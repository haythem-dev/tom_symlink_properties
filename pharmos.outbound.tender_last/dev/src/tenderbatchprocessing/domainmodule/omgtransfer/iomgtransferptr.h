#ifndef GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGTRANSFERPTR_H
#define GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGTRANSFERPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderbatchprocessing
{
namespace domMod
{
namespace omgtransfer
{
    class IOMGTransfer;
    typedef boost::shared_ptr< IOMGTransfer > IOMGTransferPtr;
}
}
}

#endif //end GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGTRANSFERPTR_H