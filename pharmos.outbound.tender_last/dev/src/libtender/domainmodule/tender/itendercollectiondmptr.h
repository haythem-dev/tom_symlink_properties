#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERCOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERCOLLECTIONDMPTR_H

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
            class ITenderCollectionDM;
            typedef boost::shared_ptr< libtender::domMod::tender::ITenderCollectionDM > ITenderCollectionDMPtr;
        }
    }
}

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_ITENDERCOLLECTIONDMPTR_H
