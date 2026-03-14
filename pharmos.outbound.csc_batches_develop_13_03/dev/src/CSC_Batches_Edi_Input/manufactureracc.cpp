//-------------------------------------------------------------------------------------
/*! \file  manufactureracc.cpp
*  \brief  Accessordefinitions for reading manufacturerdata from table bliefer, liefer etc. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   13.06.2013 
*  history: 13.06.13 pk new
*/
//-------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------//
// include section 
//-------------------------------------------------------------------------------------------------//

#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers
#include "manufactureracc.h"
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
		namespace accManufacturer
		{
			using basar::db::aspect::SQLStringBuilder;

//-------------------------------------------------------------------------------------------------//
// Builder Definition SelDeliverernoByManufacturereanBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelDeliverernoByManufacturereanBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// (belongs to class SQLStringBuilder, method isExecutable() {return arePropertiesSet()}    )
				// Therefore we don't need to define the isExecutable()-method by ourselves.

				ENSURE_PROPERTIES_ARE_SET(	SelDeliverernoByManufacturereanBuilder,					// Builder name
											  libabbauw::properties::MANUFACTUREREAN.toPropertyString()	// property names
											)															// state



			// this method could be replaced by the SQL_PATTERN macro
			void SelDeliverernoByManufacturereanBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelDeliverernoByManufacturereanBuilder::buildSQLString()"; 				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// select lant_nr from bliefer where ean1 = '#manufacturerean#'    
				// orig: select lant_nr from bliefer where herstell_ean = '#manufacturerean#'    

				// Watch out!! Check for correct ean1-values in Austria AT, 
				// Watch out!! if ABBA-UW has to be implemented there 

				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       b.lant_nr AS "		+ DELIVERERNO.getName() );  
				stmt.append(" FROM   bliefer b " );
				stmt.append(" WHERE  b.ean1	= '"	+ MANUFACTUREREAN.toSQLReplacementString() +"'" + ";"); 

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
			//! \ingroup SQLSTMT  sql-statement of SelDeliverernoByManufacturereanBuilder
			//basar::ConstString SelDeliverernoByManufacturereanBuilder::s_SqlStmt ; 	             
	/*			s_SqlStmt.append( "SELECT " );		        
				s_SqlStmt.append( "       b.lant_nr AS "		+ DELIVERERNO.getName() );  
				s_SqlStmt.append(" FROM   bliefer b " );
				s_SqlStmt.append(" WHERE  b.ean1	= '"	+ MANUFACTUREREAN.toSQLReplacementString() +"'" + ";");*/
			//--- END for documenting purposes only! ---

			/* Select als Text
				SELECT b.lant_nr 
				FROM   bliefer b 
				WHERE  b.ean1	= '1234567890123'
			*/



//-------------------------------------------------------------------------------------------------//
// Builder Definition SelManufacturernoByDeliverernoBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelManufacturernoByDeliverernoBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// (belongs to class SQLStringBuilder, method isExecutable() {return arePropertiesSet()}    )
				// Therefore we don't need to define the isExecutable()-method by ourselves.

				ENSURE_PROPERTIES_ARE_SET(	SelManufacturernoByDeliverernoBuilder,					// Builder name
											libabbauw::properties::DELIVERERNO.toPropertyString()	// property names
											)															// state


			// this method could be replaced by the SQL_PATTERN macro
			void SelManufacturernoByDeliverernoBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelManufacturernoByDeliverernoBuilder::buildSQLString()"; 
				using namespace libabbauw::properties;		


				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// select hersteller_nr from liefer where lant_nr = #delivererno#'    
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       l.hersteller_nr AS "		+ MANUFACTURERNO.getName() ); 
				stmt.append(" FROM   liefer l " );
				stmt.append(" WHERE  l.lant_nr = "				+ DELIVERERNO.toSQLReplacementString() );

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
			//! \ingroup SQLSTMT  sql-statement of SelManufacturernoByDeliverernoBuilder
			//basar::ConstString SelManufacturernoByDeliverernoBuilder::s_SqlStmt ; 	             
				//s_SqlStmt.append( "SELECT " );		        
				//s_SqlStmt.append( "       l.hersteller_nr AS "		+ MANUFACTURERNO.getName() );  
				//s_SqlStmt.append(" FROM   liefer l " );
				//s_SqlStmt.append(" WHERE  l.lant_nr = "				+ DELIVERERNO.toSQLReplacementString() );
			//--- END for documenting purposes only! ---

			/* Select als text
				SELECT l.hersteller_nr 
				FROM   liefer l 
				WHERE  l.lant_nr = 1041;
			*/
			

//-------------------------------------------------------------------------------------------------//
// Builder Definition SelManufacturernameByManufacturernoBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelManufacturernameByManufacturernoBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// (belongs to class SQLStringBuilder, method isExecutable() {return arePropertiesSet()}    )
				// Therefore we don't need to define the isExecutable()-method by ourselves.

				ENSURE_PROPERTIES_ARE_SET(	SelManufacturernameByManufacturernoBuilder,					// Builder name
											libabbauw::properties::MANUFACTURERNO.toPropertyString()	// property names
											)															// state


			// this method could be replaced by the SQL_PATTERN macro
			void SelManufacturernameByManufacturernoBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelManufacturernameByManufacturernoBuilder::buildSQLString()"; 
				using namespace libabbauw::properties;		


				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// Solution with table herstel and Herstellername
				//// select name from herstel where hersteller_nr = #manufacturerno#'    
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       h.name AS "				+ MANUFACTURERNAME.getName() ); 
				stmt.append(" FROM   herstel h " );
				stmt.append(" WHERE  h.hersteller_nr = "		+ MANUFACTURERNO.toSQLReplacementString() );

				// Solution with lant_nr ant Lieferantname
				// select name from herstel where hersteller_nr = #manufacturerno#'    
				//basar::I18nString stmt;
				//stmt.append( "SELECT " );		        
				//stmt.append( "       l.lant_name AS "	+ MANUFACTURERNAME.getName() ); 
				//stmt.append(" FROM   liefer l " );
				//stmt.append(" WHERE  l.lant_nr = "		+ MANUFACTURERNO.toSQLReplacementString() );

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
			//! \ingroup SQLSTMT  sql-statement of SelManufacturernoByDeliverernoBuilder
			//basar::ConstString SelManufacturernoByDeliverernoBuilder::s_SqlStmt ; 	             
				//s_SqlStmt.append( "SELECT " );		        
				//s_SqlStmt.append( "       h.name AS "						+ MANUFACTURERNAME.getName() );  
				//s_SqlStmt.append(" FROM   herstel h " );
				//s_SqlStmt.append(" WHERE  h.hersteller_nr = "				+ MANUFACTURERNO.toSQLReplacementString() );
			//--- END for documenting purposes only! ---


//-------------------------------------------------------------------------------------------------//
// Accessor Definition ACC_MANUFACTURER
//-------------------------------------------------------------------------------------------------//
			// macros BEGIN.. and END.. care for creation and registration in the manager before main() was started.
			BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::ACC_MANUFACTURER )
		        using namespace libabbauw::properties;

				BEGIN_PROPERTY_DESCRIPTION_LIST
					PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTURERNO  );
					PROPERTY_DESCRIPTION_LIST_ADD( DELIVERERNO  );
					PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTUREREAN  );
					PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTURERNAME  );
				END_PROPERTY_DESCRIPTION_LIST

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_DELIVERERNO_BY_MANUFACTUREREAN )
					SQL_BUILDER_CREATE( SelDeliverernoByManufacturereanBuilder )
					SQL_BUILDER_PUSH_BACK( SelDeliverernoByManufacturereanBuilder )

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_MANUFACTURERNO_BY_DELIVERERNO )
					SQL_BUILDER_CREATE( SelManufacturernoByDeliverernoBuilder )
					SQL_BUILDER_PUSH_BACK( SelManufacturernoByDeliverernoBuilder )

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_MANUFACTURERNAME_BY_MANUFACTURERNO )
					SQL_BUILDER_CREATE( SelManufacturernameByManufacturernoBuilder )
					SQL_BUILDER_PUSH_BACK( SelManufacturernameByManufacturernoBuilder )

			END_ACCESSOR_DEFINITION


		} // namespace accManufacturer
	} // namespace abbauwDM
} // namespace domMod
