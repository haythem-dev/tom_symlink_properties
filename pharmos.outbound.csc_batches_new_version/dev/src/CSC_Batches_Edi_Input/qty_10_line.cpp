//-------------------------------------------------------------------------------------
/*! \file  qty_10_line.cpp
*  \brief  subclass of EDIFACTLineBase for German 10-Segment (QTY). 
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


#include "loggerpool.h"	// for LoggerPool
#include "abbauwexception.h"
#include "unprocessableexception.h"
#include "qty_10_line.h"
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
		const basar::I18nString	Qty_10_Line::s_LineIndicator = "10";	
		const basar::UInt32		Qty_10_Line::s_MinLength     = 20;	// 3+3+5=11 (bis incl. QTY_MENGE)
															  		// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		Qty_10_Line::Qty_10_Line()
		{
			resetMembers();
		}

		Qty_10_Line::~Qty_10_Line(){}

		EDIFACTLineBasePtr Qty_10_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Qty_10_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Qty_10_Line::getLineTypeEnum()		const
		{
			return en10;
		}
		domMod::line::Qty_10_Line::Qty_10_QualifierEnum Qty_10_Line::getQualifier()	const
		{
			return m_Qualifier;		
		}
		basar::Int32 Qty_10_Line::getQuantity()	const
		{
			return m_Quantity;		
		}

		// Setter
		void	Qty_10_Line::setQuantity(const basar::Int32 qty)		
		{
			m_Quantity = qty;		
		}


		// Qualifier must be enOrderedQty or enChargedQty.
		bool	Qty_10_Line::isQualifierOK()		const
		{
			if (   enChargedQty == m_Qualifier   
				|| enNonChargedQty == m_Qualifier )
			{
				return true;
			}
			return false;
		}



		// The QTY-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		bool	Qty_10_Line::isValidLine() const
		{
			if (!isQualifierOK() )
			{
				return false;
			}

			// Quantity positiv?
			if (0 > m_Quantity )
			{
				return false;
			}

			return true;
		}



		
		// ============================================
		// Structure of DTM_03_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "1021 000000000000003"
		// "10130000000000000010                                                                 "
		//  ^ ^  ^         ^    ^  ^        ...   
		//  a b  c         d    e  x      
		//
		// Field a= Line-Indicator			Offset  0, length  2
		// Field b= QTY_MENGQUAL qualifier:	Offset  2, length  3 (mandatory, QTY-MENGQUAL, must be "21"-ordered quantity or "130"-noncharged quantity
		// Field c= Filler:  				Offset  5, length 10 (           spaces to fill up the line to 80 chars)
		// Field d= Quantity:				Offset 15, length  5 (mandatory, QTY-MENGE, interpreted as ordered quantity or charged quantity
		//                                                                   depending on qualifier 
		// Field e= Filler:					Offset 20, length  3 (optional,  spaces to fill up the line
		// Field x= Filler:  				Offset 23, length 66 (optional,  spaces to fill up the line to 550 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Qty_10_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Qty_10_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			// remark: use abs() to avoid compiler warning on AIX 
			//if( std::abs(s_MinLength) > lineStr.length() )
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "QTY_10-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No QTY_10-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				basar::I18nString strQualifier = lineStr.substr( 2,  3 );// extract field b: Qualifier / interpretation of quantity
				if (   "21 " == strQualifier
					|| " 21" == strQualifier) {
					m_Qualifier = enChargedQty;
				} 
				else if (   "130" == strQualifier) {
					m_Qualifier = enNonChargedQty;
				} 
				else if (strQualifier.length() > 0 ){ 
					m_Qualifier = enOTHER;
				} 
				else { 
					std::stringstream str;
					str << "QTY_10-line without QTY-MENGQUAL (Qualifier) field: " << lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

					m_Qualifier = enUNKNOWN;
				}


				basar::I18nString strQuantity = lineStr.substr( 15,  5 );// extract field d: Quantity 
				// check for numerical string
				if (!strQuantity.isDigit())
				{
					std::stringstream str;
					str << "QTY_10-line contains unprocessable QTY-MENGE field (not numerical): \"" << strQuantity << "\" " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}
				m_Quantity = strQuantity.stoi();						
				if( m_Quantity < 0 )
				{
					std::stringstream str;
					str << "Wrong QTY_10 line: Cannot convert quantity-Field to positive int32: \"" << strQuantity << "\" " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable QTY_10 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Qty_10_Line::resetMembers()
		{
			m_Qualifier		= enUNKNOWN;	// field b
			m_Quantity		= -1;			// field c
		}

	} // namespace line
} // namespace domMod
