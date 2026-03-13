//-------------------------------------------------------------------------------------
/*! \file  irrelevcant_de_line.cpp
*  \brief  subclass of EDIFACTLineBase for Segments 05 (ALC), 07 (MOA), 11 (FTX) , 12 (PRI)
*  \brief  These segments can be included in a GERMAN .inh-flatfile, but they are not 
*  \brief  evaluated by ABBA-UW.
*  \brief  Main purpose: Extract an irrelevant Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of known irelevant Lines can be processed within this method
*  \author P. Kessling
*  history 29.10.14  pk new
*          29.06.16  pk: "else if" instead of "if" necessary in  IrrLine_DE::process (09)
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
#include "irrelevant_de_line.h"
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
		const basar::UInt32		IrrLine_DE::s_MinLength     = 2;	// 2: Only for the line identifier, e.g. "11"
																	// Minimum length of netto data
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		IrrLine_DE::IrrLine_DE()
		{
			resetMembers();
		}

		IrrLine_DE::~IrrLine_DE(){}

		EDIFACTLineBasePtr IrrLine_DE::clone() const 
		{
			return EDIFACTLineBasePtr (new IrrLine_DE(*this) );
		
		}

		// Getter 
		LineTypeEnum		IrrLine_DE::getLineTypeEnum()		const
		{
			return enIRRELEVANT;
		}
		domMod::line::IrrLine_DE::IrrLine_DE_TypeEnum IrrLine_DE::getLineType()	const
		{
			return m_LineType;		
		}
		const basar::I18nString& IrrLine_DE::getLine()	const
		{
			return m_Line;		
		}


		// The irrelevant line has to be valid, that is of a wellknown type. 
		// Otherwise it cannot be inserted into a transfer order.
		bool	IrrLine_DE::isValidLine() const
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
		// Structure of DTM_03_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// Can be lines of type 07 (MOA), 11 (FTX), 12 (PRI)
		// There are more lines without info like UNZ, UNH. But tese are not irrelevant because they 
		// are brackets embracing essential parts of UEWAUF 
		// Examples: 
		// "11INV            A12807                      "
		//  ^ ^  
		//  a y  
		//
		// Field a= Line-Indicator		Offset  0, length  2   (mandatory), e.g. "11"
		// Field y= Rest of line		Offset  2, length  498 (optional)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void IrrLine_DE::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( IrrLine_DE, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "IRR-DE-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// extract line type
				basar::I18nString strLineType = lineStr.substr( 0,  2 );
				if( strLineType == "07")
				{
					m_LineType = en07;
				}
				else if( strLineType == "09")		// 29.06.16 pk "else" added to be able to accept "07" lines as irrelevant
				{
					m_LineType = en09;
				}
				else if( strLineType == "12")
				{
					m_LineType = en12;
				}
				else if( strLineType == "98")
				{
					m_LineType = en98;
				}
				else if( strLineType == "99")
				{
					m_LineType = en99;
				}
				else 
				{
					m_LineType = enOTHER;
				}

				// Extract netto data:
				m_Line	= lineStr.substr(  0,  550 );				// Extract field a and y: Whole line

			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable irrelevant DE line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void IrrLine_DE::resetMembers()
		{
			m_LineType		= enUNKNOWN;	
			m_Line			= "";			
		}

	} // namespace line
} // namespace domMod
