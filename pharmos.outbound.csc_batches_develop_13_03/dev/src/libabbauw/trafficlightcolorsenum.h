#ifndef GUARD_LIBABBAUW_PROPERTIES_TRAFFICLIGHTSCOLORENUM_H
#define GUARD_LIBABBAUW_PROPERTIES_TRAFFICLIGHTSCOLORENUM_H

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{
    // bitmask: don't change values
	enum TrafficLightEnum
	{
		NO_COLOR,
		GREEN,
		YELLOW,
		RED     = 4,
		PURPLE  = 8
	};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_PROPERTIES_TRAFFICLIGHTSCOLORENUM_H
