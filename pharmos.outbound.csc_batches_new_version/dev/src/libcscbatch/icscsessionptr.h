#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_SESSION_ICSCSESSIONPTR_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_SESSION_ICSCSESSIONPTR_H

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
namespace session
{
    class ICSCSession;
    typedef boost::shared_ptr< ICSCSession > ICSCSessionPtr;

} // end namespace session
} // end namespace infrastructure
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_SESSION_ICSCSESSIONPTR_H
