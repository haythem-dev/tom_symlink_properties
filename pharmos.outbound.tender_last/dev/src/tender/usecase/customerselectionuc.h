#ifndef GUARD_TENDER_USECASE_CUSTOMERSELECTIONUC_H
#define GUARD_TENDER_USECASE_CUSTOMERSELECTIONUC_H

#include "customerselectionvcptr.h"
#include "tender/usecase/ipickeruc.h"

//Libs
#include <libutil/irunnable.h>
#include <libutil/session.h>

#include <libbasarappl_eventmacros.h>
#include <libbasarguitie.h>


namespace tender
{
	namespace useCase
	{
		class CustomerSelectionUC : public tender::useCase::IPickerUC
		{
			public:
				CustomerSelectionUC();
				virtual ~CustomerSelectionUC();

				void injectCustomerSelectionVC(tender::viewConn::CustomerSelectionVCPtr);
                void setCustomerList(basar::db::aspect::AccessorPropertyTableRef propTab);

				virtual void run();
				virtual basar::db::aspect::AccessorPropertyTable_YIterator getPicked();
				virtual void											   setPicked(basar::db::aspect::AccessorPropertyTable_YIterator);

			private:
				CustomerSelectionUC(const CustomerSelectionUC&);
				CustomerSelectionUC& operator= (const CustomerSelectionUC&);

				void init();
				void reset();
				void registerEventHandlers(const bool forSave);

				const log4cplus::Logger&                    getLogger() const;
				tender::viewConn::CustomerSelectionVCPtr    getCustomerSelectionVC();

				tender::viewConn::CustomerSelectionVCPtr    m_CustomerSelectionVC;

				const log4cplus::Logger	                    m_Logger;

                basar::db::aspect::AccessorPropertyTableRef		   m_CustomerList;
				basar::db::aspect::AccessorPropertyTable_YIterator m_Picked;
		};
	}
} 

#endif
