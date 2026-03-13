#ifndef GUARD_TENDER_USECASE_TENDERPROTOCOLUC_H
#define GUARD_TENDER_USECASE_TENDERPROTOCOLUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderprotocolvcptr.h"
#include "iusecasegetterptr.h"

#include <domainmodule/tender/itenderptr.h>
#include <domainmodule/tender/itenderprotocolptr.h>
#include <domainmodule/tender/itenderprogressptr.h>


// libs
#include <libutil/irunnable.h>
#include <libutil/session.h>
#include <libutil/dbtransaction.h>

#include <libbasarappl_eventmacros.h>
#include <libbasarguitie.h>


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
        class TenderProtocolUC : public libutil::useCase::IRunnable
        {
            public:
                TenderProtocolUC();
                virtual ~TenderProtocolUC();

                SYSTEM_EVENT_HANDLER_DECLARATION( SearchTenderProtocols				);
				SYSTEM_EVENT_HANDLER_DECLARATION( ChangeQuantityToChargeRequested	);
				SYSTEM_EVENT_HANDLER_DECLARATION( CreateInvoiceRequested	        );
				SYSTEM_EVENT_HANDLER_DECLARATION( SelectOrderRequested				);
				

                void injectTenderProtocolVC				( tender::viewConn::TenderProtocolVCPtr tenderProtocolVC );
				void injectTenderProgressDM				( libtender::domMod::tender::ITenderProgressPtr tenderProgress );
				void injectUseCaseGetter				( tender::componentManager::IUseCaseGetterPtr useCaseGetter );
				void setTender							( libtender::domMod::tender::ITenderPtr tender );
				void setArea							( basar::Int16 area );

                virtual void run();

            private:
                TenderProtocolUC( const TenderProtocolUC& );
                TenderProtocolUC& operator= ( const TenderProtocolUC& );

                void																	init();
                void																	reset();
																
                void																	registerEventHandlers( const bool forSave );
																
                const log4cplus::Logger&												getLogger() const;
																
                tender::viewConn::TenderProtocolVCPtr									getTenderProtocolVC();
				libtender::domMod::tender::ITenderProgressPtr							getTenderProgressDM();
				tender::componentManager::IUseCaseGetterPtr								getUseCaseGetter();


                //////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////

                tender::viewConn::TenderProtocolVCPtr									m_TenderProtocolVC;
				libtender::domMod::tender::ITenderProgressPtr							m_TenderProgress;
				libtender::domMod::tender::ITenderPtr									m_Tender;
				basar::Int16															m_Area;
				tender::componentManager::IUseCaseGetterPtr								m_UseCaseGetter;

                const log4cplus::Logger													m_Logger;
        };
    } // namespace useCase
} // namespace tender

#endif //end GUARD_TENDER_USECASE_TENDERPROTOCOLUC_H
