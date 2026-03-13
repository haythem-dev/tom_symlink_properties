//-------------------------------------------------------------------------------------
/*! \file  linetypeenum.h
*  \brief  Enumeration of all line types. 
*  \author Jia Li, P. Kessling
*  \date   02.01.2013
*	history:
*			05.04.13 pk redo
*			27.10.14 pk integrating DE line types
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_LINETPYEENUM_H
#define GUARD_DOMMOD_LINE_LINETPYEENUM_H

#include <libbasarcmnutil_i18nstring.h>
#include <libbasar_definitions.h>
#include <map>

namespace domMod
{
	namespace line
	{
		// length of edifact line identifier, all edifact line identifiers e.g. "NAD" are 3 chars long.
		static const basar::Int16		EDILINE_LENGTH_IDF						= 3;						// for CH
		static const basar::Int16		EDILINE_LENGTH_IDF_DE					= 2;						// for DE

		// line identifier for orders that have been read and mapped correctly. Overwrites the original "UNB"
		// for CH
		static const basar::I18nString	EDILINE_IDF_ORDER_WAS_READ_CH			= "XNB";
		static const basar::I18nString	EDILINE_BON_ORDER_WAS_READ				= "BON";		// "Batch Order Number"
		static const basar::Int32		EDILINE_UNB_OFFSET_WHOLESALER_EAN		= 17;
		static const basar::Int32		EDILINE_LENGTH_EAN						= 14;

		// for DE
		static const basar::I18nString	EDILINE_IDF_ORDER_WAS_READ_DE			= "X";
		static const basar::I18nString	EDILINE_IDF_ORDER_WAS_IGNORED_DE		= "Y";
		static const basar::Int32		EDILINE_UNB_00_OFFSET_UNB_FILLER1		= 15;
		static const basar::Int32		EDILINE_LENGTH_UNB_FILLER1				= 14;


		// Types of Edifact-flatfile-lines
		enum LineTypeEnum 
		{
			enUNKNOWN,		// lineType is not known yet

			// line types for .uewauf-files for CH
			enUNB,
			enUNH,
			enBGM,
			enDTM,
			enNAD,
			enUNS,
			enLIN,
			enIMD,
			enQTY,
			enUNT,
			enUNZ,
			enPCD,

			// line types for .inh-files for DE
			en00,			// UNB
			en01,			// UNH
			en02,			// BGM
			en03,			// DTM
			en04,			// NAD
			en05,			// ALC
			en06,			// PCD
			en08,			// LIN
			en09,			// IMD
			en10,			// QTY
			en11,			// FTX
			en97,			// UNS

			enIRRELEVANT,	// lineType is well known but irrelevant to ABBA-UW (TDT, LOC, FTX, 11, 09, RFF, PRI, MOA, 98, 99)
			enOTHER			// lineType is something else, not known to ABBA-UW, therefore suspicious
		};


	} // namespace line


	typedef std::map< const domMod::line::LineTypeEnum, std::string > EdiLineInfoMap;
	typedef EdiLineInfoMap::iterator EdiLineInfoMapIterator;


} // namespace domMod

#endif // GUARD_DOMMOD_LINE_LINETPYEENUM_H

