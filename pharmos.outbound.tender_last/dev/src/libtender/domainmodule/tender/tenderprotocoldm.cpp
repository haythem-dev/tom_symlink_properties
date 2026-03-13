#include "tenderprotocoldm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/tender/tenderprotocolacc_definitions.h>

#include <libbasardbaspect.h>
#include <libutil/exception.h>

namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            using basar::db::aspect::AccessorPropertyTable_YIterator;
            using basar::db::aspect::AccessorPropertyTableRef;
            using libtender::LoggerPool;
        }
    }
}

namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            const libutil::misc::ClassInfo& TenderProtocolDM::getClassInfo()
            {
                static const libutil::misc::ClassInfo info( "TenderProtocolDM", libutil::misc::Version( 1, 0, 0, 0 ) );
                return info;
            }

            TenderProtocolDM::TenderProtocolDM()
            {
                METHODNAME_DEF( TenderProtocolDM, TenderProtocolDM )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
            }

            TenderProtocolDM::~TenderProtocolDM()
            {
                METHODNAME_DEF( TenderProtocolDM, ~TenderProtocolDM )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
            }

            void TenderProtocolDM::injectTenderProtocolAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
            {
                METHODNAME_DEF( TenderProtocolDM, injectTenderProtocolAccessor )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                m_Accessor = accessor;
            }

            libutil::infrastructure::accessor::IAccessorPtr TenderProtocolDM::getTenderProtocolAccessor() const
            {
                CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
                return m_Accessor;
            }

			//void TenderProtocolDM::findByTenderBG( AccessorPropertyTable_YIterator yitSearch )
			//{
			//	METHODNAME_DEF( TenderProtocolDM, findByTenderBG )
            //    BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
			//
			//	using libtender::infrastructure::accessor::tenderprotocol::lit::SELECT_TENDERPROTOCOL_BY_TENDER_BG;
			//	
            //    basar::db::aspect::ExecuteResultInfo executeResultInfo;
            //    try
			//	{
            //        executeResultInfo = m_Accessor->execute( SELECT_TENDERPROTOCOL_BY_TENDER_BG, yitSearch );
            //    }
            //    catch( basar::Exception & e  )
            //    {
            //        libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_TENDERPROTOCOL_BY_TENDER_BG, __FILE__, __LINE__ );
            //        throw;
            //    }
			//	if( executeResultInfo.hasError() )
            //    {
            //        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDERPROTOCOL_BY_TENDER_BG, executeResultInfo )
            //    }
			//}

			void TenderProtocolDM::saveProtocol()
			{
				METHODNAME_DEF( TenderProtocolDM, saveProtocolBG )
				BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
			
				using libtender::infrastructure::accessor::tenderprotocol::lit::INSERT_TENDERPROTOCOL;
				
				basar::db::aspect::ExecuteResultInfo executeResultInfo;
				try
				{
					basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin();
					for( ; !yit.isEnd(); ++yit )
					{
						if( !isSavable( yit ) )
						{
							continue;
						}

						executeResultInfo = getTenderProtocolAccessor()->execute( INSERT_TENDERPROTOCOL, yit );
					}
				}
				catch( basar::Exception & e )
				{
					libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, INSERT_TENDERPROTOCOL, __FILE__, __LINE__ );
					throw;
				}
				
				if( executeResultInfo.hasError() )
				{
					THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, INSERT_TENDERPROTOCOL, executeResultInfo );
				}
			}

			bool TenderProtocolDM::isSavable( basar::db::aspect::AccessorPropertyTable_YIterator yit ) const
			{
				METHODNAME_DEF( TenderProtocolDM, isSavable )
				BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

				bool isSavable = false;

				//checkNumberHeader();
				isSavable |= hasSavableStates( yit );

				return isSavable;
			}

			bool TenderProtocolDM::hasSavableStates( basar::db::aspect::AccessorPropertyTable_YIterator yit ) const
			{
				METHODNAME_DEF( TenderProtocolDM, hasSavableStates )
				BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
			
				using namespace basar::property;
			
				PropertyStateSet pss;
			
				pss.insert( basar::SS_INSERT );
				pss.insert( basar::SS_UPDATE );
				pss.insert( basar::SS_DELETE );
			
				if( yit.getState().contains( pss ) )
				{
					return true;
				}
			
				return false ;
			}

			void TenderProtocolDM::checkNumberHeader() const
			{
				METHODNAME_DEF( TenderProtocolDM, checkNumberHeader )
				BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
			
				if( 1 != getTenderProtocolAccessor()->getPropertyTable().size() )
				{
					basar::VarString msg( "There's none or more than one headers!" );
					throw libutil::exceptions::InconsistentDataException( basar::ExceptInfo( "TenderHeader::checkNumberHeader()", msg, __FILE__, __LINE__ ) );
				}
			}

            basar::db::aspect::AccessorPropertyTableRef TenderProtocolDM::get() const
            {
                METHODNAME_DEF( TenderProtocolDM, get )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                return m_Accessor->getPropertyTable();
            }

            void TenderProtocolDM::resetSearchYit()
            {
                METHODNAME_DEF( TenderProtocolDM, resetSearchYit )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

                getSearchYIterator()->reset();
            }

            AccessorPropertyTable_YIterator TenderProtocolDM::getSearchYit() const
            {
                METHODNAME_DEF( TenderProtocolDM, getSearchYit )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                return getSearchYIterator()->get();
            }

            libutil::domMod::SearchYIteratorPtr TenderProtocolDM::getSearchYIterator() const
            {
                METHODNAME_DEF( TenderProtocolDM, getSearchYIterator )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                if( NULL == m_SearchYIterator.get() )
                {
                    m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
                        new libutil::domMod::SearchYIterator( properties::TENDER_NO.toDescriptionString() +
															  properties::SHOW_INVOICED_ORDERS.toDescriptionString() 
                                                            )
                    );
                }

                return m_SearchYIterator;
            }

            bool TenderProtocolDM::isEmpty() const
            {
                METHODNAME_DEF( TenderProtocolDM, isEmpty )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                if( 0 == getTenderProtocolAccessor().get() || m_Accessor->getPropertyTable().isNull() ||
                    m_Accessor->getPropertyTable().isEmpty() )
                {
                    return true;
                }

                return false;
            }

        } // namespace tender
    }	  // namespace domMod
}		  // namespace libtender
