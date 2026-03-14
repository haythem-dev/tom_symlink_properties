#ifndef GUARD_ORDER_PROPOSAL_DM_DEFINITIONS_H
#define GUARD_ORDER_PROPOSAL_DM_DEFINITIONS_H

#include <libbasarcmnutil_parameterlist.h>
#include <libbasarproperty_propertydescription.h>

namespace orderproposaldm
{    
    const basar::Int32 MIN_NUM_CHARS_FOR_ARTICLE_SEARCH = 4;
}

namespace orderproposaldm {    
namespace lit {

	const basar::VarString ACC_ORDERPROPOSAL					( "AccOrderProposalBatch"				);
	const basar::VarString ACC_ORDERPROPOSAL_INST				( "AccOrderProposalInstance"			);
	const basar::VarString SELECT_ORDER_PROPOSALS				( "SelectOrderProposals"				);
	const basar::VarString UPDATE_TRANSFER_DATE					( "UpdateTransferDate"					);
	const basar::VarString UPDATE_TRANSFER_TIME_BI				( "UpdateTransferTimeBI"				);
	const basar::VarString UPDATE_ORDER_PROPOSALS				( "UpdateOrderProposals"				);
	const basar::VarString UPDATE_ORDER_PROPOSAL_COMPLETED_FLAG	( "UpdateOrderProposalCompletedFlag"	);

}
} 

#endif	
