#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliveryheaderacc.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliveryheaderacc_definitions.h>

namespace libtender 
{
namespace infrastructure 
{
namespace accessor 
{
namespace scheduledDelivery 
{

BEGIN_ACCESSOR_DEFINITION( lit::SCHEDULEDDELIVERY_HEADER_ACC )
		
	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_ID )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::DELIVERY_DATE )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CSC_ORDER_NO )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CSC_ORDER_DATE )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::LAST_CHANGED_DATETIME )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CREATION_DATETIME )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO )
	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD( accessor::scheduledDelivery::lit::SELECT_SCHEDULEDDELIVERY_HEADER_BY_KEY );
		SQL_BUILDER_CREATE   ( SelectScheduledDeliveryHeaderByKey )
		SQL_BUILDER_PUSH_BACK( SelectScheduledDeliveryHeaderByKey )

	ACCESS_METHOD( accessor::scheduledDelivery::lit::SAVE_SCHEDULEDDELIVERY_HEADER );
		SQL_BUILDER_CREATE   ( DeleteScheduledDeliveryHeader )
		SQL_BUILDER_PUSH_BACK( DeleteScheduledDeliveryHeader )

		SQL_BUILDER_CREATE   ( UpdateScheduledDeliveryHeader )
		SQL_BUILDER_PUSH_BACK( UpdateScheduledDeliveryHeader )

		SQL_BUILDER_CREATE   ( InsertScheduledDeliveryHeader )
		SQL_BUILDER_PUSH_BACK( InsertScheduledDeliveryHeader )

END_ACCESSOR_DEFINITION

//-----------------------------------------------------------------------------------------

BUILDER_DEFINITION( SelectScheduledDeliveryHeaderByKey )

bool SelectScheduledDeliveryHeaderByKey::isExecutable() const
{
	METHODNAME_DEF( SelectScheduledDeliveryHeaderByKey, isExecutable );
	static const log4cplus::Logger & logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = arePropertiesSet( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString() );

	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

	return executable;
}

void SelectScheduledDeliveryHeaderByKey::buildSQLString()
{
	resolve(
		"SELECT "
			"sh.tenderdeliveryscheduleheadid AS "	+	properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.getName()	+ ", "
			"sh.tenderid AS "						+	properties::TENDER_ID.getName()							+ ", "
			"sh.deliverydate AS "					+	properties::DELIVERY_DATE.getName()						+ ", "
			"sh.cscorderno AS "						+	properties::CSC_ORDER_NO.getName()						+ ", "
			"sh.cscorderdate AS "					+	properties::CSC_ORDER_DATE.getName()					+ ", "
			"sh.lastchangeddatetime AS "			+	properties::LAST_CHANGED_DATETIME.getName()				+ ", "
			"sh.creationdatetime AS "				+	properties::CREATION_DATETIME.getName()					+ ", "
            "sh.customerno AS "						+	properties::CUSTOMER_NO.getName()					    + ", "
            "sh.branchno AS "						+	properties::BRANCH_NO.getName()							+ " "
		"FROM "
			"tenderdeliveryschedule sh "
		"WHERE "
			"sh.tenderdeliveryscheduleheadid="		+  properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toSQLReplacementString()
		);

	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-----------------------------------------------------------------------------------------

BUILDER_DEFINITION( InsertScheduledDeliveryHeader )
	
using namespace properties;

ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( InsertScheduledDeliveryHeader,  
	TENDER_ID.toPropertyString() +
	DELIVERY_DATE.toPropertyString(),

	TENDER_ID.toPropertyString()+
	DELIVERY_DATE.toPropertyString(),

	basar::SS_INSERT ) 

void InsertScheduledDeliveryHeader::buildSQLString()
{
    resolve( 
		"INSERT INTO tenderdeliveryschedule " 
		"("
		"tenderid, "
        "customerno, "
        "branchno, "
		"deliverydate, "
		"lastchangeddatetime, "
		"creationdatetime "
		") VALUES ("
		+ properties::TENDER_ID.toSQLReplacementString() + ", "
        + properties::CUSTOMER_NO.toSQLReplacementString() + ", "
        + properties::BRANCH_NO.toSQLReplacementString() + ", "
        "DATE('" + properties::DELIVERY_DATE.toSQLReplacementString() + "'), "
		"CURRENT, "
		"CURRENT "
		") "
		);

    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}	

//-----------------------------------------------------------------------------------------

BUILDER_DEFINITION( UpdateScheduledDeliveryHeader )
	
	ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( UpdateScheduledDeliveryHeader, 
		TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString() +
		DELIVERY_DATE.toPropertyString() +
		BRANCH_NO.toPropertyString() +
		CUSTOMER_NO.toPropertyString(),

		DELIVERY_DATE.toPropertyString() +
		BRANCH_NO.toPropertyString() +
		CUSTOMER_NO.toPropertyString() +
		CSC_ORDER_NO.toPropertyString() +
		CSC_ORDER_DATE.toPropertyString(),

		basar::SS_UPDATE )

void UpdateScheduledDeliveryHeader::buildSQLString()
{
	resolve(
		"UPDATE tenderdeliveryschedule " 
		"SET "
	    "customerno = "						+ properties::CUSTOMER_NO.toSQLReplacementString()		+ ", "
	    "branchno = "						+ properties::BRANCH_NO.toSQLReplacementString()		+ ", "
		"deliverydate = DATE('"				+ properties::DELIVERY_DATE.toSQLReplacementString()	+ "'), "
		"cscorderno = "						+ properties::CSC_ORDER_NO.toSQLReplacementString()		+ ", "
		"cscorderdate = DATE('"				+ properties::CSC_ORDER_DATE.toSQLReplacementString()	+ "'), "
		"lastchangeddatetime = CURRENT " 
		"WHERE "
		"tenderdeliveryscheduleheadid = "	+ properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toSQLReplacementString() 
		);
	
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}	

//-----------------------------------------------------------------------------------------

BUILDER_DEFINITION( DeleteScheduledDeliveryHeader )
	
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( DeleteScheduledDeliveryHeader,  
	TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString(),
	TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString(),
	basar::SS_UPDATE )

void DeleteScheduledDeliveryHeader::buildSQLString()
{ 
	resolve(
		"DELETE tenderdeliveryschedule " 
		"WHERE "
		"tenderdeliveryscheduleheadid = " + properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toSQLReplacementString() + ";"
	);

    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

} //end namespace scheduledDelivery
} //end namespace accessor
} //end namespace infrastructure
} //end namespace libtender
