#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONFINDER_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONFINDER_H

#include "itransferorderheadcollectiongetter.h"
#include "itransferorderptr.h"
#include "reservequantityfactortype.h"

#include <libbasar_definitions.h>
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
	namespace domMod
	{
		namespace transferOrder
		{

			class ITransferOrderHeadCollectionFinder : virtual public ITransferOrderHeadCollectionGetter
			{
			public:
				virtual ~ITransferOrderHeadCollectionFinder() {}
				typedef std::vector< basar::db::aspect::AccessorPropertyTable_YIterator > YIterators;

				virtual void                                               resetSearchYit() = 0;
				virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit() = 0;
				virtual void                                               findByPattern(const basar::db::aspect::AccessorPropertyTable_YIterator, bool isOrderView) = 0;
				virtual void                                               findByKey(const basar::db::aspect::AccessorPropertyTable_YIterator) = 0;

				virtual basar::Int32                                       getNumberOfGreenOrders() const = 0;
				virtual void                                               getGreenOrders(YIterators& greenOrders) = 0;
				virtual basar::Int32                                       getTotalNumberOfOrders() = 0;

				virtual transferOrder::ITransferOrderPtr                   getOrder(const basar::db::aspect::AccessorPropertyTable_YIterator) = 0;

				virtual void                                               setReserveQtyFactorType(ReserveQuantityFactorType) = 0;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONFINDER_H
