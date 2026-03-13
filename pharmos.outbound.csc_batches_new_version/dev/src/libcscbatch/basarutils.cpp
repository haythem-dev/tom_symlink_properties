//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <string>
#include "basarutils.h"
#include <libbasardbaspect_accessor.h>
#include <libbasarcmnutil_bstring.h> 
#include <libbasarproperty_propertytable_xiterator.h>
#include <libbasardbaspect_connectionref.h>
#include <libbasardbsql_connectionref.h>
#include <libbasarproperty_propertystate.h>
#include <libbasarlogin.h>

namespace nsBasarUtils {
	//**************************************************************************************************
	//         Konstruktoren / Destruktoren
	//**************************************************************************************************
	
	// no constructors and no destructors, because all functions are static

	//**************************************************************************************************
	//         Methoden
	//**************************************************************************************************
	//************************************ replaceString ************************************************
	//
	// Descr.: Replaces findStr by replaceStr in sourceStr
	//
	//***********************************************************************************************************
	basar::I18nString BasarUtils::replaceString(basar::I18nString sourceStr, basar::I18nString findStr, basar::I18nString replaceStr) {
		try {
			basar::I18nString string="";
			//returns the first occurrence of str within the current string, starting at index, string::npos if nothing is found, 
//size_type find( const string& str, size_type index );

			size_t pos = sourceStr.find(findStr,0);
			if(pos==std::string::npos) {
				return("");
			}
			// replaces characters of the current string with up to num characters from str, beginning at index, 
			sourceStr.replace(pos, findStr.length(), replaceStr);
//				string& replace(size_type index, size_type num, const string& str );
			return sourceStr;

		}
		catch(...) {
//			BLOG_ERROR(LoggerPool::loggerUseCases, "BatchOrderUC::createYitString()."); 
			throw; 
		}// catch
	}// replaceString

	//************************************ createYitString ************************************************
	//
	// Descr.:	Gets all the fields out of string "fieldsSrc" and puts them into a string ready to create
	//			a yit. The values are taken from yitSrc. New property names can be given by fieldsDest.
	// Param.:	The fields in string "fieldsSrc" and "fieldsDest" must be separated by ';' 
	//			and there must be a ';' at the end!
	// Ret.:	"" on error
	//
	//***********************************************************************************************************
	basar::I18nString BasarUtils::createYitString(basar::db::aspect::AccessorPropertyTable_YIterator yitSrc, basar::I18nString fieldsSrc, basar::I18nString fieldsDest, basar::I18nString& strYit) {
/*!
\todo Parameter strYit is not necessary!
*/
		try {
			strYit = "";
			basar::I18nString strYit1 = "";
			//--- split fieldsSrc delimeted by ';' into single fieldsSrc and build a string for creating a yit
			int startSrc=0;
			int startDest=0;
			unsigned int j=0;
			bool stopped=false;
			char delimiter=';';
			basar::I18nString fieldSrc = "";
			basar::I18nString fieldDest = "";
			for(unsigned int i=0; i<fieldsSrc.length(); i++) {
				if(fieldsSrc[i] == delimiter) {
					fieldSrc = fieldsSrc.substr(startSrc, i - startSrc);
					startSrc=i+1;
					fieldDest = "";
					j=startDest;
					stopped=false;
					while((j<fieldsDest.length()) && !stopped) {
						if(fieldsDest[j] == delimiter) {
							fieldDest = fieldsDest.substr(startDest, j - startDest);
							startDest=j+1;
							stopped=true;
						}
						j++;
					}// for j
					if(""==fieldDest) { // no correspondent field in destination found
						return "";
					}
					if(nsBasarUtils::BasarUtils::isPropSet(yitSrc,fieldSrc)) {  // selection is unset
						strYit1.format("%s=%s;", fieldDest.c_str(), yitSrc.getSQLString(fieldSrc).c_str());
						strYit.append(strYit1);						
					}
				} //if(fieldsSrc[i] == delimiter)
			}// for i
			return strYit;
		}// try
		catch(...) {
//			BLOG_ERROR(LoggerPool::loggerUseCases, "BatchOrderUC::createYitString()."); 
			throw; 
		}// catch
	}// createYitString

