#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_PARAMETER_APPLPARAMETERPTR_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_PARAMETER_APPLPARAMETERPTR_H

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
namespace parameter
{
    class ApplParameter;
    typedef boost::shared_ptr< ApplParameter > ApplParameterPtr;

} // end namespace parameter
} // end namespace infrastructure
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_PARAMETER_APPLPARAMETERPTR_H
