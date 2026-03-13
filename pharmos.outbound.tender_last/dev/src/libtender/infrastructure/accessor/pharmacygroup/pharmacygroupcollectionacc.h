#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYGROUPCOLLECTIONACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYGROUPCOLLECTIONACC_H

//-------------------------------------------------------------------------------------------------//
// sqlstring builder declaration section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
namespace infrastructure
{
namespace accessor
{
namespace pharmacygroup
{
    BEGIN_QUERY_BUILDER_DECLARATION( SelectPharmacyGroupByPattern )
    END_BUILDER_DECLARATION
    
    BEGIN_QUERY_BUILDER_DECLARATION( SelectPharmacyGroupCustomerAssignmentByPattern )
    END_BUILDER_DECLARATION

    BEGIN_QUERY_BUILDER_DECLARATION( SelectPharmacyGroupByGroupID )
    END_BUILDER_DECLARATION

    BEGIN_QUERY_BUILDER_DECLARATION( SelectPharmacyGroupByPseudoCustomerNo )
    END_BUILDER_DECLARATION

} // end namespace article
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender

#endif // end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYGROUPCOLLECTIONACC_H
