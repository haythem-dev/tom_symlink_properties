//--------------------------------------------------------------------------------------------//
/*! \file	libabbauw_definitions.h
 *  \brief	Definitions necessary for libAbbauw
 *  \author	Petra Kessling
 *  \date   07.05.13
 *  \version
 *  history 07.05.13 pk new
 */
 //--------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBABBAUW_DEFINITIONS_H
#define GUARD_LIBABBAUW_DEFINITIONS_H

//--------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include <libbasarcmnutil_i18nstring.h>

//--------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------//
namespace libabbauw
{
	namespace lit
	{
		static const basar::I18nString OMG_ORDERTYPE = "OM";
		static const basar::I18nString OMG_ORDERPICKINGTYPE = "2";
		static const basar::I18nString OMG_BOOKENTRYTYPE = "2";
		static const basar::I18nString OMG_CUSTOMERGROUP = "18";

		static const basar::I18nString UW_HEADERTEXT_PREFIX = "UEWAUF ";
		static const basar::I18nString UW_ORDERTYPE = "UW";
		static const basar::I18nString UW_ORDERPICKINGTYPE = "0";
		static const basar::I18nString UW_BOOKENTRYTYPE = "0";

		static const basar::I18nString UN_ORDERTYPE = "UN";
		static const basar::I18nString NO_ORDERTYPE = "NO";
		static const basar::I18nString MD_ORDERTYPE = "MD";
		static const basar::I18nString AK_ORDERTYPE = "AK";
		static const basar::I18nString HZ_ORDERTYPE = "HZ";
		static const basar::I18nString DG_ORDERTYPE = "DG";
		static const basar::I18nString GM_ORDERTYPE = "GM";

		static const basar::I18nString DG_ORDERTYPECOMBINATION = "DG03";

		static const int PSEUDO_PHARMACYNO = 56265;

	} // end namespace lit
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DEFINITIONS_H
