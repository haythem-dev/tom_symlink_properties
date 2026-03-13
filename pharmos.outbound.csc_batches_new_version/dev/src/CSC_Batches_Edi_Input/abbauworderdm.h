//-------------------------------------------------------------------------------------
/*! \file  abbauworderdm.h
*  \brief  Domain module for database operations regarding complete Master-detail-data of orders from project ABBA-UW (UW and OM) 
*  \author P. Kessling
*  \date   30.04.2013, 
*  history 30.04.13  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LIBABBAUW_ABBAUWORDERDM_H
#define GUARD_DOMMOD_LIBABBAUW_ABBAUWORDERDM_H

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

#include "manufacturerconsultantdm.h"
#include "customerconsultantdm.h"
#include "delivererconsultantdm.h"
#include "articleconsultantdm.h"

#include "editransferorder.h"

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
		namespace abbauwOrderDM
		{

			//-------------------------------------------------------------------------------------------------//
			// class declaration section
			//-------------------------------------------------------------------------------------------------//
			class AbbauwOrderDM 
			{
				//--------------------------------------------------------------------------------------------------//
				// static declaration section
				//--------------------------------------------------------------------------------------------------//
			
				//--------------------------------------------------------------------------------------------------//
				// object declaration section
				//--------------------------------------------------------------------------------------------------//
				public:
						AbbauwOrderDM();									
						~AbbauwOrderDM();

						// This method inits the module, creates AccessorInstances for the given connection
						// no-throw 
						// Basar-docu, p.40
						void init(const ConnectionRef activeConn, const basar::I18nString inputDir);

						// call this function to clear all accessor instances and the contents of property tabels
						// no-throw */
						// Basar-docu, p.41
						void shutdown();

						//---------------------------------------------------------------------------
						/////////////////////////// FINDS ///////////////////////////////////////////
						//---------------------------------------------------------------------------
						basar::Int32	findBiggestExistingOrdernoByBranchno(const basar::Int16	branchno);
						basar::Int32	findPharmacyno						(const basar::Int16 branchno, const basar::I18nString& pharmacynoean);
						bool			findIsOmgOrder						(const basar::Int16 branchno, const basar::Int32	   pharmacyno);
						bool			isCMIOrder							(const basar::I18nString supplierEAN, const basar::I18nString articlePZN);
						bool			isFromPharmaMall					(const basar::I18nString supplierEAN);

						basar::Int16	findBranchnoInhDE					(const basar::I18nString& delivererBga);
						basar::Int16    getBranchnoByPharmacyno             (const basar::Int16 branchNo, const basar::Int32 pharmacyno);

						//---------------------------------------------------------------------------
						/////////////////////////// SAVES (Update / Insert)//////////////////////////	
						//---------------------------------------------------------------------------
						// Basar-docu, p.41
						bool		saveInsertAbbauwOrder			(      domMod::transferOrderDM::EdiTransferOrder&	eto);
						bool		saveInsertAbbauwOrderInhCH		(      domMod::transferOrderDM::EdiTransferOrder&	eto);
						bool		saveInsertAbbauwOrderInhDE		(      domMod::transferOrderDM::EdiTransferOrder&	eto);
						bool		saveInsertAbbauwOrderPosInhCH	(const domMod::transferOrderDM::PositionSegmentPtr& etoPosPtr, basar::Int16 branchno, basar::Int32 orderno, basar::Int32 posno);
						bool		saveInsertAbbauwOrderPosInhDE	(const domMod::transferOrderDM::PositionSegmentPtr& etoPosPtr, basar::Int16 branchno, basar::Int32 orderno, basar::Int32 posno);

						//---------------------------------------------------------------------------
						/////////////////////////// DELETES /////////////////////////////////////////
						//---------------------------------------------------------------------------

						//---------------------------------------------------------------------------
						/////////////////////////// CHECKS //////////////////////////////////////////
						//---------------------------------------------------------------------------

						//---------------------------------------------------------------------------
						/////////////////////////// OTHER METHODS //////////////////////////////////////////
						//---------------------------------------------------------------------------
						bool		completeInfoFromEdiTransferOrder(domMod::transferOrderDM::EdiTransferOrder&		eto);
						//bool		completeInfoFromEdiPosition		(domMod::transferOrderDM::PositionSegmentPtr&	etoPosPtr);


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



			private:
					/////////////////////////////////////////////////////////////////////////
					// private member methods
					/////////////////////////////////////////////////////////////////////////

					bool		completeInfoFromEdiTransferOrderInhCH	(domMod::transferOrderDM::EdiTransferOrder&		eto);
					bool		completeInfoFromEdiPositionInhCH		(domMod::transferOrderDM::PositionSegmentPtr&	etoPosPtr);
					bool		completeInfoFromEdiTransferOrderInhDE	(domMod::transferOrderDM::EdiTransferOrder&		eto);
					bool		completeInfoFromEdiPositionInhDE		(domMod::transferOrderDM::PositionSegmentPtr&	etoPosPtr);


					// special member functions
					AbbauwOrderDM(const AbbauwOrderDM &r);				// Copy-Konstruktor : abschalten
					AbbauwOrderDM &operator = (const AbbauwOrderDM &r);	// Assignment operator : abschalten 

					/////////////////////////////////////////////////////////////////////////
					// private member variables
					/////////////////////////////////////////////////////////////////////////
					basar::I18nString	specialConversionPbs993NovartisWanderBranchnoean(const basar::I18nString branchnoean);
					basar::Int32		specialConversionPbs993Nadmf(const basar::I18nString manufacturernoean);

					//---------------------------------------------------------------------------
					// accessor instances ( created in init() )
					//---------------------------------------------------------------------------
					AccessorInstanceRef		m_ACCInstOrderheadRef;	//!< instance of accessor ACC_ORDERHEAD, wird als Ref-Klasse automatisch freigegeben
					AccessorInstanceRef		m_ACCInstOrderposRef;	//!< instance of accessor ACC_ORDERPOS, wird als Ref-Klasse automatisch freigegeben
					basar::Int32			m_Initialized;			//!< counter of init-calls
					basar::I18nString		m_InputDir;				//!< directory for input files, set in init()

					domMod::libAbbauw::manufacturerConsultantDM::ManufacturerConsultantDM	m_manufacturerConsultantDM;	// initialized Instance of CustomerConsultantDM
					domMod::libAbbauw::customerConsultantDM::CustomerConsultantDM			m_customerConsultantDM;		// initialized Instance of CustomerConsultantDM
					domMod::libAbbauw::delivererConsultantDM::DelivererConsultantDM			m_delivererConsultantDM;	// initialized Instance of DelivererConsultantDM
					domMod::libAbbauw::articleConsultantDM::ArticleConsultantDM				m_articleConsultantDM;		// initialized Instance of ArticleConsultantDM

			};
		

		} // namespace abbauwOrderDM
	} // namespace libAbbauw
} // namespace domMod

#endif // GUARD_DOMMOD_LIBABBAUW_ABBAUWORDERDM_H



