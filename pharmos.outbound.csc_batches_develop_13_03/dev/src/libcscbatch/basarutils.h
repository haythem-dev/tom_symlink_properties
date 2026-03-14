//-------------------------------------------------------------------------------------------------//
/*! \file	basarutils.h
 *  \brief  useful GUI functions
 *  \author Beatrix Trömel
 *  \date   16.06.2008
 */
//-------------------------------------------------------------------------------------------------//
#ifndef BASARUTILS_H
#define BASARUTILS_H

#include <libbasar_definitions.h>
#include <libbasarappl.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <QtCore/QString>
#include "libcscbatch_definitions.h"

//-------------------------------------------------------------------------------------------------//
// usings
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorInstanceRef;

namespace nsBasarUtils {
	class BasarUtils {
	//**************************************************************************************************
	//         public
	//**************************************************************************************************
	public:
		//**************************************************************************************************
		/*! \brief Replaces findStr by replaceStr in sourceStr
			\param sourceStr string in which replacement takes part
			\param findStr string which is searched for in sourceStr
			\param replaceStr string by findStr is replaced
			\return string with replaced string
			\throw no-throw 
		*/
		//**************************************************************************************************
		static basar::I18nString replaceString(basar::I18nString sourceStr, basar::I18nString findStr, basar::I18nString replaceStr);
		
		//**************************************************************************************************
		/*! \brief Creates a yitString (a string ready to create a yit) from a given yit.
			\param yitSrc yit as a source for the new yit
			\param fieldsSrc string which contains all properties (separated by ';') from yitSrc to be copied into new yit; there must be a ';' at the end of the string!
			\param fieldsDest string representing the new names of the properties (separated by ';') in the new yit; there must be a ';' at the end of the string!
			\return strYit string ready to create a new yit with; "" on error
			\throw no-throw 
		*/
		//**************************************************************************************************
		static basar::I18nString createYitString(basar::db::aspect::AccessorPropertyTable_YIterator yitSrc, basar::I18nString fieldsSrc, basar::I18nString fieldsDest, basar::I18nString& strYit);
		
		//**************************************************************************************************
		/*! \brief Creates a yitString (a string ready to create a yit) from a given yit.
			\param yitSrc yit as a source for the new yit
			\param fields string which contains all properties (separated by ';') from yitSrc to be copied into new yit; there must be a ';' at the end of the string!
			\return strYit string ready to create a new yit with; "" on error
			\throw no-throw 
		*/
		//**************************************************************************************************
		static basar::I18nString createYitString(basar::db::aspect::AccessorPropertyTable_YIterator yitSrc, basar::I18nString fields);

		//**************************************************************************************************
		/*! \brief Creates a yitString (a string ready to create a yit) from a given yit.
			\param yitSrc yit as a source for the new yit
			\return strYit string ready to create a new yit with; "" on error
			\throw no-throw 
		*/
		//**************************************************************************************************
		static basar::I18nString createYitString(basar::db::aspect::AccessorPropertyTable_YIterator yitSrc);

		//**************************************************************************************************
		/*! \brief Checks whether property fieldname is set in the given yit. String-Properties have to be != "". Otherwise they're unset.
			\param yit yit which is searched in
			\param fieldname property which is looked for
			\return true, if fieldname is found in yit\n
					false otherwise
			\throw no-throw 
		*/
		//**************************************************************************************************
		static bool isPropSet(basar::db::aspect::AccessorPropertyTable_YIterator yit, basar::VarString fieldname);

		//**************************************************************************************************
		/*! \brief Sets all properties of yit to State pState.
			\param yit in this yit the state is changed; parameter will be changed
			\param pState this is the new state
			\return basar::appl::HANDLER_OK, if everything is ok\n
					basar::appl::HANDLER_ERROR on error
			\throw no-throw 
		*/
		//**************************************************************************************************
		static basar::appl::EventReturnStruct setYitToState(AccessorPropertyTable_YIterator &yit, basar::db::aspect::PropertyState pState);
		
		//**************************************************************************************************
		/*! \brief Reformats a date given as string
			\param srcStr string that holds the date
			\param srcFormat string that holds the format of the given date-string, e.g. "yyyymmdd"
			\param destFormat string that holds the destination format of new string, e.g. "ddmmyyyy"
			\returns the reformatted string
			\throw no-throw 
		*/
		//**************************************************************************************************
		static basar::I18nString reformatDate(basar::I18nString const srcStr, QString const srcFormat, QString const destFormat);
		
		//**************************************************************************************************
		/*! \brief Begins a transaction, if configuration is NOT set to _UNITTEST
			\param acc accessor, that holds the database connection
			\throw no-throw 
		*/
		//**************************************************************************************************
		static void beginTransaction(const AccessorInstanceRef acc);
		
		//**************************************************************************************************
		/*! \brief Commits a transaction, if configuration is NOT set to _UNITTEST
			\param acc accessor, that holds the database connection
			\throw no-throw 
		*/
		//**************************************************************************************************
		static void commitTransaction(const AccessorInstanceRef acc);
		
		//**************************************************************************************************
		/*! \brief Rolls back a transaction, if configuration is NOT set to _UNITTEST
			\param acc accessor, that holds the database connection
			\throw no-throw 
		*/
		//**************************************************************************************************
		static void rollbackTransaction(const AccessorInstanceRef acc);

	//**************************************************************************************************
	//         private
	//**************************************************************************************************
	};


	CountryEnum	getCountry();
	bool		isAustria();
	bool		isBulgaria();
	bool		isCroatia();
	bool		isFrance();	
	bool		isGermany();
	bool		isSerbia();
	bool		isSwitzerland();

}// namespace nsBasarUtils
#endif
