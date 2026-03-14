#ifndef GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_ORDERPROPOSALACC_H
#define GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_ORDERPROPOSALACC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasardbaspect_macros.h>
#include <libbasardbaspect_sqlstringbuilder.h>

//-------------------------------------------------------------------------------------------------//
// sqlstring builder declaration section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace infrastructure
{
namespace accessor
{
namespace orderProposal
{
    BEGIN_QUERY_BUILDER_DECLARATION( SelectOrderProposalByKey )
    END_BUILDER_DECLARATION

	BEGIN_WRITE_BUILDER_DECLARATION( DeleteOrderProposal )
    END_BUILDER_DECLARATION

	BEGIN_WRITE_BUILDER_DECLARATION( InsertOrderProposal )
    END_BUILDER_DECLARATION

	BEGIN_WRITE_BUILDER_DECLARATION( UpdateOrderProposal )
    END_BUILDER_DECLARATION

} // end namespace orderProposal
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_ORDERPROPOSALACC_H
