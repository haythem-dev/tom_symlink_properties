//-------------------------------------------------------------------------------------
/*! \file  customerconsultantdm.cpp
*  \brief  Domain module for database operations regarding custmers
*  \author P. Kessling
*  \date   03.05.2013, 
*  history 03.05.13  pk new, similar to batchorderdm.h
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


#include "cmnaccessor.h"		// Usually we need only this one: a precompiled header, that has to be included first
								// but VStudio has to be prepared to utilize precompiled headers

#include <libbasar_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasarappl_systemeventmanager.h>
#include <libbasarlogin.h>
#include <libbasardbaspect.h>



#include "loggerpool.h"	// for LoggerPool
#include "abbauwexception.h"
#include "unprocessableexception.h"
#include "dataacquisitionexception.h"

#include "edi_input_properties_definitions.h"
#include "edi_input_accessor_definitions.h"
#include "customerconsultantdm.h"

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
		namespace customerConsultantDM
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
			CustomerConsultantDM::CustomerConsultantDM() : m_Initialized(0) 
			{
				METHODNAME_DEF( CustomerConsultantDM, CustomerConsultantDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}

			CustomerConsultantDM::~CustomerConsultantDM() 
			{
				METHODNAME_DEF( CustomerConsultantDM, ~CustomerConsultantDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}


			//**************************************************************************************************
			//         Methoden
			//**************************************************************************************************
			//*************************************** init *****************************************************
			void CustomerConsultantDM::init(const ConnectionRef activeConn) 
			{
				METHODNAME_DEF( CustomerConsultantDM, init )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


				try {		

					if(0 == m_Initialized) {
						m_ACCCustomerInstRef 
							  = basar::db::aspect::Manager::getInstance().createAccessorInstance(libabbauw::domMod::ACC_INST_CUSTOMER, libabbauw::domMod::ACC_CUSTOMER, activeConn, basar::db::aspect::FULL_CACHING);  
					}

					++m_Initialized;
				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << "createAccessorInstance failed!" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// init

			//*************************************** shutdown *****************************************************
			void CustomerConsultantDM::shutdown() 
			{
				METHODNAME_DEF( CustomerConsultantDM, shutdown )  
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

						if(!m_ACCCustomerInstRef.getPropertyTable().isNull())
						{
							m_ACCCustomerInstRef.getPropertyTable().clear();
						}
						m_ACCCustomerInstRef.reset();

					}// if(0==m_Initialized)

				}
				catch(...) {
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun );

						std::stringstream str;
						str << fun <<" Unknown exception " << std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

			}// shutdown


			//*************************************** findCustomerGroupByBranchnoPharmacyno *****************************************************
			basar::I18nString		CustomerConsultantDM::findCustomerGroupByBranchnoPharmacyno(const basar::Int16 branchno, const basar::Int32 pharmacyno)
			{
				METHODNAME_DEF( CustomerConsultantDM, findCustomerGroupByBranchnoPharmacyno )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				m_ACCCustomerInstRef.getPropertyTable().clear();					
				m_ACCCustomerInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
				AccessorPropertyTable_YIterator yitSearch = m_ACCCustomerInstRef.getPropertyTable().begin();
				yitSearch.setInt16(libabbauw::properties::BRANCHNO.getName(), branchno);
				yitSearch.setInt32(libabbauw::properties::PHARMACYNO.getName(), pharmacyno);

				try {		
					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCCustomerInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_CUSTOMERGROUP_BY_BRANCHNO_PHARMCYNO, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<" SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					basar::I18nString retstr("");
					if ( !m_ACCCustomerInstRef.getPropertyTable().empty())
					{
						retstr = m_ACCCustomerInstRef.getPropertyTable().begin().getString(libabbauw::properties::CUSTOMERGROUP.getName());
					}
					return retstr;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
					throw; 
				}// catch
			}// findCustomerGroupByBranchnoPharmacyno



			//*************************************** findPharmacynoByBranchnoPharmacynoean *****************************************************
			basar::Int32 CustomerConsultantDM::findPharmacynoByBranchnoPharmacynoean(const basar::Int16 branchno, const basar::I18nString pharmacynoean)
			{
				METHODNAME_DEF( CustomerConsultantDM, findPharmacynoByBranchnoPharmacynoean )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::Int32 ret = 0;

				if ( 0 >= branchno || 0 >= pharmacynoean.length() )
				{
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Cannot acquire pharmacyno: Branchno or pharmacyean not set" << std::endl;
					//throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );

					return ret;
				}

				try {	
					m_ACCCustomerInstRef.getPropertyTable().clear();					
					m_ACCCustomerInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCCustomerInstRef.getPropertyTable().begin();
					yitSearch.setInt16 (libabbauw::properties::BRANCHNO.getName(),		branchno);
					yitSearch.setString(libabbauw::properties::PHARMACYNOEAN.getName(), pharmacynoean);

					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCCustomerInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_PHARMACYNO_BY_BRANCHNO_PHARMCYNOEAN, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					basar::Int32 retun = 0;
					if ( !m_ACCCustomerInstRef.getPropertyTable().empty())
					{
						retun = m_ACCCustomerInstRef.getPropertyTable().begin().getInt32(libabbauw::properties::PHARMACYNO.getName());
					}
					return retun;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findPharmacynoByBranchnoPharmacynoean

			
			//*************************************** findPseudopharmacynoByBranchno *****************************************************
			basar::Int32 CustomerConsultantDM::findPseudopharmacynoByBranchno(const basar::Int16 branchno)
			{
				METHODNAME_DEF( CustomerConsultantDM, findPseudopharmacynoByBranchno )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::Int32 ret = 0;

				if ( 0 >= branchno )
				{
					std::stringstream str;
					str << fun << " Cannot acquire pseudopharmacyno: Branchno not set" << std::endl;
					//throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str()); 

					return ret;
				}

				try {	
					m_ACCCustomerInstRef.getPropertyTable().clear();					
					m_ACCCustomerInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCCustomerInstRef.getPropertyTable().begin();
					yitSearch.setInt16 (libabbauw::properties::BRANCHNO.getName(),		branchno);


					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCCustomerInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_PSEUDOPHARMACYNO_BY_BRANCHNO, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					basar::Int32 retun = 0;
					if ( !m_ACCCustomerInstRef.getPropertyTable().empty())
					{
						retun = m_ACCCustomerInstRef.getPropertyTable().begin().getInt32(libabbauw::properties::PHARMACYNO.getName());
					}
					return retun;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findPseudopharmacynoByBranchno



			//*************************************** checkExistenceByBranchnoPharmacyno *****************************************************
			bool		CustomerConsultantDM::checkExistenceByBranchnoPharmacyno(const basar::Int16 branchno, const basar::Int32 pharmacyno)
			{
				METHODNAME_DEF( CustomerConsultantDM, checkExistenceByBranchnoPharmacyno )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				m_ACCCustomerInstRef.getPropertyTable().clear();					
				m_ACCCustomerInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
				AccessorPropertyTable_YIterator yitSearch = m_ACCCustomerInstRef.getPropertyTable().begin();
				yitSearch.setInt16(libabbauw::properties::BRANCHNO.getName(), branchno);
				yitSearch.setInt32(libabbauw::properties::PHARMACYNO.getName(), pharmacyno);

				try {		
					// call execute if your connection has been established!	
					// Use an existing accessor, we are only interested in getting a row back or not
					ExecuteResultInfo	resInfo  
						= m_ACCCustomerInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_CUSTOMERGROUP_BY_BRANCHNO_PHARMCYNO, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<" SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					if ( m_ACCCustomerInstRef.getPropertyTable().empty())
					{
						// The given pharmacyno is not defined for the given branchno
						return false;
					}
					else
					{
						// The given pharmacyno exists in the given branchno
						return true;
					}

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
					throw; 
				}// catch
			}// checkExistenceByBranchnoPharmacyno




		} // namespace CustomerConsultantDM
	} // namespace libAbbauw
} // namespace domMod

