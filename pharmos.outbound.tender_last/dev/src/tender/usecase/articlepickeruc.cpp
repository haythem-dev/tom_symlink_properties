
//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "articlepickeruc.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/article/iarticlecollectiondmptr.h"
#include "domainmodule/article/iarticlecollectiondm.h"
#include "iusecasegetter.h"

#include "articlepickervc.h"

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
		BEGIN_HANDLERS_REGISTRATION( ArticlePickerUC )
            SYSTEM_EVENT_HANDLER_REGISTRATION( ArticlePickerUC, SearchArticleRequested )
        END_HANDLERS_REGISTRATION()

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // ArticlePickerUC::ArticlePickerUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		ArticlePickerUC::ArticlePickerUC()
		: m_Logger( libtender::LoggerPool::getLoggerUseCases() ),
		  m_CheckMode(false)
		{
			METHODNAME_DEF( ArticlePickerUC, ArticlePickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			registerEventHandlers( true );
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // ArticlePickerUC::~ArticlePickerUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ArticlePickerUC::~ArticlePickerUC()
		{
			METHODNAME_DEF( ArticlePickerUC, ~ArticlePickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			registerEventHandlers( false );
		}

		void ArticlePickerUC::injectArticlePickerVC( tender::viewConn::ArticlePickerVCPtr articlePickerVC )
		{
			METHODNAME_DEF( ArticlePickerUC, injectArticlePickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_ArticlePickerVC = articlePickerVC;
		}

		void ArticlePickerUC::injectArticleCollectionDM ( libtender::domMod::article::IArticleCollectionDMPtr articleCollectionDMPtr )
		{
			METHODNAME_DEF( ArticlePickerUC, injectArticleCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_Article = articleCollectionDMPtr;
		}

		void ArticlePickerUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
		{
			METHODNAME_DEF( ArticlePickerUC, injectUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_UseCaseGetter = useCaseGetter;
		}

		void ArticlePickerUC::setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( ArticlePickerUC, setSearchCriteria )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_ArticlePickerVC->setSearchCriteria( yit );
		}

		void ArticlePickerUC::setCheckMode( bool checkMode )
		{
			METHODNAME_DEF( ArticlePickerUC, setCheckMode )
            BLOG_TRACE_METHOD( m_Logger, fun );

			m_CheckMode = checkMode;
		}

		tender::viewConn::ArticlePickerVCPtr ArticlePickerUC::getArticlePickerVC()
		{
			METHODNAME_DEF( ArticlePickerUC, getArtickePickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			CHECK_INSTANCE_EXCEPTION( m_ArticlePickerVC.get() );
            return m_ArticlePickerVC;
		}

		libtender::domMod::article::IArticleCollectionDMPtr ArticlePickerUC::getArticleCollectionDM()
		{
			METHODNAME_DEF( ArticlePickerUC, getArticleCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			CHECK_INSTANCE_EXCEPTION( m_Article.get() );
			return m_Article;
		}

		tender::componentManager::IUseCaseGetterPtr ArticlePickerUC::getUseCaseGetter()
        {
            METHODNAME_DEF( ArticlePickerUC, getUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
            return m_UseCaseGetter;
        }

		const log4cplus::Logger& ArticlePickerUC::getLogger() const
        {
            METHODNAME_DEF( ArticlePickerUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

		void ArticlePickerUC::init()
        {
            METHODNAME_DEF( ArticlePickerUC, init )
            BLOG_TRACE_METHOD( m_Logger, fun );

        }

        void ArticlePickerUC::reset()
        {
            METHODNAME_DEF( ArticlePickerUC, reset )
            BLOG_TRACE_METHOD( m_Logger, fun );

            init();
        }

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void ArticlePickerUC::run()
		{
			METHODNAME_DEF( ArticlePickerUC, run )
            BLOG_TRACE_METHOD( m_Logger, fun );

			init();

			if (true == m_CheckMode)
			{
				checkArticle();
			}
			else
			{
				searchArticle();
				getArticlePickerVC()->show();
			}
		}

		basar::db::aspect::AccessorPropertyTable_YIterator ArticlePickerUC::getPicked()
		{
			METHODNAME_DEF( ArticlePickerUC, getArticle )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if (true == m_CheckMode)
			{
				return getArticleCollectionDM()->get().begin();
			}
			return getArticlePickerVC()->getCurrentArticle();
		}

		basar::db::aspect::AccessorPropertyTable_YIterator ArticlePickerUC::getSearchYIterator()
		{
			METHODNAME_DEF( ArticlePickerUC, getSearchYIterator )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getArticleCollectionDM()->getSearchYit();
		}

		basar::appl::EventReturnType ArticlePickerUC::searchArticle()
		{
			METHODNAME_DEF( ArticlePickerUC, searchArticle )
            BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

			try
			{

				getArticleCollectionDM()->resetSearchYit();
				getArticlePickerVC()->matchToSearchCriteria( getArticleCollectionDM()->getSearchYit() );

				if( !getArticleCollectionDM()->getSearchYit().arePropertiesSet( properties::ARTICLE_CODE ) &&
					!getArticleCollectionDM()->getSearchYit().arePropertiesSet( properties::ARTICLE_NAME ) )
				{
					return result;
				}

				getArticleCollectionDM()->findByPattern( getArticleCollectionDM()->getSearchYit() );

				if( getArticleCollectionDM()->isEmpty() &&
				  ( getArticleCollectionDM()->getSearchYit().isContainedAndSet( properties::ARTICLE_CODE ) ||
				    getArticleCollectionDM()->getSearchYit().isContainedAndSet( properties::ARTICLE_NAME ) ))
				{
					result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::ArticlePickerUC", "No article found!").toLocal8Bit().data();

					getArticlePickerVC()->displayNoArticleFound();
				}

				getArticlePickerVC()->matchFromArticleCollection( getArticleCollectionDM()->get() );
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}

			return result;
		}

		basar::appl::EventReturnType ArticlePickerUC::checkArticle()
		{
			METHODNAME_DEF( ArticlePickerUC, checkArticle )
            BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

			try
			{
				getArticleCollectionDM()->resetSearchYit();
				getArticlePickerVC()->matchToSearchCriteria( getArticleCollectionDM()->getSearchYit() );
				getArticleCollectionDM()->findByArticleCode( getArticleCollectionDM()->getSearchYit() );

				if( getArticleCollectionDM()->isEmpty() &&
					getArticleCollectionDM()->getSearchYit().isContainedAndSet( properties::ARTICLE_CODE ))
				{
					result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::ArticlePickerUC", "No article found!").toLocal8Bit().data();
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
		SYSTEM_EVENT_HANDLER_DEFINITION( ArticlePickerUC, SearchArticleRequested)
		{
			METHODNAME_DEF( ArticlePickerUC, SearchArticleRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
			rSource;

			return searchArticle();
		}

	} //end namespace useCase
} // end namespace tender
