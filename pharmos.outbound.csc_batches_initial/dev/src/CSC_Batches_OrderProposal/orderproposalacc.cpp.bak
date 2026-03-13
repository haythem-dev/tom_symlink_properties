#include "orderproposalacc.h"
#include <libbasardbaspect.h>
#include "loggerpool.h"
#include "properties_definitions.h"
#include "orderproposaldm_definitions.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include <libbasarproperty_propertydescriptionlistref.h>

#include <libbasardbaspect_accessorpropertytable.h>

#include <libbasarproperty.h>
#include <libbasar_definitions.h>
#include <libbasarcmnutil_parameterlist.h> 

namespace acc_orderproposal {

    using namespace properties;

    BEGIN_ACCESSOR_DEFINITION( orderproposaldm::lit::ACC_ORDERPROPOSAL )
        BEGIN_PROPERTY_DESCRIPTION_LIST
			PROPERTY_DESCRIPTION_LIST_ADD( BRANCH_NO) //smallint
			PROPERTY_DESCRIPTION_LIST_ADD( ARTICLE_NO);
			PROPERTY_DESCRIPTION_LIST_ADD( ORDER_PROPOSAL_QTY);
			PROPERTY_DESCRIPTION_LIST_ADD( ORDER_NO_BATCH);
			PROPERTY_DESCRIPTION_LIST_ADD( REASON_ID);
			PROPERTY_DESCRIPTION_LIST_ADD( ORDER_PROPOSAL_TIME);
			PROPERTY_DESCRIPTION_LIST_ADD( ORDER_PROP_PROCESSED_TIME);
			PROPERTY_DESCRIPTION_LIST_ADD( ORDER_PROP_DISCLAIM_NOTE);
			PROPERTY_DESCRIPTION_LIST_ADD( ORDER_PROPOSAL_NO); 
			PROPERTY_DESCRIPTION_LIST_ADD( PURCHASE_ORDER_NO);
			PROPERTY_DESCRIPTION_LIST_ADD( DATE_OF_PURCHASE);
			PROPERTY_DESCRIPTION_LIST_ADD( TRANSFER_TIME);
			PROPERTY_DESCRIPTION_LIST_ADD( POS_NO_BATCH);
			PROPERTY_DESCRIPTION_LIST_ADD( EXPECTED_GI_DATE);
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( orderproposaldm::lit::SELECT_ORDER_PROPOSALS );
            SQL_BUILDER_CREATE( SelectOrderProposals )
            SQL_BUILDER_PUSH_BACK( SelectOrderProposals )

        ACCESS_METHOD( orderproposaldm::lit::UPDATE_TRANSFER_DATE );
            SQL_BUILDER_CREATE( UpdateTransferDate )
            SQL_BUILDER_PUSH_BACK( UpdateTransferDate )

        ACCESS_METHOD( orderproposaldm::lit::UPDATE_ORDER_PROPOSALS );
            SQL_BUILDER_CREATE( UpdateOrderProposals )
            SQL_BUILDER_PUSH_BACK( UpdateOrderProposals )

        ACCESS_METHOD( orderproposaldm::lit::UPDATE_ORDER_PROPOSAL_COMPLETED_FLAG )
            SQL_BUILDER_CREATE( UpdateOrderProposalCompletedFlag )
            SQL_BUILDER_PUSH_BACK( UpdateOrderProposalCompletedFlag )

    END_ACCESSOR_DEFINITION


	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( SelectOrderProposals )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( SelectOrderProposals )

    DONT_CHECK_PROPERTIES(SelectOrderProposals)

