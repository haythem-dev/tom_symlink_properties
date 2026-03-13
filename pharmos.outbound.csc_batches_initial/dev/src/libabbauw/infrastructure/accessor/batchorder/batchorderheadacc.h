#ifndef GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_BATCHORDERHEADACC_H
#define GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_BATCHORDERHEADACC_H

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
namespace batchOrder
{
    BEGIN_QUERY_BUILDER_DECLARATION( SelectBatchOrderHeadByKey )
    END_BUILDER_DECLARATION

} // end namespace batchOrder
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_BATCHORDERHEADACC_H
