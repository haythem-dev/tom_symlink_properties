#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERCOLLECTIONACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERCOLLECTIONACC_H

//-------------------------------------------------------------------------------------------------//
// sqlstring builder declaration section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace customer
            {
                BEGIN_QUERY_BUILDER_DECLARATION( SelectCustomerByPattern )
                END_BUILDER_DECLARATION

                BEGIN_QUERY_BUILDER_DECLARATION( SelectCustomerByPharmacyGroupId )
                END_BUILDER_DECLARATION

                BEGIN_QUERY_BUILDER_DECLARATION( SelectCustomerByCustomerNo )
                END_BUILDER_DECLARATION
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERCOLLECTIONACC_H
