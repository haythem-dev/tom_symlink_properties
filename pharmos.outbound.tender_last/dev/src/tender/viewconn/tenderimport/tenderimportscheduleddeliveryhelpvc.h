#ifndef GUARD_TENDER_VIEWCONN_TENDER_IMPORT_SCHEDULED_DELIVERY_HELP_VC_H
#define GUARD_TENDER_VIEWCONN_TENDER_IMPORT_SCHEDULED_DELIVERY_HELP_VC_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4481 4512 4800)
	#include <QDialog>
	#include "ui_tenderimportscheduleddeliveryhelpvc.h"
#pragma warning (pop)

#include <libbasarguitie_iviewconnector.h>
#include <libutil/viewconn.h>

namespace tender 
{
namespace viewConn
{
class TenderImportScheduledDeliveryHelpVC : public libutil::viewConn::BaseVC
{
	Q_OBJECT

public:
	TenderImportScheduledDeliveryHelpVC( QWidget * parent = 0 )
	: BaseVC( parent )
	{
		ui.setupUi( this );
		connect( ui.btnClose, SIGNAL( clicked() ),
		this,				  SLOT  ( reject() ) );
	}
	~TenderImportScheduledDeliveryHelpVC()
	{
	}

private:
	Ui::TenderImportScheduledDeliveryHelpVC ui;
};

} // end namespace viewConn
} // end namespace tender 

#endif // GUARD_TENDER_VIEWCONN_TENDER_IMPORT_SCHEDULED_DELIVERY_HELP_VC_H
