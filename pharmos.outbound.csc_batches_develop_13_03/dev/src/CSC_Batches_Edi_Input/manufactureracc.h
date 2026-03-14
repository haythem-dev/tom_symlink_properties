//-------------------------------------------------------------------------------------
/*! \file  manufactureracc.h
*  \brief  Accessordefinitions for reading manufacturerdata from table bliefer, liefer etc. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   13.06.2013 
*  history: 13.06.13 pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_ABBAUWDM_ACCMANUFACTURER_H
#define GUARD_DOMMOD_ABBAUWDM_ACCMANUFACTURER_H


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
		namespace accManufacturer
		{
		//-------------------------------------------------------------------------------------------------//
		// sqlstring builder declaration section
		//-------------------------------------------------------------------------------------------------//

			BEGIN_QUERY_BUILDER_DECLARATION( SelDeliverernoByManufacturereanBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION( SelManufacturernoByDeliverernoBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION( SelManufacturernameByManufacturernoBuilder )
			END_BUILDER_DECLARATION
			
		} // namespace accManufacturer
	} // namespace abbauwDM
} // namespace domMod

#endif // GUARD_DOMMOD_ABBAUWDM_ACCMANUFACTURER_H
