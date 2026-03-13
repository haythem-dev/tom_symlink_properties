//-------------------------------------------------------------------------------------
/*! \file  linline.cpp
*  \brief  subclass of EDIFACTLineBase for LIN-Segment. 
*  \brief  Main purpose: Extract an LIN-Line from a UEWAUF-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of LIN-lines can be processed within this method	
*  \author Jia Li ,P. Kessling
*  \date   02.01.2013
*  history 26.03.13 pk rework
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
#include "linline.h"
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
		const basar::I18nString	LinLine::s_LineIndicator = "LIN";	
		const basar::UInt32		LinLine::s_MinLength     = 3;	// 3=3 (only service-segment, rest is optional according to UEWAUF-documentation)
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		LinLine::LinLine()
		{
			resetMembers();
		}

		LinLine::~LinLine(){}

		EDIFACTLineBasePtr LinLine::clone() const 
		{
			return EDIFACTLineBasePtr (new LinLine(*this) );
		
		}

		// Getter 
		LineTypeEnum		LinLine::getLineTypeEnum()		const
		{
			return enLIN;
		}
		basar::Int32 LinLine::getPositionno()	const
		{
			return m_Positionno;		
		}
		const basar::I18nString& LinLine::getArticleno()	const
		{
			return m_Articleno;		
		}
		domMod::line::LinLine::LinCodeEnum LinLine::getArticlenoCode()	const
		{
			return m_ArticlenoCode;		
		}

		// Checker												isEan()				const;
		bool		LinLine::isEan()		const
		{
			if ( enEanCode == getArticlenoCode() )
			{
				return true;
			}
			return false;
		}
		bool		LinLine::isManufacturerDefined()		const
		{
			if ( enMfCode == getArticlenoCode() )
			{
				return true;
			}
			return false;
		}
		bool		LinLine::isPzn()		const
		{
			if ( enPznCode == getArticlenoCode() )
			{
				return true;
			}
			return false;
		}



		// The LIN-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		bool	LinLine::isValidLine() const
		{
			// Only two articleno-codetypes are possible for ABBA-UW
			// m_ArticlenoCode is enUnknown as long as line is not processed.
			if (   enEanCode == m_ArticlenoCode		// Code for articleno is  EAN or
				|| enMfCode  == m_ArticlenoCode		// Code for articleno is  defined by manufacturer or
				|| enPznCode == m_ArticlenoCode)	// Code for articleno is PZN
			{
						return true;
			}

			// Watch out: LINlines without articleno are permitted!

			return false;
		}



		
		// ============================================
		// Structure of LIN data-Line: Docu "EDIFACT-UEWAUF (CH) DECHU-Inhouse-file", Version 06.08.2012
		// Example: 
		// "LIN0000017680516821141                                                          "
		// "LIN0000027680516821141 EN                                                       "
		// "LIN000003My_Own Artno  MF                                                       "
		//  ^  ^     ^             ^  ^     
		//  a  b     c             d  x       
		//
		// Field a= Line-Indicator		Offset  0, length  3
		// Field b= Positionnumber:		Offset  3, length  6 (optional,	 current positionnumber in this transferorder
		// Field c= Article number:		Offset  9, length 14 (optional,  article number, encoded with respect to field ArticlenoCode
		// Field d= ArticlenoCode:	    Offset 23, length  3 (optional,  empty ("   ") or "EN"= EAN-Coding, 
		//                                                    "MF"=manufacturers own coding, "ZZZ"=PZN-Coding
		//                                                    nothing else is permitted
		// Field x= Filler:  			Offset 26, length 54 (optional,  spaces to fill up the line to 80 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void LinLine::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( LinLine, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "LIN-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No LIN-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:

				// Extract optional positionnumber if existent
				if ( 3 < lineStr.length() ) {						// Extract field b, may be shorter than 3 chars
																	// Then substr copies as many as possible
					basar::I18nString strPositionno = lineStr.substr( 3,  6 );// extract field c: Quantity 
					// check for numerical string
					if ("      " == strPositionno)
					{
						// Empty position number is ok
						m_Positionno =0;
					}
					else {
						if (!strPositionno.isDigit())
						{
							std::stringstream str;
							str << "LIN-line contains unprocessable LIN-POSNR field (not numerical): \"" << strPositionno << "\" " << lineStr << std::endl;
							throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
						}
						m_Positionno = strPositionno.stoi();						
						if( m_Positionno < 0 )
						{
							std::stringstream str;
							str << "Wrong LIN line: Cannot convert LIN-POSNR-Field to positive int32: \"" << strPositionno << "\" " << lineStr << std::endl;
							throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
						}
					}
				}
				// Extract optional articlenumber if existent
				if ( 9 < lineStr.length() ) {						// > 9: s_MinLength + posno
					m_Articleno	= lineStr.substr(  9, 14 );			// Extract field c, may be shorter than 14 chars
																	// Then substr copies as many as possible
				}
				// Extract optional articleno-code if existent
				// ArticlenoCode must be empty ("   "), "EN " or "MF ". 
				if ( 23 < lineStr.length() )						// > 23: s_MinLength_3 + posno_6 + articleno_14
				{						
					basar::I18nString strArticlenoCode = lineStr.substr( 23,  3 );// extract field d 

					if ("   " == strArticlenoCode) {
						m_ArticlenoCode = enEanCode;
					} 
					else if ("EN " == strArticlenoCode) {
						m_ArticlenoCode = enEanCode;
					} 
					else if ("MF " == strArticlenoCode) {
						m_ArticlenoCode = enMfCode;
					} 
					else if ("ZZZ" == strArticlenoCode) {
						m_ArticlenoCode = enPznCode;
					} 
					else if (0 == strArticlenoCode.length()){ 
						// This is an optional field
						m_ArticlenoCode = enEanCode;
					} 
					else { 
						// Not permitted: error!
						m_ArticlenoCode = enOTHER;

						std::stringstream str;
						str << "LIN-line with unknown LIN-ARTNRC (Articleno-Code) field: \"" << strArticlenoCode << "\"  " << lineStr;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

						m_ArticlenoCode = enUNKNOWN;
					}
				}
				else 					// == 23: articleno is in line, code is missing: Then it is EAN-Code
										// <23  : articleno is not in line, code is missing: Code "would be" EAN-Code
				{	
						m_ArticlenoCode = enEanCode;
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable LIN line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void LinLine::resetMembers()
		{
			m_Positionno	= 0;			// field b
			m_Articleno		= "";			// field c
			m_ArticlenoCode	= enUNKNOWN;	// field d
		}

	} // namespace line
} // namespace domMod
