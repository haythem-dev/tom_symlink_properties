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

#include "loggerpool.h"
#include "abbauwexception.h"
#include "unprocessableexception.h"
#include "dataacquisitionexception.h"
#include "orderprotocolexception.h"

#include "edi_input_properties_definitions.h"
#include "edi_input_accessor_definitions.h"
#include "orderposcollectionprotocoldm.h"

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
			OrderPosCollectionProtocolDM::OrderPosCollectionProtocolDM() 
                : m_Initialized(0) 
			{
				METHODNAME_DEF( OrderPosCollectionProtocolDM, OrderPosCollectionProtocolDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}

			OrderPosCollectionProtocolDM::~OrderPosCollectionProtocolDM() 
			{
				METHODNAME_DEF( OrderPosCollectionProtocolDM, ~OrderPosCollectionProtocolDM )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
			}

			void OrderPosCollectionProtocolDM::init(const ConnectionRef activeConn) 
			{
				METHODNAME_DEF( OrderPosCollectionProtocolDM, init )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

				try 
                {	
				    if(0 == m_Initialized) 
                    {
				    		m_Accessor = basar::db::aspect::Manager::getInstance().createAccessorInstance(libabbauw::domMod::ACC_INST_ORDERPOSCOLLECTIONPROTOCOL, 
                                                                                                     libabbauw::domMod::ACC_ORDERPOSCOLLECTIONPROTOCOL, activeConn, 
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

			void OrderPosCollectionProtocolDM::shutdown() 
			{
				METHODNAME_DEF( OrderPosCollectionProtocolDM, shutdown )  
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

            void OrderPosCollectionProtocolDM::protocolCreateOrder(basar::db::aspect::AccessorPropertyTable_YIterator yitPos)
            {
                METHODNAME_DEF( OrderPosCollectionProtocolDM, protocolCreateOrder )  
				BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

                try 
                {	
					ExecuteResultInfo resInfo = m_Accessor.execute(libabbauw::domMod::ACC_ORDERPOSCOLLECTIONPROTOCOL_SAVE, yitPos);
                    
					if( resInfo.hasError() )
                    {
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, fun + ( resInfo.getError() ) );
                    
						std::stringstream str;
						str << fun <<"SQL execution has errors: " << resInfo.m_error<< std::endl;

						throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
					}
				}
                catch(const basar::Exception& e) 
                {
					e.what();
                   

					throw;
				}
                catch(...) 
                {
					std::stringstream str;
                    str << fun << "Error during saving order pos protocol" << std::endl;
                    BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

					throw libabbauw::DataAcquisitionException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
				}
            }

			basar::db::aspect::AccessorPropertyTable_YIterator OrderPosCollectionProtocolDM::getPos()
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
