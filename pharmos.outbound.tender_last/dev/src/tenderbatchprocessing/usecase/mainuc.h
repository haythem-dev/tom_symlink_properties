#ifndef GUARD_TENDERBATCHPROCESSING_USECASE_MAINUC_H
#define GUARD_TENDERBATCHPROCESSING_USECASE_MAINUC_H

#include "icmdlineparametergetterptr.h"

#include <infrastructure/mail/imailalertptr.h>
#include <domainmodule/tender/itendercollectiondmptr.h>

#include <libutil/irunnableptr.h>
#include <libutil/irunnable.h>

#include <libbasardbaspect_accessorpropertytable_yiterator.h>

namespace log4cplus 
{
    class Logger;
}

namespace tenderbatchprocessing {
namespace useCase {


class MainUC : public libutil::useCase::IRunnable
{
	public:
		MainUC();
		virtual ~MainUC();
	
		void										injectCmdLineParameterGetter( infrastructure::ICmdLineParameterGetterPtr cmdLineParameterGetter );
		void										injectMailAlert( infrastructure::mail::IMailAlertPtr mailAlert );
	    void                                        injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM );
		void                                        injectSuccessorTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM );
	
		void										setOMGDuration(basar::Int32 duration);
		void										setTenderEndDateOffset(basar::Int32 offset);

		virtual void								run();
	
	private:
		MainUC(const MainUC&);
		MainUC& operator= (const MainUC&);

		infrastructure::ICmdLineParameterGetterPtr          getCmdLineParameterGetter();
		infrastructure::mail::IMailAlertPtr                 getMailAlert();
		basar::Date                                         determineStartDate();
		basar::Date                                         determineEndDate();
		libtender::domMod::tender::ITenderCollectionDMPtr   getTenderCollectionDM();
		libtender::domMod::tender::ITenderCollectionDMPtr   getSuccessorTenderCollectionDM();

		basar::Date											calculateTenderEndDate();

		const log4cplus::Logger								m_Logger;
		libtender::domMod::tender::ITenderCollectionDMPtr   m_TenderCollection;
		libtender::domMod::tender::ITenderCollectionDMPtr   m_SuccessorTenderCollection;
		infrastructure::ICmdLineParameterGetterPtr			m_CmdLineParameterGetter;
		infrastructure::mail::IMailAlertPtr					m_MailAlert;

		basar::Int32										m_OMGDuration;
		basar::Int32										m_TenderEndDateOffset;
};

} // namespace useCase
} // namespace tenderbatchprocessing

#endif //end GUARD_TENDERBATCHPROCESSING_USECASE_MAINUC_H
