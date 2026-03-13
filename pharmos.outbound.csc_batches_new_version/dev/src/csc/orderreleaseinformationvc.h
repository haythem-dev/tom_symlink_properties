#ifndef GUARD_CSCBATCHES_VIEWCONN_ORDERRELEASEINFORMATIONVC_H
#define GUARD_CSCBATCHES_VIEWCONN_ORDERRELEASEINFORMATIONVC_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4481 4512 4800 )
#include "ui_orderreleaseinformationvc.h"
#pragma warning (pop)

#include "iorderreleaseinformationvc.h"
#include <QtWidgets/QDialog>

namespace CSC_Batches
{
namespace viewConn
{

    class OrderReleaseInformationVC : public QDialog, public IOrderReleaseInformationVC
    {
        Q_OBJECT

        public:
            OrderReleaseInformationVC( QWidget *parent = NULL );
            ~OrderReleaseInformationVC();

            // from IOrderReleaseInformationVC interface
            void                                                        setNumberOfOrders( basar::Int32 );
            basar::gui::tie::WidgetReturnEnum                           show();
            void                                                        waitForEvents();
            bool								                        shutdown();

            void                                                        printError( basar::Int32 orderNo, const basar::VarString& msg );
            void                                                        printOrderLockedMsg( basar::Int32 orderNo );

            void                                                        update();

        private:
            void                                                        wireEvents();
            void                                                        clear();
            void                                                        finish();
            void                                                        closeEvent( QCloseEvent* );

        private:
            Ui::OrderReleaseInformationVCClass                          ui;
    };

} // end namespace viewConn
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_VIEWCONN_ORDERRELEASEINFORMATIONVC_H
