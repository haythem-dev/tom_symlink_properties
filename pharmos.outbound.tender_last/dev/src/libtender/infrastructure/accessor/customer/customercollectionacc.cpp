// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/customer/customercollectionacc.h>
#include <infrastructure/accessor/customer/customercollectionacc_definitions.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace customer
            {
                //---------------------------------------------------------------------------------------------//
                // accessor definition
                //---------------------------------------------------------------------------------------------//
                BEGIN_ACCESSOR_DEFINITION( accessor::customer::lit::CUSTOMER_COLLECTION_ACC )

                    BEGIN_PROPERTY_DESCRIPTION_LIST
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_NAME )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_STREET )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_LOCATION )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::ORIGINAL_BRANCH_NO )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::PERSONAL_NR_VERTR )
                        /////////////////////////////////////////////////////////////
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_GROUPID )
                        //////////////////////////////////////////////////////////////////////
                    END_PROPERTY_DESCRIPTION_LIST

                        ACCESS_METHOD( accessor::customer::lit::SELECT_CUSTOMER_BY_PATTERN );
                        SQL_BUILDER_CREATE( SelectCustomerByPattern )
                        SQL_BUILDER_PUSH_BACK( SelectCustomerByPattern )

						ACCESS_METHOD( accessor::customer::lit::SELECT_CUSTOMER_BY_CUSTOMER_NO );
						SQL_BUILDER_CREATE( SelectCustomerByCustomerNo )
						SQL_BUILDER_PUSH_BACK( SelectCustomerByCustomerNo )

                        ACCESS_METHOD( accessor::customer::lit::SELECT_CUSTOMER_BY_PHARMACY_GROUP_ID );
                        SQL_BUILDER_CREATE( SelectCustomerByPharmacyGroupId )
                        SQL_BUILDER_PUSH_BACK( SelectCustomerByPharmacyGroupId )


                END_ACCESSOR_DEFINITION

                //////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectCustomerByPattern )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectCustomerByPattern )

                bool SelectCustomerByPattern::isExecutable() const
                {
                    METHODNAME_DEF( SelectCustomerByPattern, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = false;

					if( arePropertiesSet( properties::CUSTOMER_NO.toPropertyString() ) || arePropertiesSet( properties::PHARMACY_NAME.toPropertyString() ) )
					{
						executable = true;
					}
				    
                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectCustomerByPattern::buildSQLString()
                {
                    /////////////////////////////////////////////////////////////////////////////////////////////////
                    // const attributes
                    /////////////////////////////////////////////////////////////////////////////////////////////////
					static const basar::VarString constSQL(
						"SELECT "
							"k.idfnr AS "				+ properties::CUSTOMER_NO.getName()			+ ", "
							"k.filialnr AS "			+ properties::BRANCH_NO.getName()			+ ", "
							"k.nameapo AS "				+ properties::PHARMACY_NAME.getName()		+ ", "
							"k.strasse AS "				+ properties::PHARMACY_STREET.getName()		+ ", "
							"k.ort AS "					+ properties::PHARMACY_LOCATION.getName()	+ ", "
							"k.originalfilialnr AS "	+ properties::ORIGINAL_BRANCH_NO.getName()  + ", "
							"k.personalnrvertr AS "		+ properties::PERSONAL_NR_VERTR.getName()	+ " "
						"FROM kunde AS k "
							"INNER JOIN kundeschalter AS ks ON k.idfnr=ks.idfnr AND k.filialnr=ks.vertriebszentrumnr "
						"WHERE ks.skdkundeaegesperrt='0' " );

                    basar::VarString variableSQL;

                    if( arePropertiesSetAndOneMustBeInState(properties::CUSTOMER_NO.toPropertyString(), basar::SS_CLEAN) )
                    {
                        variableSQL.append( " AND k.idfnr = "  + properties::CUSTOMER_NO.toSQLReplacementString() );
                    }

					if( arePropertiesSetAndOneMustBeInState(properties::PHARMACY_NAME.toPropertyString(), basar::SS_CLEAN) )
                    {
                        variableSQL.append( " AND UPPER(k.nameapo) LIKE UPPER('"  + properties::PHARMACY_NAME.toSQLReplacementString() + "%')" );
                    }
					
					if( arePropertiesSetAndOneMustBeInState( properties::BRANCH_NO.toPropertyString(), basar::SS_CLEAN ) )
					{
						variableSQL.append(" AND k.filialnr = " + properties::BRANCH_NO.toSQLReplacementString() );
					}

					basar::VarString organizeResult(" ORDER BY k.idfnr ASC");

                    resolve( constSQL + variableSQL + organizeResult);

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

                //////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectCustomerByCustomerNo )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectCustomerByCustomerNo )

                bool SelectCustomerByCustomerNo::isExecutable() const
                {
                    METHODNAME_DEF( SelectCustomerByCustomerNo, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = false;

					if( arePropertiesSet( properties::CUSTOMER_NO.toPropertyString() ) )
					{
						executable = true;
					}
				    
                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectCustomerByCustomerNo::buildSQLString()
                {
					resolve(
						"SELECT DISTINCT "
							"k.filialnr AS "			+ properties::BRANCH_NO.getName()			+ ", "
							"k.idfnr AS "				+ properties::CUSTOMER_NO.getName()			+ ", "
							"k.nameapo AS "				+ properties::PHARMACY_NAME.getName()		+ ", "
							"k.strasse AS "				+ properties::PHARMACY_STREET.getName()		+ ", "
							"k.ort AS "					+ properties::PHARMACY_LOCATION.getName()	+ ", "
							"k.originalfilialnr AS "	+ properties::ORIGINAL_BRANCH_NO.getName()	+ ", "
							"k.personalnrvertr AS "		+ properties::PERSONAL_NR_VERTR.getName()	+ " "
						"FROM kunde AS k "
							"INNER JOIN kundeschalter AS ks ON k.idfnr=ks.idfnr AND k.filialnr=ks.vertriebszentrumnr "
						"WHERE "
							"ks.skdkundeaegesperrt='0' AND "
							"k.idfnr="					+ properties::CUSTOMER_NO.toSQLReplacementString()
						);

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

                //////////////////////////////////////////////////////////////////////////////////////////
                //BUILDER_DEFINITION( SelectCustomerByPharmacyGroupId )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectCustomerByPharmacyGroupId )

                bool SelectCustomerByPharmacyGroupId::isExecutable() const
                {
                    METHODNAME_DEF( SelectCustomerByPharmacyGroupId, isExecutable );
                    static const log4cplus::Logger & logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

                    const bool executable = arePropertiesSet( properties::PHARMACY_GROUPID.toPropertyString() );

                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectCustomerByPharmacyGroupId::buildSQLString()
                {

                    using namespace properties;
                    using basar::VarString;

                    static const VarString & customerno = CUSTOMER_NO.getName();
                    static const VarString & pharmacyname = PHARMACY_NAME.getName();
                    static const VarString & location = PHARMACY_LOCATION.getName();
                    static const VarString & branch = BRANCH_NO.getName();
                   
                    static const basar::VarString constSQL("Select "
                            "k.idfnr AS "                           + customerno + ", "
                            "k.nameapo AS "                         + pharmacyname + ", "
                            "k.ort AS "                             + location + ", "
                            "cpg.branchno AS "                      + branch + " "
                            "FROM kunde AS k "
                            "INNER JOIN customerpharmacygr AS cpg ON k.idfnr = cpg.customerno AND k.filialnr = cpg.branchno "
                            "INNER JOIN kundeschalter AS ks ON k.idfnr = ks.idfnr AND ks.vertriebszentrumnr = cpg.branchno "
                            "INNER JOIN filiale AS f ON f.filialnr = cpg.branchno "
                            "WHERE "
                            "cpg.pharmacygroupid = '" + properties::PHARMACY_GROUPID.toSQLReplacementString() + "'"
                            "AND k.originalfilialnr = 0 AND ks.skdkundeaegesperrt = '0' order by 1"
                            );
                    resolve( constSQL );

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

            } // end namespace customer
        } // end namespace accessor
    }   // end namespace infrastructure
}   // end namespace libtender
