#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROTOCOLPTR_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROTOCOLPTR_H

//----------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//----------------------------------------------------------------------------//
// typedef section
//----------------------------------------------------------------------------//
namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            class ITenderProtocol;
            typedef boost::shared_ptr< libtender::domMod::tender::ITenderProtocol > ITenderProtocolPtr;
        }
    }
}

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROTOCOLPTR_H
