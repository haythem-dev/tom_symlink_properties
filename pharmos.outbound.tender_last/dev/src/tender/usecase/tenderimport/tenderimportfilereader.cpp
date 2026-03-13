#include "tenderimportfilereader.h"
#include "tenderimportparser.h"
#include <libtender/exception/fileopenexception.h>
#include <sstream>
#include "iprogress.h"
#include "ierrorhandler.h"

#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4251 4311 4312 4481 4512 4800)
	#include <QFile>
	#include <QTextStream>
	#include <QCoreApplication>
#pragma warning (pop)

namespace tender
{
namespace useCase
{
TenderImportFileReader::TenderImportFileReader()
{
}

TenderImportFileReader::~TenderImportFileReader()
{
}

void TenderImportFileReader::inject( IProgressPtr progress )
{
	m_Progress = progress;
}

void TenderImportFileReader::inject( IErrorHandlerPtr errorHandler )
{
	m_ErrorHandler = errorHandler;
}

void TenderImportFileReader::readTenderPositions( const basar::VarString & filename )
{
	m_TenderLineValueCollection.clear();
	QFile file( QString::fromLocal8Bit( filename.c_str() ) );
	if( false == file.open( QFile::ReadOnly | QFile::Text ) )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportFileReader", "Could not open file:" ).toLocal8Bit().constData();
		ss << " "<< filename.c_str();
		m_ErrorHandler->handleError( ss.str().c_str() );
		return;
	}

	basar::Int32 lineNumber = 1;
	basar::Int32 estimatedLineCount = 0;
	QTextStream textStream( &file );
	while( false == textStream.atEnd() )
	{
		QString line = textStream.readLine().trimmed();
		if( 0 < line.length() )
		{
			if( 0 == estimatedLineCount )
			{
				estimatedLineCount = static_cast<basar::Int32>( file.size() / line.length() );
				if( 0 == estimatedLineCount )
				{
					estimatedLineCount = 1;
				}
			}
			TenderLineValue lv;
			lv.lineNumber = lineNumber;
			lv.text = line.toLocal8Bit().constData();
			if( true == TenderImportParser::parse( lv.text, lv.positionData ) )
			{
				m_TenderLineValueCollection.push_back( lv );
			}
			else
			{
				std::stringstream ss;
				ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << lineNumber << ": ";
				ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Error reading line" ).toLocal8Bit().constData();
				m_ErrorHandler->handleError( ss.str().c_str() );
				m_TenderLineValueCollection.clear();
				return;
			}
		}
		else
		{
			std::stringstream ss;
			ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << lineNumber << ": ";
			ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Empty line ignored" ).toLocal8Bit().constData();
			m_ErrorHandler->handleWarning( ss.str().c_str() );
		}

		m_Progress->progress( lineNumber * 100 / estimatedLineCount );
		++lineNumber;
	}
}

basar::Int32 TenderImportFileReader::getTenderLineCount() const
{
	return static_cast<basar::Int32>( m_TenderLineValueCollection.size() );
}

libtender::domMod::tender::TenderImportStruct & TenderImportFileReader::getTenderPosition( const basar::Int32 index )
{
	return m_TenderLineValueCollection[ index ].positionData;
}

TenderImportFileReader::TenderLineValue & TenderImportFileReader::getTenderLineValue( const basar::Int32 index )
{
	return m_TenderLineValueCollection[ index ];
}

void TenderImportFileReader::readScheduledDeliveryPositions( const basar::VarString & filename )
{
	m_ScheduledDeliveryLineValueCollection.clear();
	QFile file( QString::fromLocal8Bit( filename.c_str() ) );
	if( false == file.open( QFile::ReadOnly | QFile::Text ) )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportFileReader", "Could not open file:" ).toLocal8Bit().constData();
		ss << " "<< filename.c_str();
		m_ErrorHandler->handleError( ss.str().c_str() );
		return;
	}

	basar::Int32 lineNumber = 1;
	basar::Int32 estimatedLineCount = 0;
	QTextStream textStream( &file );
	while( false == textStream.atEnd() )
	{
		QString line = textStream.readLine().trimmed();
		if( 0 < line.length() )
		{
			if( 0 == estimatedLineCount )
			{
				estimatedLineCount = static_cast<basar::Int32>( file.size() / line.length() );
				if( 0 == estimatedLineCount )
				{
					estimatedLineCount = 1;
				}
			}
			ScheduledDeliveryLineValue lv;
			lv.lineNumber = lineNumber;
			lv.text = line.toLocal8Bit().constData();
			if( true == TenderImportParser::parse( lv.text, lv.positionData ) )
			{
				m_ScheduledDeliveryLineValueCollection.push_back( lv );
			}
			else
			{
				std::stringstream ss;
				ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << lineNumber << ": ";
				ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Error reading line" ).toLocal8Bit().constData();
				m_ErrorHandler->handleError( ss.str().c_str() );
				m_ScheduledDeliveryLineValueCollection.clear();
				return;
			}
		}
		else
		{
			std::stringstream ss;
			ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << lineNumber << ": ";
			ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Empty line ignored" ).toLocal8Bit().constData();
			m_ErrorHandler->handleWarning( ss.str().c_str() );
		}

		m_Progress->progress( lineNumber * 100 / estimatedLineCount );
		++lineNumber;
	}
}

basar::Int32 TenderImportFileReader::getScheduledDeliveryLineCount() const
{
	return static_cast<basar::Int32>( m_ScheduledDeliveryLineValueCollection.size() );
}

libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct & TenderImportFileReader::getScheduledDeliveryPosition( const basar::Int32 index )
{
	return m_ScheduledDeliveryLineValueCollection[ index ].positionData;
}

TenderImportFileReader::ScheduledDeliveryLineValue & TenderImportFileReader::getScheduledDeliveryLineValue( const basar::Int32 index )
{
	return m_ScheduledDeliveryLineValueCollection[ index ];
}

} // namespace useCase
} // namespace tender
