#ifndef GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLECONSULTANTINITPARAMS_H
#define GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLECONSULTANTINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace article
{
    struct ArticleConsultantInitParams
    {
	    basar::Int16     m_BranchNo;

	    ArticleConsultantInitParams( const basar::Int16 branchNo ) 
	    : m_BranchNo( branchNo )
	    {
	    }

		ArticleConsultantInitParams() 
	    : m_BranchNo( 0 )
	    {
	    }
    };

} // end namespace article
} // end namespace domMod
} // end namespace libabbauw
#endif // GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLECONSULTANTINITPARAMS_H