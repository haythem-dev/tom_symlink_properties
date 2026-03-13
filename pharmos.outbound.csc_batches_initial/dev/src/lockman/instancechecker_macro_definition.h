#ifndef GUARD_LIBLOCKMANAGER_MISC_INSTANCECHECKER_MACRO_DEFINITION_H
#define GUARD_LIBLOCKMANAGER_MISC_INSTANCECHECKER_MACRO_DEFINITION_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "instancechecker.h"

//-------------------------------------------------------------------------------------------------//
// macro define section
//-------------------------------------------------------------------------------------------------//
namespace libLockManager
{
namespace misc
{
#ifdef _DEBUG
    #define CHECK_INSTANCE_EXCEPTION( ptr ) libLockManager::misc::InstanceChecker::check( ptr.get(), basar::ExceptInfo("", "", __FILE__, __LINE__) );
#else
    #define CHECK_INSTANCE_EXCEPTION( ptr )
#endif

} // end namespace misc
} // end namespace libLockManager
#endif // GUARD_LIBLOCKMANAGER_MISC_INSTANCECHECKER_MACRO_DEFINITION_H
