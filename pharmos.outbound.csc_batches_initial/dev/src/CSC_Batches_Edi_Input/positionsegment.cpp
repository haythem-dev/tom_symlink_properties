//-------------------------------------------------------------------------------------------------//
/*! \file	positionsegment.cpp
 *  \brief  positionsegment
*  \author Jia Li, P. Kessling
*  \date   02.01.2013 
*  history: 09.04.13 pk rework
*			10.11.14 pk DE stuff added
 */
//-------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//

#include <classinfo_macros.h>	// for BLOG_TRACE_METHOD-Makros
#include "loggerpool.h"			// for LoggerPool

#include <libbasarcmnutil_exceptions.h> 

#include "linline.h"
#include "imdline.h"
#include "qtyline.h"
#include "pcdline.h"

#include "lin_08_line.h"
#include "qty_10_line.h"
#include "alc_05_line.h"
#include "pcd_06_line.h"
#include "ftx_11_line.h"

#include "edifactline.h"
#include "ediline_definitions.h"

#include "editransferorder.h"

#include "positionsegment.h"


//-------------------------------------------------------------------------------------------------//
// using declarations section
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//

namespace domMod
{
	namespace transferOrderDM
	{
    //--------------------------------------------------------------------------------------------------//
    // static definition section
    //--------------------------------------------------------------------------------------------------//

	//--------------------------------------------------------------------------------------------------//
    // object definition section
    //--------------------------------------------------------------------------------------------------//

		// construktors
		PositionSegment::PositionSegment()
		{
			resetMembers();
		}

		PositionSegment::~PositionSegment(){}


		// *************** getters *********************************
		basar::Int32 PositionSegment::getArticleno() const	
		{
			return m_Articleno;
		}
		void PositionSegment::setArticleno(const basar::Int32 articleno)	
		{
			m_Articleno = articleno;
		}

		const basar::I18nString& PositionSegment::getArticleCode() const	
		{
			return m_Article_code;
		}
		void PositionSegment::setArticleCode(const basar::I18nString article_code)	
		{
			m_Article_code = article_code;
		}

		const basar::I18nString& PositionSegment::getTextfield() const	
		{
			return m_Textfield;
		}
		void PositionSegment::setTextfield(const basar::I18nString textfield)	
		{
			m_Textfield = textfield;
		}


		basar::Decimal PositionSegment::getWholesalerpurchaseprice() const	
		{
			return m_Wholesalerpurchaseprice;
		}
		void PositionSegment::setWholesalerpurchaseprice(const basar::Decimal wholesalerpurchaseprice)	
		{
			m_Wholesalerpurchaseprice = wholesalerpurchaseprice;
		}

		basar::cmnutil::Decimal PositionSegment::getDiscountPct() const	
		{
			return m_DiscountPct;
		}
		void PositionSegment::setDiscountPct(const basar::cmnutil::Decimal discountPct)	
		{
			m_DiscountPct = discountPct;
		}


		// For InhCH
		domMod::line::LinLinePtr PositionSegment::getLinLine_LinLinePtr() const	
		{
			return m_LinLinePtr;
		}
		domMod::line::ImdLinePtr PositionSegment::getImdLine_ImdLinePtr() const	
		{
			return m_ImdLinePtr;
		}
		domMod::line::QtyLinePtr PositionSegment::getQtyLine_OrderedQtyPtr() const	
		{
			return m_QtyLine_OrderedQtyPtr;
		}
		domMod::line::QtyLinePtr PositionSegment::getQtyLine_ChargedQuantityPtr() const	
		{
			return m_QtyLine_ChargedQuantityPtr;
		}
		domMod::line::PcdLinePtr PositionSegment::getPcdLine_PcdLinePtr() const	
		{
			return m_PcdLinePtr;
		}

