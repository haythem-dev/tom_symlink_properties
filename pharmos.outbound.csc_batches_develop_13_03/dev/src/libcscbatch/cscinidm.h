//-------------------------------------------------------------------------------------------------//
/*! \file	cscinidm.h
 *  \brief  implemtation of domain module (cscini)
 *  \author Beatrix Trömel
 *  \date   29.01.2009
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_CSCINIDM_H
#define GUARD_CSCINIDM_H

#include <libbasar_definitions.h>
#include <libbasardbaspect.h>
#include <libbasarappl_systemeventmanager.h>
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
	class CscIniDM {
	public:
		//---------------------------------------------------------------------------
		// global functions of this domMod
		//---------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------//
		/*! \brief std constructor
		 *!	\throw no-throw 
		*/
		//-------------------------------------------------------------------------------------------------//
		CscIniDM();

		//-------------------------------------------------------------------------------------------------//
		/*! \brief std constructor
		 *!	\throw no-throw 
		*/
		//-------------------------------------------------------------------------------------------------//
		~CscIniDM();

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

		//---------------------------------------------------------------------------
		// functions for accessor m_TemplateACC
		//---------------------------------------------------------------------------
		/////////////////////////// FINDS ///////////////////////////////////////////
		//---------------------------------------------------------------------------
		AccessorPropertyTableRef SelEnterByBranch(const AccessorPropertyTable_YIterator yit);

		//---------------------------------------------------------------------------
		/////////////////////////// SAVES (Update / Insert)//////////////////////////	
		//---------------------------------------------------------------------------
		basar::appl::EventReturnStruct SaveEnterByBranch(const AccessorPropertyTable_YIterator yit);
		basar::appl::EventReturnStruct DeleteEnterByBranch(const AccessorPropertyTable_YIterator yit);

		/////////////////////////// DELETES /////////////////////////////////////////
		//---------------------------------------------------------------------------

		/////////////////////////// CHECKS //////////////////////////////////////////
		//---------------------------------------------------------------------------

		/////////////////////////// PROPTABS / YITERATORS ///////////////////////////

		/////////////////////////// DomainModules //////////////////////////////////////////
		//---------------------------------------------------------------------------
			
	private:
		/*! \brief copy constructor 
		\n     no-throw */
		CscIniDM(const CscIniDM &r);

		/*! \brief assign operator 
		\n     no-throw */
		CscIniDM &operator = (const CscIniDM &r);
			
		//---------------------------------------------------------------------------
		// accessor instances ( created in init() )
		//---------------------------------------------------------------------------
		AccessorInstanceRef	m_CscInitACC;	//!< instance of accessor iniAcc, wird als Ref-Klasse automatisch freigegeben
		basar::Int32 m_Initialized;	//!< counter of init-calls
	};
} // domMod
}
#endif // GUARD
