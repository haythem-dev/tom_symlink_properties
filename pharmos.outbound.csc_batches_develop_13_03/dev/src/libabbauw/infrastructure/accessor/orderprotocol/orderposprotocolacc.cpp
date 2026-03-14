//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/accessor.h>
#include <libutil/util.h>

#include "orderposprotocolacc.h"
#include "orderposprotocolacc_definitions.h"

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
    BEGIN_ACCESSOR_DEFINITION( lit::ORDER_POS_PROTOCOL_ACC )

        BEGIN_PROPERTY_DESCRIPTION_LIST
            PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCHNO          );
		    PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNO           );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::POSNO			 );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::PROTOCOL_DATE     );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ACTIVITY_TYPE     );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::EVENT_TYPE        );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::PROCESSED_BY		 );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLENO		 );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDEREDQTY		 );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::NONCHARGEDQTY	 );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNTPCT		 );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNTCALCFROM  );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNTAPPLYTO   );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNTTYPE		 );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::VIEW				 );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERREQUESTQTY	 );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::ITEMTEXT	         );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::EXTRATEXT	     );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( lit::SELECT_ORDER_POS_PROTOCOL_BY_KEY )
            SQL_BUILDER_CREATE   ( SelectOrderPosProtocolByKey )
            SQL_BUILDER_PUSH_BACK( SelectOrderPosProtocolByKey )

		ACCESS_METHOD( lit::SAVE_ORDER_POS_PROTOCOL )
            SQL_BUILDER_CREATE   ( InsertOrderPosProtocol )
            SQL_BUILDER_PUSH_BACK( InsertOrderPosProtocol )

    END_ACCESSOR_DEFINITION

    //-------------------------------------------------------------------------------------------------//
    // Builder Definition 'SelectOrderPosProtocolByKey'
    //-------------------------------------------------------------------------------------------------//
    BUILDER_DEFINITION( SelectOrderPosProtocolByKey )

    bool SelectOrderPosProtocolByKey::isExecutable() const
    {
        METHODNAME_DEF(SelectOrderPosProtocolByKey, isExecutable);
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;

        bool executable = isContainedAndSet( properties::ORDERNO );

        CHECK_EXECUTABILITY(getPropertyList(), logger, fun);

        return executable;
    }

    void SelectOrderPosProtocolByKey::buildSQLString()
    {
        METHODNAME_DEF(SelectOrderPosProtocolByKey, buildSQLString);
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;
        BLOG_TRACE_METHOD(logger, fun);

        static const basar::I18nString sqlStatement( 
            "SELECT "
			"opp.branchno AS "				+ properties::BRANCHNO.getName()			+ ", "
            "opp.orderno AS "				+ properties::ORDERNO.getName()				+ ", "
            "opp.posno AS "					+ properties::POSNO.getName()				+ ", "
			"opp.protocoldate AS "			+ properties::PROTOCOL_DATE.getName()		+ ", " 
			"opp.activitytype AS "			+ properties::ACTIVITY_TYPE.getName()		+ ", " 
            "opp.eventtype AS "				+ properties::EVENT_TYPE.getName()			+ ", "
			"opp.processedby AS "			+ properties::PROCESSED_BY.getName()		+ ", "
			"opp.articleno AS "				+ properties::ARTICLENO.getName()			+ ", "
			"opp.orderedqty AS "			+ properties::ORDEREDQTY.getName()			+ ", "
			"opp.nonchargedqty AS "			+ properties::NONCHARGEDQTY.getName()		+ ", "
			"opp.discountpct AS "			+ properties::DISCOUNTPCT.getName()			+ ", "
            "opp.discountcalcfrom AS "      + properties::DISCOUNTCALCFROM.getName()    + ", "
            "opp.discountapplyto AS "       + properties::DISCOUNTAPPLYTO.getName()     + ", "
			//TODO DZ "opp.discounttype AS "			+ properties::DISCOUNTTYPE.getName()		+ ", "
			"opp.view AS "					+ properties::VIEW.getName()				+ ", " 
			"opp.orderrequestqty AS "		+ properties::ORDERREQUESTQTY.getName()	    + ", "
            "opp.itemtext AS "		        + properties::ITEMTEXT.getName()	        + ", "
            "opp.extratext AS "		        + properties::EXTRATEXT.getName()	        + " "
            "FROM orderposprotocol opp "
            "WHERE orderno = "              + properties::ORDERNO.toSQLReplacementString()
        );

        resolve(sqlStatement);
        BLOG_TRACE_SQLSTRING(logger);
    }

	//-------------------------------------------------------------------------------------------------//
    // Builder Definition 'InsertOrderPosProtocol'
    //-------------------------------------------------------------------------------------------------//
    BUILDER_DEFINITION( InsertOrderPosProtocol )

    bool InsertOrderPosProtocol::isExecutable() const
    {
        METHODNAME_DEF(InsertOrderPosProtocol, isExecutable);
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;

        bool executable = isContainedAndSet( properties::ORDERNO ) &&
						  isContainedAndSet( properties::POSNO );

        CHECK_EXECUTABILITY(getPropertyList(), logger, fun);

        return executable;
    }

    void InsertOrderPosProtocol::buildSQLString()
    {
        METHODNAME_DEF(InsertOrderPosProtocol, buildSQLString);
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;
        BLOG_TRACE_METHOD(logger, fun);

		std::stringstream sql;
        sql << "INSERT INTO orderposprotocol "
			"( "
			<< properties::BRANCHNO.getName()			<< ", "
			<< properties::ORDERNO.getName()			<< ", "
            << properties::PROTOCOL_DATE.getName()      << ", "
			<< properties::POSNO.getName()				<< ", "
			<< properties::ACTIVITY_TYPE.getName()		<< ", "
			<< properties::EVENT_TYPE.getName()			<< ", "
			<< properties::PROCESSED_BY.getName()		<< ", "
			<< properties::ARTICLENO.getName()			<< ", "
			<< properties::ORDEREDQTY.getName()			<< ", "
			<< properties::NONCHARGEDQTY.getName()		<< ", "
			<< properties::DISCOUNTPCT.getName()		<< ", "
            << properties::DISCOUNTCALCFROM.getName()   << ", "
            << properties::DISCOUNTAPPLYTO.getName()    << ", "
			//TODO DZ << properties::DISCOUNTTYPE.getName()		<< ", "
			<< properties::VIEW.getName()				<< ", "
			<< properties::ORDERREQUESTQTY.getName()	<< " ";

		if( isContainedAndSet( properties::EXTRATEXT ) )
		{
			sql << "," << properties::EXTRATEXT.getName() << " ";
		}

		sql << ") VALUES ( " 
			<< properties::BRANCHNO.toSQLReplacementString()				<< ", " 
            << properties::ORDERNO.toSQLReplacementString()					<< ", "
            << "CURRENT, "
			<< properties::POSNO.toSQLReplacementString()					<< ", "
			<< properties::ACTIVITY_TYPE.toSQLReplacementString("'","'")	<< ", " 
			<< properties::EVENT_TYPE.toSQLReplacementString()				<< ", " 
			<< properties::PROCESSED_BY.toSQLReplacementString()			<< ", "
			<< properties::ARTICLENO.toSQLReplacementString()				<< ", " 
			<< properties::ORDEREDQTY.toSQLReplacementString()				<< ", "
            << properties::NONCHARGEDQTY.toSQLReplacementString()			<< ", "  
			<< properties::DISCOUNTPCT.toSQLReplacementString()				<< ", " 
            << properties::DISCOUNTCALCFROM.toSQLReplacementString()        << ", "
            << properties::DISCOUNTAPPLYTO.toSQLReplacementString()         << ", "
			//TODO DZ << properties::DISCOUNTTYPE.toSQLReplacementString()	        << ", "
			<< properties::VIEW.toSQLReplacementString("'","'")				<< ", "
			<< properties::ORDERREQUESTQTY.toSQLReplacementString()			<< " ";

		if( isContainedAndSet( properties::EXTRATEXT ) )
		{
			sql << ",'" << properties::EXTRATEXT.toSQLReplacementString() << "' ";
		}

		sql << ")";

        resolve( sql.str().c_str() );
        BLOG_TRACE_SQLSTRING(logger);
    }

} // end namespace orderprotocol
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw
