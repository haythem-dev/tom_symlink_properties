//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "collectiveorderwishuc.h"

#include "icollectiveorderwishconfirmationvc.h"

#include "domainmodule/orderproposal/iorderproposalcollection.h"
#include "libabbauw/domainmodule/orderprotocol/iorderprotocol.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libutil/usecase.h>
#include <libutil/util.h>

namespace CSC_Batches
{
	namespace useCase
	{
		BEGIN_HANDLERS_REGISTRATION(CollectiveOrderWishUC)
			SYSTEM_EVENT_HANDLER_REGISTRATION(CollectiveOrderWishUC, CreateOrderWishesRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(CollectiveOrderWishUC, RemoveOrderWishRequested)
			END_HANDLERS_REGISTRATION()

			CollectiveOrderWishUC::CollectiveOrderWishUC()
		{
			METHODNAME_DEF(CollectiveOrderWishUC, CollectiveOrderWishUC);
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);
		}

		CollectiveOrderWishUC::~CollectiveOrderWishUC()
		{
			METHODNAME_DEF(CollectiveOrderWishUC, ~CollectiveOrderWishUC);
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);
		}

		void CollectiveOrderWishUC::injectVC(viewConn::ICollectiveOrderWishConfirmationVCPtr vc)
		{
			m_CollectiveOrderWishConfirmationVC = vc;
		}

		void CollectiveOrderWishUC::injectDM(libabbauw::domMod::orderProposal::IOrderProposalCollectionPtr proposalColl)
		{
			m_OrderProposalCollection = proposalColl;
		}

		void CollectiveOrderWishUC::injectDM(libabbauw::domMod::orderProtocol::IOrderProtocolPtr orderProtocol)
		{
			m_OrderProtocol = orderProtocol;
		}

		void CollectiveOrderWishUC::setIsOverViewActive(bool isOrderViewActive)
		{
			m_IsOrderViewActive = isOrderViewActive;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(CollectiveOrderWishUC, RemoveOrderWishRequested)
		{
			METHODNAME_DEF(CollectiveOrderWishUC, RemoveOrderWishRequested);
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::db::aspect::AccessorPropertyTable_YIterator yit = m_CollectiveOrderWishConfirmationVC->getSelectedOrderWish();
			if (!yit.isEnd())
			{
				m_OrderProtocol->setBackupPos(yit);

				m_OrderProposalCollection->removeItem(yit);

				m_OrderProtocol->protocolCancelOrderRequest(m_IsOrderViewActive);

				m_CollectiveOrderWishConfirmationVC->matchFromOrderWishes(m_OrderProposalCollection->get());
			}

			return basar::appl::EventReturnStruct();
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(CollectiveOrderWishUC, CreateOrderWishesRequested)
		{
			METHODNAME_DEF(CollectiveOrderWishUC, CreateOrderWishesRequested);
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);
			try
			{
				m_OrderProposalCollection->save();

				basar::db::aspect::AccessorPropertyTable_YIterator pos = m_OrderProposalCollection->get().begin();

				while (!pos.isEnd())
				{
					m_OrderProtocol->protocolOrderRequest(pos, m_IsOrderViewActive);

					++pos;
				}

				m_CollectiveOrderWishConfirmationVC->shutdown();

				result = basar::appl::HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				//m_CollectiveOrderWishConfirmationVC->error( e.what() );
				result.message = e.what();
			}

			basar::appl::SystemEventManager::getInstance().fire("TransferOrderChanged");
			return result;
		}

		void CollectiveOrderWishUC::run()
		{
			METHODNAME_DEF(CollectiveOrderWishUC, run)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);

			registerEventHandlers(true);
			try
			{
				m_CollectiveOrderWishConfirmationVC->matchFromOrderWishes(m_OrderProposalCollection->get());
				m_CollectiveOrderWishConfirmationVC->show();
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				throw;
			}

			registerEventHandlers(false);
		}

	} // end namespace useCase
} // end namespace CSC_Batches
