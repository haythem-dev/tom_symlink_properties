#ifndef GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLEDMPTR_H
#define GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLEDMPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//

namespace libabbauw
{
namespace domMod
{
namespace article
{
	class ArticleCollectionDM;
    typedef boost::shared_ptr< ArticleCollectionDM > ArticleCollectionDMPtr;
	
} // end namespace article
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ARTICLE_ARTICLEDMPTR_H
