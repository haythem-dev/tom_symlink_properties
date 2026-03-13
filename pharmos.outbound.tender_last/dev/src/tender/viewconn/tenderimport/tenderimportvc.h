#ifndef GUARD_TENDER_VIEWCONN_TENDER_IMPORT_VC_H
#define GUARD_TENDER_VIEWCONN_TENDER_IMPORT_VC_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4481 4512 4800)
	#include <QWidget>
	#include "ui_tenderimportvc.h"
	#include "tenderimportpositionshelpvc.h"
	#include "tenderimportscheduleddeliveryhelpvc.h"
#pragma warning (pop)

#include <libbasarguitie_iviewconnector.h>
#include <libbasardbaspect.h>
#include <libutil/viewconn.h>

namespace tender 
{
namespace viewConn
{
class TenderImportVC : public libutil::viewConn::BaseVC
{
	Q_OBJECT

public:
	TenderImportVC( QWidget * parent = 0 );
	~TenderImportVC();

	void setIconPath( const basar::I18nString & path );

	void matchToPositionsFilename( basar::VarString & filename ) const;
	void matchToDeliveriesFilename( basar::VarString & filename ) const;

	//virtual basar::gui::tie::WidgetReturnEnum show();
	//virtual void hide();

	/*virtual void init( const basar::VarString & filename );
	virtual bool shutdown();
*/

	void resetProgress();
	void setProgressDescription( const basar::I18nString & description );
	void progress( const basar::Int32 percent );

	void addMessage( const basar::I18nString & msg );
	void addInfoMessage( const basar::I18nString & msg );
	void addWarningMessage( const basar::I18nString & msg );
	void addErrorMessage( const basar::I18nString & msg );

private slots:
	void OnSelectPositionsFileClicked();
	void OnSelectDeliveriesFileClicked();
	void OnStartClicked();
	void OnCloseClicked();
	void OnPositionsHelpClicked();
	void OnScheduledDeliveryHelpClicked();

private:
	enum LogGroupEnum
	{
		LG_NONE = 0,
		LG_INFORMATION,
		LG_WARNING,
		LG_CRITICAL
	};

	void addLogMessage( const LogGroupEnum group, const QString & message );
	const QString getFilename( const QString & title );

	Ui::TenderImportVC						ui;
	QString									m_IconPath;
};

} // end namespace viewConn
} // end namespace tender 

#endif // GUARD_TENDER_VIEWCONN_TENDER_IMPORT_VC_H
