//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmnaccessor.h"
#include "customeracc.h"
#include "customerdm_definitions.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace acc_customer
{
	using namespace libabbauw::properties;
//---------------------------------------------------------------------------------------------//
// accessor definition 
//---------------------------------------------------------------------------------------------//
	BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::customer::ACC_CUSTOMER )
        BEGIN_PROPERTY_DESCRIPTION_LIST
			PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNO     );
			PROPERTY_DESCRIPTION_LIST_ADD( MATCHCODE      );
			PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNAME   );
			PROPERTY_DESCRIPTION_LIST_ADD( CUSTOMER_NAME  );
			PROPERTY_DESCRIPTION_LIST_ADD( LOCATION       );
			PROPERTY_DESCRIPTION_LIST_ADD( POSTCODE       );
			PROPERTY_DESCRIPTION_LIST_ADD( ADDRESS        );
			PROPERTY_DESCRIPTION_LIST_ADD( PHONENO        );
            PROPERTY_DESCRIPTION_LIST_ADD( TELEPHONY_INFO );
            PROPERTY_DESCRIPTION_LIST_ADD( CUSTOMERLOCKED );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( libabbauw::domMod::customer::SELECT_CUSTOMER )
			SQL_BUILDER_CREATE( SelectCustomer    );
			SQL_BUILDER_PUSH_BACK( SelectCustomer );

    END_ACCESSOR_DEFINITION


	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( SelectCustomer )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( SelectCustomer )

	bool SelectCustomer::isExecutable() const
	{ 
        return arePropertiesSet( BRANCHNO.toPropertyString() + PHARMACYNO.toPropertyString() );
	}

    void SelectCustomer::buildSQLString()
    {	
		METHODNAME_DEF( SelectCustomer, buildSQLString ) 
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
                ", k.telkurzwahl AS "                                                        + SPEED_DIAL_NO.getName()                     +
                ", k.kundetext AS "                                                          + TELEPHONY_INFO.getName()                    +
                ", CAST(ks.skdkundeaegesperrt AS SMALLINT) AS "                              + CUSTOMERLOCKED.getName()                    +
			"\n FROM kunde k \n"
            " LEFT JOIN kundeschalter ks ON k.idfnr = ks.idfnr AND k.filialnr = ks.vertriebszentrumnr \n"
            " WHERE k.filialnr = "                                                           + BRANCHNO.toSQLReplacementString()           +
            " AND k.idfnr = "                                                                + PHARMACYNO.toSQLReplacementString()
        );

        basar::VarString whereClause;
        if( getPropertyList().isContainedAndSet( CUSTOMERLOCKED ) )
            whereClause = " AND ks.skdkundeaegesperrt = 0";

    	resolve( sql + whereClause );
		BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerCustomer );
    }
} // end namespace acc_customer
} // end namespace libabbauw
