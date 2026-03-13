#include "cmndm.h"
#include "stockcollectiondm.h"
#include "stockcollection_definitions.h"
#include "stockcollectionacc.h"

#include "istockparameter.h"

namespace libabbauw
{
	namespace domMod
	{
		namespace stock
		{

			const libutil::misc::ClassInfo& StockCollectionDM::getClassInfo()
			{
				static const libutil::misc::ClassInfo info("stock::StockCollectionDM", libutil::misc::Version(1, 0, 0, 0));
				return info;
			}

			StockCollectionDM::StockCollectionDM(basar::db::aspect::ConnectionRef connection)
				: DMBase(connection, domMod::stock::ACC_STOCK, basar::db::aspect::FULL_CACHING, libabbauw::LoggerPool::loggerStock)
			{
				METHODNAME_DEF(StockCollectionDM, StockCollectionDM)
					BLOG_TRACE_METHOD(getLogger(), fun);

				acc_stock::SelectStock(); // don't let the linker eliminate the accessor
			}

			StockCollectionDM::~StockCollectionDM()
			{
				METHODNAME_DEF(StockCollectionDM, ~StockCollectionDM)
					BLOG_TRACE_METHOD(getLogger(), fun);

				shutdown();
			}

			void StockCollectionDM::injectStockCalcParam(parameter::IStockParameterPtr stockCalcParamPtr)
			{
				m_StockParameterPtr = stockCalcParamPtr;
			}

			AccessorExecuteParameterization StockCollectionDM::getFindByPatternExecuteParameters() const
			{
				METHODNAME_DEF(StockCollectionDM, getFindByPatternExecuteParameters)
					BLOG_TRACE_METHOD(getLogger(), fun);

				return AccessorExecuteParameterization(stock::SELECT_STOCK);
			}

			basar::VarString StockCollectionDM::getSearchYIteratorAttributeNames()  const
			{
				METHODNAME_DEF(StockCollectionDM, getSearchYIteratorAttributeNames)
					BLOG_TRACE_METHOD(getLogger(), fun);

				using namespace libabbauw::properties;

				return properties::BRANCHNO.toDescriptionString() +
					properties::ORDERNO.toDescriptionString() +
					properties::PREDICTIONDATE.toDescriptionString() +
					properties::ARTICLENO.toDescriptionString();
			}

			void StockCollectionDM::doInit(const StockCollectionInitParams& p)
			{
				METHODNAME_DEF(StockCollectionDM, doInit)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_InitParams = p;
				m_PseudoStorageLocationSlot = findPseudoStorageLocationSlot();
				m_PredictionDate = findLastPredictionDate();
			}

			void StockCollectionDM::doShutdown()
			{
				METHODNAME_DEF(StockCollectionDM, doShutdown)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			void StockCollectionDM::resetSearchYit()
			{
				DMBase::resetSearchYit();
			}

			basar::db::aspect::AccessorPropertyTableRef StockCollectionDM::get()
			{
				return DMBase::getPropTab();
			}

			basar::db::aspect::AccessorPropertyTable_YIterator StockCollectionDM::findByOrderNo(basar::Int32 orderNo)
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yit = get().end();

				PropTabIndex::iterator indexIt = m_IndexOrderNo.find(orderNo);
				if (indexIt != m_IndexOrderNo.end())
					yit = indexIt->second;

				// yit contains the orderdata (only keys for order and positions) with stock for the given orderno 
				return yit;
			}

			basar::db::aspect::AccessorPropertyTable_YIterator StockCollectionDM::findOrderPositionInOrder(basar::Int32 orderNo, basar::Int32 position)
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yit = findByOrderNo(orderNo);

				for (; !yit.isEnd() && yit.getInt32(libabbauw::properties::ORDERNO) == orderNo
					&& yit.getInt32(libabbauw::properties::POSNO) != position; ++yit);

				return yit;
			}

			basar::db::aspect::AccessorPropertyTable_YIterator StockCollectionDM::getSearchYit()
			{
				return DMBase::getSearchYit();
			}

			void StockCollectionDM::findStock(const basar::db::aspect::AccessorPropertyTable_YIterator yIt)
			{
				METHODNAME_DEF(StockCollectionDM, findStockByOrderNo)
					BLOG_TRACE_METHOD(getLogger(), fun);


#ifdef _DEBUG
				std::stringstream strstream;
				yIt.toStream(strstream);
			/*	BLOG_TRACE(LoggerPool::loggerDomModules, "### SPION CSCOrderDM::saveBatchOrderByPattern yit");
				BLOG_TRACE(LoggerPool::loggerDomModules, strstream.str());*/

#endif

				basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = yIt;
				yitSearchTmp.setInt16(properties::BRANCHNO, m_InitParams.m_BranchNo);
				yitSearchTmp.setInt32(properties::PREDICTIONDATE, m_PredictionDate);
				DMBase::findByPattern(yitSearchTmp);

				createIndex();
			}

