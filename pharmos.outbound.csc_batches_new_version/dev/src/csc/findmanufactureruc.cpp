//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "findmanufactureruc.h"

#include "imanufacturerfindervc.h"
#include "libabbauw/ifindmanufacturer.h"

#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libutil/usecase.h>
#include <libutil/util.h>

namespace CSC_Batches
{
namespace useCase
{
	BEGIN_HANDLERS_REGISTRATION( FindManufacturerUC )
        SYSTEM_EVENT_HANDLER_REGISTRATION( FindManufacturerUC, FindManufacturerRequested )
		SYSTEM_EVENT_HANDLER_REGISTRATION( FindManufacturerUC, NewManufacturerRequested  )
        SYSTEM_EVENT_HANDLER_REGISTRATION( FindManufacturerUC, DialogCloseRequested  )
    END_HANDLERS_REGISTRATION()

	FindManufacturerUC::FindManufacturerUC() 
	{
        METHODNAME_DEF( FindManufacturerUC, FindManufacturerUC );
	    BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );
	}

	FindManufacturerUC::~FindManufacturerUC()
	{
        METHODNAME_DEF( FindManufacturerUC, ~FindManufacturerUC );
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );
	}

    void FindManufacturerUC::injectVC(viewConn::IManufacturerFinderVCPtr supplierfinder )
    {
		m_ManufacturerFinderVC = supplierfinder;
	}

	void FindManufacturerUC::injectDM( libabbauw::domMod::manufacturerdm::IFindManufacturerPtr findManufacturer )
    {
		m_FindManufacturerDM = findManufacturer;
	}

	SYSTEM_EVENT_HANDLER_DEFINITION( FindManufacturerUC, FindManufacturerRequested )
    {
        METHODNAME_DEF( FindManufacturerUC, FindManufacturerRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );
        try
        {
            m_FindManufacturerDM->resetSearchYit();
            m_ManufacturerFinderVC->matchToSearchCriteria( m_FindManufacturerDM->getSearchYit() );
			m_FindManufacturerDM->findManufacturer( m_FindManufacturerDM->getSearchYit() );
			m_ManufacturerFinderVC->matchSearchResults();
			
            result = basar::appl::HANDLER_OK;
		}
        catch( basar::Exception& e ) {
			m_ManufacturerFinderVC->error( e.what() );
            result.message = e.what();
        }

        return result;
    }

	SYSTEM_EVENT_HANDLER_DEFINITION( FindManufacturerUC, NewManufacturerRequested )
    {
        METHODNAME_DEF( FindManufacturerUC, NewManufacturerRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        
        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );
        try
        {
			basar::appl::SystemEventManager::getInstance().fire( "UpdateManufacturerRequested", yitFromEvSrc );
            m_ManufacturerFinderVC->shutdown();
            result = basar::appl::HANDLER_OK;
        }
        catch( basar::Exception& e )
        {
			m_ManufacturerFinderVC->error( e.what() );
            result.message = e.what();
        }
    
        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( FindManufacturerUC, DialogCloseRequested )
    {
        METHODNAME_DEF( FindManufacturerUC, DialogCloseRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;
        
        m_ManufacturerFinderVC->shutdown();
        return basar::appl::EventReturnStruct();
    }
	
	void FindManufacturerUC::run()
	{
		registerEventHandlers( true );
		try
        {
			m_ManufacturerFinderVC->init( m_FindManufacturerDM->getPropTab() );
			m_ManufacturerFinderVC->show();
		}
        catch(basar::Exception&)
        {
			throw;
		}
		registerEventHandlers( false );
	}


}// namespace useCase
}//namespace CSC_Batches