#include "cmndm.h"

#include "transferorderposinpreparationcollection.h"
#include "transferorderposinpreparationcollection_definitions.h"

#include "transferorderposinpreparationcollectionpropertystatechecker.h"
#include "transferorderposinpreparationcollectionacc.h"

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

const libutil::misc::ClassInfo& TransferOrderPosInPreparationCollection::getClassInfo()
{
	static const libutil::misc::ClassInfo info( "transferorderposinpreparationcollectionndm::TransferOrderPosInPreparationCollection", libutil::misc::Version( 1, 0, 0, 0 ) );
	return info;
}

TransferOrderPosInPreparationCollection::TransferOrderPosInPreparationCollection( basar::db::aspect::ConnectionRef connection )
: DMBase( connection, domMod::transferOrder::ACC_TRANSFER_ORDER_POS_IN_PREPARATION_COLLECTION, basar::db::aspect::FULL_CACHING, libabbauw::LoggerPool::loggerTransferOrder ),
  m_NumberOfPositions( 0 )
{
	METHODNAME_DEF( TransferOrderPosInPreparationCollection, TransferOrderPosInPreparationCollection )
	BLOG_TRACE_METHOD( getLogger(), fun );

	// avoid runtime error
	
	acc_transfer_order_in_preparation::SelectCurrentTransferOrderPosInPreparationCollection();
}

TransferOrderPosInPreparationCollection::~TransferOrderPosInPreparationCollection()
{
	METHODNAME_DEF( TransferOrderPosInPreparationCollection, ~TransferOrderPosInPreparationCollection )
	BLOG_TRACE_METHOD( getLogger(), fun );

    shutdown();
}

AccessorExecuteParameterization TransferOrderPosInPreparationCollection::getFindByPatternExecuteParameters() const
{
	METHODNAME_DEF( TransferOrderPosInPreparationCollection, getFindByPatternExecuteParameters )
	BLOG_TRACE_METHOD( getLogger(), fun );

    return AccessorExecuteParameterization ( SELECT_CURRENT_TRANSFER_ORDER_POS_IN_PREPARATION_COLLECTION );
}

basar::VarString TransferOrderPosInPreparationCollection::getSearchYIteratorAttributeNames()  const
{
	METHODNAME_DEF( TransferOrderPosInPreparationCollection, getSearchYIteratorAttributeNames )
	BLOG_TRACE_METHOD( getLogger(), fun );

	using namespace libabbauw::properties;

	return libabbauw::properties::BRANCHNO.toDescriptionString()  +
		   libabbauw::properties::USER.toDescriptionString()        
		   ;
}

void TransferOrderPosInPreparationCollection::doInit( const TransferOrderPosInPreparationCollectionInitParams& p )
{
    METHODNAME_DEF( TransferOrderPosInPreparationCollection, doInit )
	BLOG_TRACE_METHOD( getLogger(), fun );

	m_InitParams = p;
}

void TransferOrderPosInPreparationCollection::doShutdown()
{
	METHODNAME_DEF( TransferOrderPosInPreparationCollection, doShutdown )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void TransferOrderPosInPreparationCollection::resetSearchYit()
{
	DMBase::resetSearchYit();
}

basar::db::aspect::AccessorPropertyTableRef TransferOrderPosInPreparationCollection::get()
{
    return DMBase::getPropTab();
}

basar::db::aspect::AccessorPropertyTableRef TransferOrderPosInPreparationCollection::get() const
{
    return DMBase::getPropTab();
}

basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderPosInPreparationCollection::getSearchYit()
{
	return DMBase::getSearchYit();
}

void TransferOrderPosInPreparationCollection::findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator yIt )
{
	METHODNAME_DEF( TransferOrderPosInPreparationCollection, findByPattern )
	BLOG_TRACE_METHOD( getLogger(), fun );

	basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = yIt;
	yitSearchTmp.setInt16(  libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo );
    yitSearchTmp.setString( libabbauw::properties::USER,     m_InitParams.m_UserName );

	DMBase::findByPattern( yitSearchTmp );
    m_NumberOfPositions = static_cast<basar::Int32>(getPropTab().size());
}

bool TransferOrderPosInPreparationCollection::isEmpty() const
{
    bool empty = true;
    for( basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin(); !yit.isEnd(); ++ yit)
    {
        accessor::transferOrder::PosInPreparationCollectionPropertyStateChecker checker( yit );
        if( ! yit.getState().contains( basar::SS_DELETE ) )
            empty = false;
    }

    return empty;
}

bool TransferOrderPosInPreparationCollection::isChanged() const
{
    bool changed = false;
    for( basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin(); !yit.isEnd() && !changed; ++ yit)
    {
        accessor::transferOrder::PosInPreparationCollectionPropertyStateChecker checker( yit );
        if( checker.isSavable() )
            changed = true;
    }

    return changed;
}

