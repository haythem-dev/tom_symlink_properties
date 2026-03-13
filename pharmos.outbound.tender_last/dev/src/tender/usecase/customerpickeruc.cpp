#include "customerpickeruc.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/customer/icustomercollectiondmptr.h"
#include "domainmodule/customer/icustomercollectiondm.h"
#include "iusecasegetter.h"

#include "customerpickervc.h"

#include <loggerpool/tenderloggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>

#include <libbasardbaspect_exceptions.h>


namespace tender
{
	namespace useCase
	{
		BEGIN_HANDLERS_REGISTRATION( CustomerPickerUC )
            SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerPickerUC, SearchCustomerRequested )
        END_HANDLERS_REGISTRATION()

		CustomerPickerUC::CustomerPickerUC() :
			m_Logger( libtender::LoggerPool::getLoggerUseCases() ),
			m_CheckMode(false)
		{
			METHODNAME_DEF( CustomerPickerUC, CustomerPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			registerEventHandlers( true );
		}

		CustomerPickerUC::~CustomerPickerUC()
		{
			METHODNAME_DEF( CustomerPickerUC, ~CustomerPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			registerEventHandlers( false );
		}

		void CustomerPickerUC::injectCustomerPickerVC( tender::viewConn::CustomerPickerVCPtr customerPickerVC )
		{
			METHODNAME_DEF( CustomerPickerUC, injectCustomerPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_CustomerPickerVC = customerPickerVC;
		}

		void CustomerPickerUC::injectCustomerCollectionDM ( libtender::domMod::customer::ICustomerCollectionDMPtr customerCollectionDMPtr )
		{
			METHODNAME_DEF( CustomerPickerUC, injectCustomerCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_Customer = customerCollectionDMPtr;
		}

		void CustomerPickerUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
		{
			METHODNAME_DEF( CustomerPickerUC, injectUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_UseCaseGetter = useCaseGetter;
		}

		tender::viewConn::CustomerPickerVCPtr CustomerPickerUC::getCustomerPickerVC()
		{
			METHODNAME_DEF( CustomerPickerUC, getCustomerPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			CHECK_INSTANCE_EXCEPTION( m_CustomerPickerVC.get() );
            return m_CustomerPickerVC;
		}

		libtender::domMod::customer::ICustomerCollectionDMPtr CustomerPickerUC::getCustomerCollectionDM()
		{
			METHODNAME_DEF( CustomerPickerUC, getCustomerCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			CHECK_INSTANCE_EXCEPTION( m_Customer.get() );
			return m_Customer;
		}

		tender::componentManager::IUseCaseGetterPtr CustomerPickerUC::getUseCaseGetter()
        {
            METHODNAME_DEF( CustomerPickerUC, getUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
            return m_UseCaseGetter;
        }

		const log4cplus::Logger& CustomerPickerUC::getLogger() const
        {
            METHODNAME_DEF( CustomerPickerUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

		void CustomerPickerUC::setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( CustomerPickerUC, setSearchCriteria )
            BLOG_TRACE_METHOD( m_Logger, fun );

			getCustomerPickerVC()->setCustomerNo( yit );
		}

		void CustomerPickerUC::setCheckMode( bool checkMode )
		{
			METHODNAME_DEF( CustomerPickerUC, setCheckMode )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_CheckMode = checkMode;
		}

		void CustomerPickerUC::init()
        {
            METHODNAME_DEF( CustomerPickerUC, init )
            BLOG_TRACE_METHOD( m_Logger, fun );
        }

        void CustomerPickerUC::reset()
        {
            METHODNAME_DEF( CustomerPickerUC, reset )
            BLOG_TRACE_METHOD( m_Logger, fun );

            init();
        }

		void CustomerPickerUC::run()
		{
			METHODNAME_DEF( CustomerPickerUC, run )
            BLOG_TRACE_METHOD( m_Logger, fun );

			init();
			
			if (true == m_CheckMode)
			{
				checkCustomer();
			}
			else
			{
			    searchCustomer();
				getCustomerPickerVC()->show();
			}
		}

		basar::db::aspect::AccessorPropertyTable_YIterator CustomerPickerUC::getPicked()
		{
			METHODNAME_DEF( CustomerPickerUC, getPickedCustomer )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if (true == m_CheckMode)
			{
				return getCustomerCollectionDM()->get().begin();
			}
			return getCustomerPickerVC()->getPickedCustomer();
		}

		basar::db::aspect::AccessorPropertyTable_YIterator CustomerPickerUC::getSearchYIterator()
		{
			METHODNAME_DEF( CustomerPickerUC, getSearchYIterator )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getCustomerCollectionDM()->getSearchYit();
		}

		basar::appl::EventReturnType CustomerPickerUC::searchCustomer()
		{
			METHODNAME_DEF( CustomerPickerUC, searchCustomer )
            BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

			try
			{
				getCustomerCollectionDM()->resetSearchYit();
				getCustomerPickerVC()->matchToSearchCriteria( getCustomerCollectionDM()->getSearchYit() );

				if( !getCustomerCollectionDM()->getSearchYit().arePropertiesSet( properties::CUSTOMER_NO ) &&
					!getCustomerCollectionDM()->getSearchYit().arePropertiesSet( properties::PHARMACY_NAME ) )
				{
					return result;
				}

				getCustomerCollectionDM()->findByPattern( getCustomerCollectionDM()->getSearchYit() );

				if( getCustomerCollectionDM()->isEmpty() && 
				  ( getCustomerCollectionDM()->getSearchYit().isContainedAndSet( properties::CUSTOMER_NO   ) ||
					getCustomerCollectionDM()->getSearchYit().isContainedAndSet( properties::PHARMACY_NAME ) ))
				{
					result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::CustomerPickerUC", "No customer found!").toLocal8Bit().data();

					getCustomerPickerVC()->displayNoCustomerFound();
				}

				getCustomerPickerVC()->matchFromCustomerCollection( getCustomerCollectionDM()->get() );

				return result;
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}
		}

		basar::appl::EventReturnType CustomerPickerUC::checkCustomer()
		{
			METHODNAME_DEF( CustomerPickerUC, checkCustomer )
            BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

			try
			{
				getCustomerCollectionDM()->findByPattern( getCustomerCollectionDM()->getSearchYit() );

				if( getCustomerCollectionDM()->isEmpty() &&
					getCustomerCollectionDM()->getSearchYit().isContainedAndSet( properties::CUSTOMER_NO ))
				{
					result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::CustomerPickerUC", "No customer found!").toLocal8Bit().data();
				}
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION( CustomerPickerUC, SearchCustomerRequested)
		{
			METHODNAME_DEF( CustomerPickerUC, SearchCustomerRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
			rSource;

			return searchCustomer();
		}

	} // end namespace useCase
} //end namespace tender
