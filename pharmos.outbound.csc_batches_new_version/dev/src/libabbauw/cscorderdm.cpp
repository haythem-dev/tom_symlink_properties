#include "cmndm.h"

#include "cscorderdm.h"
#include "orderstatus.h"
#include "libabbauw_definitions.h"

#include "iordergetter.h"
#include "icscordercreator.h"

#include "exceptions/orderreleaseexception.h"

namespace libabbauw
{
namespace domMod
{
namespace cscorder
{

    const libutil::misc::ClassInfo& CSCOrderDM::getClassInfo()
    {
	    static const libutil::misc::ClassInfo info( "cscorder::CSCOrderDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	    return info;
    }

    CSCOrderDM::CSCOrderDM( basar::db::aspect::ConnectionRef )
        : m_Logger( libabbauw::LoggerPool::loggerCSCOrder ), m_isCreatorInitialized( false )
    {
	    METHODNAME_DEF( CSCOrderDM, CSCOrderDM )
	    BLOG_TRACE_METHOD( getLogger(), fun );
    }

    CSCOrderDM::~CSCOrderDM()
    {
	    METHODNAME_DEF( CSCOrderDM, ~CSCOrderDM )
	    BLOG_TRACE_METHOD( getLogger(), fun );

        shutdown();
    }

    void CSCOrderDM::injectCSCOrderCreator( ICSCOrderCreatorPtr cscOrderCreator )
    {
        m_CSCOrderCreator = cscOrderCreator;
    }

    void CSCOrderDM::injectOrderGetter( IOrderGetterPtr orderGetter )
    {
        m_OrderGetter = orderGetter;
    }

    void CSCOrderDM::doInit( const CSCOrderInitParams& p )
    {
        METHODNAME_DEF( CSCOrderDM, doInit )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_InitParams = p;
    }

    void CSCOrderDM::doShutdown()
    {
        METHODNAME_DEF( CSCOrderDM, doShutdown )
        BLOG_TRACE_METHOD( getLogger(), fun );

        if( m_isCreatorInitialized )
        {
            m_CSCOrderCreator->shutdown();
            m_isCreatorInitialized = false;
        }
    }

    void CSCOrderDM::checkForCorrectOrderStatus( const basar::db::aspect::AccessorPropertyTable_YIterator order ) const
    {
        METHODNAME_DEF( CSCOrderDM, checkForCorrectOrderStatus );

        OrderStatus orderStatus( order.getString( properties::STATUS ) );
        if( orderStatus.getStatus() != OrderStatus::ZU )
        {
            QString status = QString::fromLocal8Bit( orderStatus.toString().c_str() );
            QString warn = tr( "Order '%1' has wrong status '%2' and therefore will not be released!" );
            warn = warn.arg( order.getInt32( properties::ORDERNO ) ).arg( status );

            basar::ExceptInfo info( fun, warn.toLocal8Bit().constData(), __FILE__, __LINE__ );
            throw exceptions::OrderReleaseException( info, exceptions::OrderReleaseException::WRONG_ORDER_STATUS );
        }
    }

    void CSCOrderDM::release( const bool postpone )
    {
	    METHODNAME_DEF( CSCOrderDM, release )
	    BLOG_TRACE_METHOD( getLogger(), fun );

        basar::db::aspect::AccessorPropertyTable_YIterator yitOrder = m_OrderGetter->get();
        releaseOrder( yitOrder, postpone );
    }

    void CSCOrderDM::releaseOrder( basar::db::aspect::AccessorPropertyTable_YIterator yit, bool postpone )
    {
	    METHODNAME_DEF( CSCOrderDM, releaseOrder )
	    BLOG_TRACE_METHOD( getLogger(), fun );

        checkForCorrectOrderStatus( yit );

        if( ! m_isCreatorInitialized )
        {
            m_CSCOrderCreator->init();
            m_isCreatorInitialized = true;
        }

        basar::Int16 branchnoorig = 0;
        if( yit.isContainedAndSet( properties::BRANCHNO_ORIG ) )
        {
            branchnoorig = yit.getInt16( properties::BRANCHNO_ORIG );
        }

	    basar::appl::EventReturnType retType = m_CSCOrderCreator->createCSCOrderABBAUW( m_InitParams.m_BranchNo, 
                                                                                        branchnoorig,
		                                                                                yit.getInt32(  properties::ORDERNO             ),
		                                                                                yit.getInt32(  properties::PHARMACYNO          ),
                                                                                        yit.getInt32(  properties::PURCHASEORDERNO     ), //P_17000393
		                                                                                yit.getString( properties::DATASOURCETYPE      ), 
                                                                                        yit.getString( properties::ORDERTYPE           ), 
                                                                                        yit.getString( properties::HEADERTEXT          ),
		                                                                                yit.getDate(   properties::DEFERREDPAYMENTDATE ), 
                                                                                        yit.getDate(   properties::EDIDELIVERYDATE     ), 
                                                                                        postpone
                                                                                      );

		doShutdown();

	    if( ! retType.isOK() )
	    {
		    basar::ExceptInfo info( fun, retType.message, __FILE__, __LINE__ );
		    throw exceptions::OrderReleaseException( info );
	    }
    }

    const log4cplus::Logger& CSCOrderDM::getLogger() const
    {
        return m_Logger;
    }

} // end namespace cscorder
} // end namespace domMod
} // end namespace libabbauw
