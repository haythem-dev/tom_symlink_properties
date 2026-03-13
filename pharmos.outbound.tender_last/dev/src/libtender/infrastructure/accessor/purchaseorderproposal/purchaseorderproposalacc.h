#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PURCHASEORDERPROPOSALACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PURCHASEORDERPROPOSALACC_H

//-------------------------------------------------------------------------------------------------//
// sqlstring builder declaration section
//-------------------------------------------------------------------------------------------------//
namespace libtender {
namespace infrastructure {
namespace accessor {
namespace purchaseOrderProposal  {

//-------------------------------------------------------------------------------------------------//

BEGIN_QUERY_BUILDER_DECLARATION( SelectPurchaseOrderProposalByKey )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertPurchaseOrderProposal )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( UpdatePurchaseOrderProposal )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( DeletePurchaseOrderProposal )
END_BUILDER_DECLARATION

//-------------------------------------------------------------------------------------------------//

} // end namespace purchaseOrderProposal
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender

//-------------------------------------------------------------------------------------------------//

#endif // end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PURCHASEORDERPROPOSALACC_H

//-------------------------------------------------------------------------------------------------//
