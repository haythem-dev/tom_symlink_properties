//-------------------------------------------------------------------------------------
/*! \file  bgmline.cpp
*  \brief  subclass of EDIFACTLineBase for BGM-Segment. 
*  \brief  Main purpose: Extract an BGM-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author Jia Li ,P. Kessling
*  \date   02.01.2013
*  history 22.03.13 pk rework
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
#include "bgmline.h"

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
		const basar::I18nString	BgmLine::s_LineIndicator = "BGM";	
		const basar::UInt32		BgmLine::s_MinLength     = 6;	// 3+3=6 (incl. DOKCODE ohne OrdernoMf und \n\r)
																// Minimum length of netto data
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
																									
		BgmLine::BgmLine()
		{
			resetMembers();
		}

		BgmLine::~BgmLine(){}

		EDIFACTLineBasePtr BgmLine::clone() const 
		{
			return EDIFACTLineBasePtr (new BgmLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		BgmLine::getLineTypeEnum()		const
		{
			return enBGM;
		}

		const basar::I18nString& BgmLine::getOrdernoMF()	const
		{
			return m_OrdernoMF;		
		}


		bool	BgmLine::isDokcode640()		const
		{
			if (m_FixedDokCode == "640")		
			{
				return true;
			}

			return false;
		}

		// The BGM-Line has to be valid, otherwise the currently read order is not a correct transfer order.
		bool	BgmLine::isValidLine() const
		{
			if (!isDokcode640())
			{
				return false;
			}
			return true;
		}



		
		// ============================================
		// Structure of BGM data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example:  
		// "BGM64070231                                                                     "
		//  ^  ^  ^                                  ^     
		//  a  b  c                                  x       
		//
		// Field a= Line-Indicator		Offset  0, length  3
		// Field b= DokCode fixed Text:	Offset  3, length  3 (mandatory, must be "640")
		// Field c= OrdernoMF:			Offset  6, length 35 (optional, Orderno defined by and for manufacturer)
		// Field x= Filler  :			Offset 41, length 39 (optional, spaces to fill up the line to 80 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void BgmLine::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( BgmLine, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "BGM-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No BGM-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				m_FixedDokCode	= lineStr.substr(  3,  3 );		// Extract field b

				// Extract optional OrdernoMF if existent
				if ( 6 < lineStr.length() ) {						// > 6 = s_MinLength + minimum 1 char for OrdernoMF
					m_OrdernoMF		= lineStr.substr(  6, 35 );		// Extract field c, may be shorter than 35 chars
																	// Then substr copies as many as possible
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable BGM line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void BgmLine::resetMembers()
		{
			m_FixedDokCode	= "";		// field b
			m_OrdernoMF		= "";		// field c
		}

	} // namespace line
} // namespace domMod

