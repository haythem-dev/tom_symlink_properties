#include "printprocessor.h"
#include "iprintlayout.h"
#include "printdataprovider.h"
#include "filenotopenableexception.h"
#include "filenotwritableexception.h"

#include <libutil/misc/metaclassmacro_definitions.h>
#include <libbasarcmnutil_print.h>
#include <libbasarguiqt_printpreview.h>
#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"
#pragma warning (push)
#pragma warning(disable: 4251)
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtPrintSupport/QPrinterInfo>
#pragma warning (pop)

#include <QtGui/QDesktopServices>

namespace print
{

PrintProcessor::PrintProcessor()
{
}

void PrintProcessor::print( ::print::IPrintLayoutPtr printLayout )
{
	QPrinter printer( QPrinterInfo::defaultPrinter() );
	basar::cmnutil::InterpretPrint print;
    print.setPrintSelect( basar::cmnutil::InterpretPrint::PRINTER_STANDARD, printer.printerName().toLocal8Bit().constData() );

    printLayout->preparePrintData();

    PrintDataProvider dataProvider( printLayout->getPrintBuffer() );
    dataProvider.setFormFileName( printLayout->getFormFileName() );
    dataProvider.setFormName( printLayout->getFormName() );
	print.print( dataProvider );
}

void PrintProcessor::preview( ::print::IPrintLayoutPtr printLayout )
{
    printLayout->preparePrintData();

    PrintDataProvider dataProvider( printLayout->getPrintBuffer() );
    dataProvider.setFormFileName( printLayout->getFormFileName() );
    dataProvider.setFormName( printLayout->getFormName() );
	basar::gui::qt::PrintPreview preview( dataProvider );

	preview.setOutputFormats( static_cast<basar::gui::qt::OutputEnum>( basar::gui::qt::PDF | basar::gui::qt::WIN_PRINT ) );
	preview.show();
}

void PrintProcessor::previewPDF( ::print::IPrintLayoutPtr printLayout )
{
	METHODNAME_DEF( PrintProcessor, previewPDF );

    QString s1 = QDir::tempPath();
    basar::VarString s2 = s1.toStdString();
    BLOG_INFO(  libabbauw::LoggerPool::loggerUseCases, s2 );


	QUrl tmpPDF = QUrl::fromLocalFile( QDir::tempPath() + QStringLiteral( "/print-preview.pdf" ) );
	basar::VarString fileName = QDir::toNativeSeparators( tmpPDF.toLocalFile() ).toLatin1().constData();

	// check if file is still opened from previous call (happens if user did not close the PDF reader)
	if( fileIsOpen( fileName ) )
	{
		basar::VarString msg = fileName + " still opened!";
		throw exceptions::FileNotWritableException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), fileName );
	}

    printLayout->preparePrintData();

    PrintDataProvider dataProvider( printLayout->getPrintBuffer() );
    dataProvider.setFormFileName( printLayout->getFormFileName() );
    dataProvider.setFormName( printLayout->getFormName() );

	basar::cmnutil::InterpretPrint print;
	print.savePdf( dataProvider, fileName );

	// try to open PDF with default viewer
	if( ! QDesktopServices::openUrl( tmpPDF ) )
	{
		basar::VarString msg = fileName + " could not be opened!";
		throw exceptions::FileNotOpenableException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ), fileName );
	}
}

bool PrintProcessor::fileIsOpen( const basar::VarString& file )
{
	QFile testFile( file.c_str() );
	return ! testFile.open( QIODevice::WriteOnly );
}

} // end namespace print
