#include "customerselectionuc.h"

#include "customerselectionvc.h"
#include "domainmodule/libtender_properties_definitions.h"

#include <loggerpool/tenderloggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>

#include <libbasardbaspect_exceptions.h>


namespace tender
{
	namespace useCase
    {
		CustomerSelectionUC::CustomerSelectionUC()
		: m_Logger(libtender::LoggerPool::getLoggerUseCases())
		{
			METHODNAME_DEF(CustomerSelectionUC, CustomerSelectionUC)
            BLOG_TRACE_METHOD(m_Logger, fun);

			//registerEventHandlers(true);
		}

		CustomerSelectionUC::~CustomerSelectionUC()
		{
			METHODNAME_DEF( CustomerSelectionUC, ~CustomerSelectionUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			//registerEventHandlers(false);
		}

		void CustomerSelectionUC::injectCustomerSelectionVC(tender::viewConn::CustomerSelectionVCPtr customerSelectionVC)
		{
			METHODNAME_DEF(CustomerSelectionUC, injectCustomerSelectionVC)
            BLOG_TRACE_METHOD(m_Logger, fun);

			m_CustomerSelectionVC = customerSelectionVC;
		}

        void CustomerSelectionUC::setCustomerList(basar::db::aspect::AccessorPropertyTableRef propTab)
		{
			METHODNAME_DEF( CustomerSelectionUC, setCustomerList )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_CustomerList = propTab;
		}

		tender::viewConn::CustomerSelectionVCPtr CustomerSelectionUC::getCustomerSelectionVC()
		{
			METHODNAME_DEF( CustomerSelectionUC, getCustomerSelectionVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			CHECK_INSTANCE_EXCEPTION( m_CustomerSelectionVC.get() );
            return m_CustomerSelectionVC;
		}

		const log4cplus::Logger& CustomerSelectionUC::getLogger() const
        {
            METHODNAME_DEF( CustomerSelectionUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

		void CustomerSelectionUC::init()
        {
            METHODNAME_DEF( CustomerSelectionUC, init )
            BLOG_TRACE_METHOD( m_Logger, fun );

            getCustomerSelectionVC()->matchFromCustomerCollection(m_CustomerList);
        }

		void CustomerSelectionUC::run()
		{
			METHODNAME_DEF(CustomerSelectionUC, run)
            BLOG_TRACE_METHOD(m_Logger, fun);

			init();
			getCustomerSelectionVC()->setSelectedCustomer(m_Picked);
		    getCustomerSelectionVC()->show();
		}

		basar::db::aspect::AccessorPropertyTable_YIterator CustomerSelectionUC::getPicked()
		{
			METHODNAME_DEF(CustomerSelectionUC, getPicked)
            BLOG_TRACE_METHOD(m_Logger, fun);

            return getCustomerSelectionVC()->getSelectedCustomer();
		}

		void CustomerSelectionUC::setPicked(basar::db::aspect::AccessorPropertyTable_YIterator picked)
		{
			METHODNAME_DEF(CustomerSelectionUC, setPicked)
            BLOG_TRACE_METHOD(m_Logger, fun);

			if(picked.arePropertiesSet(properties::CUSTOMER_NO) && 0 != picked.getInt32(properties::CUSTOMER_NO))
			{
				m_Picked = picked;
			}
		}

	} //end namespace useCase
} // end namespace tender
