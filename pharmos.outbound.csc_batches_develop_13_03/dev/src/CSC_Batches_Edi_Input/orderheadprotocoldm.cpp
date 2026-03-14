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
#include "orderheadprotocoldm.h"

using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::cmnutil::ParameterList;
using basar::db::aspect::ExecuteResultInfo;


namespace domMod
{
	namespace libAbbauw
	{
		namespace orderProtocol
		{
			OrderHeadProtocolDM::OrderHeadProtocolDM() 
                : m_Initialized(0) 
			{
				METHODNAME_DEF( OrderHeadProtocolDM, OrderHeadProtocolDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}

			OrderHeadProtocolDM::~OrderHeadProtocolDM() 
			{
				METHODNAME_DEF( OrderHeadProtocolDM, ~OrderHeadProtocolDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}

			void OrderHeadProtocolDM::init(const ConnectionRef activeConn) 
			{
				METHODNAME_DEF( OrderHeadProtocolDM, init )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try 
                {	
				    if(0 == m_Initialized) 
                    {
				    		m_Accessor = basar::db::aspect::Manager::getInstance().createAccessorInstance(libabbauw::domMod::ACC_INST_ORDERHEADPROTOCOL, 
                                                                                                          libabbauw::domMod::ACC_ORDERHEADPROTOCOL, activeConn, 
                                                                                                          basar::db::aspect::FULL_CACHING);  
				    }

					++m_Initialized;
				}
				catch(...) 
                {
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 

					std::stringstream str;
					str << fun << "createAccessorInstance failed!" << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}
			}

			void OrderHeadProtocolDM::shutdown() 
			{
				METHODNAME_DEF( OrderHeadProtocolDM, shutdown )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try 
                {		
					if(0 == m_Initialized)
					{
						return;
					}			

					--m_Initialized;

					if(0==m_Initialized) 
                    {
						if(!m_Accessor.getPropertyTable().isNull())
						{
							m_Accessor.getPropertyTable().clear();
						}

						m_Accessor.reset();
					}

				}
				catch(...) 
                {
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun );

					std::stringstream str;
					str << fun <<" Unknown exception " << std::endl;
					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}

			}

            void OrderHeadProtocolDM::protocolCreateOrder(const basar::db::aspect::AccessorPropertyTable_YIterator yitHead)
            {
                METHODNAME_DEF( OrderHeadProtocolDM, protocolCreateOrder )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

                try 
                {	
					// call execute if your connection has been established!	
					ExecuteResultInfo resInfo = m_Accessor.execute(libabbauw::domMod::ACC_ORDERHEADPROTOCOL_SAVE, yitHead);
                    
					if( resInfo.hasError() )
                    {
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + ( resInfo.getError() ) );
                    
						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;

				        throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}
				}
                catch(...) 
                {
					std::stringstream str;
                    str << fun << "Error during saving order head protocol" << std::endl;
                    BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}
            }

            basar::db::aspect::AccessorPropertyTable_YIterator OrderHeadProtocolDM::getHead()
            {
                m_Accessor.getPropertyTable().clear();

                if( m_Accessor.getPropertyTable().isNull() || m_Accessor.getPropertyTable().isEmpty() )
                {
                    m_Accessor.getPropertyTable().insert(basar::FOR_INSERT);
                }

                return m_Accessor.getPropertyTable().begin();
            }

		} // namespace orderProtocol
	} // namespace libAbbauw
} // namespace domMod
