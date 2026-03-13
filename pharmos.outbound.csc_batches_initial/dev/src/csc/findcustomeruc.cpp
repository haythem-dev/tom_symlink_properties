//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "findcustomeruc.h"

#include "icustomerfindervc.h"
#include "libabbauw/icustomercollectionfinder.h"

#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libutil/usecase.h>
#include <libutil/util.h>

namespace CSC_Batches
{
namespace useCase
{
	BEGIN_HANDLERS_REGISTRATION( FindCustomerUC )
        SYSTEM_EVENT_HANDLER_REGISTRATION( FindCustomerUC, FindCustomerRequested )
		SYSTEM_EVENT_HANDLER_REGISTRATION( FindCustomerUC, NewCustomerRequested  )
        SYSTEM_EVENT_HANDLER_REGISTRATION( FindCustomerUC, DialogCloseRequested  )
    END_HANDLERS_REGISTRATION()

	FindCustomerUC::FindCustomerUC() 
	{
	    BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, "FindCustomerUC::FindCustomerUC().");
	}

	FindCustomerUC::~FindCustomerUC()
	{
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, "FindCustomerUC::~FindCustomerUC().");
	}

	SYSTEM_EVENT_HANDLER_DEFINITION( FindCustomerUC, FindCustomerRequested )
    {
        METHODNAME_DEF( FindCustomerUC, FindCustomerRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        using namespace basar::appl;
        EventReturnStruct result( HANDLER_ERROR );
        
        try
        {
            m_CustomerCollectionFinderDM->resetSearchYit();
            m_CustomerFinderVC->matchToSearchCriteria( m_CustomerCollectionFinderDM->getSearchYit() );
			m_CustomerCollectionFinderDM->findByPattern( m_CustomerCollectionFinderDM->getSearchYit() );
			m_CustomerFinderVC->matchSearchResults();
			
            result = HANDLER_OK;
		}
        catch( basar::Exception& e )
        {
			m_CustomerFinderVC->error( e.what() );
            result.message = e.what();
        }
    
        return result;
    }

	SYSTEM_EVENT_HANDLER_DEFINITION( FindCustomerUC, NewCustomerRequested )
    {
        METHODNAME_DEF( FindCustomerUC, NewCustomerRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        using namespace basar::appl;
        EventReturnStruct result( HANDLER_ERROR );
        
        try
        {
			basar::appl::SystemEventManager::getInstance().fire( "UpdatePharmacyRequested", yitFromEvSrc );
            m_CustomerFinderVC->shutdown();
            result = HANDLER_OK;
        }
        catch( basar::Exception& e )
        {
			m_CustomerFinderVC->error( e.what() );
            result.message = e.what();
        }
    
        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( FindCustomerUC, DialogCloseRequested )
    {
        METHODNAME_DEF( FindCustomerUC, DialogCloseRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;
        
        m_CustomerFinderVC->shutdown();
        return basar::appl::EventReturnStruct();
    }

	void FindCustomerUC::injectVC(viewConn::ICustomerFinderVCPtr customerFinder )
    {
		m_CustomerFinderVC = customerFinder;
	}

	void FindCustomerUC::injectDM( libabbauw::domMod::customer::ICustomerCollectionFinderPtr customerCollectionFinder )
    {
		m_CustomerCollectionFinderDM = customerCollectionFinder;
	}
	
	void FindCustomerUC::run()
	{
		registerEventHandlers( true );
		try
        {
			m_CustomerFinderVC->init( m_CustomerCollectionFinderDM->get() );
			m_CustomerFinderVC->show();
		}
        catch( basar::Exception& )
        {
			throw;
		}
		registerEventHandlers( false );
	}
}// namespace useCase
}//namespace CSC_Batches
