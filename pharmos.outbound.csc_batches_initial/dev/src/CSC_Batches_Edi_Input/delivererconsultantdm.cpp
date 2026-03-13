//-------------------------------------------------------------------------------------
/*! \file  delivererconsultantdm.cpp
*  \brief  Domain module for database operations regarding deliverers
*  \author P. Kessling
*  \date   25.11.14, 
*  history 25.11.14  pk new, similar to customerconsultantdm.cpp
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
#include "delivererconsultantdm.h"

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
		namespace delivererConsultantDM
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
			DelivererConsultantDM::DelivererConsultantDM() : m_Initialized(0) 
			{
				METHODNAME_DEF( DelivererConsultantDM, DelivererConsultantDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}

			DelivererConsultantDM::~DelivererConsultantDM() 
			{
				METHODNAME_DEF( DelivererConsultantDM, ~DelivererConsultantDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}


			//**************************************************************************************************
			//         Methoden
			//**************************************************************************************************
			//*************************************** init *****************************************************
			void DelivererConsultantDM::init(const ConnectionRef activeConn) 
			{
				METHODNAME_DEF( DelivererConsultantDM, init )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


				try {		

					if(0 == m_Initialized) {
						m_ACCDelivererInstRef 
							  = basar::db::aspect::Manager::getInstance().createAccessorInstance(libabbauw::domMod::ACC_INST_DELIVERER, libabbauw::domMod::ACC_DELIVERER, activeConn, basar::db::aspect::FULL_CACHING);  
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
			void DelivererConsultantDM::shutdown() 
			{
				METHODNAME_DEF( DelivererConsultantDM, shutdown )  
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

						if(!m_ACCDelivererInstRef.getPropertyTable().isNull())
						{
							m_ACCDelivererInstRef.getPropertyTable().clear();
						}
						m_ACCDelivererInstRef.reset();

					}// if(0==m_Initialized)

				}
				catch(...) {
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun );

						std::stringstream str;
						str << fun <<" Unknown exception " << std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

			}// shutdown



			//*************************************** findBranchnoByDelivererbga *****************************************************
			basar::Int16 DelivererConsultantDM::findBranchnoByDelivererbga(const basar::I18nString delivererbga)
			{
				METHODNAME_DEF( DelivererConsultantDM, findBranchnoByDelivererbga )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::Int16 ret = 0;

				if ( 0 >= delivererbga.length() )
				{
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Cannot acquire branchno: Delivererbga not set" << std::endl;

					return ret;
				}

				try {	
					m_ACCDelivererInstRef.getPropertyTable().clear();					
					m_ACCDelivererInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCDelivererInstRef.getPropertyTable().begin();
					yitSearch.setString(libabbauw::properties::DELIVERERBGA.getName(), delivererbga);

					// call execute if your connection has been established!	
					ExecuteResultInfo	resInfo  
						= m_ACCDelivererInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_BRANCHNO_BY_DELIVERERBGA, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					ret = 0;
					if ( !m_ACCDelivererInstRef.getPropertyTable().empty())
					{
						ret = m_ACCDelivererInstRef.getPropertyTable().begin().getInt16(libabbauw::properties::BRANCHNO.getName());
					}
					return ret;

				}// try
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findBranchnoByDelivererbga



		} // namespace CustomerConsultantDM
	} // namespace libAbbauw
} // namespace domMod

