//-------------------------------------------------------------------------------------------------//
/*! \file  editransferorder.cpp
*  \brief  class for transfer order. 
*  \author P. Kessling
*  \date   09.04.2013 
*  history: 09.04.13 pk new
*           xx.11.14 pk German linetypes added
*/
//-------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <vector>

#include <classinfo_macros.h>	// for BLOG_TRACE_METHOD-Makros
#include "loggerpool.h"			// for LoggerPool

#include <libbasarcmnutil_exceptions.h> 

#include "unbline.h"

#include "unhline.h"
#include "bgmline.h"
#include "dtmline.h"
#include "nadline.h"
#include "unsline.h"

#include "bgm_02_line.h"


#include "positionsegment.h"
#include "edifactline.h"
#include "ediline_definitions.h"

#include "editransferorder.h"

#include "articleconsultantdm.h"

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
		// Constructor
		EdiTransferOrder::EdiTransferOrder()
		{
			resetMembers();
		}
		EdiTransferOrder::~EdiTransferOrder(){}


		// Getters and Setters
		domMod::orderMapper::InputtypeEnum	EdiTransferOrder::getInputtype() const
		{
			return m_Inputtype;
		}


		void EdiTransferOrder::setInputtype(const domMod::orderMapper::InputtypeEnum inputtype)
		{
			m_Inputtype = inputtype;
			return;
		}


		const std::ostream::pos_type EdiTransferOrder::getOrderStartPos() const
		{
			return m_OrderStartPos;
		}
		
		void EdiTransferOrder::setOrderStartPos(const std::ostream::pos_type& orderStartPos)
		{
			m_OrderStartPos = orderStartPos;
			return;
		}


		basar::Int16 EdiTransferOrder::getBranchno() const		// gets the orderno for orderhead table or -1 if not set	
		{
			return m_Branchno;
		}
		void EdiTransferOrder::setBranchno(const basar::Int16 branchno)	
		{
			m_Branchno = branchno;
		}

		basar::Int32 EdiTransferOrder::getOrdernoBatch() const	// gets m_OrdernoBatch or -1 if not set	
		{
			return m_OrdernoBatch;
		}
		void EdiTransferOrder::setOrdernoBatch(const basar::Int32 ordernoBatch)	// sets m_OrdernoBatch	
		{
			m_OrdernoBatch = ordernoBatch;
		}

		const basar::I18nString& EdiTransferOrder::getStatus() const				// gets m_Status 
		{
			return m_Status;
		}
		void EdiTransferOrder::setStatus(const basar::I18nString status)	// sets m_Status	
		{
			m_Status = status;
		}


		const basar::I18nString& EdiTransferOrder::getOrdertype() const	// gets m_Ordertype 
		{
			return m_Ordertype;
		}
		void EdiTransferOrder::setOrdertype(const basar::I18nString ordertype)	// sets m_Ordertype	
		{
			m_Ordertype = ordertype;
		}

		const basar::I18nString& EdiTransferOrder::getOrderpickingtype() const	// gets m_Orderpickingtype 
		{
			return m_Orderpickingtype;
		}
		void EdiTransferOrder::setOrderpickingtype(const basar::I18nString orderpickingtype)	// sets m_Ordertype	
		{
			m_Orderpickingtype = orderpickingtype;
		}

		const basar::I18nString& EdiTransferOrder::getBookentrytype() const	// gets m_Bookentrytype 
		{
			return m_Bookentrytype;
		}
		void EdiTransferOrder::setBookentrytype(const basar::I18nString bookentrytype)	// sets m_Ordertype	
		{
			m_Bookentrytype = bookentrytype;
		}

		const basar::I18nString& EdiTransferOrder::getHeadertext() const	// gets m_Headertext 
		{
			return m_Headertext;
		}
		void EdiTransferOrder::setHeadertext(const basar::I18nString headertext)	// sets m_Headertext	
		{
			m_Headertext = headertext;
		}

		// kes 021213: set orderdate to "today" instead of DTM137 segment.
		//             Because sme manufacturers send wrong data in DTM137.
		basar::Int32 EdiTransferOrder::getOrderDateCH() const				// gets Orderdate if set, otherwise 0
		{
			//if (m_DtmLine_OrderDatePtr)
			//{
			//	return std::max<basar::Int32>( 0, m_DtmLine_OrderDatePtr->getDate().getDate() );	// last getDate() converts to Int32
			//}
			//else
			//{
			//	return 0;
			//}

			basar::DateTime dtToday; 
			basar::Int32 intToday = dtToday.getDate();			

			return intToday;	

		}
		// No longer: In DE we get the date from the EdiFact-DTM137
		// kes 101115: set orderdate to "today" instead of DTM137 segment.
		//             Because sme manufacturers send wrong data in DTM137.

		basar::Int32 EdiTransferOrder::getOrderDateDE() const				// gets Orderdate if set, otherwise 0
		{
			//if (m_Dtm_03_Line_OrderDatePtr)
			//{
			//	return std::max<basar::Int32>( 0, m_Dtm_03_Line_OrderDatePtr->getDate().getDate() );	// last getDate() converts to Int32
			//}
			//else
			//{
			//	return 0;
			//}

			basar::DateTime dtToday; 
			basar::Int32 intToday = dtToday.getDate();			

			return intToday;	

		}
		basar::Int32 EdiTransferOrder::getDeferredPaymentDateCH() const				// gets DeferredPaymentDate if set, otherwise 0
		{
			if (m_DtmLine_DeferredPaymentDatePtr)
			{
				return std::max<basar::Int32>( 0, m_DtmLine_DeferredPaymentDatePtr->getDate().getDate() );	// last getDate() converts to Int32
			}
			else
			{
				return 0;
			}
		}
		basar::Int32 EdiTransferOrder::getDeferredPaymentDateDE() const				// gets DeferredPaymentDate if set, otherwise 0
		{
			if (m_Dtm_03_Line_DeferredPaymentDatePtr)
			{
				return std::max<basar::Int32>( 0, m_Dtm_03_Line_DeferredPaymentDatePtr->getDate().getDate() );	// last getDate() converts to Int32
			}
			else
			{
				return 0;
			}
		}
		basar::Int32 EdiTransferOrder::getEdiDeliveryDateCH() const				// gets EdiDeliveryDate if set, otherwise 0
		{
			if (m_DtmLine_EdiDeliveryDatePtr)
			{
				return std::max<basar::Int32>( 0, m_DtmLine_EdiDeliveryDatePtr->getDate().getDate() );	// last getDate() converts to Int32
			}
			else
			{
				return 0;
			}
		}
		basar::Int32 EdiTransferOrder::getEdiDeliveryDateDE() const				// gets EdiDeliveryDate if set, otherwise 0
		{
			if (m_Dtm_03_Line_EdiDeliveryDatePtr)
			{
				return std::max<basar::Int32>( 0, m_Dtm_03_Line_EdiDeliveryDatePtr->getDate().getDate() );	// last getDate() converts to Int32
			}
			else
			{
				return 0;
			}
		}

		basar::Int32 EdiTransferOrder::getPharmacyno() const	
		{
			return m_Pharmacyno;
		}
		void EdiTransferOrder::setPharmacyno(const basar::Int32 pharmacyno)	
		{
			m_Pharmacyno = pharmacyno;
		}

		const basar::I18nString EdiTransferOrder::getOrdernoMFCH() const	
		{
			if (m_BgmLinePtr)
			{
				return m_BgmLinePtr->getOrdernoMF();
			}
			else
			{
				basar::I18nString empty("");
				return empty;
			}

		}
		const basar::I18nString EdiTransferOrder::getOrdernoMFDE() const	
		{
			if (m_Bgm_02_LinePtr)
			{
				return m_Bgm_02_LinePtr->getOrdernoMF();
			}
			else
			{
				basar::I18nString empty("");
				return empty;
			}

		}


		basar::Int32 EdiTransferOrder::getManufacturerno() const	
		{
			return m_Manufacturerno;
		}
		void EdiTransferOrder::setManufacturerno(const basar::Int32 manufacturerno)	
		{
			m_Manufacturerno = manufacturerno;
		}

		// kes 26.06.14: New field pharmacynomf
		const basar::I18nString EdiTransferOrder::getPharmacynoMFCH() const	
		{
			if (m_NadLine_PharmacyPtr)
			{
				return m_NadLine_PharmacyPtr->getIdentEan();
			}
			else
			{
				basar::I18nString empty("");
				return empty;
			}

		}
		const basar::I18nString EdiTransferOrder::getPharmacynoMFDE() const	
		{
			if (m_Nad_04_Line_PharmacyCNPtr)
			{
				return m_Nad_04_Line_PharmacyCNPtr->getIdent();
			}
			else if (m_Nad_04_Line_PharmacyBYPtr)
			{
				return m_Nad_04_Line_PharmacyBYPtr->getIdent();
			}
			else
			{
				basar::I18nString empty("");
				return empty;
			}

		}





		std::vector <PositionSegmentPtr> EdiTransferOrder::getPositionSegments() const			// gets the list of positions
		{
			return m_PositionSegments;
		}





		// for CH
		domMod::line::UnbLinePtr EdiTransferOrder::getUnbLine_Ptr() const	
		{
			return m_UnbLinePtr;
		}

		domMod::line::NadLinePtr EdiTransferOrder::getNadLine_ManufacturerPtr() const	
		{
			return m_NadLine_ManufacturerPtr;
		}
		domMod::line::NadLinePtr EdiTransferOrder::getNadLine_BranchPtr() const	
		{
			return m_NadLine_BranchPtr;
		}
		domMod::line::NadLinePtr EdiTransferOrder::getNadLine_PharmacyPtr() const	
		{
			return m_NadLine_PharmacyPtr;
		}

		// for DE
		domMod::line::Unb_00_LinePtr EdiTransferOrder::getUnb_00_Line_Ptr() const	
		{
			return m_Unb_00_LinePtr;
		}
		domMod::line::Nad_04_LinePtr EdiTransferOrder::getNad_04_Line_BranchPtr() const	
		{
			return m_Nad_04_Line_BranchPtr;
		}
		domMod::line::Nad_04_LinePtr EdiTransferOrder::getNad_04_Line_PharmacyBYPtr() const	
		{
			return m_Nad_04_Line_PharmacyBYPtr;
		}
		domMod::line::Nad_04_LinePtr EdiTransferOrder::getNad_04_Line_PharmacyCNPtr() const	
		{
			return m_Nad_04_Line_PharmacyCNPtr;
		}
		domMod::line::Nad_04_LinePtr EdiTransferOrder::getNad_04_Line_ManufacturerPtr() const	
		{
			return m_Nad_04_Line_ManufacturerPtr;
		}

		domMod::line::Ftx_11_LinePtr EdiTransferOrder::getFtx_11_Line_Ptr()	const
		{
			return m_Ftx_11_LinePtr;
		}






		basar::Int32 EdiTransferOrder::getCntPositions() const	// gets the number of positions currently stored	
		{
			return static_cast<basar::Int32>(m_PositionSegments.size());
		}


		// resets everything in class
		// ToDo: Check from time to time: Is it still complete? All members are resetted?
		void EdiTransferOrder::resetMembers()
		{
			m_Inputtype = domMod::orderMapper::enInputtype_UNKNOWN;

			m_OrderStartPos = 0;

			// CH linetypes
			m_UnbLinePtr.reset();
			m_UnhLinePtr.reset();
			m_BgmLinePtr.reset();
			m_DtmLine_OrderDatePtr.reset();			
			m_DtmLine_EdiDeliveryDatePtr.reset();		
			m_DtmLine_DeferredPaymentDatePtr.reset();	
			m_NadLine_ManufacturerPtr.reset();		
			m_NadLine_BranchPtr.reset();				
			m_NadLine_PharmacyPtr.reset();			

			m_UnsLine_DPtr.reset();	
			m_UnsLine_SPtr.reset();	

			m_UntLinePtr.reset();	
			m_UnzLinePtr.reset();	

			// DE linetypes
			m_Unb_00_LinePtr.reset();	
			m_Unh_01_LinePtr.reset();
			m_Bgm_02_LinePtr.reset();
			m_Dtm_03_Line_OrderDatePtr.reset();
			m_Dtm_03_Line_EdiDeliveryDatePtr.reset();
			m_Dtm_03_Line_DeferredPaymentDatePtr.reset();
			m_Nad_04_Line_ManufacturerPtr.reset();
			m_Nad_04_Line_BranchPtr.reset();
			m_Nad_04_Line_PharmacyBYPtr.reset();
			m_Nad_04_Line_PharmacyCNPtr.reset();
			m_Ftx_11_LinePtr.reset();
			m_Uns_97_Line_DPtr.reset();
			m_Uns_97_Line_SPtr.reset();



			if ( !m_PositionSegments.empty())
			{
				basar::Int32 cntElems = static_cast<basar::Int32>(m_PositionSegments.size());

				for (basar::Int32 index = cntElems -1 ; index >= 0 ; --index )
				{
					m_PositionSegments.at(index)->empty();
				}

				m_PositionSegments.clear();
			}

			m_Branchno			= -1;		
			m_OrdernoBatch		= -1;		
			m_Status			= "";				
			m_Ordertype			= "";				
			m_Orderpickingtype	= "";		 
			m_Bookentrytype		= "";			
			m_Headertext		= "";		
			m_Pharmacyno		= -1;		
			m_Manufacturerno	= -1;		
		}



		// clears all stored lines
		bool EdiTransferOrder::empty()
		{
			resetMembers();
			return true;
		}

		// mount this line to the correct place in transferOrder
		//
		// param linePtr [transfer ownership]:		line is already processed and checked for validity
		// Remark: Unfortunately the lines cannot mount themselves because this ould lead to circular includes of some .h-files
		// Remark: This method and its submethods implement the plausibility conditions for valid transfer orders.
		//         A state machine wouldn't be bad at this point too, but this way seems to be less work by same results.
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountLine(domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			bool retval = false;

			// identify type of line
			domMod::line::LineTypeEnum lineTypeEnum = linePtr->getLineTypeEnum();

			// Mounting depends on type of line
			switch (lineTypeEnum)
			{
			// line types for CH
			case domMod::line::enUNB:

				retval = mountUNBLine(linePtr);

				break;
			case domMod::line::enUNH:

				retval = mountUNHLine(linePtr);

				break;
			case domMod::line::enBGM:

				retval = mountBGMLine(linePtr);

				break;
			case domMod::line::enDTM:

				retval = mountDTMLine(linePtr);

				break;
			case domMod::line::enNAD:

				retval = mountNADLine(linePtr);

				break;
			case domMod::line::enUNS:

				retval = mountUNSLine(linePtr);

				break;
			case domMod::line::enLIN:

				retval = mountLINLine(linePtr);

				break;
			case domMod::line::enIMD:

				retval = mountIMDLine(linePtr);

				break;
			case domMod::line::enQTY:

				retval = mountQTYLine(linePtr);

				break;
			case domMod::line::enPCD:

				retval = mountPCDLine(linePtr);

				break;
			case domMod::line::enUNT:

				retval = mountUNTLine(linePtr);

				break;
			case domMod::line::enUNZ:

				retval = mountUNZLine(linePtr);

				break;
			//..........................................................
			// line types for DE1, DE2
			//..........................................................
			case domMod::line::en00:

				retval = mountUNB_00_Line(linePtr);

				break;

			case domMod::line::en01:

				retval = mountUNH_01_Line(linePtr);

				break;

			case domMod::line::en02:

				retval = mountBGM_02_Line(linePtr);

				break;

			case domMod::line::en03:

				retval = mountDTM_03_Line(linePtr);

				break;
			case domMod::line::en04:

				retval = mountNAD_04_Line(linePtr);

				break;
			case domMod::line::en05:

				retval = mountALC_05_Line(linePtr);

				break;
			case domMod::line::en06:

				retval = mountPCD_06_Line(linePtr);

				break;
			case domMod::line::en08:

				retval = mountLIN_08_Line(linePtr);

				break;
			case domMod::line::en10:

				retval = mountQTY_10_Line(linePtr);

				break;
			case domMod::line::en11:

				retval = mountFTX_11_Line(linePtr);

				break;
			case domMod::line::en97:

				retval = mountUNS_97_Line(linePtr);

				break;



			// types for both formats
			case domMod::line::enIRRELEVANT:

				// Skip line
				retval = true;

				break;
			case domMod::line::enUNKNOWN:
			case domMod::line::enOTHER:
			default: 
				// Fehlerbehandlung und loggen!
				{
					std::stringstream str;
					str << "Error: Mount line () got unknown line type! "  << "\"";
					BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				}

				retval = false;
				
				break;
			}

			return retval;
		}

		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountUNBLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountUNBLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::UnbLinePtr unbLinePtr( boost::dynamic_pointer_cast<domMod::line::UnbLine>( linePtr ) );
				if( NULL == unbLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting UNB-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. line may not exist yet (would be a duplicate or the begin of another order)
				if ( m_UnbLinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple UNB-lines in a single transfer order!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_UnbLinePtr		= unbLinePtr;
				return true;

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting UNB-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		
		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountUNHLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountUNHLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::UnhLinePtr unhLinePtr( boost::dynamic_pointer_cast<domMod::line::UnhLine>( linePtr ) );
				if( NULL == unhLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting UNH-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in envelope state (UNB ex) 
				if ( !m_UnbLinePtr )
				{
					std::stringstream str;
					str << "Error:  UNH-line misses preceding UNB-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}


				// .. line may not exist yet (would be a duplicate or the begin of another UNH/UNT-section)
				if ( m_UnhLinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple UNH-lines in a single transfer order!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_UnhLinePtr		= unhLinePtr;
				return true;

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting UNH-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		
		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountBGMLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountBGMLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::BgmLinePtr bgmLinePtr( boost::dynamic_pointer_cast<domMod::line::BgmLine>( linePtr ) );
				if( NULL == bgmLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting BGM-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH ex, UNT does not ex) 
				if ( !m_UnhLinePtr )
				{
					std::stringstream str;
					str << "Error:  BGM-line misses preceding UNH-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_UntLinePtr )
				{
					std::stringstream str;
					str << "Error:  BGM-line outside of UNH/UNT-line pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}


				// .. line must not exist yet (would be a duplicate)
				if ( m_BgmLinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple BGM-lines in a single transfer order!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_BgmLinePtr	= bgmLinePtr;
				return true;

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting BGM-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountDTMLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountDTMLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				domMod::line::DtmLinePtr dtmLinePtr( boost::dynamic_pointer_cast<domMod::line::DtmLine>( linePtr ) );
				if( NULL == dtmLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting DTM-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH ex, UNT does not ex) 
				if ( !m_UnhLinePtr )
				{
					std::stringstream str;
					str << "Error:  DTM-line misses preceding UNH-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_UntLinePtr )
				{
					std::stringstream str;
					str << "Error:  DTM-line outside of UNH/UNT-line pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// .. Different handling of dtm-subtypes
				if (dtmLinePtr->isOrderDate())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_DtmLine_OrderDatePtr )
					{
						std::stringstream str;
						str << "Error:  Multiple DTM 137-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_DtmLine_OrderDatePtr				= dtmLinePtr;
				}
				else if (dtmLinePtr->isEdiDeliveryDate())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_DtmLine_EdiDeliveryDatePtr )
					{
						std::stringstream str;
						str << "Error:  Multiple DTM 2-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_DtmLine_EdiDeliveryDatePtr		= dtmLinePtr;
				}
				else if (dtmLinePtr->isDeferredPaymentDate())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_DtmLine_DeferredPaymentDatePtr )
					{
						std::stringstream str;
						str << "Error:  Multiple DTM 209-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_DtmLine_DeferredPaymentDatePtr	= dtmLinePtr;
				}
				else if (dtmLinePtr->isCustomerOrderDate())
				{
					// Skip this line. Not used for ABBA-UW.
					return true;
				}
				else
				{
					// Unknown type of DTM-line
					std::stringstream str;
					str << "Error:  DTM-line: Unknown DTM-type!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				return true;

			}
			catch (...)
			{
				std::stringstream str;
				str << "Error mounting DTM-line: Unknown error" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountNADLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountNADLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				domMod::line::NadLinePtr nadLinePtr( boost::dynamic_pointer_cast<domMod::line::NadLine>( linePtr ) );
				if( NULL == nadLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting NAD-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH ex, UNT does not ex) 
				if ( !m_UnhLinePtr )
				{
					std::stringstream str;
					str << "Error:  NAD-line misses preceding UNH-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_UntLinePtr )
				{
					std::stringstream str;
					str << "Error:  NAD-line outside of UNH/UNT-line pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// .. Different handling of nad-subtypes
				if (domMod::line::NadLine::enMF == nadLinePtr->getRole())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_NadLine_ManufacturerPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple NAD MF-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_NadLine_ManufacturerPtr				= nadLinePtr;
				}
				else if (domMod::line::NadLine::enDS == nadLinePtr->getRole())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_NadLine_BranchPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple NAD DS-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_NadLine_BranchPtr		= nadLinePtr;
				}
				else if (domMod::line::NadLine::enBY == nadLinePtr->getRole())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_NadLine_PharmacyPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple NAD BY-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_NadLine_PharmacyPtr	= nadLinePtr;
				}

				// Unknown type of NAD-line: Ignore them!! Host-software ignored ever since.

				//else 
				//{

				//	std::stringstream str;
				//	str << "Error:  NAD-line: Unknown NAD-type!" ;
				//	BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				//	return false;
				//}

				return true;

			}
			catch (...)
			{
				std::stringstream str;
				str << "Error mounting NAD-line: Unknown error" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

				
		
		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountUNSLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountUNSLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::UnsLinePtr unsLinePtr( boost::dynamic_pointer_cast<domMod::line::UnsLine>( linePtr ) );
				if( NULL == unsLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting UNS-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH ex, UNT does not ex) 
				if ( !m_UnhLinePtr )
				{
					std::stringstream str;
					str << "Error:  UNS-line misses preceding UNH-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_UntLinePtr )
				{
					std::stringstream str;
					str << "Error:  UNS-line outside of UNH/UNT-line pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}


				// .. Different handling of uns-subtypes
				if (unsLinePtr->isBeginOfPositionSection())
				{
					//    UNS-D line
					// .. line must not exist yet (would be a duplicate)
					if ( m_UnsLine_DPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple UNS D -lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_UnsLine_DPtr				= unsLinePtr;
				}
				else 
				{
					//    UNS-S line
					// .. line must not exist yet (would be a duplicate)
					//
					// !! Special manufacturer: At least one manufacturer (7601001002043 Biomed AG)
					// !! writes UNS-S at the beginning AND at the end of the position section.
					// !! We tolerate this, because the UEWAUF-description is not very precise at this point.
					// !!      --> if line uns_d was not read yet: we place this line there 

					if ( !m_UnsLine_DPtr )
					{
						// !! tolerate: if UNS-D is missing, we handle first UNS-S as if it was a UNS-D
						std::stringstream str;
						str << "Info :  Accept a pair of UNS-S instead of UNS-D/UNS-S to embrace the positionlist." ;
						BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

						// Mount it to the place of UNS-D
						m_UnsLine_DPtr		= unsLinePtr;

						return true;
					}
					if ( m_UnsLine_SPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple UNS S-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_UnsLine_SPtr		= unsLinePtr;
				}

				return true;

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting UNS-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		
		// Mounts a line of this special type into transfer order
		//
		// A LIN line starts a position, therefore a new position is created and appended to the position segment vector.
		// A LIN line is element of a position, therefore it is mounted into a position
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountLINLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountLINLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::LinLinePtr linLinePtr( boost::dynamic_pointer_cast<domMod::line::LinLine>( linePtr ) );
				if( NULL == linLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting LIN-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in position segment state (UNS-D ex, UNS-S does not ex) 
				if ( !m_UnsLine_DPtr )
				{
					std::stringstream str;
					str << "Error:  LIN-line misses preceding UNSD-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_UnsLine_SPtr )
				{
					std::stringstream str;
					str << "Error:  LIN-line outside of UNSD / UNSS-line pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// LIN line starts a new position, that is to be appended into the position segment vector 
				PositionSegmentPtr positionSegmentPtr(new PositionSegment());
				m_PositionSegments.push_back (positionSegmentPtr);

				// Mount line into last position of position vector
				//bool retval = positionSegmentPtr->mountLINLine(linLinePtr);
				PositionSegmentPtr lastPositionSegmentPtr = m_PositionSegments.at(m_PositionSegments.size() -1);		

				return lastPositionSegmentPtr->mountLINLine(linLinePtr);

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting LIN-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// Mounts a line of this special type into transfer order
		//
		// A IMD line is element of a position, therefore it is mounted into a position
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountIMDLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountIMDLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::ImdLinePtr imdLinePtr( boost::dynamic_pointer_cast<domMod::line::ImdLine>( linePtr ) );
				if( NULL == imdLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting IMD-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in position segment state (UNS-D ex, UNS-S does not ex) 
				if ( !m_UnsLine_DPtr )
				{
					std::stringstream str;
					str << "Error:  IMD-line misses preceding UNSD-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_UnsLine_SPtr )
				{
					std::stringstream str;
					str << "Error:  IMD-line outside of UNSD / UNSS-line pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount line into last position of position vector
				PositionSegmentPtr lastPositionSegmentPtr = m_PositionSegments.at(m_PositionSegments.size() -1);		

				return lastPositionSegmentPtr->mountIMDLine(imdLinePtr);

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting IMD-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}



		// Mounts a line of this special type into transfer order
		//
		// A QTY line is element of a position, therefore it is mounted into a position
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountQTYLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountQTYLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::QtyLinePtr qtyLinePtr( boost::dynamic_pointer_cast<domMod::line::QtyLine>( linePtr ) );
				if( NULL == qtyLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting QTY-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in position segment state (UNS-D ex, UNS-S does not ex) 
				if ( !m_UnsLine_DPtr )
				{
					std::stringstream str;
					str << "Error:  QTY-line misses preceding UNSD-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_UnsLine_SPtr )
				{
					std::stringstream str;
					str << "Error:  QTY-line outside of UNSD / UNSS-line pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount line into last position of position vector
				PositionSegmentPtr lastPositionSegmentPtr = m_PositionSegments.at(m_PositionSegments.size() -1);		

				return lastPositionSegmentPtr->mountQTYLine(qtyLinePtr);

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting QTY-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		
		// Mounts a line of this special type into transfer order
		//
		// A PCD line is element of a position, therefore it is mounted into a position
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountPCDLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountPCDLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::PcdLinePtr pcdLinePtr( boost::dynamic_pointer_cast<domMod::line::PcdLine>( linePtr ) );
				if( NULL == pcdLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting PCD-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in position segment state (UNS-D ex, UNS-S does not ex) 
				if ( !m_UnsLine_DPtr )
				{
					std::stringstream str;
					str << "Error:  PCD-line misses preceding UNSD-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_UnsLine_SPtr )
				{
					std::stringstream str;
					str << "Error:  PCD-line outside of UNSD / UNSS-line pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount line into last position of position vector
				PositionSegmentPtr lastPositionSegmentPtr = m_PositionSegments.at(m_PositionSegments.size() -1);		

				return lastPositionSegmentPtr->mountPCDLine(pcdLinePtr);

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting PCD-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}



				
		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountUNTLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountUNTLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::UntLinePtr untLinePtr( boost::dynamic_pointer_cast<domMod::line::UntLine>( linePtr ) );
				if( NULL == untLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting UNT-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH ex, UNT does not ex) 
				if ( !m_UnhLinePtr )
				{
					std::stringstream str;
					str << "Error:  UNS-line misses preceding UNH-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}


				// Mount it
				m_UntLinePtr		= untLinePtr;

				return true;

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting UNT-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

				
		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountUNZLine(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountUNZLine )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::UnzLinePtr unzLinePtr( boost::dynamic_pointer_cast<domMod::line::UnzLine>( linePtr ) );
				if( NULL == unzLinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting UNZ-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in envelope state (BGM ex) 
				if ( !m_BgmLinePtr )
				{
					std::stringstream str;
					str << "Error:  UNZ-line misses preceding BGM-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}


				// Mount it
				m_UnzLinePtr		= unzLinePtr;

				return true;

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting UNZ-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		// =====================  MOUNT DE lines   ========================================
		
		
		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountUNB_00_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountUNB_00_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::Unb_00_LinePtr unb_00_LinePtr( boost::dynamic_pointer_cast<domMod::line::Unb_00_Line>( linePtr ) );
				if( NULL == unb_00_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting UNB_00-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. line may not exist yet (would be a duplicate or the begin of another order)
				if ( m_Unb_00_LinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple UNB_00-lines in a single transfer order!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_Unb_00_LinePtr		= unb_00_LinePtr;
				return true;

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting UNB_00-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountUNH_01_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountUNH_01_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::Unh_01_LinePtr unh_01_LinePtr( boost::dynamic_pointer_cast<domMod::line::Unh_01_Line>( linePtr ) );
				if( NULL == unh_01_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting UNH_01-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in envelope state (UNB ex) 
				if ( !m_Unb_00_LinePtr )
				{
					std::stringstream str;
					str << "Error:  UNH_01-line misses preceding UNB_00-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}


				// .. line may not exist yet (would be a duplicate or the begin of another UNH/UNT_98-section)
				if ( m_Unh_01_LinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple UNH_01-lines in a single transfer order!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_Unh_01_LinePtr		= unh_01_LinePtr;
				return true;

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting UNH_01-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountBGM_02_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountBGM_02_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::Bgm_02_LinePtr bgm_02_LinePtr( boost::dynamic_pointer_cast<domMod::line::Bgm_02_Line>( linePtr ) );
				if( NULL == bgm_02_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting BGM_02-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH ex, UNT does not ex) 
				if ( !m_Unh_01_LinePtr )
				{
					std::stringstream str;
					str << "Error:  BGM_02-line misses preceding UNH_01-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				// UNT_98 is not sent in inhDE-Fileformat


				// .. line must not exist yet (would be a duplicate)
				if ( m_Bgm_02_LinePtr )
				{
					std::stringstream str;
					str << "Error:  Multiple BGM_02-lines in a single transfer order!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount it
				m_Bgm_02_LinePtr	= bgm_02_LinePtr;
				return true;

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting BGM_02-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

				// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountDTM_03_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountDTM_03_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				domMod::line::Dtm_03_LinePtr dtm_03_LinePtr( boost::dynamic_pointer_cast<domMod::line::Dtm_03_Line>( linePtr ) );
				if( NULL == dtm_03_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting DTM_03-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH ex, UNT does not ex) 
				if ( !m_Unh_01_LinePtr )
				{
					std::stringstream str;
					str << "Error:  DTM_03-line misses preceding UNH_01-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				// UNT_98 is not sent in DE

				// .. Different handling of dtm-subtypes
				if (dtm_03_LinePtr->isOrderDate())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_Dtm_03_Line_OrderDatePtr )
					{
						std::stringstream str;
						str << "Error:  Multiple DTM_03 137-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					//*m_DtmLine_OrderDatePtr				= dtmLinePtr.get()  /* *linePtr*/;
					m_Dtm_03_Line_OrderDatePtr				= dtm_03_LinePtr;
				}
				else if (dtm_03_LinePtr->isEdiDeliveryDate())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_Dtm_03_Line_EdiDeliveryDatePtr )
					{
						std::stringstream str;
						str << "Error:  Multiple DTM_03 2-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					//*m_DtmLine_EdiDeliveryDatePtr		= dtmLinePtr.get()  /* *linePtr*/;
					m_Dtm_03_Line_EdiDeliveryDatePtr		= dtm_03_LinePtr;
				}
				else if (dtm_03_LinePtr->isDeferredPaymentDate() )
				{
					// .. valuedate must not exist yet (would be a duplicate date)
					if ( m_Dtm_03_Line_DeferredPaymentDatePtr )
					{
						std::stringstream str;
						str << "Error:  Multiple DTM_03 209- or ZZZ-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_Dtm_03_Line_DeferredPaymentDatePtr	= dtm_03_LinePtr;
				}
				else if (dtm_03_LinePtr->is3MonthDeferredPaymentDate())
				{
					// .. valuedate must not exist yet (would be a duplicate)
					if (  m_Dtm_03_Line_DeferredPaymentDatePtr )
					{
						std::stringstream str;
						str << "Error:  Multiple DTM_03 ZZZ- or 209-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_Dtm_03_Line_DeferredPaymentDatePtr	= dtm_03_LinePtr;
				}
				else if (dtm_03_LinePtr->isToleratedDatcodeDate())
				{
					// .. just log and ignore it
						std::stringstream str;
						str << "Info:  DTM_03 line: Unknown DTM_DATCODE 117 or 128 found! Ignore this DTM_03 line. " ;
						BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				}
				else
				{
					// Unknown type of DTM_03-line
					std::stringstream str;
					str << "Error:  DTM_03-line: Unknown DTM_03-type!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				return true;

			}
			catch (...)
			{
				std::stringstream str;
				str << "Error mounting DTM_03-line: Unknown error" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

		
		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountNAD_04_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountNAD_04_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				domMod::line::Nad_04_LinePtr nad_04_LinePtr( boost::dynamic_pointer_cast<domMod::line::Nad_04_Line>( linePtr ) );
				if( NULL == nad_04_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting NAD_04-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH ex, UNT does not ex) 
				if ( !m_Unh_01_LinePtr )
				{
					std::stringstream str;
					str << "Error:  NAD_04-line misses preceding UNH_01-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// .. Different handling of nad-subtypes
				if (domMod::line::Nad_04_Line::enMF == nad_04_LinePtr->getRole())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_Nad_04_Line_ManufacturerPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple NAD_04 MF-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_Nad_04_Line_ManufacturerPtr				= nad_04_LinePtr;
				}
				else if (domMod::line::Nad_04_Line::enDS == nad_04_LinePtr->getRole())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_Nad_04_Line_BranchPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple NAD_04 DS-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_Nad_04_Line_BranchPtr		= nad_04_LinePtr;
				}
				else if (domMod::line::Nad_04_Line::enBY == nad_04_LinePtr->getRole())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_Nad_04_Line_PharmacyBYPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple NAD_04 BY-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_Nad_04_Line_PharmacyBYPtr	= nad_04_LinePtr;
				}
				else if (domMod::line::Nad_04_Line::enCN == nad_04_LinePtr->getRole())
				{
					// .. line must not exist yet (would be a duplicate)
					if ( m_Nad_04_Line_PharmacyCNPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple NAD_04 CN-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_Nad_04_Line_PharmacyCNPtr	= nad_04_LinePtr;
				}

				// Unknown type of NAD-line: Ignore them!! Host-software ignored ever since.

				//else 
				//{

				//	std::stringstream str;
				//	str << "Error:  NAD-line: Unknown NAD-type!" ;
				//	BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				//	return false;
				//}

				return true;

			}
			catch (...)
			{
				std::stringstream str;
				str << "Error mounting NAD_04_-line: Unknown error" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// Mounts a line of this special type into transfer order
		//
		// A ALC line is element of a position, therefore it is mounted into a position
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountALC_05_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountALC_05_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::Alc_05_LinePtr alc_05_LinePtr( boost::dynamic_pointer_cast<domMod::line::Alc_05_Line>( linePtr ) );
				if( NULL == alc_05_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting ALC_05-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH_01 ex, UNS97 does not ex) 
				if ( !m_Unh_01_LinePtr )
				{
					std::stringstream str;
					str << "Error:  ALC_05-line misses preceding UNH_01-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_Uns_97_Line_SPtr )
				{
					std::stringstream str;
					str << "Error:  ALC_05-line outside of UNH_01 / UNS_97 line-pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// .. Only allowed if we are in the position section
				if (0 >= m_PositionSegments.size())
				{
					std::stringstream str;
					str << "Error:  ALC_05-line outside of a position segment!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount line into last position of position vector
				PositionSegmentPtr lastPositionSegmentPtr = m_PositionSegments.at(m_PositionSegments.size() -1);		

				return lastPositionSegmentPtr->mountALC_05_Line(alc_05_LinePtr);

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting ALC_05-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// Mounts a line of this special type into transfer order
		//
		// A PCD line is element of a position, therefore it is mounted into a position
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountPCD_06_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountPCD_06_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::Pcd_06_LinePtr pcd_06_LinePtr( boost::dynamic_pointer_cast<domMod::line::Pcd_06_Line>( linePtr ) );
				if( NULL == pcd_06_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting PCD_06-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH_01 ex, UNS97 does not ex) 
				if ( !m_Unh_01_LinePtr )
				{
					std::stringstream str;
					str << "Error:  PCD_06-line misses preceding UNH_01-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_Uns_97_Line_SPtr )
				{
					std::stringstream str;
					str << "Error:  PCD_06-line outside of UNH_01 / UNS_97 line-pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// .. Only allowed if we are in the position section
				if (0 >= m_PositionSegments.size())
				{
					std::stringstream str;
					str << "Error:  PCD_06-line outside of a position segment!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount line into last position of position vector
				PositionSegmentPtr lastPositionSegmentPtr = m_PositionSegments.at(m_PositionSegments.size() -1);		

				return lastPositionSegmentPtr->mountPCD_06_Line(pcd_06_LinePtr);

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting PCD_06-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}

				
		// Mounts a line of this special type into transfer order
		//
		// A LIN_08 line starts a position, therefore a new position is created and appended to the position segment vector.
		// A LIN_08 line is element of a position, therefore it is mounted into a position
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountLIN_08_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountLIN_08_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::Lin_08_LinePtr lin_08_LinePtr( boost::dynamic_pointer_cast<domMod::line::Lin_08_Line>( linePtr ) );
				if( NULL == lin_08_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting LIN_08-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// The following check does not work in DE. Ex another check? They do not have UNS-D: So, don't check.
				// .. Only allowed if in position segment state (UNS-D ex, UNS-S does not ex) 
				//if ( !m_UnsLine_DPtr )
				//{
				//	std::stringstream str;
				//	str << "Error:  LIN_08_-line misses preceding UNSD-line!" ;
				//	BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				//	return false;
				//}
				//if ( m_UnsLine_SPtr )
				//{
				//	std::stringstream str;
				//	str << "Error:  LIN-line outside of UNSD / UNSS-line pair!" ;
				//	BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				//	return false;
				//}

				// LIN line starts a new position, that is to be appended into the position segment vector 
				PositionSegmentPtr positionSegmentPtr(new PositionSegment());
				m_PositionSegments.push_back (positionSegmentPtr);

				// Mount line into last position of position vector
				//bool retval = positionSegmentPtr->mountLINLine(linLinePtr);
				PositionSegmentPtr lastPositionSegmentPtr = m_PositionSegments.at(m_PositionSegments.size() -1);		

				return lastPositionSegmentPtr->mountLIN_08_Line(lin_08_LinePtr);
			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting LIN_08-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}
		
		
		// Mounts a line of this special type into transfer order
		//
		// A QTY_10 line is element of a position, therefore it is mounted into a position
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountQTY_10_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountQTY_10_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::Qty_10_LinePtr qty_10_LinePtr( boost::dynamic_pointer_cast<domMod::line::Qty_10_Line>( linePtr ) );
				if( NULL == qty_10_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting QTY_10-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH_01 ex, UNS97 does not ex) 
				if ( !m_Unh_01_LinePtr )
				{
					std::stringstream str;
					str << "Error:  QTY_10-line misses preceding UNH_01-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_Uns_97_Line_SPtr )
				{
					std::stringstream str;
					str << "Error:  QTY_10-line outside of UNH_01 / UNS_97 line-pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// .. Only allowed if we are in the position section
				if (0 >= m_PositionSegments.size())
				{
					std::stringstream str;
					str << "Error:  QTY_10-line outside of a position segment!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}

				// Mount line into last position of position vector
				PositionSegmentPtr lastPositionSegmentPtr = m_PositionSegments.at(m_PositionSegments.size() -1);		
				return lastPositionSegmentPtr->mountQTY_10_Line(qty_10_LinePtr);

			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting QTY_10-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


					
		// Mounts a line of this special type into transfer order
		//
		// Special for DE: This line can occur in the header section of an order  (ZUCODE = "AAQ"), then mount it there
		//                 It can also occur in a position section (ZUCODE = "AAA"), then mount it as a position line
		//                 If the line contains a ZUCODE != "AAQ" or "AAA", then ignore the line. No logging. (This is the way PBS999 handled it)
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountFTX_11_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountFTX_11_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::Ftx_11_LinePtr ftx_11_LinePtr( boost::dynamic_pointer_cast<domMod::line::Ftx_11_Line>( linePtr ) );
				if( NULL == ftx_11_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting FTX_11-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH ex, UNT does not ex) 
				if ( !m_Unh_01_LinePtr )
				{
					std::stringstream str;
					str << "Error:  FTX_11-line misses preceding UNH_01-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				if ( m_Uns_97_Line_SPtr )
				{
					std::stringstream str;
					str << "Error:  FTX_11-line outside of UNH_01 / UNS_97 line-pair!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}


				// Decide whether it is a FTX-line for the order-header (ZUCODE="AAQ") or the orderposition (ZUCODE="AAA")
				if ( ftx_11_LinePtr->isOrderheadText())
				{
					// .. is for the orderheader
					// .. line must not exist yet in header (would be a duplicate, we ignore that)
					if ( m_Ftx_11_LinePtr )
					{
						std::stringstream str;
						str << "Error:  Multiple FTX_11-AAQ-lines for orderhead in a single transfer order! (Keep only the first one)" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return true;		// Do not mount it
					}

					// Mount it
					m_Ftx_11_LinePtr				= ftx_11_LinePtr;
					return true;
				}
				else if ( ftx_11_LinePtr->isOrderposText())
				{
					// .. is for the orderposition
					// .. line must not exist yet in header (would be a duplicate, we ignore that)

					// .. Only allowed if we are in the position section
					if (0 >= m_PositionSegments.size())
					{
						std::stringstream str;
						str << "Error:  FTX_11-AAA-line outside of a position segment!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount line into last position of position vector
					PositionSegmentPtr lastPositionSegmentPtr = m_PositionSegments.at(m_PositionSegments.size() -1);		
					return lastPositionSegmentPtr->mountFTX_11_Pos_Line(ftx_11_LinePtr);

				}
				else  
				{
					// It is not a relevant text. Just ignore it.
					// e.G. ZUCODE="INV" will be ignored
					return true;
				}
			
			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting FTX_11_-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// Mounts a line of this special type into transfer order
		//
		// Param: linePtr: Must contain a correct linetype! Not checked inside method!
		// throws: basar::cmnutil::BasarInvalidParameterException
		bool EdiTransferOrder::mountUNS_97_Line(const domMod::line::EDIFACTLineBasePtr& linePtr)
		{
			METHODNAME_DEF( EdiTransferOrder, mountUNS_97_Line )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			try
			{
				// All preconditions are ok? ..

				// .. check for datatype of line, to which the parameter points to
				//    Watch out: - dynamic_cast is only available with Compileroption -GR (VS2003: Eigenschaften->Sprache)
				//               - need special boost-cast for shared-pointers from boost
				domMod::line::Uns_97_LinePtr uns_97_LinePtr( boost::dynamic_pointer_cast<domMod::line::Uns_97_Line>( linePtr ) );
				if( NULL == uns_97_LinePtr.get() )
				{
					// programming error 
					std::stringstream str;
					str << "Error mounting UNS_97-line: Wrong linetype" << std::endl;
					throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

				// .. Only allowed if in function group state (UNH ex, UNT does not ex) 
				if ( !m_Unh_01_LinePtr )
				{
					std::stringstream str;
					str << "Error:  UNS_97-line misses preceding UNH_01-line!" ;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					return false;
				}
				// UNT is not sent in DE


				// .. Different handling of uns-subtypes
				// although PBS999 accepted only 'S' in DE
				if (uns_97_LinePtr->isBeginOfPositionSection())
				{
					//    UNS-D line
					// .. line must not exist yet (would be a duplicate)
					if ( m_Uns_97_Line_DPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple UNS_97 D -lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_Uns_97_Line_DPtr				= uns_97_LinePtr;
				}
				else 
				{
					//    UNS-S line
					// .. line must not exist yet (would be a duplicate)
					//
					// !! In DE only one UNS-S-Segment is sent to end the order

					// TODO: Check whether handling is correct for DE!

					if ( m_Uns_97_Line_SPtr )
					{
						std::stringstream str;
						str << "Error:  Multiple UNS_97 S-lines in a single transfer order!" ;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						return false;
					}

					// Mount it
					m_Uns_97_Line_SPtr		= uns_97_LinePtr;
				}

				return true;
			
			}
			catch (...)
			{
				// programming error :(
				std::stringstream str;
				str << "Error mounting UNS_97_-line: Wrong linetype? Something else?" << std::endl;
				throw basar::cmnutil::BasarInvalidParameterException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}
		}


		// ================================================================================

		// is Info complete for storage? Checks existence of mandatory lines but not for syntax
		bool EdiTransferOrder::isValid() const
		{
			METHODNAME_DEF( EdiTransferOrder, isValid )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			// Correkt InputType?
			if (domMod::orderMapper::enInputtype_inhCH	==	m_Inputtype )
			{
				return isValidInhCH();
			}
			else if (domMod::orderMapper::enInputtype_inhDE	==	m_Inputtype )
			{
				return isValidInhDE();
			}
			else
			{
				return false;
			}
		}


		// is Info complete for storage? Checks existence of mandatory lines but not for syntax
		bool EdiTransferOrder::isValidInhCH() const
		{
			METHODNAME_DEF( EdiTransferOrder, isValidInhCH )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		
			if ( !m_UnbLinePtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing UNB line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_UnhLinePtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing UNH line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_BgmLinePtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing BGM line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			/* kes 021213: DTM137 no longer in use because some manufacturers send incorrect dates here
			if ( !m_DtmLine_OrderDatePtr )
			{
			std::stringstream str;
			str << "Error:  Invalid transfer order: Missing DTM 137 (orderDate) line!" ;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			return false;
			}
			*/
			if ( !m_DtmLine_EdiDeliveryDatePtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing DTM 2 (EdiDeliveryDate) line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			// DTM 209 ( valutadate) is not mandatory
			//if ( !m_DtmLine_DeferredPaymentDatePtr )
			//{
			//		std::stringstream str;
			//		str << "Error:  Invalid transfer order: Missing DTM 209 (valutadate) line!" ;
			//		BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			//		return false;
			//}
			if ( !m_NadLine_ManufacturerPtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing NAD MF line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_NadLine_BranchPtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing NAD DS line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_NadLine_PharmacyPtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing NAD BY line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_UnsLine_DPtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing UNS-D (resp. beginning UNS-S) line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_UnsLine_SPtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing UNS-S line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_UntLinePtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing UNT line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_UnzLinePtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing UNZ line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}


			// check the list of positions
			std::vector <PositionSegmentPtr>::const_iterator itr;			// const-itr because we are in a const method

			for (itr = m_PositionSegments.begin(); itr != m_PositionSegments.end(); ++itr)
			{
				if (!(*itr)->isValidInhCH())										// double deref: 1. itr is pointer, PositionSegmentspointer is pointer
				{
					return false;
				}
			}

			// Everythig was ok
			return true;
		}


		// is Info complete for storage? Checks existence of mandatory lines but not for syntax
		bool EdiTransferOrder::isValidInhDE() const
		{
			METHODNAME_DEF( EdiTransferOrder, isValidInhDE )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


			if ( !m_Unb_00_LinePtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing UNB_00 line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_Unh_01_LinePtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing UNH_01 line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_Bgm_02_LinePtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing BGM_02 line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			/* kes 021213: DTM137 no longer in use because some manufacturers send incorrect dates here (ToDo: Same in DE?)
			if ( !m_DtmLine_OrderDatePtr )
			{
			std::stringstream str;
			str << "Error:  Invalid transfer order: Missing DTM 137 (orderDate) line!" ;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			return false;
			}
			*/
			/* kes 12.01.15: DTM2 is optional in DE (-> Sonderfälle 3) */
			//if ( !m_Dtm_03_Line_EdiDeliveryDatePtr )
			//{
			//	std::stringstream str;
			//	str << "Error:  Invalid transfer order: Missing DTM 3 (EdiDeliveryDate) line!" ;
			//	BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			//	return false;
			//}
			// DTM 209 ( valutadate) is not mandatory
			//if ( !m_DtmLine_DeferredPaymentDatePtr )
			//{
			//		std::stringstream str;
			//		str << "Error:  Invalid transfer order: Missing DTM 209 (valutadate) line!" ;
			//		BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			//		return false;
			//}
			/* kes 12.01.15: NAD_04_MF is optional in DE (-> Sonderfälle 3) */
			/* (Manufacturer is derived from first article) */
			//if ( !m_Nad_04_Line_ManufacturerPtr )
			//{
			//	std::stringstream str;
			//	str << "Error:  Invalid transfer order: Missing NAD_04 MF line!" ;
			//	BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			//	return false;
			//}
			if ( !m_Nad_04_Line_BranchPtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing NAD_04 DS line (branch)!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}
			if ( !m_Nad_04_Line_PharmacyBYPtr && !m_Nad_04_Line_PharmacyCNPtr)
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing NAD_04 CN or BY line (pharmacy)!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}

			if ( !m_Uns_97_Line_SPtr )
			{
				std::stringstream str;
				str << "Error:  Invalid transfer order: Missing UNS_97-S line!" ;
				BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				return false;
			}


			// check the list of positions
			std::vector <PositionSegmentPtr>::const_iterator itr;			// const-itr because we are in a const method

			for (itr = m_PositionSegments.begin(); itr != m_PositionSegments.end(); ++itr)
			{
				if (!(*itr)->isValidInhDE())										// double deref: 1. itr is pointer, PositionSegmentspointer is pointer
				{
					return false;
				}
			}

			return true;



		}

		// ================================================================================

		// is this order to be ignored (Tick Off with 'Y' and do not store it)?
		bool EdiTransferOrder::isToBeIgnored(const std::vector<basar::Int16>	vecForBranchnos) const	
		{
			METHODNAME_DEF( EdiTransferOrder, isToBeIgnored )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			bool ret = false;	// false: is NOT to be ignored, it is to be accepted

			// Correct InputType?
			if (domMod::orderMapper::enInputtype_inhCH	==	m_Inputtype )
			{
				// Never ignore Swiss orders
				ret = false;
			}
			else if (domMod::orderMapper::enInputtype_inhDE	==	m_Inputtype )
			{
				// Test for German conditions
				ret = isToBeIgnoredDE();
			}
			else
			{
				// cannot happen. Ignore it.
				ret = true;
			}

			if (ret)
			{
				// Cannot get worse for this order 
				return ret;
			}

			// The transfer order belongs to a branch that is accepted? Otherwise ignore it.
			ret = true;
			if ( !vecForBranchnos.empty())			// Otherwise no branch was given at all in parameters: Then ignore all orders
			{
				basar::Int32 cntElems = static_cast<basar::Int32>(vecForBranchnos.size());

				for (basar::Int32 index = cntElems -1 ; index >= 0 ; --index )
				{
					if ( m_Branchno == vecForBranchnos.at(index))
					{
						// The branchno of this order belongs to the accepted ones from the appargs-list
						ret = false;
						break;
					}
				}
			}

			// Branchno-List is empty or it didn't contain the branchno of this transfer order
			return ret;
		}


		bool EdiTransferOrder::isToBeIgnoredDE() const
		{
			METHODNAME_DEF( EdiTransferOrder, isToBeIgnoredDE )  
			BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

			if ( m_Bgm_02_LinePtr )
			{
				// orders with BGM_DOKCODE = '130' have to be ignored
				// See PBS999, Sonderfall 2: "* SAETZE MIT 130 IM BGM UBERLESEN"
				if (m_Bgm_02_LinePtr->isDokcode130())
				{
					std::stringstream str;
					str << "INFO: Transfer order has to be ignored (BGM-DOKCODE=130)";
					BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					
					return true;
				}	
			}
			
			// Don't ignore
			return false;
		}

		

	} // namespace transferOrderDM
} // namespace domMod
