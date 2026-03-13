#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverycollectionacc.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverycollectionacc_definitions.h>

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace infrastructure {
namespace accessor {
namespace scheduledDelivery {

//-----------------------------------------------------------------------------------------

BEGIN_ACCESSOR_DEFINITION( accessor::scheduledDelivery::lit::SCHEDULEDDELIVERY_COLLECTION_ACC )

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_ID                        )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO                      )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CSC_ORDER_DATE                   )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CSC_ORDER_NO                     )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::DELIVERY_DATE                    )
		PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY							)
	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD( accessor::scheduledDelivery::lit::SELECT_SCHEDULEDDELIVERY_BY_PATTERN );
		SQL_BUILDER_CREATE   ( SelectScheduledDeliveryByPattern )
		SQL_BUILDER_PUSH_BACK( SelectScheduledDeliveryByPattern )

	ACCESS_METHOD( accessor::scheduledDelivery::lit::IS_EXISTING );
		SQL_BUILDER_CREATE   ( IsExisting )
		SQL_BUILDER_PUSH_BACK( IsExisting )

	ACCESS_METHOD( accessor::scheduledDelivery::lit::SELECT_QUANTITY_BY_PATTERN );
		SQL_BUILDER_CREATE   ( SelectQuantityByPattern )
		SQL_BUILDER_PUSH_BACK( SelectQuantityByPattern )

END_ACCESSOR_DEFINITION

//-----------------------------------------------------------------------------------------

BUILDER_DEFINITION( SelectScheduledDeliveryByPattern )

bool SelectScheduledDeliveryByPattern::isExecutable() const
{
	METHODNAME_DEF( SelectScheduledDeliveryByPattern, isExecutable );
	static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = 
		arePropertiesSet(properties::TENDER_ID.toPropertyString()) ||
		arePropertiesSet(properties::DELIVERY_DATE.toPropertyString()) ||
		arePropertiesSet(properties::TENDER_NO.toPropertyString()) || //TODO
		arePropertiesSet(properties::DELIVERY_DATE_FROM.toPropertyString()) ||
		arePropertiesSet(properties::DELIVERY_DATE_TO.toPropertyString()) ||
		arePropertiesSet(properties::ARTICLE_CODE.toPropertyString()) ;

	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

	return executable;
}

