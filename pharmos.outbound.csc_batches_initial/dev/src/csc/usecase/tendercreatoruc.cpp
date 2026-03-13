#include "tendercreatoruc.h"

#include "libabbauw/itransferorder.h"
#include "libabbauw/itransferorderhead.h"
#include "libabbauw/itransferorderposcollection.h"
#include "libabbauw/orderstatus.h"
#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/exceptions/tendercreationexception.h"
#include "libabbauw/domainmodule/orderprotocol/iorderprotocol.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include "domainmodule/tender/itendercollectiondm.h"
#include "domainmodule/tender/tenderdm.h"
#include "domainmodule/pharmacygroup/ipharmacygroupcollectiondm.h"
#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/tender/tenderstateenum.h"

#pragma warning (push)
#pragma warning(disable: 4512)
	#include <QtCore/QCoreApplication>
#pragma warning (pop)

namespace CSC_Batches
{
namespace useCase
{
    TenderCreatorUC::TenderCreatorUC()
    {
        METHODNAME_DEF( TenderCreatorUC, TenderCreatorUC );
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );
        m_TenderEndDateOffset = 1;
    }

    TenderCreatorUC::~TenderCreatorUC()
    {
        METHODNAME_DEF( TenderCreatorUC, ~TenderCreatorUC );
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );
    }

    void TenderCreatorUC::injectPharmacyGroupCollectionDM( libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr pharmacyGroupCollectionDM )
    {
        METHODNAME_DEF( TenderCreatorUC, injectPharmacyGroupCollectionDM )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

        m_PharmacyGroupCollection = pharmacyGroupCollectionDM;
    }

	void TenderCreatorUC::injectOrderProtocol( libabbauw::domMod::orderProtocol::IOrderProtocolPtr orderProtocol )
	{
        METHODNAME_DEF( TenderCreatorUC, injectOrderProtocol )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

		m_OrderProtocol = orderProtocol;
	}

	void TenderCreatorUC::injectTransferOrderHead( libabbauw::domMod::transferOrder::ITransferOrderHeadPtr transferOrderHead )
	{
        METHODNAME_DEF( TenderCreatorUC, injectOrderProtocol )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

		m_TransferOrderHead = transferOrderHead;
	}

    libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr TenderCreatorUC::getPharmacyGroupCollectionDM()
    {
        METHODNAME_DEF( TenderCreatorUC, getPharmacyGroupCollectionDM )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

        return m_PharmacyGroupCollection;
    }

    libtender::domMod::tender::ITenderCollectionDMPtr TenderCreatorUC::getTenderCollectionDM()
    {
        METHODNAME_DEF( TenderCreatorUC, getTenderCollectionDM )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

        return m_TenderCollection;
    }

    void TenderCreatorUC::injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM )
    {
        METHODNAME_DEF( TenderCreatorUC, injectTenderCollectionDM )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

        m_TenderCollection = tenderCollectionDM;
    }

	bool TenderCreatorUC::hasErrors() const
	{
		return ( false == m_Errors.empty() );
	}

	const ITenderCreator::ErrorList & TenderCreatorUC::getErrors() const
	{
		return m_Errors;
	}

	basar::cmnutil::Date TenderCreatorUC::getTenderEndDate()
	{
		METHODNAME_DEF( TenderCreatorUC, getTenderEndDate )
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

		using basar::cmnutil::Date;

		Date endDate = Date::getCurrent();
		endDate.addMonths( m_OMGDuration );
		basar::Int32 daysOfMonth = Date::getDaysOfMonth( endDate.getYear(), endDate.getMonth() );
		endDate = Date( endDate.getYear(), endDate.getMonth(), daysOfMonth );
		endDate.addDays( -m_TenderEndDateOffset );

		if( basar::cmnutil::Date::SUNDAY == endDate.getDayOfWeek() )
		{
			endDate.addDays( -2 );
		}
		else if( basar::cmnutil::Date::SATURDAY == endDate.getDayOfWeek() )
		{
			endDate.addDays( -1 );
		}

		return endDate;
	}

     basar::db::aspect::AccessorPropertyTable_YIterator TenderCreatorUC::getPharmacyGroup( basar::Int32 pharmacyNo )
     {
        METHODNAME_DEF( TenderCreatorUC, getPharmacyGroup )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

        try
        {
            getPharmacyGroupCollectionDM()->resetSearchYit();
            basar::db::aspect::AccessorPropertyTable_YIterator findPharGrpYit = getPharmacyGroupCollectionDM()->getSearchYit();
            findPharGrpYit.setInt32( properties::CUSTOMER_NO, pharmacyNo );
            getPharmacyGroupCollectionDM()->findByPseudoCustomerNo(findPharGrpYit);

            return getPharmacyGroupCollectionDM()->get().begin();
        }
        catch( basar::Exception& e )
        {
            BLOG_ERROR(  libabbauw::LoggerPool::loggerUseCases, e.what() );
            basar::VarString error = "Failed to find pharmacy group\n" + e.what();
            basar::ExceptInfo info( fun, error, __FILE__, __LINE__ );
            throw libabbauw::exceptions::TenderCreationException( info );
        }
     }

     void TenderCreatorUC::setTenderEndDateOffset(basar::Int32 days)
     {
        METHODNAME_DEF( TenderCreatorUC, setTenderEndDateOffset )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

        m_TenderEndDateOffset = days;
     }

	 void TenderCreatorUC::setOMGDuration(basar::Int32 omgDuration)
	 {
		METHODNAME_DEF( TenderCreatorUC, setOMGDuration )
		BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

		m_OMGDuration = omgDuration;
	 }

     void TenderCreatorUC::createTenderHeader( libtender::domMod::tender::ITenderPtr tender,
                                               libabbauw::domMod::transferOrder::ITransferOrderPtr transferOrder,
                                               basar::db::aspect::AccessorPropertyTable_YIterator pharmgpCollYit )
    {
        METHODNAME_DEF( TenderCreatorUC, createTenderHeader )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

		basar::db::aspect::AccessorPropertyTable_YIterator transferOrdHeadYit = transferOrder->getHead()->get();
		basar::db::aspect::AccessorPropertyTable_YIterator tenderHeadYit = tender->getHeader();

		tenderHeadYit.setString( properties::PHARMACY_GROUPID, pharmgpCollYit.getString( properties::PHARMACY_GROUPID ) );
		tenderHeadYit.setInt32( properties::TENDER_TYPE, 1 );
		tenderHeadYit.setString( properties::CONTRACT_NO, transferOrdHeadYit.getString( libabbauw::properties::HEADERTEXT ) );
		tenderHeadYit.setInt16( properties::BRANCH_NO, transferOrdHeadYit.getInt16( libabbauw::properties::BRANCHNO ) );
		tenderHeadYit.setInt32( properties::CUSTOMER_NO, transferOrdHeadYit.getInt32( libabbauw::properties::PHARMACYNO ) );
		tenderHeadYit.setInt16( properties::ACC_BRANCH_NO, transferOrdHeadYit.getInt16( libabbauw::properties::BRANCHNO ) );
		tenderHeadYit.setInt32( properties::ACC_CUSTOMER_NO, transferOrdHeadYit.getInt32( libabbauw::properties::PHARMACYNO ) );
		basar::cmnutil::Date invalidDate( 9999, 12, 31 );
		tenderHeadYit.setDate( properties::DATE_FROM, invalidDate );
		tenderHeadYit.setDate( properties::DATE_TO, invalidDate );
		tenderHeadYit.setDecimal(properties::EXCEEDANCE_PCT, basar::Decimal(0.0) );
		tenderHeadYit.setInt16( properties::TENDER_STATE, libtender::domMod::tender::TS_UNCOMPLETED );
		tenderHeadYit.setInt32( properties::PHARMACY_CHAIN_NO, 0 );
		tenderHeadYit.setInt32( properties::PARTNER_NO, 0 );
		tenderHeadYit.setInt16( properties::DELETED_FLAG, 0 );
		tenderHeadYit.setInt16( properties::PURCHASE_ORDER_PROPOSAL_FLAG, 0 );
		tenderHeadYit.setString( properties::CUSTOMERGROUPID, "00" );
	}

    void TenderCreatorUC::createTenderPositions( libtender::domMod::tender::ITenderPtr tender,
                                               basar::db::aspect::AccessorPropertyTable_YIterator tranOrdPosYit,
                                               basar::db::aspect::AccessorPropertyTable_YIterator pharmgpCollYit )
    {
        METHODNAME_DEF( TenderCreatorUC, createTenderPositions )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

		static const basar::Decimal ZERO( 0.0 );
		basar::Int16 nRecallFlag =  atoi( pharmgpCollYit.getString( properties::RECLAIM_OMG_DISC ).c_str() ) == 0 ? 0 : 1;

		basar::Decimal discountPct = tranOrdPosYit.getDecimal( libabbauw::properties::DISCOUNTPCT );
		if( discountPct < ZERO )
		{
			discountPct *= -1;
		}

		basar::Int32 rebateInKindQty = tranOrdPosYit.getInt32( libabbauw::properties::NONCHARGEDQTY );
		basar::Int32 orderedQty = tranOrdPosYit.getInt32( libabbauw::properties::ORDEREDQTY );
		if( 0 < rebateInKindQty )
		{
			discountPct = basar::Decimal( rebateInKindQty ) / ( rebateInKindQty + orderedQty );
			discountPct *= 100;
			if( basar::Decimal( 100 ) < discountPct )
			{
				discountPct = basar::Decimal( 100 );
			}
		}
			
		if( ZERO == discountPct )
		{
			std::stringstream error;
			error << QCoreApplication::translate( "TenderCreatorUC", "Article Code" ).toLocal8Bit().constData() << ": " << tranOrdPosYit.getString( libabbauw::properties::PZN ).c_str() << ": ";
			error << QCoreApplication::translate( "TenderCreatorUC", "Rebate is zero! No Tender created." ).toLocal8Bit().constData();
			m_Errors.push_back( error.str().c_str() );
			basar::ExceptInfo info( fun, error.str().c_str(), __FILE__, __LINE__ );
			throw libabbauw::exceptions::TenderCreationException( info );
		}

        basar::db::aspect::AccessorPropertyTable_YIterator tenderPosYit = tender->addEmptyPosition();

        tenderPosYit.setInt32( properties::ARTICLE_NO, tranOrdPosYit.getInt32( libabbauw::properties::ARTICLENO ) );
        tenderPosYit.setString( properties::ARTICLE_CODE, tranOrdPosYit.getString( libabbauw::properties::PZN ) );
        tenderPosYit.setInt32( properties::CONTRACT_QTY, orderedQty + rebateInKindQty );
        tenderPosYit.setDecimal( properties::CONTRACT_PRICE, ZERO );
        tenderPosYit.setDecimal( properties::ADD_DISCOUNT_PCT, discountPct );
        tenderPosYit.setInt16( properties::DISCOUNT_CALC_FROM, tranOrdPosYit.getInt16( libabbauw::properties::DISCOUNTCALCFROM ) );
        tenderPosYit.setInt16( properties::DISCOUNT_APPLY_TO, tranOrdPosYit.getInt16( libabbauw::properties::DISCOUNTAPPLYTO ) );
        tenderPosYit.setInt16( properties::TENDER_STATE, libtender::domMod::tender::TS_UNCOMPLETED );
        tenderPosYit.setInt32( properties::OWNQUOTA_QTY, 0 );
        tenderPosYit.setInt16( properties::OWNQUOTA_FLAG, 0 );
        tenderPosYit.setInt16( properties::RECALL_FLAG, nRecallFlag );
    }

    bool TenderCreatorUC::checkActiveTenderAvailable( basar::Int32 articleNo, basar::VarString pharmGrpId )
    {
        METHODNAME_DEF( TenderCreatorUC, checkActiveTenderAvailable )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );
        try
        {
            getTenderCollectionDM()->resetSearchYit();
            basar::db::aspect::AccessorPropertyTable_YIterator findValidTender = getTenderCollectionDM()->getSearchYit();
            findValidTender.setString( properties::PHARMACY_GROUPID, pharmGrpId );
            findValidTender.setInt32( properties::ARTICLE_NO, articleNo );
            findValidTender.setInt16( properties::TENDER_STATE, libtender::domMod::tender::TS_OK );
            getTenderCollectionDM()->findValidTenders(findValidTender);
            return !getTenderCollectionDM()->isEmpty();
        }
        catch(  basar::Exception& e )
        {
            BLOG_ERROR(  libabbauw::LoggerPool::loggerUseCases, e.what() );
            basar::VarString error = "Find active tender failed \n" + e.what();
            basar::ExceptInfo info( fun, error, __FILE__, __LINE__ );
            throw libabbauw::exceptions::TenderCreationException( info );
        }
    }

    basar::appl::EventReturnType TenderCreatorUC::createTender( libabbauw::domMod::transferOrder::ITransferOrderPtr transferOrder )
    {
        METHODNAME_DEF( BatchOrderEditionUC, createTender )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

        try
        {
            basar::Int32 pharmacyNo = transferOrder->getHead()->get().getInt32( libabbauw::properties::PHARMACYNO );

            basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroupYit = getPharmacyGroup( pharmacyNo );
            if( pharmacyGroupYit.isEnd() )
            {
                basar::ExceptInfo info( fun, "Pharmacy group not found.", __FILE__, __LINE__ );
                throw libabbauw::exceptions::TenderCreationException( info );
            }

			bool isError = false;
			m_Errors.clear();
			basar::VarString pharmacyGroupID = pharmacyGroupYit.getString( properties::PHARMACY_GROUPID );
			basar::db::aspect::AccessorPropertyTable_YIterator transferOrderPosYit = transferOrder->getPosCollection()->get().begin();
			for( ; !transferOrderPosYit.isEnd(); ++transferOrderPosYit )
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yitTender;
				libtender::domMod::tender::ITenderPtr tender = getTenderCollectionDM()->getTender(yitTender);

				try
				{
					createTenderHeader( tender, transferOrder, pharmacyGroupYit );
					createTenderPositions( tender, transferOrderPosYit, pharmacyGroupYit );
				}
				catch( const libabbauw::exceptions::TenderCreationException & )
				{
					// nothing to do, continue with next order position
					isError = true;
					continue;
				}

				if( !checkActiveTenderAvailable( transferOrderPosYit.getInt32( libabbauw::properties::ARTICLENO ), pharmacyGroupID ) )
				{
					tender->getHeader().setInt16( properties::TENDER_STATE, libtender::domMod::tender::TS_OK );
					tender->getHeader().setDate( properties::DATE_FROM, basar::cmnutil::Date::getCurrent() );
					tender->getHeader().setDate( properties::DATE_TO, getTenderEndDate() );
				}

				tender->save();

				// protocol tender creations
				m_OrderProtocol->setBackupPos( transferOrderPosYit );
				m_OrderProtocol->protocolCreateTender( tender->getHeader().getInt32( properties::TENDER_NO ) );
			}

			if( false == isError )
			{
				// change transfer order state
				m_TransferOrderHead->findById( transferOrder->getOrderNo() );
				basar::db::aspect::AccessorPropertyTable_YIterator yitTransferOrderHead = m_TransferOrderHead->get();
				libabbauw::domMod::OrderStatus orderStatus( libabbauw::domMod::OrderStatus::ER );
				yitTransferOrderHead.setString( libabbauw::properties::STATUS, orderStatus.toString() );
				m_TransferOrderHead->save();
			}

            return basar::appl::HANDLER_OK;
        }
        catch( libabbauw::exceptions::TenderCreationException& e )
        {
            throw e;
        }
        catch( basar::Exception& e )
        {
            BLOG_ERROR(  libabbauw::LoggerPool::loggerUseCases, e.what() );

            basar::ExceptInfo info( fun, e.what(), __FILE__, __LINE__ );
            throw libabbauw::exceptions::TenderCreationException( info );
        }
    }

}
}
