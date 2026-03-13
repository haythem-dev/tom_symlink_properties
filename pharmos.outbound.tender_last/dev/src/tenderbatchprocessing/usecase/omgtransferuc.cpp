#include "omgtransferuc.h"
#include <loggerpool/loggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>
#include <libbasardbaspect_exceptions.h>

#include "domainmodule/omgsuppliercalc/iomgsuppliercalc.h"

#include "domainmodule/omgtransfer/omgtransferdm.h"
#include "domainmodule/omgtransfer/iomgtransfer.h"

namespace tenderbatchprocessing
{
namespace useCase
{
OMGTransferUC::OMGTransferUC()
: m_Logger( tenderbatchprocessing::LoggerPool::getLoggerUseCases() )
{
    METHODNAME_DEF( OMGTransferUC, OMGTransferUC )
    BLOG_TRACE_METHOD( getLogger(), fun );
}

OMGTransferUC::~OMGTransferUC()
{
    METHODNAME_DEF( OMGTransferUC, ~OMGTransferUC )
    BLOG_TRACE_METHOD( getLogger(), fun );
}

void OMGTransferUC::injectOMGSupplierCalcDM( tenderbatchprocessing::domMod::omgsuppliercalc::IOMGSupplierCalcPtr omgCalcDM )
{
    METHODNAME_DEF( OMGTransferUC, injectOMGSupplierCalcDM )
    BLOG_TRACE_METHOD( getLogger(), fun );

    m_OMGCalcDM = omgCalcDM;
}

void OMGTransferUC::injectOMGTransferDM( tenderbatchprocessing::domMod::omgtransfer::IOMGTransferPtr omgTransferDM )
{
    METHODNAME_DEF( OMGTransferUC, injectOMGTransferDM )
    BLOG_TRACE_METHOD( getLogger(), fun );

    m_OMGTransferDM = omgTransferDM;
}


void OMGTransferUC::run()
{
    METHODNAME_DEF( OMGTransferUC, run )
    BLOG_TRACE_METHOD( getLogger(), fun );

    m_OMGCalcDM->findForTransfer();
	basar::db::aspect::AccessorPropertyTable_YIterator yitStart = m_OMGCalcDM->get().begin();
	basar::db::aspect::AccessorPropertyTable_YIterator yitEnd = yitStart;
	basar::Int32 counter = 0;
	basar::Int32 maxPositionCount = m_OMGTransferDM->getMaxPositionsCount();
	do
	{
		while(	  false == yitEnd.isEnd() 
			   && maxPositionCount > counter )
		{
			++counter;
			++yitEnd;
		}

		if( 0 < counter )
		{
			m_OMGTransferDM->addPositions( yitStart, yitEnd );
			try
			{
				m_OMGTransferDM->save();
				m_OMGCalcDM->markAsTransferred( yitStart, yitEnd );
			}
			catch( const basar::Exception & e )
			{
				BLOG_ERROR( getLogger(), e.what().c_str() );
			}
			if( m_OMGTransferDM->hasError() )
			{
			}
			counter = 0;
			yitStart = yitEnd;
		}
	}
	while( false == yitEnd.isEnd() );
}

const log4cplus::Logger OMGTransferUC::getLogger() const
{
    METHODNAME_DEF( OMGTransferUC, getLogger )
    BLOG_TRACE_METHOD( m_Logger, fun );

    return m_Logger;
}

} // namespace usecase
} // namespace tender