    void SelectOrderProposals::buildSQLString()
    {
        using namespace properties;
		
		METHODNAME_DEF( SelectOrderProposals, buildSQLString ) 
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // const attributes
        /////////////////////////////////////////////////////////////////////////////////////////////////
		//Get all important values but skip the rows which are already transfered or do have a quantity of 0
        static const basar::VarString sql(  
            "SELECT " 
			+			BRANCH_NO.getName()
			+ ", " +	ARTICLE_NO.getName()
			+ ", " +	ORDER_PROPOSAL_QTY.getName()
			+ ", " +	ORDER_NO_BATCH.getName()
			+ ", " +	POS_NO_BATCH.getName()
			+ ", " +	ORDER_PROPOSAL_TIME.getName()
			+ " FROM orderproposal"
			+ " WHERE ordertypebatch = 1 AND " 
			//+			"transfertime = '" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3")+"'"
			+			"transfertime = '" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S.%F3")+"'"
			+ " AND " + ORDER_PROPOSAL_QTY.getName() + ">0"
		);

    	resolve( sql );

        BLOG_DEBUG( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
    }

	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( UpdateTransferDate )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( UpdateTransferDate )

	ENSURE_PROPERTIES_ARE_SET(
		UpdateTransferDate,
		TRANSFER_TIME.toPropertyString()+BRANCH_NO.toPropertyString()+ORDER_PROPOSAL_TIME.toPropertyString()+ORDER_NO_BATCH.toPropertyString()+POS_NO_BATCH.toPropertyString()
	)
  
    void UpdateTransferDate::buildSQLString()
    {
        using namespace properties;

		METHODNAME_DEF( UpdateTransferDate, buildSQLString ) 
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // const attributes
        /////////////////////////////////////////////////////////////////////////////////////////////////

        static const basar::VarString sql(  
            "UPDATE orderproposal SET " 
			+			TRANSFER_TIME.getName() + "='" + TRANSFER_TIME.toSQLReplacementString() + "'"
			+ " WHERE "
			+			BRANCH_NO.getName()				+ "=" + BRANCH_NO.toSQLReplacementString()
			+ " AND " + ORDER_PROPOSAL_TIME.getName()	+ "=" + ORDER_PROPOSAL_TIME.toSQLReplacementString("'","'")
			+ " AND " + ORDER_NO_BATCH.getName()		+ "=" + ORDER_NO_BATCH.toSQLReplacementString()
			+ " AND " + POS_NO_BATCH.getName()			+ "=" + POS_NO_BATCH.toSQLReplacementString()
		);
    	resolve( sql );

        BLOG_DEBUG( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
    }


	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( UpdateOrderProposals )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( UpdateOrderProposals )

	ENSURE_PROPERTIES_ARE_SET(
		UpdateOrderProposals,
		BRANCH_NO.toPropertyString() + ORDER_PROPOSAL_TIME.toPropertyString() + ORDER_NO_BATCH.toPropertyString() 
		+ ORDER_PROP_PROCESSED_TIME.toPropertyString() + PURCHASE_ORDER_NO.toPropertyString() + DATE_OF_PURCHASE.toPropertyString() 
		+ ORDER_PROP_DISCLAIM_NOTE.toPropertyString() + REASON_ID.toPropertyString() + ORDER_PROPOSAL_NO.toPropertyString()
	)

    void UpdateOrderProposals::buildSQLString()
    {
        using namespace properties;

		METHODNAME_DEF( UpdateDataFromPuBatchOrderPos, buildSQLString ) 
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // const attributes
        /////////////////////////////////////////////////////////////////////////////////////////////////		

		//Update the Data from pubatchoderpos but only change Transfertimebi when dateofpurchase or purchaseorderno will be updated
		static const basar::VarString sql(
            "UPDATE orderproposal SET "
			+  TRANSFER_TIME_BI.getName() + "="
			" (CASE"
				" WHEN " + PURCHASE_ORDER_NO.getName() + "!=" + PURCHASE_ORDER_NO.toSQLReplacementString()
				+ " OR "
				+ DATE_OF_PURCHASE.getName() + "!=" + DATE_OF_PURCHASE.toSQLReplacementString()
				+ " THEN CAST('9999-12-31 23:59:59.000' AS datetime year to fraction(3))" 
				+ " ELSE " + TRANSFER_TIME_BI.getName()
			+ " END) "
			+ "," +		ORDER_PROP_PROCESSED_TIME.getName()	+ "=" + ORDER_PROP_PROCESSED_TIME.toSQLReplacementString("'","'")
			+ "," +		PURCHASE_ORDER_NO.getName()         + "=" + PURCHASE_ORDER_NO.toSQLReplacementString()
			+ "," +		DATE_OF_PURCHASE.getName()          + "=" + DATE_OF_PURCHASE.toSQLReplacementString()
			+ "," +		ORDER_PROP_DISCLAIM_NOTE.getName()  + "=" + ORDER_PROP_DISCLAIM_NOTE.toSQLReplacementString("'","'")
			+ "," +		REASON_ID.getName()					+ "=" + REASON_ID.toSQLReplacementString()
			+ "," +		ORDER_PROPOSAL_NO.getName()			+ "=" + ORDER_PROPOSAL_NO.toSQLReplacementString()
            + "," +     EXPECTED_GI_DATE.getName()          + "=" + EXPECTED_GI_DATE.toSQLReplacementString()
            + " WHERE "
			+			BRANCH_NO.getName()				+ " = " + BRANCH_NO.toSQLReplacementString()
			+ " AND " + ORDER_PROPOSAL_TIME.getName()	+ " = " + ORDER_PROPOSAL_TIME.toSQLReplacementString("'","'")
			+ " AND " + ORDER_NO_BATCH.getName()		+ " = " + ORDER_NO_BATCH.toSQLReplacementString() 
			+ " AND " + ORDER_PROPOSAL_QTY.getName()	+ " > 0"
		);
		
    	resolve( sql );
	
        BLOG_DEBUG( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
    }

	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( UpdateOrderProposalCompletedFlag )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( UpdateOrderProposalCompletedFlag )

	DONT_CHECK_PROPERTIES( UpdateOrderProposalCompletedFlag )

    void UpdateOrderProposalCompletedFlag::buildSQLString()
    {
		METHODNAME_DEF( UpdateOrderProposalCompletedFlag, buildSQLString ) 
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // const attributes
        /////////////////////////////////////////////////////////////////////////////////////////////////		

		//Update the Data from pubatchoderpos but only change Transfertimebi when dateofpurchase or purchaseorderno will be updated
		static const basar::VarString sql(
            "UPDATE orderproposal opp"
			" SET opp.completed = 1"
            " WHERE (SELECT bk.status_akt FROM beskopf bk WHERE opp.branchno = bk.filialnr AND opp.purchaseorderno = bk.bestell_nr) IN ('D', 'F')"
		);
		
    	resolve( sql );
	
        BLOG_DEBUG( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
    }
} 
