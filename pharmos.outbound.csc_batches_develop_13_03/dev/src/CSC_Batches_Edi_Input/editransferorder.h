//-------------------------------------------------------------------------------------
/*! \file  editransferorder.h
*  \brief  class for transfer order. 
*  \author Jia Li, P. Kessling
*  \date   02.01.2013 
*  history: 09.04.13 pk rework
*           xx.11.14 pk German linetypes added
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_TRANSFERORDERDM_EDITRANSFERORDER_H
#define GUARD_DOMMOD_TRANSFERORDERDM_EDITRANSFERORDER_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <boost/filesystem/fstream.hpp> // fuer filestream

#include <libbasardbaspect_connectionref.h>

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

#include "unb_00_line.h"
#include "unh_01_line.h"
#include "bgm_02_line.h"
#include "dtm_03_line.h"
#include "nad_04_line.h"
#include "ftx_11_line.h"
#include "uns_97_line.h"

//#include "abbauworderdm.h"

#include "positionsegment.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//

namespace domMod
{
	namespace transferOrderDM
	{

    //-------------------------------------------------------------------------------------------------//
    // class declaration section
    //-------------------------------------------------------------------------------------------------//

		class EdiTransferOrder 
		{

		//--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		
		public:

				EdiTransferOrder();
				~EdiTransferOrder();

				domMod::orderMapper::InputtypeEnum	getInputtype() const;	// getter, setter
				void								setInputtype(const domMod::orderMapper::InputtypeEnum inputtype);

				const std::ostream::pos_type	getOrderStartPos() const;	// getter, setter
				void							setOrderStartPos(const std::ostream::pos_type& orderStartPos);	

				basar::Int16					getBranchno() const;	// gets the orderno for orderhead table or -1 if not set	
				void							setBranchno(const basar::Int16 branchno);	
				basar::Int32					getOrdernoBatch() const;	
				void							setOrdernoBatch(const basar::Int32 ordernoBatch);	
				const basar::I18nString&		getStatus() const;	
				void							setStatus(const basar::I18nString status);	
				const basar::I18nString&		getOrdertype() const;	
				void							setOrdertype(const basar::I18nString ordertype);	
				const basar::I18nString&		getOrderpickingtype() const;	
				void							setOrderpickingtype(const basar::I18nString orderpickingtype);	
				const basar::I18nString&		getBookentrytype() const;	
				void							setBookentrytype(const basar::I18nString bookentrytype);	
				const basar::I18nString&		getHeadertext() const;	
				void							setHeadertext(const basar::I18nString headertext);	
				basar::Int32					getOrderDateCH() const;	
				basar::Int32					getOrderDateDE() const;	
				basar::Int32					getDeferredPaymentDateCH() const;	
				basar::Int32					getDeferredPaymentDateDE() const;	
				basar::Int32					getEdiDeliveryDateCH() const;	
				basar::Int32					getEdiDeliveryDateDE() const;	
				const basar::I18nString			getOrdernoMFCH() const;	
				const basar::I18nString			getOrdernoMFDE() const;	
				basar::Int32					getPharmacyno() const;	
				void							setPharmacyno(const basar::Int32 pharmacyno);	
				basar::Int32					getManufacturerno() const;	
				void							setManufacturerno(const basar::Int32 manufacturerno);	
				const basar::I18nString			getPharmacynoMFCH() const;	
				const basar::I18nString			getPharmacynoMFDE() const;	

				std::vector <PositionSegmentPtr>	getPositionSegments() const;			// gets the list of positions


				// for CH
				domMod::line::UnbLinePtr		getUnbLine_Ptr()				const;	
				domMod::line::NadLinePtr		getNadLine_ManufacturerPtr()	const;	
				domMod::line::NadLinePtr		getNadLine_BranchPtr()			const;	
				domMod::line::NadLinePtr		getNadLine_PharmacyPtr()		const;	

				// for DE
				domMod::line::Unb_00_LinePtr	getUnb_00_Line_Ptr()				const;	
				domMod::line::Nad_04_LinePtr	getNad_04_Line_BranchPtr()			const;	
				domMod::line::Nad_04_LinePtr	getNad_04_Line_PharmacyBYPtr()		const;	
				domMod::line::Nad_04_LinePtr	getNad_04_Line_PharmacyCNPtr()		const;	
				domMod::line::Nad_04_LinePtr	getNad_04_Line_ManufacturerPtr()	const;	
				domMod::line::Ftx_11_LinePtr	getFtx_11_Line_Ptr()				const;


				basar::Int32					getCntPositions() const;	// gets the number of positions currently stored	


				bool empty();											// clear all stored lines etc.
				bool mountLine(domMod::line::EDIFACTLineBasePtr&);		// mounts this processed and valid line into transfer order
				bool isValid() const;									// is Info complete and ok for storage?
				bool isToBeIgnored(const std::vector<basar::Int16>	vecForBranchnos) const;		// is this order to be ignored and not to be stored in the database?


			private:
  
			/////////////////////////////////////////////////////////////////////////
		    // private member methods
		    /////////////////////////////////////////////////////////////////////////

				EdiTransferOrder( const EdiTransferOrder& );				// abschalten
				EdiTransferOrder& operator=( const EdiTransferOrder& );		// abschalten

				void resetMembers();									// resets everything in class

				// mount a processed and valid line into transfer order
				bool mountUNBLine(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountUNHLine(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountBGMLine(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountDTMLine(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountNADLine(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountUNSLine(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountLINLine(const domMod::line::EDIFACTLineBasePtr&);		// into position
				bool mountIMDLine(const domMod::line::EDIFACTLineBasePtr&);		// into position
				bool mountQTYLine(const domMod::line::EDIFACTLineBasePtr&);		// into position
				bool mountPCDLine(const domMod::line::EDIFACTLineBasePtr&);		// into position
				bool mountUNTLine(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountUNZLine(const domMod::line::EDIFACTLineBasePtr&);	

				bool mountUNB_00_Line(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountUNH_01_Line(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountBGM_02_Line(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountDTM_03_Line(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountNAD_04_Line(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountALC_05_Line(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountPCD_06_Line(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountLIN_08_Line(const domMod::line::EDIFACTLineBasePtr&);
				bool mountQTY_10_Line(const domMod::line::EDIFACTLineBasePtr&);
				bool mountFTX_11_Line(const domMod::line::EDIFACTLineBasePtr&);	
				bool mountUNS_97_Line(const domMod::line::EDIFACTLineBasePtr&);	


				bool isValidInhCH() const;									// is Info complete and ok for storage?
				bool isValidInhDE() const;									// is Info complete and ok for storage?
				bool isToBeIgnoredDE() const;								// is this order to be ignored and not to be stored in the database?


		    /////////////////////////////////////////////////////////////////////////
		    // private member variables
		    /////////////////////////////////////////////////////////////////////////

				domMod::orderMapper::InputtypeEnum  m_Inputtype;			// Format of inputfile with transferorders to be read  (DE or CH)

				std::ostream::pos_type				m_OrderStartPos;		// position of the first character of this order in the flatfile

				// Inhouse CH lines
				domMod::line::UnbLinePtr			m_UnbLinePtr;
				domMod::line::UnhLinePtr			m_UnhLinePtr;
				domMod::line::BgmLinePtr			m_BgmLinePtr;
				domMod::line::DtmLinePtr			m_DtmLine_OrderDatePtr;				// DTM 137 	
				domMod::line::DtmLinePtr			m_DtmLine_EdiDeliveryDatePtr;		// DTM 2
				domMod::line::DtmLinePtr			m_DtmLine_DeferredPaymentDatePtr;	// DTM 209	(valutadate, Zahlungsziel), optional
				//domMod::line::DtmLinePtr			m_DtmLine_4;						// DTM 4		// info of this field is not utilized	
				domMod::line::NadLinePtr			m_NadLine_ManufacturerPtr;			// NAD MF		
				domMod::line::NadLinePtr			m_NadLine_BranchPtr;				// NAD DS (wholesale, deliveryservice)		
				domMod::line::NadLinePtr			m_NadLine_PharmacyPtr;				// NAD BY (pharmacy)		
				domMod::line::UnsLinePtr			m_UnsLine_DPtr;						// the UnsLines embrace the Positionssegment ..
				domMod::line::UnsLinePtr			m_UnsLine_SPtr;						// .. logically it should be a segment of its own, but we want to avoid overkill
				domMod::line::UntLinePtr			m_UntLinePtr;		
				domMod::line::UnzLinePtr			m_UnzLinePtr;	

				// Inhouse DE lines
				domMod::line::Unb_00_LinePtr		m_Unb_00_LinePtr;
				domMod::line::Unh_01_LinePtr		m_Unh_01_LinePtr;
				domMod::line::Bgm_02_LinePtr		m_Bgm_02_LinePtr;
				domMod::line::Dtm_03_LinePtr		m_Dtm_03_Line_OrderDatePtr;				// DTM 137 	
				domMod::line::Dtm_03_LinePtr		m_Dtm_03_Line_EdiDeliveryDatePtr;		// DTM 2
				domMod::line::Dtm_03_LinePtr		m_Dtm_03_Line_DeferredPaymentDatePtr;	// DTM 209	or DTM ZZZ (valutadate, Zahlungsziel), optional
				domMod::line::Nad_04_LinePtr		m_Nad_04_Line_ManufacturerPtr;			// NAD MF		
				domMod::line::Nad_04_LinePtr		m_Nad_04_Line_BranchPtr;				// NAD DS (wholesale, deliveryservice)		
				domMod::line::Nad_04_LinePtr		m_Nad_04_Line_PharmacyBYPtr;			// NAD BY (pharmacy)		
				domMod::line::Nad_04_LinePtr		m_Nad_04_Line_PharmacyCNPtr;			// NAD CN (pharmacy, CN has preference before BY)																							// TODO: Evtl. fehltnoch Eintrag für NAD_04 CN
																							// TODO: Evtl. fehlt ALC-line
				domMod::line::Ftx_11_LinePtr		m_Ftx_11_LinePtr;						// FTX - order head text
				domMod::line::Uns_97_LinePtr		m_Uns_97_Line_DPtr;						// .. logically it should be a segment of its own, but we want to avoid overkill
				domMod::line::Uns_97_LinePtr		m_Uns_97_Line_SPtr;						// .. logically it should be a segment of its own, but we want to avoid overkill


				std::vector <PositionSegmentPtr>	m_PositionSegments;			// for the list of positions

				// Additional order info for info into orderhead table
				basar::Int16						m_Branchno;				
				basar::Int32						m_OrdernoBatch;			// the orderno in table orderhead is inserted as soon as it is generated
				basar::I18nString					m_Status;				
				basar::I18nString					m_Ordertype;				
				basar::I18nString					m_Orderpickingtype;		 
				basar::I18nString					m_Bookentrytype;	
				basar::I18nString					m_Headertext;						
				basar::Int32						m_Pharmacyno;			
				basar::Int32						m_Manufacturerno;
		};

		class positionSegment;		// forward
		typedef boost::shared_ptr< EdiTransferOrder > EdiTransferOrderPtr;

	} // namespace transferOrderDM
} // namespace domMod

#endif // GUARD_DOMMOD_TRANSFERORDERDM_EDITRANSFERORDER_H
