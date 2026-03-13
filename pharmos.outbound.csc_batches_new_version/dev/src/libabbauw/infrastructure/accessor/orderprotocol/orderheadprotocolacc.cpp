//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/accessor.h>
#include <libutil/util.h>

#include "orderheadprotocolacc.h"
#include "orderheadprotocolacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

//---------------------------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace infrastructure
{
namespace accessor
{
namespace orderprotocol
{
    //-------------------------------------------------------------------------------------------------//
    // Accessor Definition
    //-------------------------------------------------------------------------------------------------//
    BEGIN_ACCESSOR_DEFINITION( lit::ORDER_HEAD_PROTOCOL_ACC )

        BEGIN_PROPERTY_DESCRIPTION_LIST
            PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCHNO          );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNO           );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::PROTOCOL_DATE     );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ACTIVITY_TYPE     );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::EVENT_TYPE        );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::PROCESSED_BY      );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::CHANGED_ATTRIBUTE );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::OLD_VALUE         );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::NEW_VALUE         );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::SPLIT_FROM        );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::SPLIT_TO          );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::HEADERTEXT        );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNOCSC        );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::EXTRATEXT         );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( lit::SELECT_ORDER_HEAD_PROTOCOL_BY_KEY )
            SQL_BUILDER_CREATE   ( SelectOrderHeadProtocolByKey )
            SQL_BUILDER_PUSH_BACK( SelectOrderHeadProtocolByKey )

		ACCESS_METHOD( lit::SAVE_ORDER_HEAD_PROTOCOL )
            SQL_BUILDER_CREATE   ( InsertOrderHeadProtocol )
            SQL_BUILDER_PUSH_BACK( InsertOrderHeadProtocol )

    END_ACCESSOR_DEFINITION

    //-------------------------------------------------------------------------------------------------//
    // Builder Definition 'SelectOrderHeadProtocolByKey'
    //-------------------------------------------------------------------------------------------------//
    BUILDER_DEFINITION( SelectOrderHeadProtocolByKey )

    bool SelectOrderHeadProtocolByKey::isExecutable() const
    {
        METHODNAME_DEF(SelectOrderHeadProtocolByKey, isExecutable);
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;

        bool executable = isContainedAndSet( properties::ORDERNO );

        CHECK_EXECUTABILITY(getPropertyList(), logger, fun);

        return executable;
    }

    void SelectOrderHeadProtocolByKey::buildSQLString()
    {
        METHODNAME_DEF(SelectOrderHeadProtocolByKey, buildSQLString);
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;
        BLOG_TRACE_METHOD(logger, fun);

        static const basar::I18nString sqlStatement( 
            "SELECT "
			"ohp.branchno AS "				+ properties::BRANCHNO.getName()			+ ", " 
            "ohp.orderno AS "				+ properties::ORDERNO.getName()				+ ", " 
            "ohp.protocoldate AS "			+ properties::PROTOCOL_DATE.getName()		+ ", " 
            "ohp.activitytype AS "			+ properties::ACTIVITY_TYPE.getName()		+ ", " 
            "ohp.eventtype AS "				+ properties::EVENT_TYPE.getName()			+ ", "
            "ohp.processedby AS "           + properties::PROCESSED_BY.getName()        + ", "
		    "ohp.changedattribute AS "      + properties::CHANGED_ATTRIBUTE.getName()	+ ", " 
		    "ohp.oldvalue AS "				+ properties::OLD_VALUE.getName()			+ ", " 
            "ohp.newvalue AS "				+ properties::NEW_VALUE.getName()			+ ", " 
			"ohp.splitfrom AS "				+ properties::SPLIT_FROM.getName()			+ ", "
            "ohp.splitto AS "               + properties::SPLIT_TO.getName()            + ", "
            "ohp.headertext AS "            + properties::HEADERTEXT.getName()          + ", "
            "ohp.ordernocsc AS "            + properties::ORDERNOCSC.getName()          + ", "
            "ohp.extratext AS "             + properties::EXTRATEXT.getName()           + " "
            "FROM orderheadprotocol ohp "
            "WHERE orderno = "              + properties::ORDERNO.toSQLReplacementString()
        );

        resolve(sqlStatement);
        BLOG_TRACE_SQLSTRING(logger);
    }

	//-------------------------------------------------------------------------------------------------//
    // Builder Definition 'InsertOrderHeadProtocol'
    //-------------------------------------------------------------------------------------------------//
    BUILDER_DEFINITION( InsertOrderHeadProtocol )

    bool InsertOrderHeadProtocol::isExecutable() const
    {
        METHODNAME_DEF(InsertOrderHeadProtocol, isExecutable);
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;

        bool executable = isContainedAndSet( properties::ORDERNO ) &&
						  isContainedAndSet( properties::ACTIVITY_TYPE );

        CHECK_EXECUTABILITY(getPropertyList(), logger, fun);

        return executable;
    }

    void InsertOrderHeadProtocol::buildSQLString()
    {
        METHODNAME_DEF(InsertOrderHeadProtocol, buildSQLString);
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;
        BLOG_TRACE_METHOD(logger, fun);

        static const basar::I18nString sqlStatement( 
            "INSERT INTO orderheadprotocol ( "
			+ properties::BRANCHNO.getName()			+ ", "
			+ properties::ORDERNO.getName()				+ ", "
            + properties::PROTOCOL_DATE.getName()       + ", "
			+ properties::ACTIVITY_TYPE.getName()		+ ", "
			+ properties::EVENT_TYPE.getName()			+ ", "
			+ properties::PROCESSED_BY.getName()		+ ", "
			+ properties::CHANGED_ATTRIBUTE.getName()	+ ", "
			+ properties::OLD_VALUE.getName()			+ ", "
			+ properties::NEW_VALUE.getName()			+ ", "
			+ properties::SPLIT_FROM.getName()			+ ", "
            + properties::SPLIT_TO.getName()			+ ", "
            + properties::HEADERTEXT.getName()			+ ", "
            + properties::ORDERNOCSC.getName()			+ ", "
            + properties::EXTRATEXT.getName()			+ " "
			") "
			"VALUES ( " 
			+ properties::BRANCHNO.toSQLReplacementString()				    + ", " 
            + properties::ORDERNO.toSQLReplacementString()				    + ", " 
            + "CURRENT"                                                     + ", "
			+ properties::ACTIVITY_TYPE.toSQLReplacementString("'","'")		+ ", " 
			+ properties::EVENT_TYPE.toSQLReplacementString()			    + ", "
			+ properties::PROCESSED_BY.toSQLReplacementString()			    + ", " 
			+ properties::CHANGED_ATTRIBUTE.toSQLReplacementString("'","'")	+ ", " 
			+ properties::OLD_VALUE.toSQLReplacementString("'","'")			+ ", " 
			+ properties::NEW_VALUE.toSQLReplacementString("'","'")			+ ", " 
			+ properties::SPLIT_FROM.toSQLReplacementString()				+ ", " 
			+ properties::SPLIT_TO.toSQLReplacementString()				    + ", "
            + properties::HEADERTEXT.toSQLReplacementString("'","'")		+ ", "
            + properties::ORDERNOCSC.toSQLReplacementString()				+ ", "
            + properties::EXTRATEXT.toSQLReplacementString("'","'")			+ " "
            ")"
        );

        resolve(sqlStatement);
        BLOG_TRACE_SQLSTRING(logger);
    }

} // end namespace orderprotocol
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw
