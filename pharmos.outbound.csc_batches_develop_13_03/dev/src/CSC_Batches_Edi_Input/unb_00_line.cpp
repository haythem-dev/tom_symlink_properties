//-------------------------------------------------------------------------------------
/*! \file  unbline.cpp
*  \brief  subclass of EDIFACTLineBase for German 00-Segment (UNB). 
*  \brief  Main purpose: Extract an IMD-Line rom a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 29.10.14  pk new
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

#include "ediline_definitions.h"

#include "loggerpool.h"	// for LoggerPool
#include "abbauwexception.h"
#include "unprocessableexception.h"
#include "unb_00_line.h"

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

		const basar::I18nString	Unb_00_Line::s_LineIndicator = "00";	
		const basar::UInt32		Unb_00_Line::s_MinLength     = 123;	// incl. UNB_ZEIT
																	// Minimum length of netto data

    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
															
		Unb_00_Line::Unb_00_Line()
		{
			resetMembers();
		}

		Unb_00_Line::~Unb_00_Line(){}

		EDIFACTLineBasePtr Unb_00_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Unb_00_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Unb_00_Line::getLineTypeEnum()		const
		{
			return en00;
		}
		const basar::I18nString& Unb_00_Line::getSupplierEan()	const
		{
			return m_SupplierBfa;		
		}
		const basar::I18nString& Unb_00_Line::getWholesalerEan()	const
		{
			return m_WholesalerBfa;		
		}


		bool	Unb_00_Line::isSyntaxOk()		const
		{
			// kes 06.03.15: Manche Firmen senden (oft) UNOC/UNOB3 statt ..2
			// kes 31.03.15: UNOA kann lt. H. Knoechel auch vorkommen
			if (   m_FixedSyntax == "UNOB2"
				|| m_FixedSyntax == "UNOC2"					// Difference between UNOB und UNOC und UNOA?	ABC is hint on character set, see original EDIFact-Docu
				|| m_FixedSyntax == "UNOA2"					// Difference between UNOB und UNOC und UNOA?	ABC is hint on character set, see original EDIFact-Docu
				|| m_FixedSyntax == "UNOB3"					// UNOC occurs in Branches 06, 09,33,40,46	= DE NORD plus Hanau (56, umgezogen aus Nord nach Süd)
				|| m_FixedSyntax == "UNOC3"					// UNOB occurs in  DE-Sued und HAGEDA (21)
				|| m_FixedSyntax == "UNOA3")				// UNOA occurs very seldomly
			{
				return true;
			}
			return false;
		}
		bool	Unb_00_Line::isWholesaleOk()		const
		{
			
			// This BFA-number for PHOENIX Germany is the same for all German branches including HAGEDA
			// In DE  wholesaleBFA must be Phoenix for all. Otherwise this transferOrder 
			// is not to be handled by Phoenix.

			// kes 04.02.19: Messages sent via our QAS mailbox "PHOENIXQAS" at the OpenText VAN also possible (meant only for test messages to be sent to our QAS system) 
			
			if (    PHOENIX_DE_BFA		  == m_WholesalerBfa 
				 || PHOENIX_DE_QASMAILBOX == m_WholesalerBfa ) {
				// TransferOrder is  dedicated to PROD wholesalers PHOENIX / DE or HAGEDA 
				// or it was sent to our QAS testsystem via the OpenText QAS mailbox 
				return true;
			}
			return false;
		}

		//Check if the order comes from PharmaMall
		bool Unb_00_Line::isFromPharmaMall() const
		{
			bool isFromPharmaMall = false;

			if (m_SupplierBfa == domMod::line::BFA_ASTELLAS ||
				m_SupplierBfa == domMod::line::BFA_MSD)
			{
				isFromPharmaMall = true;
			}

			return isFromPharmaMall;
		}

		bool	Unb_00_Line::isTestOrder()	const
		{
			if(m_TestOrderFlag){
				return true;
			}
			return false;
		}

		// The UNB-Line has to be valid to start a new order.
		bool	Unb_00_Line::isValidLine() const
		{
			// This fixed text has to be "UNOB2" or "UNOC2"
			if (!isSyntaxOk())
			{
				std::stringstream str;
				str << "Error: UNB_00-line didn't pass syntax check!" << std::endl;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

				return false;
			}

			// Wholesale must be PHOENIX
			if (!isWholesaleOk())
			{
				std::stringstream str;
				str << "Error: Wrong wholesaler-BFA: " << m_WholesalerBfa.c_str() << ". Should be " << PHOENIX_DE_BFA << " or " << PHOENIX_DE_QASMAILBOX << std::endl;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

				return false;
			}

			// Testorders will be stopped before saving them into DB.
			// Util then process them normally.
			//if (isTestOrder())			{
			//	return false;
			//}

			return true;
		}



		
		// ============================================
		// Structure of UNB_00_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "UNB7601001002470 7601001358034 UEWAUF1                                           "
		// "00UNOB203721440                           PH                76539419                           PH                14070908352090444"
		// "00UNOB203721440                           PH                76539419                           PH                14070908402090458"
		// "00UNOB203721440                           PH                PHOENIXQAS                         PH                14070908402090458"		
		//  ^ ^    ^       ^                                            ^       ^                                            ^     ^   ^             ...  ^^       
		//  a b    c       d                                            e       f                                            g     h   i             ...  jx
		//
		// Field a= Line-Indicator		Offset  0,  length  2
		// Field b= UNOB2 fixed text:	Offset  2,  length  5 (mandatory, field UNB-SYNTAX,  must contain "UNOB2")
		// Field c= Supplier-BFA  :		Offset  7,  length  8 (mandatory, field UNB-ABSBEZ, = BFA number of supplier)
		// Field d= Filler  :			Offset 15,  length 45 (mandatory, field UNB-FILLER, spaces to fill up the line)
		// Field e= Wholesaler-BFA:		Offset 60,  length  8 (mandatory, field UNB-EMPFBEZ, = BFA number of PHX-wholesale (76539419) 
		//																					or address QAS-mailbox (PHOENIXQAS-only first 8 chars relevant) )
		// Field f= Filler  :			Offset 68,  length 45 (mandatory, field UNB-FILLER, spaces to fill up the line)
		// Field g= creation date:      Offset 113, length 6  (mandatory, field UNB-DATUM, Format JJMMDD)
		// Field h= creation time:      Offset 119, length 4  (mandatory, field UNB-ZEIT,  Format HHMM)
		// Field i= Filler  :			Offset 123, length 81 (optional,  field UNB-FILLER, spaces to fill up the line)
		// Field j= Test-Flag:			Offset 204, length 1  (optional), field UNB-TEST-X, ="1": ist Testübermittlung, leer sonst)
		// Field x= Filler  :			Offset 205, length 345 (optional, spaces to fill up the line to 550 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Unb_00_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Unb_00_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "UNB_00-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No UNB_00-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				m_FixedSyntax	= lineStr.substr(  2,  5 );		// extract field b		UNB-SYNTAX
				m_SupplierBfa	= lineStr.substr(  7,  8 );		// extract field c		UNB-ABSBEZ
				m_WholesalerBfa = lineStr.substr( 60,  8 );		// extract field e		UNB-EMPFBEZ

				// If date and time are necesary for German Edi-Input, we have to check them and to transform it into a date format
				// Until now (in PBS999) the date and time was not used. So do we in csc_batches_edi_input. As long as we don't use it, we ignore them.
				m_CreationDate  = lineStr.substr(113,  6 );		// extract field g		UNB-DATUM
				// check for numerical string
				if (!m_CreationDate.isDigit())
				{
					std::stringstream str;
					str << "UNB_01-line contains unprocessable UNB-DATUM (date) field (not numerical): \"" << m_CreationDate << "\" " << lineStr << std::endl;
					// Really an exception for an unused field?? -> No! Just log it.
					//throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

				}

				m_CreationTime  = lineStr.substr(119,  4 );		// extract field h		UNB-ZEIT
				// check for numerical string
				if (!m_CreationTime.isDigit())
				{
					std::stringstream str;
					str << "UNB_01-line contains unprocessable UNB-ZEIT (time) field (not numerical): \"" << m_CreationTime << "\" " << lineStr << std::endl;
					// Really an exception for an unused field?? -> No! Just log it.
					//throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				}

				// Extract optional TestOrderFlag if existent
				if ( 205 <= lineStr.length() ) {					// 205 = offset + 1 for testOrderFlag
					basar::I18nString strTestOrderFlag;

					strTestOrderFlag	= lineStr.substr( 204,  1 );		// extract field j     UNB-TEST-X
					if(strTestOrderFlag == "1" ) {	
						m_TestOrderFlag		=	true;
					}
					else {
						m_TestOrderFlag		=	false;
					}
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable UNB-00 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Unb_00_Line::resetMembers()
		{
			m_FixedSyntax	= "";		
			m_SupplierBfa	= "";		
			m_WholesalerBfa = "";		
			m_CreationDate	= "";
			m_CreationTime	= "";
			m_TestOrderFlag	= false;	
		}

	} // namespace line
} // namespace domMod