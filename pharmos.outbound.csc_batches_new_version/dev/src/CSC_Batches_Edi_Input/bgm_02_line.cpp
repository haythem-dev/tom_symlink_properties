//-------------------------------------------------------------------------------------
/*! \file  bgm_02_Line.cpp
*  \brief  subclass of EDIFACTLineBase for  German 02-Segment. 
*  \brief  Main purpose: Extract an 02-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 27.10.14 pk neu
*/
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <string>
#include <sstream>		// for stringstream
#include <iomanip>		// for stringstream
#include <exception>	// for exceptions
#include <classinfo_macros.h>	// for BLOG_TRACE_METHOD-Makros
#include <libbasarcmnutil_logging.h>


#include "loggerpool.h"	// for LoggerPool
#include "abbauwexception.h"
#include "unprocessableexception.h"
#include "bgm_02_line.h"

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
		const basar::I18nString	Bgm_02_Line::s_LineIndicator = "02";	
		const basar::UInt32		Bgm_02_Line::s_MinLength     = 5;	// 2+3=5 (incl. DOKCODE ohne OrdernoMf und Filler\r)
																	// Minimum length of netto data
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
																									
		Bgm_02_Line::Bgm_02_Line()
		{
			resetMembers();
		}

		Bgm_02_Line::~Bgm_02_Line(){}

		EDIFACTLineBasePtr Bgm_02_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Bgm_02_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Bgm_02_Line::getLineTypeEnum()		const
		{
			return en02;
		}

		const basar::I18nString& Bgm_02_Line::getOrdernoMF()	const
		{
			return m_OrdernoMF;		
		}


		bool	Bgm_02_Line::isDokcode640()		const
		{
			if (m_FixedDokCode == "640")		
			{
				return true;
			}

			return false;
		}
		bool	Bgm_02_Line::isDokcode130()		const
		{
			if (m_FixedDokCode == "130")		
			{
				return true;
			}

			return false;
		}

		// The BGM-Line has to be valid, otherwise the currently read order is not a correct transfer order.
		// In Germany: orders with "130" have to be ignored without programm abort
		bool	Bgm_02_Line::isValidLine() const
		{
			
			if (!isDokcode640() && !isDokcode130())
			{
				return false;
			}
			return true;
		}



		
		// ============================================
		// Structure of BGM_02_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "02640                                         4902482101                         jhgjgh              "
		//  ^ ^  ^                                        ^                                  ^     ^ 
		//  a b  c                                        d                                  e     x
		//
		// Field a= Line-Indicator		Offset  0, length  2
		// Field b= Dokcode fixed Text:	Offset  2, length  3  (mandatory, must be "640")
		// Field c= Filler  :			Offset  5, length 41  (optional, spaces or text to be ignored )		
		// Field d= OrdernoMF (DOKNUM): Offset  46, length 35 (optional, Orderno defined by and for manufacturer)
		// Field e= Filler  :			Offset  81, length 6  (optional, spacesor text to be ignored )
		// Field x= Filler  :			Offset  87, length 463(optional, spaces to fill up the line to 550 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Bgm_02_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Bgm_02_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "BGM-02-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No BGM-02-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				m_FixedDokCode	= lineStr.substr(  2,  3 );		// Extract field b

				// Extract optional OrdernoMF if existent
				if ( 46 < lineStr.length() ) {						// > 47 = s_MinLength + Filler c (41) + minimum 1 char for OrdernoMF
					m_OrdernoMF		= lineStr.substr(  46, 35 );	// Extract field d, may be shorter than 35 chars
																	// Then substr copies as many as possible
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable BGM_02 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Bgm_02_Line::resetMembers()
		{
			m_FixedDokCode	= "";		// field b
			m_OrdernoMF		= "";		// field d
		}

	} // namespace line
} // namespace domMod

