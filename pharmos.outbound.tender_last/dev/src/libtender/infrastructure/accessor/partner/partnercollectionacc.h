#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PARTNERCOLLECTIONACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PARTNERCOLLECTIONACC_H

//-------------------------------------------------------------------------------------------------//
// sqlstring builder declaration section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace partner
            {
                BEGIN_QUERY_BUILDER_DECLARATION( SelectPartnerByPattern )
                END_BUILDER_DECLARATION

				BEGIN_QUERY_BUILDER_DECLARATION( SelectPartnerByPartnerNo )
                END_BUILDER_DECLARATION
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PARTNERCOLLECTIONACC_H
