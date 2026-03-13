#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROTOCOLGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROTOCOLGETTER_H

#include "iorderprotocolgetterptr.h"
#include "domainmodule/orderprotocol/iorderprotocolptr.h"


namespace libabbauw
{
namespace componentManager
{
class IOrderProtocolGetter
{
    public:
        virtual ~IOrderProtocolGetter() {}

		virtual libabbauw::domMod::orderProtocol::IOrderProtocolPtr getOrderProtocolDM() = 0;
};

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROTOCOLGETTER_H
