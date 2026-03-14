#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCREATOR_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCREATOR_H

#include "domainmodule/orderproposal/iorderproposalcreator.h"
#include "icustomerfinderptr.h"

#include "componentmanager/iorderproposalfactoryptr.h"

#include <libutil/classinfo.h>
#include <log4cplus/logger.h>

namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    class OrderProposalCreatorDM : public IOrderProposalCreator
    {
    public:
        static const libutil::misc::ClassInfo& getClassInfo();

    public:
        OrderProposalCreatorDM();
        ~OrderProposalCreatorDM();

        void                                                injectOrderProposalFactory( componentManager::IOrderProposalFactoryPtr );
        void                                                injectCustomerFinder( customer::ICustomerFinderPtr );

        // from IOrderProposalCreator interface
        void                                                createOrderProposal( transferOrder::ITransferOrderGetterPtr, const basar::Int32 posNo ) const;
        void                                                createOrderProposals( transferOrder::ITransferOrderGetterPtr ) const;

    private:
        // forbidden
        OrderProposalCreatorDM( const OrderProposalCreatorDM& );
		OrderProposalCreatorDM& operator = ( const OrderProposalCreatorDM& );

        const log4cplus::Logger&                            getLogger() const;
        componentManager::IOrderProposalFactoryPtr          getOrderProposalFactory() const;
        customer::ICustomerFinderPtr                        getCustomerFinder() const;

        void                                                checkCustomerLocked( basar::Int32 pharmacyNo ) const;

    private:
        const log4cplus::Logger&                            m_Logger;
        componentManager::IOrderProposalFactoryPtr          m_OrderProposalFactory;
        customer::ICustomerFinderPtr                        m_CustomerFinder;
    };

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_ORDERPROPOSALCREATOR_H
