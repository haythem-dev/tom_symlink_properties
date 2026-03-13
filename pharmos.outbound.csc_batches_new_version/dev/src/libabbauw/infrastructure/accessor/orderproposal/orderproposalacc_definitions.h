#ifndef GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_ORDERPROPOSALACC_DEFINITIONS_H
#define GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_ORDERPROPOSALACC_DEFINITIONS_H

//----------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------//
#include <libbasarcmnutil_parameterlist.h>
#include <libbasarproperty_propertydescription.h>

//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace infrastructure
{
namespace accessor
{
namespace orderProposal
{
    namespace lit
    {
        const basar::VarString ACC_ORDERPROPOSAL				= "AccOrderProposal";
        const basar::VarString SELECT_ORDER_PROPOSAL_BY_KEY     = "SelectOrderProposalByKey";
	    const basar::VarString DELETE_ORDER_PROPOSAL			= "DeleteOrderProposal";
	    const basar::VarString SAVE_ORDER_PROPOSAL              = "SaveOrderProposal";
    }

} // end namespace orderProposal
} // end namespace domMod
} // end namespace infrastructure
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_INFRASTRUCTURE_ACCESSOR_ORDERPROPOSALACC_DEFINITIONS_H
