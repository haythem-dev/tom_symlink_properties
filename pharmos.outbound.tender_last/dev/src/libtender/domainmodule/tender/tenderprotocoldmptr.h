#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROTOCOLDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROTOCOLDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            class TenderProtocolDM;
            typedef boost::shared_ptr< TenderProtocolDM > TenderProtocolDMPtr;
        }
    }
}

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROTOCOLDMPTR_H
