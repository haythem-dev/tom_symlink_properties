//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "pharmacygrouppickeruc.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/pharmacygroup/ipharmacygroupcollectiondmptr.h"
#include "domainmodule/pharmacygroup/ipharmacygroupcollectiondm.h"
#include "iusecasegetter.h"

#include "pharmacygrouppickervc.h"

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
        BEGIN_HANDLERS_REGISTRATION( PharmacyGroupPickerUC )
            SYSTEM_EVENT_HANDLER_REGISTRATION( PharmacyGroupPickerUC, SearchPharmacyGroupRequested )
        END_HANDLERS_REGISTRATION()
        
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // PharmacyGroupPickerUC::PharmacyGroupPickerUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        PharmacyGroupPickerUC::PharmacyGroupPickerUC() :
            m_Logger( libtender::LoggerPool::getLoggerUseCases() ),
            m_CheckMode(false)
        {
            
            METHODNAME_DEF( PharmacyGroupPickerUC, PharmacyGroupPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            registerEventHandlers( true );
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // PharmacyGroupPickerUC::~PharmacyGroupPickerUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        PharmacyGroupPickerUC::~PharmacyGroupPickerUC()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, ~PharmacyGroupPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            registerEventHandlers( false );
        }
        
        void PharmacyGroupPickerUC::injectPharmacyGroupPickerVC( tender::viewConn::PharmacyGroupPickerVCPtr pharmacyGroupPickerVC )
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, injectPharmacyGroupPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            m_PharmacyGroupPickerVC = pharmacyGroupPickerVC;
        }
        
        void PharmacyGroupPickerUC::injectPharmacyGroupCollectionDM ( libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr pharmacyGroupCollectionDMPtr )
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, injectPharmacyGroupCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            m_PharmacyGroup = pharmacyGroupCollectionDMPtr;
        }
        
        void PharmacyGroupPickerUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, injectUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            m_UseCaseGetter = useCaseGetter;
        }
        
        tender::viewConn::PharmacyGroupPickerVCPtr PharmacyGroupPickerUC::getPharmacyGroupPickerVC()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, getPharmacyGroupPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            CHECK_INSTANCE_EXCEPTION( m_PharmacyGroupPickerVC.get() );
            return m_PharmacyGroupPickerVC;
        }
        
        libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr PharmacyGroupPickerUC::getPharmacyGroupCollectionDM()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, getPharmacyGroupCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            CHECK_INSTANCE_EXCEPTION( m_PharmacyGroup.get() );
            return m_PharmacyGroup;
        }
        
        tender::componentManager::IUseCaseGetterPtr PharmacyGroupPickerUC::getUseCaseGetter()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, getUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );
        
            CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
            return m_UseCaseGetter;
        }

        const log4cplus::Logger& PharmacyGroupPickerUC::getLogger() const
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

        void PharmacyGroupPickerUC::setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit )
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, setSearchCriteria )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            getPharmacyGroupPickerVC()->setPharmacyGroupId( yit );
        }
        
        void PharmacyGroupPickerUC::setCheckMode( bool checkMode )
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, setCheckMode )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            m_CheckMode = checkMode;
        }
        
        void PharmacyGroupPickerUC::init()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, init )
            BLOG_TRACE_METHOD( m_Logger, fun );
        }

        void PharmacyGroupPickerUC::reset()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, reset )
            BLOG_TRACE_METHOD( m_Logger, fun );

            init();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void PharmacyGroupPickerUC::run()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, run )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            init();
            
            if (true == m_CheckMode)
            {
                checkPharmacyGroup();
            }
            else
            {
                searchPharmacyGroup();
                getPharmacyGroupPickerVC()->show();
            }
        }
        
        basar::db::aspect::AccessorPropertyTable_YIterator PharmacyGroupPickerUC::getPicked()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, getPickedPharmacyGroup )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            if (true == m_CheckMode)
            {
                return getPharmacyGroupCollectionDM()->get().begin();
            }
            return getPharmacyGroupPickerVC()->getPickedPharmacyGroup();
        }
        
        basar::db::aspect::AccessorPropertyTable_YIterator PharmacyGroupPickerUC::getSearchYIterator()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, getSearchYIterator )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            return getPharmacyGroupCollectionDM()->getSearchYit();
        }
        
        basar::appl::EventReturnType PharmacyGroupPickerUC::searchPharmacyGroup()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, searchPharmacyGroup )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
            
            try
            {
				// TODO SK: have a look at pharmacygroupcollectionacc.cpp for details

                getPharmacyGroupCollectionDM()->resetSearchYit();
                getPharmacyGroupPickerVC()->matchToSearchCriteria( getPharmacyGroupCollectionDM()->getSearchYit() );
                
                if( !getPharmacyGroupCollectionDM()->getSearchYit().arePropertiesSet( properties::PHARMACY_GROUPID ) &&
                    !getPharmacyGroupCollectionDM()->getSearchYit().arePropertiesSet( properties::PHARMACY_GROUP_NAME ) )
                {
                    return result;
                }
                
                getPharmacyGroupCollectionDM()->findByPattern( getPharmacyGroupCollectionDM()->getSearchYit() );
                
                if( getPharmacyGroupCollectionDM()->isEmpty() && 
                  ( getPharmacyGroupCollectionDM()->getSearchYit().isContainedAndSet( properties::PHARMACY_GROUPID )||
                    getPharmacyGroupCollectionDM()->getSearchYit().isContainedAndSet( properties::PHARMACY_GROUP_NAME ) ) )
                {
                    result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::PharmacyGroupPickerUC", "No pharmacygroup found!").toLocal8Bit().data();
                
                    getPharmacyGroupPickerVC()->displayNoPharmacyGroupFound();
                }
                
                getPharmacyGroupPickerVC()->matchFromPharmacyGroupCollection( getPharmacyGroupCollectionDM()->get() );
                
                return result;
            }
            catch( const basar::Exception & e )
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }
        }

        basar::appl::EventReturnType PharmacyGroupPickerUC::checkPharmacyGroup()
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, checkPharmacyGroup )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
            
            try
            {
				// TODO SK: have a look at pharmacygroupcollectionacc.cpp for details

                getPharmacyGroupCollectionDM()->findByPattern( getPharmacyGroupCollectionDM()->getSearchYit() );
                
                if( getPharmacyGroupCollectionDM()->isEmpty() &&
                    getPharmacyGroupCollectionDM()->getSearchYit().isContainedAndSet( properties::PHARMACY_GROUPID ) )
                {
                    result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::PharmacyGroupPickerUC", "No pharmacygroup found!").toLocal8Bit().data();
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
        SYSTEM_EVENT_HANDLER_DEFINITION( PharmacyGroupPickerUC, SearchPharmacyGroupRequested)
        {
            METHODNAME_DEF( PharmacyGroupPickerUC, SearchPharmacyGroupRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;
            
            return searchPharmacyGroup();
        }

	} // end namespace useCase
} //end namespace tender
