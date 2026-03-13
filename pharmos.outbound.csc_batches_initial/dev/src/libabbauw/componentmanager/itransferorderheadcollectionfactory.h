#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADCOLLECTIONFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADCOLLECTIONFACTORY_H

#include "itransferorderheadcollectiondmptr.h"
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
		class ITransferOrderHeadCollectionFactory
		{
		public:
			virtual ~ITransferOrderHeadCollectionFactory() {}

			typedef std::vector< basar::db::aspect::AccessorPropertyTable_YIterator > AccessorPropertyTableYIteratorCollection;
			virtual domMod::transferOrder::ITransferOrderHeadCollectionDMPtr create(const AccessorPropertyTableYIteratorCollection&) = 0;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADCOLLECTIONFACTORY_H

