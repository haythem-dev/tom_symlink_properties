#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERHEADERACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERHEADERACC_H

//
//
//using basar::db::aspect::SQLStringBuilder;

namespace libtender
{
namespace infrastructure
{
namespace accessor
{
namespace tender
{
BEGIN_QUERY_BUILDER_DECLARATION( SelectTenderHeaderByKey )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertTenderHeader )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( UpdateTenderHeader )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( DeleteTenderHeader )
END_BUILDER_DECLARATION

} // end namespace tender
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERHEADERACC_H

