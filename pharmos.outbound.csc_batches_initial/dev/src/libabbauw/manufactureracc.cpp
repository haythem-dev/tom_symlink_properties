//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief 
 *  \author Hendrik Polzin 
 *  \date 15.08.2013
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#include "cmnaccessor.h"
#include "manufactureracc.h"
#include "manufacturerdm_definitions.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace acc_manufacturer
{
	using namespace libabbauw::properties;
//---------------------------------------------------------------------------------------------//
// accessor definition 
//---------------------------------------------------------------------------------------------//
	BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::manufacturerdm::lit::ACC_MANUFACTURER )
        BEGIN_PROPERTY_DESCRIPTION_LIST
			PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTURERNO   );
			PROPERTY_DESCRIPTION_LIST_ADD( MATCHCODE        );
			PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTURERNAME );
			PROPERTY_DESCRIPTION_LIST_ADD( LOCATION         );
			PROPERTY_DESCRIPTION_LIST_ADD( POSTCODE         );
			PROPERTY_DESCRIPTION_LIST_ADD( ADDRESS          );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( libabbauw::domMod::manufacturerdm::lit::SELECT_MANUFACTURER );
			SQL_BUILDER_CREATE( SelectManufacturer    )
			SQL_BUILDER_PUSH_BACK( SelectManufacturer )

    END_ACCESSOR_DEFINITION


	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( SelectManufacturer )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( SelectManufacturer )

	bool SelectManufacturer::isExecutable() const
	{ 
		return arePropertiesSet( MANUFACTURERNO.toPropertyString() ) || arePropertiesSet( MATCHCODE.toPropertyString() );
	}

    void SelectManufacturer::buildSQLString()
    {
        using namespace libabbauw::properties;
		
		METHODNAME_DEF( SelectManufacturer, buildSQLString ) 
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerManufacturer, fun );

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // const attributes
        /////////////////////////////////////////////////////////////////////////////////////////////////
		//Get all important values but skip the rows which are already transfered or do have a quantity of 0
        static const basar::VarString sql(  
            "SELECT hersteller_nr AS "                                                       + MANUFACTURERNO.getName()
				+ ", sortiername AS "                                                        + MATCHCODE.getName()
				+ ", name AS "                                                               + MANUFACTURERNAME.getName()
				+ ", ort AS "                                                                + LOCATION.getName()
				+ ", plz AS "                                                                + POSTCODE.getName()
				+ ", strasse AS "                                                            + ADDRESS.getName()
			+ " FROM manufacturer"
			+ " WHERE"
		);

    	basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();
        basar::VarString whereClause;

        if( yit.isContainedAndSet( properties::MATCHCODE ) )
        {
			whereClause = " sortiername LIKE UPPER("                                         + MATCHCODE.toSQLReplacementString("'","%')");
        }

        if( yit.isContainedAndSet( properties::MANUFACTURERNO ) )
        {
            if( ! whereClause.empty() )
            {
                whereClause.append( " AND" );
            }
		    whereClause = " hersteller_nr = "                                                + MANUFACTURERNO.toSQLReplacementString();
        }
		
        static const basar::VarString orderStmt( " ORDER BY hersteller_nr ASC" );

    	resolve( sql + whereClause + orderStmt );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerManufacturer );
    }

} // end namespace acc_manufacturer
} // end namespace libabbauw
