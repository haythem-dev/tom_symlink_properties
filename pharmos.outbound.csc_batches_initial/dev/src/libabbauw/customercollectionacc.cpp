//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmnaccessor.h"
#include "customercollectionacc.h"
#include "customercollectiondm_definitions.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace acc_customercollection
{
	using namespace libabbauw::properties;
//---------------------------------------------------------------------------------------------//
// accessor definition 
//---------------------------------------------------------------------------------------------//
	BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::customer::ACC_CUSTOMER_COLLECTION )
        BEGIN_PROPERTY_DESCRIPTION_LIST
			PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNO     );
			PROPERTY_DESCRIPTION_LIST_ADD( MATCHCODE      );
			PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNAME   );
			PROPERTY_DESCRIPTION_LIST_ADD( CUSTOMER_NAME  );
			PROPERTY_DESCRIPTION_LIST_ADD( LOCATION       );
			PROPERTY_DESCRIPTION_LIST_ADD( POSTCODE       );
			PROPERTY_DESCRIPTION_LIST_ADD( ADDRESS        );
			PROPERTY_DESCRIPTION_LIST_ADD( PHONENO        );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( libabbauw::domMod::customer::SELECT_CUSTOMER_COLLECTION )
			SQL_BUILDER_CREATE( SelectCustomerCollection    );
			SQL_BUILDER_PUSH_BACK( SelectCustomerCollection );

    END_ACCESSOR_DEFINITION


	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( SelectCustomerCollection )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( SelectCustomerCollection )

	bool SelectCustomerCollection::isExecutable() const
	{ 
        return arePropertiesSet( BRANCHNO.toPropertyString() + PHARMACYNO.toPropertyString() ) ||
               arePropertiesSet( BRANCHNO.toPropertyString() + MATCHCODE.toPropertyString() );
	}

    void SelectCustomerCollection::buildSQLString()
    {	
		METHODNAME_DEF( SelectCustomerCollection, buildSQLString ) 
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerCustomer, fun );

        using namespace libabbauw::properties;

        static const basar::VarString sql( "\n"
            "SELECT k.idfnr AS "                                                             + PHARMACYNO.getName()                        +
				", k.matchcode as "                                                          + MATCHCODE.getName()                         +
				", k.nameapo AS "                                                            + PHARMACYNAME.getName()                      +
				", k.nameinhaber AS "                                                        + CUSTOMER_NAME.getName()                     +
				", k.ort AS "                                                                + LOCATION.getName()                          +
				", k.postleitzahl AS "                                                       + POSTCODE.getName()                          +
				", k.strasse AS "                                                            + ADDRESS.getName()                           +
				", k.telnr AS "                                                              + PHONENO.getName()                           +
			"\n FROM kunde k \n"
            " LEFT JOIN kundeschalter ks ON k.idfnr = ks.idfnr AND k.filialnr = ks.vertriebszentrumnr \n"
            " WHERE k.filialnr = "                                                           + BRANCHNO.toSQLReplacementString()           +
            " AND ks.skdkundeaegesperrt = 0"
        );
        
        basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
        basar::VarString whereClause;

        if( yit.isContainedAndSet( properties::MATCHCODE ) )
        {
			whereClause = " AND k.matchcode LIKE UPPER("                                     + MATCHCODE.toSQLReplacementString("'","%')");
        }

        if( yit.isContainedAndSet( properties::PHARMACYNO ) )
        {
		    whereClause = " AND k.idfnr = "                                                  + PHARMACYNO.toSQLReplacementString();
        }
		
        static const basar::VarString orderStmt( "\n ORDER BY k.idfnr ASC" );

    	resolve( sql + whereClause + orderStmt );
		BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerCustomer );
    }
} // end namespace acc_customercollection
} // end namespace libabbauw
