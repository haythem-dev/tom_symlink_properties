//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderinvoiceuc.h"

#include <libtender/domainmodule/constants.h>
#include "domainmodule/libtender_properties_definitions.h"
#include <tenderinvoicesvc.h>
#include <domainmodule/tender/itenderprotocol.h>
#include <domainmodule/tender/itenderprogress.h>

#include <domainmodule/cscorder/icscorder.h>
#include "domainmodule/cscorder/icscorderposition.h"

#include <libtender/domainmodule/constants.h>

#include <loggerpool/tenderloggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>

#include <libbasardbaspect_exceptions.h>
#include <libbasarlogin.h>

#include "infrastructure/cscclient/icscclientconnection.h"
#include "infrastructure/cscclient/icscclientconnectionptr.h"

#include <infrastructure/cscclient/icscclientparameterproxy.h>
#include <infrastructure/cscclient/icscclientparameterproxyptr.h>

#ifdef WIN32
#pragma warning (push)
#pragma warning (disable : 4512) // assignment operator could not be generated
#pragma warning (disable : 4127) // conditional expression is constant
#pragma warning (disable : 4505) // unreferenced local function has been removed
#pragma warning (disable : 4706)
#endif
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TEncodedProtocol.h>
#include <thrift/protocol/BoostEncoder.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/TApplicationException.h>
#include <gen-cpp/CscServiceBatch.h>
#include <gen-cpp/cscbase_constants.h>
#ifdef WIN32
#pragma warning (pop)
#endif


using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace csc::base;
using namespace csc::service;

//-------------------------------------------------------------------------------------------------//
// namespace sections
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace useCase
    {
        BEGIN_HANDLERS_REGISTRATION( TenderInvoiceUC )
			SYSTEM_EVENT_HANDLER_REGISTRATION( TenderInvoiceUC, CreateTenderInvoiceRequested )
        END_HANDLERS_REGISTRATION()

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // TenderInvoiceUC::TenderInvoiceUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TenderInvoiceUC::TenderInvoiceUC()
        : m_Logger( libtender::LoggerPool::getLoggerUseCases() )
        {
            METHODNAME_DEF( TenderInvoiceUC, TenderInvoiceUC )
            BLOG_TRACE_METHOD( getLogger(), fun );

			m_OrderType = constants::ORDER_TYPE_TI;
            registerEventHandlers( true );
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // TenderInvoiceUC::~TenderInvoiceUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TenderInvoiceUC::~TenderInvoiceUC()
        {
            METHODNAME_DEF( TenderInvoiceUC, ~TenderInvoiceUC )
            BLOG_TRACE_METHOD( getLogger(), fun );

            registerEventHandlers( false );
        }

		void TenderInvoiceUC::injectCscParamProxy(libtender::infrastructure::cscclient::ICscClientParameterProxyPtr paramProxy)
		{
			METHODNAME_DEF(TenderInvoiceUC, injectCscParamProxy)
			BLOG_TRACE_METHOD(getLogger(), fun);

			m_ParamProxy = paramProxy;
		}

        void TenderInvoiceUC::injectTenderInvoiceVC( tender::viewConn::TenderInvoicesVCPtr tenderInvoiceVC)
        {
            METHODNAME_DEF( TenderInvoiceUC, injectTenderInvoiceVC )
            BLOG_TRACE_METHOD( getLogger(), fun );

            m_TenderInvoiceVC = tenderInvoiceVC;
        }

		void TenderInvoiceUC::injectCscOrderDM( libtender::domMod::cscorder::ICscOrderPtr cscOrderDM )
		{
			METHODNAME_DEF( TenderInvoiceUC, injectCscOrderDM )
            BLOG_TRACE_METHOD( getLogger(), fun );

			m_CscOrderDM = cscOrderDM;
		}

		void TenderInvoiceUC::injectTenderProtocolDM( libtender::domMod::tender::ITenderProtocolPtr tenderProtocol )
		{
			METHODNAME_DEF( TenderInvoiceUC, injectTenderProtocol )
            BLOG_TRACE_METHOD( getLogger(), fun );

			m_TenderProtocol = tenderProtocol;
		}

		void TenderInvoiceUC::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory )
		{
			METHODNAME_DEF( TenderInvoiceUC, injectTransactionFactory )
            BLOG_TRACE_METHOD( getLogger(), fun );

			m_TransactionFactory = transactionFactory;
		}

		void TenderInvoiceUC::injectOrderType( basar::VarString orderType )
		{
			METHODNAME_DEF( TenderInvoiceUC, injectOrderType )
            BLOG_TRACE_METHOD( getLogger(), fun );

			m_OrderType = orderType;
		}

		void TenderInvoiceUC::setTenderProgress( libtender::domMod::tender::ITenderProgressPtr tenderProgress )
		{
			METHODNAME_DEF( TenderInvoiceUC, setTenderProgress )
            BLOG_TRACE_METHOD( getLogger(), fun );

			m_TenderProgress = tenderProgress;
		}
		
		libtender::domMod::tender::ITenderProgressPtr TenderInvoiceUC::getTenderProgress()
		{
			METHODNAME_DEF( TenderInvoiceUC, getTenderProtocol )
			BLOG_TRACE_METHOD( getLogger(), fun );

			return m_TenderProgress;
		}

        tender::viewConn::TenderInvoicesVCPtr TenderInvoiceUC::getTenderInvoiceVC()
        {
            METHODNAME_DEF( TenderInvoiceUC, getTenderInvoiceVC )
            BLOG_TRACE_METHOD( getLogger(), fun );

            CHECK_INSTANCE_EXCEPTION( m_TenderInvoiceVC.get() );
            return m_TenderInvoiceVC;
        }

		libtender::domMod::cscorder::ICscOrderPtr TenderInvoiceUC::getCscOrderDM()
		{
			METHODNAME_DEF( TenderInvoiceUC, getCscOrderDM )
            BLOG_TRACE_METHOD( getLogger(), fun );

            CHECK_INSTANCE_EXCEPTION( m_CscOrderDM.get() );
            return m_CscOrderDM;
		}

        const log4cplus::Logger& TenderInvoiceUC::getLogger() const
        {
            METHODNAME_DEF( TenderInvoiceUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

        void TenderInvoiceUC::init()
        {
            METHODNAME_DEF( TenderInvoiceUC, init )
            BLOG_TRACE_METHOD( getLogger(), fun );
        }

        void TenderInvoiceUC::reset()
        {
            METHODNAME_DEF( TenderInvoiceUC, reset )
			BLOG_TRACE_METHOD( getLogger(), fun );

            init();
        }

        void TenderInvoiceUC::run()
        {
            METHODNAME_DEF( TenderInvoiceUC, run )
            BLOG_TRACE_METHOD( getLogger(), fun );

            init();

			getTenderInvoiceVC()->matchFromTenderProtocol( m_TenderProgress->get() );
            getTenderInvoiceVC()->show();
        }

//#define TENDER_KSC_SERVER
#ifdef TENDER_KSC_SERVER
		int TenderInvoiceUC::createCscOrder()
		{
			int cscRetVal;
			basar::db::aspect::AccessorPropertyTable_YIterator progress = getTenderProgress()->get().begin();
			basar::Int16 branch = progress.getInt16( properties::ACC_BRANCH_NO );
			basar::Int32 customerNo = progress.getInt32( properties::ACC_CUSTOMER_NO );

			cscRetVal = m_CscOrderDM->openOrderVz( branch, customerNo, m_OrderType );
			if( cscRetVal != 0 )
			{
				std::stringstream message;
				message << "openOrder rc=" <<  cscRetVal << ", " << getCscOrderDM()->getLackReason() << " " << getCscOrderDM()->getText();
				
				getTenderInvoiceVC()->showErrorMessage( message.str(), "Error while creating CSC order" );
				BLOG_ERROR( m_Logger, message.str() );

				return cscRetVal;
			}

			cscRetVal = m_CscOrderDM->setOrderType(m_OrderType, constants::PICKING_TYPE, constants::BOOKING_TYPE);
			if( cscRetVal != 0 )
			{
				std::stringstream message;
				message << "setOrderType rc=" <<  cscRetVal << ", " << getCscOrderDM()->getLackReason() << " " << getCscOrderDM()->getText();
				
				getTenderInvoiceVC()->showErrorMessage( message.str(), "Error while setting order type" );
				BLOG_ERROR( m_Logger, message.str() );

				return cscRetVal;
			}

			cscRetVal = getCscOrderDM()->setTenderNo( progress.getInt32( properties::TENDER_NO ) );

			basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_TenderProtocol->get().begin();

			for( ; !yitPos.isEnd(); ++yitPos )
			{
				libtender::domMod::cscorder::ICscOrderPositionPtr cscPos;

				cscPos = createCscOrderPosition(yitPos);
				if( cscPos->getLackReason() > 0 )
				{
					std::stringstream message;
					message << cscPos->getOrderQuantity() << " x Article " << cscPos->getArticleCode() << ": delivered " 
						    << cscPos->getDeliveryQuantity() << " " << cscPos->getLackReason() << " " << cscPos->getText();

					getTenderInvoiceVC()->showErrorMessage( message.str(), "Error while creating CSC order" );
					BLOG_ERROR(m_Logger, message.str());

					return cscRetVal;
				}
			}

			cscRetVal = getCscOrderDM()->closeOrder();

            if (cscRetVal != 0)
			{
				std::stringstream message;
				message << "closeOrder rc=" <<  cscRetVal << ", " << getCscOrderDM()->getLackReason() << " " << getCscOrderDM()->getText();
				
				getTenderInvoiceVC()->showErrorMessage( message.str(), "Error while closing CSC order" );
				BLOG_ERROR( m_Logger, message.str() );

				return cscRetVal;
			}

			return cscRetVal;
		}
#else //CSC Service
		csc::order::PlaceOrderResponse TenderInvoiceUC::createCscOrder()
		{
			METHODNAME_DEF(TenderInvoiceUC, createCscOrder)
			BLOG_TRACE_METHOD(m_Logger, fun);

			boost::shared_ptr<TTransport>				socket(new TSocket(m_ParamProxy->getCscServer(), m_ParamProxy->getCscPort()));
			boost::shared_ptr<TTransport>				transport(new TBufferedTransport(socket));
			boost::shared_ptr<TProtocol>				binaryProtocol(new TBinaryProtocol(transport));
			boost::shared_ptr<TEncoder>					encoder(new BoostEncoder(BoostEncoder::CodePage::WEST));
			boost::shared_ptr<TProtocol>				encodedProtocol(new TEncodedProtocol(binaryProtocol, encoder));
			boost::shared_ptr<CscServiceBatchClient>	client(new CscServiceBatchClient(encodedProtocol));
			
			basar::db::aspect::AccessorPropertyTable_YIterator progress = getTenderProgress()->get().begin();
			basar::Int16 branch = progress.getInt16(properties::ACC_BRANCH_NO);
			basar::Int32 customerNo = progress.getInt32(properties::ACC_CUSTOMER_NO);

			basar::cmnutil::CollBString	posErrorList;
						
			csc::order::PlaceOrderRequest request;

			// openorder
			csc::order::OpenOrderRequest openrequest;

			csc::base::SessionRequest	sessionRequest;
			csc::base::SessionResponse	sessionResponse;

			try
			{
				transport->open();
				sessionRequest.__set_DeviceName("TENDER");
				sessionRequest.__set_Uid(0);
				client->openSession(sessionResponse, sessionRequest);
			}
			catch (basar::Exception& e)
			{
				libutil::exceptions::ExceptionHandler::processBasarException(e, getLogger(), fun, __FILE__, __LINE__);
			}

			openrequest.__set_BranchNo(branch);
			openrequest.__set_CustomerNo(customerNo);
			openrequest.__set_TenderNo(progress.getInt32(properties::TENDER_NO));  
			openrequest.__set_OrderType(m_OrderType); //TODO use UADM?  
			openrequest.__set_BookingType(constants::BOOKING_TYPE); //TODO use UADM?
			openrequest.__set_ShipmentType(constants::PICKING_TYPE); //TODO use UADM?
			request.__set_OpenRequest(openrequest);

			// add positions
			csc::order::OrderPositionRequestList poslist;

			basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_TenderProtocol->get().begin();
			while (!yitPos.isEnd())
			{
				csc::order::OrderPositionRequest requestPos;

				std::stringstream article;
				article << yitPos.getInt32(properties::ARTICLE_NO);

				requestPos.__set_PosType(csc::types::PositionTypeEnum::TypeProductStocked);//TODO do we need that? 
				requestPos.__set_ArticleCode(article.str());
				requestPos.__set_QuantityOrdered(yitPos.getInt32(properties::QUANTITY_TO_CHARGE));
				requestPos.__set_AcceptInterBranchDelivery(true);
				requestPos.__set_AcceptPartialDelivery(true);
				requestPos.__set_AcceptDispoDelivery(false);
				requestPos.__set_AcceptRestDelivery(false);
				poslist.push_back(requestPos);
				++yitPos;
			}

			request.__set_Positions(poslist);

			// close order
			csc::order::CloseOrderRequest closerequest;

			closerequest.__set_CloseMode(csc::order::OrderCloseMode::CLOSE_MODE_CLOSE);
			request.__set_CloseRequest(closerequest);

			csc::order::PlaceOrderResponse response;
			client->placeNewOrder(response, request);

			client->closeSession(sessionResponse);
			transport->close();

			return response;
		}

#endif //TENDER_KSC_SERVER

		libtender::domMod::cscorder::ICscOrderPositionPtr TenderInvoiceUC::createCscOrderPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitPos )
		{
			basar::Int32 articleCode	= yitPos.getInt32( properties::ARTICLE_NO         );
			basar::Int32 quantity		= yitPos.getInt32( properties::QUANTITY_TO_CHARGE );
			std::stringstream article;
			article << articleCode;

			libtender::domMod::cscorder::ICscOrderPositionPtr cscPos;

			cscPos = getCscOrderDM()->createEmptyPosition( article.str(), quantity );
			cscPos->setIbtPossible(true);
			cscPos->setRestDeliveryPossible(false);
			cscPos->setDispoPossible(false);
			cscPos->setPartialDeliveryPossible(true);
			cscPos->save();
			return cscPos;
		}

		void TenderInvoiceUC::copyProgressToProtocol()
		{
			basar::db::aspect::AccessorPropertyTable_YIterator progress = getTenderProgress()->get().begin();
			for( ; !progress.isEnd(); ++progress )
			{
				if( 0 < progress.getInt32( properties::QUANTITY_TO_CHARGE ) )
				{
					basar::db::aspect::AccessorPropertyTable_YIterator order = m_TenderProtocol->get().insert( basar::FOR_INSERT );
					
					order.setInt32( properties::ORDER_NO,			progress.getInt32( properties::ORDER_NO )			);
					order.setDate ( properties::ORDER_DATE,			progress.getDate(  properties::ORDER_DATE )			);
					order.setInt32( properties::ARTICLE_NO,			progress.getInt32( properties::ARTICLE_NO )			);
					order.setInt32( properties::QUANTITY_TO_CHARGE, progress.getInt32( properties::QUANTITY_TO_CHARGE ) );
				}
			}
		}

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SYSTEM_EVENT_HANDLER_DEFINITION( TenderInvoiceUC, CreateTenderInvoiceRequested )
        {
            METHODNAME_DEF( TenderInvoiceUC, CreateTenderInvoiceRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;
			
            basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
			
            try
            {
				libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_TransactionFactory->createDBTransaction();

				copyProgressToProtocol();

				transaction->begin();
				m_TenderProtocol->saveProtocol();

				csc::order::PlaceOrderResponse response = createCscOrder();
				
				//TODO put into separate method
				bool hasResponseAnError = false;
				if (response.OpenOrder.RetValue.ErrorCode != csc::types::MessageCodeEnum::CMSG_OK)
				{
					hasResponseAnError = true;

					std::stringstream ss;
					ss << "OpenOrder error for order no " << response.CloseOrder;
					ss << " BranchNo: " << response.OpenOrder.BranchNo << " CustomerNo: " << response.OpenOrder.CustomerNo;
					ss << " ErrorCode: " << response.OpenOrder.RetValue.ErrorCode << " ErrorMessage: " << response.OpenOrder.RetValue.ErrorMessage;
					BLOG_ERROR(m_Logger, ss.str());
				}

				if (response.CloseOrder.RetValue.ErrorCode != csc::types::MessageCodeEnum::CMSG_OK)
				{
					hasResponseAnError = true;

					std::stringstream ss;
					ss << "CloseOrder error for order no " << response.CloseOrder.OrderNo;
					ss << " BranchNo: " << response.CloseOrder.BranchNo << " CustomerNo: " << response.CloseOrder.CustomerNo;
					ss << " ErrorCode: " << response.CloseOrder.RetValue.ErrorCode << " ErrorMessage: " << response.CloseOrder.RetValue.ErrorMessage;
					BLOG_ERROR(m_Logger, ss.str());
				}

				csc::order::OrderPositionResponseList::const_iterator it = response.Positions.begin();

				for (; it != response.Positions.end(); ++it)
				{
					if (it->RetValue.ErrorCode != csc::types::MessageCodeEnum::CMSG_OK)
					{
						hasResponseAnError = true;

						std::stringstream ss;
						ss << "Position error for order no " << response.OpenOrder.OrderNo;
						ss << " BranchNo: " << response.CloseOrder.BranchNo << " CustomerNo: " << response.CloseOrder.CustomerNo;
						ss << " ArticleCode: " << it->ArticleDelivered.ArticleCode;
						ss << " QtyOrdered: " << it->QuantityOrdered << " QtyLack: " << it->QuantityLack << " QtyIBT: " << it->QuantityInterBranch;
						ss << " ErrorCode: " << it->RetValue.ErrorCode << " ErrorMessage: " << it->RetValue.ErrorMessage;
						BLOG_ERROR(m_Logger, ss.str());
					}
				}

				//show error to user, rollback transaction if needed
				if (hasResponseAnError)
				{
					std::stringstream message;
					message << "Error code: " << response.RetValue.ErrorCode << ": " << response.RetValue.ErrorMessage;
					getTenderInvoiceVC()->showErrorMessage(message.str(), "Error creating CSC order");

					transaction->rollback();
					return result;
				}

				transaction->commit();
				getTenderInvoiceVC()->close();
			}
            catch( const basar::Exception & e )
            {
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
            }
		
            return result;
        }


    } // namespace usecase
} // namespace tender
