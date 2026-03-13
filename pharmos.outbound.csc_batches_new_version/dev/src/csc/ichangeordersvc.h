#ifndef GUARD_CSC_BATCHES_VIEWCONN_ICHANGEORDERSVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_ICHANGEORDERSVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasarguitie_definitions.h>
#include <libbasarguitie_msgboxes.h.>
#include <vector>

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

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
	namespace viewConn
	{

		class IChangeOrdersVC
		{
		public:

			virtual ~IChangeOrdersVC() {};

			virtual void                                                        init(basar::db::aspect::AccessorPropertyTableRef) = 0;

			virtual basar::gui::tie::WidgetReturnEnum                           show() = 0;
			virtual void                                                        waitForEvents() = 0;
			virtual bool								                        shutdown() = 0;

			virtual void                                                        info(const basar::I18nString&) = 0;
			virtual void                                                        error(const basar::I18nString&) = 0;
			virtual const basar::gui::tie::MessageBoxButtonEnum                 question(const basar::I18nString&) = 0;

			virtual void                                                        setSavingAllowed(bool) = 0;

			virtual void                                                        matchToOrders() = 0;
			virtual void														matchFromOrders() = 0;

			virtual void														matchFromPharmacy(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
			virtual void                                                        matchToPharmacy(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;

			virtual void														matchFromManufacturer(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
			virtual void                                                        matchToManufacturer(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;

			virtual void														matchFromOrderDiscount(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
			virtual void                                                        matchToOrderDiscount(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
			virtual basar::VarString											getOrderDiscount() = 0;
		};

	} // end namespace viewConn
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_ICHANGEORDERSVC_H
