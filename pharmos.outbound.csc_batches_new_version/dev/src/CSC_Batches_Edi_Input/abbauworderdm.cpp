//-------------------------------------------------------------------------------------
/*! \file  abbauworderdm.cpp
*  \brief  Domain module for database operations regarding complete Master-detail-data of orders from project ABBA-UW (UW and OM) 
*  \author P. Kessling
*  \date   30.04.2013, 
*  history 30.04.13  pk new
*          26.06.14  pk put new field pharmacynomf to DB. 
*                    Contains originally sent pharmacyEN from manufacturer.
*/
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <string>
#include <sstream>		// for stringstream
#include <iomanip>		// for stringstream
#include <exception>	// for exceptions
#include <classinfo_macros.h>	// for BLOG_TRACE_METHOD-Makros
//#include <applicationlog.h>		// for BLOG_TRACE_METHOD-Makros and Logger


#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers

#include <libbasar_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasarappl_systemeventmanager.h>
#include "loggerpool.h"
#include <libbasarlogin.h>
#include <libbasardbaspect.h>

#include "abbauwexception.h"
#include "dataacquisitionexception.h"
#include "datastoringexception.h"

#include "edi_input_properties_definitions.h"
#include "edi_input_accessor_definitions.h"
#include "edi_input_definitions.h"
#include "articleconsultantdm.h"
#include "customerconsultantdm.h"
#include "delivererconsultantdm.h"
#include "manufacturerconsultantdm.h"
#include "editransferorder.h"
//#include "pcdline.h"
#include "abbauworderdm.h"

#include "alc_05_line.h"

#ifdef WIN32
#pragma warning (push)
#pragma warning (disable : 4800) 
#endif

//-------------------------------------------------------------------------------------------------//
// using section
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::cmnutil::ParameterList;
using basar::db::aspect::ExecuteResultInfo;
//-------------------------------------------------------------------------------------------------//
// namespace definition section
//-------------------------------------------------------------------------------------------------//

namespace domMod
{
	namespace libAbbauw
	{
		namespace abbauwOrderDM
		{
			//--------------------------------------------------------------------------------------------------//
			// static definition section
			//--------------------------------------------------------------------------------------------------//
			//--------------------------------------------------------------------------------------------------//
			// object definition section
			//--------------------------------------------------------------------------------------------------//

