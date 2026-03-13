#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverypositionsacc.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverypositionsacc_definition.h>

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace infrastructure {
namespace accessor {
namespace scheduledDelivery {

//-----------------------------------------------------------------------------------------

BEGIN_ACCESSOR_DEFINITION( lit::SCHEDULEDDELIVERY_POSITIONS_ACC )

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO_POP )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_CODE )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NO )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NAME )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY_DELIVERED )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::LAST_CHANGED_DATETIME )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CREATION_DATETIME )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASE_ORDER_PROPOSAL_FLAG )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_PROPOSAL_NO )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASE_ORDER_NO )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_NAME )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_LOCATION )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_NO )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO )

	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD( accessor::scheduledDelivery::lit::SELECT_SCHEDULEDDELIVERY_POSITIONS_BY_KEY );
		SQL_BUILDER_CREATE	 ( SelectScheduledDeliveryPositionsByKey )
		SQL_BUILDER_PUSH_BACK( SelectScheduledDeliveryPositionsByKey )

	ACCESS_METHOD( accessor::scheduledDelivery::lit::SELECT_ALREADY_RESERVED_QUANTITY );
		SQL_BUILDER_CREATE	 ( SelectAlreadyReservedQuantity )
		SQL_BUILDER_PUSH_BACK( SelectAlreadyReservedQuantity )

	ACCESS_METHOD( accessor::scheduledDelivery::lit::SAVE_SCHEDULEDDELIVERY_POSITION )
		SQL_BUILDER_CREATE   ( DeleteScheduledDeliveryPosition )
		SQL_BUILDER_PUSH_BACK( DeleteScheduledDeliveryPosition )	
		SQL_BUILDER_CREATE   ( UpdateScheduledDeliveryPosition )
		SQL_BUILDER_PUSH_BACK( UpdateScheduledDeliveryPosition )
		SQL_BUILDER_CREATE   ( InsertScheduledDeliveryPosition )
		SQL_BUILDER_PUSH_BACK( InsertScheduledDeliveryPosition )

END_ACCESSOR_DEFINITION

//-----------------------------------------------------------------------------------------

BUILDER_DEFINITION( SelectScheduledDeliveryPositionsByKey )

bool SelectScheduledDeliveryPositionsByKey::isExecutable() const
{
	METHODNAME_DEF( SelectScheduledDeliveryPositionsByKey, isExecutable );
	static const log4cplus::Logger & logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = arePropertiesSet( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString() );

	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

	return executable;
}

void SelectScheduledDeliveryPositionsByKey::buildSQLString()
{
	resolve(
		"SELECT "
			"sp.tenderdeliveryscheduleheadid AS "	+	properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.getName()	+ ", "
			"sp.articlecode AS "					+	properties::ARTICLE_CODE.getName()						+ ", "
			"ac.articleno AS "						+	properties::ARTICLE_NO.getName()						+ ", "
			"az.artikel_name AS "					+	properties::ARTICLE_NAME.getName()						+ ", "
			"sp.quantity AS "						+	properties::QUANTITY.getName()							+ ", "
			"sp.quantitydelivered AS "				+	properties::QUANTITY_DELIVERED.getName()				+ ", "
			"sp.lastchangeddatetime AS "			+	properties::LAST_CHANGED_DATETIME.getName()				+ ", "
			"sp.creationdatetime AS "				+	properties::CREATION_DATETIME.getName()					+ ", "
			"sp.purchaseorderproposalflag AS "		+	properties::PURCHASE_ORDER_PROPOSAL_FLAG.getName()		+ ", "
			"sp.branchnopop AS "					+   properties::BRANCH_NO_POP.getName()						+ " "
			);
	if (getPropertyList().isContainedAndSet(properties::PURCHASE_DBSRV) && getPropertyList().isContainedAndSet(properties::PURCHASE_DB))
	{
		resolve(
			", NVL(pb.orderproposalno, 0) AS "		+ 	properties::ORDER_PROPOSAL_NO.getName()					+ " "
			", NVL(pb.purchaseorderno, 0) AS "		+	properties::PURCHASE_ORDER_NO.getName()					+ " "
			);
	}
	resolve(	
		"FROM "
			"tenderdeliveryscheduleitem sp "
			"INNER JOIN articlecodes ac ON sp.articlecode=ac.article_code "
			"INNER JOIN artikelzentral az ON ac.articleno=az.artikel_nr "
			);

	if (getPropertyList().isContainedAndSet(properties::PURCHASE_DBSRV) && getPropertyList().isContainedAndSet(properties::PURCHASE_DB))
	{
		resolve(
			"INNER JOIN tenderdeliveryschedule sh ON sp.tenderdeliveryscheduleheadid=sh.tenderdeliveryscheduleheadid "
			"INNER JOIN tenderhead th ON sh.tenderid=th.tenderno "
			"LEFT OUTER JOIN " + properties::PURCHASE_DB.toSQLReplacementString() + "@" + properties::PURCHASE_DBSRV.toSQLReplacementString() + ":pubatchorderpos pb ON "
				"sp.branchnopop=pb.branchno AND "
				"ac.articleno=pb.articleno AND "
				"sp.tenderdeliveryscheduleheadid=pb.ordernobatch AND "
				"pb.ordertypebatch=2 "
			);
	}
	resolve(
		"WHERE "
			"sp.tenderdeliveryscheduleheadid="		+  properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toSQLReplacementString() + " "
		"ORDER BY "
			"sp.articlecode "
		);
	
	BLOG_TRACE( libtender::LoggerPool::getLoggerDomModules(), getSQLString() );
}

