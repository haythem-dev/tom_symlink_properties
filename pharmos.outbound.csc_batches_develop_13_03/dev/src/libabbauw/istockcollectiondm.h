#ifndef GUARD_LIBABBAUW_DOMMOD_STOCK_ISTOCKCOLLECTIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_STOCK_ISTOCKCOLLECTIONDM_H

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
		namespace stock
		{

			class IStockCollectionDM
			{
			public:
				virtual ~IStockCollectionDM() {}

				virtual void                                               resetSearchYit() = 0;
				virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit() = 0;
				virtual bool                                               itemInStock(const basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
				virtual bool                                               itemInTransitStorage(const basar::db::aspect::AccessorPropertyTable_YIterator) = 0;

				virtual void                                               findStock(const basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
				virtual basar::Int32                                       findCMIReservedQuantity(basar::Int32 articleNo) = 0;
				virtual basar::db::aspect::AccessorPropertyTable_YIterator findByOrderNo(basar::Int32 orderNo) = 0;
				virtual basar::db::aspect::AccessorPropertyTable_YIterator findOrderPositionInOrder(basar::Int32 orderNo, basar::Int32 position) = 0;

				virtual bool                                               orderContainsNonStockItem(const basar::Int32 orderNo) = 0;
			};

		} // end namespace stock
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_STOCK_ISTOCKCOLLECTIONDM_H
