#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IDAYSTOCOLORPARAMETER_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IDAYSTOCOLORPARAMETER_H

namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class IDaysToColorParameter
{
public:	
    virtual ~IDaysToColorParameter() {}

	virtual basar::Int32                                        getDaysToColor()                         = 0;
};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_PARAMETER_IDAYSTOCOLORPARAMETER_H
