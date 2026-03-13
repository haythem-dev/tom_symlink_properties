// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/article/articlepriceacc.h>
#include <infrastructure/accessor/article/articlepriceacc_definitions.h>

namespace libtender {
namespace infrastructure {
namespace accessor {
namespace article {

	BEGIN_ACCESSOR_DEFINITION(accessor::article::lit::ARTICLE_PRICE_ACC)

  	    BEGIN_PROPERTY_DESCRIPTION_LIST
             PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_CODE	)
			 PROPERTY_DESCRIPTION_LIST_ADD( properties::CONTRACT_PRICE	)
             PROPERTY_DESCRIPTION_LIST_ADD( properties::PRICE_INCL_VAT	)
			 PROPERTY_DESCRIPTION_LIST_ADD( properties::AEP				)
        END_PROPERTY_DESCRIPTION_LIST
  
		ACCESS_METHOD(accessor::article::lit::SELECT_PRICE_BY_ARTICLECODE);
			SQL_BUILDER_CREATE	 ( SelectPriceByArticleCode )
			SQL_BUILDER_PUSH_BACK( SelectPriceByArticleCode )

		ACCESS_METHOD(accessor::article::lit::SELECT_PRICE_BY_ARTICLECODE_BG);
			SQL_BUILDER_CREATE	 ( SelectPriceByArticleCodeBG )
			SQL_BUILDER_PUSH_BACK( SelectPriceByArticleCodeBG )
  
    END_ACCESSOR_DEFINITION
  
	//////////////////////////////////////////////////////////////////////////////////////////
    BUILDER_DEFINITION(SelectPriceByArticleCode)

  	bool SelectPriceByArticleCode::isExecutable() const
  	{
        METHODNAME_DEF( SelectPriceByArticleCode, isExecutable );
		static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
        BLOG_TRACE_METHOD( logger, fun );

		bool executable = false;

		if( arePropertiesSet( properties::ARTICLE_CODE.toPropertyString() ) )
		{
			executable = true;
		}

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
  
        return executable;
  	}
  
  	void SelectPriceByArticleCode::buildSQLString()
  	{
  		static const basar::VarString constSQL(
			"SELECT "
			"round( " + properties::CONTRACT_PRICE.toSQLReplacementString() + "*(1+tr.salestaxrate/100),2)	AS " + properties::PRICE_INCL_VAT.getName() + ", "
			"ap.preisekapo	AS " + properties::AEP.getName() + "  "
			"FROM artikelpreis AS ap "
			"LEFT OUTER JOIN artikelzentral az ON az.artikel_nr	= ap.artikel_nr " 
			"LEFT OUTER JOIN taxrates tr ON tr.taxlevel	= az.taxlevel "
			"WHERE preis_typ = 0 AND " 
            "ap.artikel_nr = " + properties::ARTICLE_NO.toSQLReplacementString()
		);
  
        resolve(constSQL);
        BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
  	}

	//////////////////////////////////////////////////////////////////////////////////////////
	BUILDER_DEFINITION(SelectPriceByArticleCodeBG)

  	bool SelectPriceByArticleCodeBG::isExecutable() const
  	{
        METHODNAME_DEF( SelectPriceByArticleCodeBG, isExecutable );
		static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
        BLOG_TRACE_METHOD( logger, fun );

		bool executable = false;

		if( arePropertiesSet( properties::ARTICLE_CODE.toPropertyString() ) )
		{
			executable = true;
		}

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
  
        return executable;
  	}
  
  	void SelectPriceByArticleCodeBG::buildSQLString()
  	{
  		static const basar::VarString constSQL(
			"SELECT "
			"round( " + properties::CONTRACT_PRICE.toSQLReplacementString() + "*(1+tr.salestaxrate/100),2)	AS " + properties::PRICE_INCL_VAT.getName() + ", "
			"round(ap.preisekapo*(1+tr.salestaxrate/100),2)													AS " + properties::AEP.getName()			+ "  "
			"FROM artikelpreis AS ap "
			"LEFT OUTER JOIN artikelzentral az ON az.artikel_nr	= ap.artikel_nr " 
			"LEFT OUTER JOIN taxrates		tr ON tr.taxlevel	= az.taxlevel   "
			"WHERE "
			"preis_typ = 0 "
			"AND ap.artikel_nr = " + properties::ARTICLE_NO.toSQLReplacementString() 
		);
  
        resolve(constSQL);
        BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
  	}

} // namespace article
} // namespace accessor
} // namespace infrastructure
} // namespace libtender
