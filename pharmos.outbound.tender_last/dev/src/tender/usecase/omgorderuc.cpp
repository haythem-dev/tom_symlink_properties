#include "omgorderuc.h"

#include "domainmodule/libtender_properties_definitions.h"

#include <libtender/domainmodule/constants.h>
#include <tenderinvoicesvc.h>

#include <domainmodule/tender/itenderpositioncollectionptr.h>
#include <domainmodule/tender/tenderpositioncollection.h>

#include <domainmodule/tender/itender.h>
#include <domainmodule/tender/tenderdm.h>

#include <domainmodule/tender/itenderprotocol.h>
#include <domainmodule/tender/itenderprogress.h>

#include <domainmodule/cscorder/icscorder.h>
#include <domainmodule/cscorder/icscorderposition.h>

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

namespace tender
{
    namespace useCase
    {
        OMGOrderUC::OMGOrderUC(libtender::domMod::tender::ITenderPtr tender)
        : m_Logger( libtender::LoggerPool::getLoggerUseCases() )
        {
            METHODNAME_DEF( OMGOrderUC, OMGOrderUC )
            BLOG_TRACE_METHOD( getLogger(), fun );
            m_Tender = tender;
        }

        OMGOrderUC::~OMGOrderUC()
        {
            METHODNAME_DEF( OMGOrderUC, ~OMGOrderUC )
            BLOG_TRACE_METHOD( getLogger(), fun );

        }

        void OMGOrderUC::init()
        {
        
        }

        void OMGOrderUC::reset()
        {
        
        }


        const log4cplus::Logger& OMGOrderUC::getLogger() const
        {
            METHODNAME_DEF( OMGOrderUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );
            return m_Logger;
        }

        void OMGOrderUC::injectCscParamProxy(libtender::infrastructure::cscclient::ICscClientParameterProxyPtr paramProxy)
        {
            METHODNAME_DEF(TenderInvoiceUC, injectCscParamProxy)
            BLOG_TRACE_METHOD(getLogger(), fun);

            m_ParamProxy = paramProxy;
        }

        void OMGOrderUC::injectCscOrderDM( libtender::domMod::cscorder::ICscOrderPtr cscOrderDM )
        {
            METHODNAME_DEF( OMGOrderUC, injectCscOrderDM )
            BLOG_TRACE_METHOD( getLogger(), fun );

            m_CscOrderDM = cscOrderDM;
        }

        libtender::domMod::cscorder::ICscOrderPtr OMGOrderUC::getCscOrderDM()
        {
            METHODNAME_DEF( OMGOrderUC, getCscOrderDM )
            BLOG_TRACE_METHOD( getLogger(), fun );

            CHECK_INSTANCE_EXCEPTION( m_CscOrderDM.get() );
            return m_CscOrderDM;
        }

        void OMGOrderUC::setTender( libtender::domMod::tender::ITenderPtr tender )
        {
            METHODNAME_DEF( OMGOrderUC, setTender )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_Tender = tender;
        }

        libtender::domMod::tender::ITenderPtr OMGOrderUC::getTender()
        {
            METHODNAME_DEF( OMGOrderUC, getTender )
            BLOG_TRACE_METHOD( m_Logger, fun );
            return m_Tender;

        }

        //DZ: is this obsolete??? seems like maintainuc uses cscorderuc to create the csc orders 
        void OMGOrderUC::run()
        {
            std::stringstream strCSCOrderCreated;
            std::stringstream strCSCOrderFailed;
            basar::Int32 nCSCOrderCreated = 0;
            basar::Int32 nCSCOrderfailed = 0;

            if( NULL != getTender().get() )
            {                
                csc::order::PlaceOrderResponse response = createCscOrder();

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

                if (hasResponseAnError)
                {
                    ++nCSCOrderfailed;
                    
                }
                else
                {
                    ++nCSCOrderCreated;
                }
            }

            if ((nCSCOrderCreated + nCSCOrderfailed) == 0) return;

            std::stringstream ss;
            if (nCSCOrderfailed == 0)
            {
                ss << "All orders are created successfully. \n Order Created : " << nCSCOrderCreated << "\n" << strCSCOrderCreated.str();
            }
            else
            {
                ss << "Order Created : " << nCSCOrderCreated << "\n" + strCSCOrderCreated.str() + "\nOrder Failed : " << nCSCOrderfailed << "\n" << strCSCOrderFailed.str();               
            }
            BLOG_ERROR(m_Logger, ss.str());
        }
//#define TENDER_KSC_SERVER
#ifdef TENDER_KSC_SERVER
        int OMGOrderUC::createCscOrder()
        {
            int cscRetVal;

            basar::db::aspect::AccessorPropertyTable_YIterator tenHeader = getTender()->getHeader();

            libtender::domMod::tender::ITenderPositionCollectionPtr tenderPos = getTender()->getPositionCollection();


            basar::Int16 branch = tenHeader.getInt16( properties::ACC_BRANCH_NO );
            basar::Int32 customerNo = tenHeader.getInt32( properties::ACC_CUSTOMER_NO );

            cscRetVal = m_CscOrderDM->openOrderVz( branch, customerNo, constants::ORDER_TYPE_OM);
            if( cscRetVal != 0 )
            {
                std::stringstream message;
                message << "openOrder rc=" <<  cscRetVal << ", " << getCscOrderDM()->getLackReason() << " " << getCscOrderDM()->getText();
                //getTenderInvoiceVC()->showErrorMessage( message.str(), "Error while creating CSC order" );
                BLOG_ERROR( m_Logger, message.str() );
                return cscRetVal;
            }

            cscRetVal = m_CscOrderDM->setOrderType(constants::ORDER_TYPE_OM, constants::PICKING_TYPE_OM, constants::BOOKING_TYPE_OM);
            if( cscRetVal != 0 )
            {
                std::stringstream message;
                message << "setOrderType rc=" <<  cscRetVal << ", " << getCscOrderDM()->getLackReason() << " " << getCscOrderDM()->getText();
                //getTenderInvoiceVC()->showErrorMessage( message.str(), "Error while setting order type" );
                BLOG_ERROR( m_Logger, message.str() );
                return cscRetVal;
            }

            cscRetVal = getCscOrderDM()->setTenderNo( tenHeader.getInt32( properties::TENDER_NO ) );

            basar::db::aspect::AccessorPropertyTable_YIterator yitPos = tenderPos->get().begin();

            for( ; !yitPos.isEnd(); ++yitPos )
            {
                libtender::domMod::cscorder::ICscOrderPositionPtr cscPos;

                cscPos = createCscOrderPosition(yitPos);
                if( cscPos->getLackReason() > 0 )
                {
                    std::stringstream message;
                    message << cscPos->getOrderQuantity() << " x Article " << cscPos->getArticleCode() << ": delivered " 
                            << cscPos->getDeliveryQuantity() << " " << cscPos->getLackReason() << " " << cscPos->getText();

                    //getTenderInvoiceVC()->showErrorMessage( message.str(), "Error while creating CSC order" );
                    BLOG_ERROR(m_Logger, message.str());

                    return cscRetVal;
                }
            }

            cscRetVal = getCscOrderDM()->closeOrder();

            return cscRetVal;
        }
#else //CSC Service
        csc::order::PlaceOrderResponse OMGOrderUC::createCscOrder()
        {
            METHODNAME_DEF(MainUC, createCscOrder)
            BLOG_TRACE_METHOD(m_Logger, fun);

            boost::shared_ptr<TTransport>				socket(new TSocket(m_ParamProxy->getCscServer(), m_ParamProxy->getCscPort()));
            boost::shared_ptr<TTransport>				transport(new TBufferedTransport(socket));
            boost::shared_ptr<TProtocol>				binaryProtocol(new TBinaryProtocol(transport));
            boost::shared_ptr<TEncoder>					encoder(new BoostEncoder(BoostEncoder::CodePage::WEST));
            boost::shared_ptr<TProtocol>				encodedProtocol(new TEncodedProtocol(binaryProtocol, encoder));
            boost::shared_ptr<CscServiceBatchClient>	client(new CscServiceBatchClient(encodedProtocol));


            basar::db::aspect::AccessorPropertyTable_YIterator tenHeader = getTender()->getHeader();
            libtender::domMod::tender::ITenderPositionCollectionPtr tenderPos = getTender()->getPositionCollection();

            basar::Int16 branch = tenHeader.getInt16(properties::ACC_BRANCH_NO);
            basar::Int32 customerNo = tenHeader.getInt32(properties::ACC_CUSTOMER_NO);

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
            openrequest.__set_TenderNo(tenHeader.getInt32(properties::TENDER_NO));
            openrequest.__set_OrderType(constants::ORDER_TYPE_OM);
            openrequest.__set_BookingType(constants::BOOKING_TYPE_OM);
            openrequest.__set_ShipmentType(constants::PICKING_TYPE_OM);
            request.__set_OpenRequest(openrequest);

            // add positions
            csc::order::OrderPositionRequestList poslist;

            basar::db::aspect::AccessorPropertyTable_YIterator yitPos = tenderPos->get().begin();
            while (!yitPos.isEnd())
            {
                csc::order::OrderPositionRequest requestPos;

                basar::Int32 articleCode = yitPos.getInt32(properties::ARTICLE_NO); //TODO articleno or code to check in test
                basar::Int32 quantity = yitPos.getInt32(properties::CONTRACT_QTY);
                std::stringstream article;
                article << articleCode;

                requestPos.__set_PosType(csc::types::PositionTypeEnum::TypeProductStocked); //TODO do we need that? 
                requestPos.__set_ArticleCode(article.str());
                requestPos.__set_QuantityOrdered(quantity);
                requestPos.__set_AcceptInterBranchDelivery(true);
                requestPos.__set_AcceptPartialDelivery(true);
                requestPos.__set_AcceptRestDelivery(false);
                requestPos.__set_AcceptDispoDelivery(false);
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

#endif TENDER_KSC_SERVER

        libtender::domMod::cscorder::ICscOrderPositionPtr OMGOrderUC::createCscOrderPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitPos )
        {
            basar::Int32 articleCode    = yitPos.getInt32( properties::ARTICLE_NO         );
            basar::Int32 quantity       = yitPos.getInt32( properties::CONTRACT_QTY );
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

    }
}