//-------------------------------------------------------------------------------------
/*! \file  unzline.cpp
*  \brief  subclass of EDIFACTLineBase for UNZ-Segment. 
*  \brief  Main purpose: Extract an UNZ-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  \date   26.03.2013
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
#include "unzline.h"

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
		const basar::I18nString	UnzLine::s_LineIndicator = "UNZ";	
		const basar::UInt32		UnzLine::s_MinLength     = 3;	// 3 - UNZ consists only of a line identifier, nothing else
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		UnzLine::UnzLine()
		{
			resetMembers();
		}

		UnzLine::~UnzLine(){}

		EDIFACTLineBasePtr UnzLine::clone() const 
		{
			return EDIFACTLineBasePtr (new UnzLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		UnzLine::getLineTypeEnum()		const
		{
			return enUNZ;
		}

		bool	UnzLine::isValidLine() const
		{
			if (!m_extractedCorrectlyFlag)
			{
				return false;
			}
			return true;
	}



		
		// ============================================
		// Structure of UNZ data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example:  
		// "UNZ                                                                             "
		//  ^  ^
		//  a  x
		//
		// Field a= Line-Indicator:		Offset  0, length  3
		// Field x= Filler:             Offset  3, length 77    fills the line up to 80 char with blanks
		// ============================================


		// throw: basar::cmnutil::BasarException
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void UnzLine::process(const std::string& lineStr)
		{
			METHODNAME_DEF( UnzLine, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "UNZ-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No UNZ-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data: 
				// No netto data in UNZ line: Everything ok.
				m_extractedCorrectlyFlag		=	true;
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable UNZ line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void UnzLine::resetMembers()
		{
			m_extractedCorrectlyFlag	= false;	
		}

	} // namespace line
} // namespace domMod