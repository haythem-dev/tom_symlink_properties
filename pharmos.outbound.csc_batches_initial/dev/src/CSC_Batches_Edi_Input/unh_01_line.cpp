//-------------------------------------------------------------------------------------
/*! \file  unh_01_line.cpp
*  \brief  subclass of EDIFACTLineBase for German 01-Segment.  
*  \brief  Main purpose: Extract an 01-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  \date   27.10.2014
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
#include "unh_01_line.h"

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
		const basar::I18nString	Unh_01_Line::s_LineIndicator = "01";	
		const basar::UInt32		Unh_01_Line::s_MinLength     = 28;	// 3 - 01-line consists only of a line identifier, nothing else
																	// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//

		Unh_01_Line::Unh_01_Line()
		{
			resetMembers();
		}


		EDIFACTLineBasePtr Unh_01_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Unh_01_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Unh_01_Line::getLineTypeEnum()		const
		{
			return en01;
		}

		bool	Unh_01_Line::isValidLine() const
		{
			if (!m_extractedCorrectlyFlag)
			{
				return false;
			}
			return true;
		}


		// UNH_TYP must be "UEWAUF". 
		bool	Unh_01_Line::isUnhTypOK()		const
		{
			if ( "UEWAUF" == m_FixedUnhTyp) {	// value resp. documentation
						return true;
				}
			
			return false;
		}

		// UNH_TYP must be "UEWAUF". 
		bool	Unh_01_Line::isUnhVersionOK()		const
		{
			if (       "S  " == m_FixedUnhVersion		
					|| " S " == m_FixedUnhVersion 
					|| "  S" == m_FixedUnhVersion) {
				return true;
			}
			
			return false;
		}

		// UNH_FREIGABE must be "93A". 
		bool	Unh_01_Line::isUnhFreigabeOK()		const
		{
			if ( "93A" == m_FixedUnhFreigabe) {	// value resp. documentation
						return true;
				}
			
			return false;
		}



		
		// ============================================
		// Structure of UNH_01_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "010400856148    UEWAUFS  93AUNPHOUEW"
		//  ^ ^             ^     ^  ^  ^                          ^       
		//  a b             c     d  e  f                          g 
		//
		// Field a= UNH-SATZART:		Offset  0,  length   2	(mandatory, line-indicator)
		// Field b= Filler:     		Offset  2,  length  14	(mandatory, spaces to fill up the line, ignore content)
		// Field c= UNH-Typ:     		Offset  16, length   6	(mandatory, Nachrichtentyp-Kennung, always "UEWAUF")
		// Field d= UNH-VERSION:  		Offset  22, length   3	(mandatory, Version der Nachricht, always "S" with two blanks)
		// Field e= UNH-FREIGABE:  		Offset  25, length   3	(mandatory, Freigabenummer, always "93A")
		// Field f= UNH-Filler:    		Offset  28, length  46	(optional,  spaces to fill up the line, ignore content)
		// Field g= UNH-Filler:    		Offset  74, length  476	(optional,  spaces to fill up the line to 550 chars, ignore content)
		// ============================================


		// throw: basar::cmnutil::BasarException
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Unh_01_Line::process(const std::string& lineStr)
		{
			METHODNAME_DEF( Unh_01_line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "UNH_01-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( /*fun*/"Unh_01_Line::process", str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No UNH_01-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( "Unh_01_Line::process", str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data: 
				m_extractedCorrectlyFlag		=	true;

				// Extract field c: mandatory UNH-TYP 
				m_FixedUnhTyp	= lineStr.substr(  16, 6 );		// Extract field c
																			
				if (!isUnhTypOK()) {
					std::stringstream str;
					str << "UNH_01-line contains unknown UNH-TYP-field (Should be UEWAUF): " << lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );
					m_extractedCorrectlyFlag		=	false;
				}

				// Extract field d: mandatory UNH-VERSION 
				m_FixedUnhVersion	= lineStr.substr(  22, 3 );		// Extract field d
																			
				if (!isUnhVersionOK()) {
					std::stringstream str;
					str << "UNH_01-line contains unknown UNH-VERSION-field (Should be S ): " << lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );
					m_extractedCorrectlyFlag		=	false;
				}

				// Extract field e: mandatory UNH-FREIGABE 
				m_FixedUnhFreigabe	= lineStr.substr(  25, 3 );		// Extract field e
																			
				if (!isUnhFreigabeOK()) {
					std::stringstream str;
					str << "UNH_01-line contains unknown UNH-FREIGABE-field (Should be 93A ): " << lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );
					m_extractedCorrectlyFlag		=	false;
				}

			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable UNH_01 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( "Unh_01_Line::process", str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Unh_01_Line::resetMembers()
		{
			m_extractedCorrectlyFlag	= false;	
		}

	} // namespace line
} // namespace domMod