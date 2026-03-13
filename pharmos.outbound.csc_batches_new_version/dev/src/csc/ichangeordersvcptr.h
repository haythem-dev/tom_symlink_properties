//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  
 *  \author  
 *  \date    */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_CSC_BATCHES_VIEWCONN_ITCHANGE_ORDER_VCPTR_H
#define GUARD_CSC_BATCHES_VIEWCONN_ITCHANGE_ORDER_VCPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace viewConn
    {
        class IChangeOrdersVC;
        typedef boost::shared_ptr< IChangeOrdersVC > IChangeOrdersVCPtr;
    }
}

#endif // GUARD_CSC_BATCHES_VIEWCONN_ITCHANGE_ORDER_VCPTR_H
