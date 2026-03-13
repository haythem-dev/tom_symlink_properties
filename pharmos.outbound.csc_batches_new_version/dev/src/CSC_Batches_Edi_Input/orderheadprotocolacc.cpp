#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers

#include "orderheadprotocolacc.h"
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

			BUILDER_DEFINITION( SaveOrderHeadProtocol )

			ENSURE_PROPERTIES_ARE_SET( SaveOrderHeadProtocol, 
                                       libabbauw::properties::BRANCHNO.toPropertyString() + 
                                       libabbauw::properties::ORDERNO.toPropertyString() )														

			void SaveOrderHeadProtocol::buildSQLString()
			{
				basar::ConstString fun = "SaveOrderHeadProtocol::buildSQLString()"; 
				using namespace libabbauw::properties;			
  
				static const basar::I18nString sqlStatement( 
                    "INSERT INTO orderheadprotocol "
		        	"( "
		        	+ BRANCHNO.getName()			+ ", "
		        	+ ORDERNO.getName()				+ ", "
		        	+ PROTOCOL_DATE.getName()		+ ", "
		        	+ ACTIVITY_TYPE.getName()		+ ", "
		        	+ EVENT_TYPE.getName()			+ ", "
		        	+ PROCESSED_BY.getName()		+ ", "
		        	+ CHANGED_ATTRIBUTE.getName()	+ ", "
		        	+ OLD_VALUE.getName()			+ ", "
		        	+ NEW_VALUE.getName()			+ ", "
		        	+ SPLIT_FROM.getName()			+ ", "
                    + SPLIT_TO.getName()			+ ", "
		        	+ HEADERTEXT.getName()			+ ", "
                    + ORDERNOCSC.getName()			+ ", "
                    + EXTRATEXT.getName()			+ " "
                    " ) "
		        	"VALUES "
                    "( "
		        	+ BRANCHNO.toSQLReplacementString()				    + ", " 
                    + ORDERNO.toSQLReplacementString()				    + ", " 
                    + "CURRENT"		                                    + ", " 
		        	+ "'" + ACTIVITY_TYPE.toSQLReplacementString()		+ "', " 
		        	+ EVENT_TYPE.toSQLReplacementString()			    + ", "
		        	+ PROCESSED_BY.toSQLReplacementString()			    + ", " 
		        	+ "'" + CHANGED_ATTRIBUTE.toSQLReplacementString()	+ "', " 
		        	+ "'" + OLD_VALUE.toSQLReplacementString()			+ "', " 
		        	+ "'" + NEW_VALUE.toSQLReplacementString()			+ "', " 
		        	+ SPLIT_FROM.toSQLReplacementString()				+ ", "
                    + SPLIT_TO.toSQLReplacementString()				    + ", "
                    + HEADERTEXT.toSQLReplacementString("'","'")		+ ", "
                    + ORDERNOCSC.toSQLReplacementString()				+ ", "
                    + EXTRATEXT.toSQLReplacementString("'","'")			+ " "
		        	" )"
                );

                resolve(sqlStatement);

				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun + getSQLString() );
			}

			BEGIN_ACCESSOR_DEFINITION( libabbauw::domMod::ACC_ORDERHEADPROTOCOL )
		        using namespace libabbauw::properties;

				BEGIN_PROPERTY_DESCRIPTION_LIST
					PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO          );
		            PROPERTY_DESCRIPTION_LIST_ADD( ORDERNO           );
			        PROPERTY_DESCRIPTION_LIST_ADD( PROTOCOL_DATE     );
			        PROPERTY_DESCRIPTION_LIST_ADD( ACTIVITY_TYPE     );
			        PROPERTY_DESCRIPTION_LIST_ADD( EVENT_TYPE        );
			        PROPERTY_DESCRIPTION_LIST_ADD( PROCESSED_BY      );
			        PROPERTY_DESCRIPTION_LIST_ADD( CHANGED_ATTRIBUTE );
			        PROPERTY_DESCRIPTION_LIST_ADD( OLD_VALUE         );
			        PROPERTY_DESCRIPTION_LIST_ADD( NEW_VALUE         );
			        PROPERTY_DESCRIPTION_LIST_ADD( SPLIT_FROM        );
				    PROPERTY_DESCRIPTION_LIST_ADD( SPLIT_TO          );
                    PROPERTY_DESCRIPTION_LIST_ADD( HEADERTEXT        );
                    PROPERTY_DESCRIPTION_LIST_ADD( ORDERNOCSC        );
                    PROPERTY_DESCRIPTION_LIST_ADD( EXTRATEXT         );
                END_PROPERTY_DESCRIPTION_LIST

				ACCESS_METHOD( libabbauw::domMod::ACC_ORDERHEADPROTOCOL_SAVE )
					SQL_BUILDER_CREATE( SaveOrderHeadProtocol )
					SQL_BUILDER_PUSH_BACK( SaveOrderHeadProtocol )

			END_ACCESSOR_DEFINITION


		} // namespace accCustomer
	} // namespace abbauwDM
} // namespace domMod
