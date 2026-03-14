#ifndef GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLE_H
#define GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLE_H

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTableRef;
			class AccessorPropertyTable_YIterator;
        }
    }
}

//----------------------------------------------------------------------------//
// class declaration section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace article
{
    class IFindArticle
    {
        public:	
            virtual ~IFindArticle(){}

            virtual void                                                resetSearchYit()                                                   = 0;
            virtual basar::db::aspect::AccessorPropertyTable_YIterator	getSearchYit()                                                     = 0;
			virtual void			                                    findArticle( basar::db::aspect::AccessorPropertyTable_YIterator )  = 0;
            virtual basar::db::aspect::AccessorPropertyTableRef			get()                                                              = 0;
            virtual void                                                clear()                                                            = 0;
    };
} 
} 
}

#endif // GUARD_LIBABBAUW_DOMMOD_ARTICLE_IFINDARTICLE_H
