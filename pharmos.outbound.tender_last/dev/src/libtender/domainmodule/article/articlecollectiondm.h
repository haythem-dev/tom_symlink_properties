#ifndef GUARD_LIBTENDER_DOMMOD_ARTICLE_ARTICLECOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_ARTICLE_ARTICLECOLLECTIONDM_H

#include "iarticlecollectiondm.h"

#include <componentmanager/itenderconnectedfactoryptr.h>

#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace libtender
{
namespace domMod
{
namespace article
{

class ArticleCollectionDM : public IArticleCollectionDM
{
public:
	static const libutil::misc::ClassInfo& getClassInfo();

	ArticleCollectionDM();
	~ArticleCollectionDM();

	void injectArticleAccessor( libutil::infrastructure::accessor::IAccessorPtr );

	virtual void                                                findByPattern(basar::db::aspect::AccessorPropertyTable_YIterator );
	virtual void                                                findByArticleCode(basar::db::aspect::AccessorPropertyTable_YIterator );

	virtual basar::db::aspect::AccessorPropertyTableRef         get() const;
	virtual void                                                resetSearchYit();
	virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;
	void                                                        setSearchYit( basar::db::aspect::AccessorPropertyTable_YIterator yit );
	virtual bool                                                isEmpty() const;

    void setAreaID(basar::Int16 areaID);


private:
	/////////////////////////////////////////////////////////////////////////
    // private member methods
    /////////////////////////////////////////////////////////////////////////
	ArticleCollectionDM( const ArticleCollectionDM & );
	ArticleCollectionDM & operator = ( const ArticleCollectionDM & );

	libutil::domMod::SearchYIteratorPtr		        getSearchYIterator() const;
	libutil::infrastructure::accessor::IAccessorPtr getArticleCollectionAccessor() const;
	const log4cplus::Logger &				        getLogger() const;

    /////////////////////////////////////////////////////////////////////////
    // private member variables
    /////////////////////////////////////////////////////////////////////////
	const log4cplus::Logger 				                m_Logger;
	
	mutable libutil::domMod::SearchYIteratorPtr		        m_SearchYIterator;
	libutil::infrastructure::accessor::IAccessorPtr         m_Accessor;
	basar::Int16									m_AreaID;
};

} //end namespace article
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_ARTICLE_ARTICLECOLLECTIONDM_H
