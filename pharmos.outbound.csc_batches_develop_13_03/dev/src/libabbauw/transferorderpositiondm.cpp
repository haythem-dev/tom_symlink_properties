//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "transferorderpositiondm.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"
#include "trafficlightcolorsenum.h"

#include <libutil/domainmodule.h>
#include <libutil/util.h>

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{
    TransferOrderPositionDM::TransferOrderPositionDM()
        : m_Logger( LoggerPool::loggerTransferOrder )
    {
    }

    TransferOrderPositionDM::~TransferOrderPositionDM()
    {
    }

    /*
    void TransferOrderPositionDM::injectTransferOrderPositionAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
    {
        m_Accessor = accessor;
    }

    libutil::infrastructure::accessor::IAccessorPtr TransferOrderPositionDM::getAccessor() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
        return m_Accessor;
    }
    */

    void TransferOrderPositionDM::set( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        m_YitPos = yit;
    }

    const log4cplus::Logger& TransferOrderPositionDM::getLogger() const
    {
        METHODNAME_DEF( TransferOrderPositionDM, getLogger )
        BLOG_TRACE_METHOD( m_Logger, fun );

        return m_Logger;
    }
    
    basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderPositionDM::get() const
    {
        return m_YitPos;
    }

    bool TransferOrderPositionDM::isEmpty() const
    {
        return m_YitPos.isEnd();
    }

    bool TransferOrderPositionDM::hasOrderProposalSet() const
    {
        // check if there's an orderproposal time set - which means that there's an orderproposal already been created for this item
        return get().isContainedAndSet( properties::ORDERPROPOSALTIME ) && get().getDateTime( properties::ORDERPROPOSALTIME ).isValid();
    }

    bool TransferOrderPositionDM::isOrderProposalAllowed() const
    {
        const bool orderProposalUnset  = ! hasOrderProposalSet();
        const bool isShortfallPosition = get().isContainedAndSet( properties::COLOR ) && 
                                         (get().getInt16( properties::COLOR ) & libabbauw::domMod::transferOrder::RED) == libabbauw::domMod::transferOrder::RED;
        const bool orderedQtySet       = get().isContainedAndSet( properties::ORDEREDQTY ) &&
                                         get().getInt32( properties::ORDEREDQTY ) > 0;
		const bool nonChargedQtySet =	 get().isContainedAndSet( properties::NONCHARGEDQTY ) &&
                                         get().getInt32( properties::NONCHARGEDQTY ) > 0;
        const bool knownArticle        = get().getInt32( properties::ARTICLENO )  > 0;

        return orderProposalUnset && isShortfallPosition && (orderedQtySet || nonChargedQtySet) && knownArticle;
    }

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw
