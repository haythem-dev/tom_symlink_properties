#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONFINDERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONFINDERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace orderProposal
{
    class IOrderProposalCollectionFinder;
	typedef boost::shared_ptr< IOrderProposalCollectionFinder > IOrderProposalCollectionFinderPtr;

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONFINDERPTR_H
