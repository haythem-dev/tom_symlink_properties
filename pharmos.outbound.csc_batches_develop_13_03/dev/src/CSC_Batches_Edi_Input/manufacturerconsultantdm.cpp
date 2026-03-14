//-------------------------------------------------------------------------------------
/*! \file  manufacturerconsultantdm.cpp
*  \brief  Domain module for database operations regarding manufacturers
*  \author P. Kessling
*  \date   13.06.2013, 
*  history 13.06.13  pk new, similar to customerconsultantdm.cpp
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
#include "manufacturerconsultantdm.h"

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
		namespace manufacturerConsultantDM
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
			ManufacturerConsultantDM::ManufacturerConsultantDM() : m_Initialized(0) 
			{
				METHODNAME_DEF( ManufacturerConsultantDM, ManufacturerConsultantDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}

			ManufacturerConsultantDM::~ManufacturerConsultantDM() 
			{
				METHODNAME_DEF( ManufacturerConsultantDM, ~ManufacturerConsultantDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}


			//**************************************************************************************************
			//         Methoden
			//**************************************************************************************************
			//*************************************** init *****************************************************
			void ManufacturerConsultantDM::init(const ConnectionRef activeConn) 
			{
				METHODNAME_DEF( ManufacturerConsultantDM, init )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );


				try {		

					if(0 == m_Initialized) {
						m_ACCManufacturerInstRef 
							  = basar::db::aspect::Manager::getInstance().createAccessorInstance(libabbauw::domMod::ACC_INST_MANUFACTURER, libabbauw::domMod::ACC_MANUFACTURER, activeConn, basar::db::aspect::FULL_CACHING);  
					}

					++m_Initialized;
				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );	// basar-Exception-what() needs .c_str(), because it returns const VarString

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );		// std::exception does NOT need  .c_str(), because it returns const char *

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << "createAccessorInstance failed!" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// init

			//*************************************** shutdown *****************************************************
			void ManufacturerConsultantDM::shutdown() 
			{
				METHODNAME_DEF( ManufacturerConsultantDM, shutdown )  
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

						if(!m_ACCManufacturerInstRef.getPropertyTable().isNull())
						{
							m_ACCManufacturerInstRef.getPropertyTable().clear();
						}
						m_ACCManufacturerInstRef.reset();

					}// if(0==m_Initialized)

				}
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what());

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun );

						std::stringstream str;
						str << fun <<" Unknown exception " << std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

			}// shutdown




			//*************************************** findManufacturernoByManufacturerean *****************************************************
			// Returns: 0 if no manufacturernumber could be found 
			basar::Int32 ManufacturerConsultantDM::findManufacturernoByManufacturerean(const basar::I18nString manufacturerean)
			{
				METHODNAME_DEF( ManufacturerConsultantDM, findManufacturernoByManufacturerean )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::Int32 ret = 0;

				if ( 0 >= manufacturerean.length() )
				{
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Cannot acquire manufacturerno: manufacturerean not set" << std::endl;
					//throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );

					return ret;
				}

				try {	
					m_ACCManufacturerInstRef.getPropertyTable().clear();					
					m_ACCManufacturerInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCManufacturerInstRef.getPropertyTable().begin();

					// 1st part: Search for delivererno by manufacturerean  
					yitSearch.setString(libabbauw::properties::MANUFACTUREREAN.getName(), manufacturerean);

					ExecuteResultInfo	resInfo  
						= m_ACCManufacturerInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_DELIVERERNO_BY_MANUFACTUREREAN, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution first part has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					basar::Int32 delivererno = 0;
					if ( !m_ACCManufacturerInstRef.getPropertyTable().empty())
					{
						delivererno = m_ACCManufacturerInstRef.getPropertyTable().begin().getInt32(libabbauw::properties::DELIVERERNO.getName());
					}


					if (0 >= delivererno)
					{
						// could not find data
						return 0;
					}

					

					// 2nd part: Search for manufacturerno by delivererno
					// Watch out: We need a new synonym in outbound-DB for table liefer
					m_ACCManufacturerInstRef.getPropertyTable().clear();					
					m_ACCManufacturerInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch2 = m_ACCManufacturerInstRef.getPropertyTable().begin();

					yitSearch2.setInt32(libabbauw::properties::DELIVERERNO.getName(), delivererno);

					resInfo  
						= m_ACCManufacturerInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_MANUFACTURERNO_BY_DELIVERERNO, yitSearch2);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution second part has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}

					basar::Int32 retun = 0;
					if ( !m_ACCManufacturerInstRef.getPropertyTable().empty())
					{
						retun = m_ACCManufacturerInstRef.getPropertyTable().begin().getInt32(libabbauw::properties::MANUFACTURERNO.getName());
					}


					return retun;

				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findManufacturernoByManufacturerean


			//*************************************** findManufacturernameByManufacturerno *****************************************************
			// Returns: "" if no manufacturername could be found 
			basar::I18nString ManufacturerConsultantDM::findManufacturernameByManufacturerno(const basar::Int32 manufacturerno)
			{
				METHODNAME_DEF( ManufacturerConsultantDM, findManufacturernameByManufacturerno )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				basar::I18nString manufacturername("");

				if ( 0 >= manufacturerno )
				{
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					//std::stringstream str;
					//str << fun << " Cannot acquire manufacturername: manufacturerno not set" << std::endl;
					//throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );

					return manufacturername;
				}

				try {	
					m_ACCManufacturerInstRef.getPropertyTable().clear();					
					m_ACCManufacturerInstRef.getPropertyTable().insert(basar::FOR_UNKNOWN);		
					AccessorPropertyTable_YIterator yitSearch = m_ACCManufacturerInstRef.getPropertyTable().begin();

					// 1st part: Search for delivererno by manufacturerean  
					yitSearch.setInt32(libabbauw::properties::MANUFACTURERNO.getName(), manufacturerno);

					ExecuteResultInfo	resInfo  
						= m_ACCManufacturerInstRef.execute(libabbauw::domMod::ACCMETH_SELECT_MANUFACTURERNAME_BY_MANUFACTURERNO, yitSearch);

					if( resInfo.hasError() ){
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + (resInfo.getError()));

						std::stringstream str;
						str << fun <<"SQL execution first part has errors: " << resInfo.m_error<< std::endl;
						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}


					if ( !m_ACCManufacturerInstRef.getPropertyTable().empty())
					{
						manufacturername = m_ACCManufacturerInstRef.getPropertyTable().begin().getString(libabbauw::properties::MANUFACTURERNAME.getName());
					}

					return manufacturername;

				}// try
				catch (basar::Exception& e)
				{
						basar::VarString msg;
						msg.format( "Basar-Exception: <%s>", e.what().c_str() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch (std::exception& e)
				{
						basar::VarString msg;
						msg.format( "Exception: <%s>", e.what() );

						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, msg); 
						throw e;
				}
				catch(...) {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << " Unknown exception" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}// catch
			}// findManufacturernameByManufacturerno




		} // namespace ManufacturerConsultantDM
	} // namespace libAbbauw
} // namespace domMod

