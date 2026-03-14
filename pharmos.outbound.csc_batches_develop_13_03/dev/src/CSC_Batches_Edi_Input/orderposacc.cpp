//-------------------------------------------------------------------------------------------------//
/*! \file  orderposacc.cpp
*  \brief  Accessordefinitions for writing the transfer order positions to table orderpos. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   30.04.2013 
*  history: 30.04.13 pk new
/*/
//-------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------//
// include section 
//-------------------------------------------------------------------------------------------------//

#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers
#include <libbasardbaspect.h>
//#include <libbasardbaspect_definitions.h>
#include "loggerpool.h"
//#include <log_macros.h>
#include <libbasarproperty_propertydescriptionlistref.h>

#include "edi_input_properties_definitions.h"
#include "edi_input_accessor_definitions.h"
#include "orderposacc.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
	namespace abbauwDM
	{
		namespace accOrderpos
		{
			using basar::db::aspect::SQLStringBuilder;

//-------------------------------------------------------------------------------------------------//
// Builder Definition InsertIntoOrderposBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( InsertIntoOrderposBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// Therefore we don't need to define the isExecutable()-method by ourselves.
				ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(InsertIntoOrderposBuilder,			// Builder name
											  libabbauw::properties::BRANCHNO.toPropertyString()		// property names
											+ libabbauw::properties::ORDERNO.toPropertyString() 
											+ libabbauw::properties::POSNO.toPropertyString()	
											+ libabbauw::properties::ARTICLENO.toPropertyString()	
											+ libabbauw::properties::ARTICLE_CODE.toPropertyString()	
											+ libabbauw::properties::ORDEREDQTY.toPropertyString()	
											+ libabbauw::properties::NONCHARGEDQTY.toPropertyString()	
											+ libabbauw::properties::TEXTFIELD.toPropertyString()	
											+ libabbauw::properties::CUSTOMERPOSNO.toPropertyString()		
											+ libabbauw::properties::WHOLESALERPURCHASEPRICE.toPropertyString()	
											+ libabbauw::properties::DISCOUNTPCT.toPropertyString()	
											, "INSERT")											// state


			// this method could be replaced by the SQL_PATTERN macro
			void InsertIntoOrderposBuilder::buildSQLString()
			{
				basar::ConstString fun = "InsertIntoOrderposBuilder::buildSQLString()";
				using namespace libabbauw::properties;	

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// Example: insert into orderpos (branchno, orderno, ...) values (#branchno#, #orderno#, ...)

				basar::I18nString stmt;
				stmt.append( "INSERT INTO orderpos ( " );		        
				stmt.append( "       branchno, " );
				stmt.append( "       orderno, " );
				stmt.append( "       posno, " );
				stmt.append( "       articleno, " );
				stmt.append( "       article_code, " );
				stmt.append( "       orderedqty, " );
				stmt.append( "       nonchargedqty, " );
				stmt.append( "       textfield, " );
				stmt.append( "       customerposno, " );
				stmt.append( "       wholesalerpurchaseprice, " );
				stmt.append( "       discountpct " );
				stmt.append( "       ) VALUES ( " );
				stmt.append(         BRANCHNO.toSQLReplacementString()				+", " );	// toSQLReplacementString returns name in hash-signs, e.g. "#branchno#"
				stmt.append(         ORDERNO.toSQLReplacementString()				+", " ); 
				stmt.append(         POSNO.toSQLReplacementString()					+", " );
				stmt.append(         ARTICLENO.toSQLReplacementString()				+", " );
				stmt.append(" '"+    ARTICLE_CODE.toSQLReplacementString()		+"'"+", " );	// string fields need additional apostrophs to embrace the replacement points of values
				stmt.append(         ORDEREDQTY.toSQLReplacementString()			+", " );
				stmt.append(         NONCHARGEDQTY.toSQLReplacementString()			+", " );
				stmt.append(" '"+    TEXTFIELD.toSQLReplacementString()			+"'"+", " );
				stmt.append(         CUSTOMERPOSNO.toSQLReplacementString()			+", " );
				stmt.append(         WHOLESALERPURCHASEPRICE.toSQLReplacementString()+", " );
				stmt.append(         DISCOUNTPCT.toSQLReplacementString() );


				stmt.append( "       ) " );
	  

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// variable part of the DML statement
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// doesnot exist here

				// finish builder
				resolve( stmt );

				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
			}


			//--- START for documenting purposes only! (because we didn't operate with SQL_PATTERN macro)---
			//! \ingroup SQLSTMT  sql-statement of InsertIntoOrderheadBuilder
			//basar::ConstString InsertIntoOrderheadBuilder::s_SqlStmt = 	             
				//s_SqlStmt.append( "INSERT INTO orderpos ( " );		        
				//s_SqlStmt.append( "       branchno, " );
				//s_SqlStmt.append( "       orderno, " );
				//s_SqlStmt.append( "       posno, " );
				//s_SqlStmt.append( "       articleno, " );
				//s_SqlStmt.append( "       article_code, " );
				//s_SqlStmt.append( "       orderedqty, " );
				//s_SqlStmt.append( "       nonchargedqty, " );
				//s_SqlStmt.append( "       textfield, " );
				//s_SqlStmt.append( "       customerposno, " );
				//s_SqlStmt.append( "       wholesalerpurchaseprice, " );
				//s_SqlStmt.append( "       discountpct " );
				//s_SqlStmt.append( "       ) VALUES ( " );
				//s_SqlStmt.append(         BRANCHNO.toSQLReplacementString()				+", " );	// toSQLReplacementString returns name in hash-signs, e.g. "#branchno#"
				//s_SqlStmt.append(         ORDERNO.toSQLReplacementString()				+", " ); 
				//s_SqlStmt.append(         POSNO.toSQLReplacementString()					+", " );
				//s_SqlStmt.append(         ARTICLENO.toSQLReplacementString()				+", " );
				//s_SqlStmt.append(" '"+    ARTICLE_CODE.toSQLReplacementString()		+"'"+", " );	// string fields need additional apostrophs to embrace the replacement points of values
				//s_SqlStmt.append(         ORDEREDQTY.toSQLReplacementString()			+", " );
				//s_SqlStmt.append(         NONCHARGEDQTY.toSQLReplacementString()			+", " );
				//s_SqlStmt.append(" '"+    TEXTFIELD.toSQLReplacementString()			+"'"+", " );
				//s_SqlStmt.append(         CUSTOMERPOSNO.toSQLReplacementString()			+", " );
				//s_SqlStmt.append(         WHOLESALERPURCHASEPRICE.toSQLReplacementString()+", " );
				//s_SqlStmt.append(         DISCOUNTPCT.toSQLReplacementString() );
			//--- END for documenting purposes only! ---


//---------------------------------------------------------------------------------------------//
// accessor definition ACC_ORDERPOS
//---------------------------------------------------------------------------------------------//
			// macros BEGIN.. and END.. care for creation and registration in the manager before main() was started.
			BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::ACC_ORDERPOS )
				using namespace libabbauw::properties;

				BEGIN_PROPERTY_DESCRIPTION_LIST
					PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO );
					PROPERTY_DESCRIPTION_LIST_ADD( ORDERNO  );
					PROPERTY_DESCRIPTION_LIST_ADD( POSNO  );
					PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENO  );
					PROPERTY_DESCRIPTION_LIST_ADD( ARTICLE_CODE  );
					PROPERTY_DESCRIPTION_LIST_ADD( ORDEREDQTY  );
					PROPERTY_DESCRIPTION_LIST_ADD( NONCHARGEDQTY  );
					PROPERTY_DESCRIPTION_LIST_ADD( TEXTFIELD  );
					PROPERTY_DESCRIPTION_LIST_ADD( CUSTOMERPOSNO  );
					PROPERTY_DESCRIPTION_LIST_ADD( WHOLESALERPURCHASEPRICE  );
					PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTPCT  );
				END_PROPERTY_DESCRIPTION_LIST

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_INSERT_INTO_ORDERPOS )
					SQL_BUILDER_CREATE		( InsertIntoOrderposBuilder )
					SQL_BUILDER_PUSH_BACK	( InsertIntoOrderposBuilder )

			END_ACCESSOR_DEFINITION


		} // namespace accOrderpos
	} // namespace abbauwDM
} // namespace domMod
