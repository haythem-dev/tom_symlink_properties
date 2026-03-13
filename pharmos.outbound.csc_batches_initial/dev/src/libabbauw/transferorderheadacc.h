#ifndef GUARD_LIBABBAUW_TRANSFERORDERHEADACC_H
#define GUARD_LIBABBAUW_TRANSFERORDERHEADACC_H

namespace libabbauw
{
namespace acc_transfer_order_head
{
using basar::db::aspect::SQLStringBuilder;

BEGIN_QUERY_BUILDER_DECLARATION( SelectTransferOrderHead )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( UpdateTransferOrderHead )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertTransferOrderHead )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( DeleteOrderHead )
END_BUILDER_DECLARATION


} // end namespace acc_transfer_order_head
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_TRANSFERORDERHEADACC_H
