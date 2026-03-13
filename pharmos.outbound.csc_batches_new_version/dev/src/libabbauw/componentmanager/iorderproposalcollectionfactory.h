#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALCOLLECTIONFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALCOLLECTIONFACTORY_H

#include "domainmodule/orderproposal/iorderproposalcollectionptr.h"
#include <vector>

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
		class IOrderProposalCollectionFactory
		{
		public:
			virtual ~IOrderProposalCollectionFactory() {}

			typedef std::vector< basar::db::aspect::AccessorPropertyTable_YIterator > AccessorPropertyTableYIteratorCollection;
			virtual domMod::orderProposal::IOrderProposalCollectionPtr create(const AccessorPropertyTableYIteratorCollection&) = 0;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALCOLLECTIONFACTORY_H

