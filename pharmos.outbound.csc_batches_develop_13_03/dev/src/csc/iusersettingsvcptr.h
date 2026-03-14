//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  
 *  \author  
 *  \date    */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_CSC_BATCHES_VIEWCONN_IUSER_SETTINGS_VCPTR_H
#define GUARD_CSC_BATCHES_VIEWCONN_IUSER_SETTINGS_VCPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace viewConn
    {
        class IUserSettingsVC;
        typedef boost::shared_ptr< IUserSettingsVC > IUserSettingsVCPtr;
    }
}

#endif // GUARD_CSC_BATCHES_VIEWCONN_IUSER_SETTINGS_VCPTR_H
