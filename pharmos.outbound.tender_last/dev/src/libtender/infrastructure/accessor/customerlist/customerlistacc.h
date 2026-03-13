#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERLISTACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERLISTACC_H

//-------------------------------------------------------------------------------------------------//
// sqlstring builder declaration section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace customerlist
            {
                BEGIN_QUERY_BUILDER_DECLARATION( SelectCustomerByPattern )
                END_BUILDER_DECLARATION

                BEGIN_QUERY_BUILDER_DECLARATION( SelectCustomerByTenderNo )
                END_BUILDER_DECLARATION

				BEGIN_WRITE_BUILDER_DECLARATION( InsertCustomerList )
				END_BUILDER_DECLARATION
				
				BEGIN_WRITE_BUILDER_DECLARATION( UpdateCustomerList )
				END_BUILDER_DECLARATION
				
				BEGIN_WRITE_BUILDER_DECLARATION( DeleteCustomerList )
				END_BUILDER_DECLARATION
			}
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERLISTACC_H
