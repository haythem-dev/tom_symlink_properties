#ifndef GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERRELEASER_H
#define GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERRELEASER_H

namespace libabbauw
{
namespace domMod
{
namespace cscorder
{

class ICSCOrderReleaser
{
public:
	virtual ~ICSCOrderReleaser() {}

	virtual void                                                release( bool postpone )                         = 0;
};

} // end namespace cscorder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_CSCORDER_ICSCORDERRELEASER_H
