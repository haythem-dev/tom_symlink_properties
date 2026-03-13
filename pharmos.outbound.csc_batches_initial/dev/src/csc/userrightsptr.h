#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_RIGHTS_USERRIGHTSPTR_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_RIGHTS_USERRIGHTSPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
namespace infrastructure
{
namespace rights
{
    class UserRights;
    typedef boost::shared_ptr< UserRights > UserRightsPtr;

} // end namespace parameter
} // end namespace infrastructure
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_RIGHTS_USERRIGHTSPTR_H
