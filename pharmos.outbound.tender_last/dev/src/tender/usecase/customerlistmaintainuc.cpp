//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "customerlistmaintainuc.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/customerlist/icustomerlistdmptr.h"
#include "domainmodule/customerlist/icustomerlistdm.h"
#include "iusecasegetter.h"

#include "customerlistmaintainvc.h"
#include "customerlistmaintainvcptr.h"
#include "ipickeruc.h"

#include <loggerpool/tenderloggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>

#include <libbasardbaspect_exceptions.h>

//-------------------------------------------------------------------------------------------------//
// namespace sections
//-------------------------------------------------------------------------------------------------//
namespace tender
{
	namespace useCase
	{
		BEGIN_HANDLERS_REGISTRATION( CustomerListMaintainUC )
			SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerListMaintainUC, AddCustomerRequested    )
			SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerListMaintainUC, EditCustomerRequested   )
			SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerListMaintainUC, DeleteCustomerRequested )
            SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerListMaintainUC, SearchCustomerRequested )
			SYSTEM_EVENT_HANDLER_REGISTRATION( CustomerListMaintainUC, CheckCustomerRequested  )
        END_HANDLERS_REGISTRATION()

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // CustomerListMaintainUC::CustomerListMaintainUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CustomerListMaintainUC::CustomerListMaintainUC() :
			m_Logger( libtender::LoggerPool::getLoggerUseCases() )
		{
			METHODNAME_DEF( CustomerListMaintainUC, CustomerListMaintainUC )
            BLOG_TRACE_METHOD( getLogger(), fun );

			registerEventHandlers( true );
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // CustomerListMaintainUC::~CustomerListMaintainUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CustomerListMaintainUC::~CustomerListMaintainUC()
		{
			METHODNAME_DEF( CustomerListMaintainUC, ~CustomerListMaintainUC )
            BLOG_TRACE_METHOD( getLogger(), fun );

			registerEventHandlers( false );
		}

		void CustomerListMaintainUC::injectCustomerListMaintainVC( tender::viewConn::CustomerListMaintainVCPtr customerListMaintainVC )
		{
			METHODNAME_DEF( CustomerPickerUC, injectCustomerPickerVC )
            BLOG_TRACE_METHOD( getLogger(), fun );

			m_CustomerListMaintainVC = customerListMaintainVC;
		}

		void CustomerListMaintainUC::injectCustomerListDM( libtender::domMod::customerlist::ICustomerListDMPtr customerListDM )
		{
			METHODNAME_DEF( CustomerListMaintainUC, injectCustomerListDM )
            BLOG_TRACE_METHOD( getLogger(), fun );
		
			m_CustomerList = customerListDM;
		}

		void CustomerListMaintainUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
		{
			METHODNAME_DEF( CustomerListMaintainUC, injectUseCaseGetter )
            BLOG_TRACE_METHOD( getLogger(), fun );

			m_UseCaseGetter = useCaseGetter;
		}

		tender::viewConn::CustomerListMaintainVCPtr CustomerListMaintainUC::getCustomerListMaintainVC()
		{
			METHODNAME_DEF( CustomerListMaintainUC, getCustomerListMaintainVC )
            BLOG_TRACE_METHOD( getLogger(), fun );

			CHECK_INSTANCE_EXCEPTION( m_CustomerListMaintainVC.get() );
            return m_CustomerListMaintainVC;
		}

		libtender::domMod::customerlist::ICustomerListDMPtr CustomerListMaintainUC::getCustomerListDM()
		{
			METHODNAME_DEF( CustomerListMaintainUC, getCustomerListDM )
            BLOG_TRACE_METHOD( getLogger(), fun );
		
			CHECK_INSTANCE_EXCEPTION( m_CustomerList.get() );
			return m_CustomerList;
		}

		tender::componentManager::IUseCaseGetterPtr CustomerListMaintainUC::getUseCaseGetter()
        {
            METHODNAME_DEF( CustomerListMaintainUC, getUseCaseGetter )
            BLOG_TRACE_METHOD( getLogger(), fun );

            CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
            return m_UseCaseGetter;
        }

		const log4cplus::Logger& CustomerListMaintainUC::getLogger() const
        {
            METHODNAME_DEF( CustomerListMaintainUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

		void CustomerListMaintainUC::run()
		{
			METHODNAME_DEF( CustomerListMaintainUC, run )
            BLOG_TRACE_METHOD( m_Logger, fun );

			getCustomerListMaintainVC()->matchFromCustomerList( m_CustomerList );
			getCustomerListMaintainVC()->show();
		}

		basar::appl::EventReturnType CustomerListMaintainUC::addCustomer( basar::db::aspect::AccessorPropertyTable_YIterator customerYit )
		{
			METHODNAME_DEF( CustomerListMaintainUC, addCustomer )
            BLOG_TRACE_METHOD( getLogger(), fun );

			int customerNo = getCustomerListMaintainVC()->getCustomerNoToBeAdded();
			int branchNo = getCustomerListMaintainVC()->getBranchNo();

			for( ; !customerYit.isEnd(); ++customerYit )
			{
				if( customerNo == customerYit.getInt32( properties::CUSTOMER_NO ) &&
					branchNo   == customerYit.getInt16( properties::BRANCH_NO   ) )
				{
					if( basar::SS_DELETE != customerYit.getState( properties::CUSTOMER_NO ).getState() &&
						branchNo == customerYit.getInt16( properties::BRANCH_NO ) )
					{
						getCustomerListMaintainVC()->displayCustomerAlreadySelected();
						return basar::appl::EventReturnType( basar::appl::HANDLER_ERROR );
					}

					getCustomerListMaintainVC()->matchToCustomer( customerYit );

					if( customerYit.isContainedAndSet( properties::TENDER_NO ) )
					{
						customerYit.setPropertyState( properties::TENDER_NO, basar::SS_UPDATE );
					}

					customerYit.setPropertyState( properties::CUSTOMER_NO,		basar::SS_UPDATE );
					customerYit.setPropertyState( properties::BRANCH_NO,		basar::SS_UPDATE );
					customerYit.setPropertyState( properties::PHARMACY_NAME,	basar::SS_UPDATE );

					getCustomerListMaintainVC()->matchFromCustomerList( m_CustomerList );

					getCustomerListMaintainVC()->resetCustomer();
					return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
				}		
			}

			getCustomerListMaintainVC()->matchToCustomer( m_CustomerList->addEmptyCustomer() );
			getCustomerListMaintainVC()->matchFromCustomerList( m_CustomerList );

			getCustomerListMaintainVC()->resetCustomer();

			return basar::appl::EventReturnType( basar::appl::HANDLER_OK );
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		SYSTEM_EVENT_HANDLER_DEFINITION( CustomerListMaintainUC, SearchCustomerRequested )
		{
			METHODNAME_DEF( CustomerListMaintainUC, SearchCustomerRequested )
			BLOG_TRACE_METHOD( getLogger(), fun );
			rSource;

			try
			{
				getCustomerListDM()->resetSearchYit();
				getCustomerListMaintainVC()->matchToCustomer( getCustomerListDM()->getSearchYit() );
			
				tender::useCase::IPickerUCPtr customerPickerUC;
				
				customerPickerUC = getUseCaseGetter()->getCustomerPickerUC( getCustomerListDM()->getSearchYit() );
				customerPickerUC->run();
				
				getCustomerListMaintainVC()->matchFromCustomer( customerPickerUC->getPicked() );
				
				getCustomerListDM()->resetSearchYit();
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( getLogger(), e.what().c_str() );
				throw;
			}
			
			return basar::appl::EventReturnStruct();
		}

		SYSTEM_EVENT_HANDLER_DEFINITION( CustomerListMaintainUC, AddCustomerRequested )
		{
			METHODNAME_DEF( CustomerListMaintainUC, AddCustomerRequested )
			BLOG_TRACE_METHOD( getLogger(), fun );
			rSource;

			try
			{
				if( !handleCheckCustomerRequested( rSource, yitFromEvSrc ).isHandlerOK() ) 
				{
					getCustomerListMaintainVC()->displayInvalidCustomer();
					return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
				}

				basar::db::aspect::AccessorPropertyTable_YIterator customer = m_CustomerList->get().begin();
				return addCustomer( customer );
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( getLogger(), e.what().c_str() );
				throw;
			}
		}

		SYSTEM_EVENT_HANDLER_DEFINITION( CustomerListMaintainUC, EditCustomerRequested )
		{
			METHODNAME_DEF( CustomerListMaintainUC, EditCustomerRequested )
			BLOG_TRACE_METHOD( getLogger(), fun );
			rSource;

			try
			{
				getCustomerListMaintainVC()->matchFromCustomer( getCustomerListMaintainVC()->getSelectedCustomer( getCustomerListDM()->get() ) );
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( getLogger(), e.what().c_str() );
				throw;
			}
			
			return basar::appl::EventReturnStruct();
		}

		SYSTEM_EVENT_HANDLER_DEFINITION( CustomerListMaintainUC, DeleteCustomerRequested )
		{
			METHODNAME_DEF( CustomerListMaintainUC, DeleteCustomerRequested )
			BLOG_TRACE_METHOD( getLogger(), fun );
			rSource;
			
			try
			{
				basar::db::aspect::AccessorPropertyTableRef customers = getCustomerListDM()->get();
				basar::db::aspect::AccessorPropertyTable_YIterator customerToBeDeleted = getCustomerListMaintainVC()->getSelectedCustomer( getCustomerListDM()->get() );

				if( customerToBeDeleted.isNull() || customerToBeDeleted.isEnd() )
				{
					return basar::appl::EventReturnStruct( basar::appl::HANDLER_OK );
				}

				if(  basar::SS_CLEAN  == customerToBeDeleted.getState( properties::CUSTOMER_NO.getName() ).getState() ||
					 basar::SS_UPDATE == customerToBeDeleted.getState( properties::CUSTOMER_NO.getName() ).getState() )
				{
					customers.markForDelete( customerToBeDeleted );
				}

				if( basar::SS_INSERT == customerToBeDeleted.getState( properties::CUSTOMER_NO.getName() ).getState() )
				{
					customers.erase( customerToBeDeleted );
				}

				getCustomerListMaintainVC()->resetCustomer();
				getCustomerListMaintainVC()->matchFromCustomerList( m_CustomerList );
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( getLogger(), e.what().c_str() );
				throw;
			}
			
			return basar::appl::EventReturnStruct();
		}

		SYSTEM_EVENT_HANDLER_DEFINITION( CustomerListMaintainUC, CheckCustomerRequested )
		{
			METHODNAME_DEF( CustomerListMaintainUC, CheckCustomerRequested )
			BLOG_TRACE_METHOD( getLogger(), fun );
			rSource;
			
			tender::useCase::IPickerUCPtr customerPickerUC( getUseCaseGetter()->getCustomerPickerUC() );
			getCustomerListMaintainVC()->matchToCustomer( customerPickerUC->getSearchYIterator() );

			customerPickerUC->setCheckMode(true);
			customerPickerUC->run();

			basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer = customerPickerUC->getPicked();
			if( false == yitCustomer.isNull() && false == yitCustomer.isEnd() )
			{
				if (yitCustomer.getInt16(properties::ORIGINAL_BRANCH_NO) > 0)
				{
					yitCustomer.setInt16(properties::BRANCH_NO, yitCustomer.getInt16(properties::ORIGINAL_BRANCH_NO));
				}
				getCustomerListMaintainVC()->matchFromCustomer( yitCustomer );
			}
			else
			{
				getCustomerListMaintainVC()->resetCustomer();
				customerPickerUC->setCheckMode(false);
				return basar::appl::EventReturnType( basar::appl::HANDLER_NOT_FOUND );
			}

			customerPickerUC->setCheckMode(false);
			
			return basar::appl::EventReturnStruct();
		}

	} // end namespace useCase
} //end namespace tender
