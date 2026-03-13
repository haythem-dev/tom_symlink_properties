//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "partnerpickeruc.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/partner/ipartnercollectiondmptr.h"
#include "domainmodule/partner/ipartnercollectiondm.h"
#include "iusecasegetter.h"

#include "partnerpickervc.h"

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
		BEGIN_HANDLERS_REGISTRATION( PartnerPickerUC )
			SYSTEM_EVENT_HANDLER_REGISTRATION( PartnerPickerUC, SearchPartnerRequested )
        END_HANDLERS_REGISTRATION()

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // PartnerPickerUC::PartnerPickerUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		PartnerPickerUC::PartnerPickerUC()
		: m_Logger( libtender::LoggerPool::getLoggerUseCases() )
		{
			METHODNAME_DEF( PartnerPickerUC, PartnerPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			registerEventHandlers( true );
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // PartnerPickerUC::~PartnerPickerUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		PartnerPickerUC::~PartnerPickerUC()
		{
			METHODNAME_DEF( PartnerPickerUC, ~PartnerPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			registerEventHandlers( false );
		}

		void PartnerPickerUC::injectPartnerPickerVC( tender::viewConn::PartnerPickerVCPtr partnerPickerVC )
		{
			METHODNAME_DEF( PartnerPickerUC, injectPartnerPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_PartnerPickerVC = partnerPickerVC;
		}

		void PartnerPickerUC::injectPartnerCollectionDM ( libtender::domMod::partner::IPartnerCollectionDMPtr partnerCollectionDMPtr )
		{
			METHODNAME_DEF( PartnerPickerUC, injectPartnerCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_Partner = partnerCollectionDMPtr;
		}

		void PartnerPickerUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
		{
			METHODNAME_DEF( PartnerPickerUC, injectUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_UseCaseGetter = useCaseGetter;
		}

		tender::viewConn::PartnerPickerVCPtr PartnerPickerUC::getPartnerPickerVC()
		{
			METHODNAME_DEF( PartnerPickerUC, getPartnerPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			CHECK_INSTANCE_EXCEPTION( m_PartnerPickerVC.get() );
            return m_PartnerPickerVC;
		}

		libtender::domMod::partner::IPartnerCollectionDMPtr PartnerPickerUC::getPartnerCollectionDM()
		{
			METHODNAME_DEF( PartnerPickerUC, getPartnerCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			CHECK_INSTANCE_EXCEPTION( m_Partner.get() );
			return m_Partner;
		}

		tender::componentManager::IUseCaseGetterPtr PartnerPickerUC::getUseCaseGetter()
        {
            METHODNAME_DEF( PartnerPickerUC, getUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
            return m_UseCaseGetter;
        }

		const log4cplus::Logger& PartnerPickerUC::getLogger() const
        {
            METHODNAME_DEF( PartnerPickerUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

		void PartnerPickerUC::setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( PartnerPickerUC, setSearchCriteria )
            BLOG_TRACE_METHOD( m_Logger, fun );

			getPartnerPickerVC()->setSearchCriteria( yit );
		}

		void PartnerPickerUC::setCheckMode( bool checkMode )
		{
			m_CheckMode = checkMode;
		}

		void PartnerPickerUC::init()
        {
            METHODNAME_DEF( PartnerPickerUC, init )
            BLOG_TRACE_METHOD( m_Logger, fun );
        }

        void PartnerPickerUC::reset()
        {
            METHODNAME_DEF( PartnerPickerUC, reset )
            BLOG_TRACE_METHOD( m_Logger, fun );

            init();
        }

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void PartnerPickerUC::run()
		{
			METHODNAME_DEF( PartnerPickerUC, run )
            BLOG_TRACE_METHOD( m_Logger, fun );

			init();

			if (true == m_CheckMode)
			{
				checkPartner();
			}
			else
			{
				searchPartner();
				getPartnerPickerVC()->show();
			}
		}

		basar::db::aspect::AccessorPropertyTable_YIterator PartnerPickerUC::getPicked()
		{
			METHODNAME_DEF( PartnerPickerUC, getPickedPartner )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if (true == m_CheckMode)
			{
				return getPartnerCollectionDM()->get().begin();
			}

			return getPartnerPickerVC()->getPickedPartner();
		}

		basar::db::aspect::AccessorPropertyTable_YIterator PartnerPickerUC::getSearchYIterator()
		{
			METHODNAME_DEF( PartnerPickerUC, getSearchYIterator )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getPartnerCollectionDM()->getSearchYit();
		}

		basar::appl::EventReturnType PartnerPickerUC::searchPartner()
		{
			METHODNAME_DEF( PartnerPickerUC, searchPartner )
            BLOG_TRACE_METHOD( m_Logger, fun );
			
			basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

			try
			{
				getPartnerCollectionDM()->resetSearchYit();
				getPartnerPickerVC()->matchToSearchCriteria( getPartnerCollectionDM()->getSearchYit() );

				if( !getPartnerCollectionDM()->getSearchYit().arePropertiesSet( properties::PARTNER_NAME ) &&
					!getPartnerCollectionDM()->getSearchYit().arePropertiesSet( properties::PARTNER_NO ) )
				{
					return result;
				}

				getPartnerCollectionDM()->findByPattern( getPartnerCollectionDM()->getSearchYit() );

				if( getPartnerCollectionDM()->isEmpty() )
				{
					result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::PartnerPickerUC", "No partner found!").toLocal8Bit().data();

					getPartnerPickerVC()->displayNoPartnerFound();
				}

				getPartnerPickerVC()->matchFromPartnerCollection( getPartnerCollectionDM()->get() );
			}

			catch( const basar::Exception & e )
			{
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}

			return result;
		}

		basar::appl::EventReturnType PartnerPickerUC::checkPartner()
		{
			METHODNAME_DEF( CustomerPickerUC, checkPartner )
            BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

			try
			{
				getPartnerCollectionDM()->findByPartnerNo( getPartnerCollectionDM()->getSearchYit() );

				if( getPartnerCollectionDM()->isEmpty() &&
					getPartnerCollectionDM()->getSearchYit().isContainedAndSet( properties::PARTNER_NO ))
				{
					result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::PartnerPickerUC", "No partner found!").toLocal8Bit().data();
				}
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}

			return result;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		SYSTEM_EVENT_HANDLER_DEFINITION( PartnerPickerUC, SearchPartnerRequested)
		{
			METHODNAME_DEF( PartnerPickerUC, SearchPartnerRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
			rSource;

			return searchPartner();
		}

	} // end namespace useCase
} //end namespace tender
