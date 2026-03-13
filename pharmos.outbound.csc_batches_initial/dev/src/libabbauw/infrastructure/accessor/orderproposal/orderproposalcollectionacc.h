#ifndef GUARD_LIBABBAUW_ACC_ORDERPROPOSALCOLLECTION_ORDERPROPOSALCOLLECTIONACC_H
#define GUARD_LIBABBAUW_ACC_ORDERPROPOSALCOLLECTION_ORDERPROPOSALCOLLECTIONACC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasardbaspect_macros.h>
#include <libbasardbaspect_definitions.h>
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
    BEGIN_QUERY_BUILDER_DECLARATION( SelectOrderProposalsByKey )
    END_BUILDER_DECLARATION

} // end namespace orderProposal
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_ACC_ORDERPROPOSALCOLLECTION_ORDERPROPOSALCOLLECTIONACC_H
