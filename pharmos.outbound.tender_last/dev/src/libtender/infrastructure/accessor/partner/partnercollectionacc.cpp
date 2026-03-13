// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/partner/partnercollectionacc.h>
#include <infrastructure/accessor/partner/partnercollectionacc_definitions.h>

#include <domainmodule/article/businesstypemapper.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace partner
            {
                //---------------------------------------------------------------------------------------------//
                // accessor definition
                //---------------------------------------------------------------------------------------------//
                BEGIN_ACCESSOR_DEFINITION( accessor::partner::lit::PARTNER_COLLECTION_ACC )

                    BEGIN_PROPERTY_DESCRIPTION_LIST
					    PROPERTY_DESCRIPTION_LIST_ADD( properties::PARTNER_NO   )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::PARTNER_NAME )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::PARTNER_STREET )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::PARTNER_LOCATION )
                    END_PROPERTY_DESCRIPTION_LIST

                        ACCESS_METHOD( accessor::partner::lit::SELECT_PARTNER_BY_PATTERN );
                        SQL_BUILDER_CREATE   ( SelectPartnerByPattern )
                        SQL_BUILDER_PUSH_BACK( SelectPartnerByPattern )

						ACCESS_METHOD( accessor::partner::lit::SELECT_PARTNER_BY_PARTNER_NO );
						SQL_BUILDER_CREATE   ( SelectPartnerByPartnerNo )
						SQL_BUILDER_PUSH_BACK( SelectPartnerByPartnerNo )

                END_ACCESSOR_DEFINITION

                //////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectPartnerByPattern )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectPartnerByPattern )

                bool SelectPartnerByPattern::isExecutable() const
                {
                    METHODNAME_DEF( SelectCustomerByPattern, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = arePropertiesSet( properties::PARTNER_NO.toPropertyString()   ) ||
									  arePropertiesSet( properties::PARTNER_NAME.toPropertyString() );

                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectPartnerByPattern::buildSQLString()
                {
                    if( arePropertiesSetAndOneMustBeInState( properties::BUSINESS_TYPE.toPropertyString(), basar::SS_CLEAN) )
					{
						if( domMod::article::BT_SRA == getPropertyList().getInt16( properties::BUSINESS_TYPE) )
						{
							resolve(
								"SELECT hersteller_nr AS "          + properties::PARTNER_NO.getName() +       ", "
								"name AS "							+ properties::PARTNER_NAME.getName() +     ", "
								"strasse AS "						+ properties::PARTNER_STREET.getName() +   ", "
								"ort AS "							+ properties::PARTNER_LOCATION.getName() + " "
								"FROM manufacturer " 
								"WHERE konsig_partner_nr = 2875 "
							   );

							//resolve(
							//	"SELECT l.lant_nr AS "				+ properties::PARTNER_NO.getName() +       ", "
							//	"l.lant_name AS "					+ properties::PARTNER_NAME.getName() +     ", "
							//	"l.strasse AS "						+ properties::PARTNER_STREET.getName() +   ", "
							//	"l.ort AS "							+ properties::PARTNER_LOCATION.getName() + " "
							//	"FROM liefer as l " 
							//	"INNER JOIN herstel as h on l.hersteller_nr = h.hersteller_nr "
							//	"WHERE h.konsig_partner_nr = 2875 "
							//   );
						}
						else
						{
							resolve(
								"SELECT hersteller_nr AS "     + properties::PARTNER_NO.getName() +       ", "
								"name AS "					   + properties::PARTNER_NAME.getName() +     ", "
								"strasse AS "                  + properties::PARTNER_STREET.getName() +   ", "
								"ort AS "                      + properties::PARTNER_LOCATION.getName() + "  "
								"FROM manufacturer " 
							    "WHERE businesstypeno = "      + properties::BUSINESS_TYPE.toSQLReplacementString() + " "
								);

							//resolve(
							//	"SELECT l.lant_nr AS "			 + properties::PARTNER_NO.getName() +       ", "
							//	"l.lant_name AS "				 + properties::PARTNER_NAME.getName() +     ", "
							//	"l.strasse AS "                  + properties::PARTNER_STREET.getName() +   ", "
							//	"l.ort AS "                      + properties::PARTNER_LOCATION.getName() + "  "
							//	"FROM liefer as l " 
							//	"INNER JOIN herstel as h on l.hersteller_nr = h.hersteller_nr "
							//    "WHERE h.businesstypeno = "      + properties::BUSINESS_TYPE.toSQLReplacementString() + " "
							//	);
						}
					}
					else
					{
						resolve(
								"SELECT hersteller_nr AS "     + properties::PARTNER_NO.getName() +       ", "
								"name AS "					   + properties::PARTNER_NAME.getName() +     ", "
								"strasse AS "                  + properties::PARTNER_STREET.getName() +   ", "
								"ort AS "                      + properties::PARTNER_LOCATION.getName() + "  "
								"FROM manufacturer " 
								"WHERE businesstypeno > 0 "
								);

						//resolve(
						//		"SELECT l.lant_nr AS "			 + properties::PARTNER_NO.getName() +       ", "
						//		"l.lant_name AS "			     + properties::PARTNER_NAME.getName() +     ", "
						//		"l.strasse AS "                  + properties::PARTNER_STREET.getName() +   ", "
						//		"l.ort AS "                      + properties::PARTNER_LOCATION.getName() + "  "
						//		"FROM liefer as l "
						//		"INNER JOIN herstel as h on l.hersteller_nr = h.hersteller_nr " 
						//		"WHERE businesstypeno > 0 "
						//		);
					}

					if( arePropertiesSetAndOneMustBeInState( properties::PARTNER_NO.toPropertyString(), basar::SS_CLEAN) )
					{
					    resolve( " AND hersteller_nr = "  + properties::PARTNER_NO.toSQLReplacementString() );
					}

					if( arePropertiesSetAndOneMustBeInState( properties::PARTNER_NAME.toPropertyString(), basar::SS_CLEAN) )
					{
					    resolve( " AND UPPER(name) LIKE UPPER('"  + properties::PARTNER_NAME.toSQLReplacementString() + "%') " );
					}

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

				//////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectPartnerByPartnerNo )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectPartnerByPartnerNo )

                bool SelectPartnerByPartnerNo::isExecutable() const
                {
                    METHODNAME_DEF( SelectPartnerByPartnerNo, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = false;

					if( arePropertiesSet( properties::PARTNER_NO.toPropertyString() ) )
					{
						executable = true;
					}
				    
                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectPartnerByPartnerNo::buildSQLString()
                {
					resolve(
						"SELECT DISTINCT "
							"h.hersteller_nr AS "		+ properties::PARTNER_NO.getName()			+ ", "
							"h.name AS "				+ properties::PARTNER_NAME.getName()		+ ", "
							"h.strasse AS "				+ properties::PARTNER_STREET.getName()		+ ", "
							"h.ort AS "					+ properties::PARTNER_LOCATION.getName()	+ " "
						"FROM manufacturer AS h "
						"WHERE h.hersteller_nr="		+ properties::PARTNER_NO.toSQLReplacementString()	+ " "
						"AND businesstypeno = "			+ properties::BUSINESS_TYPE.toSQLReplacementString()	+ " "
						);

					//resolve(
					//	"SELECT DISTINCT "
					//		"l.lant_nr AS "				+ properties::PARTNER_NO.getName()			+ ", "
					//		"l.lant_name AS "			+ properties::PARTNER_NAME.getName()		+ ", "
					//		"l.strasse AS "				+ properties::PARTNER_STREET.getName()		+ ", "
					//		"l.ort AS "					+ properties::PARTNER_LOCATION.getName()	+ " "
					//	"FROM liefer AS l "
					//	"INNER JOIN herstel as h on l.hersteller_nr = h.hersteller_nr " 
					//	"WHERE l.lant_nr="				+ properties::PARTNER_NO.toSQLReplacementString()	+ " "
					//	"AND h.businesstypeno = "		+ properties::BUSINESS_TYPE.toSQLReplacementString()	+ " "
					//	);

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

            } // end namespace customer
        } // end namespace accessor
    }   // end namespace infrastructure
}   // end namespace libtender
