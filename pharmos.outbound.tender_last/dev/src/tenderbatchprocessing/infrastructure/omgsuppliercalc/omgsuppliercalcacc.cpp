#include "omgsuppliercalcacc.h"
#include "omgsuppliercalcacc_definitions.h"
#include <tenderbatchprocessing_property_definitions.h>
#include <loggerpool/loggerpool.h>

namespace tenderbatchprocessing
{
namespace infrastructure 
{
namespace accessor 
{
namespace omgsuppliercalc 
{
using namespace basar;
using namespace properties;

BEGIN_ACCESSOR_DEFINITION( lit::OMGSUPPLIERCALC_ACC )
    BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_NO  );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_DATE );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::INVOICE_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::INVOICE_DATE );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::INVOICE_TIME );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::DELIVERY_QTY );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::BASE_PRICE );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNT_PCT );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::SUPPLIER_NO );
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TRANSFERRED_FLAG );
    END_PROPERTY_DESCRIPTION_LIST

    ACCESS_METHOD( lit::SELECT_OMG_CALC_FOR_TRANSFER);
		SQL_BUILDER_CREATE( SelectOMGCalcForTransfer )
		SQL_BUILDER_PUSH_BACK( SelectOMGCalcForTransfer )

    ACCESS_METHOD( lit::SAVE_OMG_SUPPLIER_CALC )
		SQL_BUILDER_CREATE( InsertOMGSupplierCalc )
		SQL_BUILDER_PUSH_BACK( InsertOMGSupplierCalc )

	ACCESS_METHOD( lit::SAVE_OMG_SUPPLIER_CALC_TRANSFERRED )
		SQL_BUILDER_CREATE( UpdateOMGSupplierCalcTransferred )
		SQL_BUILDER_PUSH_BACK( UpdateOMGSupplierCalcTransferred )
END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelectOMGCalcForTransfer )

bool SelectOMGCalcForTransfer::isExecutable() const
{
	return true;
}

