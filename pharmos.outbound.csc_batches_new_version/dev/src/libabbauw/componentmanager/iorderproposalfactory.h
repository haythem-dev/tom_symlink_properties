#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALFACTORY_H

#include "domainmodule/orderproposal/iorderproposalptr.h"

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libabbauw
{
namespace componentManager
{
    class IOrderProposalFactory
    {
    public:
        virtual ~IOrderProposalFactory() {}
        
        virtual domMod::orderProposal::IOrderProposalPtr             create() = 0;
        virtual domMod::orderProposal::IOrderProposalPtr             create( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
    };

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALFACTORY_H
