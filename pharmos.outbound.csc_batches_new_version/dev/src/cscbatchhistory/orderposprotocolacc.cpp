#include "orderposprotocolacc.h"
#include "properties_definitions.h"

#include <libbasarcmnutil_bstring.h> 
#include <libbasarproperty_propertydescriptionlistref.h>
#include <libbasarcmnutil_i18nstring.h>

namespace orderposprotocolacc 
{
	BUILDER_DEFINITION(saveOrderPosProtocol)

	ENSURE_PROPERTIES_ARE_SET(saveOrderPosProtocol,"branchno;orderno;posno;")

	void saveOrderPosProtocol::buildSQLString() 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "orderproposalacc::saveOrderPosProtocol::buildSQLString().");
		try 
        {	
			using namespace cscbatchhistory::properties;

            basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			basar::I18nString sqlStatement = "INSERT INTO orderposprotocol "
                "( "
                "branchno,"
                "orderno,"
                "protocoldate,"
                "posno,"
                "activitytype,"
                "eventtype,"
                "processedby,"
                "articleno,"
                "orderedqty,"
                "nonchargedqty,"
                "discountpct,"
				"discountcalcfrom,"
				"discountapplyto,"
				//TODO DZ "discounttype,"
                "view, "
                "orderrequestqty, "
                "itemtext, "
                "extratext "
                ") "
                "VALUES ( " + 
				BRANCHNO.toSQLReplacementString() + ", " +
				ORDERNO.toSQLReplacementString() + ", " +
				"CURRENT, " + 
				POSNO.toSQLReplacementString() + ", " +
				"'" + ACTIVITY_TYPE.toSQLReplacementString() + "', " +
				EVENT_TYPE.toSQLReplacementString() + ", " +
				PROCESSED_BY.toSQLReplacementString() + ", " +
				ARTICLENO.toSQLReplacementString() + ", " +
				ORDEREDQTY.toSQLReplacementString() + ", " +
				NONCHARGEDQTY.toSQLReplacementString() + ", " +
				DISCOUNTPCT.toSQLReplacementString() + ", " +
				DISCOUNTCALCFROM.toSQLReplacementString() + ", " +
				DISCOUNTAPPLYTO.toSQLReplacementString() + ", " +
				"'" + VIEW.toSQLReplacementString() + "', " +
				ORDERREQUESTQTY.toSQLReplacementString() + ", " +
				"'" + ITEMTEXT.toSQLReplacementString() + "', " +
				"'" + EXTRATEXT.toSQLReplacementString() + "' " +
                ")";

			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString());
		}
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "orderproposalacc::saveOrderPosProtocol()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

    BUILDER_DEFINITION(deleteOrderPosProtocol)

	ENSURE_PROPERTIES_ARE_SET(deleteOrderPosProtocol,"branchno;minagefororderprotocol;")

	void deleteOrderPosProtocol::buildSQLString() 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "orderposprotocolacc::deleteOrderPosProtocol::buildSQLString().");

		using namespace cscbatchhistory::properties;

		try 
        {	
            basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

			basar::I18nString sqlStatement = "DELETE FROM orderposprotocol AS opp "
                "WHERE "
                "opp.branchno = #branchno# AND "
                "( " 
                //"opp.protocoldate <= TO_DATE('#minagefororderprotocol# 23:59:59.999', '%Y%m%d %H:%M:%S %F3') AND "
				"opp.protocoldate <= TO_DATE('#minagefororderprotocol# 23:59:59.999', '%04Y%02m%02d %02H:%02M:%02S.%03F3') AND "
                "EXISTS (SELECT * from orderheadprotocol as ohp WHERE ohp.orderno = opp.orderno AND activitytype = 'D') "
                "OR "
                "opp.protocoldate <= (today-730) "
                ")";

			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString());
		}
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "orderposprotocolacc::deleteOrderPosProtocol()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

	//*******************************************************************************************************
	BEGIN_ACCESSOR_DEFINITION("OrderPosProtocolAcc")
		using namespace cscbatchhistory::properties;

		//TODO DZ add discounttype

			BEGIN_PROPERTY_DESCRIPTION_LIST
				PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO );
				PROPERTY_DESCRIPTION_LIST_ADD( ORDERNO  );
				PROPERTY_DESCRIPTION_LIST_ADD( POSNO  );
				PROPERTY_DESCRIPTION_LIST_ADD( ACTIVITY_TYPE  );
				PROPERTY_DESCRIPTION_LIST_ADD( EVENT_TYPE  );
				PROPERTY_DESCRIPTION_LIST_ADD( PROCESSED_BY  );
				PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENO  );
				PROPERTY_DESCRIPTION_LIST_ADD( ORDEREDQTY  );
				PROPERTY_DESCRIPTION_LIST_ADD( NONCHARGEDQTY  );
				PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTPCT  );
				PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTCALCFROM  );
				PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTAPPLYTO  );
				PROPERTY_DESCRIPTION_LIST_ADD( SPLIT_FROM  );
				PROPERTY_DESCRIPTION_LIST_ADD( SPLIT_TO  );
				PROPERTY_DESCRIPTION_LIST_ADD( VIEW  );
				PROPERTY_DESCRIPTION_LIST_ADD( ORDERREQUESTQTY  );
				PROPERTY_DESCRIPTION_LIST_ADD( ITEMTEXT  );
				PROPERTY_DESCRIPTION_LIST_ADD( EXTRATEXT  );
			END_PROPERTY_DESCRIPTION_LIST

			ACCESS_METHOD("saveOrderPosProtocol")
			SQL_BUILDER_CREATE(saveOrderPosProtocol)
			SQL_BUILDER_PUSH_BACK(saveOrderPosProtocol)

            ACCESS_METHOD("deleteOrderPosProtocol")
			SQL_BUILDER_CREATE(deleteOrderPosProtocol)
			SQL_BUILDER_PUSH_BACK(deleteOrderPosProtocol)

	END_ACCESSOR_DEFINITION	

}
