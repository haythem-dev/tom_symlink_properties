#include "pubatchorderposacc.h"
#include <libbasardbaspect.h>
#include "loggerpool.h"
#include "properties_definitions.h"
#include "pubatchorderposdm_definitions.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include <libbasarproperty_propertydescriptionlistref.h>

#include <libbasardbaspect_accessorpropertytable.h>

#include <libbasarproperty.h>
#include <libbasar_definitions.h>
#include <libbasarcmnutil_parameterlist.h> 

namespace acc_pubatchorderpos {

    using namespace properties;

    BEGIN_ACCESSOR_DEFINITION( pubatchorderposdm::lit::ACC_PUBATCHORDERPOS )
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
            PROPERTY_DESCRIPTION_LIST_ADD( EXPECTED_GI_DATE);
        END_PROPERTY_DESCRIPTION_LIST

		ACCESS_METHOD( pubatchorderposdm::lit::SELECT_PU_BATCH_ORDER_POSITIONS );
            SQL_BUILDER_CREATE( SelectPuBatchOrderPositions )
            SQL_BUILDER_PUSH_BACK( SelectPuBatchOrderPositions )

		ACCESS_METHOD( pubatchorderposdm::lit::INSERT_PU_BATCH_ORDER_POSITIONS );
            SQL_BUILDER_CREATE( InsertPuBatchOrderPositions )
            SQL_BUILDER_PUSH_BACK( InsertPuBatchOrderPositions )
    END_ACCESSOR_DEFINITION


	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( SelectPuBatchOrderPositions )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( SelectPuBatchOrderPositions )

	DONT_CHECK_PROPERTIES(SelectPuBatchOrderPositions)
  
    void SelectPuBatchOrderPositions::buildSQLString()
    {
        using namespace properties;

		METHODNAME_DEF( SelectPuBatchOrderPositions, buildSQLString ) 
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // const attributes
        /////////////////////////////////////////////////////////////////////////////////////////////////		
        static const basar::VarString sql(  
            "SELECT " 
			+		BRANCH_NO.getName()+ ", "
			+		ORDER_PROPOSAL_TIME.getName()+ ", "
            +		ORDER_NO_BATCH.getName()+ ", "
            +		ORDER_PROP_PROCESSED_TIME.getName()+ ", " 
            +		PURCHASE_ORDER_NO.getName()+ ", " 
            +		DATE_OF_PURCHASE.getName() + ", "
			+		ORDER_PROP_DISCLAIM_NOTE.getName()+ ", "
			+		REASON_ID.getName() + ", "
			+		ORDER_PROPOSAL_NO.getName() + ", "
            +       EXPECTED_GI_DATE.getName()
            + " FROM  pubatchorderpos"
		);

    	resolve( sql );
        BLOG_DEBUG( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
    }

	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( InsertPuBatchOrderPositions )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( InsertPuBatchOrderPositions )

	ENSURE_PROPERTIES_ARE_SET(
		InsertPuBatchOrderPositions,
		BRANCH_NO.toPropertyString() + ARTICLE_NO.toPropertyString() + ORDER_NO_BATCH.toPropertyString() + ORDER_PROPOSAL_QTY.toPropertyString() 
		+ ORDER_PROPOSAL_TIME.toPropertyString()
	)

    void InsertPuBatchOrderPositions::buildSQLString()
    {
        using namespace properties;

		METHODNAME_DEF( InsertPuBatchOrderPositions, buildSQLString ) 
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // const attributes
        /////////////////////////////////////////////////////////////////////////////////////////////////		
        static const basar::VarString sql(  
            "INSERT INTO pubatchorderpos "
			"("
			+			BRANCH_NO.getName()
			+ ", " +	ARTICLE_NO.getName()
			+ ", " +	ORDER_PROPOSAL_QTY.getName()
			+ ", " +	ORDER_NO_BATCH.getName()
			+ ", ordertypebatch"
			+ ", " +	ORDER_PROPOSAL_TIME.getName()
            + ") VALUES ("
			+			BRANCH_NO.toSQLReplacementString()
			+ ", " +	ARTICLE_NO.toSQLReplacementString()
			+ ", " +	ORDER_PROPOSAL_QTY.toSQLReplacementString()
			+ ", " +	ORDER_NO_BATCH.toSQLReplacementString()
			+ ", 1, " +	ORDER_PROPOSAL_TIME.toSQLReplacementString("'","'")
			+ ")"
		);

    	resolve( sql );
        BLOG_DEBUG( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
    }
} 
