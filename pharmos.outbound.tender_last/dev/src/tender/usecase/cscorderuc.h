#ifndef GUARD_TENDER_USECASE_CSCORDERUC_H
#define GUARD_TENDER_USECASE_CSCORDERUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "icscordermaintainvcptr.h"
#include "infrastructure/cscclient/icscclientparameterproxyptr.h"
#include <domainmodule/cscorder/icscorderptr.h>
#include "domainmodule/cscorder/icscorderpositionptr.h"
#include <domainmodule/cscordercustomer/icscordercustomercollectiondmptr.h>
// libs
#include <libutil/irunnable.h>
#include <libutil/session.h>
#include <libutil/dbtransaction.h>

#include <libbasarappl_eventmacros.h>
#include <libbasarcmnutil.h>
#include <libbasar_definitions.h>
#include <libbasardbaspect.h>

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
        class CSCOrderUC : public libutil::useCase::IRunnable
        {
            public:
                CSCOrderUC( libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr );
                virtual ~CSCOrderUC();
                virtual void run();
                void injectParameterProxy(libtender::infrastructure::cscclient::ICscClientParameterProxyPtr paramProxy);
                void injectCscOrderDM( libtender::domMod::cscorder::ICscOrderPtr cscOrderDM );
                void injectCSCOrderMaintainVC( tender::viewConn::ICSCOrderMaintainVCPtr cscOrderMaintainVC );
                void setArea( basar::Int16 area );
            private:
                CSCOrderUC( const CSCOrderUC& );
                CSCOrderUC& operator= ( const CSCOrderUC& );

                int                                                                     createCscOrder(basar::db::aspect::AccessorPropertyTable_YIterator, std::stringstream&, std::stringstream& );
                libtender::domMod::cscorder::ICscOrderPositionPtr                       createCscOrderPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitPos );

                void                                                                    init();
                void                                                                    reset();
               
                const log4cplus::Logger&                                                getLogger() const;
                libtender::domMod::cscorder::ICscOrderPtr                               getCscOrderDM();
                tender::viewConn::ICSCOrderMaintainVCPtr                                getCSCOrderMaintainVC();
                //////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////
                libtender::infrastructure::cscclient::ICscClientParameterProxyPtr       m_ParamProxy;
                libtender::domMod::cscorder::ICscOrderPtr                               m_CscOrderDM;
                libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr   m_CscOrderCustomerCollection;
                tender::viewConn::ICSCOrderMaintainVCPtr                                m_CSCOrderMaintainVC;
                basar::Int16                                                            m_Area;
                const log4cplus::Logger                                                 m_Logger;
        };
    } // namespace useCase
} // namespace tender

#endif //end GUARD_TENDER_USECASE_CSCORDERUC_H
