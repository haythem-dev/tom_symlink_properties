#ifndef GUARD_MAINVC_H
#define GUARD_MAINVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_mainwindow.h"
#pragma warning (pop)

#include <libbasardbaspect.h>
#include <libbasarguiqt.h>

#include <boost/enable_shared_from_this.hpp>

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QObject>
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace log4cplus
{
    class Logger;
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
        /*! \brief main window class

        \n This is a final class.
        \n no-throw */
        class MainVC : public QMainWindow, public boost::enable_shared_from_this < MainVC >
        {
            Q_OBJECT

        public:
            MainVC( const basar::I18nString& dbServerName, const basar::I18nString& dbName,
                    const basar::I18nString& userName, const basar::I18nString& dokuLink, QWidget* parent = 0 );
            ~MainVC();

            void                                show();
            void                                hide();
			void								shutdown();

        private slots:
            void                                createMenus();
            void                                configAboutMenu( const basar::I18nString& dbServerName, const basar::I18nString& dbName, const basar::I18nString& dokuLink );
            void                                configStatusLine( const basar::I18nString& dbServerName, const basar::I18nString& dbName, const basar::I18nString& userName );
            void                                configMainLayout();

            void                                about();

            void                                onCreateNewTender();
            void                                onSearchTender();
            void                                onImportTender();
			void								onImportDeliveries();
            void                                onCreateTO(); // TODO remove later
			void								onDeveloperInfo();

        private:
            Ui::mainwindow                      ui;

            const log4cplus::Logger	            m_Logger;
            basar::gui::qt::StatusLine          m_Statusline;
            basar::gui::qt::MainBackground      m_Background;
            QList<QAction*>                     m_ActionList;
			basar::Int16						m_region;
        };
        //----------------------------------------------------------------------------
    } // viewConn
} // tender
#endif // GUARD_MAINVC_H
