#ifndef GUARD_TENDER_USECASE_OMGORDERUC_H
#define GUARD_TENDER_USECASE_OMGORDERUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <domainmodule/cscorder/icscorderptr.h>
#include "domainmodule/cscorder/icscorderpositionptr.h"
#include <libtender/domainmodule/tender/itenderptr.h>
//#include <domainmodule/tender/itenderpositioncollectionptr.h>

// libs
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
// forward declaration section
//-------------------------------------------------------------------------------------------------//

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
        class OMGOrderUC : public libutil::useCase::IRunnable
        {
            public:
                OMGOrderUC( libtender::domMod::tender::ITenderPtr );
                virtual ~OMGOrderUC();
                virtual void run();
                void injectCscParamProxy(libtender::infrastructure::cscclient::ICscClientParameterProxyPtr paramProxy);
                void injectCscOrderDM( libtender::domMod::cscorder::ICscOrderPtr cscOrderDM );
                void setTender( libtender::domMod::tender::ITenderPtr tender );
                libtender::domMod::tender::ITenderPtr getTender();

            private:
                OMGOrderUC( const OMGOrderUC& );
                OMGOrderUC& operator= ( const OMGOrderUC& );

                csc::order::PlaceOrderResponse                                          createCscOrder();
                libtender::domMod::cscorder::ICscOrderPositionPtr                       createCscOrderPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitPos );

                void                                                                    init();
                void                                                                    reset();
                //void                                                                    registerEventHandlers( const bool eh );
                const log4cplus::Logger&                                                getLogger() const;
                libtender::domMod::cscorder::ICscOrderPtr                               getCscOrderDM();
                //////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////
                libtender::domMod::tender::ITenderPtr                                   m_Tender;
                libtender::domMod::cscorder::ICscOrderPtr                               m_CscOrderDM;
                //tender::componentManager::IUseCaseGetterPtr                             m_UseCaseGetter;
                const log4cplus::Logger                                                 m_Logger;
                libtender::infrastructure::cscclient::ICscClientParameterProxyPtr       m_ParamProxy;
        };
    } // namespace useCase
} // namespace tender

#endif //end GUARD_TENDER_USECASE_OMGORDERUC_H
