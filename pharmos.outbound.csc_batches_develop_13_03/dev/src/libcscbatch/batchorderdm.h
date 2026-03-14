//-------------------------------------------------------------------------------------------------//
/*! \file	batchorderdm.h
 *  \brief  implemtation of domain module (batchorder)
 *  \author Beatrix Trömel
 *  \date   30.05.2008
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_BATCHORDERDM_H
#define GUARD_BATCHORDERDM_H

#include <libbasar_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasarappl_systemeventmanager.h>
#include <libbasarlogin.h>
#include <libbasardbaspect.h>
#include "libcscbatch_loggerpool.h"

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
	class BatchOrderDM {
	public:
		//---------------------------------------------------------------------------
		// global functions of this domMod
		//---------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------//
		/*! \brief std constructor
		 *!	\throw no-throw 
		*/
		//-------------------------------------------------------------------------------------------------//
		BatchOrderDM();

		//-------------------------------------------------------------------------------------------------//
		/*! \brief std constructor
		 *!	\throw no-throw 
		*/
		//-------------------------------------------------------------------------------------------------//
		~BatchOrderDM();

		//-------------------------------------------------------------------------------------------------//
		/*! \brief initilization procedure
			\param connection connection to database
		 *!	\throw no-throw 
		*/
		//-------------------------------------------------------------------------------------------------//
		void init(const ConnectionRef activeConn);

		/*! \brief call this function to clear all accessor instances and the contents of property tabels
		\n no-throw */
		void shutdown();
		/*! \brief call this function to clear the accessor iterator
		\n no-throw */
		void clearSearchIterator();
		basar::appl::EventReturnStruct beginTransaction();
		basar::appl::EventReturnStruct commitTransaction();
		basar::appl::EventReturnStruct rollbackTransaction();

		//---------------------------------------------------------------------------
		// functions for accessor m_TemplateACC
		//---------------------------------------------------------------------------
		/////////////////////////// FINDS ///////////////////////////////////////////
		//---------------------------------------------------------------------------
		AccessorPropertyTableRef findBatchOrdersByPattern(const AccessorPropertyTable_YIterator yit);
	
		AccessorPropertyTableRef findArticleByBranchNoAndArticleNo(const AccessorPropertyTable_YIterator yit);

		/*! \brief select xyz by ID (parameterlist containing ID) - one row expected
		\n no-throw */
		AccessorPropertyTableRef selOrderTypesByOrderNo(const AccessorPropertyTable_YIterator yit);

		/*! \brief select xyz by ID (parameterlist containing ID) - one row expected
		\n no-throw */
		AccessorPropertyTableRef SelCustomerBlocked(const AccessorPropertyTable_YIterator yit);

		/*! \brief select xyz by ID (parameterlist containing ID) - one row expected
		\n no-throw */
		AccessorPropertyTableRef findBatchOrdersByBranchNo(const basar::Int16 BranchNo);

		AccessorPropertyTableRef findPositionsByBranchNoAndOrderNo(const AccessorPropertyTable_YIterator yit);

			
		/////////////////////////// SAVES (Update / Insert)//////////////////////////	
		//---------------------------------------------------------------------------
//		basar::appl::EventReturnStruct saveBatchOrder(const AccessorPropertyTable_YIterator yit);
		basar::appl::EventReturnStruct saveBatchOrderByPattern(AccessorPropertyTable_YIterator yit);
		basar::appl::EventReturnStruct saveBatchOrderStatusChange(AccessorPropertyTable_YIterator yit);
		basar::appl::EventReturnStruct saveBatchOrderForRelease(const AccessorPropertyTable_YIterator yit);
		bool savePositions(const AccessorPropertyTable_YIterator yit);

		/////////////////////////// DELETES /////////////////////////////////////////
		//---------------------------------------------------------------------------

		/////////////////////////// CHECKS //////////////////////////////////////////
		//---------------------------------------------------------------------------
		/*! \brief check xyz
		\n InvalidXYZException */

		/////////////////////////// PROPTABS / YITERATORS ///////////////////////////
		//-------------------------------------------------------------------------------------------------//
		/*! \brief clears property table for m_BatchOrderACC
			\throw no-throw 
			\return -
		*/
		//-------------------------------------------------------------------------------------------------//
		void clearOrderHeaderPropTab();
		//-------------------------------------------------------------------------------------------------//
		/*! \brief get property table for m_BatchOrderACC
			\throw no-throw 
			\return reference of accessor´s propertytable
		*/
		//-------------------------------------------------------------------------------------------------//
		AccessorPropertyTableRef getOrderHeaderPropTab();
		//-------------------------------------------------------------------------------------------------//
		/*! \brief get property table for m_BatchOrderPosACC
			\throw no-throw 
			\return reference of accessor´s propertytable
		*/
		//-------------------------------------------------------------------------------------------------//
		AccessorPropertyTableRef getOrderPosPropTab();

		//-------------------------------------------------------------------------------------------------//
		/*! \brief get property table for m_searchBatchOrderACC
			\throw no-throw 
			\return reference of accessor´s propertytable
		*/
		//-------------------------------------------------------------------------------------------------//
		AccessorPropertyTableRef getSearchOrderPropTab();

		//-------------------------------------------------------------------------------------------------//
		/*! \brief get property table for m_actionBatchOrderACC
			\throw no-throw 
			\return reference of accessor´s propertytable
		*/
		//-------------------------------------------------------------------------------------------------//
		AccessorPropertyTableRef getActionOrderPropTab();

		/////////////////////////// DomainModules //////////////////////////////////////////
		//---------------------------------------------------------------------------
			
	private:
		/*! \brief copy constructor 
		\n     no-throw */
		BatchOrderDM(const BatchOrderDM &r);

		/*! \brief assign operator 
		\n     no-throw */
		BatchOrderDM &operator = (const BatchOrderDM &r);
			
		//---------------------------------------------------------------------------
		// accessor instances ( created in init() )
		//---------------------------------------------------------------------------
		AccessorInstanceRef	m_iniACC;	//!< instance of accessor iniAcc, wird als Ref-Klasse automatisch freigegeben
		AccessorInstanceRef	m_BatchOrderSearchACC;	//!< instance of accessor SelBatchOrdersAcc, wird als Ref-Klasse automatisch freigegeben
		AccessorInstanceRef	m_BatchOrderSaveACC;	//!< instance of accessor SelBatchOrdersAcc, wird als Ref-Klasse automatisch freigegeben
		AccessorInstanceRef	m_BatchOrderPosACC;	//!< instance of accessor SelBatchOrderPosAcc, wird als Ref-Klasse automatisch freigegeben
		AccessorInstanceRef	m_BatchOrderPosFindACC;	//!< instance of accessor SelBatchOrderPosAcc, wird als Ref-Klasse automatisch freigegeben
		AccessorInstanceRef	m_searchBatchOrderACC;	//!< instance of accessor SearchBatchOrdersAcc, wird als Ref-Klasse automatisch freigegeben
		AccessorInstanceRef	m_actionBatchOrderACC;	//!< instance of accessor ActionBatchOrdersAcc, wird als Ref-Klasse automatisch freigegeben
		AccessorInstanceRef	m_CscInfosACC;	//!< instance of accessor SelBatchOrdersAcc, wird als Ref-Klasse automatisch freigegeben
		AccessorInstanceRef	m_CustomerBlockedACC;	//!< instance of accessor SelCustomerBlocked, wird als Ref-Klasse automatisch freigegeben
		basar::Int32 m_Initialized;	//!< counter of init-calls
	};
} // domMod
}
#endif // GUARD
