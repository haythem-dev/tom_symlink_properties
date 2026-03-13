//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "transferorderchangeruc.h"

#include "iusecasegetter.h"

#include "ichangeordersvc.h"

#include "libabbauw/itransferorderheadcollectionchanger.h"
#include "libabbauw/itransferorderhead.h"
#include "libabbauw/itransferorderheadptr.h"

#include "libabbauw/icustomerfinder.h"
#include "libabbauw/ifindmanufacturer.h"

#include "libabbauw/domainmodule/orderprotocol/iorderprotocol.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libutil/usecase.h>
#include <libutil/util.h>
#include <itransferorderptr.h>

using basar::db::aspect::AccessorPropertyTable_YIterator;

namespace CSC_Batches
{
	namespace useCase
	{
		BEGIN_HANDLERS_REGISTRATION(TransferOrderChangerUC)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderChangerUC, ApplyChangesRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderChangerUC, SaveOrderRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderChangerUC, FindPharmacyRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderChangerUC, UpdatePharmacyRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderChangerUC, FindManufacturerRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderChangerUC, UpdateManufacturerRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderChangerUC, CheckPharmacyNoRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderChangerUC, CheckManufacturerNoRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderChangerUC, OrderDiscountRequested)
		END_HANDLERS_REGISTRATION()

		TransferOrderChangerUC::TransferOrderChangerUC()
		{
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, "TransferOrderChangerUC::TransferOrderChangerUC().");
		}

		TransferOrderChangerUC::~TransferOrderChangerUC()
		{
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, "TransferOrderChangerUC::~TransferOrderChangerUC().");
		}

		void TransferOrderChangerUC::injectUseCaseGetter(componentManager::IUseCaseGetterPtr useCaseGetter)
		{
			m_UseCaseGetterPtr = useCaseGetter;
		}

		void TransferOrderChangerUC::injectVC(viewConn::IChangeOrdersVCPtr changeOrderVC)
		{
			m_ChangeOrdersVC = changeOrderVC;
		}

		void TransferOrderChangerUC::injectDM(libabbauw::domMod::transferOrder::ITransferOrderHeadCollectionChangerPtr transferOrderHeadColl)
		{
			m_TransferOrderHeadCollectionChangerPtr = transferOrderHeadColl;
		}

		void TransferOrderChangerUC::injectDM(libabbauw::domMod::manufacturerdm::IFindManufacturerPtr manufacturer)
		{
			m_FindManufacturerPtr = manufacturer;
		}

		void TransferOrderChangerUC::injectDM(libabbauw::domMod::customer::ICustomerFinderPtr customer)
		{
			m_CustomerFinderPtr = customer;
		}

		void TransferOrderChangerUC::injectDM(libabbauw::domMod::orderProtocol::IOrderProtocolPtr orderProtocol)
		{
			m_OrderProtocol = orderProtocol;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderChangerUC, UpdateManufacturerRequested)
		{
			METHODNAME_DEF(TransferOrderChangerUC, UpdateManufacturerRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try 
			{
				m_ChangeOrdersVC->matchFromManufacturer(yitFromEvSrc);

				result = HANDLER_OK;
			}
			catch (basar::Exception& e) 
			{
				m_ChangeOrdersVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderChangerUC, UpdatePharmacyRequested)
		{
			METHODNAME_DEF(TransferOrderChangerUC, UpdatePharmacyRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try 
			{
				m_ChangeOrdersVC->matchFromPharmacy(yitFromEvSrc);

				result = HANDLER_OK;
			}
			catch (basar::Exception& e) 
			{
				m_ChangeOrdersVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderChangerUC, CheckPharmacyNoRequested)
		{
			METHODNAME_DEF(TransferOrderChangerUC, CheckPharmacyNoRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try 
			{
				AccessorPropertyTable_YIterator yit = m_CustomerFinderPtr->getSearchYit();
				m_ChangeOrdersVC->matchToPharmacy(yit);
				m_CustomerFinderPtr->findByKey(yit.getInt32(libabbauw::properties::PHARMACYNO));

				m_ChangeOrdersVC->matchFromPharmacy(m_CustomerFinderPtr->get());

				result = HANDLER_OK;
			}
			catch (basar::Exception& e) 
			{
				m_ChangeOrdersVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderChangerUC, CheckManufacturerNoRequested)
		{
			METHODNAME_DEF(TransferOrderChangerUC, CheckManufacturerNoRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try 
			{
				AccessorPropertyTable_YIterator yit = m_FindManufacturerPtr->getSearchYit();
				m_ChangeOrdersVC->matchToManufacturer(yit);
				m_FindManufacturerPtr->findManufacturer(yit);

				m_ChangeOrdersVC->matchFromManufacturer(m_FindManufacturerPtr->getPropTab().begin());

				result = HANDLER_OK;
			}
			catch (basar::Exception& e) 
			{
				m_ChangeOrdersVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderChangerUC, OrderDiscountRequested)
		{
			METHODNAME_DEF(TransferOrderChangerUC, OrderDiscountRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;
			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try 
			{
				//AccessorPropertyTable_YIterator yit = m_FindManufacturerPtr->getSearchYit();
				//m_ChangeOrdersVC->matchToManufacturer(yit);
				//m_FindManufacturerPtr->findManufacturer(yit);

				//m_ChangeOrdersVC->matchFromOrderDiscount(m_FindManufacturerPtr->getPropTab().begin());

				result = HANDLER_OK;
			}
			catch (basar::Exception& e) 
			{
				m_ChangeOrdersVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}


		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderChangerUC, ApplyChangesRequested)
		{
			METHODNAME_DEF(TransferOrderChangerUC, ApplyChangesRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				m_ChangeOrdersVC->matchToOrders();
				m_ChangeOrdersVC->matchFromOrders();
				m_ChangeOrdersVC->setSavingAllowed(m_TransferOrderHeadCollectionChangerPtr->isChanged());

				result = HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				m_ChangeOrdersVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderChangerUC, SaveOrderRequested)
		{
			METHODNAME_DEF(TransferOrderChangerUC, SaveOrderRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				basar::VarString strDiscount = m_ChangeOrdersVC->getOrderDiscount();
				
				if (m_TransferOrderHeadCollectionChangerPtr->save(strDiscount) == true)
				{
					// Order Head Protocol
					AccessorPropertyTable_YIterator yitToProtocol = m_TransferOrderHeadCollectionChangerPtr->get().begin();

					std::vector<AccessorPropertyTable_YIterator> backupHeadCollection = m_OrderProtocol->getBackupHeadCollection();
					std::vector<AccessorPropertyTable_YIterator>::iterator itHeadCollectionBackup = backupHeadCollection.begin();

					while (!yitToProtocol.isEnd())
					{
						m_OrderProtocol->protocolChangeOrder(yitToProtocol, *itHeadCollectionBackup);

						++yitToProtocol;
						++itHeadCollectionBackup;
					}

					if (strDiscount.empty() == false)
					{
						// Order Positions Protocol	
						basar::Decimal discountDecimal("-" + strDiscount);

						std::vector<AccessorPropertyTable_YIterator> backupPosCollection = m_OrderProtocol->getBackupAllHeadPosCollection();
						std::vector<AccessorPropertyTable_YIterator>::iterator itPosCollectionBackup = backupPosCollection.begin();
						std::vector<AccessorPropertyTable_YIterator>::iterator itPosCollectionBackupEnd = backupPosCollection.end();

    					while (itPosCollectionBackup != itPosCollectionBackupEnd)
						{
#ifdef _DEBUG
							std::stringstream strstream2;
							itPosCollectionBackup->toStream(strstream2);
#endif
							m_OrderProtocol->protocolChangeOrderPositionDiscount(discountDecimal, *itPosCollectionBackup);

							++itPosCollectionBackup;
						}
					}

					// update list -> changed order is shown without new DB select
					basar::appl::SystemEventManager::getInstance().fire("TransferOrderChanged");
				}

				m_ChangeOrdersVC->shutdown();
				result = HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				m_ChangeOrdersVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderChangerUC, FindManufacturerRequested)
		{
			METHODNAME_DEF(TransferOrderChangerUC, FindManufacturerRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				m_UseCaseGetterPtr->getFindManufacturerUC()->run();
				result = basar::appl::HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				m_ChangeOrdersVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderChangerUC, FindPharmacyRequested)
		{
			METHODNAME_DEF(TransferOrderChangerUC, FindPharmacyRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				m_UseCaseGetterPtr->getFindCustomerUC()->run();
				result = basar::appl::HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				m_ChangeOrdersVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		void TransferOrderChangerUC::run()
		{
			METHODNAME_DEF(TransferOrderChangerUC, run)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);

			registerEventHandlers(true);

			m_ChangeOrdersVC->init(m_TransferOrderHeadCollectionChangerPtr->get());

			// store the current order head collection as backup
			m_OrderProtocol->setBackupHeadCollection(m_TransferOrderHeadCollectionChangerPtr->get().begin());

			// create backup for all head positions
			m_TransferOrderHeadCollectionChangerPtr->setBackupAllHeadPosCollection();

			m_OrderProtocol->setBackupAllHeadPosCollection(m_TransferOrderHeadCollectionChangerPtr->getBackupAllHeadPosCollection());

			// display screen to inform the user that we are working..
			m_ChangeOrdersVC->show();
			m_ChangeOrdersVC->matchFromOrders();

			// Displays Window and blocks it until screen is closed
			m_ChangeOrdersVC->waitForEvents();

			registerEventHandlers(false);
		}
	} // end namespace useCase
} // end namespace CSC_Batches
