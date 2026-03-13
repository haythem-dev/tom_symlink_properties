#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_ITENDERCONNECTEDFACTORY_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_ITENDERCONNECTEDFACTORY_H

//------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------//

//------------------------------------------------------------------------------//
//
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

namespace domMod
{
    namespace tender
    {
        class ITenderPtr;
    }
}

//------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------//
namespace libtender
{
    namespace componentManager
    {
        //------------------------------------------------------------------------------//
        //
        //------------------------------------------------------------------------------//
        class ITenderConnectedFactory
        {
            public:
                virtual ~ITenderConnectedFactory(){};
                virtual domMod::tender::ITenderPtr create( basar::db::aspect::AccessorPropertyTable_YIterator ) const = 0;
        };
    }
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_ITENDERCONNECTEDFACTORY_H
