#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYCHAINCOLLECTIONACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYCHAINCOLLECTIONACC_H

//-------------------------------------------------------------------------------------------------//
// sqlstring builder declaration section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace pharmacychain
            {
                BEGIN_QUERY_BUILDER_DECLARATION( SelectPharmacyChainByPattern )
                END_BUILDER_DECLARATION

				BEGIN_QUERY_BUILDER_DECLARATION( SelectPharmacyChainByPharmacyChainNo )
                END_BUILDER_DECLARATION

				BEGIN_QUERY_BUILDER_DECLARATION( SelectCustomerGroupIDByPharmacychain )
                END_BUILDER_DECLARATION
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYCHAINCOLLECTIONACC_H
