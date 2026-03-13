//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "pharmacychainpickeruc.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/pharmacychain/ipharmacychaincollectiondmptr.h"
#include "domainmodule/pharmacychain/ipharmacychaincollectiondm.h"
#include "iusecasegetter.h"

#include "pharmacychainpickervc.h"

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
		BEGIN_HANDLERS_REGISTRATION( PharmacyChainPickerUC )
			SYSTEM_EVENT_HANDLER_REGISTRATION( PharmacyChainPickerUC, SearchPharmacyChainRequested )
        END_HANDLERS_REGISTRATION()

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // PharmacyChainPickerUC::PharmacyChainPickerUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		PharmacyChainPickerUC::PharmacyChainPickerUC()
		: m_Logger( libtender::LoggerPool::getLoggerUseCases() )
		{
			METHODNAME_DEF( PharmacyChainPickerUC, PharmacyChainPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			registerEventHandlers( true );
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // PharmacyChainPickerUC::~PharmacyChainPickerUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		PharmacyChainPickerUC::~PharmacyChainPickerUC()
		{
			METHODNAME_DEF( PharmacyChainPickerUC, ~PharmacyChainPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			registerEventHandlers( false );
		}

		void PharmacyChainPickerUC::injectPharmacyChainPickerVC( tender::viewConn::PharmacyChainPickerVCPtr pharmacyChainPickerVC )
		{
			METHODNAME_DEF( PharmacyChainPickerUC, injectPharmacyChainPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_PharmacyChainPickerVC = pharmacyChainPickerVC;
		}

		void PharmacyChainPickerUC::injectPharmacyChainCollectionDM ( libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr pharmacyChainCollectionDMPtr )
		{
			METHODNAME_DEF( PharmacyChainPickerUC, injectPharmacyChainCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_PharmacyChain = pharmacyChainCollectionDMPtr;
		}

		void PharmacyChainPickerUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
		{
			METHODNAME_DEF( PharmacyChainPickerUC, injectUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_UseCaseGetter = useCaseGetter;
		}

		tender::viewConn::PharmacyChainPickerVCPtr PharmacyChainPickerUC::getPharmacyChainPickerVC()
		{
			METHODNAME_DEF( PharmacyChainPickerUC, getPharmacyChainPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			CHECK_INSTANCE_EXCEPTION( m_PharmacyChainPickerVC.get() );
            return m_PharmacyChainPickerVC;
		}

		libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr PharmacyChainPickerUC::getPharmacyChainCollectionDM()
		{
			METHODNAME_DEF( PharmacyChainPickerUC, getPharmacyChainCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			CHECK_INSTANCE_EXCEPTION( m_PharmacyChain.get() );
			return m_PharmacyChain;
		}

		tender::componentManager::IUseCaseGetterPtr PharmacyChainPickerUC::getUseCaseGetter()
        {
            METHODNAME_DEF( PharmacyChainPickerUC, getUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
            return m_UseCaseGetter;
        }

		const log4cplus::Logger& PharmacyChainPickerUC::getLogger() const
        {
            METHODNAME_DEF( PharmacyChainPickerUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

		void PharmacyChainPickerUC::setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( PharmacyChainPickerUC, setSearchCriteria )
            BLOG_TRACE_METHOD( m_Logger, fun );

			getPharmacyChainPickerVC()->setSearchCriteria( yit );
		}

		void PharmacyChainPickerUC::setCheckMode( bool checkMode )
		{
			m_CheckMode = checkMode;
		}

		void PharmacyChainPickerUC::setSearchWithoutGUIMode( bool mode )
		{
			m_SeachWithoutGUIMode = mode;
		}

		bool PharmacyChainPickerUC::isPharmacyChainValid()
		{
			return m_isPharmacyChainValid;
		}

		bool PharmacyChainPickerUC::isCustomerGroupIDValid()
		{
			return m_isCustomerGroupIDValid;
		}

		void PharmacyChainPickerUC::init()
        {
            METHODNAME_DEF( PharmacyChainPickerUC, init )
            BLOG_TRACE_METHOD( m_Logger, fun );
        }

        void PharmacyChainPickerUC::reset()
        {
            METHODNAME_DEF( PharmacyChainPickerUC, reset )
            BLOG_TRACE_METHOD( m_Logger, fun );

            init();
        }

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void PharmacyChainPickerUC::run()
		{
			METHODNAME_DEF( PharmacyChainPickerUC, run )
            BLOG_TRACE_METHOD( m_Logger, fun );

			init();

			if (true == m_CheckMode)
			{
				checkPharmacyChain();
				checkCustomerGroupID();
			}
			else if(true == m_SeachWithoutGUIMode)
			{
				searchPharmacyChain();
			}
			else
			{
				searchPharmacyChain();
				getPharmacyChainPickerVC()->show();
			}
		}

		basar::db::aspect::AccessorPropertyTable_YIterator PharmacyChainPickerUC::getPicked()
		{
			METHODNAME_DEF( PharmacyChainPickerUC, getPickedPharmacyChain )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if (true == m_CheckMode)
			{
				return getPharmacyChainCollectionDM()->get().begin();
			}

			return getPharmacyChainPickerVC()->getPickedPharmacyChain();
		}

		basar::db::aspect::AccessorPropertyTableRef PharmacyChainPickerUC::getFound()
		{
			METHODNAME_DEF( PharmacyChainPickerUC, getFound )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getPharmacyChainCollectionDM()->get();
		}

		basar::db::aspect::AccessorPropertyTable_YIterator PharmacyChainPickerUC::getSearchYIterator()
		{
			METHODNAME_DEF( PharmacyChainPickerUC, getSearchYIterator )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getPharmacyChainCollectionDM()->getSearchYit();
		}

		basar::appl::EventReturnType PharmacyChainPickerUC::searchPharmacyChain()
		{
			METHODNAME_DEF( PharmacyChainPickerUC, searchPharmacyChain )
            BLOG_TRACE_METHOD( m_Logger, fun );
			
			basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

			try
			{
				if(!m_SeachWithoutGUIMode)
				{
					getPharmacyChainCollectionDM()->resetSearchYit();
				}

				getPharmacyChainPickerVC()->matchToSearchCriteria( getPharmacyChainCollectionDM()->getSearchYit() );

				if( !getPharmacyChainCollectionDM()->getSearchYit().arePropertiesSet( properties::PHARMACY_CHAIN_NO ) &&
					!getPharmacyChainCollectionDM()->getSearchYit().arePropertiesSet( properties::CUSTOMER_NO )       &&
					!getPharmacyChainCollectionDM()->getSearchYit().arePropertiesSet( properties::PHARMACY_NAME ) )
				{
					return result;
				}

				getPharmacyChainCollectionDM()->findByPattern( getPharmacyChainCollectionDM()->getSearchYit() );

				if( getPharmacyChainCollectionDM()->isEmpty() && !m_SeachWithoutGUIMode)
				{
					result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::PharmacyChainPickerUC", "No pharmacy chain found!").toLocal8Bit().data();

					getPharmacyChainPickerVC()->displayNoPharmacyChainFound();
				}

				getPharmacyChainPickerVC()->matchFromPharmacyChainCollection( getPharmacyChainCollectionDM()->get() );
			}

			catch( const basar::Exception & e )
			{
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}

			return result;
		}

		void PharmacyChainPickerUC::checkPharmacyChain()
		{
			METHODNAME_DEF( CustomerPickerUC, checkPharmacyChain )
            BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

			try
			{
				getPharmacyChainCollectionDM()->findByPharmacyChainNo( getPharmacyChainCollectionDM()->getSearchYit() );

				if( getPharmacyChainCollectionDM()->isEmpty() &&
					getPharmacyChainCollectionDM()->getSearchYit().isContainedAndSet( properties::PHARMACY_CHAIN_NO ))
				{
					m_isPharmacyChainValid = false;
					return;
				}

				m_isPharmacyChainValid = true;
				return;
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}
		}

		void PharmacyChainPickerUC::checkCustomerGroupID()
		{
			METHODNAME_DEF( CustomerPickerUC, checkCustomerGroupID )
            BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

			try
			{
				getPharmacyChainCollectionDM()->findCustomerGroupIDByPharmacyChain( getPharmacyChainCollectionDM()->getSearchYit() );

				if( getPharmacyChainCollectionDM()->isEmpty() )
				{
					m_isCustomerGroupIDValid = false;
					return;
				}

				m_isCustomerGroupIDValid = true;
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		SYSTEM_EVENT_HANDLER_DEFINITION( PharmacyChainPickerUC, SearchPharmacyChainRequested)
		{
			METHODNAME_DEF( PharmacyChainPickerUC, SearchPharmacyChainRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
			rSource;

			return searchPharmacyChain();
		}

	} // end namespace useCase
} //end namespace tender
