//-------------------------------------------------------------------------------------
/*! \file  nadline.cpp
*  \brief  subclass of EDIFACTLineBase for NAD-Segment. 
*  \brief  Main purpose: Extract an NAD-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of NAD-lines can be processed within this method	
*  \author Jia Li ,P. Kessling
*  \date   02.01.2013
*  history 26.03.13 pk rework
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
#include "nadline.h"
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
		const basar::I18nString	NadLine::s_LineIndicator = "NAD";	
		const basar::UInt32		NadLine::s_MinLength     = 20;	// 3+3+14=20 (incl. EAN ohne Codepfl="104" und Text \n\r)
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		NadLine::NadLine()
		{
			resetMembers();
		}

		NadLine::~NadLine(){}

		EDIFACTLineBasePtr NadLine::clone() const 
		{
			return EDIFACTLineBasePtr (new NadLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		NadLine::getLineTypeEnum()		const
		{
			return enNAD;
		}
		const domMod::line::NadLine::NadRoleEnum& NadLine::getRole()	const
		{
			return m_Role;		
		}
		const basar::I18nString& NadLine::getText()	const
		{
			return m_Text;		
		}
		const basar::I18nString& NadLine::getIdentEan()	const
		{
			return m_IdentEan;		
		}

		// Codepfl should be 104. If there is no codepfl at all or an empty oe: its ok.
		// But if there is a codepfl defined in the NAD-Line, it must be 104.
		bool	NadLine::isCodepflOK()		const
		{
			if (m_FixedCodepfl.length() >= 1) {

				if (   "   " == m_FixedCodepfl		// leer?
					|| "  "  == m_FixedCodepfl 
					|| " "   == m_FixedCodepfl) {
						return true;
					}
				if (   "104" == m_FixedCodepfl) {	// value resp. documentation
						return true;
				}
				if (   "9  " == m_FixedCodepfl) {	// real life
						return true;
				}

				// Contains invalid codepfl-Info.
				// Be careful: Old host program PBS993 didn't check this field, so everything may happen.
				// In real life this field often contains "9  "
				return false;
			}

			// No codepfl-Info at all: It is ok.
			return true;
		}

		// The NAD-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		bool	NadLine::isValidLine() const
		{
			// Only Nadline types/participants MF, DS or BY are relevant for ABBA-UW
			// M_Role is enUnknown as long as line is not processed.
			if (   enMF		!= m_Role   
				&& enDS		!= m_Role 
				&& enBY		!= m_Role 
				&& enOTHER	!= m_Role)		// NADSU etc. is permitted, will be ignored (normally value equals DS)
			{
				return false;
			}

			// IdentEAN is mandatory
			if (0 == m_IdentEan.length() )
			{
				return false;
			}

			// codepfl: If existent it must be 104
			if (!isCodepflOK())
			{
				return false;
			}

			return true;
		}



		
		// ============================================
		// Structure of NAD data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example: 
		// "NADBY 7601001372085    PHARMACIE DE LA GARE NEUCHATEL SA                        "
		// "NADDS 7601001000209    AMEDIS-UE AG                                             "
		// "NADMF 7601001002470    ZELLER MAX SOHNE AG                                      "
		// "NADMF 7601001002470 104ZELLER MAX SOHNE AG                                      "
		//  ^  ^  ^             ^  ^                                   ^     
		//  a  b  c             d  e                                   x       
		//
		// Field a= Line-Indicator		Offset  0, length  3
		// Field b= Role:				Offset  3, length  3 (mandatory, must be "MF"-manufacturer, "DS"-Deliverer/wholesaler, "BY"-Buyer, 
		//													             "SE" or "CN" or others are also possible but with no meaning for ABBA-UW)
		// Field c= IdentEan:			Offset  6, length 14 (mandatory, EAN number for identification of 
		//                                                               sending manufacturer (NADMF), 
		//																 delivering wholesaler-branch (NADDS) or
		//																 buying pharmacy (NADBY)
		// Field d= Codepfl fixed Text:	Offset 20, length  3 (mandatory?, must be "104". Doku says it is mandatory. In real life this field can be empty.
		//                                                                if it contains "ZZZ", then IdentEAN is a PZN!! (implemented in old host program PBS993)
		// Field e= Text:			    Offset 23, length 35 (optional,  contains Name and/or address
		// Field x= Filler:  			Offset 58, length 22 (optional,  spaces to fill up the line to 80 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void NadLine::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( NadLine, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "NAD-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No NAD-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				basar::I18nString strRole = lineStr.substr( 3,  3 );// extract field b: NAD-BETEIL - Role / Type of NADline
				if (   "MF " == strRole
					|| " MF" == strRole) {
					m_Role = enMF;
				} 
				else if (   "DS " == strRole
					     || " DS" == strRole) {
					m_Role = enDS;
				} 
				else if (   "BY " == strRole
					     || " BY" == strRole) {
					m_Role = enBY;
				} 
				else if (strRole.length() > 0 ){ 
					// not an error, could be NADSU for example
					m_Role = enOTHER;
				} 
				else { 
					std::stringstream str;
					str << "NAD-line without NAD-BETEIL (Role) field: " << lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

					m_Role = enUNKNOWN;
				}

				m_IdentEan	= lineStr.substr(  6,  14 );			// Extract field c: Usually there are 13 chars with trailing blank

				// Extract "mandatory" Codepfl if existent
				if ( 21 <= lineStr.length() ) {						// >= 21: s_MinLength + minimum 1 char for Codepfl
					m_FixedCodepfl	= lineStr.substr(  20, 3 );		// Extract field d, may be shorter than 3 chars
																	// Then substr copies as many as possible
				}
				if (!isCodepflOK()) {
					std::stringstream str;
					str << "NAD-line contains unknown codepfl-field (Should be 104): " << lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );
				}

				// Extract optional Text if existent
				if ( 24 <= lineStr.length() ) {						// >= 24 = s_MinLength + codepfl + minimum 1 char for Text
					m_Text			= lineStr.substr(  23, 35 );	// Extract field 3, may be shorter than 35 chars
																	// Then substr copies as many as possible
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable NAD line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void NadLine::resetMembers()
		{
			m_Role			= enUNKNOWN;	// field b
			m_IdentEan		= "";			// field c
			m_FixedCodepfl	= "";			// field d
			m_Text			= "";			// field e
		}

	} // namespace line
} // namespace domMod
