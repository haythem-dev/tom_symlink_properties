#ifndef GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_PURCHASEBATCHORDERPOSACC_H
#define GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_PURCHASEBATCHORDERPOSACC_H 

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
namespace purchaseBatchOrder
{
    BEGIN_QUERY_BUILDER_DECLARATION( SelectPurchaseBatchOrderPosByKey )
    END_BUILDER_DECLARATION

    BEGIN_WRITE_BUILDER_DECLARATION( UpdatePurchaseBatchOrderPos )
    END_BUILDER_DECLARATION

    BEGIN_WRITE_BUILDER_DECLARATION( DeletePurchaseBatchOrderPos )
    END_BUILDER_DECLARATION

} // end namespace purchaseBatchOrder
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_PURCHASEBATCHORDERPOSACC_H
