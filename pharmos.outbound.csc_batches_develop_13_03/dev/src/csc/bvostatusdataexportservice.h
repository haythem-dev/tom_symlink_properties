#ifndef GUARD_USERDEFAULTSDATAEXPORTSERVICE_H
#define GUARD_USERDEFAULTSDATAEXPORTSERVICE_H

#include "csvdataexportservicebase.h"

#include <libbasar_definitions.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

namespace CSC_Batches
{
    namespace services
    {
        class BVOStatusDataExportSerivce : public CSVDataExportServiceBase
        {
            public:

                virtual ~BVOStatusDataExportSerivce();

                BVOStatusDataExportSerivce();

            protected:

                virtual basar::VarString serializeData( basar::db::aspect::AccessorPropertyTable_YIterator dataYit );

                virtual basar::VarString getExportTitle();

                virtual basar::VarString getTitleLine();

        };
    }
}

#endif
