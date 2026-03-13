//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "supplierpickeruc.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/supplier/isuppliercollectiondmptr.h"
#include "domainmodule/supplier/isuppliercollectiondm.h"
#include "iusecasegetter.h"

#include "supplierpickervc.h"

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
        BEGIN_HANDLERS_REGISTRATION( SupplierPickerUC )
            SYSTEM_EVENT_HANDLER_REGISTRATION( SupplierPickerUC, SearchSupplierRequested )
        END_HANDLERS_REGISTRATION()

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // SupplierPickerUC::SupplierPickerUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SupplierPickerUC::SupplierPickerUC()
        : m_Logger( libtender::LoggerPool::getLoggerUseCases() ),
        m_CheckMode(false)
        {
            METHODNAME_DEF( SupplierPickerUC, SupplierPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            registerEventHandlers( true );
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // SupplierPickerUC::~SupplierPickerUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SupplierPickerUC::~SupplierPickerUC()
        {
            METHODNAME_DEF( SupplierPickerUC, ~SupplierPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            registerEventHandlers( false );
        }

        void SupplierPickerUC::injectSupplierPickerVC( tender::viewConn::SupplierPickerVCPtr supplierPickerVC )
        {
            METHODNAME_DEF( SupplierPickerUC, injectSupplierPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_SupplierPickerVC = supplierPickerVC;
        }

        void SupplierPickerUC::injectSupplierCollectionDM ( libtender::domMod::supplier::ISupplierCollectionDMPtr supplierCollectionDMPtr )
        {
            METHODNAME_DEF( SupplierPickerUC, injectSupplierCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_Supplier = supplierCollectionDMPtr;
        }

        void SupplierPickerUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
        {
            METHODNAME_DEF( SupplierPickerUC, injectUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_UseCaseGetter = useCaseGetter;
        }

        tender::viewConn::SupplierPickerVCPtr SupplierPickerUC::getSupplierPickerVC()
        {
            METHODNAME_DEF( SupplierPickerUC, getSupplierPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_SupplierPickerVC.get() );
            return m_SupplierPickerVC;
        }

        libtender::domMod::supplier::ISupplierCollectionDMPtr SupplierPickerUC::getSupplierCollectionDM()
        {
            METHODNAME_DEF( SupplierPickerUC, getSupplierCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_Supplier.get() );
            return m_Supplier;
        }

        tender::componentManager::IUseCaseGetterPtr SupplierPickerUC::getUseCaseGetter()
        {
            METHODNAME_DEF( SupplierPickerUC, getUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
            return m_UseCaseGetter;
        }

        const log4cplus::Logger& SupplierPickerUC::getLogger() const
        {
            METHODNAME_DEF( SupplierPickerUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

        void SupplierPickerUC::setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit )
        {
            METHODNAME_DEF( SupplierPickerUC, setSearchCriteria )
            BLOG_TRACE_METHOD( m_Logger, fun );

            getSupplierPickerVC()->setSearchCriteria( yit );
        }

        void SupplierPickerUC::setCheckMode( bool checkMode )
        {
            m_CheckMode = checkMode;
        }

        void SupplierPickerUC::init()
        {
            METHODNAME_DEF( SupplierPickerUC, init )
            BLOG_TRACE_METHOD( m_Logger, fun );
        }

        void SupplierPickerUC::reset()
        {
            METHODNAME_DEF( SupplierPickerUC, reset )
            BLOG_TRACE_METHOD( m_Logger, fun );

            init();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void SupplierPickerUC::run()
        {
            METHODNAME_DEF( SupplierPickerUC, run )
            BLOG_TRACE_METHOD( m_Logger, fun );

            init();

            if (true == m_CheckMode)
            {
                checkSupplier();
            }
            else
            {
                searchSupplier();
                getSupplierPickerVC()->show();
            }
        }

        basar::db::aspect::AccessorPropertyTable_YIterator SupplierPickerUC::getPicked()
        {
            METHODNAME_DEF( SupplierPickerUC, getPickedSupplier )
            BLOG_TRACE_METHOD( m_Logger, fun );

            if (true == m_CheckMode)
            {
                return getSupplierCollectionDM()->get().begin();
            }

            return getSupplierPickerVC()->getPickedSupplier();
        }

        basar::db::aspect::AccessorPropertyTable_YIterator SupplierPickerUC::getSearchYIterator()
        {
            METHODNAME_DEF( SupplierPickerUC, getSearchYIterator )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return getSupplierCollectionDM()->getSearchYit();
        }

        basar::appl::EventReturnType SupplierPickerUC::searchSupplier()
        {
            METHODNAME_DEF( SupplierPickerUC, searchSupplier )
            BLOG_TRACE_METHOD( m_Logger, fun );

            basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

            try
            {
                getSupplierCollectionDM()->resetSearchYit();
                getSupplierPickerVC()->matchToSearchCriteria( getSupplierCollectionDM()->getSearchYit() );
                
                if( !getSupplierCollectionDM()->getSearchYit().arePropertiesSet( properties::SUPPLIER_NAME ) &&
                    !getSupplierCollectionDM()->getSearchYit().arePropertiesSet( properties::SUPPLIER_NO ) )
                {
                    return result;
                }

                getSupplierCollectionDM()->findByPattern( getSupplierCollectionDM()->getSearchYit() );

                if( getSupplierCollectionDM()->isEmpty() )
                {
                    result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::SupplierPickerUC", "No supplier found!").toLocal8Bit().data();

                    getSupplierPickerVC()->displayNoSupplierFound();
                }

                getSupplierPickerVC()->matchFromSupplierCollection( getSupplierCollectionDM()->get() );
            }

            catch( const basar::Exception & e )
            {
                BLOG_ERROR( m_Logger, e.what().c_str() );
                throw;
            }

            return result;
        }

        basar::appl::EventReturnType SupplierPickerUC::checkSupplier()
        {
            METHODNAME_DEF( SupplierPickerUC, checkSupplier )
            BLOG_TRACE_METHOD( m_Logger, fun );

            basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
            
            try
            {
                getSupplierCollectionDM()->findBySupplierNo( getSupplierCollectionDM()->getSearchYit() );

                if( getSupplierCollectionDM()->isEmpty() &&
                    getSupplierCollectionDM()->getSearchYit().isContainedAndSet( properties::SUPPLIER_NO ))
                {
                    result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::SupplierPickerUC", "No supplier found!").toLocal8Bit().data();
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
        SYSTEM_EVENT_HANDLER_DEFINITION( SupplierPickerUC, SearchSupplierRequested)
        {
            METHODNAME_DEF( SupplierPickerUC, SearchSupplierRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;

            return searchSupplier();
        }

    } // end namespace useCase
} //end namespace tender
