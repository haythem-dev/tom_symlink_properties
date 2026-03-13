#ifndef GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLEPRICEPTR_H
#define GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLEPRICEPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace article
{
    class IFindArticlePrice;
    typedef boost::shared_ptr< IFindArticlePrice > IFindArticlePricePtr;
	
} // end namespace article
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLEPRICEPTR_H
