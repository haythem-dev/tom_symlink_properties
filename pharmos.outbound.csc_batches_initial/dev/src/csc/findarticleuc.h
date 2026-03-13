#ifndef GUARD_CSC_BATCHES_USECASE_FINDARTICLEUC_H
#define GUARD_CSC_BATCHES_USECASE_FINDARTICLEUC_H

#include <libutil/irunnable.h>

#include "iarticlefindervcptr.h"
#include "libabbauw/ifindarticleptr.h"

#include <libbasarappl_eventmacros.h>
#include "libbasarappl_systemeventmanager.h"

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace appl
    {
        struct EventReturnStruct;
    }
}
    

namespace CSC_Batches
{
namespace useCase {

	//-------------------------------------------------------------------------------------------------//
	// class declaration
	//-------------------------------------------------------------------------------------------------//
	class FindArticleUC : public libutil::useCase::IRunnable
	{
	public:
		FindArticleUC();
		~FindArticleUC();

		SYSTEM_EVENT_HANDLER_DECLARATION( FindArticleRequested );
		SYSTEM_EVENT_HANDLER_DECLARATION( NewArticleRequested  );
        SYSTEM_EVENT_HANDLER_DECLARATION( DialogCloseRequested  );
		
		void injectVC( viewConn::IArticleFinderVCPtr );
		void injectDM( libabbauw::domMod::article::IFindArticlePtr );

        // from IRunnable interface
		void run();
		
	private:
		FindArticleUC(const FindArticleUC& orderuc);
		FindArticleUC& operator= (const FindArticleUC& order);
		
		void registerEventHandlers(const bool forSave);

    private:
		viewConn::IArticleFinderVCPtr                             m_ArticleFinderVC;
		libabbauw::domMod::article::IFindArticlePtr               m_FindArticleDM;
	};
	
}// namespace useCase
}// namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_USECASE_FINDARTICLEUC_H
