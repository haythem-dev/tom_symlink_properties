//-------------------------------------------------------------------------------------------------//
/*! \file	cscorderdm.h
 *  \brief  implemtation of domain module (cscorder)
 *  \author Beatrix Trömel
 *  \date   05.08.2008
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CSCORDERDM_H
#define GUARD_CSCORDERDM_H

#include <libbasar_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasardbaspect.h>
#include <kscclientwrapper/kscclientdm.h>

#include "libcscbatch_loggerpool.h"
#include "batchorderdm.h"
#include "libabbauw/icscordercreator.h"
#include "libabbauw/iorderreleaseparameterptr.h"
#include "icscsessionptr.h"

//---------------------------------------------------------------------------
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::cmnutil::ParameterList;

namespace libcscbatch {
namespace domMod {
	//---------------------------------------------------------------------------
	/*! \brief  domain class.
	\n This is a final class.  
	\n throws no-throw */

	//--------------------------------------------------------------------------------------------------//
	// static definition section
	//--------------------------------------------------------------------------------------------------//
	static const basar::I18nString	PARAM_VALUE_OFF = "OFF";


    class CSCOrderDM : public libabbauw::domMod::cscorder::ICSCOrderCreator
	{
	public:

		//---------------------------------------------------------------------------
		// global functions of this domMod
		//---------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------//
		/*! \brief std constructor
		 *!	\throw no-throw 
		*/
		//-------------------------------------------------------------------------------------------------//
		CSCOrderDM( const ConnectionRef, const basar::Int16 branchNo );
        CSCOrderDM( const ConnectionRef, const basar::Int16 branchNo, const basar::VarString KscServer, const basar::Int32 KscPort );
                        
		//-------------------------------------------------------------------------------------------------//
		/*! \brief std constructor
		 *!	\throw no-throw 
		*/
		//-------------------------------------------------------------------------------------------------//
		~CSCOrderDM();

		void injectParameter( libabbauw::domMod::parameter::IOrderReleaseParameterPtr );
        void setCSCSession( CSC_Batches::infrastructure::session::ICSCSessionPtr );

		/*! \brief This method inits the module
		\n no-throw */
		void init();

		/*! \brief call this function to clear all accessor instances and the contents of property tabels
		\n no-throw */
		void shutdown();

		/////////////////////////// SAVES (Update / Insert)//////////////////////////	
		//-------------------------------------------------------------------------------------------------//
		/*! \brief Creates a single KSC order.
			\param yit with parameters
			\param ReasonForCancelling ReasonForCancellingEnum
		 *!	\throw no-throw 
		*/
		//-------------------------------------------------------------------------------------------------//
		//orig without ABBA-UW: basar::appl::EventReturnType createCSCOrder(const AccessorPropertyTable_YIterator yit, const AccessorPropertyTable_YIterator yitExtra, basar::Int32 &ReasonForCancelling);
//		basar::appl::EventReturnType createCSCOrder(const AccessorPropertyTable_YIterator yit, basar::Int32 &ReasonForCancelling);
		basar::appl::EventReturnType createCSCOrder(const AccessorPropertyTable_YIterator yit, 
														const AccessorPropertyTable_YIterator yitExtra,
														const bool = false); // Parameter postpone: default=false

		basar::appl::EventReturnType createCSCOrderABBAUW(	basar::Int16		branchno,				// yit
                                                            basar::Int16        branchnoorig,           // yit
															basar::Int32		orderno,				// yit
															basar::Int32		pharmacyno, 			// yit
                                                            basar::Int32        purchaseorderno,        // yit P_17000393
															const basar::I18nString&	datasourcetype, // yit
															const basar::I18nString&	ordertype, 		// yit
															const basar::I18nString&	headertext,		// yit
															const basar::Date&		deferredpaymentdate,// yitExtra
															const basar::Date&		edideliverydate,	// yitExtra
															const bool			postpone				// true: postpone order, false: close order completely
															)	;			

		/////////////////////////// DELETES /////////////////////////////////////////
		//---------------------------------------------------------------------------

		/////////////////////////// CHECKS //////////////////////////////////////////
		//---------------------------------------------------------------------------
		bool isTransferOrder(const AccessorPropertyTable_YIterator yit);
		bool isTransferOrder(const basar::I18nString ordertype);

		bool isUWOrder(const AccessorPropertyTable_YIterator yit);
		bool isUWOrder(const basar::I18nString ordertype);

		bool isUNOrder(const AccessorPropertyTable_YIterator yit);
		bool isUNOrder(const basar::I18nString ordertype);

		bool isMDOrder(const AccessorPropertyTable_YIterator yit);
		bool isMDOrder(const basar::I18nString ordertype);

		bool isOMOrder(const AccessorPropertyTable_YIterator yit);
		bool isOMOrder(const basar::I18nString ordertype);

		bool isHZOrder(const AccessorPropertyTable_YIterator yit);
		bool isHZOrder(const basar::I18nString ordertype);

		bool isAKOrder(const AccessorPropertyTable_YIterator yit);
		bool isAKOrder(const basar::I18nString ordertype);

		bool isGMOrder(const AccessorPropertyTable_YIterator yit);
		bool isGMOrder(const basar::I18nString ordertype);

		bool isCMOrder(const AccessorPropertyTable_YIterator yit);
		bool isCMOrder(const basar::I18nString ordertype);

		/////////////////////////// PROPTABS / YITERATORS ///////////////////////////
		//---------------------------------------------------------------------------

		/////////////////////////// OTHERS //////////////////////////////////////////
		//---------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------------------//
		AccessorPropertyTableRef getOrderHeaderPropTab();
		//-------------------------------------------------------------------------------------------------//
		/*! \brief get property table for m_BatchOrderPosACC
			\throw no-throw 
			\return reference of accessor´s propertytable
		*/
		//-------------------------------------------------------------------------------------------------//
		//---------------------------------------------------------------------------
		/////////////////////////// FINDS ///////////////////////////////////////////
		//---------------------------------------------------------------------------

		/////////////////////////// SAVES (Update / Insert)//////////////////////////	
		//---------------------------------------------------------------------------

		/////////////////////////// DELETES /////////////////////////////////////////
		//---------------------------------------------------------------------------

		/////////////////////////// CHECKS //////////////////////////////////////////
		//---------------------------------------------------------------------------

		/////////////////////////// PROPTABS / YITERATORS ///////////////////////////
		//---------------------------------------------------------------------------

	private:
		/*! \brief copy constructor 
		\n     no-throw */
		CSCOrderDM(const CSCOrderDM &r);

		/*! \brief assign operator 
		\n     no-throw */
		CSCOrderDM &operator = (const CSCOrderDM &r);
		// orig without ABBA: basar::appl::EventReturnType createCSCItems(basar::Int32 OrderNo, basar::Int32 idfOrigin);
		// orig without ABBA: basar::appl::EventReturnType createCSCItem(AccessorPropertyTable_YIterator yit, AccessorPropertyTable_YIterator yitExtra);

		basar::appl::EventReturnType createCSCItems(basar::Int32 OrderNo, basar::Int32 idfOrigin, basar::I18nString strOrdertype);
		basar::appl::EventReturnType createCSCItem (AccessorPropertyTable_YIterator yit, AccessorPropertyTable_YIterator yitExtra, basar::I18nString strOrdertype);

		basar::appl::EventReturnStruct changeBatchOrderStatus(basar::Int32 OrderNo, basar::Int32 OrderAcquisitionDate, basar::Int32 OrderNoKsc);
		basar::appl::EventReturnStruct buildYitForUpdate(basar::Int32 OrderNo, basar::Int32 OrderAcquisitionDate, basar::Int32 OrderNoKsc,AccessorPropertyTable_YIterator &yitUpdate);
		bool isCustomerEnabled(basar::Int32 pharmacyno);


		//*************************************** setBatchType ***********************************************
		//
		// Descr.:	Sets the integer value BatchType for filling database attribute kdauftrag.batchschreiben.
		//	
		//	
		//***************************************************************************************************************
		basar::Int32 getBatchType(const basar::VarString& datasourcetype);
			
		//---------------------------------------------------------------------------
		// accessor instances ( created in init() )
		//---------------------------------------------------------------------------

		//---------------------------------------------------------------------------
		// Connections
		//---------------------------------------------------------------------------
		basar::db::aspect::ConnectionRef m_connection; //!< reference to the existing db connection to Informix

		//---------------------------------------------------------------------------
		// accessor instances ( created in init() )
		//---------------------------------------------------------------------------
		AccessorInstanceRef	m_BatchOrderAbbaACC;	//!< instance of accessor SelBatchOrdersAcc, wird als Ref-Klasse automatisch freigegeben

		//---------------------------------------------------------//
		// DomainModules
		//---------------------------------------------------------//	
        ::domMod::KSCClientDMConnParams m_KscConnection;
		::domMod::KSCClientDM			m_KSCClientDM; //!< domain module class
		domMod::BatchOrderDM			m_batchOrderDM; //!< domain module class
		libabbauw::domMod::parameter::IOrderReleaseParameterPtr m_ParameterDM;

		//---------------------------------------------------------//
		// Other
		//---------------------------------------------------------//	
		basar::Int32 m_Initialized;	//!< counter of init-calls
		basar::Int16 m_branchNo; //!< branch no from login
	};
} // domMod
}
#endif // GUARD
