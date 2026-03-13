#include "cmnaccessor.h"

#include "articleconsultantacc.h"

#include "articleconsultantdm_definitions.h"
#include <libbasarcmnutil_logging.h>

namespace libabbauw
{
namespace acc_articleconsultant
{
	using namespace libabbauw::properties;
	using basar::VarString;

BEGIN_ACCESSOR_DEFINITION( domMod::article::ACC_ARTICLECONSULTANT )
	
	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO                );
        PROPERTY_DESCRIPTION_LIST_ADD( USER                    );
        PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENO               );
        PROPERTY_DESCRIPTION_LIST_ADD( ARTICLEPRICE            );

	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD( domMod::article::SELECT_ARTICLECONSULTANT )
		SQL_BUILDER_CREATE(    SelectPriceForArticleNo )
		SQL_BUILDER_PUSH_BACK( SelectPriceForArticleNo )


END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelectPriceForArticleNo )

    bool SelectPriceForArticleNo::isExecutable() const
    {

        static const basar::VarString propertiesMustSet( properties::ARTICLENO.toPropertyString() );

        return getPropertyList().arePropertiesSet( propertiesMustSet );
    }

	void SelectPriceForArticleNo::buildSQLString()
	{
		static const VarString sqlStatement( "\n"
            "SELECT p.artikel_nr AS "                                                        + ARTICLENO.getName()                +
            ", p.preisekgrosso AS "                                                          + ARTICLEPRICE.getName()             +
            "\nFROM   artikelpreis p "
            "\nWHERE  p.datumgueltigbis >= TO_CHAR( TODAY, '%Y%m%d')::INT"
            " AND p.artikel_nr = "                                                           + ARTICLENO.toSQLReplacementString() +
            " AND  p.preis_typ  = '0'"
            "\nGROUP BY p.artikel_nr, p.datumgueltigbis, p.preisekgrosso"
            "\nHAVING p.datumgueltigbis = MAX(p.datumgueltigbis )");

		resolve( sqlStatement );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerArticle);
	}

} // end namespace acc_articleconsultant
} // end namespace libabbauw
