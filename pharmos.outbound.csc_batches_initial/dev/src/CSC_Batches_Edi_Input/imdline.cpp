//-------------------------------------------------------------------------------------
/*! \file  imdline.cpp
*  \brief  subclass of EDIFACTLineBase for IMD-Segment. 
*  \brief  Main purpose: Extract an IMD-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
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
#include "imdline.h"
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
		const basar::I18nString	ImdLine::s_LineIndicator = "IMD";	
		const basar::UInt32		ImdLine::s_MinLength     = 10;	// 3+7=10 (incl. WABESCH (FixedCommodityDesc) without optional fields)
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		ImdLine::ImdLine()
		{
			resetMembers();
		}

		ImdLine::~ImdLine(){}

		EDIFACTLineBasePtr ImdLine::clone() const 
		{
			return EDIFACTLineBasePtr (new ImdLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		ImdLine::getLineTypeEnum()		const
		{
			return enIMD;
		}
		const basar::I18nString& ImdLine::getArticleDescMF()	const
		{
			return m_ArticleDescMF;		
		}
		const basar::I18nString& ImdLine::getUnitPharmaFormMF()	const
		{
			return m_UnitPharmaFormMF;		
		}
		bool	ImdLine::isFixedCommodityDescOK()		const
		{
			if (   m_FixedCommodityDesc == "PHARMA "
				|| m_FixedCommodityDesc == " PHARMA")		
			{
				return true;
			}
			return false;
		}

		// The IMD-Line has to be valid, otherwise it cannot be inserted into the transfer order
		bool	ImdLine::isValidLine() const
		{
			if (!isFixedCommodityDescOK())
			{
				return false;
			}
			return true;
		}



		
		// ============================================
		// Structure of IMD data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example:  
		// "IMDPHARMA PURSANA FEIGENSI. MIT SORBITOL                                        "
		// "IMDPHARMA MERFEN WAESSERIG.LSG FARBLOS   50X(                                   "
		// "IMDPHARMA FENIPIC PLUS ROLL ON LOTION 8ML CH                                    "
		// "IMDPHARMA CONTRA-SCHMERZ IL 400              FILMTABLETTEN, STELLER D 13/11     "
		//  ^  ^      ^                                  ^     
		//  a  b      c                                  d       
		//
		// Field a= Line-Indicator				Offset  0, length  3
		// Field b= CommodityDesc fixed Text:	Offset  3, length  7 (mandatory, must be "PHARMA")
		// Field c= ArticleDescMF:				Offset 10, length 35 (optional, description of article, defined by and for manufacturer)
		// Field d= UnitPharmaFormMF:			Offset 45, length 35 (optional, description of unit and/or pharmaceuticalform, defined by and for manufacturer)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void ImdLine::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( ImdLine, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "IMD-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No IMD-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:
				m_FixedCommodityDesc	= lineStr.substr(  3,  7 );		// Extract field b

				// Extract optional ArticleDescMF if existent
				if ( 10 < lineStr.length() ) {						// > 10 = s_MinLength + FixedCommodityDesc
					m_ArticleDescMF		= lineStr.substr(  10, 35 );// Extract field c, may be shorter than 35 chars
																	// Then substr copies as many as possible
				}
				// Extract optional UnitPharmaFormMF if existent
				if ( 45 < lineStr.length() ) {						// > 45 = s_MinLength + FixedCommodityDesc + ArticleDescMF
					m_UnitPharmaFormMF	= lineStr.substr(  45, 35 );// Extract field d, may be shorter than 35 chars
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable IMD line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void ImdLine::resetMembers()
		{
			m_FixedCommodityDesc	= "";		// field b
			m_ArticleDescMF			= "";		// field c
			m_UnitPharmaFormMF		= "";		// field d
		}

	} // namespace line
} // namespace domMod

