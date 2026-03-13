#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROGRESSPTR_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROGRESSPTR_H

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
            class ITenderProgress;
            typedef boost::shared_ptr< libtender::domMod::tender::ITenderProgress > ITenderProgressPtr;
        }
    }
}

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERPROGRESSDMPTR_H
