//-------------------------------------------------------------------------------------
/*! \file  unsline.cpp
*  \brief  subclass of EDIFACTLineBase for UNS-Segment. 
*  \brief  Main purpose: Extract an UNS-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                Both types of UNS-lines can be processed within this method	
*  \author P. Kessling
*  \date   26.03.2013
*  history 26.03.13 pk new
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
#include "unsline.h"

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
		const basar::I18nString	UnsLine::s_LineIndicator = "UNS";	
		const basar::UInt32		UnsLine::s_MinLength     = 4;	// 3+1=4 (incl. qualifier)
																// Minimum length of netto data
	//--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
															
														
		UnsLine::UnsLine()
		{
			resetMembers();
		}

		//UnsLine::~UnsLine(){}

		EDIFACTLineBasePtr UnsLine::clone() const 
		{
			return EDIFACTLineBasePtr (new UnsLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		UnsLine::getLineTypeEnum()		const
		{
			return enUNS;
		}
		// Two UNS-lines embrace the UEWAUF-setion for position lines
		// At the beginning of the position section has to be a UNS-S-line
		// At the end of the position section has to be a UNS-D-line
		// "S" and "D" are called qualifiers
		// A valid UNS-line is either a begin- or an end-line.
		//
		// !! Special manufacturer: At least one manufacturer (7601001002043 Biomed AG)
		// !! writes UNS-S at the beginning AND at the end of the position section.
		// !! We tolerate this, because the UEWAUF-description is not very precise at this point..
		bool	UnsLine::isBeginOfPositionSection()		const
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
		bool	UnsLine::isValidLine() const
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
		// Structure of UNS data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example: 
		// "UNSD                                                                            "
		// "UNSS                                                                            "
		//  ^  ^^     
		//  a  bx       
		//
		// Field a= Line-Indicator		Offset  0, length  3
		// Field b= Qualifier:			Offset  3, length  1 (mandatory, must be "D"-begin of position section or "S"-end of position section, 
		// Field x= Filler:  			Offset  4, length 76 (optional,  spaces to fill up the line to 80 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void UnsLine::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( UnsLine, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "UNS-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No UNS-lineindicator: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				m_Qualifier	= lineStr.substr(  3,  1 );				// Extract field b
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable UNS line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void UnsLine::resetMembers()
		{
			m_Qualifier		= "";			// field b
		}

	} // namespace line
} // namespace domMod
