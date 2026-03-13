//-------------------------------------------------------------------------------------
/*! \file  qtyline.cpp
*  \brief  subclass of EDIFACTLineBase for QTY-Segment. 
*  \brief  Main purpose: Extract an QTY-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of QTY-lines can be processed within this method	
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
#include "qtyline.h"
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
		const basar::I18nString	QtyLine::s_LineIndicator = "QTY";	
		const basar::UInt32		QtyLine::s_MinLength     = 11;	// 3+3+5=11 (incl. qualifier and quantity)
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		QtyLine::QtyLine()
		{
			resetMembers();
		}

		QtyLine::~QtyLine(){}

		EDIFACTLineBasePtr QtyLine::clone() const 
		{
			return EDIFACTLineBasePtr (new QtyLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		QtyLine::getLineTypeEnum()		const
		{
			return enQTY;
		}
		domMod::line::QtyLine::QtyQualifierEnum QtyLine::getQualifier()	const
		{
			return m_Qualifier;		
		}
		basar::Int32 QtyLine::getQuantity()	const
		{
			return m_Quantity;		
		}
		const basar::I18nString& QtyLine::getUnit()		const
		{
			return m_Unit;		
		}

		// Qualifier must be enOrderedQty or enChargedQty.
		bool	QtyLine::isQualifierOK()		const
		{
			if (   enOrderedQty == m_Qualifier   
				|| enChargedQty == m_Qualifier )
			{
				return true;
			}
			return false;
		}
		bool	QtyLine::isUnitOK()		const
		{
			// If field unit is set and not empty, it must contain "PCE"
			if ( 0 == m_Unit.length()) {
				return true;
			}
			if ( "PCE" == m_Unit)
			{
				return true;
			}
			if ( "   " == m_Unit) {
				return true;
			}

			return false;
		}


		// The QTY-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		bool	QtyLine::isValidLine() const
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

			if (!isUnitOK() )
			{
				return false;
			}

			return true;
		}



		
		// ============================================
		// Structure of QTY data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example: 
		// "QTY21 00020PCE                                                                  "
		// "QTY47 00010PCE                                                                  "
		// "QTY47 00030                                                                     "
		//  ^  ^  ^    ^  ^    
		//  a  b  c    d  x      
		//
		// Field a= Line-Indicator		Offset  0, length  3
		// Field b= Qualifier:			Offset  3, length  3 (mandatory, QTY-MENGQUAL, must be "21"-ordered quantity or "47"-charged quantity
		// Field c= Quantity:			Offset  6, length  5 (mandatory, QTY-MENGE, interpreted as ordered quantity or charged quantity
		//                                                               depending on qualifier 
		// Field d= Unit:				Offset 11, length  3 (optional,  can be empty. Otherwise it must be "PCE" (pieces)
		// Field x= Filler:  			Offset 14, length 66 (optional,  spaces to fill up the line to 80 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void QtyLine::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( QtyLine, process )  
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
				str << "QTY-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No QTY-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				basar::I18nString strQualifier = lineStr.substr( 3,  3 );// extract field b: Qualifier / interpretation of quantity
				if (   "21 " == strQualifier
					|| " 21" == strQualifier) {
					m_Qualifier = enOrderedQty;
				} 
				else if (   "47 " == strQualifier
						 || " 47" == strQualifier) {
					m_Qualifier = enChargedQty;
				} 
				else if (strQualifier.length() > 0 ){ 
					m_Qualifier = enOTHER;
				} 
				else { 
					std::stringstream str;
					str << "QTY-line without QTY-MENGQUAL (Qualifier) field: " << lineStr << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

					m_Qualifier = enUNKNOWN;
				}


				basar::I18nString strQuantity = lineStr.substr( 6,  5 );// extract field c: Quantity 
				// check for numerical string
				if (!strQuantity.isDigit())
				{
					std::stringstream str;
					str << "QTY-line contains unprocessable QTY-MENGE field (not numerical): \"" << strQuantity << "\" " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}
				m_Quantity = strQuantity.stoi();						
				if( m_Quantity < 0 )
				{
					std::stringstream str;
					str << "Wrong QTY line: Cannot convert quantity-Field to positive int32: \"" << strQuantity << "\" " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}


																	// Extract field d: optional unit, if existent
				if ( 11 < lineStr.length() ) {						// > 11: s_MinLength 
					m_Unit	= lineStr.substr(  11, 3 );				// Extract field d, may be shorter than 3 chars
																	// Then substr copies as many as possible
				}

			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable QTY line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void QtyLine::resetMembers()
		{
			m_Qualifier		= enUNKNOWN;	// field b
			m_Quantity		= -1;			// field c
			m_Unit			= "";			// field d
		}

	} // namespace line
} // namespace domMod
