//-------------------------------------------------------------------------------------
/*! \file  customeracc.cpp
*  \brief  Accessordefinitions for reading customerdata from table kunde etc. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   03.05.2013 
*  history: 03.05.13 pk new
*/
//-------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------//
// include section 
//-------------------------------------------------------------------------------------------------//

#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers
#include "customeracc.h"
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
		namespace accCustomer
		{
			using basar::db::aspect::SQLStringBuilder;


//-------------------------------------------------------------------------------------------------//
// Builder Definition SelCustomergroupByBranchnoAndPharmacynoBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelCustomergroupByBranchnoAndPharmacynoBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// (belongs to class SQLStringBuilder, method isExecutable() {return arePropertiesSet()}    )
				// Therefore we don't need to define the isExecutable()-method by ourselves.

				ENSURE_PROPERTIES_ARE_SET(	SelCustomergroupByBranchnoAndPharmacynoBuilder,			// Builder name
											  libabbauw::properties::BRANCHNO.toPropertyString()		// property names
											+ libabbauw::properties::PHARMACYNO.toPropertyString()
											)														// state


			// this method could be replaced by the SQL_PATTERN macro
			void SelCustomergroupByBranchnoAndPharmacynoBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelCustomergroupByBranchnoAndPharmacynoBuilder::buildSQLString()"; 
				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// select kdgruppe from kunde where filialnr = #branchno# and idfnr = #pharmacyno#    
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       k.kdgruppe AS "		+ CUSTOMERGROUP.getName() );  
				stmt.append(" FROM   kunde k " );
				stmt.append(" WHERE  k.filialnr = " + BRANCHNO.toSQLReplacementString() );
				stmt.append("   AND  k.idfnr	= " + PHARMACYNO.toSQLReplacementString() + ";");

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
			//! \ingroup SQLSTMT  sql-statement of SelCustomergroupByBranchnoAndPharmacynoBuilder
			//basar::ConstString SelCustomergroupByBranchnoAndPharmacynoBuilder::s_SqlStmt ; 	             
			//s_SqlStmt.append( "SELECT " );		        
			//s_SqlStmt.append( "       k.kdgruppe AS "		+ CUSTOMERGROUP.getName() );  
			//s_SqlStmt.append(" FROM   kunde k " );
			//s_SqlStmt.append(" WHERE  k.filialnr = " + BRANCHNO.toSQLReplacementString() );
			//s_SqlStmt.append("   AND  k.idfnr	= " + PHARMACYNO.toSQLReplacementString() + ";");
			//--- END for documenting purposes only! ---



//-------------------------------------------------------------------------------------------------//
// Builder Definition SelPharmacynoByBranchnoAndPharmacynoeanBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelPharmacynoByBranchnoAndPharmacynoeanBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// (belongs to class SQLStringBuilder, method isExecutable() {return arePropertiesSet()}    )
				// Therefore we don't need to define the isExecutable()-method by ourselves.

				ENSURE_PROPERTIES_ARE_SET(	SelPharmacynoByBranchnoAndPharmacynoeanBuilder,			// Builder name
											  libabbauw::properties::BRANCHNO.toPropertyString()	// property names
											+ libabbauw::properties::PHARMACYNOEAN.toPropertyString()
											)														// state


			// this method could be replaced by the SQL_PATTERN macro
			void SelPharmacynoByBranchnoAndPharmacynoeanBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelPharmacynoByBranchnoAndPharmacynoeanBuilder::buildSQLString()"; 
				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// select idfnr from kunde where filialnr = #branchno# and kundeean = '#pharmacyno#'    
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       k.idfnr AS "		+ PHARMACYNO.getName() );  
				stmt.append(" FROM   kunde k " );
				stmt.append(" WHERE  k.filialnr = "		+ BRANCHNO.toSQLReplacementString() );
				stmt.append("   AND  k.kundeean	= '"	+ PHARMACYNOEAN.toSQLReplacementString() +"'" + ";");

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
			//! \ingroup SQLSTMT  sql-statement of SelPharmacynoByBranchnoAndPharmacynoeanBuilder
			//basar::ConstString SelPharmacynoByBranchnoAndPharmacynoeanBuilder::s_SqlStmt ; 	             
				//s_SqlStmt.append( "SELECT " );		        
				//s_SqlStmt.append( "       k.idfnr AS "		+ PHARMACYNO.getName() );  
				//s_SqlStmt.append(" FROM   kunde k " );
				//s_SqlStmt.append(" WHERE  k.filialnr = "		+ BRANCHNO.toSQLReplacementString() );
				//s_SqlStmt.append("   AND  k.kundeean	= '"	+ PHARMACYNOEAN.toSQLReplacementString() +"'" + ";");
			//--- END for documenting purposes only! ---


			
//-------------------------------------------------------------------------------------------------//
// Builder Definition SelPseudopharmacynoByBranchnoBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelPseudopharmacynoByBranchnoBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// (belongs to class SQLStringBuilder, method isExecutable() {return arePropertiesSet()}    )
				// Therefore we don't need to define the isExecutable()-method by ourselves.

				ENSURE_PROPERTIES_ARE_SET(	SelPseudopharmacynoByBranchnoBuilder,			// Builder name
											  libabbauw::properties::BRANCHNO.toPropertyString()	// property names
											)														// state


			// this method could be replaced by the SQL_PATTERN macro
			void SelPseudopharmacynoByBranchnoBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelPseudopharmacynoByBranchnoBuilder::buildSQLString()"; 
				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// select idfnr from kundeschalter where vertriebszentrumnr = #branchno# and skdpseudoidfnr_uw=1 
				// "FIRST 1": if more than one idfnr is defined as pseudoidfnr: Get the first one of them.
				//            (nur ersten Datensatz aus der Treffermenge liefern)
				basar::I18nString stmt;
				stmt.append( "SELECT FIRST 1" );		        
				stmt.append( "       k.idfnr AS "		+ PHARMACYNO.getName() );  
				stmt.append(" FROM   kundeschalter k " );
				stmt.append(" WHERE  k.vertriebszentrumnr = "		+ BRANCHNO.toSQLReplacementString() );
				stmt.append("   AND  k.skdpseudoidfnr_uw	= 1");
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
			//! \ingroup SQLSTMT  sql-statement of SelPseudopharmacynoByBranchnoBuilder
			//basar::ConstString SelPseudopharmacynoByBranchnoBuilder::s_SqlStmt ; 	             
				//s_SqlStmt.append( "SELECT " );		        
				//s_SqlStmt.append( "       k.idfnr AS "		+ PHARMACYNO.getName() );  
				//s_SqlStmt.append(" FROM   kundeschalter k " );
				//s_SqlStmt.append(" WHERE  k.vertriebszentrumnr = "		+ BRANCHNO.toSQLReplacementString() );
				//s_SqlStmt.append("   AND  k.skdpseudoidfnr_uw	= 1"	+ ";");
			//--- END for documenting purposes only! ---

//-------------------------------------------------------------------------------------------------//
// Accessor Definition ACC_CUSTOMER
//-------------------------------------------------------------------------------------------------//
			// macros BEGIN.. and END.. care for creation and registration in the manager before main() was started.
			BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::ACC_CUSTOMER )
		        using namespace libabbauw::properties;

				BEGIN_PROPERTY_DESCRIPTION_LIST
					PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO );
					PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNO  );
					PROPERTY_DESCRIPTION_LIST_ADD( CUSTOMERGROUP  );
					PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNOEAN  );
				END_PROPERTY_DESCRIPTION_LIST

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_CUSTOMERGROUP_BY_BRANCHNO_PHARMCYNO )
					SQL_BUILDER_CREATE( SelCustomergroupByBranchnoAndPharmacynoBuilder )
					SQL_BUILDER_PUSH_BACK( SelCustomergroupByBranchnoAndPharmacynoBuilder )

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_PHARMACYNO_BY_BRANCHNO_PHARMCYNOEAN )
					SQL_BUILDER_CREATE( SelPharmacynoByBranchnoAndPharmacynoeanBuilder )
					SQL_BUILDER_PUSH_BACK( SelPharmacynoByBranchnoAndPharmacynoeanBuilder )
					
				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_PSEUDOPHARMACYNO_BY_BRANCHNO )
					SQL_BUILDER_CREATE( SelPseudopharmacynoByBranchnoBuilder )
					SQL_BUILDER_PUSH_BACK( SelPseudopharmacynoByBranchnoBuilder )

			END_ACCESSOR_DEFINITION


		} // namespace accCustomer
	} // namespace abbauwDM
} // namespace domMod
