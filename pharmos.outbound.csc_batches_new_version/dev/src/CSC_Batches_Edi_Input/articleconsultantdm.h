//-------------------------------------------------------------------------------------
/*! \file  articleconsultantdm.h
*  \brief  Domain module for database operations regarding articles
*  \brief  Main purpose: get the additional article information to fill the fields of an orderhead/-pos-entry 
*  \author P. Kessling
*  \date   19.04.2013, 
*  history 19.04.13  pk new, similar to batchorderdm.h
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LIBABBAUW_ARTICLECONSULTANTDM_H
#define GUARD_DOMMOD_LIBABBAUW_ARTICLECONSULTANTDM_H

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
		namespace articleConsultantDM
		{

			//-------------------------------------------------------------------------------------------------//
			// class declaration section
			//-------------------------------------------------------------------------------------------------//
			class ArticleConsultantDM 
			{
				//--------------------------------------------------------------------------------------------------//
				// static declaration section
				//--------------------------------------------------------------------------------------------------//
			
				//--------------------------------------------------------------------------------------------------//
				// object declaration section
				//--------------------------------------------------------------------------------------------------//
				public:
						ArticleConsultantDM();									
						~ArticleConsultantDM();

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
						      basar::Int32		findManufacturernoByArticleno(const basar::Int32 articleno);
						const basar::Decimal	findWholesalerpurchasepriceByArticleno(const basar::Int32 articleno);
						const basar::I18nString	findPreferredArticleCodeByArticleEan(const basar::I18nString articleean);
						const basar::I18nString	findPreferredArticleCodeByArticlePznInhCH(const basar::I18nString articlepzn);
						const basar::I18nString	findPreferredArticleCodeByArticlePznInhDE(const basar::I18nString articlepzn);
						const basar::I18nString	findPreferredArticleCodeByArticleMF(const basar::I18nString articlemf);
						      basar::Int32		findArticlenoInternByArticleMF(const basar::I18nString articlemf) ;

						      basar::Int32		findPreferredArticlenoByPreferredArticleCode(const basar::I18nString preferred_article_code);
							  basar::Int16		findCMIArticles(const basar::I18nString articlepzn);


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
						basar::db::aspect::AccessorPropertyTableRef getArticlePropTab();



			private:
					/////////////////////////////////////////////////////////////////////////
					// private member methods
					/////////////////////////////////////////////////////////////////////////

					// special member functions
					ArticleConsultantDM(const ArticleConsultantDM &r);				// Copy-Konstruktor : abschalten
					ArticleConsultantDM &operator = (const ArticleConsultantDM &r);	// Assignment operator : abschalten 

					/////////////////////////////////////////////////////////////////////////
					// private member variables
					/////////////////////////////////////////////////////////////////////////

					//---------------------------------------------------------------------------
					// accessor instances ( created in init() )
					//---------------------------------------------------------------------------
					AccessorInstanceRef		m_ACCArticleInstRef;	//!< instance of accessor SelBatchOrdersAcc, wird als Ref-Klasse automatisch freigegeben
					basar::Int32			m_Initialized;			//!< counter of init-calls


			};
		

		} // namespace articleInfoDM
	} // namespace libAbbauw
} // namespace domMod

#endif // GUARD_DOMMOD_LIBABBAUW_ARTICLECONSULTANTDM_H



