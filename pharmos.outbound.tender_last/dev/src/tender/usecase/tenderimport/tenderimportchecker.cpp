#include "tenderimportchecker.h"
#include <loggerpool/tenderloggerpool.h>
#include <libtender/domainmodule/tender/tendertypecollection.h>
#include <libtender/domainmodule/tender/tendertypeenum.h>
#include <libtender/domainmodule/tender/tenderstatecollection.h>
#include <libtender/domainmodule/article/iarticlecollectiondm.h>
#include <libtender/domainmodule/customer/icustomercollectiondm.h>
#include <libtender/domainmodule/pharmacychain/ipharmacychaincollectiondm.h>
#include <libtender/domainmodule/partner/ipartnercollectiondm.h>
#include <libtender/domainmodule/tender/itenderimportcollection.h>
#include <libtender/domainmodule/tender/itender.h>
#include <libtender/domainmodule/tender/itenderpositioncollection.h>
#include <domainmodule/libtender_properties_definitions.h>
#include "ierrorhandler.h"
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <sstream>
#include "tender/infrastructure/iusecasegetter.h"
#include "tender/usecase/ipickeruc.h"
#include <libtender/domainmodule/constants.h>


#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4251 4311 4312 4481 4512 4800)
	#include <QCoreApplication>
#pragma warning (pop)

