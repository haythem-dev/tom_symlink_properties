#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SUPPLIERCOLLECTIONACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SUPPLIERCOLLECTIONACC_H

//-------------------------------------------------------------------------------------------------//
// sqlstring builder declaration section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace supplier
            {
                BEGIN_QUERY_BUILDER_DECLARATION( SelectSupplierByPattern )
                END_BUILDER_DECLARATION

                BEGIN_QUERY_BUILDER_DECLARATION( SelectSupplierBySupplierNo )
                END_BUILDER_DECLARATION
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SUPPLIERCOLLECTIONACC_H
