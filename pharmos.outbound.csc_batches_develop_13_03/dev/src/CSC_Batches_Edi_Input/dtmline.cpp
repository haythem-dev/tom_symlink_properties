//-------------------------------------------------------------------------------------
/*! \file  dtmline.cpp
*  \brief  subclass of EDIFACTLineBase for DTM-Segment. 
*  \brief  Main purpose: Extract an DTM-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author Jia Li ,P. Kessling
*  \date   02.01.2013
*  history 22.03.13 pk rework
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

#include <libbasarcmnutil_datetime.h> 

#include "loggerpool.h"	// for LoggerPool
#include "abbauwexception.h"
#include "unprocessableexception.h"
#include "dtmline.h"

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
		const basar::I18nString	DtmLine::s_LineIndicator = "DTM";	
		const basar::UInt32		DtmLine::s_MinLength     = 6;	// 3+3=6 (incl. DATCODE ohne DTM-Datum und DTM-Form \n\r)
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		DtmLine::DtmLine()
		{
			resetMembers();
		}

		DtmLine::~DtmLine(){}

		EDIFACTLineBasePtr DtmLine::clone() const 
		{
			return EDIFACTLineBasePtr (new DtmLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		DtmLine::getLineTypeEnum()		const
		{
			return enDTM;
		}
		const basar::Date&	DtmLine::getDate()	const	// Date in format YYYYMMDD or NULL_DATE
		{
			return m_Datum;		
		}


		// Datcode=137, orderdate, VX27:AUFDAT, date of reading from EDIBox
		bool	DtmLine::isOrderDate()		const
		{
			if (m_Datcode == "137")		
			{
				return true;
			}
			return false;
		}

		// Datcode=4  , --not used for ABBA-UW ---, Datum der Bestellung beim Kunden
		bool	DtmLine::isCustomerOrderDate()	const	
		{
			if (m_Datcode == "4  ")			// TESTEST: Ist diese Abfrage ok? "4" besser?
			{
				return true;
			}
			return false;
		}

		// Datcode=2  , edideliverydate    , VX27:TERM, gewuenschter Liefertermin
		bool	DtmLine::isEdiDeliveryDate()	const	
		{
			if (m_Datcode == "2  ")			// TESTEST: Ist diese Abfrage ok? "2" besser?
			{
				return true;
			}
			return false;
		}

		// Datcode=209, deferredpaymentdate, VX27:Valuta, Zahlungsziel
		bool	DtmLine::isDeferredPaymentDate()	const	
		{
			if (m_Datcode == "209")		
			{
				return true;
			}
			return false;
		}


		// The DTM-Line has to be valid, otherwise the currently read date cannot be interpreted
		bool	DtmLine::isValidLine() const
		{
			if (   isOrderDate() 
				|| isCustomerOrderDate() 
				|| isEdiDeliveryDate() 
				|| isDeferredPaymentDate() )
			{
				if (   m_Datum.isValid() 
					|| m_Datum == basar::cmnutil::Date::NULL_DATE )		// maybe empty!
				{
					return true;
				}
			}
			return false;
		}


		
		// ============================================
		// Structure of DTM data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Examples:  
		// "DTM13720120709102                                                               "
		// "DTM20920120709102                                                               "
		// "DTM2  20120709102                                                               "
		//  ^  ^  ^       ^  ^     
		//  a  b  c       d  x       
		//
		// Field a= Line-Indicator		Offset  0, length  3
		// Field b= DatCode:			Offset  3, length  3 (mandatory, must be "137", "4  ", "2  " or "209")
		// Field c= Datum:				Offset  6, length  8 (optional, date in format YYMMDD, for kind of date see DatCode)
		// Field d= Datform:			Offset 14, length  3 (optional, "102": dateformat is YYYYMMDD, should be always this format)
		// Field x= Filler  :			Offset 17, length 63 (optional, spaces to fill up the line to 80 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void DtmLine::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( DtmLine, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "DAT-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No DTM-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				m_Datcode	= lineStr.substr(  3,  3 );				// Extract field b

				// Extract optional Datum if existent
				if ( 14 <= lineStr.length() ) {						// > 14 because 14 = length of s_MinLength (3) + Datcode (3) + Datum(8) 

					basar::I18nString strYYYYMMDD = lineStr.substr( 6,  8 );				// extract field e: year,month,day

					// check for numerical string
					if (!strYYYYMMDD.isDigit())
					{
						std::stringstream str;
						str << "DTM-line contains unprocessable date field (not numerical): \"" << strYYYYMMDD << "\" " << lineStr << std::endl;
						throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					const basar::Int32 yyyymmdd = strYYYYMMDD.stoi();

					m_Datum.setDate(yyyymmdd);

					// Invalid date has been read
					if(!m_Datum.isValid() ) {	
						std::stringstream str;
						str << "Unprocessable DTM line - Invalid date " << strYYYYMMDD<< ": " << lineStr << std::endl;
						throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}
				}

				// Extract optional DatForm if existent
				if ( 14 < lineStr.length() ) {						// > 14 because 14 = length of s_MinLength (3) + Datcode (3) + Datum (8) 
					m_Datform	= lineStr.substr( 14,  3 );			// Extract field d
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable DTM line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void DtmLine::resetMembers()
		{
			m_Datcode	= "";									// field b
			m_Datum		= basar::cmnutil::Date::NULL_DATE;		// field c
			m_Datform	= "";									// field d
		}

	} // namespace line
} // namespace domMod


