#include "orderreleaseinformationvc.h"

#include <libutil/viewconn.h>

#pragma warning (push)
#pragma warning(disable: 4244 4800 )
#include <QtWidgets/QScrollBar>
#pragma warning (pop)

namespace CSC_Batches
{
namespace viewConn
{

OrderReleaseInformationVC::OrderReleaseInformationVC( QWidget *parent )
    : QDialog( parent )
{
    ui.setupUi( this );
    wireEvents();

    QWidget::setWindowModality( Qt::ApplicationModal );
}

OrderReleaseInformationVC::~OrderReleaseInformationVC()
{
}

void OrderReleaseInformationVC::wireEvents()
{
    connect( ui.btnClose,                          SIGNAL( clicked() ),
             this,                                 SLOT  ( close()   ) );
}

void OrderReleaseInformationVC::closeEvent( QCloseEvent* )
{
}

void OrderReleaseInformationVC::setNumberOfOrders( basar::Int32 num )
{
    ui.pgsOrderRelease->setMaximum( num );
}

basar::gui::tie::WidgetReturnEnum OrderReleaseInformationVC::show()
{
    clear();

    QDialog::show();
    QApplication::processEvents();

    return basar::gui::tie::WidgetReturnEnum();
}

void OrderReleaseInformationVC::waitForEvents()
{
    setAttribute( Qt::WA_Moved, false );

    finish();
    exec();
}

bool OrderReleaseInformationVC::shutdown()
{
    return close();
}

void OrderReleaseInformationVC::clear()
{
    ui.btnClose->setEnabled( false );
    ui.pgsOrderRelease->setValue( 0 );
    ui.txtLog->clear();
}

void OrderReleaseInformationVC::finish()
{
    ui.btnClose->setEnabled( true );

    QString text = tr("Order releasing finished.");

    ui.txtLog->insertPlainText( "---------------------------------------\n" );
    ui.txtLog->insertPlainText( text );
    
    QScrollBar *sb = ui.txtLog->verticalScrollBar();
    sb->setValue( sb->maximum() ); // scroll down
}

void OrderReleaseInformationVC::update()
{
    ui.pgsOrderRelease->setValue( ui.pgsOrderRelease->value() + 1 );
}

void OrderReleaseInformationVC::printError( basar::Int32 orderNo, const basar::VarString& msg )
{
    QColor color = ui.txtLog->textColor(); // store original text color
    ui.txtLog->setTextColor( Qt::red );

    ui.txtLog->insertPlainText( tr("Order %1: ").arg( orderNo ) );
    ui.txtLog->insertPlainText( msg.c_str() );
    ui.txtLog->insertPlainText( "\n" );

    ui.txtLog->setTextColor( color ); // restore color

    QScrollBar *sb = ui.txtLog->verticalScrollBar();
    sb->setValue( sb->maximum() ); // scroll down
}

void OrderReleaseInformationVC::printOrderLockedMsg( basar::Int32 orderNo )
{
    basar::VarString msg = tr("Order is locked!").toLocal8Bit().constData();
    printError( orderNo, msg );
}

} // end namespace viewConn
} // end namespace CSC_Batches
