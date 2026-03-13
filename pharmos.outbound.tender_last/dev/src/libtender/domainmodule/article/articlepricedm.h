#ifndef GUARD_LIBTENDER_DOMMOD_ARTICLE_ARTICLEPRICEDM_H
#define GUARD_LIBTENDER_DOMMOD_ARTICLE_ARTICLEPRICEDM_H

#include "iarticlepricedm.h"

#include <componentmanager/itenderconnectedfactoryptr.h>

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace libtender {
namespace domMod {
namespace article {

class ArticlePriceDM : public IArticlePrice
{
public:
	static const libutil::misc::ClassInfo& getClassInfo();

	ArticlePriceDM();
	~ArticlePriceDM();

	void injectArticlePriceAccessor(libutil::infrastructure::accessor::IAccessorPtr);
	void setAreaID(basar::Int16 areaID);

	virtual void findByArticleCode(basar::db::aspect::AccessorPropertyTable_YIterator);

	virtual basar::db::aspect::AccessorPropertyTableRef get() const;
	virtual bool isEmpty() const;

private:
	ArticlePriceDM( const ArticlePriceDM & );
	ArticlePriceDM & operator = ( const ArticlePriceDM & );

	void findByArticleCodeOtherCountries(basar::db::aspect::AccessorPropertyTable_YIterator);
	void findByArticleCodeBG(basar::db::aspect::AccessorPropertyTable_YIterator);

	libutil::infrastructure::accessor::IAccessorPtr getArticlePriceAccessor() const;
	const log4cplus::Logger &				        getLogger() const;


	libutil::infrastructure::accessor::IAccessorPtr m_Accessor;
	const log4cplus::Logger 				        m_Logger;
	
	basar::Int16									m_AreaID;
};

}
}
}

#endif 
