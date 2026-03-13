//-------------------------------------------------------------------------------------
/*! \file  nad_04_line.cpp
*  \brief  subclass of EDIFACTLineBase for German 04-Segment (NAD). 
*  \brief  Main purpose: Extract an NAD-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of NAD-lines can be processed within this method	
*  \author P. Kessling
*  history 28.10.14 pk   neu
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
#include "nad_04_line.h"
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
		const basar::I18nString	Nad_04_Line::s_LineIndicator = "04";	
		const basar::UInt32		Nad_04_Line::s_MinLength     = 25;	// 2+3+8+9+3=25 (incl. NAD_CODELISTE \n\r)
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		Nad_04_Line::Nad_04_Line()
		{
			resetMembers();
		}

		Nad_04_Line::~Nad_04_Line(){}

		EDIFACTLineBasePtr Nad_04_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Nad_04_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Nad_04_Line::getLineTypeEnum()		const
		{
			return en04;
		}
		const domMod::line::Nad_04_Line::NadRoleEnum& Nad_04_Line::getRole()	const
		{
			return m_Role;		
		}
		const basar::I18nString& Nad_04_Line::getIdent()	const
		{
			return m_Ident;		
		}
		const domMod::line::Nad_04_Line::NadCodelistEnum& Nad_04_Line::getCodelist()	const
		{
			return m_Codelist;		
		}
		const basar::I18nString& Nad_04_Line::getNameText()	const
		{
			return m_NameText;		
		}
		const basar::I18nString& Nad_04_Line::getOrtText()	const
		{
			return m_OrtText;		
		}

		// Codelist should be enBGA or enBFA. 
		bool	Nad_04_Line::isCodelistOK()		const
		{
			if (   enBFA	== m_Codelist   
				|| enBGA	== m_Codelist)		
			{
				return true;
			}

			// Contains invalid Codelist-Info.
			return false;
		}

		// The NAD-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		bool	Nad_04_Line::isValidLine() const
		{
			// Only Nadline types/participants MF, DS or BY are relevant for ABBA-UW
			// M_Role is enUnknown as long as line is not processed.
			if (   enMF		!= m_Role   
				&& enDS		!= m_Role 
				&& enBY		!= m_Role 
				&& enCN		!= m_Role 
				&& enOTHER	!= m_Role)		
			{
				return false;
			}

			// Ident is mandatory  (BGA or BFA-number)
			if (0 == m_Ident.length() )
			{
				return false;
			}

			// codelist: If existent it must be 104
			if (!isCodelistOK())
			{
				return false;
			}

			return true;
		}



		
		// ============================================
		// Structure of NAD_04_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "04MF 68758258         BFA
		// "04BY 3372096          BGA             Brücken-Apotheke                   Frau Möller                            Brückenstr. 12a                                                                                          Rüdersdorf                                  15562    DE
		// "04DS 3003016          BGA             PHOENIX Pharmahandel GmbH & Co. KG Vertriebszentrum Berlin                Lengeder Str. 42                                                                                         Berlin                                      13407    DE
		// "04CN 3372096          BGA             Brücken-Apotheke                   Frau Möller                            Brückenstr. 12a                                                                                          Rüdersdorf                                  15562    DE
		//  ^ ^  ^       ^        ^  ^         ...^                                                                ...^ ... ^
		//  a b  c       d        e  f         ...g                                                                ...h ... i      
		//
		// Field a= Line-Indicator		Offset  0, length  "
		// Field b= Role:				Offset  2, length   3 (mandatory, must be "MF"-manufacturer, "DS"-Deliverer/wholesaler, "BY"-Buyer, 
		//													             "CN" or others are also possible but with no meaning for ABBA-UW)
		// Field c= Ident:   			Offset  5, length   8 (mandatory, BFA or BGA number for identification of 
		//                                                                sending manufacturer (MF), 
		//												  				  delivering wholesaler-branch (DS) or
		//												  				  buying pharmacy (BY)
		// Field d= Filler:  			Offset 13, length   9 (           spaces to fill up the line )
		// Field e= Codelist:			Offset 22, length   3 (mandatory, must be "BFA" or "BGA". Informs us how to interpret field c "Ident")
		// Field f= Filler:  			Offset 25, length 178 (           spaces to fill up the line )
		// Field g= TextName:			Offset 203, length 35 (optional,   contains Name and/or address
		// Field h= Filler:  			Offset 238, length 248 (           spaces to fill up the line )
		// Field i= TextOrt:			Offset 486, length 35 (optional,   contains city etc.)
		// Field j= Filler:  			Offset 521, length 21 (           spaces to fill up the line )
		// Field x= Filler:  			Offset 542, length  8 (           spaces to fill up the line to 550)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Nad_04_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Nad_04_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "NAD_04-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No NAD_04-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				basar::I18nString strRole = lineStr.substr( 2,  3 );// extract field b: NAD-BETEIL - Role / Type of NADline
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
				else if (   "CN " == strRole
					     || " CN" == strRole) {
					m_Role = enCN;
				} 
				else if (strRole.length() > 0 ){ 
					// not an error
					m_Role = enOTHER;
				} 
				else { 
					std::stringstream str;
					str << "NAD_04-line without NAD-BETEIL (Role) field: " << lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

					m_Role = enUNKNOWN;
				}

				m_Ident	= lineStr.substr(  5,  8 );			// Extract field c: Usually there are 7 to 8 chars maybe with trailing blank

				// Extract mandatory Codelist
				//m_Codelist	= lineStr.substr(  22, 3 );		// Extract field d, may be shorter than 3 chars

				basar::I18nString strCodelist = lineStr.substr( 22,  3 );	// Extract field d
				if (   "BFA" == strCodelist) {
					m_Codelist = enBFA;
				} 
				else if (   "BGA" == strCodelist) {
					m_Codelist = enBGA;
				} 
				else if (enMF != m_Role ) {
					// kes 06.03.15: Some manufacturers send 'ZZZ' or something else.
					//               Old PBS999 accepts it as BGA then. Only NAD-MF must (!) be 'BFA'.
					m_Codelist = enBGA;
				}
				else if (strRole.length() > 0 ){ 
					// error
					m_Codelist = enOTHER_CODELIST;
				} 
				else { 
					std::stringstream str;
					str << "NAD_04-line without NAD-CODELIST (Role) field: " << lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

					m_Codelist = enUNKNOWN_CODELIST;
				}

				if (!isCodelistOK()) {			// TODO: Wird evtl. doppelt gerufen: Nochmal beim mounten-Check
					std::stringstream str;
					str << "NAD_04-line contains invalid codelist-field (Should be BGA or BFA): " << lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );
				}

				// Extract optional NameText if existent
				if ( 204 <= lineStr.length() ) {						// >= offset + minimum 1 char for Text
					m_NameText			= lineStr.substr(  203, 35 );	// Extract field 3, may be shorter than 35 chars
																	    // Then substr copies as many as possible
				}

				// Extract optional OrtText if existent
				if ( 487 <= lineStr.length() ) {						// >= offset + minimum 1 char for Text
					m_OrtText			= lineStr.substr(  486, 35 );	// Extract field 3, may be shorter than 35 chars
																	    // Then substr copies as many as possible
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable NAD_04 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Nad_04_Line::resetMembers()
		{
			m_Role			= enUNKNOWN;			// field b
			m_Ident			= "";					// field c
			m_Codelist		= enUNKNOWN_CODELIST;	// field e
			m_NameText		= "";					// field g
			m_OrtText		= "";					// field i
		}

	} // namespace line
} // namespace domMod
