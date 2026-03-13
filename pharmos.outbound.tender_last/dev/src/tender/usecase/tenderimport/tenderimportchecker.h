#ifndef GUARD_TENDER_USECASE_IMPORT_CHECKER_H
#define GUARD_TENDER_USECASE_IMPORT_CHECKER_H

#include "tender/infrastructure/iusecasegetterptr.h"
#include <libbasar_definitions.h>
#include <libbasarcmnutil_i18nstring.h>
#include <libtender/domainmodule/tender/tenderimportstruct.h>
#include <libtender/domainmodule/scheduleddelivery/scheduleddeliveryimportstruct.h>
#include "ierrorhandlerptr.h"
#include <libtender/domainmodule/tender/tendertypecollectionptr.h>
#include <libtender/domainmodule/tender/tenderstatecollectionptr.h>
#include <libtender/domainmodule/article/iarticlecollectiondmptr.h>
#include <libtender/domainmodule/customer/icustomercollectiondmptr.h>
#include <libtender/domainmodule/pharmacychain/ipharmacychaincollectiondmptr.h>
#include <libtender/domainmodule/partner/ipartnercollectiondmptr.h>
#include <libtender/domainmodule/tender/itenderimportcollectionptr.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <set>
#include <map>

namespace tender
{
namespace useCase
{
class TenderImportChecker
{
public:
	TenderImportChecker();
	virtual ~TenderImportChecker();

	void injectUseCaseGetter(tender::componentManager::IUseCaseGetterPtr);
	void inject( IErrorHandlerPtr errorHandler );
	void inject( libtender::domMod::tender::TenderTypeCollectionPtr tenderTypeCollection );
	void inject( libtender::domMod::tender::TenderStateCollectionPtr tenderStateCollection );
	void inject( libtender::domMod::article::IArticleCollectionDMPtr articleCollection );
	void inject( libtender::domMod::customer::ICustomerCollectionDMPtr customerCollection );
	void inject( libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr pharmacyChainCollection );
	void inject( libtender::domMod::partner::IPartnerCollectionDMPtr partnerCollection );
	void inject( libtender::domMod::tender::ITenderImportCollectionPtr tenderImportCollection );

	void setArea( basar::Int16 area );

	bool checkTenderPosition( const basar::Int32 lineNumber, libtender::domMod::tender::TenderImportStruct & positionData ) const;
	bool checkScheduledDeliveryPosition( const basar::Int32 lineNumber, libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct & positionData, 
		const basar::db::aspect::AccessorPropertyTable_YIterator tenderHead) const;
	void reset();
	
	basar::Int32 getArticleNoByArticleCode( const basar::Int32 tenderType, const basar::VarString & articleCode ) const;
private:
	TenderImportChecker( const TenderImportChecker & );
	TenderImportChecker operator=( const TenderImportChecker & );

	bool checkCustomer( const basar::Int32 pharmacyChainNo ) const;
	bool checkCustomerToPharmacyChain( const basar::Int32 pharmacyChainNo, const basar::Int32 customerNo, const basar::Int16 branchNo ) const;
	bool checkBranch( const basar::Int16 branchNo ) const;
	bool checkPharmacyChain( const basar::Int32 pharmacyChainNo ) const;
	bool checkCustomerGroupID( const basar::Int32 pharmacyChainNo, const basar::I18nString customerGroupID ) const;
	bool checkPartnerNo( const basar::Int32 partnerNo, const basar::Int32 tenderType ) const;
	bool checkArticle( const basar::VarString & articleCode, const basar::Int32 tenderType, basar::Int32 & articleNo ) const;
	bool checkPeriod( const basar::Date & dateFrom, const basar::Date & dateTo ) const;
	bool checkContractNo( const basar::I18nString & contractNo ) const;
	bool checkTenderType( const basar::Int32 tenderType ) const;
	bool checkTenderState( const basar::Int16 tenderState ) const;
	bool checkContractQuantity( const basar::Int32 contractQty ) const;
	bool checkContractPrice( const basar::Decimal & contractPrice, const basar::Decimal & discountPct ) const;

	bool checkID( const basar::Int32 id ) const;
	bool checkDeliveryDate( const basar::Int32 id, const basar::Date & deliveryDate ) const;
	bool checkArticle( const basar::Int32 id, const basar::VarString & articleCode ) const;
	bool checkQuantity( const basar::Int32 id, const basar::VarString & articleCode, const basar::Int32 quantity ) const;
	bool checkPurchaseOrderProposalFlag( const basar::Int16 purchaseOrderProposalFlag ) const;

	typedef std::map<basar::VarString, basar::Int32> ArticleCodeCollection;	// articleCodes -> articleNo
	ArticleCodeCollection & getValidArticles( const basar::Int32 tenderType ) const;
	ArticleCodeCollection & getInvalidArticles( const basar::Int32 tenderType ) const;

	mutable std::set<basar::Int32>		m_ValidCustomers;
	mutable std::set<basar::Int32>		m_InvalidCustomers;

	mutable std::set<basar::Int32>		m_ValidPharmacyChain;
	mutable std::set<basar::Int32>		m_InvalidPharmacyChain;

	typedef std::set<basar::Int32> PartnerNoCollection;
	PartnerNoCollection & getValidPartners( const basar::Int32 tenderType ) const;
	PartnerNoCollection & getInvalidPartners( const basar::Int32 tenderType ) const;

	typedef std::map<basar::Int32, ArticleCodeCollection> ArticleCodeMap;	// tenderType -> articleCodes
	mutable ArticleCodeMap	m_ValidArticles;	
	mutable ArticleCodeMap	m_InvalidArticles;

	typedef std::map<basar::Int32, PartnerNoCollection> PartnerNoMap;	// tenderType -> partnerNumbers
	mutable PartnerNoMap	m_ValidPartners;	
	mutable PartnerNoMap	m_InvalidPartners;

	typedef std::map<basar::VarString, basar::Int32> ScheduledQuantityCollection;					// articleCode -> quantity
	typedef std::map<basar::Int32, ScheduledQuantityCollection> TenderScheduledQuantityCollection;	// tender "id" -> ScheduledQuantityCollection

	ScheduledQuantityCollection & getScheduledQuantities( const basar::Int32 id ) const;
	mutable TenderScheduledQuantityCollection	m_ScheduledQuantities;

	bool	isPartnerNeeded(const basar::Int32 tenderType ) const;

	tender::componentManager::IUseCaseGetterPtr						m_UseCaseGetter;
	IErrorHandlerPtr												m_ErrorHandler;
	libtender::domMod::tender::TenderTypeCollectionPtr				m_TenderTypeCollection;
	libtender::domMod::tender::TenderStateCollectionPtr				m_TenderStateCollection;
	libtender::domMod::article::IArticleCollectionDMPtr				m_ArticleCollection;
	libtender::domMod::customer::ICustomerCollectionDMPtr			m_CustomerCollection;
	libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr	m_PharmacyChainCollection;
	libtender::domMod::partner::IPartnerCollectionDMPtr				m_PartnerCollection;
	libtender::domMod::tender::ITenderImportCollectionPtr			m_TenderImportCollection;
	mutable basar::Int32											m_LineNumber;

	basar::Int16													m_Area;
};

} // namespace useCase
} // namespace tender

#endif // GUARD_TENDER_USECASE_IMPORT_CHECKER_H