void SelectScheduledDeliveryByPattern::buildSQLString()
{
	resolve(
		"SELECT "
			"sh.tenderid AS "						+ properties::TENDER_ID.getName()							+ ", "
			"sh.deliverydate AS "					+ properties::DELIVERY_DATE.getName()						+ ", "
			"sh.cscorderdate AS "					+ properties::CSC_ORDER_DATE.getName()						+ ", "
			"sh.cscorderno AS "						+ properties::CSC_ORDER_NO.getName()						+ ", "
			"sh.customerno AS "						+ properties::CUSTOMER_NO.getName()							+ ", "
			"sh.tenderdeliveryscheduleheadid AS "	+ properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.getName()	+ " "
		"FROM "
			"tenderhead th "
			"INNER JOIN tenderdeliveryschedule sh ON th.tenderno=sh.tenderid " );
	
	if (isContainedAndSet( properties::ARTICLE_CODE ) )
	{
		resolve( "INNER JOIN tenderdeliveryscheduleitem si ON sh.tenderdeliveryscheduleheadid = si.tenderdeliveryscheduleheadid " );
	}

	resolve( "WHERE th.deletedflag = 0 " );

	if (isContainedAndSet(properties::DELIVERY_DATE))
	{
		resolve( "AND sh.deliverydate = DATE('"  + properties::DELIVERY_DATE.toSQLReplacementString() + "') " );
	}
	if (isContainedAndSet(properties::DELIVERY_DATE_FROM))
	{
		resolve( "AND sh.deliverydate >= DATE('"  + properties::DELIVERY_DATE_FROM.toSQLReplacementString() + "') " );
	}
	if (isContainedAndSet(properties::DELIVERY_DATE_TO))
	{
		resolve( "AND sh.deliverydate <= DATE('"  + properties::DELIVERY_DATE_TO.toSQLReplacementString() + "') " );
	}
	if (isContainedAndSet(properties::CSC_ORDER_NO))
	{
		resolve( "AND sh.cscorderno = " + properties::CSC_ORDER_NO.toSQLReplacementString() + " ");
	}
	if (isContainedAndSet(properties::TENDER_ID))
	{
		resolve( "AND sh.tenderid = " + properties::TENDER_ID.toSQLReplacementString() + " ");
	}
	if (isContainedAndSet(properties::TENDER_NO))
	{
		resolve( "AND th.tenderno = " + properties::TENDER_NO.toSQLReplacementString() + " ");
	}
	if (isContainedAndSet(properties::ARTICLE_CODE))
	{
		resolve( "AND si.articlecode = " + properties::ARTICLE_CODE.toSQLReplacementString() + " ");
	}

	resolve("ORDER BY sh.deliverydate ASC");

	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-----------------------------------------------------------------------------------------

BUILDER_DEFINITION( IsExisting )

bool IsExisting::isExecutable() const
{
	METHODNAME_DEF( IsExisting, isExecutable );
	static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = 
		arePropertiesSet(properties::TENDER_ID.toPropertyString())		||
		arePropertiesSet(properties::DELIVERY_DATE.toPropertyString())	||
		arePropertiesSet(properties::TENDER_NO.toPropertyString())		||
		arePropertiesSet(properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toPropertyString());

	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

	return executable;
}

void IsExisting::buildSQLString()
{
	resolve(
		"SELECT "
			"sh.tenderid AS "						+ properties::TENDER_ID.getName()							+ ", "
			"sh.deliverydate AS "					+ properties::DELIVERY_DATE.getName()						+ ", "
			"sh.tenderdeliveryscheduleheadid AS "	+ properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.getName()	+ " "
		"FROM "
			"tenderhead th "
			"INNER JOIN tenderdeliveryschedule sh ON th.tenderno=sh.tenderid "
		"WHERE 1 = 1 ");

	if (isContainedAndSet(properties::BRANCH_NO) )
	{
		resolve( "AND sh.branchno = " + properties::BRANCH_NO.toSQLReplacementString() + " " );
	}
	if (isContainedAndSet(properties::CUSTOMER_NO) )
	{
		resolve( "AND sh.customerno = " + properties::CUSTOMER_NO.toSQLReplacementString() + " " );
	}
	if (isContainedAndSet(properties::DELIVERY_DATE))
	{
		resolve( "AND sh.deliverydate = DATE('"  + properties::DELIVERY_DATE.toSQLReplacementString() + "') " );
	}
	if (isContainedAndSet(properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID))
	{
		resolve( "AND sh.tenderdeliveryscheduleheadid != "  + properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toSQLReplacementString() + " " );
	}
	if (isContainedAndSet(properties::TENDER_ID))
	{
		resolve( "AND sh.tenderid = " + properties::TENDER_ID.toSQLReplacementString() + " ");
	}
	if (isContainedAndSet(properties::TENDER_NO))
	{
		resolve( "AND th.tenderno = " + properties::TENDER_NO.toSQLReplacementString() + " ");
	}

	resolve("ORDER BY sh.deliverydate ASC");

	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-----------------------------------------------------------------------------------------

BUILDER_DEFINITION( SelectQuantityByPattern )

bool SelectQuantityByPattern::isExecutable() const
{
	METHODNAME_DEF( IsExisting, isExecutable );
	static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = arePropertiesSet( properties::TENDER_ID.toPropertyString() ) ||
							arePropertiesSet( properties::TENDER_NO.toPropertyString() );

	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

	return executable;
}

void SelectQuantityByPattern::buildSQLString()
{
	resolve(
		"SELECT "
			"CAST(SUM(item.quantity) AS INT) AS "				+ properties::QUANTITY.getName() + " " + 
		"FROM tenderdeliveryschedule as head "
			"INNER JOIN tenderdeliveryscheduleitem as item "
			"ON head.tenderdeliveryscheduleheadid=item.tenderdeliveryscheduleheadid "
		"WHERE 1=1 " ); 

	if (isContainedAndSet(properties::TENDER_ID))
	{
		resolve( "AND head.tenderid = "  + properties::TENDER_ID.toSQLReplacementString() + " " );
	}

	if (isContainedAndSet(properties::TENDER_NO))
	{
		resolve( "AND head.tenderid = "  + properties::TENDER_NO.toSQLReplacementString() + " " );
	}

	if (isContainedAndSet(properties::ARTICLE_CODE))
	{
		resolve( "AND item.articlecode = "  + properties::ARTICLE_CODE.toSQLReplacementString() + " " );
	}

	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

//-----------------------------------------------------------------------------------------

} // end namespace scheduledDelivery
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender

//-----------------------------------------------------------------------------------------
