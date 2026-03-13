#include "orderproposalcollectionfactory.h"

#include "iorderproposalcollectionprotectionproxygetter.h"
#include "itransferorderfactory.h"

#include "domainmodule/orderproposal/iorderproposalcollectionprotectionproxy.h"
#include "icustomerfinder.h"

#include "itransferorder.h"
#include "itransferorderhead.h"
#include "itransferorderposcollection.h"
#include "itransferorderposition.h"

#include "libabbauw_properties_definitions.h"

#include "baseexception.h"
#include <ilockguard.h>
#include "iorderlockfactory.h"
#include "exceptions/orderlockedexception.h"

#include "loggerpool/libabbauw_loggerpool.h"

#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libutil/util.h>

namespace libabbauw
{
	namespace componentManager
	{
		OrderProposalCollectionFactory::OrderProposalCollectionFactory()
		{
		}

		OrderProposalCollectionFactory::~OrderProposalCollectionFactory()
		{
		}

		void OrderProposalCollectionFactory::injectOrderProposalCollectionProtectionProxyGetter(IOrderProposalCollectionProtectionProxyGetterPtr ordPropCollGetter)
		{
			m_OrderProposalCollectionProtectionProxyGetter = ordPropCollGetter;
		}

		void OrderProposalCollectionFactory::injectTransferOrderFactory(ITransferOrderFactoryPtr transferOrderFactory)
		{
			m_TransferOrderFactory = transferOrderFactory;
		}

		void OrderProposalCollectionFactory::injectTransferOrderLockFactory(lock::IOrderLockFactoryPtr lockFactory)
		{
			m_TransferOrderLockFactory = lockFactory;
		}

		void OrderProposalCollectionFactory::injectCustomerFinder(domMod::customer::ICustomerFinderPtr customerFinder)
		{
			m_CustomerFinder = customerFinder;
		}

		bool OrderProposalCollectionFactory::isCustomerLocked(basar::Int32 pharmacyNo) const
		{
			m_CustomerFinder->findByKey(pharmacyNo, true);
			return m_CustomerFinder->isLocked();
		}

		libLockManager::ILockGuardPtr OrderProposalCollectionFactory::createLockGuard(basar::Int32 orderNo)
		{
			return m_TransferOrderLockFactory->createLockGuard(orderNo);
		}

		// from ITransferOrderHeadCollectionFactory interface
		domMod::orderProposal::IOrderProposalCollectionPtr OrderProposalCollectionFactory::create(const AccessorPropertyTableYIteratorCollection& orderYits)
		{
			METHODNAME_DEF(OrderProposalCollectionFactory, OrderProposalCollectionFactory);

			domMod::orderProposal::IOrderProposalCollectionProtectionProxyPtr orderProposalColl = m_OrderProposalCollectionProtectionProxyGetter->getOrderProposalCollectionProtectionProxy();
			for (AccessorPropertyTableYIteratorCollection::const_iterator it = orderYits.begin(); it != orderYits.end(); ++it)
			{
				if ((it->getInt16(properties::COLOR) & domMod::transferOrder::RED) == domMod::transferOrder::RED &&
					!isCustomerLocked(it->getInt32(properties::PHARMACYNO)))
				{
					bool positionWasAdded = false;
					const basar::Int32 orderNo = it->getInt32(properties::ORDERNO);
					libLockManager::ILockGuardPtr lockGuard = createLockGuard(orderNo);

					try
					{
						lockGuard->lock();

						domMod::transferOrder::ITransferOrderPtr transferOrder = m_TransferOrderFactory->create(*it);
						domMod::transferOrder::ITransferOrderPosCollectionPtr positions = transferOrder->getPosCollection();
						for (basar::db::aspect::AccessorPropertyTable_YIterator yitPos = positions->get().begin(); !yitPos.isEnd(); ++yitPos)
						{
							domMod::transferOrder::ITransferOrderPositionPtr position = positions->getPosition(yitPos);
							if (position->isOrderProposalAllowed())
							{
								basar::db::aspect::AccessorPropertyTable_YIterator orderPropYit = orderProposalColl->addNewItem();
								orderPropYit.match(yitPos);
								orderPropYit.setInt32(properties::MANUFACTURERNO, transferOrder->getHead()->get().getInt32(properties::MANUFACTURERNO));
								orderPropYit.setInt32(properties::ORDERNO, transferOrder->getOrderNo());
								orderPropYit.setInt32(properties::ORDERPROPOSALQTY, yitPos.getInt32(properties::SHORTFALLQTY));
								positionWasAdded = true;
							}
						}

						if (positionWasAdded) // hold lock if positions of this order are still in use
						{
							orderProposalColl->passLockGuard(it->getInt32(properties::ORDERNO), lockGuard);
						}
					}
					catch (libLockManager::exceptions::BaseException&)
					{
						basar::VarString orderno; orderno.itos(orderNo);
						basar::VarString reason = "Could not lock order '" + orderno + "'!";

						BLOG_WARN(LoggerPool::loggerLibAbbaUW, reason);
						throw exceptions::OrderLockedException(basar::ExceptInfo(fun, reason, __FILE__, __LINE__), orderNo);
					}
				}
			}

			return orderProposalColl;
		}
	} // end namespace componentManager
} // end namespace libabbauw