			//**************************************************************************************************
			//         Konstruktoren / Destruktoren
			//**************************************************************************************************
			AbbauwOrderDM::AbbauwOrderDM() : m_Initialized(0) 
			{
				METHODNAME_DEF( AbbauwOrderDM, AbbauwOrderDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}

			AbbauwOrderDM::~AbbauwOrderDM() 
			{
				METHODNAME_DEF( AbbauwOrderDM, ~AbbauwOrderDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}


			//**************************************************************************************************
			//         Methoden
			//**************************************************************************************************
			//*************************************** init *****************************************************
			void AbbauwOrderDM::init(const ConnectionRef activeConn, const basar::I18nString inputDir)
			{
				METHODNAME_DEF( AbbauwOrderDM, init )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try
				{	
					m_InputDir = inputDir;
					if (0 == m_Initialized)
					{
						// Watch out: Proptab-methods like empty(), size() dont work when caching != FULL_CACHING 
						m_ACCInstOrderheadRef = basar::db::aspect::Manager::getInstance().createAccessorInstance(libabbauw::domMod::ACC_INST_ORDERHEAD, libabbauw::domMod::ACC_ORDERHEAD, activeConn, basar::db::aspect::FULL_CACHING);  

						m_ACCInstOrderposRef = basar::db::aspect::Manager::getInstance().createAccessorInstance(libabbauw::domMod::ACC_INST_ORDERPOS, libabbauw::domMod::ACC_ORDERPOS, activeConn, basar::db::aspect::FULL_CACHING);  

						m_manufacturerConsultantDM.init(activeConn);

						m_customerConsultantDM.init(activeConn);

						m_delivererConsultantDM.init(activeConn);

						m_articleConsultantDM.init(activeConn);
					}

					++m_Initialized;
				}// try
				catch(...)
				{
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
					throw; 
				}// catch
			}// init

			//*************************************** shutdown *****************************************************
			void AbbauwOrderDM::shutdown() 
			{
				METHODNAME_DEF( AbbauwOrderDM, shutdown )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try {		
					// release accessor instance if instance was created WITH storing (default: without) in instance list
					// basar::db::aspect::Manager::getInstance().releaseAccessorInstance("AccInstance");

					if(0 == m_Initialized)
					{
						return;
					}			
					--m_Initialized;

					if(0==m_Initialized) {

						if(!m_ACCInstOrderheadRef.getPropertyTable().isNull())
						{
							m_ACCInstOrderheadRef.getPropertyTable().clear();
						}
						m_ACCInstOrderheadRef.reset();

						if(!m_ACCInstOrderposRef.getPropertyTable().isNull())
						{
							m_ACCInstOrderposRef.getPropertyTable().clear();
						}
						m_ACCInstOrderposRef.reset();


						m_manufacturerConsultantDM.shutdown();
						m_customerConsultantDM.shutdown();
						m_delivererConsultantDM.shutdown();
						m_articleConsultantDM.shutdown();

					}// if(0==m_Initialized)

				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
					throw;
				}

			}// shutdown


			//*************************************** findBiggestExistingOrdernoByBranchno *****************************************************
			basar::Int32	AbbauwOrderDM::findBiggestExistingOrdernoByBranchno(const basar::Int16	parBranchno)
			{
				METHODNAME_DEF( AbbauwOrderDM, findBiggestExistingOrdernoByBranchno )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


				try {		
					// insert a first line with status 
					// and create an iterator for the orderhead data (only one line in the property table)
					m_ACCInstOrderheadRef.getPropertyTable().clear();					
					m_ACCInstOrderheadRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		

					AccessorPropertyTable_YIterator yitSearch = m_ACCInstOrderheadRef.getPropertyTable().begin();
					yitSearch.setInt16(libabbauw::properties::BRANCHNO.getName(), parBranchno);

					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCInstOrderheadRef.execute(libabbauw::domMod::ACCMETH_SELECT_MAX_ORDERNO_BY_BRANCHNO, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun << "execute() failed" << std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					const basar::Int32 ret = m_ACCInstOrderheadRef.getPropertyTable().begin().getInt32(libabbauw::properties::ORDERNO.getName());
					return ret;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch



			} //findBiggestExistingOrdernoByBranchno



			//*************************************** findPharmacyno *****************************************************
			basar::Int32	AbbauwOrderDM::findPharmacyno(const basar::Int16 branchno, const basar::I18nString& pharmacynoean)
			{
				METHODNAME_DEF( AbbauwOrderDM, findPharmacyno )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


				try {
					const basar::Int32 pharmacyno 
						= m_customerConsultantDM.findPharmacynoByBranchnoPharmacynoean(branchno, pharmacynoean);

					return pharmacyno;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch



			} //findPharmacyno


			//*************************************** findIsOmgOrder *****************************************************
			bool AbbauwOrderDM::findIsOmgOrder(const basar::Int16 branchno, const basar::Int32	pharmacyno)
			{
				METHODNAME_DEF( AbbauwOrderDM, findIsOmgOrder )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


				try {
					
					const basar::I18nString cstgroup 
						= m_customerConsultantDM.findCustomerGroupByBranchnoPharmacyno(branchno, pharmacyno);

					if (libabbauw::domMod::lit::OMG_CUSTOMERGROUP == cstgroup)		{
						return true;
					}
					else {
						return false;
					}

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch



			} //findIsOmgOrder

			bool AbbauwOrderDM::isCMIOrder(const basar::I18nString supplierEAN, const basar::I18nString articlePZN)
			{
				const bool isFirstArticleCMI = m_articleConsultantDM.findCMIArticles(articlePZN);
				if (isFromPharmaMall(supplierEAN) && isFirstArticleCMI)
				{
					return true;
				}
				
				return false;
			}

			bool AbbauwOrderDM::isFromPharmaMall(const basar::I18nString supplierEAN)
			{
				bool isFromPharmaMall = false;

				if (supplierEAN == domMod::line::BFA_ASTELLAS ||
					supplierEAN == domMod::line::BFA_MSD)
				{
					isFromPharmaMall = true;
				}

				return isFromPharmaMall;
			}

			//*************************************** specialConversionPbs993NovartisWander *****************************************************
			// NovartisWander sends its own branchno-EAN-strings. 
			// If these EANS are detected: Convert these strings to the usual EAN strings.
			// Otherwise: Do nothing
			basar::I18nString AbbauwOrderDM::specialConversionPbs993NovartisWanderBranchnoean(const basar::I18nString branchnoean)
			{
				METHODNAME_DEF( AbbauwOrderDM, specialConversionPbs993NovartisWanderBranchnoean )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::I18nString retstr = branchnoean;

				// convert like PBS993 - special case Novartis/Wander: see specification chapter 5.5
				if (	libabbauw::domMod::lit::EAN_BRANCHNO_AMEDIS_UNTERENTFELDEN_SPECIAL == branchnoean 
					||	libabbauw::domMod::lit::EAN_BRANCHNO_AMEDIS_BRANCH14_SPECIAL == branchnoean )		// former branch 14 was integrated into branch 12
				{
					retstr = libabbauw::domMod::lit::EAN_BRANCHNO_AMEDIS_UNTERENTFELDEN;
				}
				else if (	libabbauw::domMod::lit::EAN_BRANCHNO_AMEDIS_PUIDOUX_SPECIAL == branchnoean )	
				{
					retstr = libabbauw::domMod::lit::EAN_BRANCHNO_AMEDIS_PUIDOUX;
				}
				// end convert .. 993

				return retstr;
			} //specialConversionPbs993NovartisWanderBranchnoean

			
			//*************************************** specialConversionPbs993NovartisWander *****************************************************
			// Some manufacturers send their own manufacturerno-EAN-strings. 
			// If these EANS are detected: Convert these strings directly to the manufacturerno and return it.
			// Otherwise: return 0
			basar::Int32 AbbauwOrderDM::specialConversionPbs993Nadmf(const basar::I18nString manufacturernoean)
			{
				METHODNAME_DEF( AbbauwOrderDM, specialConversionPbs993Nadmf )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::Int32 ret = 0;

				// convert like PBS993 - special case 2 NADMF: see specification chapter 5.5
				if (	manufacturernoean == libabbauw::domMod::lit::EAN_MANUFACTURERNO_SPECIAL_SSLHEALTHCARE)	
				{
					ret = libabbauw::domMod::lit::MANUFACTURERNO_SPECIAL_SSLHEALTHCARE;
				}
				else if ( manufacturernoean == libabbauw::domMod::lit::EAN_MANUFACTURERNO_SPECIAL_3MPROFESSIONAL)	
				{
					ret = libabbauw::domMod::lit::MANUFACTURERNO_SPECIAL_3MPROFESSIONAL;
				}
				else if ( manufacturernoean == libabbauw::domMod::lit::EAN_MANUFACTURERNO_SPECIAL_3MCONSUMERCARE)	
				{
					ret = libabbauw::domMod::lit::MANUFACTURERNO_SPECIAL_3MCONSUMERCARE;
				}
				else if ( manufacturernoean == libabbauw::domMod::lit::EAN_MANUFACTURERNO_SPECIAL_SANOFISYNTHLABO)	
				{
					ret = libabbauw::domMod::lit::MANUFACTURERNO_SPECIAL_SANOFISYNTHLABO;
				}
				else if ( manufacturernoean == libabbauw::domMod::lit::EAN_MANUFACTURERNO_SPECIAL_VIFORSAFRIBOURG)	
				{
					ret = libabbauw::domMod::lit::MANUFACTURERNO_SPECIAL_VIFORSAFRIBOURG;
				}
				else if ( manufacturernoean == libabbauw::domMod::lit::EAN_MANUFACTURERNO_SPECIAL_PHARMASINGER)	
				{
					ret = libabbauw::domMod::lit::MANUFACTURERNO_SPECIAL_PHARMASINGER;
				}
				else if ( manufacturernoean == libabbauw::domMod::lit::EAN_MANUFACTURERNO_SPECIAL_ZAMBONSCWEIZAG)	
				{
					ret = libabbauw::domMod::lit::MANUFACTURERNO_SPECIAL_ZAMBONSCWEIZAG;
				}
				else if ( manufacturernoean == libabbauw::domMod::lit::EAN_MANUFACTURERNO_SPECIAL_INSTITUTBIOCHIMIQUE)	
				{
					ret = libabbauw::domMod::lit::MANUFACTURERNO_SPECIAL_INSTITUTBIOCHIMIQUE;
				}
				// end convert .. 993

				return ret;
			} //specialConversionPbs993Nadmf

			//************************************** Find-Methodes for DE ********************************************
			
			
			//*************************************** findBranchnoInhDE *****************************************************
			basar::Int16	AbbauwOrderDM::findBranchnoInhDE	(const basar::I18nString& delivererbga)
			{
				METHODNAME_DEF( AbbauwOrderDM, findBranchnoInhDE )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


				try {
					const basar::Int16 branchno 
						= m_delivererConsultantDM.findBranchnoByDelivererbga(delivererbga);

					return branchno;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch

			} //findBranchnoInhDE

		


			

			//*************************************** completeInfo *****************************************************
			// Precondition: eto must be set 
			bool	AbbauwOrderDM::completeInfoFromEdiTransferOrder(domMod::transferOrderDM::EdiTransferOrder& eto)
			{
				METHODNAME_DEF( AbbauwOrderDM, completeInfoFromEdiTransferOrder )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
				
				try
				{
					// for CH
					if (	domMod::orderMapper::enInputtype_inhCH	== eto.getInputtype())
					{
						bool ret = completeInfoFromEdiTransferOrderInhCH (eto);
						return ret;
					}
					// for DE
					else if (	domMod::orderMapper::enInputtype_inhDE	== eto.getInputtype()	)
					{
						bool ret = completeInfoFromEdiTransferOrderInhDE (eto);
						return ret;
					}
					else
					{
						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
						std::stringstream str;
						str << "Unknown Inhouse format type: " << eto.getInputtype() << std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}
				}
				catch(...) {
					// Throw same exception 
					throw;
				}// catch

			}


			//*******************************************************************************************************
			//*****************************  Complete for Inhouse CH ************************************************
			//*******************************************************************************************************

			bool	AbbauwOrderDM::completeInfoFromEdiTransferOrderInhCH(domMod::transferOrderDM::EdiTransferOrder& eto)
			{
				METHODNAME_DEF( AbbauwOrderDM, completeInfoFromEdiTransferOrderInhCH )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try {

					// At first complete ORDERHEAD-data
					
					// BRANCHNO
					// get branchno: Convert the EAN-number for the branch into the branchno.
					// This is coded just for Switzerland (CH) with fixed numbers.
					// Better: Later on find a solution with EAN/branchno-Codetable. Doesn't exist yet.

					basar::I18nString nadlineBranchEan("");
					basar::Int16 branchno(0);

					if (eto.getNadLine_BranchPtr())
					{
						nadlineBranchEan = eto.getNadLine_BranchPtr()->getIdentEan();	
					}

					// convert like PBS993 - special case Novartis/Wander: see specification chapter 5.5
					nadlineBranchEan = specialConversionPbs993NovartisWanderBranchnoean (nadlineBranchEan);

					if ( libabbauw::domMod::lit::EAN_BRANCHNO_AMEDIS_UNTERENTFELDEN == nadlineBranchEan )
					{
						eto.setBranchno(libabbauw::domMod::lit::BRANCHNO_AMEDIS_UNTERENTFELDEN);
						branchno = libabbauw::domMod::lit::BRANCHNO_AMEDIS_UNTERENTFELDEN;
					}
					else if ( libabbauw::domMod::lit::EAN_BRANCHNO_AMEDIS_PUIDOUX == nadlineBranchEan )
					{
						eto.setBranchno(libabbauw::domMod::lit::BRANCHNO_AMEDIS_PUIDOUX);	
						branchno = libabbauw::domMod::lit::BRANCHNO_AMEDIS_PUIDOUX;
					}
					else
					{
						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
						std::stringstream str;
						str << "Unknown PHOENIX branchno (PERMITTED ONLY FOR BRANCHNOS FROM CH!: " << nadlineBranchEan << std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					// ORDERNO
					// generate next available orderno
					// kes 10.02.15: Orderno is calculated by database now (type serial)
					// orig beginn
					//basar::Int32 ord = findBiggestExistingOrdernoByBranchno(eto.getBranchno());
					//++ord;	
					//eto.setOrdernoBatch(ord);
					// orig end
					eto.setOrdernoBatch(0);			// Set 0, real orderno is created on INSERT in serial-field by database

					// STATUS
					if (!eto.getPositionSegments().empty() )
					{
						eto.setStatus(libabbauw::domMod::lit::STATUS_DEFERRED);			// Status "0" - "ZU" 
					}
					else
					{
						// Integrity of data (Plausibilität No 2, specification chapter 5.6) [UC_INP REQ 3.1]: 
						// An order needs at least one position. 
						// If there is none, the order will be written to orderhead, 
						// but its status will be set to "ST"-cancelled/storniert! 
						eto.setStatus(libabbauw::domMod::lit::STATUS_CANCELLED);		// Status "9" - "ST" 
					}

					// PHARMACYNO
					basar::Int32 pharmacyno = 0;
					if( eto.getNadLine_PharmacyPtr() )
					{
						pharmacyno = findPharmacyno(eto.getBranchno(), eto.getNadLine_PharmacyPtr()->getIdentEan());		

						if (0 >= pharmacyno )
						{
							// log: Cannot recognize customer-pharmacy!!
							std::stringstream str;
							str << "Cannot generate pharmacyno (=idfnr) from EAN in NADline-BY: <"
								<< eto.getNadLine_PharmacyPtr()->getIdentEan()  
								<< ">!"
								<< std::endl;
							BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
							BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						}
					}
					else
					{
						// log: An order without a customer-pharmacy!!
						std::stringstream str;
						str << "UW or OM order does not contain a pharmacyno (=customer-idfnr, no NAD-lineBY) " 
							<< std::endl;
						BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
						BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					}

					if (pharmacyno <=0)
					{
						// convert like PBS993 - special case 3 (NADBY): see ABBA-UW specification chapter 5.5:
						// set to pseudo-IDFNR for unknown pharmacy		
						
						// First solution, now deprecated: Set to fix pseudo-Idfnr
						// set to pseudo-IDFNR for unknown pharmacy
						// orig: pharmacyno = libabbauw::domMod::lit::PHARMACYNO_SPECIAL_PSEUDO;

						// kes 08.01.15: Pseudo-Idfnr is no longer fixed: Obtain it from table Outbound-DB/kundeschalter:
						//               SELECT   FIRST  1   k.idfnr AS pharmacyno FROM   kundeschalter k  WHERE  k.vertriebszentrumnr = #branchno#   AND  k.skdpseudoidfnr_uw	= 1;               

						basar::Int32 pseudoPharmacyno 
							= m_customerConsultantDM.findPseudopharmacynoByBranchno(branchno);

						if (  0 < pseudoPharmacyno )
						{
							// This should be the unique manufacturerno for the whole order
							// We utilize this. If the transfer order contains articles of different manufacturers - it is his problem, we cannot solve it.
							pharmacyno= pseudoPharmacyno;
						}
						else
						{
							// log: There exists no pseudopharmacy number for this branchno!!
							std::stringstream str;
							str << "Cannot find a pseudo-pharmacyno for branchno= " 
								<< branchno
								<< " Definition in kundeschalter missing?"
								<< std::endl;
							BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
							BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

							pharmacyno= 0;   // DE: PBS999 didnt convert to a pseudo idf-number	
						}


						// just log, no error: An order without a customer-pharmacy!!
						std::stringstream str;
						str << "UW order for branch "
							<< branchno
							<< " does not contain a valid pharmacyno. Set pseudo-Idfnr = "
							<< pseudoPharmacyno
							<< std::endl;
						BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
						BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					}

					eto.setPharmacyno(pharmacyno);


					// HEADERTEXT 
					basar::I18nString invoicetext("");

					if (0 < pharmacyno )
					{
						// Regular invoicetext for orders from EDI-flatfile
						invoicetext = invoicetext + libabbauw::domMod::lit::INVOICE_TEXT_UW;
						if( eto.getNadLine_ManufacturerPtr() )
						{
							invoicetext = invoicetext + eto.getNadLine_ManufacturerPtr()->getText();
						}
						else
						{
							invoicetext = invoicetext + libabbauw::domMod::lit::INVOICE_TEXT_UNKNOWN;
						}
					}
					else
					{
						// Customer not known: Write that on invoice
						invoicetext = invoicetext + libabbauw::domMod::lit::INVOICE_TEXT_CUST_UNKNOWN;
						if( eto.getNadLine_PharmacyPtr() )
						{
							invoicetext = invoicetext + eto.getNadLine_PharmacyPtr()->getText();
						}
						else
						{
							invoicetext = invoicetext + libabbauw::domMod::lit::INVOICE_TEXT_UNKNOWN;
						}

					}
					// invoicetext mustn`t be longer than 28 char
					// 13.02.15 kes: 30 char should be ok also. If we write more than 30 chars: kscclient crashes with an unknown exception on order release 
					eto.setHeadertext(invoicetext.substr(0,30 /*28*/));			// Text for invoice "*UEWAUF FROM <NADBY>" or "UNKNOWN EAN <NADBY>"


					// ORDERTYPE, ORDERPICKINGTYPE, BOOKENTRYTYPE 
					// decide whether its an OMG-Order (= OMG when customer (=pharmacyno) is member of customergroup =18) 
					const bool isOmgOrder = findIsOmgOrder(eto.getBranchno(), eto.getPharmacyno());

					if (isOmgOrder)
					{
						// It is an OM22
						eto.setOrdertype(libabbauw::domMod::lit::OMG_ORDERTYPE);			// OMG-order "OM22" 
						eto.setOrderpickingtype(libabbauw::domMod::lit::OMG_ORDERPICKINGTYPE);			
						eto.setBookentrytype(libabbauw::domMod::lit::OMG_BOOKENTRYTYPE);
					}
					else
					{
						// It is an UW00
						eto.setOrdertype(libabbauw::domMod::lit::UW_ORDERTYPE);			// typical "Ueberweiser" = transfer order "UW00"
						eto.setOrderpickingtype(libabbauw::domMod::lit::UW_ORDERPICKINGTYPE);
						eto.setBookentrytype(libabbauw::domMod::lit::UW_BOOKENTRYTYPE);
					}

					// ORDERVALUE: For now we do not set it, because it is redundant. Perhaps later for performance reasons

					// Now complete the positions data
					if ( !eto.getPositionSegments().empty())
					{
						basar::Int32 cntElems = static_cast<basar::Int32>(eto.getPositionSegments().size());

						for (basar::Int32 index = 0; index < cntElems ;   ++index )
						{
							if ( !completeInfoFromEdiPositionInhCH( (eto.getPositionSegments()).at(index)) )
							{
								BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
								std::stringstream str;
								str << "Could not complete Info for position number " << index << std::endl;
								throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ));
							}
						}

					}


					// ORDERNOMF: Specification, chapter 4.2 describes how to convert the NADMF to Herstellerno
					//            Some manufacturer EANs have to be converted by a special method,  
					//            that was formerly implemented in PBS993 (see specification chapter 5.5)
					//            If this is not possible: Acquire data from DB. There exist two ways: Try the first one. 
					//            If it doesnt work: Try the second one.
					//            (For both: See specification chapter 4.2)

					basar::Int32		manufacturerno = -1;
					basar::I18nString	given_manufacturernoean("");

					if (eto.getNadLine_ManufacturerPtr())
					{
						given_manufacturernoean	= eto.getNadLine_ManufacturerPtr()->getIdentEan();
					}

					// try to convert like PBS993 - special case no. 2 NADMF: see specification chapter 5.5
					manufacturerno = specialConversionPbs993Nadmf (given_manufacturernoean);
					// end convert like PBS993 - special case no. 2 NADMF

					if ( 0 < manufacturerno )
					{
						// manufacturerno was converted directly by special PBS993 case
						eto.setManufacturerno(manufacturerno);
					}
					else
					{
						// manufacturerno has to be acquired from DB

						// Primary method: spec. chapter 4.2: 
						// Get manufacturerno from a) bliefer->ean1		=> bliefer->lant_nr
						//                         b) liefer->lant_nr   => liefer->hersteller_nr
						
						manufacturerno = m_manufacturerConsultantDM.findManufacturernoByManufacturerean(given_manufacturernoean);
						
						if ( 0 < manufacturerno )
						{
							// manufacturerno was successfully acquired by first method
							eto.setManufacturerno(manufacturerno);
						}
						else
						{
							// try secondary method: Get manufacturerno from the article in the first position if there is one
							if ( !eto.getPositionSegments().empty())		// this order got positions
							{
								basar::Int32 cntElems								= static_cast<basar::Int32>(eto.getPositionSegments().size());
								basar::Int32 firstManufacturerno					= -1;

								for (basar::Int32 index = 0; index < cntElems ;   ++index )
								{
									basar::Int32 currManufacturerno 
										= m_articleConsultantDM.findManufacturernoByArticleno((eto.getPositionSegments()).at(index)->getArticleno());

									if (  -1 == firstManufacturerno )
									{
										firstManufacturerno = currManufacturerno;

										// This should be the unique manufacturerno for the whole order
										eto.setManufacturerno(firstManufacturerno);

									}
									else if (firstManufacturerno != currManufacturerno )
									{
										// Integrity of data (Plausibilität No 1, specification chapter 5.6) [UC_INP REQ 3.1]: 
										// All articles should be delivered by the same manufacturer. If not: Accept, but Log-entry!

										std::stringstream str;
										str << "UW or OM order contains positions of different manufacturers: " 
											<< firstManufacturerno	<< " and " 
											<< currManufacturerno	<< std::endl;
										BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
										BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
									}
								}
							}
						}
					}

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch

				return true;
			}

			//**************************************************************************************************************************
			//*****************************************   completeInfoFromEdiPosition   *********************************************
			//**************************************************************************************************************************

			
			//*************************************** completeInfoFromEdiPositionInhCH *****************************************************
			// Precondition: eto must be set and validated.
			bool	AbbauwOrderDM::completeInfoFromEdiPositionInhCH(	  domMod::transferOrderDM::PositionSegmentPtr& etoPosPtr)
			{
				METHODNAME_DEF( AbbauwOrderDM, completeInfoFromEdiPositionInhCH )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try {

					// ARTICLENO and ARTICLECODE
					if (etoPosPtr->getLinLine_LinLinePtr())
					{
						basar::I18nString	given_article_code			= etoPosPtr->getLinLine_LinLinePtr()->getArticleno();
						basar::I18nString	preferred_article_code("");
						basar::Int32		preferred_articleno(0);

						// conversion to phoenix articleno depends on the code_type, that was sent by the manufacturer
						if ( etoPosPtr->getLinLine_LinLinePtr()->isEan() )
						{
							preferred_article_code	= m_articleConsultantDM.findPreferredArticleCodeByArticleEan(given_article_code);
						}
						else if ( etoPosPtr->getLinLine_LinLinePtr()->isManufacturerDefined() )
						{
							preferred_article_code	= m_articleConsultantDM.findPreferredArticleCodeByArticleMF(given_article_code);
						}
						else if ( etoPosPtr->getLinLine_LinLinePtr()->isPzn() )
						{
							preferred_article_code	= m_articleConsultantDM.findPreferredArticleCodeByArticlePznInhCH(given_article_code);
						}
						else	// cannot be converted
						{
							// specification ABBA-UW chapter 4.4: 
							// set field orderpos/articleno to 0  
							// and later ..: set orderpos/textfield with a text: 
							// <EanNr-Originalnummer>/<Articlename from LinLine (as much chars as possible) >
 
							preferred_article_code = "";
						}

						etoPosPtr->setArticleCode(preferred_article_code);		

                        preferred_articleno		= m_articleConsultantDM.findPreferredArticlenoByPreferredArticleCode(preferred_article_code);
						etoPosPtr->setArticleno(preferred_articleno);		
					}
					else
					{
						// specification ABBA-UW chapter 4.4: 
						// set field orderpos/articleno to 0  
						// Log an error and put 0-values into fields
						basar::VarString msg;
						msg.format( "Found position without LIN-line !" );
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						etoPosPtr->setArticleCode("");		
						etoPosPtr->setArticleno(0);		
					}

					// TEXTFIELD: see specification chapter 4.4: entry of EDI-articledata for unknown articles
					if (0 >= etoPosPtr->getArticleno())
					{
						// specification ABBA-UW chapter 4.4: 
						// set orderpos/textfield with a text: 
						// <EanNr-Originalnummer>/<Articlename from LinLine (as much chars as possible) >

						basar::I18nString artnoEdi("");
						basar::I18nString artimdtextEdi("");
						if (etoPosPtr->getLinLine_LinLinePtr())
						{
							artnoEdi		= etoPosPtr->getLinLine_LinLinePtr()->getArticleno();
						}
						if (etoPosPtr->getImdLine_ImdLinePtr())
						{
							artimdtextEdi	= etoPosPtr->getImdLine_ImdLinePtr()->getArticleDescMF();
						}

						basar::I18nString textfield;
						textfield = "EDI Artno:" + artnoEdi + "/" + artimdtextEdi;


						etoPosPtr->setTextfield(textfield);
					}


					// WHOLESALERPURCHASEPRICE
					basar::Decimal whopp  = m_articleConsultantDM.findWholesalerpurchasepriceByArticleno(etoPosPtr->getArticleno());
					etoPosPtr->setWholesalerpurchaseprice(whopp);


					//DISCOUNTPCT with its base (=increase / decrease)
					basar::cmnutil::Decimal	discountPct(0);
					
					if (etoPosPtr->getPcdLine_PcdLinePtr())
					{
						discountPct		= etoPosPtr->getPcdLine_PcdLinePtr()->getPercentValue();

						// kes 07.01.15: Bugfix: On Decrease>100%: Set to 0%. On Increase >100%: Do nothing.
						// ToDo: Has to be tested for CH!!
						if ( etoPosPtr->getPcdLine_PcdLinePtr()->isPriceDecrease() ) {

							discountPct *= -1;						
							
							// price reduction: typical: value < 0							
							

							// Integrity of data (Plausibilität No 3, specification chapter 5.6) [UC_INP REQ 3.1]: 
							// The maximum permitted price reduction discount percentage is 100%! 
							basar::cmnutil::Decimal maxDiscount(-100);
							if ( maxDiscount > discountPct )
							{
								// Set discount > 100% to zero. Write error log, but continue processing.
								std::stringstream str;
								basar::Int16 right = discountPct.getAllDecimalPlaces();

								str << "Error: PCD-line with discountPct > 100 found: <"
									<< discountPct.toNonLocString(right) 
									<< ">!"
									<< std::endl;
								BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

								// orig. was: discountPct = maxDiscount;  Now: Set to zero discount.
								discountPct.fromNonLocString("0");
							}

						}
					}
					etoPosPtr->setDiscountPct(discountPct);

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch


				return true;
			}


						
			//*************************************** completeInfoFromEdiPositionInhDE *****************************************************
			// Precondition: eto must be set and validated.
			bool	AbbauwOrderDM::completeInfoFromEdiPositionInhDE(	  domMod::transferOrderDM::PositionSegmentPtr& etoPosPtr)
			{
				METHODNAME_DEF( AbbauwOrderDM, completeInfoFromEdiPositionInhDE )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try {

					// ARTICLENO and ARTICLECODE
					if (etoPosPtr->getLin_08_Line_Lin_08_LinePtr())
					{
						basar::I18nString	given_article_code			= etoPosPtr->getLin_08_Line_Lin_08_LinePtr()->getArticleno();
						basar::I18nString	preferred_article_code("");
						basar::Int32		preferred_articleno(0);

						// conversion to phoenix articleno depends on the code_type, that was sent by the manufacture
						if ( etoPosPtr->getLin_08_Line_Lin_08_LinePtr()->isEan() )
						{
							preferred_article_code	= m_articleConsultantDM.findPreferredArticleCodeByArticleEan(given_article_code);
						}
						// ToDo: Check: There are no manufacturer defined codes in DE (?) 
						//else if ( etoPosPtr->getLin_08_Line_Lin_08_LinePtr()->isManufacturerDefined() )
						//{
						//	preferred_article_code	= m_articleConsultantDM.findPreferredArticleCodeByArticleMF(given_article_code);
						//}
						else if ( etoPosPtr->getLin_08_Line_Lin_08_LinePtr()->isPzn() )
						{
							preferred_article_code	= m_articleConsultantDM.findPreferredArticleCodeByArticlePznInhDE(given_article_code);
						}
						else	// cannot be converted
						{
							// specification ABBA-UW chapter 4.4: 
							// set field orderpos/articleno to 0  
							// and later ..: set orderpos/textfield with a text: 
							// <EanNr-Originalnummer>/<Articlename from LinLine (as much chars as possible) >
 
							preferred_article_code = "";
						}

						etoPosPtr->setArticleCode(preferred_article_code);		

                        preferred_articleno		= m_articleConsultantDM.findPreferredArticlenoByPreferredArticleCode(preferred_article_code);
						etoPosPtr->setArticleno(preferred_articleno);		
					}
					else
					{
						// specification ABBA-UW chapter 4.4: 
						// set field orderpos/articleno to 0  
						// Log an error and put 0-values into fields
						basar::VarString msg;
						msg.format( "Found position without LIN_08-line !" );
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						etoPosPtr->setArticleCode("");		
						etoPosPtr->setArticleno(0);		
					}



					// TEXTFIELD: see specification chapter 4.4: entry of EDI-articledata for unknown articles
					basar::I18nString positiontext("");

					if (0 >= etoPosPtr->getArticleno())
					{	

						// specification ABBA-UW chapter 4.4: 
						// set orderpos/textfield with a text, if there is enough place left i textfield: 
						// <EdiFact-Originalnummer>/<Articlename from LinLine (as much chars as possible) >

						basar::I18nString artnoEdi("");

						if (etoPosPtr->getLin_08_Line_Lin_08_LinePtr())
						{
							artnoEdi		= etoPosPtr->getLin_08_Line_Lin_08_LinePtr()->getArticleno();
						}

						positiontext = positiontext + "EDI Artno:" + artnoEdi;
					}

					if ( etoPosPtr->getFtx_11_Line_PosPtr())
					{
						// Regular positiontext for orders from EDI-flatfile
						// In DE: Should be content of FTX_11-line with Zucode="AAA"
						positiontext = positiontext + " "+ etoPosPtr->getFtx_11_Line_PosPtr()->getFreeText();
					}

					if ( positiontext.length() > 0)
					{
						// Text mustn`t be longer than 29 char
						etoPosPtr->setTextfield(positiontext.substr(0,29));			// Text for invoice: textfield
					}



					// WHOLESALERPURCHASEPRICE
					basar::Decimal whopp  = m_articleConsultantDM.findWholesalerpurchasepriceByArticleno(etoPosPtr->getArticleno());
					etoPosPtr->setWholesalerpurchaseprice(whopp);



					//DISCOUNTPCT with its base (=increase / decrease)
					basar::cmnutil::Decimal	discountPct(0);
					
					if (etoPosPtr->getPcd_06_Line_Pcd_06_LinePtr())
					{
						discountPct		= etoPosPtr->getPcd_06_Line_Pcd_06_LinePtr()->getPercentValue();

						// for Debug reasons / breakpoints
						basar::Int16 right = discountPct.getAllDecimalPlaces();
						basar::I18nString dbgstring = discountPct.toNonLocString(right);
						// end Debug reasons

						if ( etoPosPtr->getPcd_06_Line_Pcd_06_LinePtr()->isPriceDecrease() ) {

							discountPct *= -1;						// price reduction: typical: value < 0

							// Integrity of data (Plausibilität No 3, specification chapter 5.6) [UC_INP REQ 3.1]: 
							// The maximum permitted price reduction discount percentage is 100%! 
							// More than 100% will be set to 0% reduction (no rebate for this item!). 
							// Program continues in this case, but we do an error log.
							// Watch out: Price Increases of more than 100% are accepted!
							basar::cmnutil::Decimal maxDiscount(-100.0);
							if ( maxDiscount > discountPct )
							{


								// Set discount > 100% to zero. Write error log, but continue processing.
								std::stringstream str;
								basar::Int16 toright = discountPct.getAllDecimalPlaces();
								str << "Error: PCD_06-line with discountPct > 100 found: <"
									<< discountPct.toNonLocString(toright)    
									<< ">!"
									<< std::endl;
								BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

								discountPct.fromNonLocString("0");							}
						}
					}
					etoPosPtr->setDiscountPct(discountPct);


					//ALC-SEGMENT / NONCHARGEDQTY: PBS999 - special condition from PBS999 (Sonderfall 8): ALC-Segment controls acceptance of NONCHARGEDQTY
					bool ignoreNR = true;

					if (etoPosPtr->getAlc_05_Line_Alc_05_LinePtr())
					{
						if (    domMod::line::Alc_05_Line::enH == etoPosPtr->getAlc_05_Line_Alc_05_LinePtr()->getPriceHandling() 
							||	domMod::line::Alc_05_Line::enM == etoPosPtr->getAlc_05_Line_Alc_05_LinePtr()->getPriceHandling() )
						{
							// Special condition (Sonderfall) 8 of PBS999:
							// A position without ALC_05-Segment with 'H' or 'M' 
							// may not have a noncharged quantity: Set it to 0 to ignore it!
							ignoreNR = false;
						}
					}

					if (ignoreNR && etoPosPtr->getQty_10_Line_NonChargedQtyPtr())
					{
						// TODO: Perhaps extract this Temporary log ignored NRs!
						std::stringstream str;
						basar::Int32 nr = etoPosPtr->getQty_10_Line_NonChargedQtyPtr()->getQuantity();
						str << "Info:  IGNORED NonchargedQuantity ("
							<< nr    
							<< ") because of missing ALC-H/M-Segment!"
							<< std::endl;
						BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						// TODO End

						etoPosPtr->getQty_10_Line_NonChargedQtyPtr()->setQuantity(0);
					}

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch


				return true;
			}

			//*******************************************************************************************************
			//*****************************  Complete for Inhouse DE ************************************************
			//*******************************************************************************************************

			bool	AbbauwOrderDM::completeInfoFromEdiTransferOrderInhDE(domMod::transferOrderDM::EdiTransferOrder& eto)
			{
				METHODNAME_DEF( AbbauwOrderDM, completeInfoFromEdiTransferOrderInhDE )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try {

					// At first complete ORDERHEAD-data
					
					// BRANCHNO
					// Read the branchno from table filiale: select filialnr from filiale where bganr=3001566. The bganr is sent within the NAD-DS line.
					// (The branchno can also be extracted from the filename of the inhouse-File that contains this order. E.G.: uewauf.070957.UNIX was sent by branch 57 (Munich))

					//			basar::Int16	AbbauwOrderDM::findBranchnoInhDE	(const basar::I18nString& branchnoBga)

					basar::Int16 branchno = 0;
					if( eto.getNad_04_Line_BranchPtr() )
					{
						branchno = findBranchnoInhDE(eto.getNad_04_Line_BranchPtr()->getIdent());		

						if (0 >= branchno )
						{
							// log: Cannot recognize the branchno!!
							std::stringstream str;
							str << "Cannot generate branchno from BGA in NADline-DS: <"
								<< eto.getNad_04_Line_BranchPtr()->getIdent()  
								<< ">!"
								<< std::endl;
							BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
							BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						}
					}
					else
					{
						// log: The branchno to which this order belongs is unknown!!
						std::stringstream str;
						str << "UW order does not contain the branchno (=Phoenix-branchno), no NAD-lineDS or no valid value in it! " 
							<< std::endl;
						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						// No real need to throw an exception Unknown error
						return false;
					}

					eto.setBranchno(branchno);


					
					// ORDERNO   (same as in CH)
					// kes 10.02.15: Orderno is calculated by database now (type serial). It is done later during INSERT.
					// orig beginn
					//basar::Int32 ord = findBiggestExistingOrdernoByBranchno(eto.getBranchno());
					//++ord;	
					//eto.setOrdernoBatch(ord);
					// orig end
					eto.setOrdernoBatch(0);			// Set 0, real orderno is created on INSERT in serial-field by database

						
					// STATUS  (same as in CH)
					if (!eto.getPositionSegments().empty() )
					{
						eto.setStatus(libabbauw::domMod::lit::STATUS_DEFERRED);			// Status "0" - "ZU" 
					}
					else
					{
						// Integrity of data (Plausibilität No 2, specification chapter 5.6) [UC_INP REQ 3.1]: 
						// An order needs at least one position. 
						// If there is none, the order will be written to orderhead, 
						// but its status will be set to "ST"-cancelled/storniert! 
						eto.setStatus(libabbauw::domMod::lit::STATUS_CANCELLED);		// Status "9" - "ST" 
					}



					// PHARMACYNO
					// In DE there are two NAD_04-line types with customer info: types 'CN' and 'BY'.
					// If both are given: 'CN' has preference. Otherwise get 'BY'. 
					basar::Int32		pharmacyno		= 0;
					basar::I18nString	pharmacynoStr	= "";
					bool				validPharmacynoFound = false; 

					// Is there a NAD_CN given: This contains the pharmacyno
					if( eto.getNad_04_Line_PharmacyCNPtr() )
					{
						// Check that NAD_CN is of type BGA (that is idfnr-type)
						// Watch out: PBS999 didn't check this!
						if ( domMod::line::Nad_04_Line::enBGA == eto.getNad_04_Line_PharmacyCNPtr()->getCodelist())
						{
							pharmacynoStr = eto.getNad_04_Line_PharmacyCNPtr()->getIdent();	
							if (pharmacynoStr.isDigit() )
							{
								pharmacyno = static_cast<basar::Int32>(pharmacynoStr.stol());
							}
							else{

								// log: Cannot recognize customer-pharmacy!!
								std::stringstream str;
								str << "Cannot generate pharmacyno (=idfnr) from NAD_04_line-CN (non-digit): <"
									<< eto.getNad_04_Line_PharmacyCNPtr()->getIdent()  
									<< ">!"
									<< std::endl;
								BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
								BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
							}

						}
						else
						{
								// log: NAD_CN with non-BGA-Code!!
								std::stringstream str;
								str << "Cannot generate pharmacyno (=idfnr) from NAD_04_line-CN (non-BGA-Code): <"
									<< eto.getNad_04_Line_PharmacyCNPtr()->getIdent()  
									<< ">!"
									<< std::endl;
								BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
								BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						}

					}

					// We didn`t find a pharmacyno yet? Try NAD_BY
					if (    0 >= pharmacyno )
					{
						if( eto.getNad_04_Line_PharmacyBYPtr())
						{
							// Check that NAD_BY is of type BGA 
							// A BGA is in idfnr-coding: No further conversion necessary. No other coding is accepted.
							// Watch out: PBS999 didn't check this!
							if ( domMod::line::Nad_04_Line::enBGA == eto.getNad_04_Line_PharmacyBYPtr()->getCodelist())
							{
								pharmacynoStr = eto.getNad_04_Line_PharmacyBYPtr()->getIdent();	
								if (pharmacynoStr.isDigit() )
								{
									pharmacyno = static_cast<basar::Int32>(pharmacynoStr.stol());
								}
								else{
									// log: Cannot recognize customer-pharmacy!!
									std::stringstream str;
									str << "Cannot generate pharmacyno (=idfnr) from NAD_04_line-BY (non-digit): <"
										<< eto.getNad_04_Line_PharmacyBYPtr()->getIdent()  
										<< ">!"
										<< std::endl;
									BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
									BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
								}
							}
							else
							{
								// log: NAD_BY with non-BGA-Code!!
								std::stringstream str;
								str << "Cannot generate pharmacyno (=idfnr) from NAD_04_line-BY (non-BGA-Code): <"
									<< eto.getNad_04_Line_PharmacyBYPtr()->getIdent()  
									<< ">!"
									<< std::endl;
								BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
								BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
							}
						}
						else
						{
							// log: An order without CN and/or BY lines for a customer-pharmacy!!
							std::stringstream str;
							str << "UW order does not contain a pharmacyno (=customer-idfnr, no NAD_CN and NAD-line_BY) "  << std::endl;

							BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
							BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
							BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
							BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						}
					}
					// special check for branchno=58 (Freiburg) as this needs to be rerouted to a different branch based on a csv file
					if ((branchno == 58) && (pharmacyno > 0))
					{
						std::stringstream str;
						str << "Mapping check for branch 58 and pharmacyno:" << pharmacyno << std::endl;

						BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun);
						BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

						basar::Int16 newBranchno = getBranchnoByPharmacyno(branchno, pharmacyno);
						if (newBranchno > 0)
						{
							branchno = newBranchno;
							eto.setBranchno(branchno);
						}
					}
					// Check whether the pharmacyno exists
					if ((pharmacyno > 0) && (m_customerConsultantDM.checkExistenceByBranchnoPharmacyno(branchno, pharmacyno) == true))
					{
						validPharmacynoFound = true;
					}



					// Still didn't get a valid pharmacyno?
					if ( !validPharmacynoFound)
					{
						// set to pseudo-IDFNR for unknown pharmacy
						basar::Int32 pseudoPharmacyno 
								= m_customerConsultantDM.findPseudopharmacynoByBranchno(branchno);

						if (  0 < pseudoPharmacyno )
						{
							// This should be the unique manufacturerno for the whole order
							// We utilize this. If the transfer order contains articles of different manufacturers - it is his problem, we cannot solve it.
							pharmacyno= pseudoPharmacyno;
						}
						else
						{
							// log: There exists no pseudopharmacy number for this branchno!!
							std::stringstream str;
							str << "Cannot find a pseudo-pharmacyno for branchno= " 
								<< branchno
								<< " Definition in kundeschalter missing?"
								<< std::endl;
							BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
							BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

							pharmacyno= 0;   // DE: PBS999 didnt convert to a pseudo idf-number	
						}


						// just log, no error: An order without a customer-pharmacy!!
						std::stringstream str;
						str << "UW order for branch "
							<< branchno
							<< "does not contain a valid pharmacyno. Set pseudo-Idfnr = "
							<< pseudoPharmacyno
							<< std::endl;
						BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
						BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					}

					eto.setPharmacyno(pharmacyno);

					// ORDERVALUE: For now we do not set it, because it is redundant. Perhaps later for performance reasons

					// Now complete the positions data
					if ( !eto.getPositionSegments().empty())
					{
						basar::Int32 cntElems = static_cast<basar::Int32>(eto.getPositionSegments().size());

						for (basar::Int32 index = 0; index < cntElems ;   ++index )
						{
							if ( !completeInfoFromEdiPositionInhDE( (eto.getPositionSegments()).at(index)) )
							{
								BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
								std::stringstream str;
								str << "Could not complete Info for position number " << index << std::endl;
								throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ));
							}
						}

					}

					BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, eto.getUnb_00_Line_Ptr()->getSupplierEan());
					BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, eto.getPositionSegments().at(0)->getArticleCode());

					// SET ORDERTYPE, ORDERPICKINGTYPE, BOOKENTRYTYPE 
					if (isCMIOrder(eto.getUnb_00_Line_Ptr()->getSupplierEan(), eto.getPositionSegments().at(0)->getArticleCode()))
					{
						eto.setOrdertype(libabbauw::domMod::lit::DG_ORDERTYPE);
						eto.setOrderpickingtype(libabbauw::domMod::lit::DG_ORDERPICKINGTYPE);
						eto.setBookentrytype(libabbauw::domMod::lit::DG_BOOKENTRYTYPE);
					}
					else
					{
						// Default is UW00
						eto.setOrdertype(libabbauw::domMod::lit::UW_ORDERTYPE);			// typical "Ueberweiser" = transfer order "UW00"
						eto.setOrderpickingtype(libabbauw::domMod::lit::UW_ORDERPICKINGTYPE);
						eto.setBookentrytype(libabbauw::domMod::lit::UW_BOOKENTRYTYPE);
					}

					
					// ORDERNOMF: Is transferred not here but in AbbauwOrderDM::saveInsertAbbauwOrderInhDE()


					// MANUFACTURERNO
					// In DE, the NAD MF is given as a BFA-Number instead of HNR. This cannot be transformed into a unique manufacturerno (phoenix-HNR).
					// Therefore we try something else:
					// Get the manufacturerno from the first articleposition that leads to a valid manufacturerno. 
					// If there is no: Set 0 as manufacturerno.
					if ( !eto.getPositionSegments().empty())		// this order got positions
					{
						basar::Int32 cntElems								= static_cast<basar::Int32>(eto.getPositionSegments().size());

						for (basar::Int32 index = 0; index < cntElems ;   ++index )
						{
							basar::Int32 currManufacturerno 
								= m_articleConsultantDM.findManufacturernoByArticleno((eto.getPositionSegments()).at(index)->getArticleno());

							if (  0 < currManufacturerno )
							{
								// This should be the unique manufacturerno for the whole order
								// We utilize this. If the transfer order contains articles of different manufacturers - it is his problem, we cannot solve it.
								eto.setManufacturerno(currManufacturerno);
								break;
							}
						}
					}


					
					// HEADERTEXT 
					// In DE: place the headertext of the customerorder here
					// In VX27 there are four cases how to format the headertext. E.g. 
					//      "11965/4902236532 UK: 12345678"
					//   or "11965/4902236532 FREIER AAQ-TEXT" 
					//	 or	"00000/0400585201 HE: 68758258" 
					//   or "11965/4902236532 HE: HEXAL#O" 
					// Format Kopftext:  <WFC_T_HNR (5char)>/<WFC_T_KAUFNR (10 char)> HE: <Lieferantname oder hnr oder lant_nr ()>
					// TODO: PBS999-headertext was max. 40 chars. Here we can have max. 28 chars only because of ksc-client (Ask German users!)
					basar::I18nString invoicetext("");

					// Generate WFC-T-HNR:	hersteller_nr (ToDo: Maybe they want the lant_nr here?)
					basar::I18nString str("");
					//str.format("%5d", eto.getManufacturerno());
					str.format("%d", eto.getManufacturerno());
					invoicetext = invoicetext + str;

					// Generate WFC-T-KAUFNR: From BGM_02 segment field BGM-DOKNUM
					invoicetext = invoicetext + "/" + eto.getOrdernoMFDE();	
					invoicetext.trim(basar::cmnutil::BString::END);		
					invoicetext = invoicetext + " ";

					// Generate WFC_TEXTZUSATZ
					// First rule: If no regular pharmacyno was found: 
					//         WFC-TEXTZUSATZ = "UK: " + NAD_IDENT + "/" + NAD_NAME       
					//        Values are to be taken from NAD_CN-Segment. If NAD_CN-segment  does notexist: From NAD_BY segment.
					if (!validPharmacynoFound)
					{
						invoicetext = invoicetext + "UK:";

						if( eto.getNad_04_Line_PharmacyCNPtr())
						{
							invoicetext =	invoicetext 
								          +	eto.getNad_04_Line_PharmacyCNPtr()->getIdent() 
										  + "/" 
										  +	eto.getNad_04_Line_PharmacyCNPtr()->getNameText();
						}
						else if( eto.getNad_04_Line_PharmacyBYPtr())
						{
							invoicetext =	invoicetext 
								          +	eto.getNad_04_Line_PharmacyBYPtr()->getIdent() 
										  + "/" 
										  +	eto.getNad_04_Line_PharmacyBYPtr()->getNameText();
						}
						else
						{
							invoicetext =	invoicetext 
								          +	"Unknown pharmacyno/-name"
										  + "/" ;
						}
					}

					// Second rule: If order contains FTX-11-segment with FTX_ZUCODE="AAQ" (This means customer created headertext): 
					//        WFC-TEXTZUSATZ =  FTX-TEXT from FTX-11-segment     
					//        Values are to be taken from NAD_CN-Segment. If NAD_CN-segment  does notexist: From NAD_BY segment.
					else if( eto.getFtx_11_Line_Ptr()  &&  eto.getFtx_11_Line_Ptr()->getFreeText().length() >0 )
					{
						// Regular invoicetext for orders from EDI-flatfile
						invoicetext = invoicetext + eto.getFtx_11_Line_Ptr()->getFreeText();

					}

					// Third rule: If the manufacturer could be identified: 
					//        WFC-TEXTZUSATZ =  HE: <Lieferanten-Kurzbezeichnung>     
					else if( eto.getManufacturerno() > 0 )
					{
						basar::I18nString manufacturername 
							= m_manufacturerConsultantDM.findManufacturernameByManufacturerno(eto.getManufacturerno());

						invoicetext =	  invoicetext 
								+ "HE:" 
								+ manufacturername
								;
					}

					// Fourth rule: If the manufacturer could NOT be identified from the articles: 
					//        WFC-TEXTZUSATZ =  HE: <BFA-number of manufacturer>     
					else
					{
						if ( eto.getNad_04_Line_ManufacturerPtr())
						{
							invoicetext =	  invoicetext 
								+ "HE:" 
								+ eto.getNad_04_Line_ManufacturerPtr()->getIdent()
								;
						}
						else
						{
							// Manufacturer couldn't be extracted from the articles and in addition to this the BFA-Number is missing
							invoicetext =	  invoicetext 
								+ "HE:" 
								+ "UNBEK. HERSTELLER + BFA FEHLT"
								;
						}
					}


					// invoicetext mustn`t be longer than 30 char (otherwise we get an exception from kscclient when the order is released)
					eto.setHeadertext(invoicetext.substr(0,30));			// Text for invoice: Headertext


				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch

				return true;
			}



			//*************************************** saveInsertAbbauwOrder *****************************************************
			// Precondition: eto must be set and validated
			bool	AbbauwOrderDM::saveInsertAbbauwOrder( domMod::transferOrderDM::EdiTransferOrder&	 eto)
			{
				METHODNAME_DEF( AbbauwOrderDM, saveInsertAbbauwOrder )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
				
				try
				{
					// for CH
					if (	domMod::orderMapper::enInputtype_inhCH	== eto.getInputtype() )
					{
						bool ret = saveInsertAbbauwOrderInhCH (eto);
						return ret;
					}
					//for DE
					else if (	domMod::orderMapper::enInputtype_inhDE	== eto.getInputtype()	)
					{
						bool ret = saveInsertAbbauwOrderInhDE (eto);
						return ret;
					}
					else
					{
						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
						std::stringstream str;
						str << "Unknown Inhouse format type: " << eto.getInputtype() << std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}
				}
				catch(...) {
					// Throw same exception 
					throw;
				}// catch

			}

			basar::Int16 AbbauwOrderDM::getBranchnoByPharmacyno(const basar::Int16 branchNo, const basar::Int32 pharmacyno)
			{
				basar::Int16 mapBranchNo = 0;
				basar::I18nString filename;
				filename.format("%s/%02i_pharmacyno_to_branchno.csv", m_InputDir.c_str(), branchNo);

				// file contains a header and is formatted like this:
				// orgbranchno;pharmacyno;mapbranchno
				boost::filesystem::fstream fileStream(filename.c_str());
				std::string line;
				bool fnd = false;
				int lineNo = 0;
				while ((fnd == false) && (std::getline(fileStream, line)))
				{
					// skip header line
					if (line.find("orgbranchno") != std::string::npos)
						continue;
					// get orgbranchno
					size_t pos = line.find(';');
					if (pos == std::string::npos)
						continue;
					std::string orgBranchNoStr = line.substr(0, pos);
					basar::Int16 orgBranchNo = (basar::Int16)atoi(orgBranchNoStr.c_str());
					if (orgBranchNo != branchNo)
						continue;
					// get pharmacyno
					size_t pos2 = line.find(';', pos + 1);
					if (pos2 == std::string::npos)
						continue;
					std::string pharmacynoStr = line.substr(pos + 1, pos2 - pos - 1);
					basar::Int32 pharmaNo = (basar::Int32)atoi(pharmacynoStr.c_str());
					if (pharmaNo != pharmacyno)
						continue;
					// get mapbranchno
					std::string mapBranchNoStr = line.substr(pos2 + 1);
					mapBranchNo = (basar::Int16)atoi(mapBranchNoStr.c_str());
					fnd = true;
					lineNo++;
				}
				fileStream.close();

				std::stringstream str;
				if (fnd == true)
				{
					str << "Mapping branch " << branchNo << " and pharmacyno: " << pharmacyno << " to: " << mapBranchNo << std::endl;
					BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				}
				else
				{
					str << "No mapping found for branch " << branchNo << " and pharmacyno: " << pharmacyno << " in " << lineNo << " lines in file: " << filename << std::endl;
					BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				}

				return mapBranchNo;
			}

			bool	AbbauwOrderDM::saveInsertAbbauwOrderInhCH( domMod::transferOrderDM::EdiTransferOrder&	 eto)
			{
				METHODNAME_DEF( AbbauwOrderDM, saveInsertAbbauwOrderInhCH )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				//Don't save the order if it is a test order
				if (eto.getUnbLine_Ptr())
				{
					if (eto.getUnbLine_Ptr()->isTestOrder())
					{
						std::stringstream str;
						str << "saveInsertAbbauwOrderInhCH(): Test order detected! We do not save it! Position:  " 
							<< eto.getOrderStartPos()	<< std::endl;
						BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

						return true;
					}
				}
				
				// insert a first line with status FOR_INSERT  
				// and create an iterator for the orderhead data (only one line in the property table)
				m_ACCInstOrderheadRef.getPropertyTable().clear();		
				m_ACCInstOrderheadRef.getPropertyTable().insert(basar::FOR_INSERT);		
				AccessorPropertyTable_YIterator yitOrderhead = m_ACCInstOrderheadRef.getPropertyTable().begin();

				try
				{
					// Set up the orderhead-Properties ..
					// .. BRANCHNO:	from NADDS, ...34 = branchno 12, ...209 = branchno 13
					yitOrderhead.setInt16(libabbauw::properties::BRANCHNO.getName(), eto.getBranchno());

					// .. ORDERNO: Fetch the next available orderno 
					// kes 10.02.15: Is zero at this point, will be created when serial no is inserted
					yitOrderhead.setInt32(libabbauw::properties::ORDERNO.getName(), eto.getOrdernoBatch());		

					// .. STATUS: 
					yitOrderhead.setString(libabbauw::properties::STATUS.getName(), eto.getStatus());		

					// .. ORDERTYPE, ORDERPICKINGTYPE, ORDERTYPE: 
					yitOrderhead.setString(libabbauw::properties::ORDERTYPE.getName(), eto.getOrdertype());		
					yitOrderhead.setString(libabbauw::properties::ORDERPICKINGTYPE.getName(), eto.getOrderpickingtype());		
					yitOrderhead.setString(libabbauw::properties::BOOKENTRYTYPE.getName(), eto.getBookentrytype());		

					// .. ORDERDATE:  
					yitOrderhead.setInt32(libabbauw::properties::ORDERDATE.getName(), eto.getOrderDateCH());		

					// .. ORDERVALUE: For now we do not set it, because it is redundant. Perhaps later for performance reasons
					yitOrderhead.setInt32(libabbauw::properties::ORDERVALUE.getName(), 0);		

					// .. DEFERREDPAYMENTDATE 
					yitOrderhead.setInt32(libabbauw::properties::DEFERREDPAYMENTDATE.getName(), eto.getDeferredPaymentDateCH());		

					// .. PHARMACYNO
					yitOrderhead.setInt32(libabbauw::properties::PHARMACYNO.getName(), eto.getPharmacyno());		

					// .. HEADERTEXT
					yitOrderhead.setString(libabbauw::properties::HEADERTEXT.getName(), eto.getHeadertext());		

					// .. ORDERACQUISITIONDATE: set to 0 
					yitOrderhead.setInt32(libabbauw::properties::ORDERACQUISITIONDATE.getName(), 0);		

					// .. DATASOURCETYPE: Fixed text "UW" or "OM"
					const bool isOmgOrder = findIsOmgOrder(eto.getBranchno(), eto.getPharmacyno());

					if ( isOmgOrder )
					{
						yitOrderhead.setString(libabbauw::properties::DATASOURCETYPE.getName(), libabbauw::domMod::lit::OMG_ORDERTYPE);		
					}
					else
					{
						yitOrderhead.setString(libabbauw::properties::DATASOURCETYPE.getName(), libabbauw::domMod::lit::UW_ORDERTYPE);		
					}

					// .. EDIDELIVERYDATE
					yitOrderhead.setInt32(libabbauw::properties::EDIDELIVERYDATE.getName(), eto.getEdiDeliveryDateCH());		

					// .. REFERENCE: set to empty
					yitOrderhead.setString(libabbauw::properties::REFERENCE.getName(), "");

					// .. ORDERNOCSC: set to 0 
					yitOrderhead.setInt32(libabbauw::properties::ORDERNOCSC.getName(), 0);		

					// .. ORDERNOMF
					yitOrderhead.setString(libabbauw::properties::ORDERNOMF.getName(), eto.getOrdernoMFCH());		

					// .. MANUFACTURERNO: 
					yitOrderhead.setInt32(libabbauw::properties::MANUFACTURERNO.getName(), eto.getManufacturerno());	

					// .. EDIDELIVERYDATEMF
					yitOrderhead.setInt32(libabbauw::properties::EDIDELIVERYDATEMF.getName(), eto.getEdiDeliveryDateCH());		

					// kes, 26.06.14: New field pharmacynomf
					// .. PHARMACYNOMF
					yitOrderhead.setString(libabbauw::properties::PHARMACYNOMF.getName(), eto.getPharmacynoMFCH());		

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error (1)" << std::endl;
					throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch


				try {	
					// make sure that only one last serial (orderno) exists which is needed 
					// to insert new positions that use our head later
					// serial is automatically created by the DB when new head is inserted
					// Resets serials from the accessor that are created earlier.
					m_ACCInstOrderheadRef.clearLastInsertedSerials();	 

					// make sure that only one last serial (orderno) exists which is needed 
					// to insert new positions that use our head later
					// serial is automatically created by the DB when new head is inserted
					// Resets serials from the accessor that are created earlier.
					m_ACCInstOrderheadRef.clearLastInsertedSerials();	 

					// insert	
					ExecuteResultInfo	resInfo  
						= m_ACCInstOrderheadRef.execute(libabbauw::domMod::ACCMETH_INSERT_INTO_ORDERHEAD, yitOrderhead);

					if( resInfo.hasError() ){
						basar::VarString msg;
						msg.format( "saveInsertAbbauwOrder - SQL execution has errors: <%ld>", resInfo.m_error );
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						std::stringstream str;
						str << fun << " Unknown error (2)" << std::endl;
						throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					// get the newly serially created orderno
					basar::Int64 ordernoSerial = m_ACCInstOrderheadRef.getLastInsertedSerial(0);
					eto.setOrdernoBatch(static_cast<basar::Int32>(ordernoSerial));

					// Now save the positions data
					if ( !eto.getPositionSegments().empty())
					{
						basar::Int32 cntElems = static_cast<basar::Int32>(eto.getPositionSegments().size());

						for (basar::Int32 index = 0; index < cntElems ;   ++index )
						{

							if ( !saveInsertAbbauwOrderPosInhCH(  (eto.getPositionSegments()).at(index)
								, eto.getBranchno()
								, eto.getOrdernoBatch()
								, index +1) )						// positionnumber counts from 1 (!) to cntElems			
							{
								BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

								std::stringstream str;
								str << fun << " Error: Could not save Orderpositions " << std::endl;
								throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
							}
						}
					}


					return true;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 


					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// saveInsertAbbauwOrderInhCH


			

			bool	AbbauwOrderDM::saveInsertAbbauwOrderInhDE( domMod::transferOrderDM::EdiTransferOrder&	 eto)
			{
				METHODNAME_DEF( AbbauwOrderDM, saveInsertAbbauwOrderInhDE )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				//Don't save the order if it is a test order
				if (eto.getUnb_00_Line_Ptr())
				{
					if (eto.getUnb_00_Line_Ptr()->isTestOrder())
					{
						std::stringstream str;
						str << "saveInsertAbbauwOrderInhDE(): Test order detected! We do not save it! Position:  " 
							<< eto.getOrderStartPos()	<< std::endl;
						BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

						return true;
					}
				}
				
				// insert a first line with status FOR_INSERT  
				// and create an iterator for the orderhead data (only one line in the property table)
				m_ACCInstOrderheadRef.getPropertyTable().clear();		
				m_ACCInstOrderheadRef.getPropertyTable().insert(basar::FOR_INSERT);		
				AccessorPropertyTable_YIterator yitOrderhead = m_ACCInstOrderheadRef.getPropertyTable().begin();


				try
				{
					// Set up the orderhead-Properties ..
					// .. BRANCHNO:	from NADDS, ...34 = branchno 12, ...209 = branchno 13
					yitOrderhead.setInt16(libabbauw::properties::BRANCHNO.getName(), eto.getBranchno());

					// .. ORDERNO: Fetch the next available orderno (serial: is 0 now, insert createst real number)
					yitOrderhead.setInt32(libabbauw::properties::ORDERNO.getName(), eto.getOrdernoBatch());		

					// .. STATUS: 
					yitOrderhead.setString(libabbauw::properties::STATUS.getName(), eto.getStatus());		

					// .. ORDERTYPE, ORDERPICKINGTYPE, ORDERTYPE: 
					yitOrderhead.setString(libabbauw::properties::ORDERTYPE.getName(), eto.getOrdertype());		
					yitOrderhead.setString(libabbauw::properties::ORDERPICKINGTYPE.getName(), eto.getOrderpickingtype());		
					yitOrderhead.setString(libabbauw::properties::BOOKENTRYTYPE.getName(), eto.getBookentrytype());		

					// .. ORDERDATE:  
					yitOrderhead.setInt32(libabbauw::properties::ORDERDATE.getName(), eto.getOrderDateDE());		

					// .. ORDERVALUE: For now we do not set it, because it is redundant. Perhaps later for performance reasons
					yitOrderhead.setInt32(libabbauw::properties::ORDERVALUE.getName(), 0);		

					// .. DEFERREDPAYMENTDATE 
					yitOrderhead.setInt32(libabbauw::properties::DEFERREDPAYMENTDATE.getName(), eto.getDeferredPaymentDateDE());		

					// .. PHARMACYNO
					yitOrderhead.setInt32(libabbauw::properties::PHARMACYNO.getName(), eto.getPharmacyno());		

					// .. HEADERTEXT
					yitOrderhead.setString(libabbauw::properties::HEADERTEXT.getName(), eto.getHeadertext());		

					// .. ORDERACQUISITIONDATE: set to 0 
					yitOrderhead.setInt32(libabbauw::properties::ORDERACQUISITIONDATE.getName(), 0);		

					// .. DATASOURCETYPE: Fixed text "UW"
					yitOrderhead.setString(libabbauw::properties::DATASOURCETYPE.getName(), libabbauw::domMod::lit::UW_ORDERTYPE);		

					// .. EDIDELIVERYDATE
					yitOrderhead.setInt32(libabbauw::properties::EDIDELIVERYDATE.getName(), eto.getEdiDeliveryDateDE());		

					// .. REFERENCE: set to empty
					yitOrderhead.setString(libabbauw::properties::REFERENCE.getName(), "");

					// .. ORDERNOCSC: set to 0 
					yitOrderhead.setInt32(libabbauw::properties::ORDERNOCSC.getName(), 0);		

					// .. ORDERNOMF
					yitOrderhead.setString(libabbauw::properties::ORDERNOMF.getName(), eto.getOrdernoMFDE());		

					// .. MANUFACTURERNO: 
					yitOrderhead.setInt32(libabbauw::properties::MANUFACTURERNO.getName(), eto.getManufacturerno());	

					// .. EDIDELIVERYDATEMF
					yitOrderhead.setInt32(libabbauw::properties::EDIDELIVERYDATEMF.getName(), eto.getEdiDeliveryDateDE());		

					// kes, 26.06.14: New field pharmacynomf
					// .. PHARMACYNOMF
					yitOrderhead.setString(libabbauw::properties::PHARMACYNOMF.getName(), eto.getPharmacynoMFDE());		

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error (1)" << std::endl;
					throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch


				try {	
					// make sure that only one last serial (orderno) exists which is needed 
					// to insert new positions that use our head later
					// serial is automatically created by the DB when new head is inserted
					// Resets serials from the accessor that are created earlier.
					m_ACCInstOrderheadRef.clearLastInsertedSerials();	 

					// insert	
					ExecuteResultInfo	resInfo  
						= m_ACCInstOrderheadRef.execute(libabbauw::domMod::ACCMETH_INSERT_INTO_ORDERHEAD, yitOrderhead);

					if( resInfo.hasError() ){
						basar::VarString msg;
						msg.format( "saveInsertAbbauwOrder - SQL execution has errors: <%ld>", resInfo.m_error );
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						std::stringstream str;
						str << fun << " Unknown error (2)" << std::endl;
						throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					basar::Int64 ordernoSerial = m_ACCInstOrderheadRef.getLastInsertedSerial(0);
					eto.setOrdernoBatch(static_cast<basar::Int32>(ordernoSerial));

					// Now save the positions data
					if ( !eto.getPositionSegments().empty())
					{
						basar::Int32 cntElems = static_cast<basar::Int32>(eto.getPositionSegments().size());

						for (basar::Int32 index = 0; index < cntElems ;   ++index )
						{

							if ( !saveInsertAbbauwOrderPosInhDE(  (eto.getPositionSegments()).at(index)
								, eto.getBranchno()
								, eto.getOrdernoBatch()
								, index +1) )						// positionnumber counts from 1 (!) to cntElems			
							{
								BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

								std::stringstream str;
								str << fun << " Error: Could not save Orderpositions " << std::endl;
								throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
							}
						}
					}


					return true;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 


					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// saveInsertAbbauwOrderInhDE


			//**********************************************************************************************************************
			//*************************************** saveInsertAbbauwOrderPos *****************************************************
			//**********************************************************************************************************************
			bool	AbbauwOrderDM::saveInsertAbbauwOrderPosInhCH(const domMod::transferOrderDM::PositionSegmentPtr& etoPosPtr
																	, basar::Int16 branchno
																	, basar::Int32 orderno
																	, basar::Int32 posno)
			{
				METHODNAME_DEF( AbbauwOrderDM, saveInsertAbbauwOrderPosInhCH )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try {		

					// insert a first line with status FOR_INSERT  
					// and create an iterator for the orderhead data (only one line in the property table)
					m_ACCInstOrderposRef.getPropertyTable().clear();		
					m_ACCInstOrderposRef.getPropertyTable().insert(basar::FOR_INSERT);		
					AccessorPropertyTable_YIterator yitOrderpos = m_ACCInstOrderposRef.getPropertyTable().begin();

					// Set up the orderhead-Properties ..
					// .. BRANCHNO:	from parameter
					yitOrderpos.setInt16(libabbauw::properties::BRANCHNO.getName(), branchno);

					// .. ORDERNO: Fetch the next available orderno 
					yitOrderpos.setInt32(libabbauw::properties::ORDERNO.getName(), orderno);		

					// .. POSNO: Fetch the next available orderno 
					yitOrderpos.setInt32(libabbauw::properties::POSNO.getName(), posno);		


					// .. ARTICLENO
					yitOrderpos.setInt32(libabbauw::properties::ARTICLENO.getName(), etoPosPtr->getArticleno());		

					// .. ARTICLE_CODE: 
					yitOrderpos.setString(libabbauw::properties::ARTICLE_CODE.getName(), etoPosPtr->getArticleCode());		

					// ORDEREDQTY
					basar::Int32 orderedqty = 0;
					if (etoPosPtr->getQtyLine_OrderedQtyPtr())
					{
						orderedqty = etoPosPtr->getQtyLine_OrderedQtyPtr()->getQuantity();
					}
					yitOrderpos.setInt32(libabbauw::properties::ORDEREDQTY.getName(), orderedqty);		

					// NONCHARGEDQTY
					basar::Int32 nonchargedqty = 0;
					if (etoPosPtr->getQtyLine_ChargedQuantityPtr())
					{
						nonchargedqty = orderedqty - etoPosPtr->getQtyLine_ChargedQuantityPtr()->getQuantity();
					}
					yitOrderpos.setInt32(libabbauw::properties::NONCHARGEDQTY.getName(), nonchargedqty);		

					// .. TEXTFIELD:			
					yitOrderpos.setString(libabbauw::properties::TEXTFIELD.getName(), etoPosPtr->getTextfield());		

					// CUSTOMERPOSNO 
					basar::Int32 custposno = 0;
					if ( etoPosPtr->getLinLine_LinLinePtr() )
					{
						custposno = etoPosPtr->getLinLine_LinLinePtr()->getPositionno();
					}
					yitOrderpos.setInt32(libabbauw::properties::CUSTOMERPOSNO.getName(), custposno);		


					// .. WHOLESALERPURCHASEPRICE 
					basar::cmnutil::Decimal	wholesalerpurchaseprice = etoPosPtr->getWholesalerpurchaseprice();
					yitOrderpos.setDecimal(libabbauw::properties::WHOLESALERPURCHASEPRICE.getName(), wholesalerpurchaseprice);		


					//DISCOUNTPCT with its base
					yitOrderpos.setDecimal(libabbauw::properties::DISCOUNTPCT.getName(), etoPosPtr->getDiscountPct() );		


					// insert	
					ExecuteResultInfo	resInfo  
						= m_ACCInstOrderposRef.execute(libabbauw::domMod::ACCMETH_INSERT_INTO_ORDERPOS, yitOrderpos);

					if( resInfo.hasError() ){
						basar::VarString msg;
						msg.format( "saveInsertAbbauwOrderPos - SQL execution has errors: <%ld>", resInfo.m_error );
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						std::stringstream str;
						str << fun << " Error: execute" << std::endl;
						throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					return true;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// saveInsertAbbauwOrderPosInhCH


			// for DE Germany
			bool	AbbauwOrderDM::saveInsertAbbauwOrderPosInhDE(const domMod::transferOrderDM::PositionSegmentPtr& etoPosPtr
																	, basar::Int16 branchno
																	, basar::Int32 orderno
																	, basar::Int32 posno)
			{
				METHODNAME_DEF( AbbauwOrderDM, saveInsertAbbauwOrderPosInhDE )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try {		

					// insert a first line with status FOR_INSERT  
					// and create an iterator for the orderhead data (only one line in the property table)
					m_ACCInstOrderposRef.getPropertyTable().clear();		
					m_ACCInstOrderposRef.getPropertyTable().insert(basar::FOR_INSERT);		
					AccessorPropertyTable_YIterator yitOrderpos = m_ACCInstOrderposRef.getPropertyTable().begin();

					// Set up the orderhead-Properties ..
					// .. BRANCHNO:	from parameter
					yitOrderpos.setInt16(libabbauw::properties::BRANCHNO.getName(), branchno);

					// .. ORDERNO: Fetch the next available orderno 
					yitOrderpos.setInt32(libabbauw::properties::ORDERNO.getName(), orderno);		

					// .. POSNO: Fetch the next available orderno 
					yitOrderpos.setInt32(libabbauw::properties::POSNO.getName(), posno);		


					// .. ARTICLENO
					yitOrderpos.setInt32(libabbauw::properties::ARTICLENO.getName(), etoPosPtr->getArticleno());		

					// .. ARTICLE_CODE: 
					yitOrderpos.setString(libabbauw::properties::ARTICLE_CODE.getName(), etoPosPtr->getArticleCode());		

					// ORDEREDQTY  getQty_10_Line_ChargedQtyPtr
					// Watch out: orderpos.orderedqty contains the sum of AMGE and NR!
					basar::Int32 orderedqty = 0;
					if (etoPosPtr->getQty_10_Line_ChargedQtyPtr())		// AMGE
					{
						orderedqty = etoPosPtr->getQty_10_Line_ChargedQtyPtr()->getQuantity();
					}
	
					// NONCHARGEDQTY
					basar::Int32 nonchargedqty = 0;
					if (etoPosPtr->getQty_10_Line_NonChargedQtyPtr())
					{
						nonchargedqty = etoPosPtr->getQty_10_Line_NonChargedQtyPtr()->getQuantity();
					}

					yitOrderpos.setInt32(libabbauw::properties::ORDEREDQTY.getName(),    orderedqty + nonchargedqty);	
					yitOrderpos.setInt32(libabbauw::properties::NONCHARGEDQTY.getName(), nonchargedqty);		

					// .. TEXTFIELD:			
					yitOrderpos.setString(libabbauw::properties::TEXTFIELD.getName(), etoPosPtr->getTextfield());		

					// CUSTOMERPOSNO 
					basar::Int32 custposno = 0;
					if ( etoPosPtr->getLin_08_Line_Lin_08_LinePtr() )
					{
						custposno = etoPosPtr->getLin_08_Line_Lin_08_LinePtr()->getPositionno();
					}
					yitOrderpos.setInt32(libabbauw::properties::CUSTOMERPOSNO.getName(), custposno);		


					// .. WHOLESALERPURCHASEPRICE 
					basar::cmnutil::Decimal	wholesalerpurchaseprice = etoPosPtr->getWholesalerpurchaseprice();
					yitOrderpos.setDecimal(libabbauw::properties::WHOLESALERPURCHASEPRICE.getName(), wholesalerpurchaseprice);		


					//DISCOUNTPCT with its base
					yitOrderpos.setDecimal(libabbauw::properties::DISCOUNTPCT.getName(), etoPosPtr->getDiscountPct() );		


					// insert	
					ExecuteResultInfo	resInfo  
						= m_ACCInstOrderposRef.execute(libabbauw::domMod::ACCMETH_INSERT_INTO_ORDERPOS, yitOrderpos);

					if( resInfo.hasError() ){
						basar::VarString msg;
						msg.format( "saveInsertAbbauwOrderPos - SQL execution has errors: <%ld>", resInfo.m_error );
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, msg );

						std::stringstream str;
						str << fun << " Error: execute" << std::endl;
						throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					return true;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown error" << std::endl;
					throw libabbauw::DataStoringException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// saveInsertAbbauwOrderPosInhDE


		} // namespace abbauwOrderDM
	} // namespace libAbbauw
} // namespace domMod

#ifdef WIN32
#pragma warning (pop)
#endif