namespace tender
{
namespace useCase
{
TenderImportChecker::TenderImportChecker()
{
}

TenderImportChecker::~TenderImportChecker()
{
}

void TenderImportChecker::injectUseCaseGetter(tender::componentManager::IUseCaseGetterPtr useCaseGetter)
{
	m_UseCaseGetter = useCaseGetter;
}

void TenderImportChecker::inject( IErrorHandlerPtr errorHandler )
{
	m_ErrorHandler = errorHandler;
}

void TenderImportChecker::inject( libtender::domMod::tender::TenderTypeCollectionPtr tenderTypeCollection )
{
	m_TenderTypeCollection = tenderTypeCollection;
}

void TenderImportChecker::inject( libtender::domMod::tender::TenderStateCollectionPtr tenderStateCollection )
{
	m_TenderStateCollection = tenderStateCollection;
}

void TenderImportChecker::inject( libtender::domMod::article::IArticleCollectionDMPtr articleCollection )
{
	m_ArticleCollection = articleCollection;
}

void TenderImportChecker::inject( libtender::domMod::customer::ICustomerCollectionDMPtr customerCollection )
{
	m_CustomerCollection = customerCollection;
}

void TenderImportChecker::inject( libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr pharmacyChainCollection )
{
	m_PharmacyChainCollection = pharmacyChainCollection;
}

void TenderImportChecker::inject( libtender::domMod::partner::IPartnerCollectionDMPtr partnerCollection )
{
	m_PartnerCollection = partnerCollection;
}

void TenderImportChecker::inject( libtender::domMod::tender::ITenderImportCollectionPtr tenderImportCollection )
{
	m_TenderImportCollection = tenderImportCollection;
}

void TenderImportChecker::setArea( basar::Int16 area )
{
	m_Area = area;
}

bool TenderImportChecker::checkTenderPosition( const basar::Int32 lineNumber, libtender::domMod::tender::TenderImportStruct & positionData ) const
{
	m_LineNumber = lineNumber;
	basar::Int32 numberErrors = 0;

	//Check either Customer or Pharmacy Chain
	if( 0 != positionData.customerno )
	{
		if( false == checkCustomer( positionData.customerno ) ) ++numberErrors;
		if( false == checkBranch( positionData.branchno ) ) ++numberErrors;
	}
	else
	{
		
		if( false == checkPharmacyChain( positionData.pharmacychainno ) ) ++numberErrors;
		if( false == checkCustomerGroupID( positionData.pharmacychainno, positionData.customergroupid ) ) ++numberErrors;
	}

	if( false == checkTenderType( positionData.tendertype ) ) ++numberErrors;
	if( false == checkPartnerNo( positionData.partnerno, positionData.tendertype ) ) ++numberErrors;
	if( false == checkArticle( positionData.articlecode, positionData.tendertype, positionData.articleno ) ) ++numberErrors;
	if( false == checkPeriod( positionData.datefrom, positionData.dateto ) ) ++numberErrors;
	if( false == checkContractNo( positionData.contractno ) ) ++numberErrors;
	if( false == checkTenderState( positionData.tenderstate ) ) ++numberErrors;
	if( false == checkContractQuantity( positionData.contractqty ) ) ++numberErrors;
	if( false == checkContractPrice( positionData.contractprice, positionData.adddiscountpct ) ) ++numberErrors;
	
	return ( 0 == numberErrors );
}

bool TenderImportChecker::checkScheduledDeliveryPosition( const basar::Int32 lineNumber, libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct & positionData, 
	const basar::db::aspect::AccessorPropertyTable_YIterator tenderHead ) const
{
	m_LineNumber = lineNumber;
	basar::Int32 numberErrors = 0;
	
	basar::Int32 customerNo = m_TenderImportCollection->getTender(positionData.id)->getHeader().getInt32(properties::CUSTOMER_NO);;
	basar::Int32 pharmacyChainNo = m_TenderImportCollection->getTender(positionData.id)->getHeader().getInt32(properties::PHARMACY_CHAIN_NO);

	if( false == checkID( positionData.id ) ) ++numberErrors;
	if( false == checkBranch( positionData.branchno ) ) ++numberErrors;
	if( m_Area == constants::REGION_RS ) 
	{
		if( false == checkCustomerToPharmacyChain( pharmacyChainNo, positionData.customerno, positionData.branchno ) ) ++numberErrors;
	}
	else if ( m_Area == constants::REGION_FR )  //France
	{
		if( customerNo != positionData.customerno) ++numberErrors;
	}
	else if( m_Area == constants::REGION_BG ) //BG only check if same customer if hospital tender! 
	{
		if( 1 == tenderHead.getInt32(properties::TENDER_TYPE))
		{
			if( customerNo != positionData.customerno) ++numberErrors;
		}
	}

	if( false == checkDeliveryDate( positionData.id, positionData.deliverydate ) ) ++numberErrors;
	if( false == checkArticle( positionData.id, positionData.articlecode ) ) ++numberErrors;
	if( false == checkQuantity( positionData.id, positionData.articlecode, positionData.quantity ) ) ++numberErrors;
	if( false == checkPurchaseOrderProposalFlag( positionData.purchaseorderproposalflag ) ) ++numberErrors;
	if( false == checkBranch( positionData.branchnopop ) ) ++numberErrors;

	return ( 0 == numberErrors );
}

void TenderImportChecker::reset()
{
	m_ScheduledQuantities.clear();
}

bool TenderImportChecker::checkCustomer( const basar::Int32 customerNo ) const
{
	if( m_ValidCustomers.find( customerNo ) != m_ValidCustomers.end() )
	{
		return true;
	}
	if( m_InvalidCustomers.find( customerNo ) != m_InvalidCustomers.end() )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid customer number:" ).toLocal8Bit().constData();
		ss << " " << customerNo;
		m_ErrorHandler->handleError( ss.str().c_str() );
		return false;
	}

	m_CustomerCollection->resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_CustomerCollection->getSearchYit();
	yitSearch.setInt32( properties::CUSTOMER_NO, customerNo );
	m_CustomerCollection->findByCustomerNo( yitSearch );
	if( true == m_CustomerCollection->get().isEmpty() )
	{
		m_InvalidCustomers.insert( customerNo );
	}
	else
	{
		m_ValidCustomers.insert( customerNo );
	}