		// For InhDE
		domMod::line::Lin_08_LinePtr PositionSegment::getLin_08_Line_Lin_08_LinePtr() const	
		{
			return m_Lin_08_LinePtr;
		}
		domMod::line::Alc_05_LinePtr PositionSegment::getAlc_05_Line_Alc_05_LinePtr() const	
		{
			return m_Alc_05_LinePtr;
		}
		domMod::line::Pcd_06_LinePtr PositionSegment::getPcd_06_Line_Pcd_06_LinePtr() const	
		{
			return m_Pcd_06_LinePtr;
		}
		domMod::line::Qty_10_LinePtr PositionSegment::getQty_10_Line_ChargedQtyPtr() const	
		{
			return m_Qty_10_Line_ChargedQtyPtr;
		}
		domMod::line::Qty_10_LinePtr PositionSegment::getQty_10_Line_NonChargedQtyPtr() const	
		{
			return m_Qty_10_Line_NonChargedQtyPtr;
		}
		domMod::line::Ftx_11_LinePtr PositionSegment::getFtx_11_Line_PosPtr() const	
		{
			return m_Ftx_11_Line_PosPtr;
		}




		// clears all stored lines
		bool PositionSegment::empty()
		{
			resetMembers();
			return true;
		}


		// Mounts a line of this special type into this position of transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool PositionSegment::mountLINLine(const domMod::line::LinLinePtr& linePtr)
		{
			METHODNAME_DEF( PositionSegment, mountLINLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions for this position are ok? ..

				// .. line may not exist yet (would be a duplicate or the begin of another position)
				if ( m_LinLinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple LIN-lines in a single position!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_LinLinePtr		= linePtr;
				return true;

			}
			catch (...)
			{
				// programming error 
				std::stringstream str;
				str << "Error mounting LIN-line into position" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		// Mounts a line of this special type into this position of transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool PositionSegment::mountIMDLine(const domMod::line::ImdLinePtr& linePtr)
		{
			METHODNAME_DEF( PositionSegment, mountIMDLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions for this position are ok? ..

				// .. line may not exist yet (would be a duplicate or the begin of another position)
				if ( m_ImdLinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple IMD-lines in a single position!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_ImdLinePtr		= linePtr;
				return true;

			}
			catch (...)
			{
				// programming error 
				std::stringstream str;
				str << "Error mounting IMD-line into position" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// Mounts a line of this special type into this position of transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool PositionSegment::mountQTYLine(const domMod::line::QtyLinePtr& linePtr)
		{
			METHODNAME_DEF( PositionSegment, mountQTYLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions for this position are ok? ..

				// .. Different handling of qty-subtypes
				if (domMod::line::QtyLine::enOrderedQty == linePtr->getQualifier())
				{
					// .. line QTY 21 must not exist yet (would be a duplicate)
					if ( m_QtyLine_OrderedQtyPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple QTY 21-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_QtyLine_OrderedQtyPtr				= linePtr;
				}
				else if (domMod::line::QtyLine::enChargedQty == linePtr->getQualifier())
				{
					// .. line QTY 47 must not exist yet (would be a duplicate)
					if ( m_QtyLine_ChargedQuantityPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple QTY 47-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_QtyLine_ChargedQuantityPtr		= linePtr;
				}
				else
				{
					// Unknown type of QTY-line
					std::stringstream str;
					str << "Error:  QTY-line: Unknown QTY-type!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				return true;

			}
			catch (...)
			{
				// programming error 
				std::stringstream str;
				str << "Error mounting QTY-line into position" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// Mounts a line of this special type into this position of transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool PositionSegment::mountPCDLine(const domMod::line::PcdLinePtr& linePtr)
		{
			METHODNAME_DEF( PositionSegment, mountPCDLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions for this position are ok? ..

				// .. line may not exist yet (would be a duplicate or the begin of another position)
				if ( m_PcdLinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple PCD-lines in a single position!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_PcdLinePtr		= linePtr;
				return true;

			}
			catch (...)
			{
				// programming error 
				std::stringstream str;
				str << "Error mounting PCD-line into position" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		// ====== DE mount methods ========================================================================================

		
		// Mounts a line of this special type into this position of transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool PositionSegment::mountLIN_08_Line(const domMod::line::Lin_08_LinePtr& linePtr)
		{
			METHODNAME_DEF( PositionSegment, mountLIN_08_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions for this position are ok? ..

				// .. line may not exist yet (would be a duplicate or the begin of another position)
				if ( m_Lin_08_LinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple LIN_08-lines in a single position!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_Lin_08_LinePtr		= linePtr;
				return true;

			}
			catch (...)
			{
				// programming error 
				std::stringstream str;
				str << "Error mounting LIN_08-line into position" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		
		// Mounts a line of this special type into this position of transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool PositionSegment::mountQTY_10_Line(const domMod::line::Qty_10_LinePtr& linePtr)
		{
			METHODNAME_DEF( PositionSegment, mountQTY_10_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions for this position are ok? ..

				// .. Different handling of qty-subtypes:
				// ..   MENGQUAL='21', ordered qty (without nonchargedqty)
				if (domMod::line::Qty_10_Line::enChargedQty == linePtr->getQualifier())
				{					

					// .. line QTY_10 21 must not exist yet (would be a duplicate)
					if ( m_Qty_10_Line_ChargedQtyPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple QTY_10 mengqual 21-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					// TODO: CHECK: Is it conform to CH? with / without NR???
					m_Qty_10_Line_ChargedQtyPtr				= linePtr;
				}
				else if (domMod::line::Qty_10_Line::enNonChargedQty == linePtr->getQualifier())
				// ..   MENGQUAL='130', only nonchargedqty
				{
					// .. line QTY 130 must not exist yet (would be a duplicate)
					if ( m_Qty_10_Line_NonChargedQtyPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple QTY_10 130-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_Qty_10_Line_NonChargedQtyPtr		= linePtr;
				}
				else
				{
					// Unknown type of QTY-line
					std::stringstream str;
					str << "Error:  QTY_10_-line: Unknown QTY_10-type!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				return true;

			}
			catch (...)
			{
				// programming error 
				std::stringstream str;
				str << "Error mounting QTY_10_-line into position" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// Mounts a line of this special type into this position of transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool PositionSegment::mountALC_05_Line(const domMod::line::Alc_05_LinePtr& linePtr)
		{
			METHODNAME_DEF( PositionSegment, mountALC_05_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions for this position are ok? ..

				// .. line may not exist yet (would be a duplicate or the begin of another position)
				if ( m_Alc_05_LinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple ALC_05-lines in a single position!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_Alc_05_LinePtr		= linePtr;
				return true;

			}
			catch (...)
			{
				// programming error 
				std::stringstream str;
				str << "Error mounting ALC_05-line into position" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// Mounts a line of this special type into this position of transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool PositionSegment::mountPCD_06_Line(const domMod::line::Pcd_06_LinePtr& linePtr)
		{
			METHODNAME_DEF( PositionSegment, mountPCD_06_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions for this position are ok? ..

				// .. line may not exist yet (would be a duplicate or the begin of another position)
				if ( m_Pcd_06_LinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple PCD_06-lines in a single position!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_Pcd_06_LinePtr		= linePtr;
				return true;

			}
			catch (...)
			{
				// programming error 
				std::stringstream str;
				str << "Error mounting PCD_06_-line into position" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

				
		// Mounts a line of this special type into this position of transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool PositionSegment::mountFTX_11_Pos_Line(const domMod::line::Ftx_11_LinePtr& linePtr)
		{
			METHODNAME_DEF( PositionSegment, mountFTX_11_Pos_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions for this position are ok? ..

					// .. line FTX_11 must not exist in this Position yet (would be a duplicate, we ignore it then)
					if ( m_Ftx_11_Line_PosPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple FTX_11_AAA lines in a single transfer order position. Ignore all but the first one!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return true;
					}

					// Mount it
					m_Ftx_11_Line_PosPtr				= linePtr;
			
					return true;
			}
			catch (...)
			{
				// programming error 
				std::stringstream str;
				str << "Error mounting FTX_11_Pos-line into position" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}




		// ====== end mount methods =======================================================================================

		
		// is Info complete for storage? Checks only the existence of mandatory lines. No check for syntax issues here.
		// For CH order only
		bool PositionSegment::isValidInhCH() const
		{
			METHODNAME_DEF( PositionSegment, isValidInhCH )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			if ( !m_LinLinePtr )
			{
					std::stringstream str;
					str << "Error:  Invalid PositionSegment: Missing LIN line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
			}
			// IMD lines are not mandatory
			//if ( !m_ImdLinePtr )
			//{
			//		std::stringstream str;
			//		str << "Error:  Invalid PositionSegment: Missing IMD line!" ;
			//		BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			//		return false;
			//}
			if ( !m_QtyLine_OrderedQtyPtr )
			{
					std::stringstream str;
					str << "Error:  Invalid PositionSegment: Missing QTY 21 line (ordered quantity)!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
			}
			if ( !m_QtyLine_ChargedQuantityPtr )
			{
					std::stringstream str;
					str << "Error:  Invalid PositionSegment: Missing QTY 47 line (charged quantity)!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
			}
			// PCD line is optional: Check  if it exists only


			// The validity of the lines was already checked while reading them.

			return true;
		}


		// is Info complete for storage? Checks only the existence of mandatory lines. No check for syntax issues here.
		// For DE order only.
		bool PositionSegment::isValidInhDE() const
		{
			METHODNAME_DEF( PositionSegment, isValidInhDE )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			if ( !m_Lin_08_LinePtr )
			{
					std::stringstream str;
					str << "Error:  Invalid PositionSegment: Missing LIN_08 line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
			}
			// IMD lines are not mandatory
			//if ( !m_ImdLinePtr )
			//{
			//		std::stringstream str;
			//		str << "Error:  Invalid PositionSegment: Missing IMD line!" ;
			//		BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			//		return false;
			//}
			if ( !m_Qty_10_Line_ChargedQtyPtr )
			{
					std::stringstream str;
					str << "Error:  Invalid PositionSegment: Missing QTY_10 21 line (ordered quantity)!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
			}
			// Not mandatory
			/*if ( !m_Qty_10_Line_NonChargedQuantityPtr )
			{
					std::stringstream str;
					str << "Error:  Invalid PositionSegment: Missing QTY_10 47 line (charged quantity)!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
			}*/
			// PCD_06 line is optional: Check  if it exists only

			// TODO: What about the ALC_05 line here?

			// The validity of the lines was already checked while reading them.


			return true;
		}



		// resets everything in class
		// ToDo: Check from time to time: Is it still complete?
		void PositionSegment::resetMembers()
		{
			// CH linetypes
			m_LinLinePtr.reset();
			m_ImdLinePtr.reset();
			m_QtyLine_OrderedQtyPtr.reset();
			m_QtyLine_ChargedQuantityPtr.reset();			
			m_PcdLinePtr.reset();		

			// DE linetypes
			m_Lin_08_LinePtr.reset();
			m_Qty_10_Line_ChargedQtyPtr.reset();
			m_Qty_10_Line_NonChargedQtyPtr.reset();
			m_Alc_05_LinePtr.reset();
			m_Pcd_06_LinePtr.reset();



			m_Articleno					= 0;
			m_Article_code				= "";
			m_Textfield					= "";

			basar::cmnutil::Decimal decZero(0);
			m_Wholesalerpurchaseprice	= decZero;
			m_DiscountPct				= decZero;	

		}

	} // namespace transferOrderDM
} // namespace domMod