//-----------------------------------------------------------------------------------------
BUILDER_DEFINITION( InsertScheduledDeliveryPosition )

ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( InsertScheduledDeliveryPosition,  
	properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString() +
	properties::BRANCH_NO_POP.toPropertyString() +
	properties::ARTICLE_CODE.toPropertyString() +
	properties::QUANTITY.toPropertyString() +
	properties::PURCHASE_ORDER_PROPOSAL_FLAG.toPropertyString(),

	properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString() +
	properties::BRANCH_NO_POP.toPropertyString() +
	properties::ARTICLE_CODE.toPropertyString() + 
	properties::QUANTITY.toPropertyString() +
	properties::PURCHASE_ORDER_PROPOSAL_FLAG.toPropertyString(),

	basar::SS_INSERT
		)

void InsertScheduledDeliveryPosition::buildSQLString()
{
	resolve( 
		"INSERT INTO tenderdeliveryscheduleitem "
		"( "
		"tenderdeliveryscheduleheadid, "
		"articlecode, "
		"quantity, "
		"purchaseorderproposalflag, "
		"lastchangeddatetime, "
		"creationdatetime, "
		"branchnopop "
		") "
		"VALUES "
		" ( " 
		+ properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toSQLReplacementString() + ", "
		"'" + properties::ARTICLE_CODE.toSQLReplacementString() + "', "
		+ properties::QUANTITY.toSQLReplacementString() + ", "
		+ properties::PURCHASE_ORDER_PROPOSAL_FLAG.toSQLReplacementString() + ", "
		"CURRENT, "
		"CURRENT, "
		+ properties::BRANCH_NO_POP.toSQLReplacementString() +
		" ) " 
		);
	
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-----------------------------------------------------------------------------------------

BUILDER_DEFINITION( UpdateScheduledDeliveryPosition )

ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( UpdateScheduledDeliveryPosition,  
	properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString() +
	properties::BRANCH_NO_POP.toPropertyString() +
	properties::ARTICLE_CODE.toPropertyString() +
	properties::QUANTITY.toPropertyString() +
	properties::QUANTITY_DELIVERED.toPropertyString() +
	properties::PURCHASE_ORDER_PROPOSAL_FLAG.toPropertyString(),

	properties::BRANCH_NO_POP.toPropertyString() +
	properties::QUANTITY.toPropertyString() +
	properties::QUANTITY_DELIVERED.toPropertyString() +
	properties::PURCHASE_ORDER_PROPOSAL_FLAG.toPropertyString(),

	basar::SS_UPDATE
	)

void UpdateScheduledDeliveryPosition::buildSQLString()
{
	resolve(
		"UPDATE tenderdeliveryscheduleitem SET "
			"quantity = "						+ properties::QUANTITY.toSQLReplacementString()						+ ", "
		    "quantitydelivered = "				+ properties::QUANTITY_DELIVERED.toSQLReplacementString()			+ ", "
			"purchaseorderproposalflag = "		+ properties::PURCHASE_ORDER_PROPOSAL_FLAG.toSQLReplacementString() + ", "
			"branchnopop = "					+ properties::BRANCH_NO_POP.toSQLReplacementString()				+ ", "
			"lastchangeddatetime = CURRENT "
		"WHERE "
			"tenderdeliveryscheduleheadid = "	+ properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toSQLReplacementString() + " AND "
			"articlecode = '"					+ properties::ARTICLE_CODE.toSQLReplacementString() + "'" 
		);

	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-----------------------------------------------------------------------------------------

BUILDER_DEFINITION( DeleteScheduledDeliveryPosition )

ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( DeleteScheduledDeliveryPosition,  
	properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString() +
	properties::ARTICLE_CODE.toPropertyString(),

	properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString() +
	properties::ARTICLE_CODE.toPropertyString(),

	basar::SS_DELETE
		)

void DeleteScheduledDeliveryPosition::buildSQLString()
{
	resolve( 
		"DELETE tenderdeliveryscheduleitem "
		"WHERE "
		"tenderdeliveryscheduleheadid = " + properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toSQLReplacementString() + " "
		"AND "
		"articlecode = " + properties::ARTICLE_CODE.toSQLReplacementString() + " "
		);
	
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
BUILDER_DEFINITION( SelectAlreadyReservedQuantity )

ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( SelectAlreadyReservedQuantity,  
	//properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString() +
	properties::ARTICLE_CODE.toPropertyString() +
	properties::TENDER_ID.toPropertyString(),

	//properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString() +
	properties::ARTICLE_CODE.toPropertyString() +
	properties::TENDER_ID.toPropertyString(),

	basar::SS_DELETE
	)

void SelectAlreadyReservedQuantity::buildSQLString()
{
	resolve( 
		"SELECT SUM(p.quantity) FROM tenderdeliveryscheduleitem p "
			"INNER JOIN tenderdeliveryschedule h ON p.tenderdeliveryscheduleheadid=h.tenderdeliveryscheduleheadid "
		"WHERE "
			"h.tenderid = "							+ properties::TENDER_ID.toSQLReplacementString() + " AND "
			"p.articlecode = '"						+ properties::ARTICLE_CODE.toSQLReplacementString() + "' AND " );
			//"p.tenderdeliveryscheduleheadid <> "	+ properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toSQLReplacementString()	+ " AND "

			if( isContainedAndSet( properties::DELIVERY_DATE ) )
			{
				resolve("h.deliverydate != TO_DATE( '"		    + properties::DELIVERY_DATE.toSQLReplacementString() + "', '%m/%d/%Y') AND ");   
			}
			
			resolve("h.cscorderno = 0");

	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

} // end namespace tender
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender
