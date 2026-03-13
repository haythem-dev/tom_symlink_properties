#ifndef GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGTRANSFERDMPTR_H
#define GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGTRANSFERDMPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderbatchprocessing
{
namespace domMod
{
namespace omgtransfer
{
    class OMGTransferDM;
    typedef boost::shared_ptr< OMGTransferDM > OMGTransferDMPtr;
}
}
}

#endif //end GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGTRANSFERDMPTR_H