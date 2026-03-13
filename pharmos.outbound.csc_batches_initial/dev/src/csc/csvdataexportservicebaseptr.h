#ifndef GUARD_CSVDATAEXPORTSERVICEBASEPTR_H
#define GUARD_CSVDATAEXPORTSERVICEBASEPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace services
    {
        class CSVDataExportServiceBase;

        typedef boost::shared_ptr<CSVDataExportServiceBase> CSVDataExportServiceBasePtr;
    }
}

#endif
