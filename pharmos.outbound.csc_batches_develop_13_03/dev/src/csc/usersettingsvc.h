#ifndef GUARD_CSC_BATCHES_VIEWCONN_USERSETTINGSVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_USERSETTINGSVC_H

#include "iusersettingsvc.h"
#include "libabbauw/iparametereditptr.h"
#include "iuserrightsptr.h"
#include "libabbauw/sortorder.h"

#include <QtWidgets/QDialog>
#include <libbasarguitie_iviewconnector.h>

#pragma warning (push)
#pragma warning(disable: 4481)
#include "ui_usersettingsvc.h"
#pragma warning (pop)

#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libutil/flag.h>

namespace CSC_Batches
{
namespace viewConn
{

class UserSettingsVC : public QDialog, public IUserSettingsVC, public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	UserSettingsVC( QWidget *parent = 0 );
	virtual ~UserSettingsVC();

    void                                                                injectParameter( libabbauw::domMod::parameter::IParameterEditPtr );
    void                                                                injectUserRights( infrastructure::rights::IUserRightsPtr );

    // from IUserSettingsVC interface
	virtual basar::gui::tie::WidgetReturnEnum	                        show();
	virtual void								                        hide(){}
	virtual void                                                        waitForEvents();
	virtual bool								                        shutdown();

    virtual void                                                        info( const basar::I18nString& );
    virtual void                                                        error( const basar::I18nString& );
    virtual const basar::gui::tie::MessageBoxButtonEnum                 question( const basar::I18nString& );

	void                                                                matchFrom();
	void                                                                matchTo();

private:
    void                                                                setupPermissions();
	void                                                                wireEvents();
	void                                                                labelWindowTitle();
	void                                                                setupList();
	void                                                                setupDateWidgets();
	void                                                                setupLineEdits();
	QIcon                                                               drawSortIcon( util::SortOrder::Order );
	void                                                                matchFromOrderSort();

private slots:
	void                                                                onSaveCancelBtnClicked( QAbstractButton* );
	void                                                                onBtnMoveRightClicked();
	void                                                                onBtnMoveLeftClicked();
	void                                                                onBtnMoveUpClicked();
	void                                                                onBtnMoveDownClicked();
	void                                                                onRadioBtnOrderClicked();
	void                                                                onLstItemSelectionChanged();

private:
	typedef std::map< QString, basar::property::PropertyDescription >   StringToProperties;

private:
	Ui::UserSettingsVCClass                                             ui;
	log4cplus::Logger                                                   m_Logger;
    libutil::misc::Flag                                                 m_EventsWiring;
	libabbauw::domMod::parameter::IParameterEditPtr                     m_ParameterEditPtr;
    infrastructure::rights::IUserRightsPtr                              m_UserRights;
	util::SortOrder                                                     m_OrderSort;
	StringToProperties                                                  m_StringToProperty;
};

} // end namespace viewConn
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_USERSETTINGSVC_H