	return checkCustomer( customerNo );
}

bool TenderImportChecker::checkCustomerToPharmacyChain( const basar::Int32 pharmacyChainNo, const basar::Int32 customerNo, const basar::Int16 branchNo ) const
{
	tender::useCase::IPickerUCPtr pharmacyChainPickerUC = m_UseCaseGetter->getPharmacyChainPickerUC();
	basar::db::aspect::AccessorPropertyTableRef foundCustomers;

	pharmacyChainPickerUC->getSearchYIterator().setInt32(properties::PHARMACY_CHAIN_NO, pharmacyChainNo);

	pharmacyChainPickerUC->setSearchWithoutGUIMode(true);
	pharmacyChainPickerUC->run();

	foundCustomers = pharmacyChainPickerUC->getFound();

	basar::db::aspect::AccessorPropertyTable_YIterator foundCustomer = foundCustomers.begin();
	for( ; !foundCustomer.isEnd(); ++foundCustomer)
	{
		if(customerNo == foundCustomer.getInt32(properties::CUSTOMER_NO) && 
		   branchNo == foundCustomer.getInt16(properties::BRANCH_NO) )
		{
			return true;
		}
	}

	return false;
}

bool TenderImportChecker::checkBranch( const basar::Int16 branchNo ) const
{
	if(	   0 < branchNo
		&& 100 > branchNo )
	{
		return true;
	}

	std::stringstream ss;
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid branch number:" ).toLocal8Bit().constData();
	ss << " " << branchNo;
	m_ErrorHandler->handleError( ss.str().c_str() );
	return false;
}

bool TenderImportChecker::checkPharmacyChain( const basar::Int32 pharmacyChainNo ) const
{
	if( m_ValidPharmacyChain.find( pharmacyChainNo ) != m_ValidPharmacyChain.end() )
	{
		return true;
	}

	if( m_InvalidPharmacyChain.find( pharmacyChainNo ) != m_InvalidPharmacyChain.end() )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid pharmacy chain number:" ).toLocal8Bit().constData();
		ss << " " << pharmacyChainNo;
		m_ErrorHandler->handleError( ss.str().c_str() );
		return false;
	}

	m_PharmacyChainCollection->resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_PharmacyChainCollection->getSearchYit();
	yitSearch.setInt32( properties::PHARMACY_CHAIN_NO, pharmacyChainNo );
	m_PharmacyChainCollection->findByPharmacyChainNo( yitSearch );

	if( true == m_PharmacyChainCollection->get().isEmpty() )
	{
		m_InvalidPharmacyChain.insert( pharmacyChainNo );
	}
	else
	{
		m_ValidPharmacyChain.insert( pharmacyChainNo );
	}

	return checkPharmacyChain( pharmacyChainNo );
}

bool TenderImportChecker::checkCustomerGroupID( const basar::Int32 pharmacyChainNo, const basar::I18nString customerGroupID ) const
{
	m_PharmacyChainCollection->resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_PharmacyChainCollection->getSearchYit();
	yitSearch.setInt32( properties::PHARMACY_CHAIN_NO, pharmacyChainNo );
	yitSearch.setString( properties::CUSTOMERGROUPID, customerGroupID );
	m_PharmacyChainCollection->findCustomerGroupIDByPharmacyChain( yitSearch );

	if( true == m_PharmacyChainCollection->get().isEmpty() )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool TenderImportChecker::isPartnerNeeded(const basar::Int32 tenderType) const
{
	if( libtender::domMod::tender::TT_CAP			== tenderType ||
		libtender::domMod::tender::TT_SRA			== tenderType ||
		libtender::domMod::tender::TT_PLATFORM		== tenderType || 
		libtender::domMod::tender::TT_PREWHOLESALE	== tenderType   )
	{
		return true;
	}

	return false;
}

bool TenderImportChecker::checkPartnerNo( const basar::Int32 partnerNo, const basar::Int32 tenderType ) const
{
	//Partner has only to be checked when country is France!  
	if( !isPartnerNeeded(tenderType) )
	{
		return true;
	}

	PartnerNoCollection & validPartners = getValidPartners( tenderType );
	if( validPartners.find( partnerNo ) != validPartners.end() )
	{
		return true;
	}

	PartnerNoCollection & invalidPartners = getInvalidPartners( tenderType );
	if( invalidPartners.find( partnerNo ) != invalidPartners.end() )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid partner number:" ).toLocal8Bit().constData();
		ss << " " << partnerNo;
		m_ErrorHandler->handleError( ss.str().c_str() );
		return false;
	}

	m_PartnerCollection->resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_PartnerCollection->getSearchYit();
	yitSearch.setInt32( properties::PARTNER_NO, partnerNo );
	yitSearch.setInt32( properties::TENDER_TYPE, tenderType );
	m_PartnerCollection->findByPattern( yitSearch );
	if( true == m_PartnerCollection->get().isEmpty() )
	{
		invalidPartners.insert( partnerNo );
	}
	else
	{
		validPartners.insert( partnerNo );
	}

	return checkPartnerNo( partnerNo, tenderType );
}

