//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "findarticleuc.h"

#include "iarticlefindervc.h"
#include "libabbauw/ifindarticle.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libutil/usecase.h>
#include <libutil/util.h>

namespace CSC_Batches
{
namespace useCase
{
	BEGIN_HANDLERS_REGISTRATION( FindArticleUC )
        SYSTEM_EVENT_HANDLER_REGISTRATION( FindArticleUC, FindArticleRequested )
		SYSTEM_EVENT_HANDLER_REGISTRATION( FindArticleUC, NewArticleRequested  )
        SYSTEM_EVENT_HANDLER_REGISTRATION( FindArticleUC, DialogCloseRequested )
    END_HANDLERS_REGISTRATION()

	FindArticleUC::FindArticleUC() 
	{
        METHODNAME_DEF( FindArticleUC, FindArticleUC );
	    BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );
	}

	FindArticleUC::~FindArticleUC()
	{
        METHODNAME_DEF( FindArticle, ~FindArticle );
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );
	}

    void FindArticleUC::injectVC(viewConn::IArticleFinderVCPtr articlefinder )
    {
		m_ArticleFinderVC = articlefinder;
	}

	void FindArticleUC::injectDM( libabbauw::domMod::article::IFindArticlePtr findArticle )
    {
		m_FindArticleDM = findArticle;
	}

	SYSTEM_EVENT_HANDLER_DEFINITION( FindArticleUC, FindArticleRequested )
    {
        METHODNAME_DEF( FindArticleUC, FindArticleRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );
        try
        {
            //m_FindArticleDM->resetSearchYit();
            m_ArticleFinderVC->matchToSearchCriteria( m_FindArticleDM->getSearchYit() );
			m_FindArticleDM->findArticle( m_FindArticleDM->getSearchYit() );
			m_ArticleFinderVC->matchSearchResults();
			
            result = basar::appl::HANDLER_OK;
		}
        catch( basar::Exception& e ) {
			m_ArticleFinderVC->error( e.what() );
            result.message = e.what();
        }

        return result;
    }
    
	SYSTEM_EVENT_HANDLER_DEFINITION( FindArticleUC, NewArticleRequested )
    {
        METHODNAME_DEF( FindArticleUC, NewArticleRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        
        basar::appl::EventReturnStruct result( basar::appl::HANDLER_ERROR );
        try
        {
			basar::appl::SystemEventManager::getInstance().fire( "UpdateArticleRequested", yitFromEvSrc );
            m_ArticleFinderVC->shutdown();
            result = basar::appl::HANDLER_OK;
        }
        catch( basar::Exception& e )
        {
			m_ArticleFinderVC->error( e.what() );
            result.message = e.what();
        }
    
        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( FindArticleUC, DialogCloseRequested )
    {
        METHODNAME_DEF( FindArticleUC, DialogCloseRequested )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;
        
        m_ArticleFinderVC->shutdown();
        return basar::appl::EventReturnStruct();
    }

	void FindArticleUC::run()
	{
		METHODNAME_DEF( FindArticleUC, run )    
        BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerUseCases, fun );

		registerEventHandlers( true );
		try
        {
			m_ArticleFinderVC->init( m_FindArticleDM->get() );
             
            // synchronize Matchcode input field with ArticleFinderVC
            if( m_FindArticleDM->getSearchYit().isContainedAndSet( libabbauw::properties::MATCHCODE ) )
            {
                m_ArticleFinderVC->setMatchCode( m_FindArticleDM->getSearchYit().getString( libabbauw::properties::MATCHCODE ) );
            }

            m_ArticleFinderVC->matchSearchResults();
			m_ArticleFinderVC->show();

            m_FindArticleDM->resetSearchYit();
            m_FindArticleDM->clear();
		}
        catch(basar::Exception&)
        {
			throw;
		}
		registerEventHandlers( false );
	}

} // namespace useCase
} //namespace CSC_Batches
