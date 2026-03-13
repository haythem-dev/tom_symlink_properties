#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_RIGHTS_IUSERRIGHTSPTR_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_RIGHTS_IUSERRIGHTSPTR_H

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
    class IUserRights;
    typedef boost::shared_ptr< IUserRights > IUserRightsPtr;

} // end namespace parameter
} // end namespace infrastructure
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_RIGHTS_IUSERRIGHTSPTR_H
