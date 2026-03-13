//-------------------------------------------------------------------------------------
/*! \file  lin_08_line.cpp
*  \brief  subclass of EDIFACTLineBase for German 08-Segment (LIN). 
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
#include "ediline_definitions.h"
#include "lin_08_line.h"
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
		const basar::I18nString	Lin_08_Line::s_LineIndicator = "08";	
		const basar::UInt32		Lin_08_Line::s_MinLength     = 2;	// 2=2 (only service-segment (LIN-SATZART), rest is optional according to DEDEU-documentation)
																// Minimum length of netto data
															
    //--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		Lin_08_Line::Lin_08_Line()
		{
			resetMembers();
		}

		Lin_08_Line::~Lin_08_Line(){}

		EDIFACTLineBasePtr Lin_08_Line::clone() const 
		{
			return EDIFACTLineBasePtr (new Lin_08_Line(*this) );
		
		}

		// Getter 
		LineTypeEnum		Lin_08_Line::getLineTypeEnum()		const
		{
			return en08;
		}
		basar::Int32 Lin_08_Line::getPositionno()	const
		{
			return m_Positionno;		
		}
		const basar::I18nString& Lin_08_Line::getArticleno()	const
		{
			return m_Articleno;		
		}
		domMod::line::Lin_08_Line::LinCodeEnum Lin_08_Line::getArticlenoCode()	const
		{
			return m_ArticlenoCode;		
		}

		// Checker												isEan()				const;
		bool		Lin_08_Line::isEan()		const
		{
			if ( enEanCode == getArticlenoCode() )
			{
				return true;
			}
			return false;
		}
		bool		Lin_08_Line::isPzn()		const
		{
			if ( enPznCode == getArticlenoCode() )
			{
				return true;
			}
			return false;
		}



		// The LIN-Line has to be valid, otherwise it cannot be inserted into a transfer order.
		bool	Lin_08_Line::isValidLine() const
		{
			// Watch out: LINlines without articleno are permitted!
			if ("" == m_Articleno)
			{
				return true;
			}


			// Only two articleno-codetypes are possible for ABBA-UW
			// m_ArticlenoCode is enUnknown as long as line is not processed.
			if (   enEanCode == m_ArticlenoCode		// Code for articleno is  EAN or
				|| enPznCode == m_ArticlenoCode)	// Code for articleno is PZN
			{
						return true;
			}
			
			return false;
		}



		
		// ============================================
		// Structure of BGM_02_Line data-Line: Docu "DEDEU Dateibeschreibung (DE) DEDEU-Inhouse-file", Version 29.08.2002
		// Example (each line comprises 550 Bytes, Fillers can contain irrelevant info): 
		// "08000002   04013749                           P"
		// "08000002   12345678901234                     EAN                      "
		//  ^ ^     ^  ^             ^                    ^  ^             ^   ... "      
		//  a b     c  d             e                    f  g             x       
		//
		// Field a= Line-Indicator		Offset  0, length  2
		// Field b= Positionnumber:		Offset  2, length  6 (optional,	 current positionnumber in this transferorder
		// Field c= Filler:  			Offset  8, length  3 (optional,  spaces to fill up the line)
		// Field d= Article number:		Offset 11, length 14 (optional,  article number LIN-ARTNR, encoded with respect to field ArticlenoCode
		// Field e= Filler:  			Offset 25, length 21 (optional,  spaces to fill up the line)
		// Field f= ArticlenoCode:	    Offset 46, length  3 (optional,  LIN_ARTNRC, empty ("   ") or "P"= PZN-Coding or "EAN"=EAN-Coding
		//                                                               "ZZZ" in filler oftenly supplements PZN-Coding
		//                                                               nothing else is permitted
		// Field g= Filler:  			Offset 49, length 14 (optional,  spaces to fill up the line)
		// Field x= Filler:  			Offset 63, length 487 (optional,  spaces to fill up the line to 550 chars)
		// ============================================


		//-------------------------------------------------------------------------------------------------//
		/*! \throw libabbauw::UnprocessableException */
		//-------------------------------------------------------------------------------------------------//
		//
		// /brief: Fills private members from lineStr. 
		// /brief: If impossible: All members are reset to "".
		void Lin_08_Line::process(const std::string& lineStr)
		{
			// METHODNAME_DEF definiert fun, BLOG_TRACE_METHOD loggt den Prozedureintritt im Trace-Level
			METHODNAME_DEF( Lin_08_Line, process )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Reset private members
			resetMembers();
				
			// Basic Syntax checks:
			// check the minimum length of the line for this line type
			if( s_MinLength > lineStr.length() )
			{
				std::stringstream str;
				str << "LIN_08-line too short: " << lineStr << std::endl;
				throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}	
			try 
			{
				// check line type
				if( s_LineIndicator != lineStr.substr(0, s_LineIndicator.length()) )
				{
					std::stringstream str;
					str << "No LIN_08-lineindicator: " << lineStr << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// Extract netto data:

				// Extract optional positionnumber if existent
				if ( 3 <= lineStr.length() ) {						// Extract field b, may be shorter than 3 chars
																	// Then substr copies as many as possible
					basar::I18nString strPositionno = lineStr.substr( 2,  6 );
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
							str << "LIN_08-line contains unprocessable LIN-POSNR field (not numerical): \"" << strPositionno << "\" " << lineStr << std::endl;
							throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
						}
						m_Positionno = strPositionno.stoi();						
						if( m_Positionno < 0 )
						{
							std::stringstream str;
							str << "Wrong LIN_08 line: Cannot convert LIN-POSNR-Field to positive int32: \"" << strPositionno << "\" " << lineStr << std::endl;
							throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
						}
					}
				}

				// Extract optional articlenumber if existent
				if ( 12 <= lineStr.length() ) {						// > 12: offset (LIN_ARTNR+1)
					m_Articleno	= lineStr.substr(  11, 14 );		// Extract field c, may be shorter than 14 chars
																	// Then substr copies as many as possible
				}

				// Extract optional articleno-code if existent
				// ArticlenoCode must be empty ("   "), "EAN" or "P  ". 
				if ( 47 <= lineStr.length() )						// > 47: offset LIN_ARTNRC+1
				{						
					basar::I18nString strArticlenoCode = lineStr.substr( 46,  3 );// extract field d 

					if (   "   " == strArticlenoCode
						|| "  "  == strArticlenoCode
						|| " "   == strArticlenoCode
						|| 0 == strArticlenoCode.length()) {

						// kes 18.2.15: If ARTNRC is empty: Do not stop the process but set artno to 0 instead. And blog.
						//m_ArticlenoCode = enOTHER;
						//// This is an optional field

						// We do not know the correct type, just set enPznCode but switch articleno to zero to avoid stopping the process
						std::stringstream str;
						str << "LIN_08-line with empty LIN-ARTNRC (Articleno-Code) field. Cannot interprete articleno: \"" << m_Articleno << "\" line: " << lineStr;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

						m_ArticlenoCode = enPznCode;
						m_Articleno = "";
					} 
					else if ("EAN" == strArticlenoCode) {
						m_ArticlenoCode = enEanCode;
					} 
					else if (   "P  " == strArticlenoCode
						     || " P " == strArticlenoCode
							 || "  P" == strArticlenoCode
							 || "P " == strArticlenoCode
							 || " P" == strArticlenoCode
							 || "P" == strArticlenoCode
							 ) {
						m_ArticlenoCode = enPznCode;
					} 
					else { 
						// kes 18.2.15: If ARTNRC is empty: Do not stop the process but set artno to 0 instead. And blog.
						// Not permitted: error!
						//m_ArticlenoCode = enOTHER;

						std::stringstream str;
						str << "LIN_08-line with unknown LIN-ARTNRC (Articleno-Code) field: \"" << strArticlenoCode << "\" line: " << lineStr;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

						// We do not know the correct type, just set enPznCode but switch articleno to zero to avoid stopping the process
						m_ArticlenoCode = enPznCode;
						m_Articleno = "";
					}
				}
				else // TODO: Oder =EAN wie in CH???
				{	
						// kes 18.2.15: If ARTNRC is empty: Do not stop the process but set artno to 0 instead. And blog.
						//m_ArticlenoCode = enOTHER;

						std::stringstream str;
						str << "LIN_08-line too short and without LIN-ARTNRC (Articleno-Code) field. Line: " << lineStr;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

						// We do not know the correct type, just set enPznCode but switch articleno to zero to avoid stopping the process
						m_ArticlenoCode = enPznCode;
						m_Articleno = "";
				}
			}
			catch (std::exception& e)
			{
					std::stringstream str;
					str << "Unprocessable LIN_08 line: " << lineStr << e.what() << std::endl;
					throw libabbauw::UnprocessableException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
			return;
		}

		void Lin_08_Line::resetMembers()
		{
			m_Positionno	= 0;			// field b
			m_Articleno		= "";			// field d
			m_ArticlenoCode	= enUNKNOWN;	// field f
		}

	} // namespace line
} // namespace domMod