TenderImportChecker::PartnerNoCollection & TenderImportChecker::getValidPartners( const basar::Int32 tenderType ) const
{
	PartnerNoMap::iterator iter = m_ValidPartners.find( tenderType );
	if( iter == m_ValidPartners.end() )
	{
		m_ValidPartners[ tenderType ] = PartnerNoCollection();
		iter = m_ValidPartners.find( tenderType );
	}

	return iter->second;
}

TenderImportChecker::PartnerNoCollection & TenderImportChecker::getInvalidPartners( const basar::Int32 tenderType ) const
{
	PartnerNoMap::iterator iter = m_InvalidPartners.find( tenderType );
	if( iter == m_InvalidPartners.end() )
	{
		m_InvalidPartners[ tenderType ] = PartnerNoCollection();
		iter = m_InvalidPartners.find( tenderType );
	}

	return iter->second;
}

basar::Int32 TenderImportChecker::getArticleNoByArticleCode( 
	const basar::Int32 tenderType,
	const basar::VarString & articleCode 
		) const
{	
	const ArticleCodeCollection & validArticles = getValidArticles( tenderType );
	ArticleCodeCollection::const_iterator iter = validArticles.find( articleCode );
	if( iter != validArticles.end() )
	{
		return iter->second;
	}

	return 0;
}

bool TenderImportChecker::checkArticle( 
	const basar::VarString & articleCode, 
	const basar::Int32 tenderType, 
	basar::Int32 & articleNo				// out param
		) const
{
	ArticleCodeCollection & validArticles = getValidArticles( tenderType );
	ArticleCodeCollection::const_iterator iter = validArticles.find( articleCode );
	if( iter != validArticles.end() )
	{
		articleNo = iter->second;
		return true;
	}

	ArticleCodeCollection & invalidArticles = getInvalidArticles( tenderType );
	iter = invalidArticles.find( articleCode );
	if( iter != invalidArticles.end() )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid article code:" ).toLocal8Bit().constData();
		ss << " " << articleCode;
		m_ErrorHandler->handleError( ss.str().c_str() );
		articleNo = iter->second;
		return false;
	}

	m_ArticleCollection->resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_ArticleCollection->getSearchYit();
	yitSearch.setString( properties::ARTICLE_CODE, articleCode );
	yitSearch.setInt32( properties::TENDER_TYPE, tenderType );
	m_ArticleCollection->findByArticleCode( yitSearch );
	if( true == m_ArticleCollection->get().isEmpty() )
	{
		invalidArticles[ articleCode ] = 0;
	}
	else
	{
		basar::db::aspect::AccessorPropertyTable_YIterator yitArticle = m_ArticleCollection->get().begin();
		validArticles[ articleCode ] = yitArticle.getInt32( properties::ARTICLE_NO );
	}

	return checkArticle( articleCode, tenderType, articleNo );
}

TenderImportChecker::ArticleCodeCollection & TenderImportChecker::getValidArticles( const basar::Int32 tenderType ) const
{
	ArticleCodeMap::iterator iter = m_ValidArticles.find( tenderType );
	if( iter == m_ValidArticles.end() )
	{
		m_ValidArticles[ tenderType ] = ArticleCodeCollection();
		iter = m_ValidArticles.find( tenderType );
	}

	return iter->second;
}

TenderImportChecker::ArticleCodeCollection & TenderImportChecker::getInvalidArticles( const basar::Int32 tenderType ) const
{
	ArticleCodeMap::iterator iter = m_InvalidArticles.find( tenderType );
	if( iter == m_InvalidArticles.end() )
	{
		m_InvalidArticles[ tenderType ] = ArticleCodeCollection();
		iter = m_InvalidArticles.find( tenderType );
	}

	return iter->second;
}

