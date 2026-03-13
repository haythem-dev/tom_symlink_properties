//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmnaccessor.h"
#include "articlecollectionacc.h"
#include "articlecollectiondm_definitions.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace acc_article
{
	using namespace libabbauw::properties;
//---------------------------------------------------------------------------------------------//
// accessor definition 
//---------------------------------------------------------------------------------------------//
	BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::article::lit::ACC_ARTICLE )
        BEGIN_PROPERTY_DESCRIPTION_LIST
            PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENO                );
			PROPERTY_DESCRIPTION_LIST_ADD( PZN			            );
			PROPERTY_DESCRIPTION_LIST_ADD( EAN			            );
			PROPERTY_DESCRIPTION_LIST_ADD( PHARMACEUTICALFORM       );
			PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENAME				);
            PROPERTY_DESCRIPTION_LIST_ADD( MATCHCODE				);
			PROPERTY_DESCRIPTION_LIST_ADD( NORMPACK					);
			PROPERTY_DESCRIPTION_LIST_ADD( UNIT						);
            PROPERTY_DESCRIPTION_LIST_ADD( ARTICLE_CLASS_ATTRIBUTES );
            PROPERTY_DESCRIPTION_LIST_ADD( ARTICLE_ATTRIBUTES       );
            PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTURERNO           );
            PROPERTY_DESCRIPTION_LIST_ADD( PURCHASEORDERSUPPLIERNO  );
            PROPERTY_DESCRIPTION_LIST_ADD( AGP                      );
            PROPERTY_DESCRIPTION_LIST_ADD( GEP                      );
        END_PROPERTY_DESCRIPTION_LIST

        ACCESS_METHOD( libabbauw::domMod::article::lit::SELECT_ARTICLE );
			SQL_BUILDER_CREATE(    SelectArticle                   )
			SQL_BUILDER_PUSH_BACK( SelectArticle                   )

    END_ACCESSOR_DEFINITION


	//////////////////////////////////////////////////////////////////////////////////////////
    // BUILDER_DEFINITION( SelectArticle )
    //////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION( SelectArticle )

	bool SelectArticle::isExecutable() const
	{ 
		return arePropertiesSet( BRANCHNO.toPropertyString() );
	}

    void SelectArticle::buildSQLString()
    {
        using namespace libabbauw::properties;
		
		METHODNAME_DEF( SelectArticle, buildSQLString ) 
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerArticle, fun );

        /////////////////////////////////////////////////////////////////////////////////////////////////
        // const attributes
        /////////////////////////////////////////////////////////////////////////////////////////////////
		
        static const basar::VarString sql( "\n"
             "SELECT az.artikel_nr AS "                            + ARTICLENO.getName()
                + ", acean.article_code AS "                       + EAN.getName()
				+ ", acpzn.article_code AS "                       + PZN.getName()
				+ ", az.darreichform AS "                          + PHARMACEUTICALFORM.getName()
				+ ", az.artikel_name AS "                          + ARTICLENAME.getName()
                + ", az.artikel_mc AS "                            + MATCHCODE.getName()
				+ ", az.normpackung AS "                           + NORMPACK.getName()
				+ ", TRIM(az.einheit) AS "                         + UNIT.getName()
                + ", az.hersteller_nr AS "                         + MANUFACTURERNO.getName()
                + ", az.etartklasse1 AS "                          + ARTICLE_CLASS_ATTRIBUTES.getName()
                + ", al.etartschalter1 AS "                        + ARTICLE_ATTRIBUTES.getName()
                + ", af.besla_nr AS "                              + PURCHASEORDERSUPPLIERNO.getName()
                + ", ap.preisekgrosso AS "                           + GEP.getName()
                + ", ap.preisagp AS "                              + AGP.getName()
			    + "\n FROM artikelzentral az \n"
                + " LEFT JOIN artikellokal al ON az.artikel_nr = al.artikel_nr \n"
                + " LEFT JOIN cscbrancharticle af ON al.artikel_nr = af.artikel_nr AND al.filialnr = af.filialnr \n"
			    + " LEFT JOIN articlecodes acean ON az.artikel_nr = acean.articleno AND acean.code_type = 13 \n"
			    + " LEFT JOIN artikelpreis ap on az.artikel_nr = ap.artikel_nr \n"
				+ " JOIN articlecodes acpzn ON az.artikel_nr = acpzn.articleno AND acpzn.preferred_flag = 1 \n"
                + " WHERE al.filialnr = "                          + BRANCHNO.toSQLReplacementString()
		);

		basar::VarString whereClause;
		if( isContainedAndSet( MATCHCODE.getName() ) )
        {
			whereClause.append( " AND az.artikel_mc LIKE UPPER("   + MATCHCODE.toSQLReplacementString("'", "%')")    );
        }
        else if( isContainedAndSet( PZN.getName() ) )
        {
			whereClause.append( " AND acpzn.article_code = LTRIM(" + PZN.toSQLReplacementString("'", "'") + ", '0' )" );
        }
        else if( isContainedAndSet( EAN.getName() ) )
        {
			whereClause.append( " AND acean.article_code = "       + EAN.toSQLReplacementString("'", "'") );
        }

        if( isContainedAndSet( MANUFACTURERNO.getName() ) )
        {
            whereClause.append( " AND ( az.hersteller_nr = "       + MANUFACTURERNO.toSQLReplacementString("'", "'") );

            if( isContainedAndSet( PURCHASEORDERSUPPLIERNO.getName() ) )
            {
                whereClause.append( " OR af.besla_nr = "           + PURCHASEORDERSUPPLIERNO.toSQLReplacementString() + " )" );
            }
            else
            {
                whereClause.append( ")" );
            }
        }

    	resolve( sql + whereClause );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerArticle );
    }

} // end namespace acc_article
} // end namespace libabbauw
