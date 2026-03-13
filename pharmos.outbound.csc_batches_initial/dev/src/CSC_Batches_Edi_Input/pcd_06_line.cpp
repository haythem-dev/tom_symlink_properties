//-------------------------------------------------------------------------------------
/*! \file  pcd_06_line.cpp
*  \brief  subclass of EDIFACTLineBase for German 06-Segment (PCD). 
*  \brief  Main purpose: Extract an DTM-Line from a GERMAN .inh-flatfile-line
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


#include "loggerpool.h"	// for LoggerPool
#include "abbauwexception.h"
#include "unprocessableexception.h"
#include <libbasarcmnutil_decimal.h>		// for datatype decimal
#include <libbasarcmnutil_locale.h>			// for datatype decimal
#include "pcd_06_line.h"

#include <libbasarcmnutil_locale.h>		// TESTEST

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
		const basar::I18nString	Pcd_06_Line::s_LineIndicator = "06";	
		const basar::UInt32		Pcd_06_Line::s_MinLength     = 2;	// 2=2 (only line qualifier, rest ist optional)
																    // Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		Pcd_06_Line::Pcd_06_Line()
		{
			resetMembers();
		}

		Pcd_06_Line::~Pcd_06_Line(){}

		EDIFACTLineBasePtr Pcd_06_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Pcd_06_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Pcd_06_Line::getLineTypeEnum()		const
		{
			return en06;
		}
		const basar::cmnutil::Decimal Pcd_06_Line::getPercentValue()	const
		{
			return m_PercentValue;		
		}
		// field PCD_ZUABSCHLAG
		domMod::line::Pcd_06_Line::Pcd_06_BaseEnum Pcd_06_Line::getBase()	const
		{
			return m_Base;		
		}

		// Is "Abschlag"?
		// Throws: AbbauwException
		bool	Pcd_06_Line::isPriceDecrease()		const
		{
			METHODNAME_DEF( Pcd_06_Line, isPriceDecrease )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			if ( enPriceDecrease == m_Base ) {
				return true;
			}
			else if ( enPriceIncrease == m_Base )
			{
				return false;
			}
			else 
			{
				std::stringstream str;
				str << "PCD_06-line cannot decide Increase or Decrease "  << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		bool	Pcd_06_Line::isBaseOK()		const
		{
			if ( enUNKNOWN == m_Base ) {
				return false;
			}
			if ( enOTHER == m_Base )
			{
				// field not assigned , ok because it is optional
				return true;
			}

			return true;
		}

		bool	Pcd_06_Line::isCodelistOK()		const
		{
			// Doku does not tell the rules to fill this field.
			// TODO:  So just return true until we get more info
			return true;
		}



		// The PCD-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		// This would mean a bad error.
		bool	Pcd_06_Line::isValidLine() const
		{
			if (!isCodelistOK() )
			{
				return false;
			}
			if (!isBaseOK() )
			{
				return false;
			}

			return true;
		}



		
		// ============================================
		// Structure of DTM_03_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "061        35                  "
		//  ^ ^  ^       ^  ^   ^ 
		//  a b  c       d  f   x      
		//
		// Field a= Line-Indicator		Offset  0, length  2
		// Field b= Base:				Offset  2, length  3 optional: "1  " = decrease, "2  " = increase, field PCD_ZUABSCHLAG
		// Field c= Percent Value:		Offset  5, length  8 optional, QTY-PROZENT, interpreted as price in- or decrease resp. field b
		// Field d= Filler:  			Offset 13, length  3 optional,  spaces to fill up the line 
		// Field e= Codelist:			Offset 16, length  3 optional: content not documented
		// Field f= Filler:  			Offset 19, length  3 optional,  spaces to fill up the line 
		// Field x= Filler:  			Offset 22, length 528 optional,  spaces to fill up the line to 550 chars
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Pcd_06_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Pcd_06_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "PCD_06-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No PCD_06-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				// Extract field b: optional PCD_ZUABSCHLAG.
				if ( 3 <= lineStr.length() ) {							// >= 3: s_MinLength +1
					basar::I18nString strBase = lineStr.substr( 2, 3 );

					if (   "1  " == strBase
						 ||" 1 " == strBase
						 ||"  1" == strBase	
						 ||"1 " == strBase	
						 ||" 1" == strBase
						 ||"1" == strBase	 ) {
						m_Base = enPriceDecrease;
					} 
					else if (   "2  " == strBase
						 ||" 2 " == strBase
						 ||"  2" == strBase	
						 ||"2 " == strBase	
						 ||" 2" == strBase
						 ||"2" == strBase	 ) {

						 // Probably PBS999 ignored complete PCD-segment, when PCD_ZUABSCHLAG = 2.
						 // We (csc_Batches_edi_nput) do not ignore because this is not EDIFact conform.
						 std::stringstream str;
						 str << "PCD_06-line with PCD_ZUABSCHLAG=2 (Price-Increase)" << std::endl;
						 BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

						 m_Base = enPriceIncrease;
					} 
					// Sonderfall 9: PCD_ZUABSCHLAG was handled as mandatory in PBS999. We do also.
					//else if ("   " == strBase
					//	   ||"  " == strBase
					//	   ||" " == strBase	) {
					//	// default, wenn empty
					//	m_Base = enPriceDecrease;
					//} 
					else { 

						std::stringstream str;
						str << "PCD_06-line with wrong PCD-Base (Increase/Decrease) field:\"" << strBase << "\" " <<lineStr << std::endl;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

						m_Base = enOTHER;
					}
				}
				else
				{
					// optional field is not set
					m_Base = enOTHER;
				}

				// Wichtig: Setze für Parameter DE1/DE2 in abbauwappl.cpp: basar::cmnutil::Locale::setLocale(basar::cmnutil::Locale::GERMAN_DE);
				if ( 6 <= lineStr.length() ) {							// >= 6: offset +1

					const basar::I18nString strPercentValue = lineStr.substr( 5, 8 );// extract field c: Percent 

					// The PCD-06 line contains this format: <3.05>. Locale German_DE needs <3,05>. The latter is no more numeric.
					//   orig, verlangt in GERMAN_DE 3,05: m_PercentValue.fromString(strPercentValue);		
					m_PercentValue.fromNonLocString(strPercentValue);			// NonLocString arbeitet mit technischem Format, z.B. "3.05", nicht "3,05"

					// for Debug reasons only
					basar::Int16 right = m_PercentValue.getAllDecimalPlaces();
					basar::I18nString dbgstring = m_PercentValue.toNonLocString(right);   // NonLocString: Decimal-sign is always "full stop"
					// end Debug reasons

				}
				

				if ( 17 <= lineStr.length() ) {							// >= 17: offset +1

					m_Codelist = lineStr.substr( 16,  3 );// extract field e: QualifierDiscount 
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable PCD_06 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Pcd_06_Line::resetMembers()
		{

			m_Base				= enUNKNOWN;	// field b
			m_PercentValue.fromString("0");		// field c
			m_Codelist			= "";			// field e		
		}

	} // namespace line
} // namespace domMod
