#ifndef GUARD_ICSVDATAEXPORTSERVICEPTR_H
#define GUARD_ICSVDATAEXPORTSERVICEPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace services
    {
        class ICSVDataExportService;

        typedef boost::shared_ptr<ICSVDataExportService> ICSVDataExportServicePtr;
    }
}


#endif
