// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/supplier/suppliercollectionacc.h>
#include <infrastructure/accessor/supplier/suppliercollectionacc_definitions.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace supplier
            {
                //---------------------------------------------------------------------------------------------//
                // accessor definition
                //---------------------------------------------------------------------------------------------//
                BEGIN_ACCESSOR_DEFINITION( accessor::supplier::lit::SUPPLIER_COLLECTION_ACC )

                    BEGIN_PROPERTY_DESCRIPTION_LIST
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::SUPPLIER_NO   )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::SUPPLIER_NAME )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::SUPPLIER_STREET )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::SUPPLIER_LOCATION )
                    END_PROPERTY_DESCRIPTION_LIST

                        ACCESS_METHOD( accessor::supplier::lit::SELECT_SUPPLIER_BY_PATTERN );
                        SQL_BUILDER_CREATE   ( SelectSupplierByPattern )
                        SQL_BUILDER_PUSH_BACK( SelectSupplierByPattern )

                        ACCESS_METHOD( accessor::supplier::lit::SELECT_SUPPLIER_BY_SUPPLIER_NO );
                        SQL_BUILDER_CREATE   ( SelectSupplierBySupplierNo )
                        SQL_BUILDER_PUSH_BACK( SelectSupplierBySupplierNo )

                END_ACCESSOR_DEFINITION

                //////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectSupplierByPattern )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectSupplierByPattern )

                bool SelectSupplierByPattern::isExecutable() const
                {
                    METHODNAME_DEF( SelectSupplierByPattern, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

                    bool executable = arePropertiesSet( properties::SUPPLIER_NO.toPropertyString()   ) ||
                                      arePropertiesSet( properties::SUPPLIER_NAME.toPropertyString() );

                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectSupplierByPattern::buildSQLString()
                {
                    resolve(
                            "SELECT lant_nr AS "           + properties::SUPPLIER_NO.getName() +       ", "
                            "lant_name AS "                + properties::SUPPLIER_NAME.getName() +     ", "
                            "strasse AS "                  + properties::SUPPLIER_STREET.getName() +   ", "
                            "ort AS "                      + properties::SUPPLIER_LOCATION.getName() + "  "
                            "FROM supplier "
                            "WHERE 1=1"
                            );

                    if( arePropertiesSetAndOneMustBeInState( properties::SUPPLIER_NO.toPropertyString(), basar::SS_CLEAN) )
                    {
                        resolve( " AND lant_nr = "  + properties::SUPPLIER_NO.toSQLReplacementString() );
                    }

                    if( arePropertiesSetAndOneMustBeInState( properties::SUPPLIER_NAME.toPropertyString(), basar::SS_CLEAN) )
                    {
                        resolve( " AND UPPER(lant_name) LIKE UPPER('"  + properties::SUPPLIER_NAME.toSQLReplacementString() + "%') " );
                    }

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

                //////////////////////////////////////////////////////////////////////////////////////////
                // BUILDER_DEFINITION( SelectSupplierBySupplierNo )
                //////////////////////////////////////////////////////////////////////////////////////////
                BUILDER_DEFINITION( SelectSupplierBySupplierNo )

                bool SelectSupplierBySupplierNo::isExecutable() const
                {
                    METHODNAME_DEF( SelectSupplierBySupplierNo, isExecutable );
                    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
                    BLOG_TRACE_METHOD( logger, fun );

                    bool executable = false;

                    if( arePropertiesSet( properties::SUPPLIER_NO.toPropertyString() ) )
                    {
                        executable = true;
                    }

                    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

                    return executable;
                }

                void SelectSupplierBySupplierNo::buildSQLString()
                {
                    resolve(
                        "SELECT DISTINCT "
                            "lf.lant_nr AS "             + properties::SUPPLIER_NO.getName()         + ", "
                            "lf.lant_name AS "           + properties::SUPPLIER_NAME.getName()       + ", "
                            "lf.strasse AS "             + properties::SUPPLIER_STREET.getName()     + ", "
                            "lf.ort AS "                 + properties::SUPPLIER_LOCATION.getName()   + " "
                        "FROM supplier AS lf "
                        "WHERE lf.lant_nr="              + properties::SUPPLIER_NO.toSQLReplacementString()	+ " "
                        );

                    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
                }

            } // end namespace customer
        } // end namespace accessor
    }   // end namespace infrastructure
}   // end namespace libtender
