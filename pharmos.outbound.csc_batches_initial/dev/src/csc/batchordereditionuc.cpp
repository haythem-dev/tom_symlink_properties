//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "batchordereditionuc.h"

#include "iusecasegetter.h"
#include "icsvexportgetter.h"

#include "iorderreleaseinformationvc.h"

#include "libabbauw/itransferorderheadcollectionfinder.h"
#include "libabbauw/itransferorderhead.h"
#include "libabbauw/itransferorderheadptr.h"
#include "libabbauw/itransferorder.h"
#include "libabbauw/itransferorderposcollection.h"
#include "libabbauw/itransferorderposition.h"

#include "libabbauw/domainmodule/orderprotocol/iorderprotocol.h"

#include "libabbauw/icscorderreleaser.h"
#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libabbauw/exceptions/orderreleaseexception.h>
#include <libabbauw/exceptions/outofsyncexception.h>
#include <libabbauw/exceptions/orderlockedexception.h>
#include <alreadylockedexception.h>
#include "libabbauw/exceptions/ordernotfoundexception.h"
#include "libabbauw/domainmodule/orderprotocol/orderprotocolenums.h"
#include "libabbauw/iparameteredit.h"
#include "libabbauw/exceptions/tendercreationexception.h"
#include "libabbauw/domainmodule/orderprotocol/iorderheadprotocol.h"
#include "usecase/itendercreator.h"
#include <libutil/usecase.h>

#include "libabbauw_properties_definitions.h"

#include <QtWidgets/QApplication.h>

namespace CSC_Batches
{
	namespace useCase
	{
		// Registration of basic usecase methods (buttons of order screen and change-order screen)
		BEGIN_HANDLERS_REGISTRATION(BatchOrderEditionUC)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, SearchBatchOrderRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, ChangeOrderRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, ChangeOrderFilteredRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, SendOrderRequestsSelectedRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, SendOrderRequestsFilteredRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, ShowOrderPositionsRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, ReleaseOrdersRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, ReleaseOrdersPostponeRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, ReleaseGreenOrdersRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, ShowBVOStatusAll)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderEditionUC, TransferOrderChanged)
			END_HANDLERS_REGISTRATION()

			BatchOrderEditionUC::BatchOrderEditionUC()
		{
			METHODNAME_DEF(BatchOrderEditionUC, BatchOrderEditionUC);
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);
		}

		BatchOrderEditionUC::~BatchOrderEditionUC()
		{
			METHODNAME_DEF(BatchOrderEditionUC, ~BatchOrderEditionUC);
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);
		}

		void BatchOrderEditionUC::injectUseCaseGetter(componentManager::IUseCaseGetterPtr useCaseGetter)
		{
			m_UseCaseGetterPtr = useCaseGetter;
		}

		void BatchOrderEditionUC::injectVC(viewConn::IBatchOrderEditionVCPtr batchOrderEditionVC)
		{
			m_BatchOrderEditionVCPtr = batchOrderEditionVC;
		}

		void BatchOrderEditionUC::injectVC(viewConn::IOrderReleaseInformationVCPtr orderReleaseInfo)
		{
			m_OrderReleaseInformation = orderReleaseInfo;
		}

		void BatchOrderEditionUC::injectDM(libabbauw::domMod::transferOrder::ITransferOrderHeadCollectionFinderPtr orderHeadCollFinder)
		{
			m_TransferOrderHeadCollectionFinderPtr = orderHeadCollFinder;
		}

		void BatchOrderEditionUC::injectDM(libabbauw::domMod::orderProtocol::IOrderProtocolPtr orderProtocol)
		{
			m_OrderProtocol = orderProtocol;
		}

		void BatchOrderEditionUC::injectDM(libabbauw::domMod::parameter::IParameterEditPtr dm)
		{
			m_ParameterEdit = dm;
		}

		void BatchOrderEditionUC::injectTenderCreatorUC(useCase::ITenderCreatorPtr tcUCPtr)
		{
			m_tenderCreatorUCPtr = tcUCPtr;
		}

		useCase::ITenderCreatorPtr BatchOrderEditionUC::getTenderCreatorUC()
		{
			return m_tenderCreatorUCPtr;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, ShowBVOStatusAll)
		{
			METHODNAME_DEF(BatchOrderEditionUC, ShowBVOStatusAll)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				m_UseCaseGetterPtr->getBVOStatusUC()->run();
				result = basar::appl::HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->error(e.what());
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, SearchBatchOrderRequested)
		{
			METHODNAME_DEF(BatchOrderEditionUC, SearchBatchOrderRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			// For order search  and refreshing order screen 
			if (searchBatchOrders())
			{
				result = HANDLER_OK;
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, ChangeOrderRequested)
		{
			METHODNAME_DEF(BatchOrderEditionUC, ChangeOrderRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				m_UseCaseGetterPtr->getTransferOrderChangerUC(m_BatchOrderEditionVCPtr->getSelectedOrders())->run();
				result = HANDLER_OK;
			}
			catch (libLockManager::exceptions::AlreadyLockedException& e)
			{
				BLOG_INFO(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->info(e.reason());
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->error(e.what());
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, ChangeOrderFilteredRequested)
		{
			METHODNAME_DEF(BatchOrderEditionUC, ChangeOrderFilteredRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				m_UseCaseGetterPtr->getTransferOrderChangerUC(filteredOrdersToYitCollection())->run();
				result = HANDLER_OK;
			}
			catch (libLockManager::exceptions::AlreadyLockedException& e)
			{
				BLOG_INFO(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->info(e.reason());
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->error(e.what());
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, SendOrderRequestsSelectedRequested)
		{
			METHODNAME_DEF(BatchOrderEditionUC, SendOrderRequestsSelectedRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				viewConn::IBatchOrderEditionVC::YIterators selOrders = m_BatchOrderEditionVCPtr->getSelectedOrders();
				m_UseCaseGetterPtr->getCollectiveOrderWishUC(selOrders, m_BatchOrderEditionVCPtr->isOrderViewActive())->run();
				result = HANDLER_OK;
			}
			catch (libabbauw::exceptions::OrderLockedException& e)
			{
				BLOG_WARN(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->info(e.reason());
			}
			catch (basar::Exception& e)
			{
				BLOG_WARN(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->error(e.what());
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, SendOrderRequestsFilteredRequested)
		{
			METHODNAME_DEF(BatchOrderEditionUC, SendOrderRequestsFilteredRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				m_UseCaseGetterPtr->getFilteredCollectiveOrderWishUC(filteredOrdersToYitCollection(), m_BatchOrderEditionVCPtr->isOrderViewActive())->run();
				result = HANDLER_OK;
			}
			catch (libabbauw::exceptions::OrderLockedException& e)
			{
				BLOG_WARN(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->info(e.reason());
			}
			catch (basar::Exception& e)
			{
				BLOG_WARN(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->error(e.what());
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, TransferOrderChanged)
		{
			METHODNAME_DEF(BatchOrderEditionUC, TransferOrderChanged)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			searchBatchOrders();
			result = HANDLER_OK;

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, ShowOrderPositionsRequested)
		{
			METHODNAME_DEF(BatchOrderEditionUC, ShowOrderPositionsRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);

			try 
			{
				m_UseCaseGetterPtr->getTransferOrderPosUC(m_BatchOrderEditionVCPtr->getSelectedOrder(), m_BatchOrderEditionVCPtr->isOrderViewActive())->run();
				result = basar::appl::HANDLER_OK;
			}
			catch (libLockManager::exceptions::AlreadyLockedException& e) 
			{
				BLOG_INFO(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->info(e.reason());
			}
			catch (libabbauw::exceptions::OrderNotFoundException& e) 
			{
				BLOG_INFO(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->info(e.reason());
			}
			catch (libabbauw::exceptions::OutOfSyncException& e) 
			{
				BLOG_INFO(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->showOrderChangedNotification();
			}
			catch (basar::Exception& e) 
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->error(e.what());
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, ReleaseOrdersRequested)
		{
			METHODNAME_DEF(BatchOrderEditionUC, ReleaseOrdersRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			m_OrderProtocol->setEventType(libabbauw::domMod::orderProtocol::RELEASE);

			return releaseOrders(m_BatchOrderEditionVCPtr->getSelectedOrders(), false);
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, ReleaseOrdersPostponeRequested)
		{
			METHODNAME_DEF(BatchOrderEditionUC, ReleaseOrdersPostponeRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			m_OrderProtocol->setEventType(libabbauw::domMod::orderProtocol::DEFERRED_RELEASE);

			return releaseOrders(m_BatchOrderEditionVCPtr->getSelectedOrders(), true);
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderEditionUC, ReleaseGreenOrdersRequested)
		{
			METHODNAME_DEF(BatchOrderEditionUC, ReleaseGreenOrdersRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using libabbauw::domMod::transferOrder::ITransferOrderHeadCollectionFinder;
			ITransferOrderHeadCollectionFinder::YIterators greenOrders;

			m_OrderProtocol->setEventType(libabbauw::domMod::orderProtocol::GREEN_RELEASE);

			m_TransferOrderHeadCollectionFinderPtr->getGreenOrders(greenOrders);
			return releaseOrders(greenOrders, false);
		}

		basar::appl::EventReturnType BatchOrderEditionUC::releaseOrders(viewConn::IBatchOrderEditionVC::YIterators yits, bool postpone)
		{
			basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);
			m_OrderReleaseInformation->setNumberOfOrders(static_cast<basar::Int32>(yits.size()));
			m_OrderReleaseInformation->show();

			std::vector<basar::db::aspect::AccessorPropertyTable_YIterator> backups;
			std::vector<basar::db::aspect::AccessorPropertyTable_YIterator> tenderBackups;

			try
			{
				for (viewConn::IBatchOrderEditionVC::YIterators::const_iterator it = yits.begin(); it != yits.end(); ++it)
				{
					const basar::Int32 orderNo = it->getInt32(libabbauw::properties::ORDERNO);
					try
					{
						libabbauw::domMod::transferOrder::ITransferOrderPtr transferOrder = m_TransferOrderHeadCollectionFinderPtr->getOrder(*it);

						if (transferOrder->hasOrderTypeOM())
						{
							if (transferOrder->getOrderStatus().getStatus() != libabbauw::domMod::OrderStatus::ZU)
							{
								m_OrderReleaseInformation->printError(orderNo, QApplication::tr("Order doesn't have status ZU. Order might be released already!").toLocal8Bit().constData());
								continue;
							}

							tenderBackups.push_back(transferOrder->getHead()->get());
							getTenderCreatorUC()->createTender(transferOrder);
							if (getTenderCreatorUC()->hasErrors())
							{
								const ITenderCreator::ErrorList& errorList(getTenderCreatorUC()->getErrors());
								ITenderCreator::ErrorList::const_iterator iter = errorList.begin();
								for (; iter != errorList.end(); ++iter)
								{
									m_OrderReleaseInformation->printError(orderNo, *iter);
								}
							}
						}
						else
						{
							backups.push_back(transferOrder->getHead()->get());
							transferOrder->release(postpone);
						}
					}
					catch (libabbauw::exceptions::OrderReleaseException& e)
					{
						m_OrderReleaseInformation->printError(orderNo, e.reason());
					}
					catch (libLockManager::exceptions::AlreadyLockedException&)
					{
						m_OrderReleaseInformation->printOrderLockedMsg(orderNo);
					}
					catch (libabbauw::exceptions::OutOfSyncException& e)
					{
						BLOG_INFO(libabbauw::LoggerPool::loggerUseCases, e.what());
						m_BatchOrderEditionVCPtr->showOrderChangedNotification();
						break;
					}

					m_OrderReleaseInformation->update();
				}

				for (unsigned int backupCounter = 0; backups.size() > backupCounter; ++backupCounter)
				{
					basar::db::aspect::AccessorPropertyTable_YIterator backup = backups.at(backupCounter);

					m_OrderProtocol->setBackupHead(backup);

					m_TransferOrderHeadCollectionFinderPtr->findByKey(backup);
					basar::Int32 orderNoCsc = m_TransferOrderHeadCollectionFinderPtr->get().begin().getInt32(libabbauw::properties::ORDERNOCSC);

					m_OrderProtocol->protocolReleaseOrder(orderNoCsc);
				}

				std::vector<basar::db::aspect::AccessorPropertyTable_YIterator>::const_iterator iter = tenderBackups.begin();
				for (; iter != tenderBackups.end(); ++iter)
				{
					m_OrderProtocol->setBackupHead(*iter);
					m_OrderProtocol->protocolCreateTender();
				}

				result = basar::appl::HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->error(e.what());
			}

			m_OrderReleaseInformation->waitForEvents();
			searchBatchOrders(); // always refresh list with changed status of item
			return result;
		}

		bool BatchOrderEditionUC::isReserveQtyDifferent()
		{
			if (m_ParameterEdit->getReserveQuantityOrderFactor() == m_ParameterEdit->getReserveQuantityReleaseFactor())
			{
				if (!m_ParameterEdit->isNotSalesEffectiveReserveCheckSet())
				{
					return false;
				}
				else if (m_ParameterEdit->getNotSalesEffectiveReserveQtyOrderFactor() == m_ParameterEdit->getNotSalesEffectiveReserveQtyReleaseFactor())
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}

		viewConn::IBatchOrderEditionVC::YIterators BatchOrderEditionUC::filteredOrdersToYitCollection()
		{
			std::vector< basar::db::aspect::AccessorPropertyTable_YIterator > yitColl;
			yitColl.reserve(m_TransferOrderHeadCollectionFinderPtr->get().size());
			for (basar::db::aspect::AccessorPropertyTable_YIterator yit = m_TransferOrderHeadCollectionFinderPtr->get().begin(); !yit.isEnd(); ++yit)
			{
				yitColl.push_back(yit);
			}

			return yitColl;
		}

		// This method refreshes the UW-list
		bool BatchOrderEditionUC::searchBatchOrders()
		{
			bool ret = false;

			try
			{
				QApplication::setOverrideCursor(Qt::WaitCursor);
				QApplication::processEvents();

				m_TransferOrderHeadCollectionFinderPtr->resetSearchYit();
				basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_TransferOrderHeadCollectionFinderPtr->getSearchYit();
				m_BatchOrderEditionVCPtr->matchToSearchCriteria(yitSearch);

				// Gets UW-data according to where-condition from database and fills result set.
				// Calculates values and colours for traffic light fields.
				// Not only orders but also position data and stock calculation are included.
				libabbauw::domMod::transferOrder::ReserveQuantityFactorType resQtyFactorType;
				if (m_BatchOrderEditionVCPtr->isOrderViewActive())
				{
					resQtyFactorType = libabbauw::domMod::transferOrder::ORDER_FACTOR;
				}
				else
				{
					resQtyFactorType = libabbauw::domMod::transferOrder::RELEASE_FACTOR;
				}

				m_TransferOrderHeadCollectionFinderPtr->setReserveQtyFactorType(resQtyFactorType);
				m_TransferOrderHeadCollectionFinderPtr->findByPattern(yitSearch, m_BatchOrderEditionVCPtr->isOrderViewActive());

				m_BatchOrderEditionVCPtr->setNumberOfGreenOrders(m_TransferOrderHeadCollectionFinderPtr->getNumberOfGreenOrders());
				m_BatchOrderEditionVCPtr->setTotalNumberOfOrders(m_TransferOrderHeadCollectionFinderPtr->getTotalNumberOfOrders());

				// Refreshes the newly calculated data into visible list.
				m_BatchOrderEditionVCPtr->matchFromTransferOrders();

				ret = true;
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderEditionVCPtr->error(e.what());
			}

			QApplication::restoreOverrideCursor();

			return ret;
		}

		void BatchOrderEditionUC::run()
		{
			registerEventHandlers(true);

			// show window (non-blocking)
			m_BatchOrderEditionVCPtr->init(m_TransferOrderHeadCollectionFinderPtr->get(), isReserveQtyDifferent());
			m_BatchOrderEditionVCPtr->show();

			//fill list with data and wait for further events
			searchBatchOrders();
			m_BatchOrderEditionVCPtr->waitForEvents();

			registerEventHandlers(false);
		}

	} // namespace useCase
} //namespace CSC_Batches
