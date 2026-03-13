#include "transferorderprint.h"

#include "libabbauw/itransferordergetter.h"
#include "libabbauw/itransferorderhead.h"
#include "libabbauw/itransferorderposcollection.h"

#include "libabbauw/icustomerfinder.h"
#include "libabbauw/libabbauw_properties_definitions.h"

#include <libbasarlogin_manager.h>
#include <libbasarcmnutil_datetime.h>

#pragma warning (push)
#pragma warning(disable: 4512)
#include <QtCore/QCoreApplication>
#pragma warning (pop)

#include <iomanip>

namespace CSC_Batches
{
namespace print
{

TransferOrderPrint::TransferOrderPrint()
{
}

TransferOrderPrint::~TransferOrderPrint()
{
}

void TransferOrderPrint::preparePrintData()
{
	clear(); // clear buffer

	printHeadLine();
	printOrderHeader();
	printOrder();
	printCustomer();
	printPositionsHeader();
	printPositions();

	finishPrint();
}

basar::VarString TransferOrderPrint::getFormName() const
{
	return "ABBAUW";
}

basar::VarString TransferOrderPrint::getFormFileName() const
{
	return "TransferOrders.ps";
}

void TransferOrderPrint::setOrder( libabbauw::domMod::transferOrder::ITransferOrderGetterPtr order )
{
	m_Order = order;
}

void TransferOrderPrint::setCustomer( libabbauw::domMod::customer::ICustomerGetterPtr customer )
{
	m_Customer = customer;
}

basar::VarString TransferOrderPrint::dateToString( const basar::Date& date ) const
{
	basar::VarString dateStr;
	if( date.isValid() )
		dateStr = date.toString( "%d.%m.%Y" );

	return dateStr;
}

void TransferOrderPrint::printHeadLine()
{
    basar::db::aspect::AccessorPropertyTable_YIterator orderYit = m_Order->getHead()->get();

	// Transfer Order
	write() << tr( "Transfer Order"       ).toLocal8Bit().constData() << endl();

	// Area: 12 Unterentfelden
	std::stringstream data;
	data   << tr( "Area:"                ).toLocal8Bit().constData()                            << " "
		   << orderYit.getInt16( libabbauw::properties::BRANCHNO )                              << " "
		   << basar::login::Manager::getInstance().getAreaName();

	// Area: 12 Unterentfelden                           05.11.2013 11:30
	write().fill( ' ' );
	write() << std::left  << std::setw( 50 ) << data.str()                                            
	        << std::right << basar::cmnutil::DateTime().getCurrent().toString( "%d.%m.%Y %R" ) << endl()
		    << std::flush;
}

void TransferOrderPrint::printOrderHeader()
{
	write() << tr( "Order No: "           ).toLocal8Bit().constData() << endl()
		    << tr( "Positions: "          ).toLocal8Bit().constData() << endl()
		    << tr( "Order Type: "         ).toLocal8Bit().constData() << endl()
		    << tr( "Order Value: "        ).toLocal8Bit().constData() << endl()
		    << tr( "Delivery Date: "      ).toLocal8Bit().constData() << endl()
		    << tr( "Valuta: "             ).toLocal8Bit().constData() << endl()
		    << tr( "ZD-LW: "              ).toLocal8Bit().constData() << endl()
		    << std::flush;
}

void TransferOrderPrint::printPositionsHeader()
{
    basar::db::aspect::AccessorPropertyTable_YIterator orderYit = m_Order->getHead()->get();

	write() << orderYit.getString( libabbauw::properties::HEADERTEXT ) << endl()
		    << tr( "PNo"                  ).toLocal8Bit().constData()  << endl()
		    << tr( "PZN"                  ).toLocal8Bit().constData()  << endl()
		    << tr( "OQTY"                 ).toLocal8Bit().constData()  << endl()
		    << tr( "DPCT"                 ).toLocal8Bit().constData()  << endl()
		    << tr( "ND"                   ).toLocal8Bit().constData()  << endl()
		    << tr( "Unit"                 ).toLocal8Bit().constData()  << endl()
		    << tr( "Item Name"            ).toLocal8Bit().constData()  << endl()
		    << tr( "PF"                   ).toLocal8Bit().constData()  << endl()
		    << tr( "Exp"                  ).toLocal8Bit().constData()  << endl()
		    << std::flush;
}

void TransferOrderPrint::printOrder()
{
    basar::db::aspect::AccessorPropertyTable_YIterator orderYit = m_Order->getHead()->get();

    const basar::Decimal orderValue = m_Order->getOrderValue();
    const basar::Int32   posCount   = m_Order->getPositionCount();
    basar::Decimal zdlw = orderValue / posCount;

	write() << orderYit.getInt32( libabbauw::properties::ORDERNO )                                                   << endl()
            << posCount                                                                                              << endl()
		    << orderYit.getString( libabbauw::properties::ORDERTYPE )                                                << endl()
            << orderValue.toFloat64()                                                                                << endl()
		    << dateToString( orderYit.getDate( libabbauw::properties::EDIDELIVERYDATE ) )                            << endl()
		    << dateToString( orderYit.getDate( libabbauw::properties::DEFERREDPAYMENTDATE ) )                        << endl()
		    << zdlw.toFloat64()                                                                                      << endl()
		    << std::flush;
}

void TransferOrderPrint::printCustomer()
{
    basar::db::aspect::AccessorPropertyTable_YIterator customer = m_Customer->get();
    basar::db::aspect::AccessorPropertyTable_YIterator orderYit = m_Order->getHead()->get();
    
    if( ! m_Customer->isEmpty() )
    {
        write() << customer.getString( libabbauw::properties::LOCATION )                                             << endl();
    }
    else
    {
        write() << tr( "Unknown" ).toLocal8Bit().constData()                                                         << endl();
    }
 
    if( m_Customer->isKnown() )
	{
		write() << customer.getString( libabbauw::properties::PHARMACYNAME )                                         << endl();
	}
	else // print customer ean if customer is unkown
	{
		write() << tr( "Customer-EAN:"    ).toLocal8Bit().constData()                                                << " "
		        << orderYit.getString( libabbauw::properties::PHARMACYNOMF )                                         << endl();
	}

    if( ! m_Customer->isEmpty() )
    {
        write() << customer.getString( libabbauw::properties::CUSTOMER_NAME )					                     << endl();
    }
    else
    {
        write() << tr( "Unknown" ).toLocal8Bit().constData()                                                         << endl();
    }

	write() << tr( "Pharmacy No"          ).toLocal8Bit().constData()                                                << " "
		    << orderYit.getInt32( libabbauw::properties::PHARMACYNO )                                                << endl();

	write() << tr( "Phone No"             ).toLocal8Bit().constData()                                                << " ";
    if( ! m_Customer->isEmpty() )
        write() << customer.getString( libabbauw::properties::PHONENO )                                              << endl();
    else
        write() << tr( "Unknown" ).toLocal8Bit().constData()                                                         << endl();

	write() << std::flush;
}

void TransferOrderPrint::printPositions()
{
    basar::db::aspect::AccessorPropertyTableRef orderPositions = m_Order->getPosCollection()->get();
	for( basar::db::aspect::AccessorPropertyTable_YIterator position = orderPositions.begin(); !position.isEnd(); ++position )
	{
		basar::VarString unit = position.getString( libabbauw::properties::UNIT );
		unit.trim( basar::VarString::FIRST_END );

		write() << position.getInt32( libabbauw::properties::POSNO )                                                 << sep() 
			    << position.getString( libabbauw::properties::PZN )                                                  << sep()
			    << position.getInt32( libabbauw::properties::ORDEREDQTY )                                            << sep()
			    << position.getDecimal( libabbauw::properties::DISCOUNTPCT ).toFloat64()                             << sep()
			    << position.getInt32( libabbauw::properties::NONCHARGEDQTY )                                         << sep()
			    << unit                                                                                              << sep()
			    << position.getString( libabbauw::properties::ARTICLENAME )                                          << sep()
			    << position.getString( libabbauw::properties::PHARMACEUTICALFORM )                                   << sep()
			    << position.getString( libabbauw::properties::EXPIRYDATE )                                           << sep()
			    << endl();
	}

	write() << std::flush;
}

} // end namespace print
} // end CSC_Batches
