#ifndef GUARD_TENDER_VIEWCONN_ITENDERCONTRACTINFOPTR_H
#define GUARD_TENDER_VIEWCONN_ITENDERCONTRACTINFOPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace viewConn
    {
        class ITenderContractInfo;
        typedef boost::shared_ptr< ITenderContractInfo > ITenderContractInfoPtr;
    }
}

#endif //end GUARD_TENDER_VIEWCONN_ITENDERCONTRACTINFOPTR_H
