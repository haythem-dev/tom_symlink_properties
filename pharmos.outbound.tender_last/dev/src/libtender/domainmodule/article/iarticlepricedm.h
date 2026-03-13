#ifndef GUARD_LIBTENDER_DOMMOD_ARTICLE_IARTICLEPRICEDM_H
#define GUARD_LIBTENDER_DOMMOD_ARTICLE_IARTICLEPRICEDM_H

namespace basar {
namespace db {
namespace aspect {
	class AccessorPropertyTable_YIterator;
	class AccessorPropertyTableRef;
}
}
}

namespace libtender {
namespace domMod {
namespace article {

class IArticlePrice
{
public:
	virtual ~IArticlePrice() {}

	virtual void findByArticleCode(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;


	virtual basar::db::aspect::AccessorPropertyTableRef get() const = 0;
	virtual bool isEmpty() const = 0;
};

}
}
}

#endif //end GUARD_LIBTENDER_DOMMOD_ARTICLE_IARTICLEPRICEDM_H
