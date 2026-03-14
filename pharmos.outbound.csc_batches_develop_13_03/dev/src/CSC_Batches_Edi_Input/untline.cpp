//-------------------------------------------------------------------------------------
/*! \file  untline.cpp
*  \brief  subclass of EDIFACTLineBase for UNT-Segment. 
*  \brief  Main purpose: Extract an UNT-Line from a UEWAUF-flatfile-line
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
#include "untline.h"

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
		const basar::I18nString	UntLine::s_LineIndicator = "UNT";	
		const basar::UInt32		UntLine::s_MinLength     = 3;	// 3 - UNT consists only of a line identifier, nothing else
																// Minimum length of netto data
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
															
		UntLine::UntLine()
		{
			resetMembers();
		}

		UntLine::~UntLine(){}

		EDIFACTLineBasePtr UntLine::clone() const 
		{
			return EDIFACTLineBasePtr (new UntLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		UntLine::getLineTypeEnum()		const
		{
			return enUNT;
		}

		bool	UntLine::isValidLine() const
		{
			if (!m_extractedCorrectlyFlag)
			{
				return false;
			}
			return true;
		}



		
		// ============================================
		// Structure of UNT data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example:  
		// "UNT                                                                             "
		//  ^  
		//  a  
		//
		// Field a= Line-Indicator:		Offset  0, length  3
		// ============================================


		// throw: basar::cmnutil::BasarException
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void UntLine::process(const std::string& lineStr)
		{
			METHODNAME_DEF( UntLine, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "UNT-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No UNT-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data: 
				// No netto data in UNT line: Everything ok.
				m_extractedCorrectlyFlag		=	true;
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable UNT line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void UntLine::resetMembers()
		{
			m_extractedCorrectlyFlag	= false;	
		}

	} // namespace line
} // namespace domMod