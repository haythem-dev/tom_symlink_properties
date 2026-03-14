#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers

#include "orderposcollectionprotocolacc.h"
#include <libbasardbaspect.h>
#include "loggerpool.h"

#include "edi_input_properties_definitions.h"
#include "edi_input_accessor_definitions.h"

#include <libbasarproperty_propertydescriptionlistref.h>


namespace domMod
{
	namespace accessor
	{
		namespace orderProtocol
		{
			using basar::db::aspect::SQLStringBuilder;

			BUILDER_DEFINITION( SaveOrderPosCollectionProtocol )

			ENSURE_PROPERTIES_ARE_SET(	SaveOrderPosCollectionProtocol,
										  libabbauw::properties::ORDERNO.toPropertyString()	
										+ libabbauw::properties::POSNO.toPropertyString()
									 )														


			void SaveOrderPosCollectionProtocol::buildSQLString()
			{
				basar::ConstString fun = "SaveOrderPosCollectionProtocol::buildSQLString()"; 
				using namespace libabbauw::properties;			
  
		        static const basar::I18nString sqlStatement( 
                    "INSERT INTO orderposprotocol "
		        	"( "
		        	+ BRANCHNO.getName()			+ ", "
		        	+ ORDERNO.getName()				+ ", "
                    + PROTOCOL_DATE.getName()       + ", "
		        	+ POSNO.getName()				+ ", "
		        	+ ACTIVITY_TYPE.getName()		+ ", "
		        	+ EVENT_TYPE.getName()			+ ", "
		        	+ PROCESSED_BY.getName()		+ ", "
		        	+ ARTICLENO.getName()			+ ", "
		        	+ ORDEREDQTY.getName()			+ ", "
		        	+ NONCHARGEDQTY.getName()		+ ", "
		        	+ DISCOUNTPCT.getName()			+ ", "
		        	+ VIEW.getName()				+ ", "
		        	+ ORDERREQUESTQTY.getName()	    + ", "
		        	+ ITEMTEXT.getName()	        + ", "
                    + EXTRATEXT.getName()	        + " "
                    ") "
		        	"VALUES (" 
		        	+ BRANCHNO.toSQLReplacementString()				 + ", " 
                    + ORDERNO.toSQLReplacementString()				 + ", " 
                    + "CURRENT"                                      + ", "
		        	+ POSNO.toSQLReplacementString()				 + ", "
		        	+ ACTIVITY_TYPE.toSQLReplacementString("'", "'") + ", " 
		        	+ EVENT_TYPE.toSQLReplacementString()			 + ", " 
		        	+ PROCESSED_BY.toSQLReplacementString()			 + ", "
		        	+ ARTICLENO.toSQLReplacementString()			 + ", " 
		        	+ ORDEREDQTY.toSQLReplacementString()			 + ", "
                    + NONCHARGEDQTY.toSQLReplacementString()         + ", "  
		        	+ DISCOUNTPCT.toSQLReplacementString()	         + ", " 
                    + VIEW.toSQLReplacementString("'", "'")          + ", "
		        	+ ORDERREQUESTQTY.toSQLReplacementString()		 + ", " 
		        	+ ITEMTEXT.toSQLReplacementString("'", "'")		 + ", "
                    + EXTRATEXT.toSQLReplacementString("'", "'")	 + " "
                    ")"
                );

                resolve(sqlStatement);

				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
			}

			BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::ACC_ORDERPOSCOLLECTIONPROTOCOL )
		        using namespace libabbauw::properties;

				BEGIN_PROPERTY_DESCRIPTION_LIST
					PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO          );
		            PROPERTY_DESCRIPTION_LIST_ADD( ORDERNO           );
			        PROPERTY_DESCRIPTION_LIST_ADD( POSNO			 );
			        PROPERTY_DESCRIPTION_LIST_ADD( PROTOCOL_DATE     );
			        PROPERTY_DESCRIPTION_LIST_ADD( ACTIVITY_TYPE     );
			        PROPERTY_DESCRIPTION_LIST_ADD( EVENT_TYPE        );
			        PROPERTY_DESCRIPTION_LIST_ADD( PROCESSED_BY		 );
			        PROPERTY_DESCRIPTION_LIST_ADD( ARTICLENO		 );
			        PROPERTY_DESCRIPTION_LIST_ADD( ORDEREDQTY		 );
			        PROPERTY_DESCRIPTION_LIST_ADD( NONCHARGEDQTY	 );
			        PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTPCT		 );
					PROPERTY_DESCRIPTION_LIST_ADD( DISCOUNTTYPE      );
			        PROPERTY_DESCRIPTION_LIST_ADD( VIEW				 );
                    PROPERTY_DESCRIPTION_LIST_ADD( ORDERREQUESTQTY	 );
                    PROPERTY_DESCRIPTION_LIST_ADD( ITEMTEXT	         );
                    PROPERTY_DESCRIPTION_LIST_ADD( EXTRATEXT	     );
                END_PROPERTY_DESCRIPTION_LIST

				ACCESS_METHOD( libabbauw::domMod::ACC_ORDERPOSCOLLECTIONPROTOCOL_SAVE )
					SQL_BUILDER_CREATE( SaveOrderPosCollectionProtocol )
					SQL_BUILDER_PUSH_BACK( SaveOrderPosCollectionProtocol )

			END_ACCESSOR_DEFINITION


		} // namespace accCustomer
	} // namespace abbauwDM
} // namespace domMod
