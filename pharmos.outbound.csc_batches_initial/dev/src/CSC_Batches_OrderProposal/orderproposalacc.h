#ifndef GUARD_ACC_ORDER_PROPOSAL_H
#define GUARD_ACC_ORDER_PROPOSAL_H

#include <libbasardbaspect_macros.h>
#include <libbasardbaspect_definitions.h>
#include <libbasardbaspect_sqlstringbuilder.h>

namespace acc_orderproposal
{
    BEGIN_QUERY_BUILDER_DECLARATION( SelectOrderProposals )
    END_BUILDER_DECLARATION

	BEGIN_WRITE_BUILDER_DECLARATION( UpdateTransferDate )
    END_BUILDER_DECLARATION

	BEGIN_WRITE_BUILDER_DECLARATION( UpdateOrderProposals )
    END_BUILDER_DECLARATION

    BEGIN_WRITE_BUILDER_DECLARATION( UpdateOrderProposalCompletedFlag )
    END_BUILDER_DECLARATION
}

#endif 
