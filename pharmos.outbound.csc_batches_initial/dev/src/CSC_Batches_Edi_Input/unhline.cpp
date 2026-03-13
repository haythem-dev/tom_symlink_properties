//-------------------------------------------------------------------------------------
/*! \file  unhline.cpp
*  \brief  subclass of EDIFACTLineBase for UNH-Segment. 
*  \brief  Main purpose: Extract an UNH-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  \date   20.03.2013
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
#include "unhline.h"

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
		const basar::I18nString	UnhLine::s_LineIndicator = "UNH";	
		const basar::UInt32		UnhLine::s_MinLength     = 3;	// 3 - UNH consists only of a line identifier, nothing else
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//

		UnhLine::UnhLine()
		{
			resetMembers();
		}


		EDIFACTLineBasePtr UnhLine::clone() const 
		{
			return EDIFACTLineBasePtr (new UnhLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		UnhLine::getLineTypeEnum()		const
		{
			return enUNH;
		}

		bool	UnhLine::isValidLine() const
		{
			if (!m_extractedCorrectlyFlag)
			{
				return false;
			}
			return true;
		}



		
		// ============================================
		// Structure of UNH data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example:  
		// "UNH                                           "
		//  ^  
		//  a  
		//
		// Field a= Line-Indicator:		Offset  0, length  3
		// ============================================


		// throw: basar::cmnutil::BasarException
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void UnhLine::process(const std::string& lineStr)
		{
			METHODNAME_DEF( Unhline, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "UNH-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( /*fun*/"UnhLine::process", str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No UNH-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( "UnbLine::process", str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data: 
				// No netto data in UNH line: Everything ok.
				m_extractedCorrectlyFlag		=	true;
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable UNH line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( "UnhLine::process", str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void UnhLine::resetMembers()
		{
			m_extractedCorrectlyFlag	= false;	
		}

	} // namespace line
} // namespace domMod