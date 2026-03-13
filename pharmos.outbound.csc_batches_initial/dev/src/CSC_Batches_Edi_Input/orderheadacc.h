//-------------------------------------------------------------------------------------
/*! \file  orderheadacc.h
*  \brief  Accessordefinitions for writing the transfer order head to table orderhead. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   18.04.2013 
*  history: 18.04.13 pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_ABBAUWDM_ACCORDERHEAD_H
#define GUARD_DOMMOD_ABBAUWDM_ACCORDERHEAD_H


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
		namespace accOrderhead
		{
		//-------------------------------------------------------------------------------------------------//
		// sqlstring builder declaration section
		//-------------------------------------------------------------------------------------------------//

			BEGIN_WRITE_BUILDER_DECLARATION( InsertIntoOrderheadBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION( SelMaxOrdernoByBranchnoBuilder )
			END_BUILDER_DECLARATION

		} // namespace accOrderhead

	} // namespace abbauwDM
} // namespace domMod

#endif // GUARD_DOMMOD_ABBAUWDM_ACCORDERHEAD_H