void TransferOrderPosInPreparationCollection::save()
{
    METHODNAME_DEF( TransferOrderPosInPreparationCollection, save )
	BLOG_TRACE_METHOD( getLogger(), fun );

    // basar bug: list iterator is not dereferenceable?
    // delete is not possible via normal for-loop
    // first, delete all datasets which are marked as deleteable
    for( basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin( basar::SS_DELETE ); !yit.isEnd(); )
    {
        accessor::transferOrder::PosInPreparationCollectionPropertyStateChecker stateChecker( yit );
        if( stateChecker.isDeletable() )
        {
	        try
            {
		        basar::db::aspect::ExecuteResultInfo res = getAccessor()->execute( SAVE_TRANSFER_ORDER_POS_IN_PREPARATION_COLLECTION, yit );
    	        if( res.hasError() ) 
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_TRANSFER_ORDER_POS_IN_PREPARATION_COLLECTION, res );
	            }
	        } 
            catch( basar::Exception & e )
            {
		        getLogger().forcedLog( log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__ );
		        throw;
            }

            yit = get().begin( basar::SS_DELETE );
        }
        else
        {
            ++yit;
        }
    }

    // update or insert rest
    for( basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin(); !yit.isEnd(); ++yit )
    {
        accessor::transferOrder::PosInPreparationCollectionPropertyStateChecker stateChecker( yit );
        if( stateChecker.isSavable() )
        {
	        try
            {
		        basar::db::aspect::ExecuteResultInfo res = getAccessor()->execute( SAVE_TRANSFER_ORDER_POS_IN_PREPARATION_COLLECTION, yit );
    	        if( res.hasError() ) 
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_TRANSFER_ORDER_POS_IN_PREPARATION_COLLECTION, res );
                }
                // keep posno and posno_orig in sync
                yit.setInt32( properties::POSNO_ORIG, yit.getInt32( properties::POSNO ) );
	        }
            catch( basar::Exception & e )
            {
		        getLogger().forcedLog( log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__ );
		        throw;
            }
        }
    }
}

basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderPosInPreparationCollection::addEmptyPosition()
{
    basar::db::aspect::AccessorPropertyTable_YIterator posYit = getPropTab().insert( basar::FOR_INSERT );

    ++m_NumberOfPositions;

    posYit.setInt16(   properties::BRANCHNO,      m_InitParams.m_BranchNo );
    posYit.setString(  properties::USER,          m_InitParams.m_UserName );
    posYit.setInt32(   properties::POSNO,         m_NumberOfPositions     );
    posYit.setInt32(   properties::POSNO_ORIG,    m_NumberOfPositions     );
    posYit.setDecimal( properties::DISCOUNTPCT,   basar::Decimal( 0.0 )   );
    posYit.setInt32(   properties::ORDEREDQTY,    0                       );
    posYit.setInt32(   properties::NONCHARGEDQTY, 0                       );
    posYit.setInt16(   properties::DISCOUNTAPPLYTO, 1                     );
    posYit.setInt16(   properties::DISCOUNTCALCFROM, 0                    );
    posYit.setDecimal( properties::GEP,           basar::Decimal(0.0)     );
    posYit.setDecimal( properties::AGP,           basar::Decimal(0.0)     );
    posYit.setString(  properties::POSITIONSTATUS,""                      );

    return posYit;
}

void TransferOrderPosInPreparationCollection::markPosAsDeleted( basar::db::aspect::AccessorPropertyTable_YIterator yitPos )
{
    getPropTab().markForDelete( yitPos );

    // renumberate pos numbers
    basar::Int32 posNo = yitPos.getInt32( libabbauw::properties::POSNO );
    for( basar::db::aspect::AccessorPropertyTable_YIterator yit = yitPos; yit != getPropTab().end(); ++yit )
    {
        if( yit.getState().getStates() != basar::SS_DELETE )
        {
            yit.setInt32( libabbauw::properties::POSNO, posNo );
            posNo++;
        }
    }
    --m_NumberOfPositions;
}

void TransferOrderPosInPreparationCollection::markAsDeleted()
{
    for( basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropTab().begin(); yit != getPropTab().end(); ++yit )
    {
        getPropTab().markForDelete( yit );
    }
    m_NumberOfPositions = 0;
}

basar::Int32 TransferOrderPosInPreparationCollection::getNextFreePosNo()
{
    return m_NumberOfPositions + 1;
}

bool TransferOrderPosInPreparationCollection::containsNonChargedQtyPosition()
{
    bool containsNonChargedQtyPosition = false;

    for( basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropTab().begin(); ! yit.isEnd() && ! containsNonChargedQtyPosition; ++yit )
    {
        containsNonChargedQtyPosition |= ( yit.isContainedAndSet( properties::NONCHARGEDQTY ) && yit.getInt32( properties::NONCHARGEDQTY ) > 0 );
    }

    return containsNonChargedQtyPosition;
}

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw
