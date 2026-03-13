#ifndef GUARD_TENDERBATCHPROCESSING_USECASE_OMGCALCUC_H
#define GUARD_TENDERBATCHPROCESSING_USECASE_OMGCALCUC_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------/
#include <domainmodule/tender/itenderprogressptr.h>
#include <domainmodule/tender/itendercollectiondmptr.h>
#include "domainmodule/omgcalc/iomgcalcptr.h"
// libs
#include <libutil/irunnable.h>
#include <libutil/session.h>
#include <libutil/dbtransaction.h>

#include <libbasarappl_eventmacros.h>
#include <domainmodule/tender/itenderptr.h> //sk
//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace tenderbatchprocessing
{
namespace useCase
{
//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
class OMGCalcUC : public libutil::useCase::IRunnable
{
    public:
			// TODO SK: rename to OMGCustomerCalcUC
			// TODO SK: copy to OMGSupplierCalcUC

            OMGCalcUC();
            virtual ~OMGCalcUC();

            void injectOMGCalcDM( tenderbatchprocessing::domMod::omgcalc::IOMGCalcPtr omgCalcDM );
            void injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr tenderProgressDM );
            void injectTenderCollectionDM(libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDMPtr );

			// TODO SK: implement
			void setTender( libtender::domMod::tender::ITenderPtr );

            virtual void run();

            //SYSTEM_EVENT_HANDLER_DECLARATION( InsertOMGCalcRequested);

    private:
            OMGCalcUC( const OMGCalcUC& );
            OMGCalcUC& operator= ( const OMGCalcUC& );

            //void registerEventHandlers( const bool forSave );
            const log4cplus::Logger    getLogger() const;

            tenderbatchprocessing::domMod::omgcalc::IOMGCalcPtr m_OMGCalcDM;
            libtender::domMod::tender::ITenderProgressPtr       m_TenderProgressDM;
            libtender::domMod::tender::ITenderCollectionDMPtr   m_TenderCollectionDM;

            const log4cplus::Logger                             m_Logger;
        };
    }

}//end tenderbatchprocessing
#endif