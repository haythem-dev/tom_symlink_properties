//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "orderposcollectionprotocoldm.h"

#include "infrastructure/accessor/orderprotocol/orderposprotocolacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

#include "orderprotocolenums.h"

#include <libutil/exception.h>
#include <libutil/util.h>

#include <boost/make_shared.hpp>

//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
	namespace domMod
	{
		namespace orderProtocol
		{
			using basar::db::aspect::AccessorPropertyTableRef;
			using basar::db::aspect::AccessorPropertyTable_YIterator;

			//--------------------------------------------------------------------------------------------------//
			// object definition section
			//--------------------------------------------------------------------------------------------------//
			const libutil::misc::ClassInfo& OrderPosCollectionProtocolDM::getClassInfo()
			{
				static const libutil::misc::ClassInfo info("OrderPosCollectionProtocolDM", libutil::misc::Version(1, 0, 0, 0));
				return info;
			}

			OrderPosCollectionProtocolDM::OrderPosCollectionProtocolDM()
				: m_Logger(LoggerPool::loggerOrderProposal)
			{
				METHODNAME_DEF(OrderPosCollectionProtocolDM, OrderPosCollectionProtocolDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			OrderPosCollectionProtocolDM::~OrderPosCollectionProtocolDM()
			{
				METHODNAME_DEF(OrderPosCollectionProtocolDM, ~OrderPosCollectionProtocolDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			void OrderPosCollectionProtocolDM::injectAccessor(libutil::infrastructure::accessor::IAccessorPtr accessor)
			{
				m_Accessor = accessor;
			}

			const log4cplus::Logger& OrderPosCollectionProtocolDM::getLogger() const
			{
				return m_Logger;
			}

			AccessorPropertyTableRef OrderPosCollectionProtocolDM::get() const
			{
				if (isEmpty())
				{
					getAccessor()->getPropertyTable().insert(basar::FOR_INSERT);
				}

				return getAccessor()->getPropertyTable();
			}

			bool OrderPosCollectionProtocolDM::isEmpty() const
			{
				return getAccessor()->getPropertyTable().isEmpty();
			}

			AccessorPropertyTable_YIterator OrderPosCollectionProtocolDM::getSearchYit()
			{
				return getSearchYIterator()->get();
			}

			void OrderPosCollectionProtocolDM::resetSearchYit()
			{
				getSearchYIterator()->reset();
			}

			void OrderPosCollectionProtocolDM::findByKey(const AccessorPropertyTable_YIterator yitSearch)
			{
				METHODNAME_DEF(OrderPosCollectionProtocolDM, findByKey)
					BLOG_TRACE_METHOD(getLogger(), fun);

				using libabbauw::infrastructure::accessor::orderprotocol::lit::SELECT_ORDER_POS_PROTOCOL_BY_KEY;

				basar::db::aspect::ExecuteResultInfo execResultInfo;

				try
				{
					execResultInfo = m_Accessor->execute(SELECT_ORDER_POS_PROTOCOL_BY_KEY, yitSearch);
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processAccessorException(e, getLogger(), fun, SELECT_ORDER_POS_PROTOCOL_BY_KEY, __FILE__, __LINE__);
					throw;
				}

				if (execResultInfo.hasError())
				{
					THROW_ACCESSOR_EXECUTION_EXCEPTION(fun, SELECT_ORDER_POS_PROTOCOL_BY_KEY, execResultInfo)
				}
			}

			void OrderPosCollectionProtocolDM::save()
			{
				METHODNAME_DEF(OrderPosCollectionProtocolDM, save)
					BLOG_TRACE_METHOD(getLogger(), fun);

				if (isEmpty())
				{
					return;
				}

				using libabbauw::infrastructure::accessor::orderprotocol::lit::SAVE_ORDER_POS_PROTOCOL;

				basar::db::aspect::ExecuteResultInfo execResultInfo;

				try
				{
					AccessorPropertyTable_YIterator yitPos = get().begin(basar::SS_INSERT | basar::SS_UPDATE);
					if (yitPos.isContainedAndSet(properties::DISCOUNTCALCFROM) == false)
					{
						yitPos.setInt16(properties::DISCOUNTCALCFROM, 0);
					}

					if (yitPos.isContainedAndSet(properties::DISCOUNTAPPLYTO) == false)
					{
						yitPos.setInt16(properties::DISCOUNTAPPLYTO, 1);
					}

					execResultInfo = m_Accessor->execute(SAVE_ORDER_POS_PROTOCOL, yitPos);

					if (execResultInfo.hasError() || 1 != execResultInfo.getAffectedRows())
					{
						THROW_ACCESSOR_EXECUTION_EXCEPTION(fun, SAVE_ORDER_POS_PROTOCOL, execResultInfo)
					}
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processAccessorException(e, getLogger(), fun, SAVE_ORDER_POS_PROTOCOL, __FILE__, __LINE__);
					throw;
				}
			}

			void OrderPosCollectionProtocolDM::protocolCreateOrder(AccessorPropertyTableRef posCollection)
			{
				AccessorPropertyTable_YIterator yitPos = posCollection.begin();
				AccessorPropertyTable_YIterator yitPosProtocol = get().begin();

				setProtocolValues(yitPosProtocol, CREATE, NO_EVENT_TYPE, "");

				int orderedQuantity = 0;

				for (; !yitPos.isEnd(); ++yitPos)
				{
					yitPosProtocol.match(yitPos);

					//Needed because orderedqty = AMGE + NR
					orderedQuantity = yitPos.getInt32(properties::ORDEREDQTY) + yitPos.getInt32(properties::NONCHARGEDQTY);
					yitPosProtocol.setInt32(properties::ORDEREDQTY, orderedQuantity);

					save();
				}
			}

			void OrderPosCollectionProtocolDM::protocolChangeOrderPositionDiscount(const basar::Decimal newValue, const AccessorPropertyTable_YIterator yitPos)
			{
				if (yitPos.getDecimal(properties::DISCOUNTPCT) != newValue)
				{
					AccessorPropertyTable_YIterator yitPosProtocol = get().begin();

					yitPosProtocol.match(yitPos);

					setProtocolValues(yitPosProtocol, UPDATE, NO_EVENT_TYPE, "");

					save();
				}					
			}

			void OrderPosCollectionProtocolDM::protocolDeletePositon()
			{
				if (m_BackUpPos.isEnd() || m_BackUpPos.isNull())
				{
					throw;
				}

				AccessorPropertyTable_YIterator yitPosProtocol = get().begin();

				setProtocolValues(yitPosProtocol, DELET, NO_EVENT_TYPE, "");

				yitPosProtocol.match(m_BackUpPos);

				int orderedQuantity = m_BackUpPos.getInt32(properties::ORDEREDQTY) + m_BackUpPos.getInt32(properties::NONCHARGEDQTY);
				yitPosProtocol.setInt32(properties::ORDEREDQTY, orderedQuantity);

				save();
			}

			void OrderPosCollectionProtocolDM::protocolOrderRequest(AccessorPropertyTable_YIterator yitPos, bool isOrderViewActive)
			{
				AccessorPropertyTable_YIterator yitPosProtocol = get().begin();

				if (isOrderViewActive)
				{
					setProtocolValues(yitPosProtocol, UPDATE, ORDER_REQUEST_CREATED, ORDERVIEW, yitPos.getInt32(properties::ORDERPROPOSALQTY));
				}
				else
				{
					setProtocolValues(yitPosProtocol, UPDATE, ORDER_REQUEST_CREATED, RELEASEVIEW, yitPos.getInt32(properties::ORDERPROPOSALQTY));
				}

				yitPosProtocol.match(yitPos);

				int orderedQuantity = 0;
				if (yitPos.isContainedAndSet(properties::NONCHARGEDQTY))
				{
					orderedQuantity = yitPos.getInt32(properties::ORDEREDQTY) + yitPos.getInt32(properties::NONCHARGEDQTY);
				}
				else
				{
					int propsalQuantity = yitPos.getInt32(properties::ORDERPROPOSALQTY);
					int nonchargedQuantity = propsalQuantity - yitPos.getInt32(properties::ORDEREDQTY);
					orderedQuantity = propsalQuantity;

					yitPosProtocol.setInt32(properties::NONCHARGEDQTY, nonchargedQuantity);
					yitPosProtocol.setDecimal(properties::DISCOUNTPCT, basar::Decimal(0.0));
				}


				yitPosProtocol.setInt32(properties::ORDEREDQTY, orderedQuantity);

				save();
			}

			void OrderPosCollectionProtocolDM::protocolCancelOrderRequest(bool isOrderViewActive)
			{
				AccessorPropertyTable_YIterator yitPosProtocol = get().begin();

				if (isOrderViewActive)
				{
					setProtocolValues(yitPosProtocol, UPDATE, ORDER_REQUEST_CANCELED, ORDERVIEW, m_BackUpPos.getInt32(properties::ORDERPROPOSALQTY));
				}
				else
				{
					setProtocolValues(yitPosProtocol, UPDATE, ORDER_REQUEST_CANCELED, RELEASEVIEW, m_BackUpPos.getInt32(properties::ORDERPROPOSALQTY));
				}

				yitPosProtocol.setInt32(properties::ORDEREDQTY, 0);
				yitPosProtocol.setInt32(properties::NONCHARGEDQTY, 0);
				yitPosProtocol.setDecimal(properties::DISCOUNTPCT, basar::Decimal(0.0));
				yitPosProtocol.match(m_BackUpPos);

				save();
			}

			void OrderPosCollectionProtocolDM::protocolSplitOrder(AccessorPropertyTableRef posCollection, const basar::VarString activityType, const EventTypeEnum eventType, bool isOrderViewActive)
			{
				AccessorPropertyTable_YIterator yitPosProtocol = get().begin();

				if (isOrderViewActive)
				{
					setProtocolValues(yitPosProtocol, activityType, eventType, ORDERVIEW);
				}
				else
				{
					setProtocolValues(yitPosProtocol, activityType, eventType, RELEASEVIEW);
				}

				AccessorPropertyTable_YIterator yitPos = posCollection.begin();
				for (; !yitPos.isEnd(); ++yitPos)
				{
					//Article which were not splitted, don't need to be written in den protocol table!
					int orderedQuantity = yitPos.getInt32(properties::ORDEREDQTY) + yitPos.getInt32(properties::NONCHARGEDQTY);

					if (UPDATE == activityType && 0 < orderedQuantity)
					{
						continue;
					}

					yitPosProtocol.match(yitPos);

					yitPosProtocol.setInt32(properties::ORDEREDQTY, orderedQuantity);

					save();
				}
			}

			void OrderPosCollectionProtocolDM::protocolCreateTender(const basar::Int32 tenderNo)
			{
				std::stringstream ss;
				ss << "Tender: " << tenderNo;
				AccessorPropertyTable_YIterator yitPosProtocol = get().begin();
				yitPosProtocol.match(m_BackUpPos);
				yitPosProtocol.setString(properties::ACTIVITY_TYPE, UPDATE);
				yitPosProtocol.setInt16(properties::EVENT_TYPE, static_cast<basar::Int16>(CREATE_TENDER));
				yitPosProtocol.setInt16(properties::PROCESSED_BY, ABBA_UW);
				yitPosProtocol.setString(properties::VIEW, "");
				yitPosProtocol.setString(properties::ITEMTEXT, "");
				yitPosProtocol.setInt32(properties::ORDERREQUESTQTY, 0);
				yitPosProtocol.setString(properties::EXTRATEXT, ss.str().c_str());
				yitPosProtocol.setInt32(properties::ORDEREDQTY, 0);
				yitPosProtocol.setInt32(properties::NONCHARGEDQTY, 0);
				yitPosProtocol.setDecimal(properties::DISCOUNTPCT, basar::Decimal(0.0));
				save();
			}

			void OrderPosCollectionProtocolDM::setBackupAllHeadPosCollection(std::vector<AccessorPropertyTable_YIterator> posCollection)
			{
				METHODNAME_DEF(OrderPosCollectionProtocolDM, setBackupPosCollection)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_BackUpAllHeadPosCollection = posCollection;
			}

			std::vector<AccessorPropertyTable_YIterator> OrderPosCollectionProtocolDM::getBackupAllHeadPosCollection()
			{
				return m_BackUpAllHeadPosCollection;
			}

			void OrderPosCollectionProtocolDM::setBackupPos(AccessorPropertyTable_YIterator yitPos)
			{
				METHODNAME_DEF(OrderPosCollectionProtocolDM, setBackupPos)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_BackUpPos = yitPos.cloneToUnboundedPropertyRow();
			}

			void OrderPosCollectionProtocolDM::setBackupPos(AccessorPropertyTable_YIterator yitPos, const basar::Int16 branchNo)
			{
				METHODNAME_DEF(OrderPosCollectionProtocolDM, setBackupPos)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_BackUpPos = yitPos.cloneToUnboundedPropertyRow();
				m_BackUpPos.setInt16(properties::BRANCHNO, branchNo);
			}


			libutil::infrastructure::accessor::IAccessorPtr OrderPosCollectionProtocolDM::getAccessor() const
			{
				return m_Accessor;
			}

			libutil::domMod::SearchYIteratorPtr OrderPosCollectionProtocolDM::getSearchYIterator()
			{
				METHODNAME_DEF(OrderPosCollectionProtocolDM, getSearchYIterator)
					BLOG_TRACE_METHOD(getLogger(), fun);

				//TODO properties anpassen
				if (NULL == m_SearchYIterator.get())
				{
					m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(new libutil::domMod::SearchYIterator(
						properties::BRANCHNO.toDescriptionString() +
						properties::ORDERNO.toDescriptionString() +
						properties::POSNO.toDescriptionString())
					);
				}

				return m_SearchYIterator;
			}

			void OrderPosCollectionProtocolDM::setProtocolValues(AccessorPropertyTable_YIterator& yitPosProtocol,
				basar::I18nString activityType, libabbauw::domMod::orderProtocol::EventTypeEnum eventType,
				basar::I18nString view, int orderRequestQuantity)
			{
				METHODNAME_DEF(OrderPosCollectionProtocolDM, setProtocolValues)
					BLOG_TRACE_METHOD(getLogger(), fun);

				yitPosProtocol.setString(properties::ACTIVITY_TYPE, activityType);
				yitPosProtocol.setInt16(properties::EVENT_TYPE, static_cast<basar::Int16>(eventType));
				yitPosProtocol.setInt16(properties::PROCESSED_BY, ABBA_UW);
				yitPosProtocol.setString(properties::VIEW, view);
				yitPosProtocol.setString(properties::ITEMTEXT, "");
				yitPosProtocol.setString(properties::EXTRATEXT, "");
				yitPosProtocol.setInt32(properties::ORDERREQUESTQTY, orderRequestQuantity);
			}

		} // end namespace orderProposal
	} // end namespace domMod
} // end namespace libabbauw
