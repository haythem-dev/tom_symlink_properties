#include "cmndm.h"

#include "transferorderheadinpreparation.h"
#include "transferorderheadinpreparation_definitions.h"

#include "transferorderheadinpreparationpropertystatechecker.h"
#include "transferorderheadinpreparationacc.h"

#include "dmbase.h"

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

const libutil::misc::ClassInfo& TransferOrderHeadInPreparation::getClassInfo()
{
	static const libutil::misc::ClassInfo info( "transferorderheadinpreparationndm::TransferOrderHeadInPreparation", libutil::misc::Version( 1, 0, 0, 0 ) );
	return info;
}

TransferOrderHeadInPreparation::TransferOrderHeadInPreparation( basar::db::aspect::ConnectionRef connection )
: DMBase( connection, domMod::transferOrder::ACC_TRANSFER_ORDER_HEAD_INPREPARATION, basar::db::aspect::FULL_CACHING, LoggerPool::loggerTransferOrder )
{
	METHODNAME_DEF( TransferOrderHeadInPreparation, TransferOrderHeadInPreparation )
	BLOG_TRACE_METHOD( getLogger(), fun );

	// avoid runtime error
	acc_transfer_order_in_preparation::SelectCurrentTransferOrderHeadInPreparation();
}

TransferOrderHeadInPreparation::~TransferOrderHeadInPreparation()
{
	METHODNAME_DEF( TransferOrderHeadInPreparation, ~TransferOrderHeadInPreparation )
	BLOG_TRACE_METHOD( getLogger(), fun );

    shutdown();
}

void TransferOrderHeadInPreparation::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr dbTransactionFactory )
{
    m_DBTransactionFactory = dbTransactionFactory;
}

AccessorExecuteParameterization TransferOrderHeadInPreparation::getFindByPatternExecuteParameters() const
{
	METHODNAME_DEF( TransferOrderHeadInPreparation, getFindByPatternExecuteParameters )
	BLOG_TRACE_METHOD( getLogger(), fun );

    return AccessorExecuteParameterization ( SELECT_CURRENT_TRANSFER_ORDER_HEAD_INPREPARATION );
}

basar::VarString TransferOrderHeadInPreparation::getSearchYIteratorAttributeNames()  const
{
	METHODNAME_DEF( TransferOrderHeadInPreparation, getSearchYIteratorAttributeNames )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using namespace libabbauw::properties;

	return libabbauw::properties::BRANCHNO.toDescriptionString()                   +
		   libabbauw::properties::USER.toDescriptionString()        
		   ;
}

void TransferOrderHeadInPreparation::doInit( const TransferOrderHeadInPreparationInitParams& p )
{
	METHODNAME_DEF( TransferOrderHeadInPreparation, doInit )
	BLOG_TRACE_METHOD( getLogger(), fun );

	m_InitParams = p;
}

void TransferOrderHeadInPreparation::doShutdown()
{
	METHODNAME_DEF( TransferOrderHeadInPreparation, doShutdown )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void TransferOrderHeadInPreparation::resetSearchYit()
{
	DMBase::resetSearchYit();
}

basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderHeadInPreparation::get()
{
    // get current transfer order the user is working with or create new one if not existing
    basar::db::aspect::AccessorPropertyTable_YIterator yit;
    if( ! DMBase::getPropTab().isEmpty() )
    {
        yit = DMBase::getPropTab().begin();
    }
    else
    {
        yit =  DMBase::getPropTab().insert( basar::FOR_INSERT );
        
        yit.setInt16( libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo );
        yit.setString( libabbauw::properties::USER,    m_InitParams.m_UserName );
    }

    return yit;
}

basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderHeadInPreparation::getSearchYit()
{
	return DMBase::getSearchYit();
}

void TransferOrderHeadInPreparation::findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator yIt )
{
    METHODNAME_DEF( TransferOrderHeadInPreparation, findByPattern )
    BLOG_TRACE_METHOD( getLogger(), fun );

    basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = yIt;
    yitSearchTmp.setInt16(  libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo );
    yitSearchTmp.setString( libabbauw::properties::USER,     m_InitParams.m_UserName );

    DMBase::findByPattern( yitSearchTmp );
}

bool TransferOrderHeadInPreparation::isComplete()
{
    accessor::transferOrder::HeadInPreparationPropertyStateChecker checker( get() );
    return checker.isComplete();
}

bool TransferOrderHeadInPreparation::isChanged()
{
    accessor::transferOrder::HeadInPreparationPropertyStateChecker checker( get() );
    return checker.isSavable();
 }

bool TransferOrderHeadInPreparation::isDeleted()
{
    accessor::transferOrder::HeadInPreparationPropertyStateChecker checker( get() );
    return checker.isDeletable();
}

bool TransferOrderHeadInPreparation::containsSavableData()
{
    accessor::transferOrder::HeadInPreparationPropertyStateChecker checker( get() );
    return checker.isInsertable() || checker.isUpdatable();
}

void TransferOrderHeadInPreparation::remove()
{
    METHODNAME_DEF( TransferOrderHeadInPreparation, remove )
	BLOG_TRACE_METHOD( getLogger(), fun );

    if( ! isDeleted() )
    {
        return;
    }

    try 
	{
        clearDeleteMark();

        basar::db::aspect::ExecuteResultInfo res = getAccessor()->execute( DELETE_TRANSFER_ORDER_HEAD_INPREPARATION, get() );
        if( res.hasError() ) 
        {
		    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, DELETE_TRANSFER_ORDER_HEAD_INPREPARATION, res );
	    }

        markForInsert();

    } catch( basar::Exception & e ) 
	{
		getLogger().forcedLog( log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__ );
		throw;
    }
}

void TransferOrderHeadInPreparation::save()
{
    METHODNAME_DEF( TransferOrderHeadInPreparation, save )
	BLOG_TRACE_METHOD( getLogger(), fun );

    if( ! isChanged() )
    {
        return;
    }

	libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction;

	try 
	{
        transaction = m_DBTransactionFactory->createDBTransaction();
        transaction->begin();

        remove(); // first delete old dataset if deletable
        if( containsSavableData() )
        {
            basar::db::aspect::ExecuteResultInfo res = getAccessor()->execute( SAVE_TRANSFER_ORDER_HEAD_INPREPARATION, get() );
            if( res.hasError() ) 
            {
			    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_TRANSFER_ORDER_HEAD_INPREPARATION, res );
	        }
        }

        transaction->commit();
	} 
	catch( basar::Exception & e ) 
	{
		transaction->rollback();

		getLogger().forcedLog( log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__ );
		throw;
    }
}

void TransferOrderHeadInPreparation::clearDeleteMark()
{
    METHODNAME_DEF( TransferOrderHeadInPreparation, clearDeleteMark )
	BLOG_TRACE_METHOD( getLogger(), fun );

    // Basar HACK: remove all SS_DELETED properties from yit via setting branchno/user to avoid yit gets deleted after executing SQL
    if( isDeleted() )
    {
        get().setPropertyState( properties::BRANCHNO, basar::SS_UNSET );
        get().setInt16(         properties::BRANCHNO, m_InitParams.m_BranchNo );
        get().setPropertyState( properties::USER,     basar::SS_UNSET );
        get().setString(        properties::USER,     m_InitParams.m_UserName );
    }
}

void TransferOrderHeadInPreparation::markAsDeleted()
{
    METHODNAME_DEF( TransferOrderHeadInPreparation, markAsDeleted )
	BLOG_TRACE_METHOD( getLogger(), fun );

    // first empty all values in yit to avoid matcher matches them
    for( basar::db::aspect::AccessorPropertyTable_XIterator xit = get().begin(); xit != get().end(); ++xit )
    {
        xit.setPropertyState( basar::SS_UNSET );
    }

    // next restore branchno and user and mark yit as deleted
    get().setInt16(  properties::BRANCHNO, m_InitParams.m_BranchNo );
    get().setPropertyState( properties::BRANCHNO, basar::SS_DELETE );
    get().setString( properties::USER,     m_InitParams.m_UserName );
    get().setPropertyState( properties::USER, basar::SS_DELETE );
}

void TransferOrderHeadInPreparation::markForInsert()
{
    for( basar::db::aspect::AccessorPropertyTable_XIterator xit = get().begin(); xit != get().end(); ++xit )
    {
        if( xit.getState() != basar::SS_UNSET )       // unset states can't be changed
            xit.setPropertyState( basar::SS_INSERT );
    }
}

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw
