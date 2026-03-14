//-------------------------------------------------------------------------------------------------//
/*! \file	lineidentifier.cpp
*  \brief   class for identification of a Edifact-flatfile-line.
*  \brief        Cannot be instantiated, only static methods.
 *  \author Jia Li, P. Kessling
 *  \date   07.01.2013
 *  history:
 *			05.04.13 pk redo
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <string>

#include <sstream>				// for stringstream
#include <iomanip>				// for stringstream
#include <classinfo_macros.h>	// for BLOG_TRACE_METHOD-Makros
#include <libbasarcmnutil_logging.h>
#include "loggerpool.h"			// for LoggerPool

#include <exception>	// for exceptions
#include <map>

#include "editransferordermapper_definitions.h"
#include "ediline_definitions.h"
#include "edilineidentifier.h"

//-------------------------------------------------------------------------------------------------//
// using declarations section
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//

namespace domMod 
{
	namespace line
	{
    //--------------------------------------------------------------------------------------------------//
    // static definition section
    //--------------------------------------------------------------------------------------------------//
		// definition static member
		std::map <std::string, LineTypeEnum> EdiLineIdentifier::s_mapLineInfo;

    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		//EdiLineIdentifier::EdiLineIdentifier(const basar::I18nString& Countrycode)
		EdiLineIdentifier::EdiLineIdentifier(const domMod::orderMapper::InputtypeEnum Inputtype)
		{
			m_Inputtype = Inputtype;			

			initLineTypeMap();
		}

		EdiLineIdentifier::~EdiLineIdentifier()
		{
		}



		// Finds the Enum for the lineTypeString, e.g. finds enNAD for "NAD"
		// lineTypeStr must be identical (3 char) to identifier-String
		LineTypeEnum	EdiLineIdentifier::findEnumByStr	( const std::string& lineTypeStr ) const
		{
			METHODNAME_DEF( EdiLineIdentifier, findEnumByStr ) 
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				std::map <std::string, LineTypeEnum>::iterator itr;

				itr = s_mapLineInfo.find(lineTypeStr);	

				if ( itr == s_mapLineInfo.end())
				{
					// did not find type
					return enOTHER;
				}
				else
				{
					return itr->second;
				}
				
				// The search for the key-elem is much faster with find()
				//std::map <std::string, LineTypeEnum>::iterator itr;
				//for(itr = s_mapLineInfo.begin(); itr != s_mapLineInfo.end(); ++itr)
				//{
				//	if ( lineTypeStr == itr->first )				
				//	{
				//		// type found
				//		return itr->second;
				//	}
				//}
			}
			catch (...)
			{
				std::stringstream str;
				str << "Error in " << fun << " Unknown exception! " << std::endl;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

				return enUNKNOWN;

			}
		}

		// Finds the lineTypeString  for the Enum, e.g. finds "NAD" for enNAD
		std::string		EdiLineIdentifier::findStrByEnum	( const LineTypeEnum lineTypeEnum ) const
		{
			METHODNAME_DEF( EdiLineIdentifier, findStrByEnum ) 
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				std::map <std::string, LineTypeEnum>::iterator itr;
				for(itr = s_mapLineInfo.begin(); itr != s_mapLineInfo.end(); ++itr)
				{
					if ( lineTypeEnum == itr->second )				
					{
						// type found
						return itr->first;
					}
				}
			}
			catch (...)
			{
				std::stringstream str;
				str << "Error in " << fun << " Unknown exception! " << std::endl;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			}

			// did not find type
			return "";
		}


		// The first 2 resp. 3 chars of an ediline contain its identifier, e.g. "04" resp. "NAD".
		// Finds the Enum for the lineTypeString, e.g. finds enNAD for complete NAD-Line
		LineTypeEnum	EdiLineIdentifier::identifyEdiLine	( const std::string& line ) const
		{
			METHODNAME_DEF( EdiLineIdentifier, identifyEdiLine ) 
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				std::string lineTypeStr("");

				if (domMod::orderMapper::enInputtype_inhCH == m_Inputtype )
				{
					lineTypeStr = line.substr(0,EDILINE_LENGTH_IDF);
				}
				else if (domMod::orderMapper::enInputtype_inhDE == m_Inputtype )
				{
					lineTypeStr = line.substr(0,EDILINE_LENGTH_IDF_DE);
				}
				else
				{
					std::stringstream str;
					str << "Error in " << fun << " Unknown Inputtype! " << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return enUNKNOWN;
				}

				return findEnumByStr (lineTypeStr) ;
			}
			catch (...)
			{
				std::stringstream str;
				str << "Error in " << fun << " Unknown exception! " << std::endl;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	
				return enUNKNOWN;
			}
		}



		// Inits the private and static vector 
		void EdiLineIdentifier::initLineTypeMap()
		{
			METHODNAME_DEF( EdiLineIdentifier, initLineTypeMap ) 
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// initialize only once
				if ( s_mapLineInfo.empty() )
				{
					//if (enCountrycode_CH == m_Countrycode )
					if (domMod::orderMapper::enInputtype_inhCH == m_Inputtype)
					{

						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("UNB",enUNB));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("UNH",enUNH));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("BGM",enBGM));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("DTM",enDTM));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("NAD",enNAD));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("UNS",enUNS));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("LIN",enLIN));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("IMD",enIMD));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("QTY",enQTY));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("UNT",enUNT));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("UNZ",enUNZ));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("PCD",enPCD));

						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("TDT",enIRRELEVANT));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("LOC",enIRRELEVANT));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("FTX",enIRRELEVANT));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("RFF",enIRRELEVANT));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("PRI",enIRRELEVANT));
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("MOA",enIRRELEVANT));
						
					}
					//else if (enCountrycode_DE == m_Countrycode )
					else if (domMod::orderMapper::enInputtype_inhDE == m_Inputtype)
					{

						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("00",en00));			// UNB
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("01",en01));			// UNH
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("02",en02));			// BGM
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("03",en03));			// DTM
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("04",en04));			// NAD
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("05",en05));			// ALC
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("06",en06));			// PCD	
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("08",en08));			// LIN
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("10",en10));			// QTY
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("11",en11));			// FTX
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("97",en97));			// UNS


						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("07",enIRRELEVANT));		// MOA
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("09",enIRRELEVANT));		// IMD
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("12",enIRRELEVANT));		// PRI
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("98",enIRRELEVANT));		// UNT
						s_mapLineInfo.insert (std::map<std::string, LineTypeEnum >::value_type("99",enIRRELEVANT));		// UNZ
					}
					else
					{
						std::stringstream str;
						str << "Error in " << fun << " Unknown Inputtype! " << std::endl;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return;
					}


				}
			}
			catch (...)
			{
				std::stringstream str;
				str << "Error in " << fun << " Unknown exception! " << std::endl;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

				// This is a serious error
				throw;
			}

		}



	}  // namespace line
}  // namespace domMod
