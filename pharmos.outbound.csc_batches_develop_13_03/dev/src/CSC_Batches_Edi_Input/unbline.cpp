//-------------------------------------------------------------------------------------
/*! \file  unbline.cpp
*  \brief  subclass of EDIFACTLineBase for UNB-Segment. 
*  \brief  Main purpose: Extract an UNB-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author Jia Li ,P. Kessling
*  \date   02.01.2013
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
#include "unbline.h"

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

		const basar::I18nString	UnbLine::s_LineIndicator = "UNB";	
		const basar::UInt32		UnbLine::s_MinLength     = 37;	// 3+14+14+6=37 (incl. UEWAUF ohne Testkennzeichen und \n\r)
																// Minimum length of netto data

    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
															
		UnbLine::UnbLine()
		{
			resetMembers();
		}

		UnbLine::~UnbLine(){}

		EDIFACTLineBasePtr UnbLine::clone() const 
		{
			return EDIFACTLineBasePtr (new UnbLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		UnbLine::getLineTypeEnum()		const
		{
			return enUNB;
		}
		const basar::I18nString& UnbLine::getSupplierEan()	const
		{
			return m_SupplierEan;		
		}
		const basar::I18nString& UnbLine::getWholesalerEan()	const
		{
			return m_WholesalerEan;		
		}


		bool	UnbLine::isUewauf()		const
		{
			if (m_FixedUewauf == "UEWAUF")		
			{
				return true;
			}

			return false;
		}
		bool	UnbLine::isTestOrder()	const
		{
			if(m_TestOrderFlag){
				return true;
			}
			return false;
		}

		// The UNB-Line has to be valid to start a new order.
		bool	UnbLine::isValidLine() const
		{
			if (!isUewauf())
			{
				std::stringstream str;
				str << "Error: UNB-line didn't pass syntax check (UEWAUF)!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

				return false;
			}

			// Testorders will be stopped before saving them into DB.
			// Util then process them normally.
			//if (isTestOrder())			{
			//	return false;
			//}
			
			// In CH  wholesaleEAN must be Amedis. Otherwise this transferOrder 
			// is not to be handled by Phoenix.
			//
			// Remark for NotCH / ToDo: 
			// Because ABBA-UW is developed only for CH by now and paid by CH, 
			// we check this condition in a simple manner: The CSC_Batches_Edi_Input.exe
			// doesnt work for countrycode-Cmdline-Parameters other than CH yet.
			// So, at the moment we can be sure that we are in CH.
			// If other countries want to use CSC_Batches_Edi_Input.exe: 
			// Implement a more general solution, e.g. as a parameter.

			if (m_WholesalerEan != PHOENIX_AMEDIS_CH_BFA) {
				// TransferOrder is not dedicated to wholesaler AMEDIS/CH
				std::stringstream str;
				str << "Error: Wrong wholesaler-EAN: " << m_WholesalerEan.c_str() << ". Should be " << PHOENIX_AMEDIS_CH_BFA;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

				return false;
			}

			return true;
	}



		
		// ============================================
		// Structure of UNB data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example:  
		// "UNB7601001002470 7601001358034 UEWAUF1                                           "
		//  ^  ^             ^             ^     ^^ 
		//  a  b             c             d     ex
		//
		// Field a= Line-Indicator		Offset  0, length  3
		// Field b= Supplier-EAN  :		Offset  3, length 14 (last char = blank)
		// Field c= Wholesaler-EAN:		Offset 17, length 14 (last char = blank)
		// Field d= UEWAUF fixed Text:	Offset 31, length 6 
		// Field e= Test-Flag:			Offset 37, length 1	 (optional)
		// Field x= Filler  :			Offset 38, length 42 (optional, spaces to fill up the line to 80 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void UnbLine::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Unbline, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "UNB-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No UNB-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				m_SupplierEan	= lineStr.substr(  3, 14 );		// extract field b
				m_WholesalerEan = lineStr.substr( 17, 14 );		// extract field c
				m_FixedUewauf	= lineStr.substr( 31,  6 );		// extract field d

				// Extract optional TestOrderFlag if existent
				if ( 38 <= lineStr.length() ) {					// 38 = s_MinLength +1 for testOrderFlag
					basar::I18nString strTestOrderFlag;

					strTestOrderFlag	= lineStr.substr( 37,  1 );		// extract field e
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
					str << "Unprocessable UNB line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void UnbLine::resetMembers()
		{
			m_SupplierEan	= "";		// field b
			m_WholesalerEan = "";		// field c
			m_FixedUewauf	= "";		// field d
			m_TestOrderFlag	= false;	// field e 
		}

	} // namespace line
} // namespace domMod