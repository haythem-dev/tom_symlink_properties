#ifndef GUARD_TENDER_USECASE_I_TENDERSTATE_H
#define GUARD_TENDER_USECASE_I_TENDERSTATE_H

#include <libbasarappl_systemeventmanager.h>
#include "domainmodule/tender/itendercollectiondmptr.h"
#include "domainmodule/tender/itenderptr.h"
#include "statevalidationenum.h"


namespace tender
{
namespace useCase
{
class ITenderStateUC
{
public:
    virtual ~ITenderStateUC() {}
    virtual void setTender( libtender::domMod::tender::ITenderPtr ) = 0;
    virtual void setTenderEndDateOffset( const basar::Int32 days) = 0;
	virtual void setOMGDuration(const basar::Int32) = 0;
    virtual tender::useCase::StateValidationEnum validateTenderState( const basar::Int16 oldTenderState ) = 0;
    virtual tender::useCase::StateValidationEnum validateTenderStateForDelete( const basar::Int32 articleNo, const basar::Int32 tenderNo, 
                                                                       const basar::VarString & pharmGrpID, const basar::Date & endDate ) = 0;
    virtual basar::VarString getValidationErrorMessage( const StateValidationEnum stateError) = 0;

	virtual basar::cmnutil::Date getTenderEndDate(basar::cmnutil::Date dateFrom) = 0;
};

} // namespace useCase
} // namespace tender

#endif // GUARD_TENDER_USECASE_I_TENDERSTATE_H
