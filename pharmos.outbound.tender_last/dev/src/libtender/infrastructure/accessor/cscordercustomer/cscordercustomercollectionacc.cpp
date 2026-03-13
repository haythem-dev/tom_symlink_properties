// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/cscordercustomer/cscordercustomercollectionacc.h>
#include <infrastructure/accessor/cscordercustomer/cscordercustomercollectionacc_definitions.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace cscordercustomer
            {
                //---------------------------------------------------------------------------------------------//
                // accessor definition
                //---------------------------------------------------------------------------------------------//
                BEGIN_ACCESSOR_DEFINITION( accessor::cscordercustomer::lit::CSCORDER_CUSTOMERCOLLECTION_ACC )

                    BEGIN_PROPERTY_DESCRIPTION_LIST
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_CODE )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_ID )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::BRANCH_NO )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_NAME )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_LOCATION )
                        PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY )
                    END_PROPERTY_DESCRIPTION_LIST
                    ACCESS_METHOD( accessor::cscordercustomer::lit::SELECT_CSCORDER_CUSTCOLL_BY_PATTERN );
                        SQL_BUILDER_CREATE   ( SelectCscOrderCustCollByPattern )
                        SQL_BUILDER_PUSH_BACK( SelectCscOrderCustCollByPattern )
                END_ACCESSOR_DEFINITION

                 BUILDER_DEFINITION( SelectCscOrderCustCollByPattern )

                bool SelectCscOrderCustCollByPattern::isExecutable() const
                {
                   return false;
                }

                void SelectCscOrderCustCollByPattern::buildSQLString()
                {
                    
                }



            } // end namespace cscordercustomer
        } // end namespace accessor
    }   // end namespace infrastructure
}   // end namespace libtender
