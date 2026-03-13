#include "orderproposalfactory.h"

#include "domainmodule/orderproposal/orderproposaldm.h"

#include "infrastructure/accessor/orderproposal/orderproposalacc.h"
#include "infrastructure/accessor/orderproposal/orderproposalacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include <libbasarcmnutil_logging.h>
#include <libutil/util.h>

#include <boost/make_shared.hpp>

namespace libabbauw
{
namespace componentManager
{
    OrderProposalFactory::OrderProposalFactory()
    {
    }

    OrderProposalFactory::~OrderProposalFactory()
    {
    }

    void OrderProposalFactory::injectDBConnectionDataGetter( libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr dbConnDataGetter )
    {
        m_DBConnectionDataGetter = dbConnDataGetter;
    }

    libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr OrderProposalFactory::getDBConnectionDataGetter()
    {
        CHECK_INSTANCE_EXCEPTION( m_DBConnectionDataGetter.get() );
        return m_DBConnectionDataGetter;
    }

    libutil::infrastructure::accessor::IAccessorPtr OrderProposalFactory::getOrderProposalAccessor()
    {
        infrastructure::accessor::orderProposal::SelectOrderProposalByKey();

        using libutil::infrastructure::accessor::AccessorProxy;
        return boost::make_shared< AccessorProxy >( infrastructure::accessor::orderProposal::lit::ACC_ORDERPROPOSAL, 
                                                    getDBConnectionDataGetter()->getCurrentConnection(), basar::db::aspect::FULL_CACHING );
    }

    domMod::orderProposal::OrderProposalDMPtr OrderProposalFactory::getOrderProposalDM()
    {
        METHODNAME_DEF( DMComponentManager, getOrderProposalDM )
	    BLOG_TRACE_METHOD( LoggerPool::loggerLibAbbaUW, fun );

        domMod::orderProposal::OrderProposalDMPtr dm = boost::make_shared< domMod::orderProposal::OrderProposalDM >();
        dm->injectOrderProposalAccessor( getOrderProposalAccessor() );

	    return dm;
    }

    domMod::orderProposal::IOrderProposalPtr OrderProposalFactory::create()
    {
        METHODNAME_DEF( DMComponentManager, getOrderProposal )
	    BLOG_TRACE_METHOD( LoggerPool::loggerLibAbbaUW, fun );

        return getOrderProposalDM();
    }

    domMod::orderProposal::IOrderProposalPtr OrderProposalFactory::create( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        domMod::orderProposal::IOrderProposalPtr orderProposal = create();
        orderProposal->get().match( yit );

        return orderProposal;
    }

} // end namespace componentManager
} // end namespace libabbauw
