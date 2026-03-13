#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_ARTICLECOLLECTIONACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_ARTICLECOLLECTIONACC_DEFINITIONS_H

#include <libbasarcmnutil_parameterlist.h>

namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
			namespace article
			{
				namespace lit
				{
					const basar::VarString ARTICLE_COLLECTION_ACC    ( "AccTenderArticle" );
					const basar::VarString SELECT_ARTICLE_BY_PATTERN ( "SelectArticleByPattern" );
                    const basar::VarString SELECT_ARTICLE_BY_PATTERN_RS ( "SelectArticleByPatternRS" );
					const basar::VarString SELECT_ARTICLE_BY_ARTICLE_CODE ( "SelectArticleByArticleCode" );
                    const basar::VarString SELECT_ARTICLE_BY_ARTICLE_CODE_RS ( "SelectArticleByArticleCodeRS" );
				}
			}
		}
	}
}

#endif // end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_ARTICLECOLLECTIONACC_DEFINITIONS_H
