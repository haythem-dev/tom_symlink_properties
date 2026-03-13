//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/accessor.h>
#include <libutil/util.h>

#include "orderproposalacc.h"
#include "orderproposalacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

namespace libabbauw
{
namespace infrastructure
{
namespace accessor
{
namespace orderProposal
{
    BEGIN_ACCESSOR_DEFINITION( lit::ACC_ORDERPROPOSAL )
        BEGIN_PROPERTY_DESCRIPTION_LIST
			PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCHNO                  );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNO                   );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERNO_UPDATE            );
            PROPERTY_DESCRIPTION_LIST_ADD( properties::POSNO                     );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLENO                 );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERPROPOSALQTY          );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDERPROPOSALTIME         );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::TRANSFER_TIME             );
			PROPERTY_DESCRIPTION_LIST_ADD( properties::ORDER_PROP_NO             );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( lit::SELECT_ORDER_PROPOSAL_BY_KEY );
			SQL_BUILDER_CREATE   ( SelectOrderProposalByKey )
			SQL_BUILDER_PUSH_BACK( SelectOrderProposalByKey )

		ACCESS_METHOD( lit::DELETE_ORDER_PROPOSAL );
			SQL_BUILDER_CREATE   ( DeleteOrderProposal )
			SQL_BUILDER_PUSH_BACK( DeleteOrderProposal )

		ACCESS_METHOD( lit::SAVE_ORDER_PROPOSAL );
			SQL_BUILDER_CREATE   ( InsertOrderProposal )
			SQL_BUILDER_PUSH_BACK( InsertOrderProposal )
            SQL_BUILDER_CREATE   ( UpdateOrderProposal )
			SQL_BUILDER_PUSH_BACK( UpdateOrderProposal )

    END_ACCESSOR_DEFINITION

     //////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( SelectOrderProposalByKey )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( SelectOrderProposalByKey )

	ENSURE_PROPERTIES_ARE_SET( SelectOrderProposalByKey,
		properties::BRANCHNO.toPropertyString()            + 
		properties::ORDERNO.toPropertyString()             +
		properties::POSNO.toPropertyString()
	);

    void SelectOrderProposalByKey::buildSQLString()
    {
        METHODNAME_DEF( SelectOrderProposalByKey, buildSQLString ) 
		BLOG_TRACE_METHOD( LoggerPool::loggerOrderProposal, fun );

		static const basar::VarString sql(  
            "SELECT op.articleno AS "                                  + properties::ARTICLENO.getName()
                + ", op.branchno AS "                                  + properties::BRANCHNO.getName()
				+ ", op.ordernobatch AS "                              + properties::ORDERNO.getName()
                + ", op.ordernobatch AS "                              + properties::ORDERNO_UPDATE.getName()
				+ ", op.posnobatch AS "                                + properties::POSNO.getName()
                + ", op.orderproposalqty AS "                          + properties::ORDERPROPOSALQTY.getName()
				+ ", op.orderproposaltime AS "                         + properties::ORDERPROPOSALTIME.getName()
				+ ", op.transfertime AS "                              + properties::TRANSFER_TIME.getName()
				+ ", op.orderproposalno AS "                           + properties::ORDER_PROP_NO.getName()
            + " FROM orderproposal op"
            + " WHERE op.branchno = "                                  + properties::BRANCHNO.toSQLReplacementString()
            + " AND op.ordernobatch = "                                + properties::ORDERNO.toSQLReplacementString()
            + " AND op.posnobatch = "                                  + properties::POSNO.toSQLReplacementString()
            + " AND op.ordertypebatch = 1"
		);
		
    	resolve( sql );
		BLOG_TRACE_SQLSTRING( LoggerPool::loggerOrderProposal );
    }

	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( DeleteOrderProposal )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( DeleteOrderProposal )

    bool DeleteOrderProposal::isExecutable() const
    {
        METHODNAME_DEF( DeleteOrderProposal, isExecutable ) 
        const static log4cplus::Logger& logger = LoggerPool::loggerTransferOrder;

        bool executable = arePropertiesSet( properties::BRANCHNO.toPropertyString()            +
                                            properties::ORDERNO.toPropertyString()             +
                                            properties::POSNO.toPropertyString()               +
                                            properties::ORDERPROPOSALTIME.toPropertyString()
                                          );

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

        return executable;
    }

    void DeleteOrderProposal::buildSQLString()
    {
        METHODNAME_DEF( DeleteOrderProposal, buildSQLString ) 
		BLOG_TRACE_METHOD( LoggerPool::loggerOrderProposal, fun );

		static const basar::VarString sql(  
            "DELETE FROM orderproposal WHERE branchno = "              + properties::BRANCHNO.toSQLReplacementString()          +
			" AND ordernobatch = "                                     + properties::ORDERNO.toSQLReplacementString()           +
			" AND posnobatch = "                                       + properties::POSNO.toSQLReplacementString()             +
			" AND orderproposaltime = '"                               + properties::ORDERPROPOSALTIME.toSQLReplacementString() +
            "' AND ordertypebatch = 1 "
		);
		
    	resolve( sql );
		BLOG_TRACE_SQLSTRING( LoggerPool::loggerOrderProposal );
    }

	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( InsertOrderProposal )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( InsertOrderProposal )

    bool InsertOrderProposal::isExecutable() const
    {
        METHODNAME_DEF( InsertOrderProposal, isExecutable );
        static const log4cplus::Logger& logger = libabbauw::LoggerPool::loggerOrderProposal;
        BLOG_TRACE_METHOD( logger, fun );

        const bool executable = are1stPropertiesSetAnd2ndOneInState( properties::BRANCHNO.toPropertyString()       +
                                                                     properties::ORDERNO.toPropertyString()        +
                                                                     properties::ARTICLENO.toPropertyString()      +
                                                                     properties::POSNO.toPropertyString(),
                                                                     properties::ORDERPROPOSALQTY.toPropertyString(),
                                                                     basar::SS_INSERT );

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
        return executable;
    }

    void InsertOrderProposal::buildSQLString()
    {
		METHODNAME_DEF( InsertOrderProposal, buildSQLString ) 
		BLOG_TRACE_METHOD( LoggerPool::loggerOrderProposal, fun );

		static const basar::VarString sql(
			"INSERT INTO orderproposal(branchno, articleno, orderproposalqty, ordernobatch, "
			            "posnobatch, ordertypebatch, orderproposaltime)"
			" VALUES("
			" '"                                + properties::BRANCHNO.toSQLReplacementString()                +   
			"', '"                              + properties::ARTICLENO.toSQLReplacementString()               +
			"', '"                              + properties::ORDERPROPOSALQTY.toSQLReplacementString()        +
			"', '"                              + properties::ORDERNO.toSQLReplacementString()                 +
			"', '"                              + properties::POSNO.toSQLReplacementString()                   +
			"', '1'"
			", CURRENT )"
		);
		
    	resolve( sql );
        BLOG_TRACE_SQLSTRING( LoggerPool::loggerOrderProposal );
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( UpdateOrderProposal )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( UpdateOrderProposal )

    bool UpdateOrderProposal::isExecutable() const
    {
        METHODNAME_DEF( UpdateOrderProposal, isExecutable );
        static const log4cplus::Logger& logger = libabbauw::LoggerPool::loggerOrderProposal;
        BLOG_TRACE_METHOD( logger, fun );

        const bool executable = are1stPropertiesSetAnd2ndOneInState( properties::BRANCHNO.toPropertyString()       +
                                                                     properties::ORDERNO.toPropertyString()        +
                                                                     properties::ORDERNO_UPDATE.toPropertyString() +
                                                                     properties::POSNO.toPropertyString(),
                                                                     properties::ORDERNO_UPDATE.toPropertyString(),
                                                                     basar::SS_UPDATE );

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
        return executable;
    }

    void UpdateOrderProposal::buildSQLString()
    {
		METHODNAME_DEF( UpdateOrderProposal, buildSQLString ) 
		BLOG_TRACE_METHOD( LoggerPool::loggerOrderProposal, fun );

		static const basar::VarString sql(
			"UPDATE orderproposal SET "
            " ordernobatch = "                  + properties::ORDERNO_UPDATE.toSQLReplacementString() +
            " WHERE branchno = "                + properties::BRANCHNO.toSQLReplacementString()       +
            " AND ordernobatch = "              + properties::ORDERNO.toSQLReplacementString()        +
            " AND posnobatch = "                + properties::POSNO.toSQLReplacementString()
		);
		
    	resolve( sql );
        BLOG_TRACE_SQLSTRING( LoggerPool::loggerOrderProposal );
    }

} // end namespace orderProposal
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw
