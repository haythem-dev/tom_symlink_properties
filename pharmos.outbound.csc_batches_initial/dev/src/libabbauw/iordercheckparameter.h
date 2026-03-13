#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERCHECKPARAMETER_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERCHECKPARAMETER_H

//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class IOrderCheckParameter
{
public:	
    virtual ~IOrderCheckParameter() {}

	virtual bool                                        emitWarningOnNonChargedQty()             = 0;
};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif	// end GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERCHECKPARAMETER_H
