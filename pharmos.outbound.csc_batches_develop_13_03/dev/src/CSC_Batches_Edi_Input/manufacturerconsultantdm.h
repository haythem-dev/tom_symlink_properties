//-------------------------------------------------------------------------------------
/*! \file  manufacturerconsultantdm.h
*  \brief  Domain module for database operations regarding manufacturers
*  \author P. Kessling
*  \date   13.06.2013, 
*  history 13.06.13  pk new, similar to customerconsultantdm.h
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LIBABBAUW_MANUFACTURERCONSULTANTDM_H
#define GUARD_DOMMOD_LIBABBAUW_MANUFACTURERCONSULTANTDM_H

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
		namespace manufacturerConsultantDM
		{

			//-------------------------------------------------------------------------------------------------//
			// class declaration section
			//-------------------------------------------------------------------------------------------------//
			class ManufacturerConsultantDM 
			{
				//--------------------------------------------------------------------------------------------------//
				// static declaration section
				//--------------------------------------------------------------------------------------------------//
			
				//--------------------------------------------------------------------------------------------------//
				// object declaration section
				//--------------------------------------------------------------------------------------------------//
				public:
						ManufacturerConsultantDM();									
						~ManufacturerConsultantDM();

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
						basar::Int32		findManufacturernoByManufacturerean		(const basar::I18nString	manufacturerean);
						basar::I18nString	findManufacturernameByManufacturerno	(const basar::Int32			manufacturerno);

						/////////////////////////// SAVES (Update / Insert)//////////////////////////	
						//---------------------------------------------------------------------------

						/////////////////////////// DELETES /////////////////////////////////////////
						//---------------------------------------------------------------------------

						/////////////////////////// CHECKS //////////////////////////////////////////
						//---------------------------------------------------------------------------

						/////////////////////////// PROPTABS / YITERATORS ///////////////////////////
						//-------------------------------------------------------------------------------------------------//
						//-------------------------------------------------------------------------------------------------//



			private:
					/////////////////////////////////////////////////////////////////////////
					// private member methods
					/////////////////////////////////////////////////////////////////////////

					// special member functions
					ManufacturerConsultantDM(const ManufacturerConsultantDM &r);				// Copy-Konstruktor : abschalten
					ManufacturerConsultantDM &operator = (const ManufacturerConsultantDM &r);	// Assignment operator : abschalten 

					/////////////////////////////////////////////////////////////////////////
					// private member variables
					/////////////////////////////////////////////////////////////////////////

					//---------------------------------------------------------------------------
					// accessor instances ( created in init() )
					//---------------------------------------------------------------------------
					AccessorInstanceRef		m_ACCManufacturerInstRef;	//!< instance of accessor , wird als Ref-Klasse automatisch freigegeben
					basar::Int32			m_Initialized;				//!< counter of init-calls


			};
		

		} // namespace manufacturerConsultantDM
	} // namespace libAbbauw
} // namespace domMod

#endif // GUARD_DOMMOD_LIBABBAUW_MANUFACTURERCONSULTANTDM_H



