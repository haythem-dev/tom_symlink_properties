#include "transferorderheadcollectionprotectionproxy.h"

#include "iorderlockfactory.h"
#include <ilockmanager.h>
#include <ilockentry.h>
#include <alreadylockedexception.h>

#include <libutil/misc/metaclassmacro_definitions.h>
#include <libbasarcmnutil_logging.h>

#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning(disable: 4512 )
#endif
#include <QtCore/QCoreApplication>
#ifdef _MSC_VER
#pragma warning (pop)
#endif

#include "libabbauw_properties_definitions.h"

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			TransferOrderHeadCollectionProtectionProxy::TransferOrderHeadCollectionProtectionProxy()
			{
			}

			TransferOrderHeadCollectionProtectionProxy::~TransferOrderHeadCollectionProtectionProxy()
			{
				releaseLocks();
			}

			void TransferOrderHeadCollectionProtectionProxy::injectTransferOrderHeadCollection(ITransferOrderHeadCollectionDMPtr transferOrderHeadColl)
			{
				m_TransferOrderHeadCollection = transferOrderHeadColl;
			}

			void TransferOrderHeadCollectionProtectionProxy::injectLockManager(libLockManager::ILockManagerPtr lockManager)
			{
				m_LockManager = lockManager;
			}

			void TransferOrderHeadCollectionProtectionProxy::injectTransferOrderLockFactory(lock::IOrderLockFactoryPtr transferOrderLockFactory)
			{
				m_TransferOrderLockFactory = transferOrderLockFactory;
			}

			basar::db::aspect::AccessorPropertyTableRef TransferOrderHeadCollectionProtectionProxy::get()
			{
				return m_TransferOrderHeadCollection->get();
			}

			basar::db::aspect::AccessorPropertyTableRef TransferOrderHeadCollectionProtectionProxy::get() const
			{
				return m_TransferOrderHeadCollection->get();
			}

			void TransferOrderHeadCollectionProtectionProxy::getGreenOrders(YIterators& greenOrders)
			{
				return m_TransferOrderHeadCollection->getGreenOrders(greenOrders);
			}

			basar::Int32 TransferOrderHeadCollectionProtectionProxy::getNumberOfGreenOrders() const
			{
				return m_TransferOrderHeadCollection->getNumberOfGreenOrders();
			}

			basar::Int32 TransferOrderHeadCollectionProtectionProxy::getTotalNumberOfOrders()
			{
				return m_TransferOrderHeadCollection->getTotalNumberOfOrders();
			}

			void TransferOrderHeadCollectionProtectionProxy::resetSearchYit()
			{
				m_TransferOrderHeadCollection->resetSearchYit();
			}

			basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderHeadCollectionProtectionProxy::getSearchYit()
			{
				return m_TransferOrderHeadCollection->getSearchYit();
			}

			void TransferOrderHeadCollectionProtectionProxy::findByPattern(const basar::db::aspect::AccessorPropertyTable_YIterator yIt, bool isOrderView)
			{
				return m_TransferOrderHeadCollection->findByPattern(yIt, isOrderView);
			}

			void TransferOrderHeadCollectionProtectionProxy::findByKey(const basar::db::aspect::AccessorPropertyTable_YIterator yIt)
			{
				return m_TransferOrderHeadCollection->findByKey(yIt);
			}

			bool TransferOrderHeadCollectionProtectionProxy::save(basar::VarString strDiscount)
			{
				return m_TransferOrderHeadCollection->save(strDiscount);
			}

			void TransferOrderHeadCollectionProtectionProxy::setBackupAllHeadPosCollection()
			{
				m_TransferOrderHeadCollection->setBackupAllHeadPosCollection();
			}

			std::vector<basar::db::aspect::AccessorPropertyTable_YIterator> TransferOrderHeadCollectionProtectionProxy::getBackupAllHeadPosCollection()
			{
				return m_TransferOrderHeadCollection->getBackupAllHeadPosCollection();
			}

			bool TransferOrderHeadCollectionProtectionProxy::isChanged() const
			{
				return m_TransferOrderHeadCollection->isChanged();
			}

			transferOrder::ITransferOrderPtr TransferOrderHeadCollectionProtectionProxy::getOrder(const basar::db::aspect::AccessorPropertyTable_YIterator yit)
			{
				return m_TransferOrderHeadCollection->getOrder(yit);
			}

			void TransferOrderHeadCollectionProtectionProxy::setReserveQtyFactorType(ReserveQuantityFactorType resQtyFactorType)
			{
				m_TransferOrderHeadCollection->setReserveQtyFactorType(resQtyFactorType);
			}

			void TransferOrderHeadCollectionProtectionProxy::addTransferOrderHead(basar::db::aspect::AccessorPropertyTable_YIterator yit)
			{
				METHODNAME_DEF(BatchOrderEditionUC, addTransferOrderHead)

				try
				{
					lock(yit.getInt32(properties::ORDERNO));
				}
				catch (libLockManager::exceptions::AlreadyLockedException&)
				{
					// provide better error msg
					basar::VarString msg = QCoreApplication::translate(
						"TransferOrderHeadCollectionProtectionProxy",
						"Order %1 is locked!").arg(yit.getInt32(libabbauw::properties::ORDERNO)).toLocal8Bit().constData();

					throw libLockManager::exceptions::AlreadyLockedException(basar::ExceptInfo(fun, msg, __FILE__, __LINE__));
				}

				m_TransferOrderHeadCollection->addTransferOrderHead(yit);
			}

			void TransferOrderHeadCollectionProtectionProxy::lock(basar::Int32 orderNo)
			{
				libLockManager::ILockEntryPtr lockEntry = m_TransferOrderLockFactory->createLockEntry(orderNo);
				m_LockManager->lock(*lockEntry);

				m_LockEntryCollection.push_back(lockEntry);
			}

			void TransferOrderHeadCollectionProtectionProxy::releaseLocks()
			{
				for (LockEntryCollection::const_iterator it = m_LockEntryCollection.begin(); it != m_LockEntryCollection.end(); ++it)
				{
					const libLockManager::ILockEntryPtr lockEntry = *it;
					m_LockManager->unlock(lockEntry->getKey());
				}
			}

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw
