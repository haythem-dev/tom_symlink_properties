//--------------------------------------------------------------------------------------------//
/*! \file	editransferordermapper_definitions.h
 *  \brief	Definitions necessary for transforming UEWAUF-flatfiles
 *  \author	Petra Kessling
 *  \date   04.04.13
 *  \version
 *  history 04.04.13 pk new
 *  16.10.13 pk new naming for flatfiles: uewauf.123456.UNIX
 */
//--------------------------------------------------------------------------------------------//

#ifndef GUARD_EDITRANSFERORDERMAPPER_DEFINITIONS_H
#define GUARD_EDITRANSFERORDERMAPPER_DEFINITIONS_H

#include <libbasarcmnutil_i18nstring.h>

namespace domMod {
namespace orderMapper {

	// CH: names of flatfiles are like "uewauf.xxxxxx.UNIX". "uewauf." and ".UNIX" is fixed text. "xxxxxx" is a string of 6 numbers (the "counter").
	// DE: names of flatfiles are like "uewauf.nd.YYYYMMDDhhmm.UNIX".
	//     "uewauf." and ".UNIX" is fixed text.
	//     "nd" or "sd" defines the region: NordDeutschland or SüdDeutschland
	//     "YYYYMMDDhhmm" is a string of 12 numbers with date and time(the "counter").
	static const basar::I18nString FLATFILE_NAME_PREFIX					= "uewauf.";
	static const basar::I18nString FLATFILE_NAME_REGION_ND				= "nd.";		// Bei Countrycode=DE1
	static const basar::I18nString FLATFILE_NAME_REGION_DC				= "dc.";		// Bei Countrycode=DE2
	static const basar::Int16      FLATFILE_NAME_REGION_LENGTH_DE		= 3;
	static const basar::Int16      FLATFILE_NAME_COUNTER_LENGTH_CH		= 6;
	static const basar::Int16      FLATFILE_NAME_COUNTER_LENGTH_CH_PINT	= 14;			// USED FOR PINT
	static const basar::Int16      FLATFILE_NAME_COUNTER_LENGTH_DE		= 16;			// Formerly: YYYYMMDDhhmm = 12 char, now: YYYYMMDDhhmmssuu =16 char
	static const basar::I18nString FLATFILE_NAME_POSTFIX				= ".UNIX";

	static const basar::I18nString FLATFILE_WAS_CORRECTLY_READ_EXTENTION= ".ok";
	static const basar::I18nString FLATFILE_WAS_NOT_CORRECTLY_READ_EXTENTION = ".ERROR";

	static const basar::Int32      FLATFILE_TO_BE_CLEANED_AFTER_DAYS	= 14;
	static const basar::Int32      SECONDS_OF_A_DAY						= 86400;


	// process parameter -forbranches: Separators for branchno list
	static const basar::I18nString PARAM_FORBRANCHNOS_SEPARATORS= ",";


	// Input-Types: Different fileformats that can be used to hand out transfer orders to the ABBA_UW software
	//              E.g. CH uses uewauf.123456.unix-EdiFactInhouseformat-Files, DE uses xxxx.inh EdifactInhouseformatfiles
	enum InputtypeEnum
	{
		enInputtype_UNKNOWN,		// Inputtype is not known yet

		enInputtype_inhCH,				// Inhouseformat CH
		enInputtype_inhDE,				// Inhouseformat DE

		enInputtype_OTHER			// lineType is something else, not known to ABBA-UW, therefore suspicious
	};

} // end namespace orderMapper
} // end namespace domMod

#endif // GUARD_EDITRANSFERORDERMAPPER_DEFINITIONS_H
