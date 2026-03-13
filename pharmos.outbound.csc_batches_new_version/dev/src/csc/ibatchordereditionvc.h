#ifndef GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDEREDITIONVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDEREDITIONVC_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasarguitie_definitions.h>
#include <libbasarguitie_msgboxes.h.>
#include <vector>

#include "libabbauw/sortorder.h"

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
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

		class IBatchOrderEditionVC
		{
		public:
			virtual ~IBatchOrderEditionVC() {};
			typedef std::vector<basar::db::aspect::AccessorPropertyTable_YIterator> YIterators;

			virtual void														     init(basar::db::aspect::AccessorPropertyTableRef, bool) = 0;
			virtual basar::gui::tie::WidgetReturnEnum							     show() = 0;
			virtual void														     waitForEvents() = 0;
			virtual void														     info(const basar::I18nString&) = 0;
			virtual void														     error(const basar::I18nString&) = 0;
			virtual const basar::gui::tie::MessageBoxButtonEnum					     question(const basar::I18nString&) = 0;
			virtual void														     showOrderChangedNotification() = 0;

			virtual void														     setNumberOfGreenOrders(basar::Int32) = 0;
			virtual void														     setTotalNumberOfOrders(basar::Int32) = 0;

			virtual void														     matchToSearchCriteria(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
			virtual void															 matchFromTransferOrders() = 0;

			virtual basar::db::aspect::AccessorPropertyTable_YIterator			     getSelectedOrder() = 0;
			virtual YIterators													     getSelectedOrders() = 0;

			virtual bool														     isOrderViewActive() = 0;
		};

	} // viewConn
} // CSC_Batches

#endif
