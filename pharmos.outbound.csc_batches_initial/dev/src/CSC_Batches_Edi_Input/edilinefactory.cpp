//----------------------------------------------------------------------------//
/*! \file	edilinefactory.cpp
 *  \brief  concrete subclass of ILineFactory
*  \author Jia Li ,P. Kessling
*  \date   02.01.2013, 
*  history 05.04.13  pk redesign
*          29.10.14  pk new linetypes for DE
*/
//----------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <sstream>				// for stringstream
#include <iomanip>				// for stringstream
#include <classinfo_macros.h>	// for BLOG_TRACE_METHOD-Makros
#include <libbasarcmnutil_logging.h>
#include "loggerpool.h"			// for LoggerPool

#include "editransferordermapper_definitions.h"

#include "edifactline.h"
#include "unbline.h"
#include "unhline.h"
#include "bgmline.h"
#include "dtmline.h"
#include "nadline.h"
#include "unsline.h"
#include "untline.h"
#include "unzline.h"
#include "linline.h"
#include "imdline.h"
#include "qtyline.h"
#include "pcdline.h"
#include "irrelevantline.h"

// for DE
#include "unb_00_line.h"
#include "unh_01_line.h"
#include "bgm_02_line.h"
#include "dtm_03_line.h"
#include "nad_04_line.h"
#include "alc_05_line.h"
#include "pcd_06_line.h"
#include "lin_08_line.h"
#include "qty_10_line.h"
#include "ftx_11_line.h"
#include "uns_97_line.h"
#include "irrelevant_de_line.h"

#include "ediline_definitions.h"
#include "edilinefactory.h"

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
		domMod::line::UnbLine		m_UnbLine		= domMod::line::UnbLine();
		domMod::line::Unb_00_Line	m_Unb_01_Line	= domMod::line::Unb_00_Line();



		EDIFACTLineBasePtr EdiLineFactory::create( const LineTypeEnum lineType , const domMod::orderMapper::InputtypeEnum inputtype)
		{
			// Unless it becomes necessary to do the full blown factory pattern,
			// we implement only the simple factory idiom here

			// Remark:       This method wastes time and memory by first 
			//               generating a new local line, that is then cloned to a second new line.
			//               The first generated line dies at once at the end of the clone()-Block.
			//               One of the two objects is redundant.

			// Differentiate lines of DE and CH
			if (domMod::orderMapper::enInputtype_inhCH	==	inputtype )
			{
				if (enUNB == lineType){
					return UnbLine().clone();
				} 
				else if (enUNH == lineType){
					UnhLine line;
					return line.clone();
				} 
				else if (enBGM == lineType){
					BgmLine line;
					return line.clone();
				} 
				else if (enDTM == lineType){
					DtmLine line;
					return line.clone();
				} 
				else if (enNAD == lineType){
					NadLine line;
					return line.clone();
				} 
				else if (enUNS == lineType){
					UnsLine line;
					return line.clone();
				} 
				else if (enLIN == lineType){
					LinLine line;
					return line.clone();
				} 
				else if (enIMD == lineType){
					ImdLine line;
					return line.clone();
				} 
				else if (enQTY == lineType){
					QtyLine line;
					return line.clone();
				} 
				else if (enUNT == lineType){
					UntLine line;
					return line.clone();
				} 
				else if (enUNZ == lineType){
					UnzLine line;
					return line.clone();
				} 
				else if (enPCD == lineType){
					PcdLine line;
					return line.clone();
				} 

				else if (enIRRELEVANT == lineType){  
					IrrLine line;
					return line.clone();
				} 
				else {
					// We do not throw here because this would end the whole processing 
					// of the input directory. But we blog.
					std::stringstream str;
					str << "Error creating a line: Unknown linetype " << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());


					IrrLine line;
					return line.clone();
				}
			}

			else	// if (domMod::orderMapper::enInputtype_inhDE	==	inputtype ): We dont check DE here, it is done before 
			{
				// for DE
				if (en00 == lineType){
					return Unb_00_Line().clone();
				} 
				else if (en01 == lineType){
					Unh_01_Line line;
					return line.clone();
				} 
				else if (en02 == lineType){
					Bgm_02_Line line;
					return line.clone();
				} 
				else if (en03 == lineType){
					Dtm_03_Line line;
					return line.clone();
				} 
				else if (en04 == lineType){
					Nad_04_Line line;
					return line.clone();
				} 
				else if (en05 == lineType){
					Alc_05_Line line;
					return line.clone();
				} 
				else if (en06 == lineType){
					Pcd_06_Line line;
					return line.clone();
				} 
				else if (en08 == lineType){
					Lin_08_Line line;
					return line.clone();
				} 
				else if (en10 == lineType){
					Qty_10_Line line;
					return line.clone();
				} 
				else if (en11 == lineType){
					Ftx_11_Line line;
					return line.clone();
				} 
				else if (en97 == lineType){
					Uns_97_Line line;
					return line.clone();
				} 


				else if (enIRRELEVANT == lineType){  
					IrrLine_DE line;
					return line.clone();
				} 
				else {
					// We do not throw here because this would end the whole processing 
					// of the input directory. But we blog.
					std::stringstream str;
					str << "Error creating a line: Unknown linetype " << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());


					IrrLine_DE line;
					return line.clone();
				}
			}

		}

	//--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//
		EdiLineFactory::EdiLineFactory(){}


	}
}
