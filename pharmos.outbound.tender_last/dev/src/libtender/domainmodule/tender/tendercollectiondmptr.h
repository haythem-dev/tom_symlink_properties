#ifndef GUARD_LIBTENDER_DOMMOD_TEBDER_TENDERCOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_TEBDER_TENDERCOLLECTIONDMPTR_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//-------------------------------------------------------------------------------------------------//
// typedef section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            class TenderCollectionDM;
            typedef boost::shared_ptr< TenderCollectionDM > TenderCollectionDMPtr;
        }
    }
}

#endif //end GUARD_LIBTENDER_DOMMOD_TEBDER_TENDERCOLLECTIONDMPTR_H