bool TenderImportChecker::checkPeriod( const basar::Date & dateFrom, const basar::Date & dateTo ) const
{

	if( m_Area == constants::REGION_RS )
	{
		if( dateTo.getDate() <= dateFrom.getDate() )
		{
			std::stringstream ss;
			ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
			ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid period of time:" ).toLocal8Bit().constData();
			ss << " dateFrom = " << dateFrom << ", dateTo = " << dateTo;
			m_ErrorHandler->handleError( ss.str().c_str() );
			return false;
		}
	}
	else
	{
		if( dateTo.getDate() <= dateFrom.getDate() 
			|| std::min<basar::Int32>( dateFrom.getDate(), dateTo.getDate() ) < basar::Date().getDate() )
		{
			std::stringstream ss;
			ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
			ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid period of time:" ).toLocal8Bit().constData();
			ss << " dateFrom = " << dateFrom << ", dateTo = " << dateTo;
			m_ErrorHandler->handleError( ss.str().c_str() );
			return false;
		}
	}

	return true;
}

bool TenderImportChecker::checkContractNo( const basar::I18nString & contractNo ) const
{
	if( true == contractNo.empty() )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Empty contract number" ).toLocal8Bit().constData();
		m_ErrorHandler->handleError( ss.str().c_str() );
		return false;
	}

	return true;
}

bool TenderImportChecker::checkTenderType( const basar::Int32 tenderType ) const
{
	using namespace libtender::domMod::tender;
	TenderTypeCollection::const_iterator iter = m_TenderTypeCollection->begin();
	for( ; iter != m_TenderTypeCollection->end(); ++iter )
	{
		if( tenderType == iter->first )
		{
			return true;
		}
	}

	std::stringstream ss;
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid tender type:" ).toLocal8Bit().constData();
	ss << " " << tenderType;
	m_ErrorHandler->handleError( ss.str().c_str() );
	return false;
}

bool TenderImportChecker::checkTenderState( const basar::Int16 tenderState ) const
{
	using namespace libtender::domMod::tender;
	TenderStateCollection::const_iterator iter = m_TenderStateCollection->begin();
    for( ; iter != m_TenderStateCollection->end(); ++iter )
    {
		if( tenderState == iter->first )
		{
			return true;
		}
    }

	std::stringstream ss;
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid tender state:" ).toLocal8Bit().constData();
	ss << " " << tenderState;
	m_ErrorHandler->handleError( ss.str().c_str() );
	return false;
}

bool TenderImportChecker::checkContractQuantity( const basar::Int32 contractQty ) const
{
	if( 0 <= contractQty )
	{
		return true;
	}

	std::stringstream ss;
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid contract quantity:" ).toLocal8Bit().constData();
	ss << " " << contractQty;
	m_ErrorHandler->handleError( ss.str().c_str() );
	return false;
}

bool TenderImportChecker::checkContractPrice( const basar::Decimal & contractPrice, const basar::Decimal & discountPct ) const
{
	static basar::Decimal zero( 0 );

	if( zero > contractPrice || zero > discountPct )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid price/discount:" ).toLocal8Bit().constData();
		ss << " price = " << contractPrice << ", discount = " << discountPct;
		m_ErrorHandler->handleError( ss.str().c_str() );
		return false;
	}

	return true;
}

bool TenderImportChecker::checkID( const basar::Int32 id ) const
{
	bool ret = m_TenderImportCollection->isValidID( id );
	if( false == ret )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid ID:" ).toLocal8Bit().constData();
		ss << " " << id;
		m_ErrorHandler->handleError( ss.str().c_str() );
	}

	return ret;
}

bool TenderImportChecker::checkDeliveryDate( const basar::Int32 id, const basar::Date & deliveryDate ) const
{
	basar::Date fromDate; fromDate.setInvalid();
	basar::Date toDate; toDate.setInvalid();
	libtender::domMod::tender::ITenderPtr tender = m_TenderImportCollection->getTender( id );
	if( NULL != tender.get() )
	{
		basar::db::aspect::AccessorPropertyTable_YIterator yitHeader = tender->getHeader();
		fromDate = yitHeader.getDate( properties::DATE_FROM );
		toDate = yitHeader.getDate( properties::DATE_TO );
	}

	if(	   deliveryDate.getDate() < basar::Date().getDate() 
		|| deliveryDate.getDate() < fromDate.getDate()
		|| deliveryDate.getDate() > toDate.getDate() )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid delivery date:" ).toLocal8Bit().constData();
		ss << " " << deliveryDate;
		m_ErrorHandler->handleError( ss.str().c_str() );
		return false;
	}

	return true;
}

