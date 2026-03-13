//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  
 *  \author  
 *  \date    */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_CSC_BATCHES_VIEWCONN_IARTICLEFINDERVCPTR_H
#define GUARD_CSC_BATCHES_VIEWCONN_IARTICLEFINDERVCPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace viewConn
    {
        class IArticleFinderVC;
        typedef boost::shared_ptr< IArticleFinderVC > IArticleFinderVCPtr;
    }
}

#endif
