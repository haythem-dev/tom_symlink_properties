#ifndef GUARD_TRANSFER_ORDER_HEAD_INPREPARATION_ACC_H
#define GUARD_TRANSFER_ORDER_HEAD_INPREPARATION_ACC_H

namespace libabbauw
{
namespace acc_transfer_order_in_preparation
{
using basar::db::aspect::SQLStringBuilder;

BEGIN_QUERY_BUILDER_DECLARATION( SelectCurrentTransferOrderHeadInPreparation )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertTransferOrderHeadInPreparation )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( UpdateTransferOrderHeadInPreparation )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( DeleteTransferOrderHeadInPreparation )
END_BUILDER_DECLARATION

} // end namespace acc_transfer_order_in_preparation
} // end namespace libabbauw

#endif // GUARD_TRANSFER_ORDER_HEAD_INPREPARATION_ACC_H
