#ifndef GUARD_CSCBATCHES_PRINT_TRANSFERORDERPRINT_H
#define GUARD_CSCBATCHES_PRINT_TRANSFERORDERPRINT_H

#include "itransferorderprint.h"
#include "printlayoutbase.h"
#include "libabbauw/icustomergetterptr.h"

#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

#pragma warning (push)
#pragma warning(disable: 4512 )
#include <QtCore/QCoreApplication>
#pragma warning (pop)

namespace CSC_Batches
{
namespace print
{

class TransferOrderPrint : public ITransferOrderPrint
{
    Q_DECLARE_TR_FUNCTIONS( TransferOrderPrint )

public:
	TransferOrderPrint();
	~TransferOrderPrint();

    // from PrintLayoutBase
    basar::VarString getFormName()     const;
    basar::VarString getFormFileName() const;
    void preparePrintData();

	// from ITransferOrderPrint
	void setOrder(    libabbauw::domMod::transferOrder::ITransferOrderGetterPtr );
    void setCustomer( libabbauw::domMod::customer::ICustomerGetterPtr );

private:
	TransferOrderPrint ( const TransferOrderPrint& );
	TransferOrderPrint operator = ( const TransferOrderPrint& );

	basar::VarString dateToString( const basar::Date& ) const;
	
	void printHeadLine();
	void printOrderHeader();
	void printPositionsHeader();
	void printOrder();
	void printCustomer();
	void printPositions();

private:
    libabbauw::domMod::transferOrder::ITransferOrderGetterPtr m_Order;
    libabbauw::domMod::customer::ICustomerGetterPtr           m_Customer;
};

} // end namespace print
} // end CSC_Batches
#endif // GUARD_CSCBATCHES_PRINT_TRANSFERORDERPRINT_H
