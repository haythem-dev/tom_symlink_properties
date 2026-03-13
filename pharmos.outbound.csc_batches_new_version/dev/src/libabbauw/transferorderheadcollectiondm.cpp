#include "cmndm.h"

#include "transferorderheadcollectiondm.h"
#include "transferorderheadcollection_definitions.h"

#include "transferorderheadcollectionacc.h"
#include "itransferorderhead.h"
#include "itransferorderposcollection.h"
#include "istockcollectiondm.h"
#include "iorderparameter.h"
#include "trafficlightcolorsenum.h"
#include "ireservequantityfactorgetter.h"

#include "componentmanager/itransferorderfactory.h"
#include "componentmanager/itransferorderheadfactory.h"
#include "componentmanager/ireservequantityfactorgetterfactory.h"

#include "yiteratormatcher.h"
#include <libutil/misc/instancechecker_macro_definition.h>
#include <libutil/misc/datetime/datetime.h>
#include <itransferorder.h>
#include <itransferorderpositionptr.h>
#include <itransferordergetterptr.h>

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			using basar::db::aspect::AccessorPropertyTable_YIterator;
			using namespace libabbauw::properties;

			const libutil::misc::ClassInfo& TransferOrderHeadCollectionDM::getClassInfo()
			{
				static const libutil::misc::ClassInfo info("transferOrder::TransferOrderHeadCollectionDM", libutil::misc::Version(1, 0, 0, 0));
				return info;
			}

			TransferOrderHeadCollectionDM::TransferOrderHeadCollectionDM(basar::db::aspect::ConnectionRef connection)
				: DMBase(connection, domMod::transferOrder::ACC_TRANSFER_ORDERS, basar::db::aspect::FULL_CACHING, libabbauw::LoggerPool::loggerTransferOrder),
				m_GreenOrdersCount(0)
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, TransferOrderHeadCollectionDM)
					BLOG_TRACE_METHOD(getLogger(), fun);

				// avoid runtime error
				acc_transfer_order_head_collection::SelectTransferOrders();
			}

			TransferOrderHeadCollectionDM::~TransferOrderHeadCollectionDM()
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, ~TransferOrderHeadCollectionDM)
					BLOG_TRACE_METHOD(getLogger(), fun);

				shutdown();
			}

			void TransferOrderHeadCollectionDM::injectStockCollection(stock::IStockCollectionDMPtr stockColl)
			{
				m_StockCollection = stockColl; // uses StockCollectionDM for Bestandsabfragen.
			}

			void TransferOrderHeadCollectionDM::injectOrderParameter(parameter::IOrderParameterPtr orderParam)
			{
				m_OrderParameter = orderParam;
			}

			void TransferOrderHeadCollectionDM::injectOrderHeadFactory(componentManager::ITransferOrderHeadFactoryPtr orderHeadFactory)
			{
				m_HeadFactory = orderHeadFactory;
			}

			void TransferOrderHeadCollectionDM::injectOrderFactory(componentManager::ITransferOrderFactoryPtr orderFactory)
			{
				m_OrderFactory = orderFactory;
			}

			void TransferOrderHeadCollectionDM::injectReserveQuantityFactorGetterFactory(componentManager::IReserveQuantityFactorGetterFactoryPtr resQtyFactorGetterFactory)
			{
				m_ReserveQuantityFactorGetterFactory = resQtyFactorGetterFactory;
			}

			AccessorExecuteParameterization TransferOrderHeadCollectionDM::getFindByPatternExecuteParameters() const
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, getFindByPatternExecuteParameters)
					BLOG_TRACE_METHOD(getLogger(), fun);

				return AccessorExecuteParameterization(SELECT_TRANSFER_ORDERS);
			}

			AccessorExecuteParameterization TransferOrderHeadCollectionDM::getFindByKeyExecuteParameters() const
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, getFindByKeyExecuteParameters)
					BLOG_TRACE_METHOD(getLogger(), fun);

				return AccessorExecuteParameterization(SELECT_TRANSFER_ORDER_BY_KEY);
			}

			basar::VarString TransferOrderHeadCollectionDM::getSearchYIteratorAttributeNames()  const
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, getSearchYIteratorAttributeNames)
					BLOG_TRACE_METHOD(getLogger(), fun);

				return properties::BRANCHNO.toDescriptionString() +
					properties::MANUFACTURERNO.toDescriptionString() +
					properties::MANUFACTURERNAME.toDescriptionString() +
					properties::PZN.toDescriptionString() +
					properties::PHARMACYNO.toDescriptionString() +
					properties::PHARMACYNAME.toDescriptionString() +
					properties::LOCATION.toDescriptionString() +
					properties::ORDERNO_FROM.toDescriptionString() +
					properties::ORDERNO_TO.toDescriptionString() +
					properties::ORDERTYPE.toDescriptionString() +
					properties::STATUS.toDescriptionString() +
					properties::NONSTOCKITEM.toDescriptionString() +
					properties::LARGESCALEORDER.toDescriptionString() +
					properties::LARGESCALEORDERLIMIT.toDescriptionString() +
					properties::POS_FROM.toDescriptionString() +
					properties::POS_TO.toDescriptionString() +
					properties::ORDERDATE_FROM.toDescriptionString() +
					properties::ORDERDATE_TO.toDescriptionString() +
					properties::EDIDELIVERYDATE_FROM.toDescriptionString() +
					properties::EDIDELIVERYDATE_TO.toDescriptionString() +
					properties::DEFERREDPAYMENTDATE_FROM.toDescriptionString() +
					properties::DEFERREDPAYMENTDATE_TO.toDescriptionString() +
					properties::ORDERVALUE_FROM.toDescriptionString() +
					properties::ORDERVALUE_TO.toDescriptionString() +
					properties::SORT.toDescriptionString() +
					properties::DAYS_TO_COLOR.toDescriptionString()
					;
			}

			void TransferOrderHeadCollectionDM::doInit(const TransferOrderHeadCollectionInitParams& p)
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, doInit)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_InitParams = p;
			}

			void TransferOrderHeadCollectionDM::doShutdown()
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, doShutdown)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			void TransferOrderHeadCollectionDM::resetSearchYit()
			{
				DMBase::resetSearchYit();
			}

			basar::db::aspect::AccessorPropertyTableRef TransferOrderHeadCollectionDM::get()
			{
				return DMBase::getPropTab();
			}

			basar::db::aspect::AccessorPropertyTableRef TransferOrderHeadCollectionDM::get() const
			{
				return DMBase::getPropTab();
			}

			AccessorPropertyTable_YIterator TransferOrderHeadCollectionDM::getSearchYit()
			{
				return DMBase::getSearchYit();
			}

			void TransferOrderHeadCollectionDM::findByPattern(const AccessorPropertyTable_YIterator yIt, bool isOrderView)
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, findByPattern)
					BLOG_TRACE_METHOD(getLogger(), fun);

				AccessorPropertyTable_YIterator yitSearchTmp = yIt;
				yitSearchTmp.setInt16(properties::BRANCHNO, m_InitParams.m_BranchNo);

				// add large scale order limit to SQL
				const basar::Int32 largeScaleOrderLimit = m_OrderParameter->getLargeScaleOrderLimit();
				yitSearchTmp.setInt32(properties::LARGESCALEORDERLIMIT, largeScaleOrderLimit);

				// use day to color from searchYit if any otherwise use it from default parameter
				if (getSearchYit().isContainedAndSet(properties::DAYS_TO_COLOR))
				{
					m_DaysToColor = getSearchYit().getInt32(properties::DAYS_TO_COLOR);
				}
				else
				{
					m_DaysToColor = m_OrderParameter->getDaysToColor();
				}

				DMBase::findByPattern(yitSearchTmp);

				findStock();
				calculateMissingItemsForEachTransferOrder(isOrderView);
				//calculatePositionDiscount();
			}

			void TransferOrderHeadCollectionDM::findByKey(const AccessorPropertyTable_YIterator yit)
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, findByKey)
					BLOG_TRACE_METHOD(getLogger(), fun);

				AccessorPropertyTable_YIterator yitSearchTmp = yit;
				yitSearchTmp.setInt16(properties::BRANCHNO, m_InitParams.m_BranchNo);

				yitSearchTmp.setInt32(properties::ORDERNO, yit.getInt32(properties::ORDERNO));

				DMBase::findByKey(yitSearchTmp);
			}

			basar::Decimal absDecimal(const basar::Decimal& value)
			{
				return (value < basar::Decimal(0)) ? (value * basar::Decimal(-1)) : value;
			}

			void TransferOrderHeadCollectionDM::calculatePositionDiscount()
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, calculatePositionDiscount)
					BLOG_TRACE_METHOD(getLogger(), fun);

				// The propTab contains all orders from the main screen but no positions
				for (AccessorPropertyTable_YIterator orderIt = getPropTab().begin(); !orderIt.isEnd(); ++orderIt)
				{
					const basar::Int32 orderNo = orderIt.getInt32(ORDERNO);

					if (orderNo == 0)
						continue;

					basar::Decimal minValue(999.0);
					basar::Decimal maxValue(-999.0);
					bool started = false;

					// iterate over all order positions which belong to same order
					for (AccessorPropertyTable_YIterator posIt = m_StockCollection->findByOrderNo(orderNo); !posIt.isEnd() && posIt.getInt32(ORDERNO) == orderNo; ++posIt)
					{
						// get the order number
						const basar::Decimal discountPct = posIt.getDecimal(properties::DISCOUNTPCT);

						if (started == false)
						{
							minValue = discountPct;
							maxValue = discountPct;
							started = true;
						}
						else
						{
							if (discountPct < minValue)
							{
								minValue = discountPct;
							}
							if (maxValue < discountPct)
							{
								maxValue = discountPct;
							}
						}
					}

					// do we had any positions in stock collection?
					if (started == false)
					{
						// no positions found, so we have to check the order positions directly
						libabbauw::domMod::transferOrder::ITransferOrderPtr transferOrder = getOrderWithoutLock(orderIt);

						for (AccessorPropertyTable_YIterator yitPositions = transferOrder->getPosCollection()->get().begin(); !yitPositions.isEnd(); ++yitPositions)
						{
							// get the order number
							const basar::Decimal discountPct = yitPositions.getDecimal(properties::DISCOUNTPCT);

							if (started == false)
							{
								minValue = discountPct;
								maxValue = discountPct;
								started = true;
							}
							else
							{
								if (discountPct < minValue)
								{
									minValue = discountPct;
								}
								if (maxValue < discountPct)
								{
									maxValue = discountPct;
								}
							}
						}
					}

					std::string value = "" + minValue.toString();
					if (minValue != maxValue)
						value += " - " + maxValue.toString();

					std::cout << "Value: " << value;

					orderIt.setString(DISCOUNTPCTRANGE, value);
				}
			}

			void TransferOrderHeadCollectionDM::findStock()
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, findStock)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_StockCollection->resetSearchYit();
				m_StockCollection->findStock(m_StockCollection->getSearchYit());
			}

			bool TransferOrderHeadCollectionDM::isWithinDateRange(const basar::Date& deliveryDate) const
			{
				bool withinRange = false;

				// empty date is always within date range
				if (!deliveryDate.isValid())
				{
					withinRange = true;
				}
				else
				{
					libutil::misc::DateTime date(deliveryDate), today(basar::cmnutil::Date::getCurrent());
					date.addDays(m_DaysToColor * -1);
					withinRange = (today >= date);
				}

				return withinRange;
			}

			bool TransferOrderHeadCollectionDM::isOMOrder(const basar::VarString& orderType)
			{
				return orderType.substr(0, lit::OMG_ORDERTYPE.length()).compare(lit::OMG_ORDERTYPE) == 0;
			}

			bool TransferOrderHeadCollectionDM::isCMOrder(const basar::VarString& orderType)
			{
				return orderType.substr(0, lit::DG_ORDERTYPE.length()).compare(lit::DG_ORDERTYPE) == 0;
			}

			basar::Int32 TransferOrderHeadCollectionDM::calculateMinimalStock(const AccessorPropertyTable_YIterator stockIt, bool isOrderView) const
			{
				// this method calculates the reserved amount of goods which always have to be in stock
				const basar::Decimal prediction(stockIt.getFloat64(properties::PREDICTION));

				const basar::Int32 minimalStock = stockIt.getInt32(properties::MINSTOCK);

				const basar::Decimal reserveQtyFactor = getReserveQuantityFactorGetter()->getReserveQtyFactor(stockIt.getString(properties::ORDERTYPE), isOrderView);
				basar::Decimal predictedStock = reserveQtyFactor * prediction;

				predictedStock.round(0);
				basar::Int32 stock = predictedStock.toInt32();

				if (minimalStock > stock)
				{
					stock = minimalStock;
				}

				return stock;
			}

			// here is the calculation of the recursive reduction of stocklevels depending on the rank of an order in the orderlist
			basar::Int32 TransferOrderHeadCollectionDM::calculateRunningStock(const AccessorPropertyTable_YIterator stockIt)
			{
				basar::Int32 orderedQty = stockIt.getInt32(properties::ORDEREDQTY);
				basar::Int32 articleNo = stockIt.getInt32(properties::ARTICLENO);

				basar::Int32 stock;
				if (articleNo == 0)  // article not found in DB
					stock = -1 * orderedQty;
				else
				{
					// did we ever calculate with this article before (has it been in the orderscreen on a higher position?)
					articleStockPairs::iterator aStockIt = m_ArticleStockCollection.find(articleNo);
					if (aStockIt != m_ArticleStockCollection.end()) // stock found
					{
						aStockIt->second -= orderedQty;
						stock = aStockIt->second;
					}
					else  // no stock; take the value from the database, reduced by the quantity and minimal stock for this position  
					{
						stock = stockIt.getInt32(properties::STOCK) - orderedQty;
						// store the newly detected article with its stockrest into the collection
						m_ArticleStockCollection.insert(std::pair< basar::Int32, basar::Int32 >(articleNo, stock));
					}
				}

				return stock;
			}

			// here is the calculation of the recursive reduction of stocklevels depending on the rank of an order in the orderlist
			basar::Int32 TransferOrderHeadCollectionDM::calculateRunningStockCM(const AccessorPropertyTable_YIterator stockIt)
			{
				const basar::Int32 orderedQty = stockIt.getInt32(properties::ORDEREDQTY);
				const basar::Int32 articleNo = stockIt.getInt32(properties::ARTICLENO);
				basar::Int32 stock = 0;

				if (articleNo == 0)  // article not found in DB
				{
					stock = -1 * orderedQty;
				}
				else
				{
					// did we ever calculate with this article before (has it been in the orderscreen on a higher position?)
					articleStockPairs::iterator aStockIt = m_ArticleCMIStockCollection.find(articleNo);
					if (aStockIt != m_ArticleCMIStockCollection.end())
					{
						aStockIt->second -= orderedQty;
						stock = aStockIt->second;
					}
					else  // no stock; take the value from the database for this position  
					{
						// store the newly detected article with its stockrest into the collection
						stock = m_StockCollection->findCMIReservedQuantity(articleNo) - orderedQty;

						m_ArticleCMIStockCollection.insert(std::pair< basar::Int32, basar::Int32 >(articleNo, stock));
					}
				}

				return stock;
			}

			basar::Int16 TransferOrderHeadCollectionDM::calculateOMColor(const AccessorPropertyTable_YIterator stockIt)
			{
				basar::Int16 color = NO_COLOR;

				const basar::Int32 orderedQty = stockIt.getInt32(libabbauw::properties::ORDEREDQTY);
				if (orderedQty > 0 && (!m_StockCollection->itemInStock(stockIt) || m_StockCollection->itemInTransitStorage(stockIt)))
				{
					color = PURPLE;
				}

				return color;
			}

			basar::Int16 TransferOrderHeadCollectionDM::calculateCMColor(AccessorPropertyTable_YIterator stockIt)
			{
				basar::Int16 color = GREEN;

				const basar::Int32 orderedQty = stockIt.getInt32(libabbauw::properties::ORDEREDQTY);
				const basar::Int32 stock = calculateRunningStockCM(stockIt);
				if (orderedQty > 0 && stock < 0)
				{
					if (stock + stockIt.getInt32(properties::BACKORDER_NOT_EFFECTIVE) >= 0)  // calculation for orderrueckstand !UNWIRKSAM!
					{
						color = YELLOW;
					}
					else
					{
						color = RED;
					}
				}

				stockIt.setInt32(properties::STOCKACCUMULATED, stock); // update available stock

				return color;
			}

			basar::Int16 TransferOrderHeadCollectionDM::calculateUWColor(AccessorPropertyTable_YIterator stockIt, bool isOrderView)
			{
				basar::Int16 color = GREEN;

				// substract minimal remaining stock and add reserved stock from pharmacy group if any
				const basar::Int32 orderedQty = stockIt.getInt32(properties::ORDEREDQTY);
				const basar::Int32 stock = calculateRunningStock(stockIt) - calculateMinimalStock(stockIt, isOrderView) + stockIt.getInt32(properties::RESERVEDQTY);
				if (orderedQty > 0 && stock < 0)
				{
					if (stock + stockIt.getInt32(properties::BACKORDER) >= 0)  // calculation for orderrueckstand
					{
						color = YELLOW;
					}
					else
					{
						color = RED;
					}
				}
				stockIt.setInt32(properties::STOCKACCUMULATED, stock); // update available stock

				return color;
			}

			void TransferOrderHeadCollectionDM::calculateMissingItemsForEachTransferOrder(bool isOrderView)
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, calculateMissingItemsForEachTransferOrder)
					BLOG_TRACE_METHOD(getLogger(), fun);

				// Clear indices for list of article-stock pairs
				m_ArticleStockCollection.clear();
				m_ArticleCMIStockCollection.clear();
				m_GreenOrdersCount = 0;

				// The propTab contains all orders from the main screen but no positions
				for (AccessorPropertyTable_YIterator orderIt = getPropTab().begin(); !orderIt.isEnd(); ++orderIt)
				{
					const basar::Int32 orderNo = orderIt.getInt32(ORDERNO);
					basar::Int16 orderColor = NO_COLOR;

					basar::Int32 qtyMissingPosition = 0; //counter for positions with missing stock
					basar::Int32 qtyGreenPosition = 0; //counter for positions with enough stock (green positions)

					// iterate over all order positions which belong to same order
					for (AccessorPropertyTable_YIterator stockIt = m_StockCollection->findByOrderNo(orderNo); !stockIt.isEnd() && stockIt.getInt32(ORDERNO) == orderNo; ++stockIt)
					{
						basar::Int16     color = NO_COLOR;
						basar::VarString notice = orderIt.getString(properties::NOTICE);

						// only orders in status "ZU" and within a parameterized timespan have to be calculated here
						OrderStatus status(orderIt.getString(STATUS));
						if ((status.getStatus() == OrderStatus::ZU) && isWithinDateRange(orderIt.getDate(EDIDELIVERYDATE)))
						{
							if (isOMOrder(orderIt.getString(ORDERTYPE)))
							{
								color = calculateOMColor(stockIt);
							}
							else if (isCMOrder(orderIt.getString(ORDERTYPE)))
							{
								color = calculateCMColor(stockIt);

								basar::Int32 qty = stockIt.getInt32(properties::ORDEREDQTY);
								if (color == GREEN && 0 != qty)
								{
									++qtyGreenPosition;
								}
								else if (0 != qty)
								{
									++qtyMissingPosition;
								}
							}
							else // is UW order
							{
								basar::Int32 qty = stockIt.getInt32(properties::ORDEREDQTY);
								color = calculateUWColor(stockIt, isOrderView);
								if (color == GREEN && 0 != qty)
								{
									++qtyGreenPosition;
								}
								else if (0 != qty)
								{
									++qtyMissingPosition;
								}
							}
						}

						stockIt.setInt16(COLOR, color); // and set color for order position
						orderColor |= color;              // add this position to the color bitmask for the current order
					} // end iterating over positions

					std::stringstream ssTrafficLightInfo;

					if (0 < qtyMissingPosition && 0 < qtyGreenPosition && orderIt.getInt32(POSQTY) != qtyMissingPosition)
					{
						if (qtyMissingPosition > 9)
						{
							ssTrafficLightInfo << "Split:     ";
						}
						else
						{
							ssTrafficLightInfo << "Split:       ";
						}
					}

					ssTrafficLightInfo << qtyMissingPosition;
					orderIt.setString(TRAFFIC_LIGHT_INFO, ssTrafficLightInfo.str());

					orderIt.setInt32(SHORTFALLPOS, qtyMissingPosition); // for the value in the traffic light screen column 
					orderIt.setInt16(COLOR, orderColor);

					if (GREEN == orderColor)
					{
						++m_GreenOrdersCount;
					}
				}  // end iterating over orders
			}

			basar::Int32 TransferOrderHeadCollectionDM::getNumberOfGreenOrders() const
			{
				return m_GreenOrdersCount;
			}

			void TransferOrderHeadCollectionDM::getGreenOrders(YIterators& greenOrders)
			{
				greenOrders.reserve(m_GreenOrdersCount);
				for (AccessorPropertyTable_YIterator orderYit = getPropTab().begin(); !orderYit.isEnd(); ++orderYit)
				{
					if (orderYit.getInt16(properties::COLOR) == GREEN)
					{
						greenOrders.push_back(orderYit);
					}
				}
			}

			basar::Int32 TransferOrderHeadCollectionDM::getTotalNumberOfOrders()
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, getTotalNumberOfOrders)
					BLOG_TRACE_METHOD(getLogger(), fun);

				basar::Int32 total = 0;

				const std::pair<bool, basar::Decimal> result = getAccessor()->executeAggregate(SELECT_TRANSFER_ORDER_TOTAL_NUMBER, getSearchYit());
				if (result.first)
				{
					total = result.second.toInt32();
				}

				return total;
			}

			bool TransferOrderHeadCollectionDM::save(basar::VarString strDiscount)
			{
				METHODNAME_DEF(TransferOrderHeadCollectionDM, save)
					BLOG_TRACE_METHOD(getLogger(), fun);

				bool sthSaved = false;

				bool discountSet = !strDiscount.empty();

				for (AccessorPropertyTable_YIterator yit = get().begin(); !yit.isEnd(); ++yit)
				{
					libabbauw::domMod::transferOrder::ITransferOrderHeadPtr transferOrderHead = getHead(yit);

					//no transaction because each of TransferOrderHead is independent to another
					if (transferOrderHead->isChanged())
					{
						// save discounts to positions
						if (discountSet == true)
						{
							libabbauw::domMod::transferOrder::ITransferOrderPtr transferOrder = getOrderWithoutLock(yit);

							const basar::Int32 orderNo = transferOrder->getOrderNo();

							for (AccessorPropertyTable_YIterator yitPositions = transferOrder->getPosCollection()->get().begin(); !yitPositions.isEnd(); ++yitPositions)
							{
								// get the order number
								const basar::Int32 posOrderNo = yitPositions.getInt32(ORDERNO);
								if (posOrderNo != orderNo)
									continue;

								// set the discount percentage with a minus sign
								basar::Decimal discountDecimal("-" + strDiscount);

								yitPositions.setDecimal(properties::DISCOUNTPCT, (basar::Decimal)discountDecimal);
							}

							transferOrder->save();
						}

						transferOrderHead->save();	
						sthSaved = true;
					}
				}

				return sthSaved;
			}

			void TransferOrderHeadCollectionDM::setBackupAllHeadPosCollection()
			{
				m_BackUpAllHeadPosCollection = std::vector<AccessorPropertyTable_YIterator>();

				calculatePositionDiscount();

				for (AccessorPropertyTable_YIterator yit = get().begin(); !yit.isEnd(); ++yit)
				{
					libabbauw::domMod::transferOrder::ITransferOrderPtr transferOrder = getOrderWithoutLock(yit);
					
					for (AccessorPropertyTable_YIterator yitPositions = transferOrder->getPosCollection()->get().begin(); !yitPositions.isEnd(); ++yitPositions)
					{
						m_BackUpAllHeadPosCollection.push_back(yitPositions.cloneToUnboundedPropertyRow());
					}
				}
			}

			std::vector<AccessorPropertyTable_YIterator> TransferOrderHeadCollectionDM::getBackupAllHeadPosCollection()
			{
				return m_BackUpAllHeadPosCollection;
			}

			bool TransferOrderHeadCollectionDM::isChanged() const
			{
				for (AccessorPropertyTable_YIterator yit = get().begin(); !yit.isEnd(); ++yit)
				{
					libabbauw::domMod::transferOrder::ITransferOrderHeadPtr transferOrderHead = getHead(yit);
					if (transferOrderHead->isChanged())
					{
						return true;
					}
				}
				return false;
			}

			transferOrder::ITransferOrderHeadPtr TransferOrderHeadCollectionDM::getHead(const AccessorPropertyTable_YIterator yit) const
			{
				AccessorPropertyTable_YIterator yitTmp = yit;
				yitTmp.setInt16(properties::BRANCHNO, m_InitParams.m_BranchNo);

				return m_HeadFactory->create(yitTmp);
			}

			transferOrder::ITransferOrderPtr TransferOrderHeadCollectionDM::getOrder(const AccessorPropertyTable_YIterator yit)
			{
				return m_OrderFactory->create(yit);
			}

			transferOrder::ITransferOrderPtr TransferOrderHeadCollectionDM::getOrderWithoutLock(const AccessorPropertyTable_YIterator yit)
			{
				// used only for CPR0002688 to avoid double locking
				return m_OrderFactory->createWithoutLock(yit);
			}

			void TransferOrderHeadCollectionDM::setReserveQtyFactorType(ReserveQuantityFactorType resQtyFactorType)
			{
				m_ReserveQtyFactorGetter = m_ReserveQuantityFactorGetterFactory->getReserveQuantityFactorGetter(resQtyFactorType);
			}

			IReserveQuantityFactorGetterPtr TransferOrderHeadCollectionDM::getReserveQuantityFactorGetter() const
			{
				CHECK_INSTANCE_EXCEPTION(m_ReserveQtyFactorGetter.get());
				return m_ReserveQtyFactorGetter;
			}

			void TransferOrderHeadCollectionDM::addTransferOrderHead(AccessorPropertyTable_YIterator yit)
			{
				AccessorPropertyTable_YIterator newPosYit = get().insert(basar::FOR_CLEAN);
				util::YIteratorMatcher(yit).matchTo(newPosYit);
			}

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw
