//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cscorderuc.h"
#include "cscordermaintainvc.h"
#include <libtender/domainmodule/constants.h>
#include "domainmodule/libtender_properties_definitions.h"

#include <domainmodule/cscorder/icscorder.h>
#include "domainmodule/cscorder/icscorderposition.h"
#include <libtender/domainmodule/cscordercustomer/icscordercustomercollectiondm.h>
#include "infrastructure/cscclient/icscclientparameterproxy.h"
#include <loggerpool/tenderloggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>

#include <libbasardbaspect_exceptions.h>
#include <libbasarlogin.h>


#include <sstream>
#include <iostream>

#ifdef WIN32
#pragma warning (push)
#pragma warning (disable : 4100) 
#endif

//-------------------------------------------------------------------------------------------------//
// namespace sections
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace useCase
    {
        //BEGIN_HANDLERS_REGISTRATION( CSCOrderUC )
        //END_HANDLERS_REGISTRATION()

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // CSCOrderUC::CSCOrderUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //CSCOrderUC::CSCOrderUC()
        CSCOrderUC::CSCOrderUC( libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr cscOrderCustomerCollection)
        : m_Logger( libtender::LoggerPool::getLoggerUseCases() )
        {
            METHODNAME_DEF( CSCOrderUC, CSCOrderUC )
            BLOG_TRACE_METHOD( getLogger(), fun );

            m_CscOrderCustomerCollection = cscOrderCustomerCollection;
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // CSCOrderUC::~CSCOrderUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CSCOrderUC::~CSCOrderUC()
        {
            METHODNAME_DEF( CSCOrderUC, ~CSCOrderUC )
            BLOG_TRACE_METHOD( getLogger(), fun );
        }

        void CSCOrderUC::init()
        {
        
        }

        void CSCOrderUC::reset()
        {
        
        }


        const log4cplus::Logger& CSCOrderUC::getLogger() const
        {
            METHODNAME_DEF( CSCOrderUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );
            return m_Logger;
        }

        void CSCOrderUC::injectCSCOrderMaintainVC( tender::viewConn::ICSCOrderMaintainVCPtr cscOrderMaintainVC )
        {
            METHODNAME_DEF( CSCOrderMaintainUC, injectCSCOrderMaintainVC )
            BLOG_TRACE_METHOD( m_Logger, fun );
    
            m_CSCOrderMaintainVC = cscOrderMaintainVC;
        }

         void CSCOrderUC::injectParameterProxy(libtender::infrastructure::cscclient::ICscClientParameterProxyPtr paramProxy)
         {
             m_ParamProxy = paramProxy;
         }

        void CSCOrderUC::injectCscOrderDM( libtender::domMod::cscorder::ICscOrderPtr cscOrderDM )
        {
            METHODNAME_DEF( CSCOrderUC, injectCscOrderDM )
            BLOG_TRACE_METHOD( getLogger(), fun );

            m_CscOrderDM = cscOrderDM;
        }

        void CSCOrderUC::setArea( basar::Int16 area )
        {
            METHODNAME_DEF( CSCOrderUC, setArea )
            BLOG_TRACE_METHOD( getLogger(), fun );

            m_Area = area;
        }

        libtender::domMod::cscorder::ICscOrderPtr CSCOrderUC::getCscOrderDM()
        {
            METHODNAME_DEF( CSCOrderUC, getCscOrderDM )
            BLOG_TRACE_METHOD( getLogger(), fun );

            CHECK_INSTANCE_EXCEPTION( m_CscOrderDM.get() );
            return m_CscOrderDM;
        }

        tender::viewConn::ICSCOrderMaintainVCPtr CSCOrderUC::getCSCOrderMaintainVC()
        {
            METHODNAME_DEF( CSCOrderUC, getCSCOrderMaintainVC )
            BLOG_TRACE_METHOD( getLogger(), fun );

            return m_CSCOrderMaintainVC;
        }
        
        void CSCOrderUC::run()
        {
            std::stringstream strCSCOrderCreated;
            std::stringstream strCSCOrderFailed;
            basar::Int32 nCSCOrderCreated = 0;
            basar::Int32 nCSCOrderfailed = 0;

            basar::db::aspect::AccessorPropertyTable_YIterator cscOrderCustCollYit = m_CscOrderCustomerCollection->get().begin(basar::SS_INSERT|basar::SS_UPDATE);
            while(!cscOrderCustCollYit.isEnd())
            {
                if(cscOrderCustCollYit.getInt32(properties::QUANTITY) > 0)
                {
                    int result = createCscOrder(cscOrderCustCollYit,strCSCOrderFailed,strCSCOrderCreated);
                    if( result == 0 )
                    {
                        ++nCSCOrderCreated;
                    }
                    else
                    {
                        ++nCSCOrderfailed;
                    }
                }
                ++cscOrderCustCollYit;
            }

            if( (nCSCOrderCreated + nCSCOrderfailed) == 0) 
                return;
            
            std::stringstream ss;
            if(nCSCOrderfailed == 0)
            {
                ss << "All orders are created successfully. \n Order Created : " << nCSCOrderCreated << "\n" << strCSCOrderCreated.str();
            }
            else
            {
                ss << "Order Created : " << nCSCOrderCreated <<"\n" + strCSCOrderCreated.str() + "\nOrder Failed : " << nCSCOrderfailed <<"\n" << strCSCOrderFailed.str();;
            }

            m_CSCOrderMaintainVC->displayCscOrderCreationDetails(ss.str());
        }
#ifdef TENDER_KSC_SERVER
		int CSCOrderUC::createCscOrder( basar::db::aspect::AccessorPropertyTable_YIterator cscOrderCustCollYit, 
										std::stringstream & strCSCOrderFailed, 
										std::stringstream & strCSCOrderCreated )
		{
			basar::Int32 cscRetVal;
			basar::Int16 branchNo = cscOrderCustCollYit.getInt16( properties::BRANCH_NO );
			basar::Int32 customerNo = cscOrderCustCollYit.getInt32( properties::CUSTOMER_NO );
			cscRetVal = m_CscOrderDM->openOrderReferenceVzType( branchNo, customerNo, m_ParamProxy->getCscOrderType(), 
			m_ParamProxy->getCscOrderPickingType(), m_ParamProxy->getCscOrderBookingType() );
			if( 0 != cscRetVal )
			{
				std::stringstream ss;
				ss << "openOrder rc=" << cscRetVal << ", " << getCscOrderDM()->getLackReason() << " " << getCscOrderDM()->getText();
				BLOG_ERROR( m_Logger, ss.str() );
		
				strCSCOrderFailed << "Customer No = " << customerNo << " : " << getCscOrderDM()->getText() << "\n";
				m_CscOrderDM->cancelOrder();
				return -1;
			}
		
			cscRetVal = getCscOrderDM()->setDeliveryDate( basar::cmnutil::Date::getCurrent() );
		
			if( m_Area != constants::REGION_CH )
			{
				cscRetVal = getCscOrderDM()->setTenderNo( cscOrderCustCollYit.getInt32( properties::TENDER_ID ) );
			}
		
			libtender::domMod::cscorder::ICscOrderPositionPtr cscPos = createCscOrderPosition( cscOrderCustCollYit );
		
			strCSCOrderCreated << "Customer No = "<< customerNo <<" : Order No = " << getCscOrderDM()->getOrderNo() << "\n";
			getCscOrderDM()->closeOrder();
			return 0;
		}
#else //CSC Service
        int CSCOrderUC::createCscOrder(basar::db::aspect::AccessorPropertyTable_YIterator cscOrderCustCollYit,
            std::stringstream& strCSCOrderFailed,
            std::stringstream& strCSCOrderCreated)
        {
            METHODNAME_DEF(MainUC, createCscOrder)
            BLOG_TRACE_METHOD(m_Logger, fun);

            basar::Int32 customerNo = cscOrderCustCollYit.getInt32(properties::CUSTOMER_NO);
            basar::Int16 branchNo = cscOrderCustCollYit.getInt16(properties::BRANCH_NO);
            basar::VarString orderType = m_ParamProxy->getCscOrderType();
            basar::Int32 orderNo = 0;

            m_CscOrderDM->createCscOrder(cscOrderCustCollYit, customerNo, branchNo, orderType, orderNo);

            strCSCOrderCreated << "Customer No = " << customerNo << " : Order No = " << orderNo << "\n";
            
            return 0;
        }

#endif //TENDER_KSC_SERVER

        libtender::domMod::cscorder::ICscOrderPositionPtr CSCOrderUC::createCscOrderPosition(basar::db::aspect::AccessorPropertyTable_YIterator cscOrderCustCollYit)
        {
            libtender::domMod::cscorder::ICscOrderPositionPtr cscPos;
            basar::VarString articleCode    = cscOrderCustCollYit.getString(properties::ARTICLE_CODE);
            basar::Int32 quantity           = cscOrderCustCollYit.getInt32(properties::QUANTITY);
            cscPos = getCscOrderDM()->createEmptyPosition(articleCode, quantity);
            cscPos->setIbtPossible(true);
            cscPos->setRestDeliveryPossible(false);
            cscPos->setDispoPossible(false);
            cscPos->setPartialDeliveryPossible(true);
            cscPos->save();
            return cscPos;
        }

    }
}
#ifdef WIN32
#pragma warning (pop)
#endif