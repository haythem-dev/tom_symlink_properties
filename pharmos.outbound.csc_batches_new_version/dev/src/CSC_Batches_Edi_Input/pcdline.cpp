//-------------------------------------------------------------------------------------
/*! \file  pcdline.cpp
*  \brief  subclass of EDIFACTLineBase for PCD-Segment. 
*  \brief  Main purpose: Extract an PCD-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of PCD-lines can be processed within this method	
*  \author P. Kessling
*  \date   27.03.2013
*  history 27.03.13 pk new
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
#include "pcdline.h"

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
		const basar::I18nString	PcdLine::s_LineIndicator = "PCD";
		const basar::I18nString	PcdLine::s_LineQualifier = "12";
		const basar::UInt32		PcdLine::s_MinLength     = 16;	// 3+3+10=11 (incl. Qualifier and percent value)
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		PcdLine::PcdLine()
		{
			resetMembers();
		}

		PcdLine::~PcdLine(){}

		EDIFACTLineBasePtr PcdLine::clone() const 
		{
			return EDIFACTLineBasePtr (new PcdLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		PcdLine::getLineTypeEnum()		const
		{
			return enPCD;
		}
		const basar::cmnutil::Decimal PcdLine::getPercentValue()	const
		{
			return m_PercentValue;		
		}
		domMod::line::PcdLine::PcdBaseEnum PcdLine::getBase()	const
		{
			return m_Base;		
		}


		// Throws: AbbauwException
		bool	PcdLine::isPriceDecrease()		const
		{
			METHODNAME_DEF( PcdLine, isPriceDecrease )  
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
				str << "PCD-line cannot decide Increase or Decrease "  << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}



		bool	PcdLine::isQualifierOK()		const
		{
			if (   true == m_QualifierDiscount )
			{
				return true;
			}
			return false;
		}
		bool	PcdLine::isBaseOK()		const
		{
			if ( enUNKNOWN == m_Base ) {
				return false;
			}
			if ( enOTHER == m_Base )
			{
				return false;
			}

			return true;
		}


		// The PCD-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		// This would mean a bad error.
		bool	PcdLine::isValidLine() const
		{
			if (!isQualifierOK() )
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
		// Structure of PCD data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example: 
		// "PCD12 0000023.00                                                                "
		// "PCD12 0000023.001                                                               "
		//  ^  ^  ^         ^  ^    
		//  a  b  c         d  x      
		//
		// Field a= Line-Indicator		Offset  0, length  3
		// Field b= Qualifier:			Offset  3, length  3 mandatory, QTY-PROZQUAL, must be "12 "-discount (otherwise the value is of unprocessable type)
		// Field c= Percent Value:		Offset  6, length 10 mandatory, QTY-PROZENT, interpreted as price in- or decrease resp. field d
		//                                                               depending on qualifier 
		// Field d= Base:				Offset 16, length  3 mandatory (in reality it is empty!??): "1" = decrease, "2" = increase
		// Field x= Filler:  			Offset 19, length 61 optional,  spaces to fill up the line to 80 chars
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void PcdLine::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( PcdLine, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "PCD-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No PCD-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				//check PCD qualifier -> must be 12
				if (s_LineQualifier != lineStr.substr(3, s_LineQualifier.length()))
				{
					std::stringstream str;
					str << "Wrong PCD qualifier: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException(basar::ExceptInfo(fun, str.str().c_str(), __FILE__, __LINE__));
				}

				// Extract netto data:
				basar::I18nString strQualifierDiscount = lineStr.substr( 3,  3 );// extract field b: QualifierDiscount 
				if (   "12 " == strQualifierDiscount
					|| " 12" == strQualifierDiscount) {
					m_QualifierDiscount = true;
				} 
				else { 

					std::stringstream str;
					str << "PCD-line with wrong PCD-PROZQUAL (Qualifier) field:\"" << strQualifierDiscount << "\" " <<lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

					m_QualifierDiscount = false;
				}

				// in abbauwappl.cpp: basar::cmnutil::Locale::setLocale(basar::cmnutil::Locale::GERMAN_CH);

				const basar::I18nString strPercentValue = lineStr.substr( 6, 10 );// extract field c: Percent 
				m_PercentValue.fromString(strPercentValue);						

																		// Extract field d: "mandatory" unit, if existent
				if ( 16 < lineStr.length() ) {							// > 16: s_MinLength 
					basar::I18nString strBase = lineStr.substr( 16, 3 );

					if (   "1  " == strBase) {
						m_Base = enPriceDecrease;
					} 
					else if ("2  " == strBase) {
						m_Base = enPriceIncrease;
					} 
					else if ("   " == strBase) {
						// default, wenn empty
						m_Base = enPriceDecrease;
					} 
					else { 

						std::stringstream str;
						str << "PCD-line with wrong PCD-Base (Increase/Decrease) field:\"" << strBase << "\" " <<lineStr << std::endl;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

						m_Base = enOTHER;
					}
				}

			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable PCD line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void PcdLine::resetMembers()
		{
			m_QualifierDiscount	= false;		// field b
			m_PercentValue.fromString("0");		// field c
			m_Base				= enUNKNOWN;	// field d
		}

	} // namespace line
} // namespace domMod
