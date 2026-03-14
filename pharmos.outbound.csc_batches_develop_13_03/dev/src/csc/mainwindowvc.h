#ifndef GUARD_CSC_BATCHES_MAINWINDOWVC_H
#define GUARD_CSC_BATCHES_MAINWINDOWVC_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include <libbasardbaspect_connectionref.h>

#include <libutil/session.h>
#include <boost/enable_shared_from_this.hpp>
#include "iuserrightsptr.h"

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4512 4800)
#include <libbasarguiqt.h>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QAction>
#pragma warning (pop)

namespace CSC_Batches
{
namespace viewConn
{

    class MainWindowVC: public QMainWindow,
                        public boost::enable_shared_from_this < MainWindowVC >
    {
      Q_OBJECT

    public:
          MainWindowVC( const libutil::infrastructure::session::ISessionPtr, const basar::I18nString& dbServerName, 
                        const basar::I18nString& dbName, infrastructure::rights::IUserRightsPtr userRights, QWidget* parent = 0 );
          ~MainWindowVC();

         void show();
         void hide();

          // notification to notify user that invalid parameters are set to default
          void showSettingSetToDefaultNotification();

          void showMsg( const basar::VarString& msg );

    private slots:
        void                                onAboutDlg();
        void                                onBatchOrderMaintenanceStart();
        void                                onCreateTransferOrdersStart();
        void                                onBatchOrderEditionStart();
        void                                onSettingsStart();
        void                                onDeveloperInfo();

        private:
        void                                init( const libutil::infrastructure::session::ISessionPtr );
        void                                createMenus();
    
        void                                labelWindowTitle( const libutil::infrastructure::session::ISessionPtr );
        void                                configStatusLine( const basar::I18nString& dbServerName, const basar::I18nString& dbName, 
                                                              const libutil::infrastructure::session::ISessionPtr );
        void                                configMainLayout( const basar::I18nString& dbServerName, const basar::I18nString& dbName );

    private:
        QList<QAction*>                     m_ActionList;

	    basar::gui::qt::StatusLine          m_Statusline;     //!< standard status line
	    basar::gui::qt::MainBackground      m_Background; //!< standard background
        infrastructure::rights::IUserRightsPtr m_UserRights;

    };

} // end namespace viewConn
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_MAINWINDOWVC_H
