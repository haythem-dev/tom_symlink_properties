//-------------------------------------------------------------------------------------
/*! \file  irrelevantline.cpp
*  \brief  subclass of EDIFACTLineBase for IRR-Segment. 
*  \brief  Main purpose: Extract an irrelevant line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of Irr-lines can be processed within this method	
*  \author P. Kessling
*  \date   28.03.2013
*  history 28.03.13 pk new
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
#include "irrelevantline.h"
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
		const basar::UInt32		IrrLine::s_MinLength     = 3;	// 3: Only for the line identifier, e.g. "TDT"
																// Minimum length of netto data
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		IrrLine::IrrLine()
		{
			resetMembers();
		}

		IrrLine::~IrrLine(){}

		EDIFACTLineBasePtr IrrLine::clone() const 
		{
			return EDIFACTLineBasePtr (new IrrLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		IrrLine::getLineTypeEnum()		const
		{
			return enIRRELEVANT;
		}
		domMod::line::IrrLine::IrrLineTypeEnum IrrLine::getLineType()	const
		{
			return m_LineType;		
		}
		const basar::I18nString& IrrLine::getLine()	const
		{
			return m_Line;		
		}


		// The irrelevant line has to be valid, that is of a wellknown type. 
		// Otherwise it cannot be inserted into a transfer order.
		bool	IrrLine::isValidLine() const
		{
			// m_LineType is enUnknown as long as line is not processed.
			if (   enOTHER		== m_LineType		// LineType, which is not one of the known irrelevant lines
				|| enUNKNOWN	== m_LineType)
			{
				return false;
			}

			return true;
		}



		
		// ============================================
		// Structure of IRR data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Can be lines of type TDT, LOC, FTX, RFF, PRI, MOA
		// There are more lines without info like UNZ, UNH. But tese are not irrelevant because they 
		// are brackets embracing essential parts of UEWAUF 
		// Examples: 
		// "TDT12                                                                           "
		// "LOC8                                                                            "
		//  ^  ^  
		//  a  y  
		//
		// Field a= Line-Indicator		Offset  0, length  3  (mandatory), e.g. "TDT"
		// Field y= Rest of line		Offset  3, length  77 (optional)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void IrrLine::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( IrrLine, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "IRR-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// extract line type
				basar::I18nString strLineType = lineStr.substr( 0,  3 );
				if( strLineType == "TDT")
				{
					m_LineType = enTDT;
				}
				else if( strLineType == "LOC")
				{
					m_LineType = enLOC;
				}
				else if( strLineType == "FTX")
				{
					m_LineType = enFTX;
				}
				else if( strLineType == "RFF")
				{
					m_LineType = enRFF;
				}
				else if( strLineType == "PRI")
				{
					m_LineType = enPRI;
				}
				else if( strLineType == "MOA")
				{
					m_LineType = enMOA;
				}
				else 
				{
					m_LineType = enOTHER;
				}

				// Extract netto data:
				m_Line	= lineStr.substr(  0,  80 );				// Extract field a and y: Whole line

			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable irrelevant line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void IrrLine::resetMembers()
		{
			m_LineType		= enUNKNOWN;	
			m_Line			= "";			
		}

	} // namespace line
} // namespace domMod
