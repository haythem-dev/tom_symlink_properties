#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ICSCORDERGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ICSCORDERGETTER_H

#include "iordergetterptr.h"
#include "icscorderreleaserptr.h"

namespace libabbauw
{
namespace componentManager
{
class ICSCOrderGetter
{
    public:
        virtual ~ICSCOrderGetter() {}

        virtual libabbauw::domMod::cscorder::ICSCOrderReleaserPtr     getCSCOrderDM( domMod::cscorder::IOrderGetterPtr ) = 0;
};

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ICSCORDERGETTER_H
