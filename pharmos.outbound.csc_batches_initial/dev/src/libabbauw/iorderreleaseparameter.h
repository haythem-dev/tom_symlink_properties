#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERRELEASEPARAMETER_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERRELEASEPARAMETER_H

//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class IOrderReleaseParameter
{
public:	
    virtual ~IOrderReleaseParameter() {}

	virtual bool                                                    releaseRedYellowOrdersPostponed() = 0;
	virtual basar::VarString										getDiscountPctTrailer() = 0;
	virtual basar::VarString										getOrderrouting() = 0;
};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif	// GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERRELEASEPARAMETER_H
