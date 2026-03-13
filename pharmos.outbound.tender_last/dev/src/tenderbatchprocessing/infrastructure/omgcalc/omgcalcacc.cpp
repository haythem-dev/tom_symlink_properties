#include "omgcalcacc.h"
#include "omgcalcacc_definitions.h"
#include <tenderbatchprocessing_property_definitions.h>
#include <loggerpool/loggerpool.h>

namespace tenderbatchprocessing
{
namespace infrastructure 
{
namespace accessor 
{
namespace omgcalc 
{
using namespace basar;
using namespace properties;

BEGIN_ACCESSOR_DEFINITION( lit::OMGCALC_ACC )

    BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( BRANCH_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( TENDER_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( ORDER_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( ORDER_DATE );
		PROPERTY_DESCRIPTION_LIST_ADD( CUSTOMER_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( ARTICLE_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( INVOICE_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( INVOICE_DATE );
		PROPERTY_DESCRIPTION_LIST_ADD( INVOICE_TIME );
		PROPERTY_DESCRIPTION_LIST_ADD( DELIVERY_QTY );
		PROPERTY_DESCRIPTION_LIST_ADD( BASE_PRICE );
		PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNT_PCT );
		PROPERTY_DESCRIPTION_LIST_ADD( TRANSFERRED_FLAG );
		PROPERTY_DESCRIPTION_LIST_ADD( PHARMACY_GROUPID );
		PROPERTY_DESCRIPTION_LIST_ADD( SUPPLIER_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( ARTICLE_NAME );
		PROPERTY_DESCRIPTION_LIST_ADD( UNIT );
		PROPERTY_DESCRIPTION_LIST_ADD( DOSAGE );
    END_PROPERTY_DESCRIPTION_LIST

    ACCESS_METHOD( lit::SELECT_OMG_CALC_FOR_TRANSFER );
		SQL_BUILDER_CREATE   ( SelectOMGCalcForTransfer )
		SQL_BUILDER_PUSH_BACK( SelectOMGCalcForTransfer )

    ACCESS_METHOD( lit::SELECT_OMG_CALC_BY_ID );
    SQL_BUILDER_CREATE   ( SelectOMGCalcById )
    SQL_BUILDER_PUSH_BACK( SelectOMGCalcById )

    ACCESS_METHOD( lit::SELECT_OMG_CALC_BY_PATTERN );
    SQL_BUILDER_CREATE   ( SelectOMGCalcByPattern )
    SQL_BUILDER_PUSH_BACK( SelectOMGCalcByPattern )

    ACCESS_METHOD( lit::SAVE_OMG_CALC )
    SQL_BUILDER_CREATE( UpdateOMGCalc )
    SQL_BUILDER_PUSH_BACK( UpdateOMGCalc )
    SQL_BUILDER_CREATE( InsertOMGCalc )
    SQL_BUILDER_PUSH_BACK( InsertOMGCalc )


END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelectOMGCalcForTransfer )

bool SelectOMGCalcForTransfer::isExecutable() const
{
    return true;
}

void SelectOMGCalcForTransfer::buildSQLString()
{
	// to catch hold of a little bit readability
	static const VarString & branchno =	BRANCH_NO.getName();
	static const VarString & tenderno =	TENDER_NO.getName();
	static const VarString & orderno = ORDER_NO.getName();
	static const VarString & orderdate = ORDER_DATE.getName();
	static const VarString & customerno = CUSTOMER_NO.getName();
	static const VarString & articleno = ARTICLE_NO.getName();
	static const VarString & invoiceno = INVOICE_NO.getName();
	static const VarString & invoicedate = INVOICE_DATE.getName();
	static const VarString & invoicetime = INVOICE_TIME.getName();
	static const VarString & deliveryqty = DELIVERY_QTY.getName();
	static const VarString & baseprice = BASE_PRICE.getName();
	static const VarString & discountpct = DISCOUNT_PCT.getName();
	static const VarString & transferredflag = TRANSFERRED_FLAG.getName();
	static const VarString & pharmacygroupid = PHARMACY_GROUPID.getName();
	static const VarString & supplierno = SUPPLIER_NO.getName();
	static const VarString & articlename = ARTICLE_NAME.getName();
	static const VarString & unit = UNIT.getName();
	static const VarString & dosage = DOSAGE.getName();

	static const basar::VarString sql( 
		"SELECT first 4 "
		"tsc.branchno AS " + branchno + ", "
		"tsc.tenderno AS " + tenderno + ", "
		"tsc.orderno AS " + orderno + ", "
		"tsc.orderdate AS " + orderdate + ", "
		"tsc.customerno AS " + customerno + ", "
		"tsc.articleno AS " + articleno + ", "
		"tsc.invoiceno AS " + invoiceno + ", "
		"tsc.invoicedate AS " + invoicedate + ", "
		"tsc.invoicetime AS " + invoicetime + ", "
		"tsc.deliveryqty AS " + deliveryqty + ", "
		"tsc.baseprice AS " + baseprice + ", "
		"tsc.discountpct AS " + discountpct + ", "
		"tsc.transferredflag AS " + transferredflag + ", "
		"th.pharmacygroupid AS " + pharmacygroupid + ", "
		"az.hersteller_nr AS " + supplierno + ", "
		"az.artikel_name AS " + articlename + ", "
		"az.einheit AS " + unit + ", "
		"az.darreichform AS " + dosage + " "

		"FROM "
		"tendersuppliercalc tsc "
		"INNER JOIN tenderhead th ON (th.tenderno=tsc.tenderno) "
		"INNER JOIN artikelzentral az ON (az.artikel_nr=tsc.articleno) "

		"WHERE "
		"(tsc.transferredflag = 0) "
			);

	resolve( sql );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( SelectOMGCalcById )

bool SelectOMGCalcById::isExecutable() const
{
    basar::cmnutil::ParameterList searchValues;
    //searchValues.push_back( properties::KSC_FIL.getName() );
    return arePropertiesSet( searchValues );
}

void SelectOMGCalcById::buildSQLString()
{
    static const basar::VarString sql( "" );
    resolve( sql );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( SelectOMGCalcByPattern )

bool SelectOMGCalcByPattern::isExecutable() const
{
    basar::cmnutil::ParameterList searchValues;
    //searchValues.push_back( properties::KSC_FIL.getName() );
    return arePropertiesSet( searchValues );
}

void SelectOMGCalcByPattern::buildSQLString()
{
    static const basar::VarString sql( "" );
    resolve( sql );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( UpdateOMGCalc )
bool UpdateOMGCalc::isExecutable() const
{
    const bool executable = arePropertiesSet( "properties::BRANCH_NO.toPropertyString()" ) ;
    return executable;
}

void UpdateOMGCalc::buildSQLString()
{
	//TODO UPDATE set transferredflag=xyz WHERE ....


    //using namespace properties;
    static const basar::VarString constSQL( "UPDATE " );
    resolve( constSQL );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( InsertOMGCalc )
bool InsertOMGCalc::isExecutable() const
{
    const bool executable = arePropertiesSetAndAllInState( "properties::BRANCH_NO.toPropertyString()", basar::SS_INSERT ) ;
    return executable;
}

void InsertOMGCalc::buildSQLString()
{
    //using namespace properties;
    static const basar::VarString constSQL( "INSERT " );
    resolve( constSQL );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}


} //end namespace omgcalc
} //end namespace accessor
} //end namespace infrastructure
}
