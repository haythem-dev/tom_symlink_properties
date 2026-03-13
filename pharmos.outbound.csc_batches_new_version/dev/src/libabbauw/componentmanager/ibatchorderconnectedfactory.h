#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_IBATCHORDERCONNECTEDFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_IBATCHORDERCONNECTEDFACTORY_H

#include "domainmodule/batchorder/ibatchorderptr.h"

//------------------------------------------------------------------------------//
// Forward declarations
//------------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
        }
    }
}

namespace libabbauw
{
namespace componentManager
{
class IBatchOrderConnectedFactory
{
public:
    virtual ~IBatchOrderConnectedFactory() {}

    virtual domMod::batchOrder::IBatchOrderPtr create( basar::db::aspect::AccessorPropertyTable_YIterator ) const = 0;
};

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_IBATCHORDERCONNECTEDFACTORY_H
