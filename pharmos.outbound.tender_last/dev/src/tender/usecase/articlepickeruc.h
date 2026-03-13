#ifndef GUARD_TENDER_USECASE_ARTICLEPICKERUC_H
#define GUARD_TENDER_USECASE_ARTICLEPICKERUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "articlepickervcptr.h"
#include "ipickeruc.h"
#include "iusecasegetterptr.h"

#include <domainmodule/article/iarticlecollectiondmptr.h>

// libs
#include <libutil/irunnable.h>
#include <libutil/session.h>

#include <libbasarappl_eventmacros.h>
#include <libbasarguitie.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace tender
{
	namespace useCase
	{
		//-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
		class ArticlePickerUC : public tender::useCase::IPickerUC
		{
			public:
				ArticlePickerUC();
				virtual ~ArticlePickerUC();

				SYSTEM_EVENT_HANDLER_DECLARATION( SearchArticleRequested );

				void injectArticlePickerVC    ( tender::viewConn::ArticlePickerVCPtr articlePickerVC);
				void injectArticleCollectionDM( libtender::domMod::article::IArticleCollectionDMPtr articleCollectionDMPtr );
				void injectUseCaseGetter      ( tender::componentManager::IUseCaseGetterPtr useCaseGetter );

				virtual void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit );
				virtual void setCheckMode(bool checkMode);

				virtual void run();
				virtual basar::db::aspect::AccessorPropertyTable_YIterator getPicked();
				virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYIterator();

			private:
				ArticlePickerUC(const ArticlePickerUC& );
				ArticlePickerUC& operator= ( const ArticlePickerUC& );

				void                                        init();
				void                                        reset();
				void                                        registerEventHandlers( const bool forSave );

				const log4cplus::Logger&                              getLogger() const;
				tender::viewConn::ArticlePickerVCPtr                  getArticlePickerVC();
				libtender::domMod::article::IArticleCollectionDMPtr   getArticleCollectionDM();
				tender::componentManager::IUseCaseGetterPtr           getUseCaseGetter();

				basar::appl::EventReturnType                          searchArticle();
				basar::appl::EventReturnType                          checkArticle();

				//////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////
				tender::viewConn::ArticlePickerVCPtr                 m_ArticlePickerVC;
				libtender::domMod::article::IArticleCollectionDMPtr  m_Article;
				tender::componentManager::IUseCaseGetterPtr          m_UseCaseGetter;

				const log4cplus::Logger	                             m_Logger;
				bool												 m_CheckMode;
		};
	} //end namespace useCase
} //end namespace tender

#endif // end GUARD_TENDER_USECASE_ARTICLEPICKERUC_H
