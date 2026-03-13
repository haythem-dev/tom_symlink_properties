#include "domainmodule/orderproposal/orderproposalcreatordm.h"
#include "domainmodule/orderproposal/iorderproposal.h"

#include "componentmanager/iorderproposalfactory.h"

#include "itransferordergetter.h"
#include "itransferorderhead.h"
#include "itransferorderposcollection.h"
#include "itransferorderposcollectionptr.h"
#include "itransferorderposition.h"
#include "itransferorderpositionptr.h"

#include "icustomerfinder.h"

#include "libabbauw_properties_definitions.h"

#include "customerlockedexception.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include <libutil/util.h>

#include <libbasardbaspect.h>

namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    const libutil::misc::ClassInfo & OrderProposalCreatorDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "OrderProposalCreatorDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

    OrderProposalCreatorDM::OrderProposalCreatorDM()
        : m_Logger( LoggerPool::loggerOrderProposal )
    {
        METHODNAME_DEF( OrderProposalCreatorDM, OrderProposalCreatorDM )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    OrderProposalCreatorDM::~OrderProposalCreatorDM()
    {
        METHODNAME_DEF( OrderProposalCreatorDM, ~OrderProposalCreatorDM )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    void OrderProposalCreatorDM::injectOrderProposalFactory( componentManager::IOrderProposalFactoryPtr orderProposalFactory )
    {
        m_OrderProposalFactory = orderProposalFactory;
    }

    componentManager::IOrderProposalFactoryPtr OrderProposalCreatorDM::getOrderProposalFactory() const
    {
        CHECK_INSTANCE_EXCEPTION( m_OrderProposalFactory.get() );
        return m_OrderProposalFactory;
    }

    void OrderProposalCreatorDM::injectCustomerFinder( customer::ICustomerFinderPtr customerFinder )
    {
        m_CustomerFinder = customerFinder;
    }

    customer::ICustomerFinderPtr OrderProposalCreatorDM::getCustomerFinder() const
    {
        CHECK_INSTANCE_EXCEPTION( m_CustomerFinder.get() );
        return m_CustomerFinder;
    }

    const log4cplus::Logger& OrderProposalCreatorDM::getLogger() const
    {
        return m_Logger;
    }

    void OrderProposalCreatorDM::checkCustomerLocked( basar::Int32 pharmacyNo ) const
    {
        METHODNAME_DEF( OrderProposalCreatorDM, checkCustomerLocked );

        getCustomerFinder()->findByKey( pharmacyNo, true );
        if( getCustomerFinder()->isLocked() )
        {
            basar::VarString reason = "Customer is locked!";
            throw exceptions::CustomerLockedException( basar::ExceptInfo( fun, reason, __FILE__, __LINE__  ) );
        }
    }

    void OrderProposalCreatorDM::createOrderProposal( transferOrder::ITransferOrderGetterPtr transferOrder, const basar::Int32 posNo ) const
    {
        METHODNAME_DEF( OrderProposalCreatorDM, createOrderProposal )
        BLOG_TRACE_METHOD( getLogger(), fun );
        checkCustomerLocked( transferOrder->getHead()->get().getInt32( properties::PHARMACYNO ) );

        // find position
        basar::db::aspect::AccessorPropertyTable_YIterator yitPos;
        for( yitPos = transferOrder->getPosCollection()->get().begin(); ! yitPos.isEnd() && yitPos.getInt32( properties::POSNO ) != posNo; ++yitPos );

        // check if order contains additional positions with same article number and create one order proposal containing
        // the whole quantity of all similar articles and an empty order proposal for all other ones
        basar::Int32 shortfall = yitPos.getInt32( properties::SHORTFALLQTY );
        const basar::Int32 articleNo = yitPos.getInt32( properties::ARTICLENO );
        for( basar::db::aspect::AccessorPropertyTable_YIterator yit = transferOrder->getPosCollection()->get().begin(); ! yit.isEnd(); ++yit )
        {
            if( yit.getInt32( properties::ARTICLENO ) == articleNo && yit.getInt32( properties::POSNO ) != posNo )
            {
                shortfall += yit.getInt32( properties::SHORTFALLQTY );

                // create blocking entry with qantity 0
                IOrderProposalPtr orderProposal = getOrderProposalFactory()->create();
                orderProposal->get().match( yit );
                orderProposal->get().setInt32( properties::ORDERPROPOSALQTY, 0 );
                orderProposal->save();
            }
        }

        IOrderProposalPtr orderProposal = getOrderProposalFactory()->create();
        orderProposal->get().match( yitPos );
        orderProposal->get().setInt32( properties::ORDERPROPOSALQTY, shortfall );
        orderProposal->save();

        //needed for protocol!
        yitPos.setInt32( properties::ORDERPROPOSALQTY, shortfall );
    }

    void OrderProposalCreatorDM::createOrderProposals( transferOrder::ITransferOrderGetterPtr transferOrder ) const
    {
        METHODNAME_DEF( OrderProposalCreatorDM, createOrderProposals )
        BLOG_TRACE_METHOD(getLogger(), fun );
        checkCustomerLocked( transferOrder->getHead()->get().getInt32( properties::PHARMACYNO ) );

        // iterate over all positions and sum up the ordered qty for articles with same article number
        typedef std::map< basar::Int32, basar::Int32 > ArticleNoToQtyMap;
        ArticleNoToQtyMap articleNoToQty;

        for( basar::db::aspect::AccessorPropertyTable_YIterator yit = transferOrder->getPosCollection()->get().begin(); ! yit.isEnd(); ++yit )
        {
            transferOrder::ITransferOrderPositionPtr position = transferOrder->getPosCollection()->getPosition( yit );
            if( position->isOrderProposalAllowed() )
            {
                const basar::Int32 articleNo = yit.getInt32( properties::ARTICLENO );
                if( articleNoToQty.find( articleNo ) == articleNoToQty.end() )
                {
                    articleNoToQty[ articleNo ] = yit.getInt32( properties::SHORTFALLQTY );
                }
                else
                {
                    basar::Int32 formerQty = articleNoToQty[ articleNo ];
                    articleNoToQty[ articleNo ] = formerQty + yit.getInt32( properties::SHORTFALLQTY );

                    yit.setInt32( properties::ORDERPROPOSALQTY, 0 ); // important: update position data too
                    yit.setDateTime( properties::ORDERPROPOSALTIME, basar::DateTime::getCurrent() );
                    IOrderProposalPtr orderProposal = getOrderProposalFactory()->create();
                    orderProposal->get().match( yit );
                    orderProposal->save();
                }
            }
        }

        for( basar::db::aspect::AccessorPropertyTable_YIterator yit = transferOrder->getPosCollection()->get().begin(); ! yit.isEnd(); ++yit )
        {
            transferOrder::ITransferOrderPositionPtr position = transferOrder->getPosCollection()->getPosition( yit );
            if( position->isOrderProposalAllowed() ) // duplicates are filtered out in the loop above
            {
                const basar::Int32 articleNo = yit.getInt32( properties::ARTICLENO );
                yit.setInt32( properties::ORDERPROPOSALQTY, articleNoToQty.find( articleNo )->second );
                yit.setDateTime( properties::ORDERPROPOSALTIME, basar::DateTime::getCurrent() );
                IOrderProposalPtr orderProposal = getOrderProposalFactory()->create();
                orderProposal->get().match( yit );
                orderProposal->save();
            }
        }
    }

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw
