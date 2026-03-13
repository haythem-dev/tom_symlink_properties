#ifndef GUARD_TENDER_VIEWCONN_TENDERPROTOCOLVCPTR_H
#define GUARD_TENDER_VIEWCONN_TENDERPROTOCOLVCPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace viewConn
    {
        class TenderProtocolVC;
        typedef boost::shared_ptr< TenderProtocolVC > TenderProtocolVCPtr;

    } // end namespace viewConn
} // end namespace tender

#endif //end GUARD_TENDER_VIEWCONN_TENDERPROTOCOLVCPTR_H
