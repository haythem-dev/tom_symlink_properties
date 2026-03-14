#ifndef GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLECOLLECTIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLECOLLECTIONDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ifindarticle.h"
#include "articleinitparams.h"

#include "dmbase.h"
#include <libutil/misc/componentinitialization.hpp>

namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class ConnectionRef;
			class AccessorInstanceRef;
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

using basar::db::aspect::AccessorInstanceRef;
//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace article
{
    //-------------------------------------------------------------------------------------------------//
    // class declaration section
    //-------------------------------------------------------------------------------------------------//
    class ArticleCollectionDM : public libutil::misc::ComponentInitialization< ArticleInitParams, ArticleCollectionDM >,
		               public IFindArticle,
	                   private DMBase
	{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
        public:
            static const libutil::misc::ClassInfo& getClassInfo();

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
	    public:
		    ArticleCollectionDM( const basar::db::aspect::ConnectionRef );
		    ~ArticleCollectionDM();

            // defined in ifindarticle interface
			void                                                        findArticle( basar::db::aspect::AccessorPropertyTable_YIterator );
	        void                                                        resetSearchYit();
	        basar::db::aspect::AccessorPropertyTable_YIterator          getSearchYit();
	        basar::db::aspect::AccessorPropertyTableRef                 get();
            void                                                        clear();

        protected:
            // from DMBase
            AccessorExecuteParameterization						        getFindByPatternExecuteParameters() const;
            basar::VarString									        getSearchYIteratorAttributeNames()  const;
	        
            // from ComponentInitialization
            void doInit( const ArticleInitParams& );
	        void doShutdown();

	    private:
		
            /////////////////////////////////////////////////////////////////////////
		    // private member methods
		    /////////////////////////////////////////////////////////////////////////
		    ArticleCollectionDM(const ArticleCollectionDM& ArticleCollectionDM);
		    ArticleCollectionDM& operator= (const ArticleCollectionDM& ArticleCollectionDM);
            
        private:
		    /////////////////////////////////////////////////////////////////////////
		    // private member variables
		    /////////////////////////////////////////////////////////////////////////
            ArticleInitParams						                    m_InitParams;
	};
} // end namespace article
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLECOLLECTIONDM_H

