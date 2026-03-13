//-------------------------------------------------------------------------------------
/*! \file  imd_09_line.cpp
*  \brief  subclass of EDIFACTLineBase for German 09-Segment. 
*  \brief  Main purpose: Extract an IMD-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 27.10.14 pk new
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
#include "imd_09_line.h"
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
		const basar::I18nString	Imd_09_Line::s_LineIndicator = "09";	
		const basar::UInt32		Imd_09_Line::s_MinLength     = 2;	// 2+0=2 
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		Imd_09_Line::Imd_09_Line()
		{
			resetMembers();
		}

		Imd_09_Line::~Imd_09_Line(){}

		EDIFACTLineBasePtr Imd_09_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Imd_09_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Imd_09_Line::getLineTypeEnum()		const
		{
			return en09;
		}

		// The IMD-Line has to be valid, otherwise it cannot be inserted into the transfer order
		bool	Imd_09_Line::isValidLine() const
		{
			//if (!m_extractedCorrectlyFlag)
			//{
			//	return false;
			//}
			return true;
		}



		
		// ============================================
		// Structure of IMD_09_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "09blablabla                         "
		//  ^ ^              ... ^                                 
		//  a b              ... c                                       
		//
		// Field a= Line-Indicator				Offset  0, length  2
		// Field b= IMD-Filler:					Offset  2, length 92
		// Field c= FILLER:						Offset 94, length456, fills the 550 chars of the line
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Imd_09_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Imd_09_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "IMD-09-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No IMD-09-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data: There are no netto data
				m_extractedCorrectlyFlag = true;
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable IMD_09 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Imd_09_Line::resetMembers()
		{
			m_extractedCorrectlyFlag	= false;	
		}

	} // namespace line
} // namespace domMod

