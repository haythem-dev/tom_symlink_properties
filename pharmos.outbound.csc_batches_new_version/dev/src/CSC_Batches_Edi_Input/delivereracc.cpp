//-------------------------------------------------------------------------------------
/*! \file  delivereracc.h
*  \brief  Accessordefinitions for reading customerdata from table filiale etc. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   25.11.14 
*  history: 25.11.14  pk new
*/
//-------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------//
// include section 
//-------------------------------------------------------------------------------------------------//

#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers
#include "delivereracc.h"
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
		namespace accDeliverer
		{
			using basar::db::aspect::SQLStringBuilder;


//-------------------------------------------------------------------------------------------------//
// Builder Definition SelBranchnoByDelivererbgaBuilder
//-------------------------------------------------------------------------------------------------//

			// macro to define a SQLQueryStringBuilder-class
			BUILDER_DEFINITION( SelBranchnoByDelivererbgaBuilder )

				// isExecutable() must fit to its StringBuilders sql statement
				// Makro vgl. Kap. 5.10.2, S. 57: The macro constructs the isExecutable()-method
				// (belongs to class SQLStringBuilder, method isExecutable() {return arePropertiesSet()}    )
				// Therefore we don't need to define the isExecutable()-method by ourselves.

				ENSURE_PROPERTIES_ARE_SET(	SelBranchnoByDelivererbgaBuilder,			// Builder name
											libabbauw::properties::DELIVERERBGA.toPropertyString()
											)														// state


			// this method could be replaced by the SQL_PATTERN macro
			void SelBranchnoByDelivererbgaBuilder::buildSQLString()
			{
				basar::ConstString fun = "SelBranchnoByDelivererbgaBuilder::buildSQLString()"; 
				using namespace libabbauw::properties;			

				//LOG_YIT( getPropertyList(), abbaUW::LoggerPool::LoggerAbbaUW, fun );

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// const attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// Fixed part of the DML text
				// select filialnr from filiale where bganr = #delivererbga#
				basar::I18nString stmt;
				stmt.append( "SELECT " );		        
				stmt.append( "       f.filialnr AS "	+ BRANCHNO.getName() );  
				stmt.append(" FROM   filiale f " );
				stmt.append(" WHERE  f.bganr_ek = '"	+ DELIVERERBGA.toSQLReplacementString()  +"'" + ";");
				// orig: stmt.append(" WHERE  f.bganr = '"		+ DELIVERERBGA.toSQLReplacementString()  +"'" + ";");

				// variable part of the DML statement

				/////////////////////////////////////////////////////////////////////////////////////////////////
				// variable attributes
				/////////////////////////////////////////////////////////////////////////////////////////////////	
				// doesnot exist here

				// finish builder
				resolve( stmt );

				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
			}


//-------------------------------------------------------------------------------------------------//
// Accessor Definition ACC_DELIVERER
//-------------------------------------------------------------------------------------------------//
			// macros BEGIN.. and END.. care for creation and registration in the manager before main() was started.
			BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::ACC_DELIVERER )
		        using namespace libabbauw::properties;

				BEGIN_PROPERTY_DESCRIPTION_LIST
					PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO );
					PROPERTY_DESCRIPTION_LIST_ADD( DELIVERERBGA  );
				END_PROPERTY_DESCRIPTION_LIST

				ACCESS_METHOD( libabbauw::domMod::ACCMETH_SELECT_BRANCHNO_BY_DELIVERERBGA )
					SQL_BUILDER_CREATE( SelBranchnoByDelivererbgaBuilder )
					SQL_BUILDER_PUSH_BACK( SelBranchnoByDelivererbgaBuilder )
					
			END_ACCESSOR_DEFINITION



		} // namespace accDeliverer
	} // namespace abbauwDM
} // namespace domMod
