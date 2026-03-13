#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERPARAMETER_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERPARAMETER_H

#include "idaystocolorparameter.h"

namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class IOrderParameter : public IDaysToColorParameter
{
public:	
    virtual ~IOrderParameter() {}

    virtual basar::Int32                                        getLargeScaleOrderLimit()                = 0;
};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERPARAMETER_H
