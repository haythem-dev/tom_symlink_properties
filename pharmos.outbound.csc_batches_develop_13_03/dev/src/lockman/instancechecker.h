#ifndef GUARD_LIBLOCKMANAGER_MISC_INSTANCECHECKER_H
#define GUARD_LIBLOCKMANAGER_MISC_INSTANCECHECKER_H

#include "invalidinstanceexception.h"
#include <libbasar_definitions.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section 
//-------------------------------------------------------------------------------------------------//
namespace basar
{
namespace cmnutil
{
    class ExceptInfoStruct;
}
}

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libLockManager
{
namespace misc
{

    class InstanceChecker
    {
    public:
        InstanceChecker()
        {
        }
        
        static void check( const void* ptr, const basar::ExceptInfo& exceptInfo )
        {
            if( 0 == ptr )
            {
                throw exceptions::InvalidInstanceException( exceptInfo );
            }
        }
    };

} // end namespace misc
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_MISC_INSTANCECHECKER_H

