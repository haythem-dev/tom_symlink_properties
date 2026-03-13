#ifndef GUARD_TENDER_USECASE_TENDERINVOICEUC_H
#define GUARD_TENDER_USECASE_TENDERINVOICEUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderinvoicesvcptr.h"
#include "iusecasegetterptr.h"

#include <domainmodule/tender/itendercollectiondmptr.h>
#include <domainmodule/tender/itenderprotocolptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverycollectiondmptr.h>

#include <domainmodule/cscorder/icscorderptr.h>
#include "domainmodule/cscorder/icscorderpositionptr.h"

#include <libtender/domainmodule/tender/itenderprogressptr.h>

//libs
#include <libutil/irunnable.h>
#include <libutil/session.h>
#include <libutil/dbtransaction.h>

#include <libbasarappl_eventmacros.h>
#include <libbasarguitie.h>

#include "infrastructure/cscclient/icscclientconnectionptr.h"
#include "infrastructure/cscclient/icscclientparameterproxyptr.h"

#ifdef WIN32
#pragma warning (push)
#pragma warning (disable : 4512) // assignment operator could not be generated
#pragma warning (disable : 4127) // conditional expression is constant
#pragma warning (disable : 4505) // unreferenced local function has been removed
#pragma warning (disable : 4706)
#endif
#include <gen-cpp/CscServiceBatch.h>
#include <gen-cpp/cscbase_constants.h>
#ifdef WIN32
#pragma warning (pop)
#endif


//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace useCase
    {
        //-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
        class TenderInvoiceUC : public libutil::useCase::IRunnable
        {
            public:
                TenderInvoiceUC();
                virtual ~TenderInvoiceUC();

                SYSTEM_EVENT_HANDLER_DECLARATION( CreateTenderInvoiceRequested	);

                void injectCscParamProxy(libtender::infrastructure::cscclient::ICscClientParameterProxyPtr paramProxy);
                void injectTenderInvoiceVC( tender::viewConn::TenderInvoicesVCPtr tenderInvoiceVC );
				void injectCscOrderDM( libtender::domMod::cscorder::ICscOrderPtr );
				void injectTenderProtocolDM( libtender::domMod::tender::ITenderProtocolPtr tenderProtocol );
				void injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory );
				void injectOrderType( basar::VarString orderType );
				void setTenderProgress( libtender::domMod::tender::ITenderProgressPtr tenderProgress );

                virtual void run();

            private:
                TenderInvoiceUC( const TenderInvoiceUC& );
                TenderInvoiceUC& operator= ( const TenderInvoiceUC& );

                void																	init();
                void																	reset();
																
                void																	registerEventHandlers( const bool forSave );

                csc::order::PlaceOrderResponse                                          createCscOrder();
				libtender::domMod::cscorder::ICscOrderPositionPtr						createCscOrderPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitPos );

				void																	copyProgressToProtocol();

                const log4cplus::Logger&												getLogger() const;										
                tender::viewConn::TenderInvoicesVCPtr									getTenderInvoiceVC();
				libtender::domMod::tender::ITenderProgressPtr							getTenderProgress();
				libtender::domMod::cscorder::ICscOrderPtr								getCscOrderDM();

                //////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////

                tender::viewConn::TenderInvoicesVCPtr									m_TenderInvoiceVC;
				libtender::domMod::tender::ITenderProgressPtr							m_TenderProgress;
				libtender::domMod::tender::ITenderProtocolPtr							m_TenderProtocol;
				libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr	m_TransactionFactory;
				libtender::domMod::cscorder::ICscOrderPtr								m_CscOrderDM;
	
				basar::VarString														m_OrderType;

                const log4cplus::Logger													m_Logger;

                libtender::infrastructure::cscclient::ICscClientParameterProxyPtr       m_ParamProxy;
        };
    } // namespace useCase
}	  // namespace tender

#endif //end GUARD_TENDER_USECASE_TENDERINVOICEUC_H
