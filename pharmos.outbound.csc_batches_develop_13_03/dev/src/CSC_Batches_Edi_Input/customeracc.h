//-------------------------------------------------------------------------------------
/*! \file  customeracc.h
*  \brief  Accessordefinitions for reading customerdata from table kunde etc. 
*  \brief  Description of Accessors to DB
*  \author P. Kessling
*  \date   03.05.2013 
*  history: 03.05.13 pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_ABBAUWDM_ACCCUSTOMER_H
#define GUARD_DOMMOD_ABBAUWDM_ACCCUSTOMER_H


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
		namespace accCustomer
		{
		//-------------------------------------------------------------------------------------------------//
		// sqlstring builder declaration section
		//-------------------------------------------------------------------------------------------------//

			BEGIN_QUERY_BUILDER_DECLARATION( SelCustomergroupByBranchnoAndPharmacynoBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION( SelPharmacynoByBranchnoAndPharmacynoeanBuilder )
			END_BUILDER_DECLARATION

			BEGIN_QUERY_BUILDER_DECLARATION( SelPseudopharmacynoByBranchnoBuilder )
			END_BUILDER_DECLARATION


		} // namespace accCustomer
	} // namespace abbauwDM
} // namespace domMod

#endif // GUARD_DOMMOD_ABBAUWDM_ACCCUSTOMER_H
