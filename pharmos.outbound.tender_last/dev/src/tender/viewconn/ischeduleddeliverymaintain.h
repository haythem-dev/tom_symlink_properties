#ifndef GUARD_TENDER_VIEWCONN_ISCHEDULEDDELIVERYMAINTAIN_H
#define GUARD_TENDER_VIEWCONN_ISCHEDULEDDELIVERYMAINTAIN_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QtWidgets/QGroupBox>
#pragma warning (pop)

#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <libutil/flag.h>
#include <libutil/viewconn.h>
//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//

//class QFrame;

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
// class declaration setcion
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace viewConn
    {
        //-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
        class IScheduledDeliveryMaintain: public libutil::viewConn::BaseVC
        {
            public:
                IScheduledDeliveryMaintain(QWidget *parent = 0):BaseVC(parent){}
                virtual ~IScheduledDeliveryMaintain(){};
                virtual void setPOPFunctionality( basar::I18nString popFunctionality ) = 0;
				virtual void setNeedsCustomerSelection( bool needsCustomerSelection ) = 0;
                virtual void setRegion( basar::Int16 region ) = 0;
				virtual void clear() = 0;
                virtual void setCustomer( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

                virtual basar::gui::tie::WidgetReturnEnum show() = 0;

				virtual void matchFromScheduledDelivery	( libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery ) = 0;
				virtual void matchToScheduledDelivery	( libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery ) = 0;
				virtual void matchFromTender			( libtender::domMod::tender::ITenderPtr tender ) = 0;
				virtual void matchFromPositionCollection( basar::db::aspect::AccessorPropertyTableRef positionPropTab ) = 0;
				virtual void matchToPositionCollection  ( basar::db::aspect::AccessorPropertyTableRef positionPropTab ) = 0;

				virtual void displayDeliveryAlreadyExists() = 0;
				virtual void displayInvalidDeliveryDate() = 0;
				virtual void displayQuantityExceedance( int maxQuantity ) = 0;
				virtual void displayPositionHasNoAvailableQuantity() = 0;
				virtual void displayBranchNoNotValid() = 0;

				virtual basar::db::aspect::AccessorPropertyTable_YIterator getCurrentAvailableArticle( basar::db::aspect::AccessorPropertyTableRef positionPropTab ) = 0;
				virtual basar::db::aspect::AccessorPropertyTable_YIterator getCurrentSelectedArticle( basar::db::aspect::AccessorPropertyTableRef positionPropTab ) = 0;
           
				virtual basar::Int32 getCustomer() = 0;

        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_ISCHEDULEDDELIVERYMAINTAIN_H
