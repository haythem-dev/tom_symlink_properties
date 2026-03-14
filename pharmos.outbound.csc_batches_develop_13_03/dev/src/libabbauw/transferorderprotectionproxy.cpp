#include "transferorderprotectionproxy.h"

#include "itransferorderhead.h"
#include "itransferorderposcollection.h"
#include "libabbauw_properties_definitions.h"

#include "exceptions/outofsyncexception.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include <log4cplus/logger.h>
#include <libutil/misc/metaclassmacro_definitions.h>

#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

#include "iorderlockfactory.h"
#include <ilockmanager.h>
#include <ilockentry.h>
#include <alreadylockedexception.h>

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning(disable: 4512 )
#endif
#include <QtCore/QCoreApplication>
#ifdef _MSC_VER
#pragma warning (pop)
#endif

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{

			TransferOrderProtectionProxy::TransferOrderProtectionProxy()
				: m_Logger(libabbauw::LoggerPool::loggerTransferOrder)
			{
				METHODNAME_DEF(TransferOrderProtectionProxy, TransferOrderProtectionProxy)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			TransferOrderProtectionProxy::~TransferOrderProtectionProxy()
			{
				METHODNAME_DEF(TransferOrderProtectionProxy, ~TransferOrderProtectionProxy)
					BLOG_TRACE_METHOD(getLogger(), fun);

				unlock();
			}

			void TransferOrderProtectionProxy::injectTransferOrder(ITransferOrderPtr transferOrder)
			{
				m_TransferOrder = transferOrder;
			}

			void TransferOrderProtectionProxy::injectLockManager(libLockManager::ILockManagerPtr lockManager)
			{
				m_LockManager = lockManager;
			}

			void TransferOrderProtectionProxy::injectTransferOrderLockFactory(lock::IOrderLockFactoryPtr transferOrderLockFactory)
			{
				m_TransferOrderLockFactory = transferOrderLockFactory;
			}

			ITransferOrderHeadPtr TransferOrderProtectionProxy::getHead()
			{
				return m_TransferOrder->getHead();
			}

			ITransferOrderHeadPtr TransferOrderProtectionProxy::getHead() const
			{
				return m_TransferOrder->getHead();
			}

			ITransferOrderPosCollectionPtr TransferOrderProtectionProxy::getPosCollection()
			{
				return m_TransferOrder->getPosCollection();
			}

			ITransferOrderPosCollectionPtr TransferOrderProtectionProxy::getPosCollection() const
			{
				return m_TransferOrder->getPosCollection();
			}

			const log4cplus::Logger& TransferOrderProtectionProxy::getLogger() const
			{
				return m_Logger;
			}

			bool TransferOrderProtectionProxy::isChanged() const
			{
				return m_TransferOrder->isChanged();
			}

			bool TransferOrderProtectionProxy::isEmpty() const
			{
				return m_TransferOrder->isEmpty();
			}

			void TransferOrderProtectionProxy::findById(basar::Int32 orderNo)
			{
				METHODNAME_DEF(TransferOrderProtectionProxy, findById);
				BLOG_TRACE_METHOD(getLogger(), fun);

				try
				{
					lock(orderNo);
					m_TransferOrder->findById(orderNo);
					checkForDataRace();
				}
				catch (libLockManager::exceptions::AlreadyLockedException&)
				{
					// provide better error msg
					basar::VarString msg = QCoreApplication::translate("TransferOrderProtectionProxy",
						"Order %1 is locked!").arg(orderNo).toLocal8Bit().constData();
					throw libLockManager::exceptions::AlreadyLockedException(basar::ExceptInfo(fun, msg, __FILE__, __LINE__));
				}
			}

			void TransferOrderProtectionProxy::findByIdWithoutLock(basar::Int32 orderNo)
			{
				METHODNAME_DEF(TransferOrderProtectionProxy, findById);
				BLOG_TRACE_METHOD(getLogger(), fun);

				try
				{
					m_TransferOrder->findById(orderNo);
					checkForDataRace();
				}
				catch (libLockManager::exceptions::AlreadyLockedException&)
				{
					// provide better error msg
					basar::VarString msg = QCoreApplication::translate("TransferOrderProtectionProxy",
						"findByIdWithoutLock %1 problem!").arg(orderNo).toLocal8Bit().constData();
					throw libLockManager::exceptions::AlreadyLockedException(basar::ExceptInfo(fun, msg, __FILE__, __LINE__));
				}
			}

			void TransferOrderProtectionProxy::save()
			{
				m_TransferOrder->save();
			}

			void TransferOrderProtectionProxy::clear()
			{
				m_TransferOrder->clear();
			}

			basar::Int32 TransferOrderProtectionProxy::getOrderNo() const
			{
				return m_TransferOrder->getOrderNo();
			}

			basar::Int32 TransferOrderProtectionProxy::getPositionCount() const
			{
				return m_TransferOrder->getPositionCount();
			}

			basar::Decimal TransferOrderProtectionProxy::getOrderValue() const
			{
				return m_TransferOrder->getOrderValue();
			}

			OrderStatus TransferOrderProtectionProxy::getOrderStatus() const
			{
				return m_TransferOrder->getOrderStatus();
			}

			TrafficLightEnum TransferOrderProtectionProxy::getColor() const
			{
				return m_TransferOrder->getColor();
			}

			bool TransferOrderProtectionProxy::hasOrderTypeOM() const
			{
				return m_TransferOrder->hasOrderTypeOM();
			}

			basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderProtectionProxy::addEmptyPosition()
			{
				return m_TransferOrder->addEmptyPosition();
			}

			void TransferOrderProtectionProxy::removePosition(basar::db::aspect::AccessorPropertyTable_YIterator yitPos)
			{
				m_TransferOrder->removePosition(yitPos);
			}

			void TransferOrderProtectionProxy::release(bool postpone)
			{
				m_TransferOrder->release(postpone);
			}

			void TransferOrderProtectionProxy::lock(basar::Int32 orderNo)
			{
				METHODNAME_DEF(TransferOrderProtectionProxy, lock)
					BLOG_TRACE_METHOD(getLogger(), fun);

				// check if lock has already been aquired
				if (m_LockEntry.get() == 0)
				{
					// lock this order before getting it from DB
					libLockManager::ILockEntryPtr lockEntry = m_TransferOrderLockFactory->createLockEntry(orderNo);
					m_LockManager->lock(*lockEntry);
					m_LockEntry = lockEntry; // Important: Assign after lock was aquired sucessfully
				}
			}

			void TransferOrderProtectionProxy::unlock()
			{
				METHODNAME_DEF(TransferOrderProtectionProxy, unlock)
					BLOG_TRACE_METHOD(getLogger(), fun);

				if (m_LockEntry.get() != 0)
				{
					m_LockManager->unlock(m_LockEntry->getKey());
					m_LockEntry.reset();
				}
			}

			void TransferOrderProtectionProxy::checkForDataRace()
			{
				METHODNAME_DEF(TransferOrderProtectionProxy, unlock)

					if (!isEmpty())
					{
						// this method checks if order status and color still matches - otherwise an exception is thrown
						// only UW orders in state "ZU" with can have green/yellow/red colors
						// it's enough here to check first position because a colored order has no colorless positions
						basar::db::aspect::AccessorPropertyTable_YIterator yit = m_TransferOrder->getPosCollection()->get().begin();

						const bool colorIsSet = yit.isContainedAndSet(properties::COLOR) && yit.getInt16(properties::COLOR) > 0;
						if (colorIsSet && getOrderStatus().getStatus() != OrderStatus::ZU)
						{
							basar::VarString msg = "order is not in sync with order head collection";
							throw exceptions::OutOfSyncException(basar::ExceptInfo(fun, msg, __FILE__, __LINE__));
						}
					}
			}

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw
