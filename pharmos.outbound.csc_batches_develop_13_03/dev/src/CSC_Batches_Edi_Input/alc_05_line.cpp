//-------------------------------------------------------------------------------------
/*! \file  alc_05_line.cpp
*  \brief  subclass of EDIFACTLineBase for  German 05-Segment.  
*  \brief  Main purpose: Extract an 05-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 07.11.14 pk new
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
#include "alc_05_line.h"

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
		const basar::I18nString	Alc_05_Line::s_LineIndicator = "05";	
		const basar::UInt32		Alc_05_Line::s_MinLength     = 2;	// 2 (incl. qualifier)
																	// Minimum length of netto data
	//--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
															
														
		Alc_05_Line::Alc_05_Line()
		{
			resetMembers();
		}

		//Uns_97_Line::~Uns_97_Line(){}

		EDIFACTLineBasePtr Alc_05_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Alc_05_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Alc_05_Line::getLineTypeEnum()		const
		{
			return en05;
		}

		const domMod::line::Alc_05_Line::AlcPriceHandlingTypeEnum&		Alc_05_Line::getPriceHandling()			const
		{
			return m_PriceHandling;
		}

		// The UNS-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		bool	Alc_05_Line::isValidLine() const
		{
			// Only "H", "M", "F", "K" were handled in former PBS999-program
			if (   enH == m_PriceHandling  
				|| enM == m_PriceHandling
				|| enF == m_PriceHandling
				|| enK == m_PriceHandling)
			{
				return true;
			}

			return false;
		}



		
		// ============================================
		// Structure of ALC_05_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "05H"
		//  ^ ^^          ... ^
		//  a bc          ... x       
		//
		// Field a= Line-Indicator		Offset  0, length  2
		// Field b= Price handling:		Offset  2, length  1   optional, ALC_ZUABSCHLAG, can contain 'H', 'M', 'F', 'K' 
		// Field c= Alc-Filler:  		Offset  3, length 55  (optional)
		// Field x= Filler:  			Offset  3, length 492 (optional,  spaces to fill up the line to 550 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Alc_05_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Alc_05_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "ALC_05-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No ALC_05-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				if ( 3 <= lineStr.length() ) {						// >= offset + minimum 1 char for ALC-ZUABSCHLAG

					basar::I18nString phstring("");
					phstring		= lineStr.substr(  2,  1 );				// Extract field b

					if ("H" == phstring) {
						m_PriceHandling = enH;
					}
					else if ("M" == phstring) {
							m_PriceHandling =  enM;
					}
					else if ("F" == phstring) {
							m_PriceHandling =  enF;
					}
					else if ("K" == phstring) {
							m_PriceHandling =  enK;
					}
					else {		
						// Undefined 
						m_PriceHandling = enOTHER;
					}
				}
				// else: keep value enUnknown
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable ALC_05 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Alc_05_Line::resetMembers()
		{
			m_PriceHandling		= enUNKNOWN;			// field b
		}

	} // namespace line
} // namespace domMod
