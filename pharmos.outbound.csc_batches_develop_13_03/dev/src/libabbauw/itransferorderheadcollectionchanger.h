#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONCHANGER_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONCHANGER_H

#include "itransferorderheadcollectiongetter.h"

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
			using basar::db::aspect::AccessorPropertyTable_YIterator;
			using basar::db::aspect::AccessorPropertyTableRef;

			class ITransferOrderHeadCollectionChanger : virtual public ITransferOrderHeadCollectionGetter
			{
			public:
				virtual ~ITransferOrderHeadCollectionChanger() {}

				virtual bool                                            isChanged() const = 0;
				virtual bool                                            save(basar::VarString strDiscount) = 0;
								
				virtual void											setBackupAllHeadPosCollection() = 0;
				
				virtual std::vector<AccessorPropertyTable_YIterator>	getBackupAllHeadPosCollection() = 0;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONCHANGER_H
