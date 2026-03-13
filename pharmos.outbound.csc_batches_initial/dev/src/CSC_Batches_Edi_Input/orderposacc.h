//-------------------------------------------------------------------------------------
/*! \file  orderposacc.h
*  \brief  Accessordefinitions for writing transfer order positions to table orderpos. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   30.04.2013 
*  history: 30.04.13 pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_ABBAUWDM_ACCORDERPOS_H
#define GUARD_DOMMOD_ABBAUWDM_ACCORDERPOS_H

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
		namespace accOrderpos
		{
		//-------------------------------------------------------------------------------------------------//
		// sqlstring builder declaration section
		//-------------------------------------------------------------------------------------------------//

			BEGIN_WRITE_BUILDER_DECLARATION( InsertIntoOrderposBuilder )
			END_BUILDER_DECLARATION


		} // namespace accOrderpos

	} // namespace abbauwDM
} // namespace domMod

#endif // GUARD_DOMMOD_ABBAUWDM_ACCORDERPOS_H
