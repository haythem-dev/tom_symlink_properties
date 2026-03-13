#ifndef GUARD_TENDER_VIEWCONN_TENDERINVOICESVCPTR_H
#define GUARD_TENDER_VIEWCONN_TENDERINVOICESVCPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace viewConn
    {
        class TenderInvoicesVC;
        typedef boost::shared_ptr< TenderInvoicesVC > TenderInvoicesVCPtr;

    } // end namespace viewConn
} // end namespace tender

#endif //end GUARD_TENDER_VIEWCONN_TENDERINVOICESVCPTR_H
