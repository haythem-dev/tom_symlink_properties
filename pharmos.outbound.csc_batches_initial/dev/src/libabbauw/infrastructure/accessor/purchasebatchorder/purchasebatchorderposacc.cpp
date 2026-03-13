//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/accessor.h>
#include <libutil/util.h>

#include "purchasebatchorderposacc.h"
#include "purchasebatchorderposacc_definitions.h"

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
namespace purchaseBatchOrder
{

    //-------------------------------------------------------------------------------------------------//
    // Accessor Definition
    //-------------------------------------------------------------------------------------------------//
    BEGIN_ACCESSOR_DEFINITION( lit::ACC_PURCHASE_BATCH_ORDER_POS )

        BEGIN_PROPERTY_DESCRIPTION_LIST
			PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCHNO                  );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNO                   );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNO_UPDATE            );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLENO                 );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERPROPOSALQTY          );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERPROPOSALTIME         );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_PROP_PROCESSED_TIME );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_PROP_NO             );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASEORDERNO           );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( lit::SELECT_PURCHASE_BATCH_ORDER_POS_BY_KEY )
            SQL_BUILDER_CREATE   ( SelectPurchaseBatchOrderPosByKey )
            SQL_BUILDER_PUSH_BACK( SelectPurchaseBatchOrderPosByKey )

        ACCESS_METHOD( lit::SAVE_PURCHASE_BATCH_ORDER_POS )
            SQL_BUILDER_CREATE   ( UpdatePurchaseBatchOrderPos )
            SQL_BUILDER_PUSH_BACK( UpdatePurchaseBatchOrderPos )

        ACCESS_METHOD( lit::DELETE_PURCHASE_BATCH_ORDER_POS )
            SQL_BUILDER_CREATE   ( DeletePurchaseBatchOrderPos )
            SQL_BUILDER_PUSH_BACK( DeletePurchaseBatchOrderPos )

    END_ACCESSOR_DEFINITION

    //-------------------------------------------------------------------------------------------------//
    // Builder Definition 'SelectPurchaseBatchOrderPosByKey'
    //-------------------------------------------------------------------------------------------------//
    BUILDER_DEFINITION( SelectPurchaseBatchOrderPosByKey )

    bool SelectPurchaseBatchOrderPosByKey::isExecutable() const
    {
        basar::ConstString fun = "SelectPurchaseBatchOrderPosByKey::isExecutable()"; fun;
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;

        bool executable = isContainedAndSet( properties::BRANCHNO.getName() ) 
                       && isContainedAndSet( properties::ORDERNO ) 
                       && isContainedAndSet( properties::ARTICLENO );

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

        return executable;
    }

    void SelectPurchaseBatchOrderPosByKey::buildSQLString()
    {
        basar::ConstString fun = "SelectPurchaseBatchOrderPosByKey::buildSQLString()";
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;
        BLOG_TRACE_METHOD( logger, fun );

		static const basar::VarString sql(  
            "SELECT pbop.articleno AS "                                + properties::ARTICLENO.getName()
                + ", pbop.branchno AS "                                + properties::BRANCHNO.getName()
				+ ", pbop.ordernobatch AS "                            + properties::ORDERNO.getName()
                + ", pbop.ordernobatch AS "                            + properties::ORDERNO_UPDATE.getName()
                + ", pbop.orderproposalqty AS "                        + properties::ORDERPROPOSALQTY.getName()
				+ ", pbop.orderproposaltime AS "                       + properties::ORDERPROPOSALTIME.getName()
				+ ", pbop.orderproposalprocessedtime AS "              + properties::ORDER_PROP_PROCESSED_TIME.getName()
				+ ", pbop.orderproposalno AS "                         + properties::ORDER_PROP_NO.getName()
				+ ", pbop.purchaseorderno AS "                         + properties::PURCHASEORDERNO.getName()
            + " FROM cscpubatchorderpos pbop"
            + " WHERE pbop.branchno = "                                + properties::BRANCHNO.toSQLReplacementString()
            + " AND pbop.ordernobatch = "                              + properties::ORDERNO.toSQLReplacementString()
            + " AND pbop.articleno = "                                 + properties::ARTICLENO.toSQLReplacementString()
            + " AND pbop.ordertypebatch = 1"
		);
        resolve( sql );

        BLOG_TRACE_SQLSTRING( logger );
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( UpdatePurchaseBatchOrderPos )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( UpdatePurchaseBatchOrderPos )

    bool UpdatePurchaseBatchOrderPos::isExecutable() const
    {
        METHODNAME_DEF( UpdatePurchaseBatchOrderPos, isExecutable );
        static const log4cplus::Logger& logger = libabbauw::LoggerPool::loggerOrderProposal;
        BLOG_TRACE_METHOD( logger, fun );

        const bool executable = are1stPropertiesSetAnd2ndOneInState( properties::BRANCHNO.toPropertyString()       +
                                                                     properties::ORDERNO.toPropertyString()        +
                                                                     properties::ORDERNO_UPDATE.toPropertyString() +
                                                                     properties::ARTICLENO.toPropertyString(),
                                                                     properties::ORDERNO_UPDATE.toPropertyString(),
                                                                     basar::SS_UPDATE );

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
        return executable;
    }

    void UpdatePurchaseBatchOrderPos::buildSQLString()
    {
		METHODNAME_DEF( UpdatePurchaseBatchOrderPos, buildSQLString ) 
		BLOG_TRACE_METHOD( LoggerPool::loggerOrderProposal, fun );

		static const basar::VarString sql(
			"UPDATE cscpubatchorderpos SET "
            " ordernobatch = "                  + properties::ORDERNO_UPDATE.toSQLReplacementString() +
            " WHERE branchno = "                + properties::BRANCHNO.toSQLReplacementString()       +
            " AND ordernobatch = "              + properties::ORDERNO.toSQLReplacementString()        +
            " AND articleno = "                 + properties::ARTICLENO.toSQLReplacementString()      +
            " AND ordertypebatch = 1"
		);
		
    	resolve( sql );
        BLOG_TRACE_SQLSTRING( LoggerPool::loggerOrderProposal );
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( DeletePurchaseBatchOrderPos )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( DeletePurchaseBatchOrderPos )

    bool DeletePurchaseBatchOrderPos::isExecutable() const
    {
        METHODNAME_DEF( DeletePurchaseBatchOrderPos, isExecutable );
        static const log4cplus::Logger& logger = libabbauw::LoggerPool::loggerOrderProposal;
        BLOG_TRACE_METHOD( logger, fun );

        bool executable = arePropertiesSet( properties::BRANCHNO.toPropertyString()            +
                                            properties::ORDERNO.toPropertyString()             +
                                            properties::ARTICLENO.toPropertyString()
                                          );

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
        return executable;
    }

    void DeletePurchaseBatchOrderPos::buildSQLString()
    {
		METHODNAME_DEF( DeletePurchaseBatchOrderPos, buildSQLString ) 
		BLOG_TRACE_METHOD( LoggerPool::loggerOrderProposal, fun );

		static const basar::VarString sql(
			"DELETE FROM cscpubatchorderpos "
            " WHERE branchno = "                + properties::BRANCHNO.toSQLReplacementString()       +
            " AND ordernobatch = "              + properties::ORDERNO.toSQLReplacementString()        +
            " AND articleno = "                 + properties::ARTICLENO.toSQLReplacementString()      +
            " AND ordertypebatch = 1"
		);
		
    	resolve( sql );
        BLOG_TRACE_SQLSTRING( LoggerPool::loggerOrderProposal );
    }

} // end namespace purchaseBatchOrder
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw
