//-------------------------------------------------------------------------------------
/*! \file  articleacc.cpp
*  \brief  Accessordefinitions for reading articledata from tables 
*  \brief  artikelzentral, artikelpreis, articlecodes. 
*  \author P. Kessling
*  \date   22.04.2013 
*  history: 22.04.13 pk new
*/
//-------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------//
// include section (from obst-example)
//-------------------------------------------------------------------------------------------------//

#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers
#include "articleacc.h"
#include <libbasardbaspect.h>
#include "loggerpool.h"

#include "edi_input_properties_definitions.h"
#include "edi_input_accessor_definitions.h"

#include <libbasarproperty_propertydescriptionlistref.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
	namespace abbauwDM
	{
		namespace accArticle
		{
			using basar::db::aspect::SQLStringBuilder;


//-------------------------------------------------------------------------------------------------//
// Builder Definition SelManufacturernoByArticlenoBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelManufacturernoByArticlenoBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// (belongs to class SQLStringBuilder, method isExecutable() {return arePropertiesSet()}    )
				// Therefore we don't need to define the isExecutable()-method by ourselves.

				ENSURE_PROPERTIES_ARE_SET(	SelManufacturernoByArticlenoBuilder,					// Builder name
											libabbauw::properties::ARTICLENO.toPropertyString()		// property names
											)														// state


			// this method could be replaced by the SQL_PATTERN macro
			void SelManufacturernoByArticlenoBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelManufacturernoByArticlenoBuilder::buildSQLString()";
				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// "select hersteller_nr from artikelzentral where artikel_nr = #articleno#"
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       z.hersteller_nr AS "   + MANUFACTURERNO.getName() );
        
				stmt.append(" FROM   artikelzentral z " );
				stmt.append(" WHERE  artikel_nr = " + ARTICLENO.toSQLReplacementString() + ";");

				//static basar::I18nString stmt(
				//	"SELECT z.hersteller_nr AS " + MANUFACTURERNO.getName()
				//	);

				//basar::I18nString sql( stmt )
				//	sql.append()

				// variable part of the DML statement

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// variable attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// doesnot exist here

				// finish builder
				resolve( stmt );

				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
			}


			//--- START for documenting purposes only! (because we didn't operate with SQL_PATTERN macro)---
			//! \ingroup SQLSTMT  sql-statement of SelManufacturernoByArticleNoBuilder
			//basar::ConstString SelManufacturernoByArticlenoBuilder::s_SqlStmt ; 	             
			//s_SqlStmt.append( "SELECT " );		        
			//s_SqlStmt.append( "       z.hersteller_nr AS "	+ MANUFACTURERNO.getName() );
			//s_SqlStmt.append(" FROM   artikelzentral z " );
			//s_SqlStmt.append(" WHERE  artikel_nr = "		+ ARTICLENO.toSQLReplacementString() + ";");
			//--- END for documenting purposes only! ---



//-------------------------------------------------------------------------------------------------//
// Builder Definition SelArticlenoInternByArticleMFBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelArticlenoInternByArticleMFBuilder )

				ENSURE_PROPERTIES_ARE_SET(	SelArticlenoInternByArticleMFBuilder,					// Builder name
											libabbauw::properties::ARTICLENOMF.toPropertyString()	// property names
											)														// state


			// this method could be replaced by the SQL_PATTERN macro
			void SelArticlenoInternByArticleMFBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelArticlenoInternByArticleMFBuilder::buildSQLString()";
				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// "select artikel_nr from artikelzentral where artikel_nr_herst = '#articlenomf#'"
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       z.artikel_nr AS "   + ARTICLENO.getName() );
        
				stmt.append(" FROM   artikelzentral z " );
				stmt.append(" WHERE  artikel_nr_herst = '" + ARTICLENOMF.toSQLReplacementString() + "' ;"); // string fields need additional apostrophs to embrace the replacement points of values

				// variable part of the DML statement

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// variable attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// doesnot exist here

				// finish builder
				resolve( stmt );

				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
			}


			//--- START for documenting purposes only! (because we didn't operate with SQL_PATTERN macro)---
			//! \ingroup SQLSTMT  sql-statement of SelArticlenoInternByArticleMFBuilder
			//basar::ConstString SelArticlenoInternByArticleMFBuilder::s_SqlStmt ; 	             
				//s_SqlStmt.append( "SELECT " );		        
				//s_SqlStmt.append( "       z.artikel_nr AS "   + ARTICLENO.getName() );
				//    
				//s_SqlStmt.append(" FROM   artikelzentral z " );
				//s_SqlStmt.append(" WHERE  artikel_nr_herst = '" + ARTICLENO.toSQLReplacementString() + "' ;"); // string fields need additional apostrophs to embrace the replacement points of values
			//--- END for documenting purposes only! ---



			
//-------------------------------------------------------------------------------------------------//
// Builder Definition SelWholesalerpurchasepriceByArticlenoBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelWholesalerpurchasepriceByArticlenoBuilder )

				ENSURE_PROPERTIES_ARE_SET(	SelWholesalerpurchasepriceByArticlenoBuilder,				// Builder name
											libabbauw::properties::ARTICLENO.toPropertyString()			// property names
										  + libabbauw::properties::VALIDTODATE.toPropertyString()		// property names
											)															// state, if set


			// this method could be replaced by the SQL_PATTERN macro
			void SelWholesalerpurchasepriceByArticlenoBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelWholesalerpurchasepriceByArticlenoBuilder::buildSQLString()"; 				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// "select FIRST 1 preisekgrosso			(nur ersten Datensatz aus der Treffermenge liefern)
				//  from artikelpreis 
				//  where artikel_nr = = #articleno# 
				//    and datumgueltigbis >= #datumgueltigbis#  (>= today)
				//  order by datumgueltigab desc"
				//
				// alternative: Look only for datumgueltigbis < today 
				// Improve Performance: Additional index on artikel_nr and datumgueltigbis and preis_typ
				basar::I18nString stmt;
				stmt.append( "SELECT FIRST 1 " );		        
				stmt.append( "       p.preisekgrosso AS "   + WHOLESALERPURCHASEPRICE.getName() );
 				stmt.append(" FROM   artikelpreis p " );
				stmt.append(" WHERE  p.artikel_nr = "		+ ARTICLENO.toSQLReplacementString() );
				stmt.append("   AND  p.datumgueltigbis >= "	+ VALIDTODATE.toSQLReplacementString() );
				stmt.append("   AND  p.preis_typ  = 0" );
				stmt.append(" ORDER BY  p.datumgueltigab DESC " );
				stmt.append(";");

				// variable part of the DML statement

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// variable attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// doesnot exist here

				// finish builder
				resolve( stmt );

				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
			}


			//--- START for documenting purposes only! (because we didn't operate with SQL_PATTERN macro)---
			//! \ingroup SQLSTMT  sql-statement of SelWholesalerpurchasepriceByArticlenoBuilder
			//basar::ConstString SelWholesalerpurchasepriceByArticlenoBuilder::s_SqlStmt ; 	             
				//s_SqlStmt.append( "SELECT FIRST 1 " );		        
				//s_SqlStmt.append( "       p.preisekgrosso AS "   + WHOLESALERPURCHASEPRICE.getName() );
 				//	s_SqlStmt.append(" FROM   artikelpreis p " );
				//s_SqlStmt.append(" WHERE  p.artikel_nr = "		+ ARTICLENO.toSQLReplacementString() );
				//s_SqlStmt.append("   AND  p.datumgueltigbis >= "	+ VALIDTODATE.toSQLReplacementString() );
				//s_SqlStmt.append(" ORDER BY  p.datumgueltigab DESC " + ";");
			//--- END for documenting purposes only! ---



//-------------------------------------------------------------------------------------------------//
// Builder Definition SelPreferredArticleCodeByArticleCodeAndCodeTypeBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelPreferredArticleCodeByArticleCodeAndCodeTypeBuilder )

				ENSURE_PROPERTIES_ARE_SET(	SelPreferredArticleCodeByArticleCodeAndCodeTypeBuilder,		// Builder name
											libabbauw::properties::ARTICLE_CODE.toPropertyString()+		// property names
											libabbauw::properties::CODE_TYPE.toPropertyString()			// property names
											)															// state, if set


			// this method could be replaced by the SQL_PATTERN macro
			void SelPreferredArticleCodeByArticleCodeAndCodeTypeBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelPreferredArticleCodeByArticleCodeAndCodeTypeBuilder::buildSQLString()"; 
				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// select article_code   
				//  from articlecodes 
				//  where preferred_flag = 1 
				//    and articleno = (
				//			select articleno 
				//				from articlecodes 
				//				where article_code = #article_code# (set to e.g. "7680516820922") 
				//				  and code_type = #code_type#  (set to e.g. "13")
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       a.article_code AS "   + ARTICLE_CODE.getName() );
 				stmt.append(" FROM   articlecodes a " );
				stmt.append(" WHERE  a.preferred_flag = 1 "	);
				stmt.append("   AND  a.articleno = ( "	);
				stmt.append("                        SELECT " );		        
				stmt.append( "                              b.articleno AS "   + ARTICLENO.getName() );
 				stmt.append("                        FROM   articlecodes b " );
				stmt.append("                        WHERE  b.article_code = '"	+ ARTICLE_CODE.toSQLReplacementString() +"'"); // string fields need additional apostrophs to embrace the replacement points of values
				stmt.append("                          AND  b.code_type = "		+ CODE_TYPE.toSQLReplacementString() );
				stmt.append("                       );");


				// variable part of the DML statement

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// variable attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// doesnot exist here

				// finish builder
				resolve( stmt );

				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
			}


			//--- START for documenting purposes only! (because we didn't operate with SQL_PATTERN macro)---
			//! \ingroup SQLSTMT  sql-statement of SelPreferredArticleCodeByArticleCodeAndCodeTypeBuilder
			//basar::ConstString SelPreferredArticleCodeByArticleCodeAndCodeTypeBuilder::s_SqlStmt ; 	             
				//s_SqlStmt.append( "SELECT " );		        
				//s_SqlStmt.append( "       a.article_code AS "   + ARTICLE_CODE.getName() );
 				//s_SqlStmt.append(" FROM   articlecodes a " );
				//s_SqlStmt.append(" WHERE  a.preferred_flag = 1 " );
				//s_SqlStmt.append("   AND  a.articleno = ( "	);
				//s_SqlStmt.append("                        SELECT " );		        
				//s_SqlStmt.append( "                              b.articleno AS "   + ARTICLENO.getName() );
 				//s_SqlStmt.append("                        FROM   articlecodes b " );
				//s_SqlStmt.append("                        WHERE  b.article_code = '"	+ ARTICLE_CODE.toSQLReplacementString() +"'"); // string fields need additional apostrophs to embrace the replacement points of values
				//s_SqlStmt.append("                          AND  b.code_type = "		+ CODE_TYPE.toSQLReplacementString() );
				//s_SqlStmt.append("                       );");
			//--- END for documenting purposes only! ---


//-------------------------------------------------------------------------------------------------//
// Builder Definition SelPreferredArticlenoByPreferredArticleCodeBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelPreferredArticlenoByPreferredArticleCodeBuilder )

				ENSURE_PROPERTIES_ARE_SET(	SelPreferredArticlenoByPreferredArticleCodeBuilder,		// Builder name
											libabbauw::properties::ARTICLE_CODE.toPropertyString()	// property names
											)															// state, if set


			// this method could be replaced by the SQL_PATTERN macro
			void SelPreferredArticlenoByPreferredArticleCodeBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelPreferredArticlenoByPreferredArticleCodeBuilder::buildSQLString()"; 
				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// select articleno   
				//  from articlecodes 
				//  where article_code = = #article_code# (set to e.g. "1482604")
				//    and preferred_flag = 1 
				//
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       a.articleno AS "		+ ARTICLENO.getName() );
 				stmt.append(" FROM   articlecodes a " );
				stmt.append(" WHERE  a.article_code =  '"	+ ARTICLE_CODE.toSQLReplacementString() +"'");// string fields need additional apostrophs to embrace the replacement points of values
				stmt.append("   AND  a.preferred_flag = 1 "	);
				stmt.append(" ;");


				// variable part of the DML statement

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// variable attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// doesnot exist here

				// finish builder
				resolve( stmt );

				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun +getSQLString() );
			}


			//--- START for documenting purposes only! (because we didn't operate with SQL_PATTERN macro)---
			//! \ingroup SQLSTMT  sql-statement of SelPreferredArticlenoByPreferredArticleCodeBuilder
				//basar::ConstString SelPreferredArticlenoByPreferredArticleCodeBuilder::s_SqlStmt ; 	             
				//s_SqlStmt.append( "SELECT " );		        
				//s_SqlStmt.append( "       a.articleno AS "		+ ARTICLENO.getName() );
 				//s_SqlStmt.append(" FROM   articlecodes a " );
				//s_SqlStmt.append(" WHERE  a.article_code =  "	+ ARTICLE_CODE.toSQLReplacementString());
				//s_SqlStmt.append("   AND  a.preferred_flag = 1 "	);
				//s_SqlStmt.append(" ;");
			//--- END for documenting purposes only! ---



			//-------------------------------------------------------------------------------------------------//
			// Builder Definition SelPreferredArticleCodeByArticlenoInternBuilder
			//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelPreferredArticleCodeByArticlenoInternBuilder )

				ENSURE_PROPERTIES_ARE_SET(	SelPreferredArticleCodeByArticlenoInternBuilder,		// Builder name
											libabbauw::properties::ARTICLENO.toPropertyString()		// property names
											)														// state, if set


			// this method could be replaced by the SQL_PATTERN macro
			void SelPreferredArticleCodeByArticlenoInternBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelPreferredArticleCodeByArticlenoInternBuilder::buildSQLString()"; 
				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// select article_code   
				//  from articlecodes 
				//  where articleno = = #articleno# (set to e.g. "1482604")
				//    and preferred_flag = 1 
				//
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       a.article_code AS "		+ ARTICLE_CODE.getName() );
 				stmt.append(" FROM   articlecodes a " );
				stmt.append(" WHERE  a.articleno =  "	+ ARTICLENO.toSQLReplacementString() );
				stmt.append("   AND  a.preferred_flag = 1 "	);
				stmt.append(" ;");


				// variable part of the DML statement

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// variable attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// doesnot exist here

				// finish builder
				resolve( stmt );

				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
			}


			//--- START for documenting purposes only! (because we didn't operate with SQL_PATTERN macro)---
			//! \ingroup SQLSTMT  sql-statement of SelPreferredArticleCodeByArticlenoInternBuilder
				//basar::ConstString SelPreferredArticleCodeByArticlenoInternBuilder::s_SqlStmt ; 	             
				//s_SqlStmt.append( "SELECT " );		        
				//s_SqlStmt.append( "       a.article_code AS "		+ ARTICLE_CODE.getName() );
 				//s_SqlStmt.append(" FROM   articlecodes a " );
				//s_SqlStmt.append(" WHERE  a.articleno =  "	+ ARTICLENO.toSQLReplacementString() );
				//s_SqlStmt.append("   AND  a.preferred_flag = 1 "	);
				//s_SqlStmt.append(" ;");
			//--- END for documenting purposes only! ---


