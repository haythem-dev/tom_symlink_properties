//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief
 *  \author Bischof Bjoern
 *  \date   23.06.2013 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_LIBOBST_DOMMOD_CUSTOMERORDER_CUSTOMERORDERPOSITIONCOLLECTIONPTR_H
#define GUARD_LIBOBST_DOMMOD_CUSTOMERORDER_CUSTOMERORDERPOSITIONCOLLECTIONPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            class TenderPositionCollection;
            typedef boost::shared_ptr< TenderPositionCollection > TenderPositionCollectionPtr;
        }
    }
}

#endif
