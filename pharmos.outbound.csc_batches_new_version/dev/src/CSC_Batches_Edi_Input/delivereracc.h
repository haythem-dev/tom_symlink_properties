//-------------------------------------------------------------------------------------
/*! \file  delivereracc.h
*  \brief  Accessordefinitions for reading customerdata from table filiale etc. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   25.11.14 
*  history: 25.11.14  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_ABBAUWDM_ACCDELIVERER_H
#define GUARD_DOMMOD_ABBAUWDM_ACCDELIVERER_H


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
		namespace accDeliverer
		{
		//-------------------------------------------------------------------------------------------------//
		// sqlstring builder declaration section
		//-------------------------------------------------------------------------------------------------//

			BEGIN_QUERY_BUILDER_DECLARATION( SelBranchnoByDelivererbgaBuilder )
			END_BUILDER_DECLARATION


		} // namespace accCustomer
	} // namespace abbauwDM
} // namespace domMod

#endif // GUARD_DOMMOD_ABBAUWDM_ACCDELIVERER_H
