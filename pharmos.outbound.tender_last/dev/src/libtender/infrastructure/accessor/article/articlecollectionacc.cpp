// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/article/articlecollectionacc.h>
#include <infrastructure/accessor/article/articlecollectionacc_definitions.h>

namespace libtender {
namespace infrastructure {
namespace accessor {
namespace article {

	BEGIN_ACCESSOR_DEFINITION( lit::ARTICLE_COLLECTION_ACC )

  	    BEGIN_PROPERTY_DESCRIPTION_LIST
             PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NO			)
             PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_CODE		)
             PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NAME		)
			 PROPERTY_DESCRIPTION_LIST_ADD( properties::PACKGROESSE			)
			 PROPERTY_DESCRIPTION_LIST_ADD( properties::DARREICHFORM		)
			 PROPERTY_DESCRIPTION_LIST_ADD( properties::MANUFACTURER_NO		)
			 PROPERTY_DESCRIPTION_LIST_ADD( properties::MANUFACTURER_NAME	)
			 PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_TYPE			)
			 PROPERTY_DESCRIPTION_LIST_ADD( properties::AEP					)
			 PROPERTY_DESCRIPTION_LIST_ADD( properties::TAX_RATE			)
        END_PROPERTY_DESCRIPTION_LIST
  
  	    ACCESS_METHOD( lit::SELECT_ARTICLE_BY_PATTERN );
			SQL_BUILDER_CREATE	 ( SelectArticleByPattern )
			SQL_BUILDER_PUSH_BACK( SelectArticleByPattern )

  	    ACCESS_METHOD( lit::SELECT_ARTICLE_BY_PATTERN_RS );
			SQL_BUILDER_CREATE	 ( SelectArticleByPatternRS )
			SQL_BUILDER_PUSH_BACK( SelectArticleByPatternRS )

		ACCESS_METHOD( lit::SELECT_ARTICLE_BY_ARTICLE_CODE );
			SQL_BUILDER_CREATE	 ( SelectArticleByArticleCode )
			SQL_BUILDER_PUSH_BACK( SelectArticleByArticleCode )

		ACCESS_METHOD( lit::SELECT_ARTICLE_BY_ARTICLE_CODE_RS );
			SQL_BUILDER_CREATE	 ( SelectArticleByArticleCodeRS )
			SQL_BUILDER_PUSH_BACK( SelectArticleByArticleCodeRS )

    END_ACCESSOR_DEFINITION

    BUILDER_DEFINITION( SelectArticleByPattern )
  
  	bool SelectArticleByPattern::isExecutable() const
  	{
        METHODNAME_DEF( SelectArticleByPattern, isExecutable );
		static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
        BLOG_TRACE_METHOD( logger, fun );

		bool executable = false;

		if( arePropertiesSet( properties::ARTICLE_CODE.toPropertyString() ) || arePropertiesSet( properties::ARTICLE_NAME.toPropertyString() ) )
		{
			executable = true;
		}

        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
  
        return executable;
  	}
  
  	void SelectArticleByPattern::buildSQLString()
  	{
  		static const basar::VarString constSQL(
			      "SELECT DISTINCT " 
				  "ac.article_code			 AS "		+ properties::ARTICLE_CODE.getName()		+	", "
			      "ac.articleno				 AS "		+ properties::ARTICLE_NO.getName()			+	", "
				  "CASE "								+
				  "WHEN az.artikel_langname != '' "		+
				  "THEN az.artikel_langname "
				  "ELSE az.artikel_name END  AS "		+ properties::ARTICLE_NAME.getName()		+	", "
				  "az.packgroesse			 AS "		+ properties::PACKGROESSE.getName()			+	", "
				  "az.darreichform			 AS "		+ properties::DARREICHFORM.getName()		+	", "
				  "az.hersteller_nr			 AS "		+ properties::MANUFACTURER_NO.getName()		+	", "
				  "h.name					 AS "		+ properties::MANUFACTURER_NAME.getName()	+	"  "
				  "FROM articlecodes         AS ac "											
				  "INNER JOIN artikelzentral AS az	ON ac.articleno		= az.artikel_nr   " 
				  "INNER JOIN artikellokal   AS al	ON az.artikel_nr	= al.artikel_nr	  "
				  "INNER JOIN manufacturer	     AS h	ON az.hersteller_nr = h.hersteller_nr "
				  "WHERE "
				  "ac.preferred_flag = 1 AND "
				  "(al.artikelaktiv = '1' OR al.artikelaktiv = '4') " );

		basar::VarString variableSQL;

		if( isContainedAndSet( properties::BUSINESS_TYPE ) )
		{
			variableSQL.append( "AND az.businesstypeno = " + properties::BUSINESS_TYPE.toSQLReplacementString() + " " );
		}

  		if( arePropertiesSetAndOneMustBeInState( properties::ARTICLE_CODE.toPropertyString(), basar::SS_CLEAN) )
        {
            variableSQL.append( "AND ac.article_code = '"  + properties::ARTICLE_CODE.toSQLReplacementString() + "' " );
        }
  
  		if( arePropertiesSetAndOneMustBeInState( properties::ARTICLE_NAME.toPropertyString(), basar::SS_CLEAN) )
        {
			variableSQL.append( "AND UPPER(az.artikel_langname) LIKE UPPER('"  + properties::ARTICLE_NAME.toSQLReplacementString() + "%') " );
        }
  
  		basar::VarString organizeResult( "ORDER BY ac.article_code ASC" );
  
        resolve( constSQL + variableSQL + organizeResult );
        BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
	}

	BUILDER_DEFINITION( SelectArticleByPatternRS )

    bool SelectArticleByPatternRS::isExecutable() const
	{
		METHODNAME_DEF( SelectArticleByPatternRS, doIsExecutable );
		static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
		BLOG_TRACE_METHOD( logger, fun );

		bool executable = false;
		if( arePropertiesSet( properties::ARTICLE_CODE.toPropertyString() ) || arePropertiesSet( properties::ARTICLE_NAME.toPropertyString() ) )
		{
			executable = true;
		}
		CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
		return executable;

	}

	void SelectArticleByPatternRS::buildSQLString()
	{
		static const basar::VarString constSQL(
				"SELECT DISTINCT " 
				"ac.article_code			 AS "		+ properties::ARTICLE_CODE.getName()		+	", "
				"ac.articleno				 AS "		+ properties::ARTICLE_NO.getName()			+	", "
				"CASE "								+
				"WHEN az.artikel_langname != '' "		+
				"THEN az.artikel_langname "
				"ELSE az.artikel_name END  AS "		+ properties::ARTICLE_NAME.getName()		+	", "
				"az.packgroesse			 AS "		+ properties::PACKGROESSE.getName()			+	", "
				"az.darreichform			 AS "		+ properties::DARREICHFORM.getName()		+	", "
				"az.hersteller_nr			 AS "		+ properties::MANUFACTURER_NO.getName()		+	", "
				"h.name					 AS "		+ properties::MANUFACTURER_NAME.getName()	+	"  "
				"FROM articlecodes         AS ac "											
				"INNER JOIN artikelzentral AS az	ON ac.articleno		= az.artikel_nr   " 
				"INNER JOIN artikellokal   AS al	ON az.artikel_nr	= al.artikel_nr	  "
				"INNER JOIN manufacturer	     AS h	ON az.hersteller_nr = h.hersteller_nr "
				"WHERE "
				"ac.preferred_flag = 1 ");

		basar::VarString variableSQL;

		if( isContainedAndSet( properties::BUSINESS_TYPE ) )
		{
			variableSQL.append( "AND az.businesstypeno = " + properties::BUSINESS_TYPE.toSQLReplacementString() + " " );
		}

		if( arePropertiesSetAndOneMustBeInState( properties::ARTICLE_CODE.toPropertyString(), basar::SS_CLEAN) )
		{
			variableSQL.append( "AND ac.article_code = '"  + properties::ARTICLE_CODE.toSQLReplacementString() + "' " );
		}

		if( arePropertiesSetAndOneMustBeInState( properties::ARTICLE_NAME.toPropertyString(), basar::SS_CLEAN) )
		{
			variableSQL.append( "AND UPPER(az.artikel_langname) LIKE UPPER('"  + properties::ARTICLE_NAME.toSQLReplacementString() + "%') " );
		}

		basar::VarString organizeResult( "ORDER BY ac.article_code ASC" );
		resolve( constSQL + variableSQL + organizeResult );
		BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
	}



    BUILDER_DEFINITION( SelectArticleByArticleCode )
  
  	bool SelectArticleByArticleCode::isExecutable() const
  	{
        METHODNAME_DEF( SelectArticleByArticleCode, isExecutable );
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
  
  	void SelectArticleByArticleCode::buildSQLString()
  	{
  		static const basar::VarString constSQL(
			      "SELECT DISTINCT "
				  "ac.article_code			 AS "		+ properties::ARTICLE_CODE.getName()		+	", "
			      "ac.articleno				 AS "		+ properties::ARTICLE_NO.getName()			+	", "
				  "az.artikel_name			 AS "		+ properties::ARTICLE_NAME.getName()		+	", "
                  "az.packgroesse			 AS "		+ properties::PACKGROESSE.getName()			+	", "
				  "az.darreichform			 AS "		+ properties::DARREICHFORM.getName()		+	", "
				  "az.hersteller_nr			 AS "		+ properties::MANUFACTURER_NO.getName()		+	", "
				  "h.name					 AS "		+ properties::MANUFACTURER_NAME.getName()	+	"  " 
				  "FROM articlecodes AS ac "											
				  "INNER JOIN artikelzentral AS az ON ac.articleno		= az.artikel_nr   " 
				  "INNER JOIN artikellokal	 AS al ON az.artikel_nr		= al.artikel_nr	  "
				  "INNER JOIN manufacturer		 AS h  ON az.hersteller_nr	= h.hersteller_nr "
				  "WHERE "
				  "ac.preferred_flag = 1 AND "
				  "(al.artikelaktiv = '1' OR al.artikelaktiv = '4') " );

		basar::VarString variableSQL;

		if( isContainedAndSet( properties::BUSINESS_TYPE ) )
		{
			variableSQL.append( "AND az.businesstypeno = " + properties::BUSINESS_TYPE.toSQLReplacementString() + " " );
		}

  		if( arePropertiesSetAndOneMustBeInState( properties::ARTICLE_CODE.toPropertyString(), basar::SS_CLEAN) )
        {
            variableSQL.append( "AND ac.article_code = '"  + properties::ARTICLE_CODE.toSQLReplacementString() + "' " );
        }
  
  		basar::VarString organizeResult( "ORDER BY ac.article_code ASC" );
  
        resolve( constSQL + variableSQL + organizeResult );
        BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
  	}

    BUILDER_DEFINITION( SelectArticleByArticleCodeRS )
  
  	bool SelectArticleByArticleCodeRS::isExecutable() const
  	{
        METHODNAME_DEF( SelectArticleByArticleCodeRS, isExecutable );
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
  
  	void SelectArticleByArticleCodeRS::buildSQLString()
  	{

  		static const basar::VarString constSQL(
			      "SELECT DISTINCT "
				  "ac.article_code			 AS "		+ properties::ARTICLE_CODE.getName()		+	", "
			      "ac.articleno				 AS "		+ properties::ARTICLE_NO.getName()			+	", "
				  "az.artikel_name			 AS "		+ properties::ARTICLE_NAME.getName()		+	", "
                  "az.packgroesse			 AS "		+ properties::PACKGROESSE.getName()			+	", "
				  "az.darreichform			 AS "		+ properties::DARREICHFORM.getName()		+	", "
				  "az.hersteller_nr			 AS "		+ properties::MANUFACTURER_NO.getName()		+	", "
				  "h.name					 AS "		+ properties::MANUFACTURER_NAME.getName()	+	"  " 
				  "FROM articlecodes AS ac "											
				  "INNER JOIN artikelzentral AS az ON ac.articleno		= az.artikel_nr   " 
				  "INNER JOIN artikellokal	 AS al ON az.artikel_nr		= al.artikel_nr	  "
				  "INNER JOIN manufacturer		 AS h  ON az.hersteller_nr	= h.hersteller_nr "
				  "WHERE "
				  "ac.preferred_flag = 1 " );

		basar::VarString variableSQL;

		if( isContainedAndSet( properties::BUSINESS_TYPE ) )
		{
			variableSQL.append( "AND az.businesstypeno = " + properties::BUSINESS_TYPE.toSQLReplacementString() + " " );
		}

  		if( arePropertiesSetAndOneMustBeInState( properties::ARTICLE_CODE.toPropertyString(), basar::SS_CLEAN) )
        {
            variableSQL.append( "AND ac.article_code = '"  + properties::ARTICLE_CODE.toSQLReplacementString() + "' " );
        }
  
  		basar::VarString organizeResult( "ORDER BY ac.article_code ASC" );
  
        resolve( constSQL + variableSQL + organizeResult );
        BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
  	}


} // end namespace article
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender
