#ifndef GUARD_TENDERBATCHPROCESSING_USECASE_OMGCUSTOMERCALCUC_H
#define GUARD_TENDERBATCHPROCESSING_USECASE_OMGCUSTOMERCALCUC_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------/
#include <domainmodule/tender/itenderprogressptr.h>
#include <domainmodule/tender/itendercollectiondmptr.h>
#include "domainmodule/omgcustomercalc/iomgcustomercalcptr.h"
#include <domainmodule/tender/itenderptr.h>
#include "iomgcalcbaseuc.h"
// libs
#include <libutil/irunnable.h>
#include <libutil/session.h>
#include <libutil/dbtransaction.h>

#include <libbasarappl_eventmacros.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
        }
    }
}
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
class OMGCustomerCalcUC : public tenderbatchprocessing::useCase::IOMGCalcBaseUC
{
    public:

            OMGCustomerCalcUC();
            virtual ~OMGCustomerCalcUC();

            void injectOMGCustomerCalcDM( tenderbatchprocessing::domMod::omgcustomercalc::IOMGCustomerCalcPtr omgCalcDM );
            void injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr tenderProgressDM );
            void injectTenderCollectionDM(libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDMPtr );

            virtual void setTender( libtender::domMod::tender::ITenderPtr );
            virtual void run();

            basar::Int32 getContractQuantity( basar::Int32 articleNo);

    private:
            OMGCustomerCalcUC( const OMGCustomerCalcUC& );
            OMGCustomerCalcUC& operator= ( const OMGCustomerCalcUC& );

            basar::db::aspect::AccessorPropertyTable_YIterator getEndOfArticleGroup( basar::db::aspect::AccessorPropertyTable_YIterator );
            void processArticle( basar::db::aspect::AccessorPropertyTable_YIterator yitStart, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd );
            void checkDeliveredQuantity( basar::db::aspect::AccessorPropertyTable_YIterator yitStart, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd );
            void saveCalculation();
            void createPosition(basar::Int32 tenderNo, basar::Int32 customerNo, basar::Int32 articleNO, basar::Decimal priceDiff, bool transFlag = false);
            void validateTenderDiscount( basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleStart, basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleEnd );
            const log4cplus::Logger&    getLogger() const;

            tenderbatchprocessing::domMod::omgcustomercalc::IOMGCustomerCalcPtr m_OMGCustomerCalcDM;
            libtender::domMod::tender::ITenderProgressPtr       m_TenderProgressDM;
            libtender::domMod::tender::ITenderCollectionDMPtr   m_TenderCollectionDM;
            libtender::domMod::tender::ITenderPtr               m_Tender;
            basar::Decimal                                      m_TenderDiscPct;
            basar::Date                                         m_TenderEndDate;
            const log4cplus::Logger                             m_Logger;
        };
    }

}//end tenderbatchprocessing
#endif