	//************************************ createYitString ************************************************
	//
	// Descr.:	Gets all the fields out of string "fields" and puts them into a string ready to create
	//			a yit. The values are taken from yitSrc. The field names given by fields are maintained.
	// Param.:	The fields in string "fields" must be separated by ';' and there must be a ';' at the end!
	//
	//***********************************************************************************************************
	basar::I18nString BasarUtils::createYitString(basar::db::aspect::AccessorPropertyTable_YIterator yitSrc, basar::I18nString fields) {
		try {
			basar::I18nString strYit = "";
			basar::I18nString strYit1 = "";
			//--- split fields delimeted by ';' into single fields and build a string for creating a yit
			int start=0;
			char delimiter=';';
			basar::I18nString field = "";
			for(unsigned int i=0; i<fields.length(); i++) {
				if(fields[i] == delimiter) {
					field = fields.substr(start, i - start);
					start=i+1;
					if(nsBasarUtils::BasarUtils::isPropSet(yitSrc,field)) {  // selection is unset
						strYit1.format("%s=%s;", field.c_str(), yitSrc.getSQLString(field).c_str());
						strYit.append(strYit1);						
					}
				} //if(fields[i] == delimiter)
			}// for i
			return strYit;
		}// try
		catch(...) {
//			BLOG_ERROR(LoggerPool::loggerUseCases, "BatchOrderUC::createYitString()."); 
			throw; 
		}// catch
	}// createYitString

	//************************************ createYitString ************************************************
	//
	// Descr.:	Gets all the fields out of string "fields" and puts them into a string ready to create
	//			a yit. The values are taken from yitSrc. The field names given by fields are maintained.
	// Param.:	The fields in string "fields" must be separated by ';' and there must be a ';' at the end!
	//
	//***********************************************************************************************************
	basar::I18nString BasarUtils::createYitString(basar::db::aspect::AccessorPropertyTable_YIterator yitSrc) {
		try {
			basar::db::aspect::AccessorPropertyTable_XIterator xit;
			basar::I18nString fieldnames="";
			xit = yitSrc.begin();  // gibt den XIterator zurück
			while(!xit.isEnd())	{
				if (yitSrc.isContainedAndSet(xit.getName())) {
					fieldnames += xit.getName()+";";
				}// if
				xit++;
			}// while(!xit.isEnd())
			return createYitString(yitSrc,fieldnames);
		}// try
		catch(...) {
//			BLOG_ERROR(LoggerPool::loggerUseCases, "BatchOrderUC::createYitString()."); 
			throw; 
		}// catch
	}// createYitString

	//************************************ isPropSet ************************************************
	bool BasarUtils::isPropSet(basar::db::aspect::AccessorPropertyTable_YIterator yit, basar::VarString fieldname) {
		try {
			if(yit.isNull()) 
				return false;
			if(yit.isEnd())
				return false;
			basar::I18nString str="";
			if(yit.isContainedAndSet(fieldname)) {
				str=yit.getSQLString(fieldname);
				str.trim(basar::cmnutil::BString::FIRST_END);
				if("" != str) 
					return true;
			}// if(yit.isContainedAndSet(fieldname))
			return false;
		}// try
		catch(...) {
//			BLOG_ERROR(LoggerPool::loggerUseCases, "BasarUtils::isPropSet()."); 
			throw;
		}// catch
	}// isPropSet

