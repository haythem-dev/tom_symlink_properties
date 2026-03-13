#ifndef GUARD_TENDER_USECASE_TENDERSTATE_H
#define GUARD_TENDER_USECASE_TENDERSTATE_H

#include "itenderstateuc.h"
#include <libbasarcmnutil.h>

namespace tender
{
namespace useCase 
{
	// TODO: refactoring!
    class TenderStateUC : public tender::useCase::ITenderStateUC
    {
        public:
            TenderStateUC();
            virtual ~TenderStateUC();

            void injectTenderCollectionDM(libtender::domMod::tender::ITenderCollectionDMPtr);

            virtual void setOMGDuration(const basar::Int32 days);
			virtual void setTenderEndDateOffset(const basar::Int32);

            virtual tender::useCase::StateValidationEnum validateTenderState( const basar::Int16 oldTenderState );
            virtual tender::useCase::StateValidationEnum validateTenderStateForDelete( const basar::Int32 articleNo, const basar::Int32 tenderNo,
                                                                               const basar::VarString & pharmGrpID, const basar::Date & endDate );
            virtual void setTender( libtender::domMod::tender::ITenderPtr ) ;
            virtual basar::VarString getValidationErrorMessage( const StateValidationEnum stateError );

			basar::cmnutil::Date getTenderEndDate(basar::cmnutil::Date = basar::cmnutil::Date::getCurrent());

        private:
            TenderStateUC(const TenderStateUC&);
            TenderStateUC& operator= (const TenderStateUC&);

            libtender::domMod::tender::ITenderCollectionDMPtr getTenderCollectionDM();
            
            void findActiveTender( const basar::Int32 articleNo, const basar::Int32 tenderNo );
            void findSuccessorTender( const basar::Int32 articleNo, const basar::Int32 tenderNo, const basar::VarString & pharmGrpID, const basar::Date & endDate );
            tender::useCase::StateValidationEnum validateStateForNewTender( const basar::Int16 currentState, const basar::Int32 articleNo );
            tender::useCase::StateValidationEnum validateStateForExistingTender( const basar::Int16 oldState, const basar::Int16 currentState, const basar::Int32 articleNo );
            void SaveSuccessorTender();
        
	private:
            const log4cplus::Logger								m_Logger;
            libtender::domMod::tender::ITenderCollectionDMPtr	m_TenderCollection;
            libtender::domMod::tender::ITenderPtr				m_Tender;
			basar::Int32										m_OMG_Duration;
            basar::Int32										m_TenderEndDateOffset;

            static const basar::Int16 TENDER_OFFSET_DAYS = 90;
            static const basar::Int16 ONE_DAY_BEFORE = -1;
            static const basar::Int16 TWO_DAYS_BEFORE = -2;
    };

}// namespace useCase
}// namespace tender
#endif // GUARD_TENDER_USECASE_TENDERSTATE_H
