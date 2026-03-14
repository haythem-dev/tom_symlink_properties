#ifndef GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLEPRICE_H
#define GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLEPRICE_H

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libabbauw
{
namespace domMod
{
namespace article
{

class IFindArticlePrice
{
public:
	virtual ~IFindArticlePrice() {}
	virtual basar::db::aspect::AccessorPropertyTable_YIterator   get()                                                                       = 0;

    virtual void                                                 findArticlePrice( basar::Int32 articleno )                                  = 0;
};

} // end namespace findpricec
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLEPRICE_H