bool TenderImportChecker::checkArticle( const basar::Int32 id, const basar::VarString & articleCode ) const
{
	bool ret = false;
	libtender::domMod::tender::ITenderPtr tender = m_TenderImportCollection->getTender( id );
	if( NULL != tender.get() )
	{
		basar::db::aspect::AccessorPropertyTable_YIterator yitPos = tender->getPositionCollection()->getByArticleCode( articleCode );
		ret = (    false == yitPos.isNull() 
				&& false == yitPos.isEnd() );
	}

	if( false == ret )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid article code:" ).toLocal8Bit().constData();
		ss << " tenderid = " << id << ", articleCode = " << articleCode;
		m_ErrorHandler->handleError( ss.str().c_str() );
	}

	return ret;
}

bool TenderImportChecker::checkQuantity( const basar::Int32 id, const basar::VarString & articleCode, const basar::Int32 quantity ) const
{
	if( 0 >= quantity )
	{
		std::stringstream ss;
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
		ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid delivery quantity:" ).toLocal8Bit().constData();
		ss << " " << quantity;
		m_ErrorHandler->handleError( ss.str().c_str() );
	}

	// get sum of scheduled quantities for id + articleCode
	ScheduledQuantityCollection & scheduledQuantities = getScheduledQuantities( id );
	ScheduledQuantityCollection::iterator iter = scheduledQuantities.find( articleCode );
	if( iter == scheduledQuantities.end() )
	{
		scheduledQuantities[ articleCode ] = 0;
		iter = scheduledQuantities.find( articleCode );
	}

	iter->second = iter->second + quantity;
	basar::Int32 scheduledQuantity = iter->second;

	// get contract quantity for id + articleCode
	basar::Int32 contractQuantity = 0;
	libtender::domMod::tender::ITenderPtr tender = m_TenderImportCollection->getTender( id );
	if( NULL != tender.get() )
	{
		basar::db::aspect::AccessorPropertyTable_YIterator yitPos = tender->getPositionCollection()->getByArticleCode( articleCode );
		if(	   false == yitPos.isNull() 
			&& false == yitPos.isEnd() )
		{
			contractQuantity = yitPos.getInt32( properties::CONTRACT_QTY );
		}
	}

	if( scheduledQuantity <= contractQuantity )
	{
		return true;
	}

	std::stringstream ss;
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Delivery quantity exceeds contract quantity:" ).toLocal8Bit().constData();
	ss << " article code=" << articleCode.c_str() << ", contract quantity=" << contractQuantity << ", scheduled quantity=" << scheduledQuantity;
	m_ErrorHandler->handleError( ss.str().c_str() );
	return false;
}

TenderImportChecker::ScheduledQuantityCollection & TenderImportChecker::getScheduledQuantities( const basar::Int32 id ) const
{
	TenderScheduledQuantityCollection::iterator iter = m_ScheduledQuantities.find( id );
	if( iter == m_ScheduledQuantities.end() )
	{
		m_ScheduledQuantities[ id ] = ScheduledQuantityCollection();
		iter = m_ScheduledQuantities.find( id );
	}

	return iter->second;
}

bool TenderImportChecker::checkPurchaseOrderProposalFlag( const basar::Int16 purchaseOrderProposalFlag ) const
{
	if(    0 == purchaseOrderProposalFlag 
		|| 1 == purchaseOrderProposalFlag )
	{
		return true;
	}

	std::stringstream ss;
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Line" ).toLocal8Bit().constData() << " " << m_LineNumber << ": ";
	ss << QCoreApplication::translate( "tender::useCase::TenderImportChecker", "Invalid purchase order proposal flag:" ).toLocal8Bit().constData();
	ss << " " << purchaseOrderProposalFlag;
	m_ErrorHandler->handleError( ss.str().c_str() );
	return false;
}

} // namespace useCase
} // namespace tender
