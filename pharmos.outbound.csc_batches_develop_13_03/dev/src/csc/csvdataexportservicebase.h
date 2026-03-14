#ifndef GUARD_CSVDATAEXPORTSERVICEBASE_H
#define GUARD_CSVDATAEXPORTSERVICEBASE_H

#include "icsvdataexportservice.h"

#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>

#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

//forward
class QWidget;
class QStringList;

namespace log4cplus
{
    class Logger;
}

namespace CSC_Batches
{
    namespace services
    {
        class CSVDataExportServiceBase : public ICSVDataExportService
        {
            public:

                virtual ~CSVDataExportServiceBase();

                CSVDataExportServiceBase();

				void setBranch( basar::Int16 branchNo );

                void setParentWidget( QWidget* parent );

                void setBaseExportLocation( const basar::VarString& baseExportLocation );

                void setSeparator( char separator );

                virtual void exportData( basar::db::aspect::AccessorPropertyTableRef data );

            protected:

                virtual basar::VarString serializeData( basar::db::aspect::AccessorPropertyTable_YIterator dataYit ) = 0;

                virtual basar::VarString getExportTitle() = 0;

                virtual basar::VarString getTitleLine() = 0;

                char getSeparator() const;

            private:
                //functions
                CSVDataExportServiceBase( const CSVDataExportServiceBase& );
                CSVDataExportServiceBase& operator=( const CSVDataExportServiceBase& );

				basar::Int16 m_BranchNo;

                basar::VarString queryExportLocation();

                QStringList showExportLocationDialog( basar::VarString baseExportLocation );

                //vars
                const log4cplus::Logger& m_Logger;

                QWidget* m_ParentWidget;

                basar::VarString m_BaseExportLocation;

                char m_Separator;
        };
    }
}

#endif
