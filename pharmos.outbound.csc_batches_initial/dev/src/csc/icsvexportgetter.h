#ifndef GUARD_CSCBATCHES_COMPONENTMANAGER_ICSVEXPORTGETTER_H
#define GUARD_CSCBATCHES_COMPONENTMANAGER_ICSVEXPORTGETTER_H

#include "icsvdataexportserviceptr.h"

#include <vector>
#include <libbasar_definitions.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
		}
	}
}

class QWidget;

namespace CSC_Batches
{
namespace componentManager
{

    class ICSVExportGetter
    {
    public:
        virtual ~ICSVExportGetter() {}

        virtual services::ICSVDataExportServicePtr getBVOStatusExportService(QWidget *widget) = 0; 
    };

} // end namespace componentManager
} // end CSC_Batches

#endif // GUARD_CSCBATCHES_COMPONENTMANAGER_ICSVEXPORTGETTER_H
