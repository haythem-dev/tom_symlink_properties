//-------------------------------------------------------------------------------------------------//
/*! \file  orderheadacc.cpp
*  \brief  Accessordefinitions for writing the transfer order head to table orderhead. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   18.04.2013 
*  history: 18.04.13 pk new
*           26.06.14: insert into new field orderhead->pharmacynomf (contains originally sent pharmacy-EAN)
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
#include "orderheadacc.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
	namespace abbauwDM
	{
		namespace accOrderhead
		{
			using basar::db::aspect::SQLStringBuilder;

//-------------------------------------------------------------------------------------------------//
// Builder Definition SelMaxOrdernoByBranchnoBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelMaxOrdernoByBranchnoBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// (belongs to class SQLStringBuilder, method isExecutable() {return arePropertiesSet()}    )
				// Therefore we don't need to define the isExecutable()-method by ourselves.

				ENSURE_PROPERTIES_ARE_SET(	SelMaxOrdernoByBranchnoBuilder,					// Builder name
											libabbauw::properties::BRANCHNO.toPropertyString()			// property names
											)														// state


			// this method could be replaced by the SQL_PATTERN macro
			void SelMaxOrdernoByBranchnoBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelMaxOrdernoByBranchnoBuilder::buildSQLString()";
				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// "select hersteller_nr from artikelzentral where artikel_nr = #articleno#"
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       MAX(o.orderno) AS "   + ORDERNO.getName() );
        
				stmt.append(" FROM   orderhead o " );
				stmt.append(" WHERE  branchno = " + BRANCHNO.toSQLReplacementString() + ";");

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
			//! \ingroup SQLSTMT  sql-statement of SelMaxOrdernoByBranchnoBuilder
			//basar::ConstString SelMaxOrdernoByBranchnoBuilder::s_SqlStmt ; 	             
			//s_SqlStmt.append( "SELECT " );		        
			//s_SqlStmt.append( "       MAX(o.orderno) AS "   + ORDERNO.getName() );
			//s_SqlStmt.append(" FROM   orderhead o " );
			//s_SqlStmt.append(" WHERE  branchno = " + BRANCHNO.toSQLReplacementString() + ";");
			//--- END for documenting purposes only! ---



//-------------------------------------------------------------------------------------------------//
// Builder Definition InsertIntoOrderheadBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( InsertIntoOrderheadBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// Therefore we don't need to define the isExecutable()-method by ourselves.
				// kes 26.06.14: new property PHARMACYNOMF added
				ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(InsertIntoOrderheadBuilder,			// Builder name
											  libabbauw::properties::BRANCHNO.toPropertyString()		// property names
											+ libabbauw::properties::ORDERNO.toPropertyString() 
											+ libabbauw::properties::STATUS.toPropertyString()	
											+ libabbauw::properties::ORDERTYPE.toPropertyString()	
											+ libabbauw::properties::ORDERPICKINGTYPE.toPropertyString()	
											+ libabbauw::properties::BOOKENTRYTYPE.toPropertyString()	
											+ libabbauw::properties::ORDERDATE.toPropertyString()	
											+ libabbauw::properties::ORDERVALUE.toPropertyString()	
											+ libabbauw::properties::DEFERREDPAYMENTDATE.toPropertyString()	
											+ libabbauw::properties::PHARMACYNO.toPropertyString()	
											+ libabbauw::properties::HEADERTEXT.toPropertyString()	
											+ libabbauw::properties::ORDERACQUISITIONDATE.toPropertyString()	
											+ libabbauw::properties::DATASOURCETYPE.toPropertyString()	
											+ libabbauw::properties::EDIDELIVERYDATE.toPropertyString()	
											+ libabbauw::properties::REFERENCE.toPropertyString()	
											+ libabbauw::properties::ORDERNOCSC.toPropertyString()	
											+ libabbauw::properties::ORDERNOMF.toPropertyString()	
											+ libabbauw::properties::MANUFACTURERNO.toPropertyString()
											+ libabbauw::properties::EDIDELIVERYDATEMF.toPropertyString()
											+ libabbauw::properties::PHARMACYNOMF.toPropertyString()	
											, "INSERT")											// state


			// this method could be replaced by the SQL_PATTERN macro
			// kes 26.06.14: new property PHARMACYNOMF added
			void InsertIntoOrderheadBuilder::buildSQLString()
			{
				basar::ConstString fun = "InsertIntoOrderheadBuilder::buildSQLString()"; 
				using namespace libabbauw::properties;	

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// Example: insert into orderhead (branchno, orderno, ...) values (#branchno#, #orderno#, ...)

				basar::I18nString stmt;
				stmt.append( "INSERT INTO orderhead ( " );		        
				stmt.append( "       branchno, " );
				stmt.append( "       orderno, " );
				stmt.append( "       status, " );
				stmt.append( "       ordertype, " );
				stmt.append( "       orderpickingtype, " );
				stmt.append( "       bookentrytype, " );
				stmt.append( "       orderdate, " );
				stmt.append( "       ordervalue, " );
				stmt.append( "       deferredpaymentdate, " );
				stmt.append( "       pharmacyno, " );
				stmt.append( "       headertext, " );
				stmt.append( "       orderacquisitiondate, " );
				stmt.append( "       datasourcetype, " );
				stmt.append( "       edideliverydate, " );
				stmt.append( "       reference, " );
				stmt.append( "       ordernocsc, " );
				stmt.append( "       ordernomf, " );
				stmt.append( "       manufacturerno, " );
				stmt.append( "       edideliverydatemf, " );
				stmt.append( "       pharmacynomf " );
				stmt.append( "       ) VALUES ( " );
				stmt.append(         BRANCHNO.toSQLReplacementString()				+", " );	// toSQLReplacementString returns name in hash-signs, e.g. "#branchno#"
				stmt.append(         ORDERNO.toSQLReplacementString()				+", " ); 
				stmt.append(" '"+    STATUS.toSQLReplacementString()			+"'"+", " );	// string fields need additional apostrophs to embrace the replacement points of values
				stmt.append(" '"+    ORDERTYPE.toSQLReplacementString()			+"'"+", " );
				stmt.append(" '"+    ORDERPICKINGTYPE.toSQLReplacementString()	+"'"+", " );
				stmt.append(" '"+    BOOKENTRYTYPE.toSQLReplacementString()		+"'"+", " );
				stmt.append(         ORDERDATE.toSQLReplacementString()				+", " );
				stmt.append(         ORDERVALUE.toSQLReplacementString()			+", " );
				stmt.append(         DEFERREDPAYMENTDATE.toSQLReplacementString()	+", " );
				stmt.append(         PHARMACYNO.toSQLReplacementString()			+", " );
				stmt.append(" '"+    HEADERTEXT.toSQLReplacementString()		+"'"+", " );
				stmt.append(         ORDERACQUISITIONDATE.toSQLReplacementString()	+", " );
				stmt.append(" '"+    DATASOURCETYPE.toSQLReplacementString()	+"'"+", " );
				stmt.append(         EDIDELIVERYDATE.toSQLReplacementString()		+", " );
				stmt.append(" '"+    REFERENCE.toSQLReplacementString()			+"'"+", " );
				stmt.append(         ORDERNOCSC.toSQLReplacementString()			+", " );
				stmt.append(" '"+    ORDERNOMF.toSQLReplacementString()			+"'"+", " );
				stmt.append(         MANUFACTURERNO.toSQLReplacementString()		+", " );
				stmt.append(         EDIDELIVERYDATEMF.toSQLReplacementString()		+", " );
				stmt.append(" '"+    PHARMACYNOMF.toSQLReplacementString()		+"'" );
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
			//		"INSERT INTO orderhead ( " 
			//		+ libabbauw::properties::BRANCHNO.getName()				+ ", "
			//		+ libabbauw::properties::ORDERNO.getName()					+ ", "
			//		+ libabbauw::properties::STATUS.getName()					+ ", "
			//		+ libabbauw::properties::ORDERTYPE.getName()				+ ", "
			//		+ libabbauw::properties::ORDERPICKINGTYPE.getName()		+ ", "
			//		+ libabbauw::properties::BOOKENTRYTYPE.getName()			+ ", "
			//		+ libabbauw::properties::ORDERDATE.getName()				+ ", "
			//		+ libabbauw::properties::ORDERVALUE.getName()				+ ", "
			//		+ libabbauw::properties::DEFERREDPAYMENTDATE.getName()		+ ", "
			//		+ libabbauw::properties::PHARMACYNO.getName()				+ ", "
			//		+ libabbauw::properties::HEADERTEXT.getName()				+ ", "
			//		+ libabbauw::properties::ORDERACQUISITIONDATE.getName()	+ ", "
			//		+ libabbauw::properties::DATASOURCETYPE.getName()			+ ", "
			//		+ libabbauw::properties::EDIDELIVERYDATE.getName()			+ ", "
			//		+ libabbauw::properties::REFERENCE.getName()				+ ", "
			//		+ libabbauw::properties::ORDERNOCSC.getName()				+ ", "
			//		+ libabbauw::properties::ORDERNOMF.getName()				+ ", "
			//		+ libabbauw::properties::MANUFACTURERNO.getName()			+ ", "
			//		+ libabbauw::properties::EDIDELIVERYDATEMF.getName()		+", " );
			//		+ libabbauw::properties::PHARMACYNOMF..getName()			+"'" );
			//		+ ") "
			//		
			//		+ "VALUES ( "

			//		+ libabbauw::properties::BRANCHNO.toSQLReplacementString()				+", "	
			//		+ libabbauw::properties::ORDERNO.toSQLReplacementString()				+", " 
			//		+ libabbauw::properties::STATUS.toSQLReplacementString()				+", " 
			//		+ libabbauw::properties::ORDERTYPE.toSQLReplacementString()			+", " 
			//		+ libabbauw::properties::ORDERPICKINGTYPE.toSQLReplacementString()		+", " 
			//		+ libabbauw::properties::BOOKENTRYTYPE.toSQLReplacementString()		+", " 
			//		+ libabbauw::properties::ORDERDATE.toSQLReplacementString()			+", " 
			//		+ libabbauw::properties::ORDERVALUE.toSQLReplacementString()			+", " 
			//		+ libabbauw::properties::DEFERREDPAYMENTDATE.toSQLReplacementString()	+", " 
			//		+ libabbauw::properties::PHARMACYNO.toSQLReplacementString()			+", " 
			//		+ libabbauw::properties::HEADERTEXT.toSQLReplacementString()			+", " 
			//		+ libabbauw::properties::ORDERACQUISITIONDATE.toSQLReplacementString() +", " 
			//		+ libabbauw::properties::DATASOURCETYPE.toSQLReplacementString()		+", " 
			//		+ libabbauw::properties::EDIDELIVERYDATE.toSQLReplacementString()		+", " 
			//		+ libabbauw::properties::REFERENCE.toSQLReplacementString()			+", " 
			//		+ libabbauw::properties::ORDERNOCSC.toSQLReplacementString()			+", " 
			//		+ libabbauw::properties::ORDERNOMF.toSQLReplacementString()			+", " 
			//		+ libabbauw::properties::MANUFACTURERNO.toSQLReplacementString()		+", " 
			//		+ libabbauw::properties::EDIDELIVERYDATEMF.toSQLReplacementString()	+", " 
			//		+ libabbauw::properties::PHARMACYNOMF.toSQLReplacementString()			
			//		+ ")" ;
			//--- END for documenting purposes only! ---


//---------------------------------------------------------------------------------------------//
// accessor definition ACC_ORDERHEAD
//---------------------------------------------------------------------------------------------//
			// macros BEGIN.. and END.. care for creation and registration in the manager before main() was started.
			// kes 26.06.14: new property PHARMACYNOMF added
			BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::ACC_ORDERHEAD )
		        using namespace libabbauw::properties;

				BEGIN_PROPERTY_DESCRIPTION_LIST
					PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO );
					PROPERTY_DESCRIPTION_LIST_ADD( ORDERNO  );
					PROPERTY_DESCRIPTION_LIST_ADD( STATUS  );
					PROPERTY_DESCRIPTION_LIST_ADD( ORDERTYPE );
					PROPERTY_DESCRIPTION_LIST_ADD( ORDERPICKINGTYPE  );
					PROPERTY_DESCRIPTION_LIST_ADD( BOOKENTRYTYPE   );
					PROPERTY_DESCRIPTION_LIST_ADD( ORDERDATE    );
					PROPERTY_DESCRIPTION_LIST_ADD( ORDERVALUE  );
					PROPERTY_DESCRIPTION_LIST_ADD( DEFERREDPAYMENTDATE   );
					PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNO   );
					PROPERTY_DESCRIPTION_LIST_ADD( HEADERTEXT    );
					PROPERTY_DESCRIPTION_LIST_ADD( ORDERACQUISITIONDATE    );
					PROPERTY_DESCRIPTION_LIST_ADD( DATASOURCETYPE    );
					PROPERTY_DESCRIPTION_LIST_ADD( EDIDELIVERYDATE      );
					PROPERTY_DESCRIPTION_LIST_ADD( REFERENCE    );
					PROPERTY_DESCRIPTION_LIST_ADD( ORDERNOCSC     );
					PROPERTY_DESCRIPTION_LIST_ADD( ORDERNOMF     );
					PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTURERNO     );
					PROPERTY_DESCRIPTION_LIST_ADD( EDIDELIVERYDATEMF    );
					PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNOMF     );
				END_PROPERTY_DESCRIPTION_LIST



				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_MAX_ORDERNO_BY_BRANCHNO )
					SQL_BUILDER_CREATE( SelMaxOrdernoByBranchnoBuilder )
					SQL_BUILDER_PUSH_BACK( SelMaxOrdernoByBranchnoBuilder )


				ACCESS_METHOD( libabbauw::domMod::ACCMETH_INSERT_INTO_ORDERHEAD )
					SQL_BUILDER_CREATE		( InsertIntoOrderheadBuilder )
					SQL_BUILDER_PUSH_BACK	( InsertIntoOrderheadBuilder )

			END_ACCESSOR_DEFINITION


		} // namespace accOrderhead
	} // namespace abbauwDM
} // namespace domMod
