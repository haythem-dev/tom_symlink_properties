//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#include "cmnaccessor.h"
#include "purchaseordersupplieracc.h"
#include "purchaseordersupplierdm_definitions.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace acc_purchaseordersupplier
{
	using namespace libabbauw::properties;
//---------------------------------------------------------------------------------------------//
// accessor definition 
//---------------------------------------------------------------------------------------------//
	BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::purchaseordersupplierdm::ACC_PURCHASEORDERSUPPLIER )
        BEGIN_PROPERTY_DESCRIPTION_LIST
            PROPERTY_DESCRIPTION_LIST_ADD( PURCHASEORDERSUPPLIERNO   );
			PROPERTY_DESCRIPTION_LIST_ADD( PURCHASEORDERSUPPLIERNAME );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( libabbauw::domMod::purchaseordersupplierdm::SELECT_PURCHASEORDERSUPPLIER );
			SQL_BUILDER_CREATE( SelectPurchaseOrderSupplier    )
			SQL_BUILDER_PUSH_BACK( SelectPurchaseOrderSupplier )

    END_ACCESSOR_DEFINITION


	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( SelectPurchaseOrderSupplier )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( SelectPurchaseOrderSupplier )

	bool SelectPurchaseOrderSupplier::isExecutable() const
	{ 
		return arePropertiesSet( PURCHASEORDERSUPPLIERNO.toPropertyString() );
	}

    void SelectPurchaseOrderSupplier::buildSQLString()
    {
        using namespace libabbauw::properties;
		
		METHODNAME_DEF( SelectPurchaseOrderSupplier, buildSQLString ) 
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerPurchaseOrderSupplier, fun );

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // const attributes
        /////////////////////////////////////////////////////////////////////////////////////////////////
        static const basar::VarString sql(  
            "\n SELECT besla_nr AS "                                                         + PURCHASEORDERSUPPLIERNO.getName()
				+ ", besla_name AS "                                                         + PURCHASEORDERSUPPLIERNAME.getName()
			+ "\n FROM pusupplier"
            + "\n WHERE besla_nr = "                                                         + PURCHASEORDERSUPPLIERNO.toSQLReplacementString()
		);

    	resolve( sql );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerPurchaseOrderSupplier );
    }

} // end namespace acc_purchaseordersupplier
} // end namespace libabbauw