void SelectOMGCalcForTransfer::buildSQLString()
{
	// to catch hold of a little bit readability
	static const VarString & branchno = properties::BRANCH_NO.getName();
	static const VarString & tenderno = properties::TENDER_NO.getName();
	static const VarString & orderno = properties::ORDER_NO.getName();
	static const VarString & orderdate = properties::ORDER_DATE.getName();
	static const VarString & articleno = properties::ARTICLE_NO.getName();
	static const VarString & customerno = properties::CUSTOMER_NO.getName();
	static const VarString & invoiceno = properties::INVOICE_NO.getName();
	static const VarString & invoicedate = properties::INVOICE_DATE.getName();
	static const VarString & invoicetime = properties::INVOICE_TIME.getName();
	static const VarString & deliveryqty = properties::DELIVERY_QTY.getName();
	static const VarString & baseprice = properties::BASE_PRICE.getName();
	static const VarString & discountpct = properties::DISCOUNT_PCT.getName();
	static const VarString & transferredflag = properties::TRANSFERRED_FLAG.getName();
	static const VarString & supplierno = properties::SUPPLIER_NO.getName();

	static const basar::VarString constSQL(
		"SELECT "
		"ts.branchno AS " + branchno + ", "
		"ts.tenderno AS " + tenderno + ", "
		"ts.articleno AS " + articleno + ", "
		"ts.customerno AS " + customerno + ", "
		"SUM(ts.deliveryqty)::int AS " + deliveryqty + ", "
		"ts.baseprice AS " + baseprice + ", "
		"ts.discountpct AS " + discountpct + ", "
		"az.hersteller_nr AS " + supplierno + " "

		"FROM "
		"tendersuppliercalc AS ts "
		"INNER JOIN tenderhead th ON ts.tenderno=th.tenderno "
		"INNER JOIN artikelzentral az ON ts.articleno=az.artikel_nr "
		
		"WHERE "
		"ts.transferredflag = 0 "

		"GROUP BY "
		"branchno, " 
		"tenderno, "
		"articleno, "
		"customerno, "
		"baseprice, "
		"discountpct, " 
		"supplierno "

		"ORDER BY "
		"branchno, " 
		"tenderno "
			);

	resolve( constSQL );

    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( UpdateOMGSupplierCalcTransferred )

ENSURE_PROPERTIES_ARE_SET( UpdateOMGSupplierCalcTransferred,  
	BRANCH_NO.toPropertyString() +
	TENDER_NO.toPropertyString()
		)

void UpdateOMGSupplierCalcTransferred::buildSQLString()
{
    static const basar::VarString constSQL( 
        "UPDATE tendersuppliercalc "
        "SET "
        "transferredflag = 1 "

        "WHERE "
        "branchno = " + properties::BRANCH_NO.toSQLReplacementString() + " "
        "AND "
        "tenderno = " + properties::TENDER_NO.toSQLReplacementString() + " "
        );
    resolve( constSQL );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( InsertOMGSupplierCalc )
bool InsertOMGSupplierCalc::isExecutable() const
{
    METHODNAME_DEF( InsertOMGSupplierCalc, isExecutable );
    static const log4cplus::Logger& logger = LoggerPool::getLoggerDomModules();
    BLOG_TRACE_METHOD( logger, fun );

    bool executable = false;

    if( arePropertiesSetAndAllInState( properties::BRANCH_NO.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::TENDER_NO.toPropertyString(), basar::SS_INSERT ) && 
        arePropertiesSetAndAllInState( properties::ORDER_NO.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::ORDER_DATE.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::CUSTOMER_NO.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::ARTICLE_NO.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::INVOICE_NO.toPropertyString(), basar::SS_INSERT ) &&
        //arePropertiesSetAndAllInState( properties::INVOICE_DATE.toPropertyString(), basar::SS_INSERT ) &&
        //arePropertiesSetAndAllInState( properties::INVOICE_TIME.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::BASE_PRICE.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::DELIVERY_QTY.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::DISCOUNT_PCT.toPropertyString(), basar::SS_INSERT ) &&
        arePropertiesSetAndAllInState( properties::TRANSFERRED_FLAG.toPropertyString(), basar::SS_INSERT ) 
    )
    {
        executable = true;
    }

    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    return executable;;
}

void InsertOMGSupplierCalc::buildSQLString()
{
    static const basar::VarString constSQL(
            "INSERT INTO tendersuppliercalc "
            "( "
            "branchno, "
            "tenderno, "
            "orderno, "
            "orderdate, "
            "customerno, "
            "articleno, "
            "invoiceno, "
           // "invoicedate, "
           // "invoicetime, "
            "deliveryqty, "
            "baseprice, "
            "discountpct, "
            "transferredflag "
            ") "
            "VALUES "
            "( "
                + properties::BRANCH_NO.toSQLReplacementString()        +", "
                + properties::TENDER_NO.toSQLReplacementString()        +", "
                + properties::ORDER_NO.toSQLReplacementString()         +", "
                + "TO_CHAR(TO_DATE('" + properties::ORDER_DATE.toSQLReplacementString() + "', '%m/%d/%Y'), '%Y%m%d')::INTEGER, "
                + properties::CUSTOMER_NO.toSQLReplacementString()      +", "
                + properties::ARTICLE_NO.toSQLReplacementString()       +", "
                + properties::INVOICE_NO.toSQLReplacementString()       +", "
             //   + "TO_CHAR(TO_DATE('" + properties::INVOICE_DATE.toSQLReplacementString() + "', '%m/%d/%Y'), '%Y%m%d')::INTEGER, "
             //   + properties::INVOICE_TIME.toSQLReplacementString()     +", "
                + properties::DELIVERY_QTY.toSQLReplacementString()     +", "
                + properties::BASE_PRICE.toSQLReplacementString()     +", "
                + properties::DISCOUNT_PCT.toSQLReplacementString()     +", "
                + properties::TRANSFERRED_FLAG.toSQLReplacementString()  +" "
            ") "
         );
    resolve( constSQL );
    BLOG_TRACE_SQLSTRING( tenderbatchprocessing::LoggerPool::getLoggerDomModules() );
}


} //end namespace omgcalc
} //end namespace accessor
} //end namespace infrastructure
}
