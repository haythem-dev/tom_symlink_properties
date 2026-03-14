#include "cmndm.h"

#include "transferorderinpreparationdm.h"

#include "itransferorderheadinpreparation.h"
#include "itransferorderposinpreparationcollection.h"

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

const libutil::misc::ClassInfo& TransferOrderInPreparationDM::getClassInfo()
{
	static const libutil::misc::ClassInfo info( "transferOrder::TransferOrderInPreparationDM", libutil::misc::Version( 1, 0, 0, 0 ) );
	return info;
}

TransferOrderInPreparationDM::TransferOrderInPreparationDM()
    : m_Logger( libabbauw::LoggerPool::loggerTransferOrder )
{
  	METHODNAME_DEF( TransferOrderInPreparationDM, TransferOrderInPreparationDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

TransferOrderInPreparationDM::~TransferOrderInPreparationDM()
{
	METHODNAME_DEF( TransferOrderInPreparationDM, ~TransferOrderInPreparationDM )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void TransferOrderInPreparationDM::injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr dbTransactionFactory )
{
    m_DBTransactionFactory = dbTransactionFactory;
}

void TransferOrderInPreparationDM::injectHead( ITransferOrderHeadInPreparationPtr transferorderhead )
{
    m_TransferOrderHead = transferorderhead;
}

void TransferOrderInPreparationDM::injectPos( ITransferOrderPosInPreparationCollectionPtr transferorderpos )
{
    m_TransferOrderPosCollection = transferorderpos;
}

libabbauw::domMod::transferOrder::ITransferOrderHeadInPreparationPtr TransferOrderInPreparationDM::getHead()
{
    return m_TransferOrderHead;
}

libabbauw::domMod::transferOrder::ITransferOrderPosInPreparationCollectionPtr TransferOrderInPreparationDM::getPosCollection()
{
    return m_TransferOrderPosCollection;
}

const log4cplus::Logger& TransferOrderInPreparationDM::getLogger() const
{
    METHODNAME_DEF( DMBase, getLogger )
	BLOG_TRACE_METHOD( m_Logger, fun );

    return m_Logger;
}

bool TransferOrderInPreparationDM::isComplete()
{
    bool headisComplete = getHead()->isComplete();
    return headisComplete && ! getPosCollection()->isEmpty();
}

bool TransferOrderInPreparationDM::isChanged()
{
    const bool headIsChanged = getHead()->isChanged();
    const bool posIsChanged = getPosCollection()->isChanged();
    const bool headIsComplete = getHead()->isComplete();

    const bool headIsDeleted = getHead()->isDeleted();
    const bool posIsComplete = !getPosCollection()->isEmpty();

    const bool ret = headIsChanged && !( headIsDeleted && !headIsComplete && posIsComplete ) || (posIsChanged && headIsComplete );

    if( headIsDeleted && !headIsComplete && !posIsComplete )
        return true;

    return ret;
}

void TransferOrderInPreparationDM::save()
{
    METHODNAME_DEF( TransferOrderInPreparationDM, save )
	BLOG_TRACE_METHOD( getLogger(), fun );

    libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction;
	
	try
	{
		transaction = m_DBTransactionFactory->createDBTransaction();
		transaction->begin();

        m_TransferOrderHead->save();
        m_TransferOrderPosCollection->save();

		transaction->commit();
	}
	catch(const basar::Exception& e)
	{
		transaction->rollback();

		getLogger().forcedLog( log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__ );
		throw;
	}
}

void TransferOrderInPreparationDM::load()
{
    METHODNAME_DEF( TransferOrderInPreparationDM, load )
    BLOG_TRACE_METHOD( getLogger(), fun );

    m_TransferOrderHead->resetSearchYit();
    m_TransferOrderHead->findByPattern( m_TransferOrderHead->getSearchYit() );

    m_TransferOrderPosCollection->resetSearchYit();
    m_TransferOrderPosCollection->findByPattern( m_TransferOrderPosCollection->getSearchYit() );
}

basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderInPreparationDM::addEmptyPosition()
{
    return m_TransferOrderPosCollection->addEmptyPosition();
}

void TransferOrderInPreparationDM::markPosAsDeleted( const basar::db::aspect::AccessorPropertyTable_YIterator yitPos )
{
    m_TransferOrderPosCollection->markPosAsDeleted( yitPos );
}

void TransferOrderInPreparationDM::markAsDeleted()
{
    m_TransferOrderHead->markAsDeleted();
    m_TransferOrderPosCollection->markAsDeleted();
}

basar::Int32 TransferOrderInPreparationDM::getNextFreePosNo()
{
    return m_TransferOrderPosCollection->getNextFreePosNo();
}

bool TransferOrderInPreparationDM::containsNonChargedQtyPosition()
{
    return m_TransferOrderPosCollection->containsNonChargedQtyPosition();
}

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw
