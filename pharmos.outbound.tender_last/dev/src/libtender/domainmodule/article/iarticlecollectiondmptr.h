#ifndef GUARD_LIBTENDER_DOMMOD_ARTICLE_IARTICLECOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_ARTICLE_IARTICLECOLLECTIONDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace article
{
	class IArticleCollectionDM;
	typedef boost::shared_ptr< IArticleCollectionDM > IArticleCollectionDMPtr;
} //end namespace article
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_ARTICLE_IARTICLECOLLECTIONDMPTR_H
