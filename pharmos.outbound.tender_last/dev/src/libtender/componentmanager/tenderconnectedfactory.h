#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_TENDERCONNECTEDFACTORY_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_TENDERCONNECTEDFACTORY_H

//------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------//
#include "tenderfactorybase.h"
#include "itenderconnectedfactory.h"

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
        class TenderConnectedFactory : public /*protected*/ TenderFactoryBase,
                                       public ITenderConnectedFactory
        {
            public:
                TenderConnectedFactory();
                virtual ~TenderConnectedFactory();

                virtual domMod::tender::ITenderPtr create( basar::db::aspect::AccessorPropertyTable_YIterator ) const;

            private:
                TenderConnectedFactory( const TenderConnectedFactory& );
                TenderConnectedFactory& operator=( const TenderConnectedFactory& );
        };
    }
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_TENDERCONNECTEDFACTORY_H