//-------------------------------------------------------------------------------------------------//
// Builder Definition SelCMIArticlesBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION(SelCMIArticlesBuilder)

				ENSURE_PROPERTIES_ARE_SET(SelCMIArticlesBuilder,		
					libabbauw::properties::ARTICLE_CODE.toPropertyString()		
				)													

			void SelCMIArticlesBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelCMIArticlesBuilder::buildSQLString()";
				using namespace libabbauw::properties;

				basar::I18nString stmt;
				stmt.append("SELECT ");
				stmt.append("       az.cmi_flag AS " + ISCMI.getName());
				stmt.append(" FROM   artikelzentral az ");
				stmt.append(" INNER JOIN articlecodes ac ON ac.articleno = az.artikel_nr AND preferred_flag = 1 ");
				stmt.append(" WHERE  ac.article_code =  '" + ARTICLE_CODE.toSQLReplacementString() + "' ");
				stmt.append(" ;");

				resolve(stmt);

				BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString());
			}

//-------------------------------------------------------------------------------------------------//
// Accessor Definition ACC_ARTICLE
//-------------------------------------------------------------------------------------------------//

			// macros BEGIN.. and END.. care for creation and registration in the manager before main() was started.
			BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::ACC_ARTICLE )
		        using namespace libabbauw::properties;

				BEGIN_PROPERTY_DESCRIPTION_LIST
					PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENO );
					PROPERTY_DESCRIPTION_LIST_ADD( ARTICLE_CODE );
					PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENOMF );
					PROPERTY_DESCRIPTION_LIST_ADD( CODE_TYPE );

					PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTURERNO  );
					PROPERTY_DESCRIPTION_LIST_ADD( WHOLESALERPURCHASEPRICE  );
					//PROPERTY_DESCRIPTION_LIST_ADD( VALIDFROMDATE  );
					PROPERTY_DESCRIPTION_LIST_ADD( VALIDTODATE  );
					PROPERTY_DESCRIPTION_LIST_ADD( ISCMI );

				END_PROPERTY_DESCRIPTION_LIST

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_MANUFACTURERNO_BY_ARTICLENO )
					SQL_BUILDER_CREATE( SelManufacturernoByArticlenoBuilder )
					SQL_BUILDER_PUSH_BACK( SelManufacturernoByArticlenoBuilder )

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_ARTICLENO_INTERN_BY_ARTICLENOMF )
					SQL_BUILDER_CREATE( SelArticlenoInternByArticleMFBuilder )
					SQL_BUILDER_PUSH_BACK( SelArticlenoInternByArticleMFBuilder )

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_WHOLESALERPURCHASEPRICE_BY_ARTICLENO )
					SQL_BUILDER_CREATE( SelWholesalerpurchasepriceByArticlenoBuilder )
					SQL_BUILDER_PUSH_BACK( SelWholesalerpurchasepriceByArticlenoBuilder )

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_PREFERRED_ARTICLECODE_BY_ARTICLECODE_AND_CODE_TYPE )
					SQL_BUILDER_CREATE( SelPreferredArticleCodeByArticleCodeAndCodeTypeBuilder )
					SQL_BUILDER_PUSH_BACK( SelPreferredArticleCodeByArticleCodeAndCodeTypeBuilder )

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_PREFERRED_ARTICLENO_BY_PREFERRED_ARTICLECODE )
					SQL_BUILDER_CREATE( SelPreferredArticlenoByPreferredArticleCodeBuilder )
					SQL_BUILDER_PUSH_BACK( SelPreferredArticlenoByPreferredArticleCodeBuilder )

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_PREFERRED_ARTICLECODE_BY_ARTICLENO_INTERN )
					SQL_BUILDER_CREATE( SelPreferredArticleCodeByArticlenoInternBuilder )
					SQL_BUILDER_PUSH_BACK( SelPreferredArticleCodeByArticlenoInternBuilder )
				
				ACCESS_METHOD(libabbauw::domMod::ACCMETH_SELECT_CMI_ARTICLES)
					SQL_BUILDER_CREATE(SelCMIArticlesBuilder)
					SQL_BUILDER_PUSH_BACK(SelCMIArticlesBuilder)

			END_ACCESSOR_DEFINITION



		} // namespace accArticle
	} // namespace abbauwDM
} // namespace domMod
