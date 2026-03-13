//-------------------------------------------------------------------------------------
/*! \file  articleacc.h
*  \brief  Accessordefinitions for reading articledata from tables 
*  \brief  artikelzentral, artikelpreis. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   22.04.2013 
*  history: 22.04.13 pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_ABBAUWDM_ACCARTICLE_H
#define GUARD_DOMMOD_ABBAUWDM_ACCARTICLE_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasardbaspect_macros.h>
#include <libbasardbaspect_definitions.h>
#include <libbasardbaspect_sqlstringbuilder.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//

namespace domMod
{
	namespace abbauwDM
	{
		namespace accArticle
		{
		//-------------------------------------------------------------------------------------------------//
		// sqlstring builder declaration section
		//-------------------------------------------------------------------------------------------------//

			BEGIN_QUERY_BUILDER_DECLARATION( SelManufacturernoByArticlenoBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION( SelArticlenoInternByArticleMFBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION( SelWholesalerpurchasepriceByArticlenoBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION( SelPreferredArticleCodeByArticleCodeAndCodeTypeBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION( SelPreferredArticlenoByPreferredArticleCodeBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION( SelPreferredArticleCodeByArticlenoInternBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION(SelCMIArticlesBuilder)
			END_BUILDER_DECLARATION

		} // namespace accArticle
	} // namespace abbauwDM
} // namespace domMod

#endif // GUARD_DOMMOD_ABBAUWDM_ACCARTICLE_H
