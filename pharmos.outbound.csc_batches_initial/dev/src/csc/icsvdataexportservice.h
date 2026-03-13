#ifndef GUARD_ICSVDATAEXPORTSERVICE_H
#define GUARD_ICSVDATAEXPORTSERVICE_H

#include <libbasardbaspect_accessorpropertytable.h>

namespace CSC_Batches
{
    namespace services
    {
        class ICSVDataExportService
        {
            public:

                virtual ~ICSVDataExportService() {};

                virtual void exportData( basar::db::aspect::AccessorPropertyTableRef data ) = 0;
        };
    }
}


#endif
