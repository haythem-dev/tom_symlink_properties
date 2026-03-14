#ifndef GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLECONSULTANTDM_H
#define GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLECONSULTANTDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ifindarticleprice.h"
#include "articleconsultantinitparams.h"

#include <libutil/misc/componentinitialization.hpp>
#include "dmbase.h"

namespace basar
{
	namespace db
	{
		namespace aspect
		{
            class ConnectionRef;
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

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
    class ArticleConsultantDM : public libutil::misc::ComponentInitialization< ArticleConsultantInitParams, ArticleConsultantDM >,
                                public IFindArticlePrice,
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
		    ArticleConsultantDM( const basar::db::aspect::ConnectionRef );
		    ~ArticleConsultantDM();

			// from IFindArticlePrice
			void                                                findArticlePrice( basar::Int32 articleno );
            basar::db::aspect::AccessorPropertyTable_YIterator  get();

		private:
            void                                                resetSearchYit();
			basar::db::aspect::AccessorPropertyTable_YIterator	getSearchYit();

			// from DMBase
			AccessorExecuteParameterization						getFindByPatternExecuteParameters() const;
            basar::VarString									getSearchYIteratorAttributeNames()  const;

        protected:
	            void doInit( const ArticleConsultantInitParams& );
	            void doShutdown();

	    private:
		    ArticleConsultantDM(const ArticleConsultantDM& ArticleConsultantDM);
		    ArticleConsultantDM& operator= (const ArticleConsultantDM& ArticleConsultantDM);
            
        private:
            ArticleConsultantInitParams	                        m_InitParams;
	};

} // end namespace article
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLECONSULTANTDM_H

