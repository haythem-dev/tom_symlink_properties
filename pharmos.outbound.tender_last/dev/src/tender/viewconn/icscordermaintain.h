#ifndef GUARD_TENDER_VIEWCONN_ICSCORDERMAINTAIN_H
#define GUARD_TENDER_VIEWCONN_ICSCORDERMAINTAIN_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
	#include <QtWidgets/QGroupBox>
#pragma warning (pop)

#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <libutil/flag.h>
#include <libutil/viewconn.h>

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

namespace tender
{
namespace viewConn
{
class ICSCOrderMaintain: public libutil::viewConn::BaseVC
{
public:
	ICSCOrderMaintain( QWidget * parent = 0 ) : BaseVC( parent ) {}
	virtual ~ICSCOrderMaintain() {};

	virtual basar::gui::tie::WidgetReturnEnum show() = 0;

	virtual void matchFromTender( libtender::domMod::tender::ITenderPtr tender ) = 0;
	virtual void matchFromOrderPositions( basar::db::aspect::AccessorPropertyTableRef positionPropTab ) = 0;
                
	virtual void displayQuantityExceedance( int maxQuantity ) = 0;
	virtual void displayPositionHasNoAvailableQuantity() = 0;
	virtual void displayCscOrderCreationDetails( const std::string orderInfo ) = 0;
                
	virtual basar::db::aspect::AccessorPropertyTable_YIterator getCurrentTenderPosition( basar::db::aspect::AccessorPropertyTableRef positionPropTab ) = 0;
	virtual basar::db::aspect::AccessorPropertyTable_YIterator getCurrentOrderPosition( basar::db::aspect::AccessorPropertyTableRef positionPropTab ) = 0;           
};

} // end namespace viewConn
} // end namespace tender

#endif //end GUARD_TENDER_VIEWCONN_ICSCORDERMAINTAIN_H
