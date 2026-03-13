// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/customerlist/customerlistacc.h>
#include <infrastructure/accessor/customerlist/customerlistacc_definitions.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace customerlist
            {
                //---------------------------------------------------------------------------------------------//
                // accessor definition
                //---------------------------------------------------------------------------------------------//
                BEGIN_ACCESSOR_DEFINITION( accessor::customerlist::lit::CUSTOMER_LIST_ACC )

                    BEGIN_PROPERTY_DESCRIPTION_LIST
						PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_NO )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO )
						PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_NAME )
                    END_PROPERTY_DESCRIPTION_LIST

                        ACCESS_METHOD( accessor::customerlist::lit::SELECT_CUSTOMER_BY_PATTERN );
                        SQL_BUILDER_CREATE   ( SelectCustomerByPattern )
                        SQL_BUILDER_PUSH_BACK( SelectCustomerByPattern )

						ACCESS_METHOD( accessor::customerlist::lit::SELECT_CUSTOMER_BY_TENDER_NO );
						SQL_BUILDER_CREATE   ( SelectCustomerByTenderNo )
						SQL_BUILDER_PUSH_BACK( SelectCustomerByTenderNo )

						ACCESS_METHOD( accessor::customerlist::lit::SAVE_CUSTOMER_LIST );
						SQL_BUILDER_CREATE   ( DeleteCustomerList )
						SQL_BUILDER_PUSH_BACK( DeleteCustomerList )
						SQL_BUILDER_CREATE	 ( UpdateCustomerList )
						SQL_BUILDER_PUSH_BACK( UpdateCustomerList )
						SQL_BUILDER_CREATE	 ( InsertCustomerList )
						SQL_BUILDER_PUSH_BACK( InsertCustomerList )

                END_ACCESSOR_DEFINITION

                //////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectCustomerByPattern )k
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectCustomerByPattern )

                bool SelectCustomerByPattern::isExecutable() const
                {
                    METHODNAME_DEF( SelectCustomerByPattern, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = false;

					if( arePropertiesSet( properties::CUSTOMER_NO.toPropertyString() ) || 
						arePropertiesSet( properties::TENDER_NO.toPropertyString() ) )
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
							"tc.tenderno AS "			+ properties::TENDER_NO.getName()			+ ", "
							"tc.branchno AS "			+ properties::BRANCH_NO.getName()			+ ", "
							"tc.customerno AS "			+ properties::CUSTOMER_NO.getName()			+ ", "
							"k.nameapo AS "				+ properties::PHARMACY_NAME.getName()		+ " "
						"FROM tendercustomer AS tc "
						"INNER JOIN kunde as k on k.idfnr = tc.customerno "
						"WHERE 1=1 " );

                    basar::VarString variableSQL;

					if( arePropertiesSetAndOneMustBeInState(properties::TENDER_NO.toPropertyString(), basar::SS_CLEAN) )
                    {
                        variableSQL.append( " AND tc.tenderno = "  + properties::TENDER_NO.toSQLReplacementString() );
                    }

					if( arePropertiesSetAndOneMustBeInState( properties::BRANCH_NO.toPropertyString(), basar::SS_CLEAN ) )
					{
						variableSQL.append(" AND tc.branchno = " + properties::BRANCH_NO.toSQLReplacementString() );
					}

                    if( arePropertiesSetAndOneMustBeInState(properties::CUSTOMER_NO.toPropertyString(), basar::SS_CLEAN) )
                    {
                        variableSQL.append( " AND tc.customerno = "  + properties::CUSTOMER_NO.toSQLReplacementString() );
                    }

					basar::VarString organizeResult(" ORDER BY tc.customerno ASC");

                    resolve( constSQL + variableSQL + organizeResult );
                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

                //////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectCustomerByTenderNo )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectCustomerByTenderNo )

                bool SelectCustomerByTenderNo::isExecutable() const
                {
                    METHODNAME_DEF( SelectCustomerByTenderNo, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

					bool executable = false;

					if( arePropertiesSet( properties::TENDER_NO.toPropertyString() ) )
					{
						executable = true;
					}
				    
                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectCustomerByTenderNo::buildSQLString()
                {
					resolve(
						"SELECT DISTINCT "
							"tc.tenderno AS "												+ properties::TENDER_NO.getName()		+ ", "
							"tc.branchno AS "												+ properties::BRANCH_NO.getName()		+ ", "
							"tc.customerno AS "												+ properties::CUSTOMER_NO.getName()		+ ", "
							"k.nameapo AS "													+ properties::PHARMACY_NAME.getName()	+ " "
 						"FROM tendercustomer AS tc "
						"INNER JOIN kunde as k on k.idfnr = tc.customerno "
						"WHERE tenderno = "													+ properties::TENDER_NO.toSQLReplacementString() + " " 
						//"AND tc.branchno = k.filialnr"
						);

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }


				//////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( DeleteCustomerList )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( DeleteCustomerList )

				using namespace properties;

                ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( DeleteCustomerList,  
				TENDER_NO.toPropertyString() +
				BRANCH_NO.toPropertyString() +
				CUSTOMER_NO.toPropertyString(),

				TENDER_NO.toPropertyString() +
				BRANCH_NO.toPropertyString() +
				CUSTOMER_NO.toPropertyString(),

				basar::SS_DELETE )

                void DeleteCustomerList::buildSQLString()
                {
					resolve(
						"DELETE "
						"FROM TENDERCUSTOMER "
						"WHERE tenderno = "		+ properties::TENDER_NO.toSQLReplacementString() +
						" AND customerno = "	+ properties::CUSTOMER_NO.toSQLReplacementString() +
						" AND branchno = "		+ properties::BRANCH_NO.toSQLReplacementString()	
						);

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }


				//////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( UpdateCustomerList )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( UpdateCustomerList )

                ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( UpdateCustomerList,  
				TENDER_NO.toPropertyString() +
				BRANCH_NO.toPropertyString() +
				CUSTOMER_NO.toPropertyString(),

				TENDER_NO.toPropertyString() +
				BRANCH_NO.toPropertyString() +
				CUSTOMER_NO.toPropertyString(),

				basar::SS_UPDATE )

                void UpdateCustomerList::buildSQLString()
                {
					resolve(
						"UPDATE TENDERCUSTOMER "
						"SET branchno = "			+ properties::BRANCH_NO.toSQLReplacementString() +
						" WHERE tenderno = "		+ properties::TENDER_NO.toSQLReplacementString() +
						" AND customerno = "		+ properties::CUSTOMER_NO.toSQLReplacementString() 
						);

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }


				//////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( InsertCustomerList )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( InsertCustomerList )

                ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( InsertCustomerList,  
				TENDER_NO.toPropertyString() +
				BRANCH_NO.toPropertyString() +
				CUSTOMER_NO.toPropertyString(),

				TENDER_NO.toPropertyString() +
				BRANCH_NO.toPropertyString() +
				CUSTOMER_NO.toPropertyString(),

				basar::SS_INSERT )

                void InsertCustomerList::buildSQLString()
                {
					resolve(
						"INSERT INTO TENDERCUSTOMER VALUES "
							"( "
							+ properties::TENDER_NO.toSQLReplacementString()	+ ", "
							+ properties::BRANCH_NO.toSQLReplacementString()	+ ", "
							+ properties::CUSTOMER_NO.toSQLReplacementString()	+
							" )"			
						);

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

            } // end namespace customerlist
        } // end namespace accessor
    }   // end namespace infrastructure
}   // end namespace libtender
