//-------------------------------------------------------------------------------------
/*! \file  uns_97_line.cpp
*  \brief  subclass of EDIFACTLineBase for  German 97-Segment.  
*  \brief  Main purpose: Extract an 97-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                Both types of UNS-lines (D and S) can be processed within this method	
*  \author P. Kessling
*  history 27.10.13 pk new
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
#include "uns_97_line.h"

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
		const basar::I18nString	Uns_97_Line::s_LineIndicator = "97";	
		const basar::UInt32		Uns_97_Line::s_MinLength     = 3;	// 2+1=3 (incl. qualifier)
																// Minimum length of netto data
	//--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
															
														
		Uns_97_Line::Uns_97_Line()
		{
			resetMembers();
		}

		//Uns_97_Line::~Uns_97_Line(){}

		EDIFACTLineBasePtr Uns_97_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Uns_97_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Uns_97_Line::getLineTypeEnum()		const
		{
			return en97;
		}
		// Two 97-lines embrace the UEWAUF-setion for position lines
		// At the beginning of the position section has to be a 97-S-line
		// At the end of the position section has to be a 97-D-line
		// "S" and "D" are called qualifiers
		// A valid UNS-line is either a begin- or an end-line.
		//
		// CH!! Special manufacturer: At least one manufacturer (7601001002043 Biomed AG)
		// CH!! writes UNS-S at the beginning AND at the end of the position section.
		// CH!! We tolerate this, because the UEWAUF-description is not very precise at this point..
		bool	Uns_97_Line::isBeginOfPositionSection()		const
		{
			if ("D" == m_Qualifier) {
					return true;
			}
			else {		
				// Undefined or "S"
				return false;
			}
		}

		// The UNS-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		bool	Uns_97_Line::isValidLine() const
		{
			// Only Uns-qualifiers "D" and "S" are relevant for ABBA-UW
			if (   "D" == m_Qualifier  
				|| "S" == m_Qualifier )
			{
				return true;
			}

			return false;
		}



		
		// ============================================
		// Structure of UNS_97_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "97S                                                                            "
		// "97D                                                                            "
		//  ^ ^^     
		//  a bx       
		//
		// Field a= Line-Indicator		Offset  0, length  2
		// Field b= Qualifier:			Offset  2, length  1 (mandatory, must be "D"-begin of position section or "S"-end of position section, 
		// Field x= Filler:  			Offset  3, length 76 (optional,  spaces to fill up the line to 550 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Uns_97_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Uns_97_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "UNS_97-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No UNS_97-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				m_Qualifier	= lineStr.substr(  2,  1 );				// Extract field b
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable UNS_97 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Uns_97_Line::resetMembers()
		{
			m_Qualifier		= "";			// field b
		}

	} // namespace line
} // namespace domMod
