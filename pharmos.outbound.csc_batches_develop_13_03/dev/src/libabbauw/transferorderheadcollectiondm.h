#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEADCOLLECTIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEADCOLLECTIONDM_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "itransferorderheadcollectiondm.h"
#include "transferorderheadcollectioninitparams.h"
#include "itransferorderheadptr.h"
#include "istockcollectiondmptr.h"
#include "ireservequantityfactorgetterptr.h"

#include "componentmanager/itransferorderfactoryptr.h"
#include "componentmanager/itransferorderheadfactoryptr.h"
#include "componentmanager/ireservequantityfactorgetterfactoryptr.h"

#include "iorderparameterptr.h"

#include "dmbase.h"
#include <libutil/misc/componentinitialization.hpp>

#include <boost/unordered_map.hpp>

namespace libutil
{
	class ClassInfo;
}

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			class TransferOrderHeadCollectionDM : public libutil::misc::ComponentInitialization< TransferOrderHeadCollectionInitParams, TransferOrderHeadCollectionDM >, public ITransferOrderHeadCollectionDM, private DMBase
			{

			public:
				TransferOrderHeadCollectionDM(basar::db::aspect::ConnectionRef);
				~TransferOrderHeadCollectionDM();

				void                                                    injectStockCollection(stock::IStockCollectionDMPtr);
				void                                                    injectOrderParameter(parameter::IOrderParameterPtr);

				void                                                    injectOrderFactory(componentManager::ITransferOrderFactoryPtr);
				void                                                    injectOrderHeadFactory(componentManager::ITransferOrderHeadFactoryPtr);

				void                                                    injectReserveQuantityFactorGetterFactory(componentManager::IReserveQuantityFactorGetterFactoryPtr);

				// defined in ITransferOrderHeadCollectionGetter
				basar::db::aspect::AccessorPropertyTableRef             get();
				basar::db::aspect::AccessorPropertyTableRef             get() const;

				// defined in transfer order interface
				void                                                    resetSearchYit();
				basar::db::aspect::AccessorPropertyTable_YIterator      getSearchYit();

				void                                                    getGreenOrders(YIterators& greenOrders);
				basar::Int32                                            getNumberOfGreenOrders() const;
				basar::Int32                                            getTotalNumberOfOrders();

				bool                                                    save(basar::VarString strDiscount);

				void													setBackupAllHeadPosCollection();				
				std::vector<AccessorPropertyTable_YIterator>			getBackupAllHeadPosCollection();

				void                                                    findByPattern(const basar::db::aspect::AccessorPropertyTable_YIterator, bool isOrderView);
				void                                                    findByKey(const basar::db::aspect::AccessorPropertyTable_YIterator);
				bool                                                    isChanged() const;

				transferOrder::ITransferOrderPtr                        getOrder(const basar::db::aspect::AccessorPropertyTable_YIterator);

				// used only for CPR0002688 to avoid double locking
				transferOrder::ITransferOrderPtr                        getOrderWithoutLock(const basar::db::aspect::AccessorPropertyTable_YIterator);

				void                                                    setReserveQtyFactorType(ReserveQuantityFactorType);

				void                                                    addTransferOrderHead(basar::db::aspect::AccessorPropertyTable_YIterator);

			public:
				static const libutil::misc::ClassInfo& getClassInfo();

			protected:
				// from ComponentInitialization
				void                                                    doInit(const TransferOrderHeadCollectionInitParams&);
				void                                                    doShutdown();

				// from DMBase
				AccessorExecuteParameterization                         getFindByPatternExecuteParameters() const;
				AccessorExecuteParameterization                         getFindByKeyExecuteParameters() const;
				basar::VarString                                        getSearchYIteratorAttributeNames()  const;

			private:
				transferOrder::ITransferOrderHeadPtr                    getHead(const basar::db::aspect::AccessorPropertyTable_YIterator) const;

				void                                                    calculatePositionDiscount();
				void                                                    findStock();
				void                                                    calculateMissingItemsForEachTransferOrder(bool isOrderView);
				basar::Int16                                            calculateOMColor(const basar::db::aspect::AccessorPropertyTable_YIterator stockIt);
				basar::Int16                                            calculateCMColor(basar::db::aspect::AccessorPropertyTable_YIterator stockIt);
				basar::Int16                                            calculateUWColor(basar::db::aspect::AccessorPropertyTable_YIterator stockIt, bool isOrderView);
				basar::Int32                                            calculateRunningStock(const basar::db::aspect::AccessorPropertyTable_YIterator stockIt);
				basar::Int32                                            calculateRunningStockCM(const basar::db::aspect::AccessorPropertyTable_YIterator stockIt);
				bool                                                    isWithinDateRange(const basar::Date&) const;
				bool                                                    isOMOrder(const basar::VarString& orderType);
				bool                                                    isCMOrder(const basar::VarString& orderType);

				basar::Int32                                            calculateMinimalStock(const basar::db::aspect::AccessorPropertyTable_YIterator, bool isOrderView) const;

				IReserveQuantityFactorGetterPtr                         getReserveQuantityFactorGetter() const;

			private:
				TransferOrderHeadCollectionDM(const TransferOrderHeadCollectionDM&);
				TransferOrderHeadCollectionDM operator = (const TransferOrderHeadCollectionDM&);

				typedef boost::unordered_map< basar::Int32, basar::Int32 > articleStockPairs;

				TransferOrderHeadCollectionInitParams						m_InitParams;
				stock::IStockCollectionDMPtr								m_StockCollection;
				parameter::IOrderParameterPtr								m_OrderParameter;
				IReserveQuantityFactorGetterPtr								m_ReserveQtyFactorGetter; // to cache value from factory (performance!)

				std::vector<AccessorPropertyTable_YIterator>  				m_BackUpAllHeadPosCollection;

				componentManager::ITransferOrderFactoryPtr					m_OrderFactory;
				componentManager::ITransferOrderHeadFactoryPtr				m_HeadFactory;
				componentManager::IReserveQuantityFactorGetterFactoryPtr	m_ReserveQuantityFactorGetterFactory;

				articleStockPairs											m_ArticleStockCollection;
				articleStockPairs											m_ArticleCMIStockCollection;
				basar::Int32												m_GreenOrdersCount;
				basar::Int32												m_DaysToColor;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEADCOLLECTIONDM_H
