#ifndef GUARD_LIBTENDER_DOMMOD_ARTICLE_IARTICLEPRICEDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_ARTICLE_IARTICLEPRICEDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender {
namespace domMod {
namespace article {

	class IArticlePrice;
	typedef boost::shared_ptr<IArticlePrice> IArticlePricePtr;
}
}
}

#endif 
