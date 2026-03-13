#include "tenderimportparser.h"

namespace tender
{
namespace useCase
{
bool TenderImportParser::parse( 
	const basar::I18nString & line,
	libtender::domMod::tender::TenderImportStruct & importStruct
		)
{
	basar::cmnutil::CollBString tokens;
	line.tokenizeAndTrim( tokens, ";", basar::cmnutil::BString::FIRST_END, true, false );
	if( 15 != tokens.size() )
	{
		return false;
	}

	try
	{
		importStruct.id =									  tokens[  0 ].stoi();
		importStruct.branchno = static_cast<basar::Int16>(	  tokens[  1 ].stoi() );
		importStruct.contractno =							  tokens[  2 ];
		importStruct.tendertype =							  tokens[  3 ].stoi();
		importStruct.customerno =							  tokens[  4 ].stoi();
		importStruct.pharmacychainno =						  tokens[  5 ].stoi();
		importStruct.customergroupid =						  tokens[  6 ];	
		importStruct.partnerno =							  tokens[  7 ].stoi();
		importStruct.datefrom.fromString( "YYYY-MM-DD",		  tokens[  8 ] );
		importStruct.dateto.fromString( "YYYY-MM-DD",		  tokens[  9 ] );
		importStruct.tenderstate = static_cast<basar::Int16>( tokens[ 10 ].stoi() );
		importStruct.articlecode =							  tokens[ 11 ];
		importStruct.contractqty =							  tokens[ 12 ].stoi();
		importStruct.contractprice.fromString(				  tokens[ 13 ] );
		importStruct.adddiscountpct.fromString(				  tokens[ 14 ] );

		importStruct.discountapplyto  = 1; // set to default, e.g. 1 : AGP
		importStruct.discountcalcfrom = 0; // set to default, e.g. 0 : GEP
	}
	catch( ... )
	{
		return false;
	}

	return true;
}

bool TenderImportParser::parse( 
	const basar::I18nString & line,
	libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct & importStruct
		)
{
	basar::cmnutil::CollBString tokens;
	line.tokenizeAndTrim( tokens, ";", basar::cmnutil::BString::FIRST_END, true, false );
	if( 8 != tokens.size() )
	{
		return false;
	}

	try
	{
		importStruct.id =														tokens[ 0 ].stoi();
		importStruct.branchno = static_cast<basar::Int16>(						tokens[ 1 ].stoi() );
		importStruct.customerno = static_cast<basar::Int32>(					tokens[ 2 ].stoi() );
		importStruct.deliverydate.fromString( "YYYY-MM-DD",						tokens[ 3 ] );
		importStruct.articlecode =												tokens[ 4 ];
		importStruct.quantity =													tokens[ 5 ].stoi();
		importStruct.purchaseorderproposalflag = static_cast<basar::Int16>(		tokens[ 6 ].stoi() );
		importStruct.branchnopop = static_cast<basar::Int16>(					tokens[ 7 ].stoi() );
	}
	catch( ... )
	{
		return false;
	}

	return true;
}

} // namespace useCase
} // namespace tender
