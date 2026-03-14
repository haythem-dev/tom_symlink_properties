#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONPTR_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace orderProposal
{
    class IOrderProposalCollection;
	typedef boost::shared_ptr< IOrderProposalCollection > IOrderProposalCollectionPtr;

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONPTR_H