	//*************************************** setYitToState ***********************************************
	//
	// Descr.:	Sets all properties of yit to State state.
	//
	//***************************************************************************************************************
	basar::appl::EventReturnStruct BasarUtils::setYitToState(AccessorPropertyTable_YIterator &yit, basar::db::aspect::PropertyState pState) {
		basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);
		basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin();  
		while(!xit.isEnd())	{
			yit.setPropertyState(xit.getName(), pState.getState());
			xit++;
		}// while(!xit.isEnd())	{
		return basar::appl::HANDLER_OK;
	}// setYitToState

	//************************************ reformatDate ************************************************
	//
	// Format "__yymmdd" means: "yyyymmdd" oder "yymmdd"
	//
	//***************************************************************************************************************
	basar::I18nString BasarUtils::reformatDate(basar::I18nString const srcStr, QString const srcFormat, QString const destFormat) {
		basar::I18nString newStr="";
		if((srcFormat=="yyyymmdd") || ((srcFormat=="__yymmdd"))) {  
			if((srcStr.length()!=6) && (srcStr.length()!=8))
				return "";
			if(destFormat=="ddmmyyyy") {
				if(srcStr.length()==6) { // srcFormat="yymmdd"
					newStr.append(srcStr.substr(4,2));
					newStr.append(srcStr.substr(2,2));
					newStr.append("20");
					newStr.append(srcStr.substr(0,2));
				}// if(srcStr.length()==6)
				if(srcStr.length()==8) { // srcFormat="yyyymmdd"
					newStr.append(srcStr.substr(6,2));
					newStr.append(srcStr.substr(4,2));
					newStr.append(srcStr.substr(0,4));
				}// if(srcStr.length()==8)
			}// if(destFormat=="ddmmyyyy")
			if(destFormat=="ddmm__yy") {
				if(srcStr.length()==6) { // srcFormat="yymmdd"
					newStr.append(srcStr.substr(4,2));
					newStr.append(srcStr.substr(2,2));
					newStr.append(srcStr.substr(0,2));
				}// if(srcStr.length()==6)
				if(srcStr.length()==8) { // srcFormat="yyyymmdd"
					newStr.append(srcStr.substr(6,2));
					newStr.append(srcStr.substr(4,2));
					newStr.append(srcStr.substr(2,2));
				}// if(srcStr.length()==8)
			}// if(destFormat=="ddmm__yy")
		}// if(srcFormat=="__yymmdd")

		if((srcFormat=="ddmmyyyy") || ((srcFormat=="ddmm__yy"))) {
			if((srcStr.length()!=6) && (srcStr.length()!=8))
				return "";
			if(destFormat=="yyyymmdd") {
				if(srcStr.length()==6) { // srcFormat="ddmmyy"
					newStr.append("20");
					newStr.append(srcStr.substr(4,2));
					newStr.append(srcStr.substr(2,2));
					newStr.append(srcStr.substr(0,2));
				}// if(srcStr.length()==6)
				if(srcStr.length()==8) { // srcFormat="ddmmyyyy"
					newStr.append(srcStr.substr(4,4));
					newStr.append(srcStr.substr(2,2));
					newStr.append(srcStr.substr(0,2));
				}// if(srcStr.length()==8)
			}// if(destFormat=="yyyymmdd")
			if(destFormat=="__yymmdd") {
				if(srcStr.length()==6) { // srcFormat="ddmmyy"
					newStr.append(srcStr.substr(4,2));
					newStr.append(srcStr.substr(2,2));
					newStr.append(srcStr.substr(0,2));
				}// if(srcStr.length()==6)
				if(srcStr.length()==8) { // srcFormat="ddmmyyyy"
					newStr.append(srcStr.substr(6,2));
					newStr.append(srcStr.substr(2,2));
					newStr.append(srcStr.substr(0,2));
				}// if(srcStr.length()==8)
			}// if(destFormat=="__yymmdd")
		}// if(srcFormat=="ddmm__yy")
		return newStr;
	}// reformatDate

	//*************************************** beginTransaction *****************************************************
	void BasarUtils::beginTransaction(const AccessorInstanceRef acc) {
		try {		
			basar::db::sql::ConnectionRef SqlConn=acc.getConnection().getSqlConnection();
			#ifndef _UNITTEST
				if(!SqlConn.isInTransaction())
					SqlConn.begin();
			#endif
		}// try
		catch(...) {
			throw; 
		}// catch
	}// beginTransaction

	//*************************************** commitTransaction *****************************************************
	void BasarUtils::commitTransaction(const AccessorInstanceRef acc) {
		try {		
			basar::db::sql::ConnectionRef SqlConn=acc.getConnection().getSqlConnection();
			#ifndef _UNITTEST
				if(SqlConn.isInTransaction())
					SqlConn.commit();
			#endif
		}// try
		catch(...) {
			throw; 
		}// catch
	}// commitTransaction

	//*************************************** rollbackTransaction *****************************************************
	void BasarUtils::rollbackTransaction(const AccessorInstanceRef acc) {
		try {		
			basar::db::sql::ConnectionRef SqlConn=acc.getConnection().getSqlConnection();
			#ifndef _UNITTEST
				if(SqlConn.isInTransaction())
					SqlConn.rollback();
			#endif
		}// try
		catch(...) {
			throw; 
		}// catch
	}// rollbackTransaction


	//******************************************************************************************************************

	static basar::ConstString GROUP_STR_AUSTRIA					= "AT";
	static basar::ConstString GROUP_STR_AUSTRIA_PREWHOLE		= "PAT";
	static basar::ConstString GROUP_STR_BULGARIA				= "BG";	
	static basar::ConstString GROUP_STR_CROATIA					= "HR";
	static basar::ConstString GROUP_STR_GERMANY					= "DE";
	static basar::ConstString GROUP_STR_GERMANY_N				= "DE1";
	static basar::ConstString GROUP_STR_GERMANY_S				= "DE2";
	static basar::ConstString GROUP_STR_FRANCE					= "FR";
	static basar::ConstString GROUP_STR_FRANCE_PREWHOLE			= "PFR";
	static basar::ConstString GROUP_STR_SERBIA					= "RS";
	static basar::ConstString GROUP_STR_SWITZERLAND				= "CH";
	static basar::ConstString GROUP_STR_SWITZERLAND_PREWHOLE	= "PCH";

	CountryEnum getCountry()
	{
		static CountryEnum s_Country = COUNTRY_UNKNOWN;

		if (COUNTRY_UNKNOWN == s_Country)
		{
			basar::login::Manager::getInstance();
			basar::Int16 branchNo = basar::login::Manager::getInstance().getAreaID();

			// get user´s branch he logged in for to get area-specific database
			basar::I18nString branchStr;
			branchStr.format("%d", branchNo);

			// get GROUP parameter
			basar::I18nString group="";
			basar::login::Manager::getInstance().getParaValue(branchStr, UADM_GROUP, group);

			if (GROUP_STR_AUSTRIA == group || GROUP_STR_AUSTRIA_PREWHOLE == group)
			{
				s_Country = COUNTRY_AUSTRIA;
			}
			else if (GROUP_STR_BULGARIA == group)
			{
				s_Country = COUNTRY_BULGARIA;
			}
			else if (GROUP_STR_CROATIA == group)
			{
				s_Country = COUNTRY_CROATIA;
			}
			else if (GROUP_STR_GERMANY_N == group || GROUP_STR_GERMANY_S == group)
			{
				s_Country = COUNTRY_GERMANY;
			}
			else if (GROUP_STR_FRANCE == group  || GROUP_STR_FRANCE_PREWHOLE == group)
			{
				s_Country = COUNTRY_FRANCE;
			}
			else if (GROUP_STR_SERBIA == group)
			{
				s_Country = COUNTRY_SERBIA;
			}
			else if (GROUP_STR_SWITZERLAND == group || GROUP_STR_SWITZERLAND_PREWHOLE == group)
			{
				s_Country = COUNTRY_SWITZERLAND;
			}
		}
		return s_Country;
	}

	bool isAustria()		{ return getCountry() == COUNTRY_AUSTRIA; }
	bool isBulgaria()		{ return getCountry() == COUNTRY_BULGARIA; }
	bool isCroatia()		{ return getCountry() == COUNTRY_CROATIA; }
	bool isFrance()			{ return getCountry() == COUNTRY_FRANCE; }
	bool isGermany()		{ return getCountry() == COUNTRY_GERMANY; }
	bool isSerbia()			{ return getCountry() == COUNTRY_SERBIA; }
	bool isSwitzerland()	{ return getCountry() == COUNTRY_SWITZERLAND; }

}// namespace nsBasarUtils