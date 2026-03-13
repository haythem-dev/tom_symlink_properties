#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALFACTORY_H

#include "iorderproposalfactory.h"

#include "domainmodule/orderproposal/iorderproposalptr.h"
#include "domainmodule/orderproposal/orderproposaldmptr.h"

#include <libutil/accessor.h>
#include <libutil/dbconnection.h>

namespace libabbauw
{
namespace componentManager
{
    class OrderProposalFactory : public IOrderProposalFactory
    {
    public:
        OrderProposalFactory();
        ~OrderProposalFactory();

        void                                                                  injectDBConnectionDataGetter( libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr );

        // from IOrderProposalFactory interface
        domMod::orderProposal::IOrderProposalPtr                              create();
        domMod::orderProposal::IOrderProposalPtr                              create( basar::db::aspect::AccessorPropertyTable_YIterator );

    private:
	    // don't allow assignment and copying of this class
	    OrderProposalFactory( const OrderProposalFactory& );
        OrderProposalFactory& operator = ( const OrderProposalFactory& );

        libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr getDBConnectionDataGetter();
        libutil::infrastructure::accessor::IAccessorPtr                       getOrderProposalAccessor();
        domMod::orderProposal::OrderProposalDMPtr                             getOrderProposalDM();

    private:
        libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr m_DBConnectionDataGetter;
    };

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALFACTORY_H
