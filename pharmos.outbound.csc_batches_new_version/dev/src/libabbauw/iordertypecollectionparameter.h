#ifndef GUARD_LIBABBAUW_PARAMETER_IORDERTYPECOLLECTIONPARAMETER_H
#define GUARD_LIBABBAUW_PARAMETER_IORDERTYPECOLLECTIONPARAMETER_H

#include <vector>

//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class IOrderTypeCollectionParameter
{
public:	
    virtual ~IOrderTypeCollectionParameter() {}

    typedef std::vector< basar::VarString >                     OrderTypeCollection;
	virtual OrderTypeCollection                                 getOrderTypes()             = 0;
    virtual OrderTypeCollection                                 getOrderTypesForFiltering() = 0;
};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif	// end GUARD_LIBABBAUW_PARAMETER_IORDERTYPECOLLECTIONPARAMETER_H
