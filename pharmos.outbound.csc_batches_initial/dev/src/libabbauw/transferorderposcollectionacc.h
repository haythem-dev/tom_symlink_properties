#ifndef GUARD_LIBABBAUW_TRANSFERORDERPOSCOLLECTIONACC_H
#define GUARD_LIBABBAUW_TRANSFERORDERPOSCOLLECTIONACC_H

namespace libabbauw
{
namespace acc_transfer_order_pos
{
using basar::db::aspect::SQLStringBuilder;

BEGIN_QUERY_BUILDER_DECLARATION( SelectTransferOrderPosCollection )    
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( DeleteTransferOrderPos )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertTransferOrderPosCollection )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( UpdateTransferOrderPosCollection )
END_BUILDER_DECLARATION

} // end namespace acc_transfer_order_pos
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_TRANSFERORDERPOSCOLLECTIONACC_H
