//-------------------------------------------------------------------------------------
/*! \file  dtm_03_line.cpp
*  \brief  subclass of EDIFACTLineBase for German 03-Segment (DTM).  
*  \brief  Main purpose: Extract an DTM-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 28.10.14  pk new
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
#include "dtm_03_line.h"

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
		const basar::I18nString	Dtm_03_Line::s_LineIndicator = "03";	
		const basar::UInt32		Dtm_03_Line::s_MinLength     = 5;	// 2+3=5 (incl. DATCODE ohne DTM-Datum und DTM-Form \n\r)
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		Dtm_03_Line::Dtm_03_Line()
		{
			resetMembers();
		}

		Dtm_03_Line::~Dtm_03_Line(){}

		EDIFACTLineBasePtr Dtm_03_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Dtm_03_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Dtm_03_Line::getLineTypeEnum()		const
		{
			return en03;
		}
		const basar::Date&	Dtm_03_Line::getDate()	const	// Date in format YYYYMMDD or NULL_DATE
		{
			return m_Datum;		
		}


		// Datcode=137, orderdate, VX27:AUFDAT, date of reading from EDIBox
		bool	Dtm_03_Line::isOrderDate()		const
		{
			if (m_Datcode == "137")		
			{
				return true;
			}
			return false;
		}

		// Datcode=2  , edideliverydate    , VX27:TERM, gewuenschter Liefertermin
		bool	Dtm_03_Line::isEdiDeliveryDate()	const	
		{
			if (   m_Datcode == "2  "
				|| m_Datcode == " 2 "
				|| m_Datcode == "  2")		
			{
				return true;
			}
			return false;
		}

		// Datcode=209, deferredpaymentdate, VX27:Valuta, Zahlungsziel
		bool	Dtm_03_Line::isDeferredPaymentDate()	const	
		{
			if (m_Datcode == "209")		
			{
				return true;
			}
			return false;
		}

		// Datcode=ZZZ  , VX27:Valuta in 3 month, Zahlungsziel heute in drei Monaten
		bool	Dtm_03_Line::is3MonthDeferredPaymentDate()	const	
		{
			if (m_Datcode == "ZZZ")			
			{
				return true;
			}
			return false;
		}

		// Datcode= '117' or '128'  , both are invalid datcodes, but they have to be tolerated, logged as "Unknown Codes" and ignored.
		bool	Dtm_03_Line::isToleratedDatcodeDate()	const	
		{
			if (m_Datcode == "117" || m_Datcode == "128")			
			{
				return true;
			}
			return false;
		}



		// The DTM-Line has to be valid, otherwise the currently read date cannot be interpreted
		bool	Dtm_03_Line::isValidLine() const
		{
			// Check DATFORM-field: if existent it must contain "102"
			if (   0 < m_Datform.length() 
				&& m_Datform != "102")
			{
				return false;
			}

			// Check type of date
			if (   isToleratedDatcodeDate() )
			{
					return true;
			}

			if (   isOrderDate() 
				|| isEdiDeliveryDate() 
				|| isDeferredPaymentDate() 
				|| is3MonthDeferredPaymentDate() )
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
		// Structure of DTM_03_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "0313720140709                           102"
		// "032  20140709                           102         blabla  "
		//  ^ ^  ^ ^     ^                          ^  ^        
		//  a b  c d     e                          f  x    
		//
		// Field a= Line-Indicator		Offset  0, length  2
		// Field b= DatCode:			Offset  2, length  3 (mandatory, must be "137", "2  " , "209" or "ZZZ" or '117' or '128')
		// Field c= Datum-JH:			Offset  5, length  2 (optional, century in format YY, z.B. "20", for kind of date see DatCode)
		// Field d= Datum-JJMMTT:		Offset  7, length  6 (optional, date in format YYMMDD, for kind of date see DatCode)
		// Field e= Filler  :			Offset 13, length 27 (optional, spaces to fill up the line )
		// Field f= Datform:			Offset 40, length  3 (optional, "102": dateformat is YYYYMMDD, should be always this format or empty)
		// Field x= Filler  :			Offset 43, length 507 (optional, spaces to fill up the line to 550 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Dtm_03_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Dtm_03_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "DAT_03-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No DTM_03-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				m_Datcode	= lineStr.substr(  2,  3 );				// Extract field b

				// For DATCODE=ZZZ: Calculate deferredpaymentdate = today + 3 month
				if (m_Datcode == "ZZZ")			
				{
					basar::cmnutil::Date dtTodayPlus3Month;				// Initialized with current date

					dtTodayPlus3Month.addDays(91);						// Add 91 days   (Add 3 month is difficult: 30.11+3 MM= 30.Feb) 
																		// If this is not ok: try with basar::cmnutil::timespan
																		// or do date-arithmetics (+3MM) until a legal date was found
					basar::Int32 intTodayPlus3Month = dtTodayPlus3Month.getDate();	// Format of int32: YYYYMMDD
					
					m_Datum.setDate(intTodayPlus3Month);
				}
				// Tolerated but ignored Datcodes
				else if (m_Datcode == "117" || m_Datcode == "128" )		
				{
					return;
				}
				// Wenn nicht ZZZ oder 117 oder 128: Extract optional Datum if existent, read JH-field and date-field together:   20140719 instead of 20 and 140719
				else if ( 13 <= lineStr.length() ) {						// > 13 because 13 = length of s_MinLength (2) + Datcode (3) + Datum(8) 

					basar::I18nString strYYYYMMDD = lineStr.substr( 5,  8 );				// extract field c and d: year,month,day

					// check for numerical string
					if (!strYYYYMMDD.isDigit())
					{
						std::stringstream str;
						str << "DTM_03-line contains unprocessable date field (not numerical): \"" << strYYYYMMDD << "\" " << lineStr << std::endl;
						throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					const basar::Int32 yyyymmdd = strYYYYMMDD.stoi();

					m_Datum.setDate(yyyymmdd);

					// Invalid date has been read
					if(!m_Datum.isValid() ) {	
						std::stringstream str;
						str << "Unprocessable DTM_03 line - Invalid date " << strYYYYMMDD<< ": " << lineStr << std::endl;
						throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}
				}


				// Extract optional DatForm if existent
				if ( 41 <= lineStr.length() ) {						// >= 41 because 41 = offset of DATFORM-field + 1
					m_Datform	= lineStr.substr( 40,  3 );			// Extract field d
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable DTM_03 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Dtm_03_Line::resetMembers()
		{
			m_Datcode	= "";									// field b
			m_Datum		= basar::cmnutil::Date::NULL_DATE;		// field c
			m_Datform	= "";									// field d
		}

	} // namespace line
} // namespace domMod


