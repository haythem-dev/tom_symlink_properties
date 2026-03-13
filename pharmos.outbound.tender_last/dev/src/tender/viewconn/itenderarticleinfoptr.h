#ifndef GUARD_TENDER_VIEWCONN_ITENDERARTICLEINFOPTR_H
#define GUARD_TENDER_VIEWCONN_ITENDERARTICLEINFOPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace viewConn
    {
        class ITenderArticleInfo;
        typedef boost::shared_ptr< ITenderArticleInfo > ITenderArticleInfoPtr;
    }
}

#endif //end GUARD_TENDER_VIEWCONN_ITENDERARTICLEINFOPTR_H
