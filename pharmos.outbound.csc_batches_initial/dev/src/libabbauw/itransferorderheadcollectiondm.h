#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONDM_H

#include "itransferorderheadcollectionfinder.h"
#include "itransferorderheadcollectionchanger.h"

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
	namespace domMod
	{
		namespace transferOrder
		{

			class ITransferOrderHeadCollectionDM : public ITransferOrderHeadCollectionFinder, public ITransferOrderHeadCollectionChanger
			{
			public:
				virtual ~ITransferOrderHeadCollectionDM() {}
				using ITransferOrderHeadCollectionGetter::get; // the same get() method is included in both derived interfaces

				virtual void                                               addTransferOrderHead(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONDM_H

