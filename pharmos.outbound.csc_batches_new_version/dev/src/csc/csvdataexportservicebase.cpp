#include "csvdataexportservicebase.h"

#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libbasarcmnutil_logging.h>

#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>
#include <libbasarcmnutil_date.h>

#pragma warning( push )
#pragma warning( disable: 4512 )
#include <QtCore/QStringList>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#pragma warning( pop )

#include <boost/filesystem.hpp>

#include <fstream>

namespace CSC_Batches
{
    namespace services
    {
        CSVDataExportServiceBase::~CSVDataExportServiceBase()
        {
        }

		CSVDataExportServiceBase::CSVDataExportServiceBase() : m_Logger( libabbauw::LoggerPool::loggerMain ), m_ParentWidget( nullptr ), m_Separator(';')
        {
        }

		void CSVDataExportServiceBase::setBranch( basar::Int16 branchNo )
		{
			m_BranchNo = branchNo;
		}

        void CSVDataExportServiceBase::setParentWidget( QWidget* parent )
        {
            m_ParentWidget = parent;
        }

        void CSVDataExportServiceBase::setBaseExportLocation( const basar::VarString& baseExportLocation )
        {
            m_BaseExportLocation = baseExportLocation;
        }

        void CSVDataExportServiceBase::setSeparator( char separator )
        {
            m_Separator = separator;
        }

        void CSVDataExportServiceBase::exportData( basar::db::aspect::AccessorPropertyTableRef data )
        {

            basar::VarString exportLocation = queryExportLocation();

            if ( false == exportLocation.empty() )
            {
                //open export location
                basar::db::aspect::AccessorPropertyTable_YIterator yit = data.begin();

                std::fstream fs;
                fs.open( exportLocation.c_str(), std::fstream::out | std::fstream::trunc );

                if ( false == fs.good() )
                {
                    //todo: log + exception
                }

                fs << getTitleLine();

                while ( yit != data.end() )
                {
                    basar::VarString dataLine = serializeData( yit );

                    fs << dataLine;

                    ++yit;
                }

                fs.flush();
                fs.close();
            }
        }

        char CSVDataExportServiceBase::getSeparator() const
        {
            return m_Separator;
        }

        basar::VarString CSVDataExportServiceBase::queryExportLocation()
        {
            bool accepted = false;

            bool abort = false;

            basar::VarString absoluteFilePath;

            basar::VarString exportLocation = m_BaseExportLocation;

            do
            {
                // Setting up Directory path
                QStringList fileNames = showExportLocationDialog( exportLocation );

                if ( false == fileNames.empty() )
                {
                    boost::filesystem::path p( fileNames.begin()->toLocal8Bit().constData() );

                    if ( ".csv" != p.extension() )
                    {
                        p += ".csv";
                    }

                    if ( boost::filesystem::exists( p ) )
                    {
                        //if file is existing, ask user if overwrite is in order

                        QMessageBox::StandardButton reply = QMessageBox::question( m_ParentWidget, 
							QObject::tr("Question").toLocal8Bit().data(), 
							QObject::tr("File with name already present. Overwrite?").toLocal8Bit().data(), 
							QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

                        switch ( reply )
                        {
                            case QMessageBox::Yes:
                            {
                                absoluteFilePath = p.make_preferred().string();
                                accepted = true;
                            }
                            break;

                            case QMessageBox::No:
                            {
                                exportLocation = boost::filesystem::basename( p );
                                continue;
                            }
                            break;

                            case QMessageBox::Cancel:
                            {
                                abort = true;
                            }
                            break;

                            default:
                                break;
                        }
                    }
                    else
                    {
                        absoluteFilePath = p.make_preferred().string();
                        accepted = true;
                    }
                }
                else
                {
                    abort = true;
                }
            }
            while ( false == accepted && false == abort );

            return absoluteFilePath;
        }

        QStringList CSVDataExportServiceBase::showExportLocationDialog( basar::VarString baseExportLocation )
        {
            QFileDialog dialog( m_ParentWidget );
            dialog.setDirectory( QString::fromLatin1( baseExportLocation.c_str() ) );

            basar::VarString fileName = getExportTitle();

			std::ostringstream branchNo;
			branchNo << m_BranchNo;

            fileName.append( "_" );
			fileName.append( branchNo.str() );
			fileName.append( "_" );
            fileName.append( basar::Date::getCurrent().toString( "yyyy_mm_dd" ) );

            QString s = QString::fromLatin1( fileName.c_str() );

            dialog.selectFile( s );
            dialog.setNameFilter( QString::fromLatin1( "Files ( *.csv )" ) );

            dialog.setFileMode( QFileDialog::AnyFile );
            dialog.setViewMode( QFileDialog::Detail  );

            if ( QDialog::Accepted == dialog.exec() )
            {
                return dialog.selectedFiles();
            }
            else
            {
                //todo: log - dialog exec failed, maybe user chose to close the dialog
                return QStringList();
            }

        }
    }
}
