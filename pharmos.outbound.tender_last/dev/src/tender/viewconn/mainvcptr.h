//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief
 *  \author Bischof Bjoern
 *  \date   05.03.2012 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_TENDER_VIEWCONN_MAINVCPTR_H
#define GUARD_TENDER_VIEWCONN_MAINVCPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace viewConn
    {
        class MainVC;
        typedef boost::shared_ptr< MainVC > MainVCPtr;
    }
}
#endif //end GUARD_TENDER_VIEWCONN_MAINVCPTR_H
