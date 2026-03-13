#ifndef GUARD_LIBABBAUW_PARAMETER_IORDERCREATIONPARAMETER_H
#define GUARD_LIBABBAUW_PARAMETER_IORDERCREATIONPARAMETER_H

//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class IOrderCreationParameter
{
public:	
    virtual ~IOrderCreationParameter() {}

	virtual bool                                        getFormatHeaderTextEnabled()             = 0;
};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif	// end GUARD_LIBABBAUW_PARAMETER_IORDERCREATIONPARAMETER_H
