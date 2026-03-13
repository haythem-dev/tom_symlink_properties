#ifndef GUARD_LIBABBAUW_TRANSFERORDERHEADCOLLECTIONACC_H
#define GUARD_LIBABBAUW_TRANSFERORDERHEADCOLLECTIONACC_H

namespace libabbauw
{
	namespace acc_transfer_order_head_collection
	{
		using basar::db::aspect::SQLStringBuilder;

		BEGIN_QUERY_BUILDER_DECLARATION(SelectTransferOrders)
	private:
		basar::I18nString basarDateToDBDateStr(const basar::Date&);
		END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION(SelectTransferOrderByKey)
			END_BUILDER_DECLARATION

			BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(SelectTransferOrderTotalNumber)
			END_BUILDER_DECLARATION

	} // end namespace acc_transfer_order_head_collection
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_TRANSFERORDERHEADCOLLECTIONACC_H
