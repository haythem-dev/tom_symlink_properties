//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "orderprotocoldm.h"

#include "domainmodule/orderprotocol/iorderheadprotocol.h"
#include "domainmodule/orderprotocol/iorderposcollectionprotocol.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

#include "trafficlightcolorsenum.h"

#include <libutil/exception.h>
#include <libutil/util.h>

#include <boost/make_shared.hpp>

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
	namespace domMod
	{
		namespace orderProtocol
		{
			using basar::db::aspect::AccessorPropertyTable_YIterator;
			using basar::db::aspect::AccessorPropertyTableRef;

			//--------------------------------------------------------------------------------------------------//
			// object definition section
			//--------------------------------------------------------------------------------------------------//
			const libutil::misc::ClassInfo& OrderProtocolDM::getClassInfo()
			{
				static const libutil::misc::ClassInfo info("OrderProtocolDM", libutil::misc::Version(1, 0, 0, 0));
				return info;
			}

			OrderProtocolDM::OrderProtocolDM()
				: m_Logger(LoggerPool::loggerOrderProposal)
			{
				METHODNAME_DEF(OrderProtocolDM, OrderProtocolDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			OrderProtocolDM::~OrderProtocolDM()
			{
				METHODNAME_DEF(OrderProtocolDM, ~OrderProtocolDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			const log4cplus::Logger& OrderProtocolDM::getLogger() const
			{
				return m_Logger;
			}

			void OrderProtocolDM::injectOrderHeadProtocol(domMod::orderProtocol::IOrderHeadProtocolPtr dm)
			{
				m_Head = dm;
			}

			void OrderProtocolDM::injectOrderPosCollectionProtocol(IOrderPosCollectionProtocolPtr dm)
			{
				m_PosCollection = dm;
			}

			void OrderProtocolDM::injectTransactionFactory(libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory)
			{
				m_TransactionFactory = transactionFactory;
			}

			IOrderHeadProtocolPtr OrderProtocolDM::getOrderHeadProtocol() const
			{
				return m_Head;
			}

			IOrderPosCollectionProtocolPtr OrderProtocolDM::getOrderPosCollectionProtocol() const
			{
				return m_PosCollection;
			}

			bool OrderProtocolDM::isEmpty() const
			{
				//If head is empty, pos collection should be empty too!
				if (m_Head->get().isEnd() || m_Head->get().isNull())
				{
					return true;
				}

				return false;
			}

			AccessorPropertyTable_YIterator OrderProtocolDM::getSearchYit()
			{
				return getSearchYIterator()->get();
			}

			void OrderProtocolDM::resetSearchYit()
			{
				getSearchYIterator()->reset();
			}

			void OrderProtocolDM::findByKey(const AccessorPropertyTable_YIterator yitSearch)
			{
				METHODNAME_DEF(OrderProtocolDM, findByKey)
					BLOG_TRACE_METHOD(getLogger(), fun);

				try
				{
					m_Head->findByKey(yitSearch);

					//Search only for pos entries, if at least one head entry was found! 
					if (!m_Head->isEmpty())
					{
						m_PosCollection->findByKey(yitSearch);
					}
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
					throw;
				}
			}

			void OrderProtocolDM::save()
			{
				METHODNAME_DEF(OrderProtocolDM, save)
					BLOG_TRACE_METHOD(getLogger(), fun);

				if (isEmpty())
				{
					return;
				}

				try
				{
					m_Head->save();
					m_PosCollection->save();
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
					throw;
				}
			}

			bool OrderProtocolDM::isOrderProposalAllowed(const AccessorPropertyTable_YIterator yitPos)
			{
				return yitPos.getDateTime(properties::ORDERPROPOSALTIME).isValid() && yitPos.getState(properties::ORDERPROPOSALTIME) == basar::SS_UPDATE;
			}

			std::vector<AccessorPropertyTable_YIterator> OrderProtocolDM::getBackupHeadCollection()
			{
				METHODNAME_DEF(OrderProtocolDM, setBackupHeadCollection)
					BLOG_TRACE_METHOD(getLogger(), fun);

				return m_Head->getBackupHeadCollection();
			}

			std::vector<AccessorPropertyTable_YIterator> OrderProtocolDM::getBackupAllHeadPosCollection()
			{
				return m_PosCollection->getBackupAllHeadPosCollection();
			}

			void OrderProtocolDM::setBackupHead(const AccessorPropertyTable_YIterator yitHead)
			{
				METHODNAME_DEF(OrderProtocolDM, setBackupHead)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_Head->setBackupYit(yitHead);
			}

			void OrderProtocolDM::setBackupHeadCollection(const AccessorPropertyTable_YIterator yitHeadCollection)
			{
				METHODNAME_DEF(OrderProtocolDM, setBackupHeadCollection)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_Head->setBackupHeadCollection(yitHeadCollection);
			}

			void OrderProtocolDM::setBackupAllHeadPosCollection(const std::vector<AccessorPropertyTable_YIterator> allheadPositionsCollection)
			{
				METHODNAME_DEF(OrderProtocolDM, setBackupHeadCollection)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_PosCollection->setBackupAllHeadPosCollection(allheadPositionsCollection);
			}

			void OrderProtocolDM::setBackupPos(const AccessorPropertyTable_YIterator yitPos)
			{
				METHODNAME_DEF(OrderProtocolDM, setBackupPos)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_PosCollection->setBackupPos(yitPos);
			}

			void OrderProtocolDM::setBackupPos(const AccessorPropertyTable_YIterator yitPos, const basar::Int16 branchNo)
			{
				METHODNAME_DEF(OrderProtocolDM, setBackupPos)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_PosCollection->setBackupPos(yitPos, branchNo);
			}

			void OrderProtocolDM::setEventType(const EventTypeEnum eventType)
			{
				METHODNAME_DEF(OrderProtocolDM, setEventType)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_EventType = eventType;
			}

			void OrderProtocolDM::protocolCreateOrder(const AccessorPropertyTable_YIterator head, const AccessorPropertyTableRef posCollection)
			{
				METHODNAME_DEF(OrderProtocolDM, protocolCreateOrder)
					BLOG_TRACE_METHOD(getLogger(), fun);

				libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();

				try
				{
					transaction->begin();

					m_Head->protocolCreateOrder(head);
					m_PosCollection->protocolCreateOrder(posCollection);

					transaction->commit();
				}
				catch (basar::Exception& e)
				{
					transaction->rollback();

					libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
					throw;
				}
			}

			void OrderProtocolDM::protocolChangeOrder(const AccessorPropertyTable_YIterator yitHead, const AccessorPropertyTable_YIterator yitBackup)
			{
				METHODNAME_DEF(OrderProtocolDM, protocolChangeOrder)
					BLOG_TRACE_METHOD(getLogger(), fun);

#ifdef _DEBUG
				std::stringstream strstream;
				yitHead.toStream(strstream);
#endif

				if (yitHead.getString(properties::STATUS) == "ST")
				{
					m_Head->protocolChangeOrder(yitHead, yitBackup, CANCEL_ORDER);
				}
				else
				{
					m_Head->protocolChangeOrder(yitHead, yitBackup, NO_EVENT_TYPE);
				}
			}

			void OrderProtocolDM::protocolChangeOrderPositionDiscount(const basar::Decimal newValue, const AccessorPropertyTable_YIterator yitBackup)
			{
				METHODNAME_DEF(OrderProtocolDM, protocolChangeOrderPositionDiscount)
					BLOG_TRACE_METHOD(getLogger(), fun);

#ifdef _DEBUG
				std::stringstream strstream;
				yitBackup.toStream(strstream);
#endif

				m_PosCollection->protocolChangeOrderPositionDiscount(newValue, yitBackup);
			}

			void OrderProtocolDM::protocolDeletePositon(bool deleteHead)
			{
				METHODNAME_DEF(OrderProtocolDM, protocolDeletePositon)
					BLOG_TRACE_METHOD(getLogger(), fun);

				libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();

				try
				{
					transaction->begin();

					m_PosCollection->protocolDeletePositon();

					//If it was the last article, protocol that also the order was deleted!
					if (deleteHead)
					{
						m_Head->protocolDeleteOrder();
					}

					transaction->commit();
				}
				catch (basar::Exception& e)
				{
					transaction->rollback();

					libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
					throw;
				}
			}

			void OrderProtocolDM::protocolOrderRequest(AccessorPropertyTable_YIterator yitPos, bool isOrderViewActive)
			{
				METHODNAME_DEF(OrderProtocolDM, protocolOrderRequest)
					BLOG_TRACE_METHOD(getLogger(), fun);

				try
				{
					m_PosCollection->protocolOrderRequest(yitPos, isOrderViewActive);
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
					throw;
				}
			}

			void OrderProtocolDM::protocolCancelOrderRequest(bool isOrderViewActive)
			{
				METHODNAME_DEF(OrderProtocolDM, protocolCancelOrderRequest)
					BLOG_TRACE_METHOD(getLogger(), fun);

				try
				{
					m_PosCollection->protocolCancelOrderRequest(isOrderViewActive);
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
					throw;
				}
			}

			void OrderProtocolDM::protocolReleaseOrder(basar::Int32 orderNoCsc = 0)
			{
				METHODNAME_DEF(OrderProtocolDM, protocolReleaseOrder)
					BLOG_TRACE_METHOD(getLogger(), fun);

				try
				{
					m_Head->protocolReleaseOrder(m_EventType, orderNoCsc);
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
					throw;
				}
			}

			void OrderProtocolDM::protocolCreateTender()
			{
				METHODNAME_DEF(OrderProtocolDM, protocolCreateTender)
					BLOG_TRACE_METHOD(getLogger(), fun);

				try
				{
					m_Head->protocolCreateTender();
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
					throw;
				}
			}

			void OrderProtocolDM::protocolCreateTender(const basar::Int32 tenderNo)
			{
				METHODNAME_DEF(OrderProtocolDM, protocolCreateTender)
					BLOG_TRACE_METHOD(getLogger(), fun);

				try
				{
					m_PosCollection->protocolCreateTender(tenderNo);
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
					throw;
				}
			}

			void OrderProtocolDM::protocolSplitOrder(const AccessorPropertyTable_YIterator yitHead, const AccessorPropertyTableRef yitPosCollection,
				const AccessorPropertyTable_YIterator yitHeadSplit, const AccessorPropertyTableRef yitPosCollectionSplit,
				const EventTypeEnum eventType,
				bool  isOrderViewActive)
			{
				METHODNAME_DEF(OrderProtocolDM, protocolSplitOrder)
					BLOG_TRACE_METHOD(getLogger(), fun);

				libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();

				try
				{
					transaction->begin();

					//Old order
					m_Head->protocolSplitOrder(yitHead, UPDATE, eventType, 0, yitHeadSplit.getInt32(properties::ORDERNO));
					m_PosCollection->protocolSplitOrder(yitPosCollection, UPDATE, eventType, isOrderViewActive);

					//New splitted order
					m_Head->protocolSplitOrder(yitHeadSplit, CREATE, eventType, yitHead.getInt32(properties::ORDERNO), 0);
					m_PosCollection->protocolSplitOrder(yitPosCollectionSplit, CREATE, eventType, isOrderViewActive);

					transaction->commit();
				}
				catch (basar::Exception& e)
				{
					transaction->rollback();

					libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
					throw;
				}
			}

			libutil::domMod::SearchYIteratorPtr OrderProtocolDM::getSearchYIterator()
			{
				METHODNAME_DEF(OrderProtocolDM, getSearchYIterator)
					BLOG_TRACE_METHOD(getLogger(), fun);

				//TODO properties anpassen
				if (NULL == m_SearchYIterator.get())
				{
					m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(new libutil::domMod::SearchYIterator(
						properties::BRANCHNO.toDescriptionString() +
						properties::ORDERNO.toDescriptionString() +
						properties::POSNO.toDescriptionString())
					);
				}

				return m_SearchYIterator;
			}

		} // end namespace orderProposal
	} // end namespace domMod
} // end namespace libabbauw
