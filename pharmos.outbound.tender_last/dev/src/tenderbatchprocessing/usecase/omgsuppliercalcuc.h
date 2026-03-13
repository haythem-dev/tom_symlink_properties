#ifndef GUARD_TENDERBATCHPROCESSING_USECASE_OMGSUPPLIERCALCUC_H
#define GUARD_TENDERBATCHPROCESSING_USECASE_OMGSUPPLIERCALCUC_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------/
#include <domainmodule/tender/itenderprogressptr.h>
#include <domainmodule/tender/itendercollectiondmptr.h>
#include "domainmodule/omgsuppliercalc/iomgsuppliercalcptr.h"
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
class OMGSupplierCalcUC : public tenderbatchprocessing::useCase::IOMGCalcBaseUC
{
    public:

            OMGSupplierCalcUC();
            virtual ~OMGSupplierCalcUC();

            void injectOMGSupplierCalcDM( tenderbatchprocessing::domMod::omgsuppliercalc::IOMGSupplierCalcPtr omgSupplierCalcDM );
            void injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr tenderProgressDM );
            void injectTenderCollectionDM(libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDMPtr );

            virtual void setTender( libtender::domMod::tender::ITenderPtr );
            virtual void run();

            basar::Decimal calculateOriginalPricePerPiece( basar::db::aspect::AccessorPropertyTable_YIterator yitTenProgCurrent );
            basar::Int32 getContractQuantity( basar::Int32 articleNo);


    private:
            OMGSupplierCalcUC( const OMGSupplierCalcUC& );
            OMGSupplierCalcUC& operator= ( const OMGSupplierCalcUC& );

            basar::db::aspect::AccessorPropertyTable_YIterator getEndOfArticleGroup( basar::db::aspect::AccessorPropertyTable_YIterator );
            void processArticle( basar::db::aspect::AccessorPropertyTable_YIterator yitStart, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd );
            void saveCalculation();
            void validateTenderDiscount( basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleStart, basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProgArticleEnd );
            void checkDeliveredQuantity( basar::db::aspect::AccessorPropertyTable_YIterator yitStart, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd );
            void createSupplierCalcPosition(basar::db::aspect::AccessorPropertyTable_YIterator yitTenderProg, basar::Decimal discountDiff, bool transFlag = false);
            const log4cplus::Logger    getLogger() const;

            tenderbatchprocessing::domMod::omgsuppliercalc::IOMGSupplierCalcPtr m_OMGSupplierCalcDM;
            libtender::domMod::tender::ITenderProgressPtr       m_TenderProgressDM;
            libtender::domMod::tender::ITenderCollectionDMPtr   m_TenderCollectionDM;
            libtender::domMod::tender::ITenderPtr               m_Tender;
            basar::Decimal                                      m_TenderDiscPct;
            const log4cplus::Logger                             m_Logger;
        };
    }

}//end tenderbatchprocessing
#endif