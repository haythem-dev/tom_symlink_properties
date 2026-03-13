#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROGRESSDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROGRESSDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            class TenderProgressDM;
            typedef boost::shared_ptr< TenderProgressDM > TenderProgressDMPtr;
        }
    }
}

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPROGRESSDMPTR_H
