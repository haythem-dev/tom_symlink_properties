#ifndef GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_ORDERPOSPROTOCOLACC_H
#define GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_ORDERPOSPROTOCOLACC_H 

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
namespace orderprotocol
{
    BEGIN_QUERY_BUILDER_DECLARATION( SelectOrderPosProtocolByKey )
    END_BUILDER_DECLARATION

	BEGIN_WRITE_BUILDER_DECLARATION( InsertOrderPosProtocol )
	END_BUILDER_DECLARATION

} // end namespace transferOrder
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_ORDERPOSPROTOCOLACC_H
