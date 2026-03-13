#include "orderheadprotocolacc.h"
#include "properties_definitions.h"

#include <libbasarcmnutil_bstring.h> 
#include <libbasarproperty_propertydescriptionlistref.h>
#include <libbasarcmnutil_i18nstring.h>

namespace orderheadprotocolacc 
{
	BUILDER_DEFINITION(saveOrderHeadProtocol)

	ENSURE_PROPERTIES_ARE_SET(saveOrderHeadProtocol,"branchno;orderno;")

	void saveOrderHeadProtocol::buildSQLString() 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "orderproposalacc::saveOrderHeadProtocol::buildSQLString().");
		
		using namespace cscbatchhistory::properties;

		try 
        {	
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

            basar::I18nString sqlStatement = "INSERT INTO orderheadprotocol "
                "( "
                "branchno,"
                "orderno,"
                "protocoldate,"
                "activitytype,"
                "eventtype,"
                "processedby,"
                "changedattribute,"
                "oldvalue,"
                "newvalue,"
                "splitfrom, "
                "splitto, "
                "headertext, "
                "ordernocsc, "
                "extratext "
                ") "
                "VALUES ( " + 
				BRANCHNO.toSQLReplacementString() + ", " +
				ORDERNO.toSQLReplacementString() + ", "
				"CURRENT, "
				"'" + ACTIVITY_TYPE.toSQLReplacementString() + "', " +
				EVENT_TYPE.toSQLReplacementString() + ", " +
				PROCESSED_BY.toSQLReplacementString() + ", " +
				"'" + CHANGED_ATTRIBUTE.toSQLReplacementString() + "', " +
				"'" + OLD_VALUE.toSQLReplacementString() + "'," +
				"'" + NEW_VALUE.toSQLReplacementString() + "'," +
				SPLIT_FROM.toSQLReplacementString() + ", " +
				SPLIT_TO.toSQLReplacementString() + ", " +
				"'" + HEADERTEXT.toSQLReplacementString() + "', " +
				ORDERNOCSC.toSQLReplacementString() + ", " +
				"'" + EXTRATEXT.toSQLReplacementString() + "'" +
                ")";

			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString());
		}
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "orderproposalacc::saveOrderHeadProtocol()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

    BUILDER_DEFINITION(deleteOrderHeadProtocol)

	ENSURE_PROPERTIES_ARE_SET(deleteOrderHeadProtocol,"branchno;minagefororderprotocol;")

	void deleteOrderHeadProtocol::buildSQLString() 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "orderheadprotocolacc::deleteOrderHeadProtocol::buildSQLString().");
		try 
        {	
            basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

            basar::I18nString sqlStatement = "DELETE FROM orderheadprotocol "
                "WHERE orderno IN "
                "( SELECT distinct orderno FROM orderheadprotocol "
                "WHERE branchno = #branchno# AND "
                //"( protocoldate <= TO_DATE('#minagefororderprotocol# 23:59:59.999', '%Y%m%d %H:%M:%S %F3') AND "
				"( protocoldate <= TO_DATE('#minagefororderprotocol# 23:59:59.999', '%04Y%02m%02d %02H:%02M:%02S.%03F3') AND "
                "activitytype = 'D'"
                " OR "
                "protocoldate <= (today-730) ) " 
                ")";

			resolve(sqlStatement);
			BLOG_TRACE(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString());
		}
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "orderheadprotocolacc::deleteOrderHeadProtocol()."); 
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, getSQLString()); 
			throw;
		}
	}

	//*******************************************************************************************************
	BEGIN_ACCESSOR_DEFINITION("OrderHeadProtocolAcc")
		using namespace cscbatchhistory::properties;

			BEGIN_PROPERTY_DESCRIPTION_LIST
				PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO );
				PROPERTY_DESCRIPTION_LIST_ADD( ORDERNO  );
				PROPERTY_DESCRIPTION_LIST_ADD( ACTIVITY_TYPE  );
				PROPERTY_DESCRIPTION_LIST_ADD( EVENT_TYPE  );
				PROPERTY_DESCRIPTION_LIST_ADD( PROCESSED_BY  );
				PROPERTY_DESCRIPTION_LIST_ADD( CHANGED_ATTRIBUTE  );
				PROPERTY_DESCRIPTION_LIST_ADD( OLD_VALUE  );
				PROPERTY_DESCRIPTION_LIST_ADD( NEW_VALUE  );
				PROPERTY_DESCRIPTION_LIST_ADD( SPLIT_FROM  );
				PROPERTY_DESCRIPTION_LIST_ADD( SPLIT_TO  );
				PROPERTY_DESCRIPTION_LIST_ADD( HEADERTEXT  );
				PROPERTY_DESCRIPTION_LIST_ADD( ORDERNOCSC  );
				PROPERTY_DESCRIPTION_LIST_ADD( EXTRATEXT  );
			END_PROPERTY_DESCRIPTION_LIST

			ACCESS_METHOD("saveOrderHeadProtocol")
			SQL_BUILDER_CREATE(saveOrderHeadProtocol)
			SQL_BUILDER_PUSH_BACK(saveOrderHeadProtocol)

            ACCESS_METHOD("deleteOrderHeadProtocol")
			SQL_BUILDER_CREATE(deleteOrderHeadProtocol)
			SQL_BUILDER_PUSH_BACK(deleteOrderHeadProtocol)

	END_ACCESSOR_DEFINITION	

}
