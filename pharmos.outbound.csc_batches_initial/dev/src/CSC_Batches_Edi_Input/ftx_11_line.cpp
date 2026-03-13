//-------------------------------------------------------------------------------------
/*! \file  ftx_11_line.cpp
*  \brief  subclass of EDIFACTLineBase for German 11-Segment (FTX free text). 
*  \brief  Main purpose: Extract an FTX-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 07.11.14  pk new
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
#include "ediline_definitions.h"
#include "ftx_11_line.h"
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
		const basar::I18nString	Ftx_11_Line::s_LineIndicator = "11";	
		const basar::UInt32		Ftx_11_Line::s_MinLength     = 5;	// 5 = 2+3 ( service-segment (02-SATZART), FTX_ZUCODE, rest is optional according to DEDEU-documentation)
																	// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		Ftx_11_Line::Ftx_11_Line()
		{
			resetMembers();
		}

		Ftx_11_Line::~Ftx_11_Line(){}

		EDIFACTLineBasePtr Ftx_11_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Ftx_11_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Ftx_11_Line::getLineTypeEnum()		const
		{
			return en11;
		}
		const basar::I18nString& Ftx_11_Line::getFreeText()		const
		{
			return m_FreeText;		
		}

		// Checker		isOrderheadText()			
		bool		Ftx_11_Line::isOrderheadText()		const
		{
			// Datafield FTX_ZUCODE must contain "AAQ", otherwise this text is not an orderheadtext
			if ( "AAQ" == m_ZuCode )
			{
				return true;
			}
			return false;
		}

		// Checker		isOrderposText()			
		bool		Ftx_11_Line::isOrderposText()		const
		{
			// Datafield FTX_ZUCODE must contain "AAQ", otherwise this text is not an orderheadtext
			if ( "AAA" == m_ZuCode )
			{
				return true;
			}
			return false;
		}



		// The FTX-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		bool	Ftx_11_Line::isValidLine() const
		{
			// Zucodes != AAA oder AAQ werden später ignoriert. 
			return true;

			//if (isOrderheadText() || isOrderposText())
			//{
			//	return true;
			//}		
			//
			//return false;
		}



		
		// ============================================
		// Structure of FTX_11_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "11AAQ            Freier Freitextkkjhkjhkjmnbmbmnmnmnbmnmb                           "
		//  ^ ^  ^           ^                                   ... ^       ...  ^             "      
		//  a b  c           d                                   ... e       ...  x             "
		//
		// Field a= Line-Indicator		Offset   0, length  2
		// Field b= Positionnumber:		Offset   2, length  3 (mandatory, must contain "AAQ" to be a orderheadtext)
		// Field c= Filler:  			Offset   5, length 12 (optional,  spaces to fill up the line)
		// Field d= Free text:			Offset  17, length 70 (optional,  text)
		// Field e= FTX-Filler: 		Offset  87, length 283 (optional,  spaces to fill up the line)
		// Field x= Filler:  			Offset 370, length 180 (optional,  spaces to fill up the line to 550 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Ftx_11_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Ftx_11_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "FTX_11-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No FTX_11-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:

				// Extract mandatory FTX_ZUCODE if existent
				m_ZuCode = lineStr.substr( 2,  3 );

				// Extract optional FTX_TEXT if existent
				if ( 18 <= lineStr.length() ) {						// > 18: offset (FTX_TEXT+1)
					m_FreeText	= lineStr.substr(  17, 70 );		// Extract field c, may be shorter than 70 chars
																	// Then substr copies as many as possible
				}

			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable FTX_11 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Ftx_11_Line::resetMembers()
		{
			m_ZuCode		= "";			
			m_FreeText	= "";			
		}

	} // namespace line
} // namespace domMod
