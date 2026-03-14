#ifndef GUARD_CSCBATCHES_USECASE_TENDERCREATOR_H
#define GUARD_CSCBATCHES_USECASE_TENDERCREATOR_H

#include "itendercreator.h"
#include <domainmodule/tender/itenderptr.h>
#include <libabbauw/domainmodule/orderprotocol/iorderprotocolptr.h>
#include <libabbauw/itransferorderheadptr.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace appl
    {
        struct EventReturnStruct;
    }
}
    
namespace CSC_Batches
{
namespace useCase {

    //-------------------------------------------------------------------------------------------------//
    // class declaration
    //-------------------------------------------------------------------------------------------------//
    class TenderCreatorUC : public CSC_Batches::useCase::ITenderCreator
    {
        public:
            TenderCreatorUC();
            virtual ~TenderCreatorUC();

            void injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM );
            void injectPharmacyGroupCollectionDM( libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr pharmacyGroupCollectionDM );
			void injectOrderProtocol( libabbauw::domMod::orderProtocol::IOrderProtocolPtr orderProtocol );
			void injectTransferOrderHead( libabbauw::domMod::transferOrder::ITransferOrderHeadPtr transferOrderHead );

            virtual basar::appl::EventReturnType createTender( libabbauw::domMod::transferOrder::ITransferOrderPtr transferOrder );
            void setTenderEndDateOffset( basar::Int32 days );
			void setOMGDuration(basar::Int32 omgDuration);
			virtual bool hasErrors() const;
			virtual const ITenderCreator::ErrorList & getErrors() const;

        private:

            TenderCreatorUC( const TenderCreatorUC& );
            TenderCreatorUC& operator= ( const TenderCreatorUC& );

            basar::cmnutil::Date getTenderEndDate();
            libtender::domMod::tender::ITenderCollectionDMPtr getTenderCollectionDM();
            libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr getPharmacyGroupCollectionDM();
            basar::db::aspect::AccessorPropertyTable_YIterator getPharmacyGroup(basar::Int32 pharmacyNo);
            void createTenderPositions( libtender::domMod::tender::ITenderPtr, basar::db::aspect::AccessorPropertyTable_YIterator, basar::db::aspect::AccessorPropertyTable_YIterator );
            void createTenderHeader( libtender::domMod::tender::ITenderPtr, libabbauw::domMod::transferOrder::ITransferOrderPtr ,basar::db::aspect::AccessorPropertyTable_YIterator );
            bool checkActiveTenderAvailable( basar::Int32 articleNo, basar::VarString pharmGrpId );

        private:
            libtender::domMod::tender::ITenderCollectionDMPtr					m_TenderCollection;
            libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr		m_PharmacyGroupCollection;
	        libabbauw::domMod::orderProtocol::IOrderProtocolPtr                 m_OrderProtocol;
			libabbauw::domMod::transferOrder::ITransferOrderHeadPtr				m_TransferOrderHead;
            basar::Int32														m_TenderEndDateOffset;
			basar::Int32														m_OMGDuration;
			ErrorList															m_Errors;

            static const basar::Int16 TENDER_OFFSET_DAYS = 90;
            static const basar::Int16 ONE_DAY_BEFORE = -1;
            static const basar::Int16 TWO_DAYS_BEFORE = -2;
    };

}// namespace useCase
}// namespace CSC_Batches
#endif // GUARD_CSCBATCHES_USECASE_TENDERCREATOR_H
