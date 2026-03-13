//-------------------------------------------------------------------------------------
/*! \file  customerconsultantdm.h
*  \brief  Domain module for database operations regarding custmers
*  \author P. Kessling
*  \date   03.05.2013, 
*  history 03.05.13  pk new, similar to batchorderdm.h
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LIBABBAUW_CUSTOMERCONSULTANTDM_H
#define GUARD_DOMMOD_LIBABBAUW_CUSTOMERCONSULTANTDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers

#include <libbasar_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasarappl_systemeventmanager.h>
#include "loggerpool.h"
#include <libbasarlogin.h>
#include <libbasardbaspect.h>

//-------------------------------------------------------------------------------------------------//
// using section
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::cmnutil::ParameterList;


//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//

namespace domMod
{
	namespace libAbbauw
	{
		namespace customerConsultantDM
		{

			//-------------------------------------------------------------------------------------------------//
			// class declaration section
			//-------------------------------------------------------------------------------------------------//
			class CustomerConsultantDM 
			{
				//--------------------------------------------------------------------------------------------------//
				// static declaration section
				//--------------------------------------------------------------------------------------------------//
			
				//--------------------------------------------------------------------------------------------------//
				// object declaration section
				//--------------------------------------------------------------------------------------------------//
				public:
						CustomerConsultantDM();									
						~CustomerConsultantDM();

						// This method inits the module, creates AccessorInstances for the given connection
						// no-throw 
						// Basar-docu, p.40
						void init(const ConnectionRef activeConn);

						// call this function to clear all accessor instances and the contents of property tabels
						// no-throw */
						// Basar-docu, p.41
						void shutdown();

						//---------------------------------------------------------------------------
						// functions for accessor m_???ACC
						//---------------------------------------------------------------------------
						/////////////////////////// FINDS ///////////////////////////////////////////
						//---------------------------------------------------------------------------

						// from artikelzentral, specification-chapter 5.6
						// Basar-docu, p.41
						basar::I18nString			findCustomerGroupByBranchnoPharmacyno(const basar::Int16 branchno, const basar::Int32 pharmacyno);
						basar::Int32				findPharmacynoByBranchnoPharmacynoean(const basar::Int16 branchno, const basar::I18nString pharmacynoean);
						basar::Int32				findPseudopharmacynoByBranchno		 (const basar::Int16 branchno);

						/////////////////////////// SAVES (Update / Insert)//////////////////////////	
						//---------------------------------------------------------------------------

						/////////////////////////// DELETES /////////////////////////////////////////
						//---------------------------------------------------------------------------

						/////////////////////////// CHECKS //////////////////////////////////////////
						//---------------------------------------------------------------------------
						bool						checkExistenceByBranchnoPharmacyno(const basar::Int16 branchno, const basar::Int32 pharmacyno);

						/////////////////////////// PROPTABS / YITERATORS ///////////////////////////
						//-------------------------------------------------------------------------------------------------//
						//-------------------------------------------------------------------------------------------------//
						//-------------------------------------------------------------------------------------------------//
						// get property table for m_???ACC
						//		throw no-throw 
						//      return reference of accessor´s propertytable
						//-------------------------------------------------------------------------------------------------//
						// BASAR-docu p. 42 chapter 5.6.9
						//basar::db::aspect::AccessorPropertyTableRef getCustomerPropTab();



			private:
					/////////////////////////////////////////////////////////////////////////
					// private member methods
					/////////////////////////////////////////////////////////////////////////

					// special member functions
					CustomerConsultantDM(const CustomerConsultantDM &r);				// Copy-Konstruktor : abschalten
					CustomerConsultantDM &operator = (const CustomerConsultantDM &r);	// Assignment operator : abschalten 

					/////////////////////////////////////////////////////////////////////////
					// private member variables
					/////////////////////////////////////////////////////////////////////////

					//---------------------------------------------------------------------------
					// accessor instances ( created in init() )
					//---------------------------------------------------------------------------
					AccessorInstanceRef		m_ACCCustomerInstRef;	//!< instance of accessor , wird als Ref-Klasse automatisch freigegeben
					basar::Int32			m_Initialized;			//!< counter of init-calls


			};
		

		} // namespace customerConsultantDM
	} // namespace libAbbauw
} // namespace domMod

#endif // GUARD_DOMMOD_LIBABBAUW_CUSTOMERCONSULTANTDM_H



