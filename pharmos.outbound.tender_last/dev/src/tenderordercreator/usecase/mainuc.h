//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author
 *  \date
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_TENDERORDERCREATOR_USECASE_MAINUC_H
#define GUARD_TENDERORDERCREATOR_USECASE_MAINUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <domainmodule/scheduleddelivery/ischeduleddeliverycollectiondmptr.h>
#include <infrastructure/mail/imailalertptr.h>

#include <componentmanager/tenderordercreatorapplcompmgrptr.h>
#include "icmdlineparametergetterptr.h"
#include "domainmodule/cscorder/icscorderptr.h"
#include "domainmodule/cscorder/icscorderpositionptr.h"
#include <domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>

#include <libutil/irunnable.h>
#include <libbasarappl_eventmacros.h>
#include <libbasarcmnutil_bstring.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace log4cplus {
    class Logger;
}

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace tenderordercreator {
namespace useCase {

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
class MainUC : public libutil::useCase::IRunnable
{
public:
	MainUC();
	virtual ~MainUC();

	void										injectCmdLineParameterGetter( infrastructure::ICmdLineParameterGetterPtr cmdLineParameterGetter );
	void										injectMailAlert( infrastructure::mail::IMailAlertPtr mailAlert );
	void										injectScheduledDeliveryCollectionDM( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollectionDM );
	void										injectCscOrderDM( libtender::domMod::cscorder::ICscOrderPtr cscOrderDM );
	void										injectOrderType( basar::VarString orderType );

	virtual void								run();

private:
	MainUC(const MainUC&);
	MainUC& operator= (const MainUC&);

	void												createCscOrder(const libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr  scheduledDelivery);
	libtender::domMod::cscorder::ICscOrderPositionPtr	createCscOrderPosition(basar::db::aspect::AccessorPropertyTable_YIterator yitPos);
	
	void										prepareErrorMail(const libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr, const basar::cmnutil::CollBString& errorList);	
	void										sendErrorMail(const libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery, const basar::cmnutil::CollBString& errorList);
	void										sendErrorMail(const basar::cmnutil::CollBString& errorList);

	void										registerEventHandlers(const bool forSave);
	infrastructure::ICmdLineParameterGetterPtr	getCmdLineParameterGetter();
	infrastructure::mail::IMailAlertPtr			getMailAlert();
	basar::Date									determineStartDate();
	basar::Date									determineEndDate();

	libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr	getScheduledDeliveryCollectionDM();
	libtender::domMod::cscorder::ICscOrderPtr								getCscOrderDM();

    const log4cplus::Logger&                                    getLogger() const;


	//////////////////////////////////////////////////////////////////////////////////////
	// member variables
	//////////////////////////////////////////////////////////////////////////////////////
	const log4cplus::Logger&												m_Logger;
	libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr	m_ScheduledDeliveryCollection;
	libtender::domMod::cscorder::ICscOrderPtr								m_CscOrderDM;
	infrastructure::ICmdLineParameterGetterPtr								m_CmdLineParameterGetter;
	infrastructure::mail::IMailAlertPtr										m_MailAlert;
	basar::VarString														m_OrderType;

	basar::cmnutil::CollBString												m_OrderErrorList;

};

} // namespace useCase
} // namespace tenderordercreator

#endif //end GUARD_TENDERORDERCREATOR_USECASE_MAINUC_H
