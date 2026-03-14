//-------------------------------------------------------------------------------------------------//
/*! \file	orderproposaldm.h
 *  \brief  implemtation of domain module (orderproposal)
 *  \author Xin Zhou
 *  \date   30.06.2013
 */
 //-------------------------------------------------------------------------------------------------//
#ifndef GUARD_ORDERPROPOSALDM_H
#define GUARD_ORDERPROPOSALDM_H

//**************************************************************************************************
// includes
//**************************************************************************************************
#include <libbasar_definitions.h>
#include <libbasardbaspect_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasardbaspect_connectionref.h>
#include <libbasarcmnutil_i18nstring.h>
#include <libbasarcmnutil_bstring.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarcmnutil_parameterlist.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include "loggerpool.h"
//#include <libbasarlogin.h>
#include <libbasardbaspect.h>
#include "searchyiteratorptr.h"
#include "searchyiterator.h"

//**************************************************************************************************

namespace domMod 
{
	//---------------------------------------------------------------------------
	/*! \brief Domain class for copying orderproposal
		This is a final class.
		\throw no-throw
	*/
	//---------------------------------------------------------------------------
	class OrderProposalDM 
	{
	public:
		//**************************************************************************************************
		/*! \brief Std constructor for domain module CopyOrderProposal
			\throw no-throw
		*/
		//**************************************************************************************************
		OrderProposalDM();

		//**************************************************************************************************
		/*! \brief Std destructor for domain module CopyOrderProposal
			\throw no-throw
		*/
		//**************************************************************************************************
		~OrderProposalDM();

		//**************************************************************************************************
		/*! \brief This method inits the module
			\param activeConn database connection
			\throw no-throw
		*/
		//**************************************************************************************************
		void init(const basar::db::aspect::ConnectionRef activeConn);

		//**************************************************************************************************
		/*! \brief Call this function to clear all accessor instances and the contents of property tables
			\throw no-throw
		*/
		//**************************************************************************************************
		void shutdown();

		//---------------------------------------------------------------------------
		// functions for accessors
		//---------------------------------------------------------------------------
		//**************************************************************************************************
		//         SAVES (Update / Insert)
		//**************************************************************************************************
		//**************************************************************************************************
		/*! \brief Call this function to move dataset from orderproposal into the archive tables
			\@param yit YIterator
			\@return true, if everything worked fine\n
					false otherwise
			\@throw no-throw
		*/
		//**************************************************************************************************
		bool copyOrderProposal(const basar::db::aspect::AccessorPropertyTable_YIterator yit);

		//**************************************************************************************************
		//         OTHER STUFF
		//**************************************************************************************************
		virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit();

	private:
		//**************************************************************************************************
		/*! \brief Copy constuctor.
			\note forbidden
		*/
		//**************************************************************************************************
		OrderProposalDM(const OrderProposalDM& r);

		//**************************************************************************************************
		/*! \brief Assignment constuctor.
			\note forbidden
		*/
		//**************************************************************************************************
		OrderProposalDM& operator = (const OrderProposalDM& r);

		//**************************************************************************************************
		/*! \brief helpfunction to convert the YIterator.
			\note for future use. It's implemented but not used
			\@param yit an AccessorPropertyTable_YIterator to convert a string type
			\@return another AccessorPropertyTable_YIterator with informix datetype
		*/
		//**************************************************************************************************
		basar::db::aspect::AccessorPropertyTable_YIterator map2SearchYit(basar::db::aspect::AccessorPropertyTable_YIterator yit);

		//**************************************************************************************************
		/*! \brief helpfunction and helpmembervariable for the unit test.
			\note
		*/
		//**************************************************************************************************
		searchyiterator::SearchYIteratorPtr             getSearchYIteratorPtr();
		searchyiterator::SearchYIteratorPtr             m_SearchYIterator;

		//---------------------------------------------------------//
		// Accessors: YIterators and AccessorPropertyTableRef
		//---------------------------------------------------------//
		basar::db::aspect::AccessorInstanceRef	m_OrderProposalACC;	//!< instance of accessor BatchOrdersAcc, wird als Ref-Klasse automatisch freigegeben
		basar::Int32 m_Initialized;	//!< counter of init-calls
	};
} // domMod
#endif // GUARD