			basar::Int32 StockCollectionDM::findCMIReservedQuantity(basar::Int32 articleNo)
			{
				METHODNAME_DEF(StockCollectionDM, findCMIReservedQuantity)
					BLOG_TRACE_METHOD(getLogger(), fun);

				basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = getSearchYit();
				yitSearchTmp.setInt16(properties::BRANCHNO, m_InitParams.m_BranchNo);
				yitSearchTmp.setInt32(properties::ARTICLENO, articleNo);

				const std::pair< bool, basar::Decimal > result = getAccessor()->executeAggregate(SELECT_CMI_RESERVED_QUANTITY, yitSearchTmp);

				basar::Int32 cmiReservedQuantity = 0;
				if (result.first)
				{
					cmiReservedQuantity = result.second.toInt32();
				}

				return cmiReservedQuantity;
			}

			bool StockCollectionDM::itemInStock(const basar::db::aspect::AccessorPropertyTable_YIterator itemIt)
			{
				METHODNAME_DEF(StockCollectionDM, itemInStock)
					BLOG_TRACE_METHOD(getLogger(), fun);

				const basar::VarString storageLocSlot = itemIt.getString(properties::STORAGELOCATIONSLOT);
				const bool hasQty = itemIt.getInt32(properties::ORDEREDQTY) > 0;
				return !hasQty || (storageLocSlot != m_PseudoStorageLocationSlot);
			}

			bool StockCollectionDM::itemInTransitStorage(const basar::db::aspect::AccessorPropertyTable_YIterator itemIt)
			{
				METHODNAME_DEF(StockCollectionDM, itemInTransitStorage)
					BLOG_TRACE_METHOD(getLogger(), fun);

				basar::VarString storageLocSlot = itemIt.getString(properties::STORAGELOCATIONSLOT);
				return storageLocSlot == m_StockParameterPtr->getTransitStorageLocationSlot();
			}

			bool StockCollectionDM::orderContainsNonStockItem(const basar::Int32 orderNo)
			{
				METHODNAME_DEF(StockCollectionDM, containsNonStockItem)
					BLOG_TRACE_METHOD(getLogger(), fun);
				using basar::db::aspect::AccessorPropertyTable_YIterator;

				bool nonStockItem = false;
				for (AccessorPropertyTable_YIterator stockIt = findByOrderNo(orderNo); !stockIt.isEnd() &&
					stockIt.getInt32(libabbauw::properties::ORDERNO) == orderNo; ++stockIt)
				{
					if (!itemInStock(stockIt))
					{
						nonStockItem = true;
						break;
					}
				}

				return nonStockItem;
			}

			basar::Int32 StockCollectionDM::findLastPredictionDate()
			{
				METHODNAME_DEF(StockCollectionDM, findLastPredictionDate)
					BLOG_TRACE_METHOD(getLogger(), fun);

				basar::db::aspect::AccessorPropertyTable_YIterator searchYit = DMBase::getSearchYit();
				searchYit.setInt16(properties::BRANCHNO, m_InitParams.m_BranchNo);

				basar::Int32 total = 0;

				const std::pair< bool, basar::Decimal > result = getAccessor()->executeAggregate(SELECT_PREDICTION_DATE, searchYit);
				if (result.first)
				{
					total = result.second.toInt32();
				}

				return total;
			}

			basar::VarString StockCollectionDM::findPseudoStorageLocationSlot()
			{
				METHODNAME_DEF(StockCollectionDM, findPseudoStorageLocationSlot)
					BLOG_TRACE_METHOD(getLogger(), fun);

				basar::db::aspect::AccessorPropertyTable_YIterator searchYit = DMBase::getSearchYit();
				searchYit.setInt16(libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo);

				basar::db::aspect::ExecuteResultInfo res = getAccessor()->execute(SELECT_PSEUDO_STORAGE_LOCATION_SLOT, searchYit);
				if (res.hasError())
				{
					THROW_ACCESSOR_EXECUTION_EXCEPTION(fun, SELECT_PSEUDO_STORAGE_LOCATION_SLOT, res);
				}

				basar::VarString pseudoStorageLocationSlot;
				if (!get().empty())
					pseudoStorageLocationSlot = get().begin().getString(properties::PSEUDOSTORAGELOCATIONSLOT);

				return pseudoStorageLocationSlot;
			}

			// Index on ordernumbers on the hashtable 
			void StockCollectionDM::createIndex()
			{
				m_IndexOrderNo.clear(); // delete old index
				// create new index
				basar::Int32 orderno = 0;
				for (basar::db::aspect::AccessorPropertyTable_YIterator it = get().begin(); !it.isEnd(); ++it)
				{
					basar::Int32 no = it.getInt32(libabbauw::properties::ORDERNO);
					if (orderno != no)
					{
						orderno = no;
						m_IndexOrderNo[no] = it;
					}
				}
			}

		} // end namespace stock
	} // end namespace domMod
} // end namespace libabbauw
