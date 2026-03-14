#ifndef GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLEPTR_H
#define GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLEPTR_H

#include <boost/shared_ptr.hpp>
namespace libabbauw
{
	namespace domMod
	{
		namespace article
		{
			class IFindArticle;
			typedef boost::shared_ptr< libabbauw::domMod::article::IFindArticle > IFindArticlePtr;
		}
	}
}
#endif // GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLEPTR_H
