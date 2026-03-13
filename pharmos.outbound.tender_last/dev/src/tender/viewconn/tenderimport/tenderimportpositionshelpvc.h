#ifndef GUARD_TENDER_VIEWCONN_TENDER_IMPORT_POSITIONS_HELP_VC_H
#define GUARD_TENDER_VIEWCONN_TENDER_IMPORT_POSITIONS_HELP_VC_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4481 4512 4800)
	#include <QDialog>
	#include "ui_tenderimportpositionshelpvc.h"
#pragma warning (pop)

#include <libbasarguitie_iviewconnector.h>
#include <libutil/viewconn.h>

namespace tender 
{
namespace viewConn
{
class TenderImportPositionsHelpVC : public libutil::viewConn::BaseVC
{
	Q_OBJECT

public:
	TenderImportPositionsHelpVC( QWidget * parent = 0 )
	: BaseVC( parent )
	{
		ui.setupUi( this );
		connect( ui.btnClose, SIGNAL( clicked() ),
		this,				  SLOT  ( reject() ) );
	}
	~TenderImportPositionsHelpVC()
	{
	}

private:
	Ui::TenderImportPositionsHelpVC ui;
};

} // end namespace viewConn
} // end namespace tender 

#endif // GUARD_TENDER_VIEWCONN_TENDER_IMPORT_POSITIONS_HELP_VC_H
