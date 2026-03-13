#ifndef GUARD_LIBTENDER_DOMMOD_ARTICLE_ARTICLECOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_ARTICLE_ARTICLECOLLECTIONDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace article
{
	class ArticleCollectionDM;
	typedef boost::shared_ptr< ArticleCollectionDM > ArticleCollectionDMPtr;
} //end namespace article
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_ARTICLE_ARTICLECOLLECTIONDMPTR_H