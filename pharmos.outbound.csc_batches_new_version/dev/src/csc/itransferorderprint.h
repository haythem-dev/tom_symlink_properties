#ifndef GUARD_PRINT_ITRANSFERORDERPRINT_H
#define GUARD_PRINT_ITRANSFERORDERPRINT_H

#include "libabbauw/itransferordergetterptr.h"
#include "libabbauw/icustomergetterptr.h"
#include <printlayoutbase.h>
#include <libbasar_definitions.h>

//-------------------------------------------------------------------------------------------------//
// interface declaration section
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
namespace print
{

class ITransferOrderPrint : public ::print::PrintLayoutBase
{
public:
	virtual ~ITransferOrderPrint() {}

	// data which will be printed
    virtual void setOrder(    libabbauw::domMod::transferOrder::ITransferOrderGetterPtr ) = 0;
    virtual void setCustomer( libabbauw::domMod::customer::ICustomerGetterPtr )           = 0;
};

} // end namespace print
} // end CSC_Batches
#endif // GUARD_PRINT_ITRANSFERORDERPRINT_H
