#ifndef GUARD_TENDER_VIEWCONN_ITENDERSEARCHCRITERIAPTR_H
#define GUARD_TENDER_VIEWCONN_ITENDERSEARCHCRITERIAPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace viewConn
    {
        class ITenderSearchCriteria;
        typedef boost::shared_ptr< ITenderSearchCriteria > ITenderSearchCriteriaPtr;
    }
}

#endif //end GUARD_TENDER_VIEWCONN_ITENDERSEARCHCRITERIAPTR_H
