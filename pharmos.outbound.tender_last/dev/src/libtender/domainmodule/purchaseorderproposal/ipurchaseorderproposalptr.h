#ifndef GUARD_LIBTENDER_DOMMOD_PURCHASEORDERPROPOSAL_IPURCHASEORDERPROPOSALPTR_H
#define GUARD_LIBTENDER_DOMMOD_PURCHASEORDERPROPOSAL_IPURCHASEORDERPROPOSALPTR_H

//-------------------------------------------------------------------------------------------------//

#include <boost/shared_ptr.hpp>

//-------------------------------------------------------------------------------------------------//

namespace libtender {
namespace domMod {
namespace purchaseOrderProposal {

//-------------------------------------------------------------------------------------------------//

	class IPurchaseOrderProposal;
	typedef boost::shared_ptr< IPurchaseOrderProposal > IPurchaseOrderProposalPtr;

//-------------------------------------------------------------------------------------------------//

} // namespace purchaseOrderProposal
} // namespace domMod
} // namespace libtender

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_LIBTENDER_DOMMOD_PURCHASEORDERPROPOSAL_IPURCHASEORDERPROPOSALPTR_H

//-------------------------------------------------------------------------------------------------//
