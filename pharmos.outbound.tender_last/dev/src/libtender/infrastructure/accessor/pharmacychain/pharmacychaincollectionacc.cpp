// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/pharmacychain/pharmacychaincollectionacc.h>
#include <infrastructure/accessor/pharmacychain/pharmacychaincollectionacc_definitions.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace pharmacychain
            {
                //---------------------------------------------------------------------------------------------//
                // accessor definition
                //---------------------------------------------------------------------------------------------//
                BEGIN_ACCESSOR_DEFINITION( accessor::pharmacychain::lit::PHARMACYCHAIN_COLLECTION_ACC )

                    BEGIN_PROPERTY_DESCRIPTION_LIST
					    PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO		  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO		  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_NAME	  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_STREET	  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_LOCATION  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_CHAIN_NO  )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::ORIGINAL_BRANCH_NO )
					END_PROPERTY_DESCRIPTION_LIST

                    ACCESS_METHOD( accessor::pharmacychain::lit::SELECT_PHARMACYCHAIN_BY_PATTERN );
                        SQL_BUILDER_CREATE   ( SelectPharmacyChainByPattern )
                        SQL_BUILDER_PUSH_BACK( SelectPharmacyChainByPattern )

					ACCESS_METHOD( accessor::pharmacychain::lit::SELECT_PHARMACYCHAIN_BY_PHARMACYCHAINNO );
						SQL_BUILDER_CREATE   ( SelectPharmacyChainByPharmacyChainNo )
						SQL_BUILDER_PUSH_BACK( SelectPharmacyChainByPharmacyChainNo )

					ACCESS_METHOD( accessor::pharmacychain::lit::SELECT_CUSTOMERGROUPID_BY_PHARMACYCHAIN );
						SQL_BUILDER_CREATE   ( SelectCustomerGroupIDByPharmacychain )
						SQL_BUILDER_PUSH_BACK( SelectCustomerGroupIDByPharmacychain )

                END_ACCESSOR_DEFINITION

                //////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectPharmacyChainByPattern )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectPharmacyChainByPattern )

                bool SelectPharmacyChainByPattern::isExecutable() const
                {
                    METHODNAME_DEF( SelectPharmacyChainByPattern, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = arePropertiesSet( properties::CUSTOMER_NO.toPropertyString()       ) ||
									  arePropertiesSet( properties::PHARMACY_NAME.toPropertyString()     ) ||	
									  arePropertiesSet( properties::PHARMACY_CHAIN_NO.toPropertyString() );

                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectPharmacyChainByPattern::buildSQLString()
                {
                    resolve(
							"SELECT "
							"k.idfnr AS "				+ properties::CUSTOMER_NO.getName()			+ ", "
							"k.filialnr AS "			+ properties::BRANCH_NO.getName()			+ ", "
							"k.mastercstno "			+ properties::PHARMACY_CHAIN_NO.getName()	+ ", "
							"k.nameapo AS "				+ properties::PHARMACY_NAME.getName()		+ ", "
							"k.strasse AS "				+ properties::PHARMACY_STREET.getName()		+ ", "
							"k.ort AS "					+ properties::PHARMACY_LOCATION.getName()	+ ", "
							"k.originalfilialnr AS "	+ properties::ORIGINAL_BRANCH_NO.getName()	+ ", "
							"k.personalnrvertr AS "		+ properties::PERSONAL_NR_VERTR.getName()	+ " "
						"FROM kunde AS k "
							"INNER JOIN kundeschalter AS ks ON k.idfnr=ks.idfnr AND k.filialnr=ks.vertriebszentrumnr "
						"WHERE ks.skdkundeaegesperrt='0' "
						"AND k.mastercstno!=0"
						   );

					if( arePropertiesSetAndOneMustBeInState(properties::PHARMACY_CHAIN_NO.toPropertyString(), basar::SS_CLEAN) )
                    {
                        resolve( " AND k.mastercstno = "  + properties::PHARMACY_CHAIN_NO.toSQLReplacementString() );
                    }

                    if( arePropertiesSetAndOneMustBeInState(properties::CUSTOMER_NO.toPropertyString(), basar::SS_CLEAN) )
                    {
                        resolve( " AND k.idfnr = "  + properties::CUSTOMER_NO.toSQLReplacementString() );
                    }

					if( arePropertiesSetAndOneMustBeInState(properties::PHARMACY_NAME.toPropertyString(), basar::SS_CLEAN) )
                    {
                        resolve( " AND UPPER(k.nameapo) LIKE UPPER('"  + properties::PHARMACY_NAME.toSQLReplacementString() + "%')" );
                    }

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

				//////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectPharmacyChainByPharmacyChainNo )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectPharmacyChainByPharmacyChainNo )

                bool SelectPharmacyChainByPharmacyChainNo::isExecutable() const
                {
                    METHODNAME_DEF( SelectPharmacyChainByPharmacyChainNo, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = false;

					if( arePropertiesSet( properties::PHARMACY_CHAIN_NO.toPropertyString() ) )
					{
						executable = true;
					}
				    
                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectPharmacyChainByPharmacyChainNo::buildSQLString()
                {
					resolve(
						"SELECT DISTINCT "
							"k.mastercstno AS "	+ properties::PHARMACY_CHAIN_NO.getName() + " "
						"FROM kunde AS k "
							"INNER JOIN kundeschalter AS ks ON k.idfnr=ks.idfnr AND k.filialnr=ks.vertriebszentrumnr "
						"WHERE "
							"ks.skdkundeaegesperrt='0' AND "
							"k.mastercstno="					+ properties::PHARMACY_CHAIN_NO.toSQLReplacementString()
						);

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

				//////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectCustomerGroupIDByPharmacychain )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectCustomerGroupIDByPharmacychain )

                bool SelectCustomerGroupIDByPharmacychain::isExecutable() const
                {
                    METHODNAME_DEF( SelectCustomerGroupIDByPharmacychain, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = false;

					if( arePropertiesSet( properties::PHARMACY_CHAIN_NO.toPropertyString() ) &&
						arePropertiesSet( properties::CUSTOMERGROUPID.toPropertyString() ) )
					{
						executable = true;
					}
				    
                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectCustomerGroupIDByPharmacychain::buildSQLString()
                {
					resolve( 
						"SELECT "
						"idfnr AS "			+	properties::CUSTOMER_NO.getName()		+ ", "  
						"mastercstno AS "	+	properties::PHARMACY_CHAIN_NO.getName() + ", "
						"kdgruppe AS "		+	properties::CUSTOMERGROUPID.getName()	+ " "
						"FROM kunde "
						"WHERE "
						"mastercstno = "	+ properties::PHARMACY_CHAIN_NO.toSQLReplacementString() + " "
						"AND kdgruppe = "	+ properties::CUSTOMERGROUPID.toSQLReplacementString()	 
						);

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

            } // end namespace customer
        } // end namespace accessor
    }   // end namespace infrastructure
}   // end namespace libtender
