#ifndef GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLEINITPARAMS_H
#define GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLEINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace article
{
    struct ArticleInitParams
    {
	    basar::Int16 m_BranchNo;

        ArticleInitParams()
			: m_BranchNo( 0 )
        {}

		ArticleInitParams( const basar::Int16 branchNo ) 
	    : m_BranchNo( branchNo )
	    {
	    }
	    
    };

} // end namespace article
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLEINITPARAMS_H
