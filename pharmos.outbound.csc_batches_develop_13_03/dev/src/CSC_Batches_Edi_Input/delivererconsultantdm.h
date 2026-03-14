//-------------------------------------------------------------------------------------
/*! \file  delivererconsultantdm.h
*  \brief  Domain module for database operations regarding deliverers
*  \author P. Kessling
*  \date   25.11.14, 
*  history 25.11.14  pk new, similar to customerconsultantdm.h
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LIBABBAUW_DELIVERERCONSULTANTDM_H
#define GUARD_DOMMOD_LIBABBAUW_DELIVERERCONSULTANTDM_H

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
		namespace delivererConsultantDM
		{

			//-------------------------------------------------------------------------------------------------//
			// class declaration section
			//-------------------------------------------------------------------------------------------------//
			class DelivererConsultantDM 
			{
				//--------------------------------------------------------------------------------------------------//
				// static declaration section
				//--------------------------------------------------------------------------------------------------//
			
				//--------------------------------------------------------------------------------------------------//
				// object declaration section
				//--------------------------------------------------------------------------------------------------//
				public:
						DelivererConsultantDM();									
						~DelivererConsultantDM();

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
						basar::Int16				findBranchnoByDelivererbga(const basar::I18nString delivererbga);

						/////////////////////////// SAVES (Update / Insert)//////////////////////////	
						//---------------------------------------------------------------------------

						/////////////////////////// DELETES /////////////////////////////////////////
						//---------------------------------------------------------------------------

						/////////////////////////// CHECKS //////////////////////////////////////////
						//---------------------------------------------------------------------------

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
					DelivererConsultantDM(const DelivererConsultantDM &r);				// Copy-Konstruktor : abschalten
					DelivererConsultantDM &operator = (const DelivererConsultantDM &r);	// Assignment operator : abschalten 

					/////////////////////////////////////////////////////////////////////////
					// private member variables
					/////////////////////////////////////////////////////////////////////////

					//---------------------------------------------------------------------------
					// accessor instances ( created in init() )
					//---------------------------------------------------------------------------
					AccessorInstanceRef		m_ACCDelivererInstRef;	//!< instance of accessor , wird als Ref-Klasse automatisch freigegeben
					basar::Int32			m_Initialized;			//!< counter of init-calls


			};
		

		} // namespace customerConsultantDM
	} // namespace libAbbauw
} // namespace domMod

#endif // GUARD_DOMMOD_LIBABBAUW_DELIVERERCONSULTANTDM_H



