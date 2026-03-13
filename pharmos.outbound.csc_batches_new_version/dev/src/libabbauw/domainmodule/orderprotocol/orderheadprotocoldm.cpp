//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "orderheadprotocoldm.h"

#include "infrastructure/accessor/orderprotocol/orderheadprotocolacc_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include "libabbauw_properties_definitions.h"

#include "orderprotocolenums.h"
#include "orderstatus.h"

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
			using basar::db::aspect::AccessorPropertyTable_YIterator;

			//--------------------------------------------------------------------------------------------------//
			// object definition section
			//--------------------------------------------------------------------------------------------------//
			const libutil::misc::ClassInfo& OrderHeadProtocolDM::getClassInfo()
			{
				static const libutil::misc::ClassInfo info("OrderHeadProtocolDM", libutil::misc::Version(1, 0, 0, 0));
				return info;
			}

			OrderHeadProtocolDM::OrderHeadProtocolDM()
				: m_Logger(LoggerPool::loggerOrderProposal)
			{
				METHODNAME_DEF(OrderHeadProtocolDM, OrderHeadProtocolDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			OrderHeadProtocolDM::~OrderHeadProtocolDM()
			{
				METHODNAME_DEF(OrderHeadProtocolDM, ~OrderHeadProtocolDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			void OrderHeadProtocolDM::injectAccessor(libutil::infrastructure::accessor::IAccessorPtr accessor)
			{
				METHODNAME_DEF(OrderHeadProtocolDM, injectAccessor)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_Accessor = accessor;
			}

			const log4cplus::Logger& OrderHeadProtocolDM::getLogger() const
			{
				return m_Logger;
			}

			AccessorPropertyTable_YIterator OrderHeadProtocolDM::get() const
			{
				// get order proposal or create new one if not existing
				AccessorPropertyTable_YIterator yit;

				if (!isEmpty())
				{
					yit = getAccessor()->getPropertyTable().begin();
				}
				else
				{
					yit = getAccessor()->getPropertyTable().insert(basar::FOR_INSERT);
				}

				return yit;
			}

			bool OrderHeadProtocolDM::isEmpty() const
			{
				return getAccessor()->getPropertyTable().isEmpty();
			}

			AccessorPropertyTable_YIterator OrderHeadProtocolDM::getSearchYit()
			{
				return getSearchYIterator()->get();
			}

			void OrderHeadProtocolDM::resetSearchYit()
			{
				getSearchYIterator()->reset();
			}

			void OrderHeadProtocolDM::findByKey(const AccessorPropertyTable_YIterator yitSearch)
			{
				METHODNAME_DEF(OrderHeadProtocolDM, findByKey)
					BLOG_TRACE_METHOD(getLogger(), fun);

				using libabbauw::infrastructure::accessor::orderprotocol::lit::SELECT_ORDER_HEAD_PROTOCOL_BY_KEY;

				basar::db::aspect::ExecuteResultInfo execResultInfo;

				try
				{
					execResultInfo = m_Accessor->execute(SELECT_ORDER_HEAD_PROTOCOL_BY_KEY, yitSearch);
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processAccessorException(e, getLogger(), fun, SELECT_ORDER_HEAD_PROTOCOL_BY_KEY, __FILE__, __LINE__);
					throw;
				}

				if (execResultInfo.hasError())
				{
					THROW_ACCESSOR_EXECUTION_EXCEPTION(fun, SELECT_ORDER_HEAD_PROTOCOL_BY_KEY, execResultInfo)
				}
			}

			void OrderHeadProtocolDM::save()
			{
				METHODNAME_DEF(OrderHeadProtocolDM, save)
					BLOG_TRACE_METHOD(getLogger(), fun);

				if (isEmpty())
				{
					return;
				}

				using libabbauw::infrastructure::accessor::orderprotocol::lit::SAVE_ORDER_HEAD_PROTOCOL;

				basar::db::aspect::ExecuteResultInfo execResultInfo;

				try
				{
					execResultInfo = m_Accessor->execute(SAVE_ORDER_HEAD_PROTOCOL, get());
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processAccessorException(e, getLogger(), fun, SAVE_ORDER_HEAD_PROTOCOL, __FILE__, __LINE__);
					throw;
				}

				if (execResultInfo.hasError())
				{
					THROW_ACCESSOR_EXECUTION_EXCEPTION(fun, SAVE_ORDER_HEAD_PROTOCOL, execResultInfo)
				}
			}

			void OrderHeadProtocolDM::protocolCreateOrder(const AccessorPropertyTable_YIterator yitHead)
			{
				AccessorPropertyTable_YIterator yitHeadProtocol = get();

				yitHeadProtocol.match(yitHead);

				yitHeadProtocol.setString(properties::ACTIVITY_TYPE, CREATE);
				yitHeadProtocol.setInt16(properties::EVENT_TYPE, NO_EVENT_TYPE);
				yitHeadProtocol.setInt16(properties::PROCESSED_BY, ABBA_UW);
				yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, "");
				yitHeadProtocol.setString(properties::OLD_VALUE, "");
				yitHeadProtocol.setString(properties::NEW_VALUE, "");
				yitHeadProtocol.setInt32(properties::SPLIT_FROM, 0);
				yitHeadProtocol.setInt32(properties::SPLIT_TO, 0);
				yitHeadProtocol.setInt32(properties::ORDERNOCSC, 0);
				yitHeadProtocol.setString(properties::EXTRATEXT, "");

				//For Germany... 
				if (!yitHeadProtocol.isContainedAndSet(properties::HEADERTEXT))
				{
					yitHeadProtocol.setString(properties::HEADERTEXT, "");
				}

				save();
			}

			void OrderHeadProtocolDM::protocolDeleteOrder()
			{
				if (m_BackUpYit.isEnd() || m_BackUpYit.isNull())
				{
					throw;
				}

				AccessorPropertyTable_YIterator yitHeadProtocol = get();

				yitHeadProtocol.match(m_BackUpYit);

				yitHeadProtocol.setString(properties::ACTIVITY_TYPE, DELET);
				yitHeadProtocol.setInt16(properties::EVENT_TYPE, NO_EVENT_TYPE);
				yitHeadProtocol.setInt16(properties::PROCESSED_BY, ABBA_UW);
				yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, "");
				yitHeadProtocol.setString(properties::OLD_VALUE, "");
				yitHeadProtocol.setString(properties::NEW_VALUE, "");
				yitHeadProtocol.setInt32(properties::SPLIT_FROM, 0);
				yitHeadProtocol.setInt32(properties::SPLIT_TO, 0);
				yitHeadProtocol.setInt32(properties::ORDERNOCSC, 0);
				yitHeadProtocol.setString(properties::EXTRATEXT, "");

				save();
			}

			void OrderHeadProtocolDM::protocolChangeOrder(const AccessorPropertyTable_YIterator yitHead, const AccessorPropertyTable_YIterator yitBackup, EventTypeEnum eventType)
			{
				AccessorPropertyTable_YIterator yitHeadProtocol = get();

				yitHeadProtocol.match(yitHead);

				yitHeadProtocol.setString(properties::ACTIVITY_TYPE, UPDATE);
				yitHeadProtocol.setInt16(properties::EVENT_TYPE, static_cast<basar::Int16>(eventType)); //because of the warning...
				yitHeadProtocol.setInt16(properties::PROCESSED_BY, ABBA_UW);
				yitHeadProtocol.setInt32(properties::SPLIT_FROM, 0);
				yitHeadProtocol.setInt32(properties::SPLIT_TO, 0);
				yitHeadProtocol.setInt32(properties::ORDERNOCSC, 0);
				yitHeadProtocol.setString(properties::EXTRATEXT, "");

				if (yitHead.getInt32(properties::MANUFACTURERNO) != yitBackup.getInt32(properties::MANUFACTURERNO))
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::MANUFACTURERNO.getName());

					basar::VarString temp = "";
					temp.itos(yitBackup.getInt32(properties::MANUFACTURERNO));
					yitHeadProtocol.setString(properties::OLD_VALUE, temp);

					temp = "";
					temp.itos(yitHead.getInt32(properties::MANUFACTURERNO));
					yitHeadProtocol.setString(properties::NEW_VALUE, temp);
					save();
				}

				if (yitHead.getInt32(properties::PHARMACYNO) != yitBackup.getInt32(properties::PHARMACYNO))
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::PHARMACYNO.getName());

					basar::VarString temp = "";
					temp.itos(yitBackup.getInt32(properties::PHARMACYNO));
					yitHeadProtocol.setString(properties::OLD_VALUE, temp);

					temp = "";
					temp.itos(yitHead.getInt32(properties::PHARMACYNO));
					yitHeadProtocol.setString(properties::NEW_VALUE, temp);
					save();
				}

				if (yitHead.getString(properties::ORDERTYPE) != yitBackup.getString(properties::ORDERTYPE))
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::ORDERTYPE.getName());

					yitHeadProtocol.setString(properties::OLD_VALUE, yitBackup.getString(properties::ORDERTYPE));
					yitHeadProtocol.setString(properties::NEW_VALUE, yitHead.getString(properties::ORDERTYPE));
					save();
				}

				if (yitHead.getString(properties::STATUS) != yitBackup.getString(properties::STATUS))
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::STATUS.getName());

					yitHeadProtocol.setString(properties::OLD_VALUE, yitBackup.getString(properties::STATUS));
					yitHeadProtocol.setString(properties::NEW_VALUE, yitHead.getString(properties::STATUS));
					save();
				}

				//Delivery date added!
				if (yitBackup.getDate(properties::EDIDELIVERYDATE) == basar::Date(99991231) && yitHead.getDate(properties::EDIDELIVERYDATE).isValid())
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::EDIDELIVERYDATE.getName());

					yitHeadProtocol.setString(properties::OLD_VALUE, "");
					yitHeadProtocol.setString(properties::NEW_VALUE, yitHead.getDate(properties::EDIDELIVERYDATE).toStrDate());
					save();
				}
				//Delivery date deleted! 
				else if (yitBackup.getDate(properties::EDIDELIVERYDATE).isValid() && yitHead.getDate(properties::EDIDELIVERYDATE) == basar::Date(99991231))
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::EDIDELIVERYDATE.getName());

					yitHeadProtocol.setString(properties::OLD_VALUE, yitBackup.getDate(properties::EDIDELIVERYDATE).toStrDate());
					yitHeadProtocol.setString(properties::NEW_VALUE, "");
					save();
				}
				//Delivery date updated!
				else if (yitHead.getDate(properties::EDIDELIVERYDATE).getDate() != yitBackup.getDate(properties::EDIDELIVERYDATE).getDate())
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::EDIDELIVERYDATE.getName());

					yitHeadProtocol.setString(properties::OLD_VALUE, yitBackup.getDate(properties::EDIDELIVERYDATE).toStrDate());
					yitHeadProtocol.setString(properties::NEW_VALUE, yitHead.getDate(properties::EDIDELIVERYDATE).toStrDate());
					save();
				}

				//Deferred payment date added! 
				if (yitBackup.getDate(properties::DEFERREDPAYMENTDATE) == basar::Date(99991231) && yitHead.getDate(properties::DEFERREDPAYMENTDATE).isValid())
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::DEFERREDPAYMENTDATE.getName());

					yitHeadProtocol.setString(properties::OLD_VALUE, "");
					yitHeadProtocol.setString(properties::NEW_VALUE, yitHead.getDate(properties::DEFERREDPAYMENTDATE).toStrDate());
					save();
				}

				//Deferred payment date deleted! 
				else if (yitBackup.getDate(properties::DEFERREDPAYMENTDATE).isValid() && yitHead.getDate(properties::DEFERREDPAYMENTDATE) == basar::Date(99991231))
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::DEFERREDPAYMENTDATE.getName());

					yitHeadProtocol.setString(properties::OLD_VALUE, yitBackup.getDate(properties::DEFERREDPAYMENTDATE).toStrDate());
					yitHeadProtocol.setString(properties::NEW_VALUE, "");
					save();
				}

				//Deferred payment date updated! 
				else if (yitHead.getDate(properties::DEFERREDPAYMENTDATE).getDate() != yitBackup.getDate(properties::DEFERREDPAYMENTDATE).getDate())
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::DEFERREDPAYMENTDATE.getName());

					yitHeadProtocol.setString(properties::OLD_VALUE, yitBackup.getDate(properties::DEFERREDPAYMENTDATE).toStrDate());
					yitHeadProtocol.setString(properties::NEW_VALUE, yitHead.getDate(properties::DEFERREDPAYMENTDATE).toStrDate());
					save();
				}
			}

			void OrderHeadProtocolDM::protocolAddDefferredPaymentDate(const AccessorPropertyTable_YIterator yitHead)
			{
				AccessorPropertyTable_YIterator yitHeadProtocol = get();

				yitHeadProtocol.match(yitHead);

				yitHeadProtocol.setString(properties::ACTIVITY_TYPE, UPDATE);
				yitHeadProtocol.setInt16(properties::EVENT_TYPE, NO_EVENT_TYPE);
				yitHeadProtocol.setInt16(properties::PROCESSED_BY, ABBA_UW);
				yitHeadProtocol.setInt32(properties::SPLIT_FROM, 0);
				yitHeadProtocol.setInt32(properties::SPLIT_TO, 0);
				yitHeadProtocol.setInt32(properties::ORDERNOCSC, 0);
				yitHeadProtocol.setString(properties::EXTRATEXT, "");

				if (yitHead.getInt32(properties::MANUFACTURERNO) != getBackupYit().getInt32(properties::MANUFACTURERNO))
				{
					yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::MANUFACTURERNO.getName());

					basar::VarString temp = "";
					temp.itos(getBackupYit().getInt32(properties::MANUFACTURERNO));
					yitHeadProtocol.setString(properties::OLD_VALUE, temp);

					temp = "";
					temp.itos(yitHead.getInt32(properties::MANUFACTURERNO));
					yitHeadProtocol.setString(properties::NEW_VALUE, temp);

					save();
				}
			}

			void OrderHeadProtocolDM::protocolReleaseOrder(EventTypeEnum eventType, const basar::Int32 orderNoCsc)
			{
				if (m_BackUpYit.isEnd() || m_BackUpYit.isNull())
				{
					throw;
				}

				AccessorPropertyTable_YIterator yitHeadProtocol = get();

				domMod::OrderStatus oldStatus(OrderStatus::ZU);
				domMod::OrderStatus newStatus(OrderStatus::ER);

				yitHeadProtocol.match(m_BackUpYit);

				yitHeadProtocol.setString(properties::ACTIVITY_TYPE, UPDATE);
				yitHeadProtocol.setInt16(properties::EVENT_TYPE, static_cast<basar::Int16>(eventType));
				yitHeadProtocol.setInt16(properties::PROCESSED_BY, ABBA_UW);
				yitHeadProtocol.setInt32(properties::SPLIT_FROM, 0);
				yitHeadProtocol.setInt32(properties::SPLIT_TO, 0);
				yitHeadProtocol.setInt32(properties::ORDERNOCSC, orderNoCsc);
				yitHeadProtocol.setString(properties::EXTRATEXT, "");

				yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::STATUS.getName());
				yitHeadProtocol.setString(properties::OLD_VALUE, oldStatus.toString());
				yitHeadProtocol.setString(properties::NEW_VALUE, newStatus.toString());

				save();
			}

			void OrderHeadProtocolDM::protocolCreateTender()
			{
				if (m_BackUpYit.isEnd() || m_BackUpYit.isNull())
				{
					throw;
				}

				domMod::OrderStatus oldStatus(OrderStatus::ZU);
				domMod::OrderStatus newStatus(OrderStatus::ER);

				AccessorPropertyTable_YIterator yitHeadProtocol = get();
				yitHeadProtocol.match(m_BackUpYit);

				yitHeadProtocol.setString(properties::ACTIVITY_TYPE, UPDATE);
				yitHeadProtocol.setInt16(properties::EVENT_TYPE, static_cast<basar::Int16>(CREATE_TENDER));
				yitHeadProtocol.setInt16(properties::PROCESSED_BY, ABBA_UW);
				yitHeadProtocol.setInt32(properties::SPLIT_FROM, 0);
				yitHeadProtocol.setInt32(properties::SPLIT_TO, 0);
				yitHeadProtocol.setInt32(properties::ORDERNOCSC, 0);
				yitHeadProtocol.setString(properties::EXTRATEXT, "");
				yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, properties::STATUS.getName());
				yitHeadProtocol.setString(properties::OLD_VALUE, oldStatus.toString());
				yitHeadProtocol.setString(properties::NEW_VALUE, newStatus.toString());

				save();
			}

			void OrderHeadProtocolDM::protocolSplitOrder(
				const AccessorPropertyTable_YIterator head,
				const basar::VarString activityType,
				const EventTypeEnum    eventType,
				const basar::Int32     splitFromOrderNo,
				const basar::Int32     splitToOrderNo)
			{
				AccessorPropertyTable_YIterator yitHeadProtocol = get();

				yitHeadProtocol.match(head);

				yitHeadProtocol.setString(properties::ACTIVITY_TYPE, activityType);
				yitHeadProtocol.setInt16(properties::EVENT_TYPE, static_cast<basar::Int16>(eventType));
				yitHeadProtocol.setInt16(properties::PROCESSED_BY, ABBA_UW);
				yitHeadProtocol.setString(properties::CHANGED_ATTRIBUTE, "");
				yitHeadProtocol.setString(properties::OLD_VALUE, "");
				yitHeadProtocol.setString(properties::NEW_VALUE, "");
				yitHeadProtocol.setInt32(properties::SPLIT_FROM, splitFromOrderNo);
				yitHeadProtocol.setInt32(properties::SPLIT_TO, splitToOrderNo);
				yitHeadProtocol.setInt32(properties::ORDERNOCSC, 0);
				yitHeadProtocol.setString(properties::EXTRATEXT, "");

				save();
			}

			void OrderHeadProtocolDM::setBackupYit(AccessorPropertyTable_YIterator backUp)
			{
				METHODNAME_DEF(OrderHeadProtocolDM, setBackupYit)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_BackUpYit = backUp.cloneToUnboundedPropertyRow();
			}

			void OrderHeadProtocolDM::setBackupHeadCollection(AccessorPropertyTable_YIterator backUp)
			{
				METHODNAME_DEF(OrderHeadProtocolDM, setBackupYit)
					BLOG_TRACE_METHOD(getLogger(), fun);

				AccessorPropertyTable_YIterator it = backUp;
				while (!it.isEnd())
				{
					m_BackUpHeadCollection.push_back(it.cloneToUnboundedPropertyRow()); 

					++it;
				}
			}

			AccessorPropertyTable_YIterator	OrderHeadProtocolDM::getBackupYit()
			{
				return m_BackUpYit;
			}

			std::vector<AccessorPropertyTable_YIterator> OrderHeadProtocolDM::getBackupHeadCollection()
			{
				return m_BackUpHeadCollection;
			}

			libutil::infrastructure::accessor::IAccessorPtr OrderHeadProtocolDM::getAccessor() const
			{
				return m_Accessor;
			}

			libutil::domMod::SearchYIteratorPtr OrderHeadProtocolDM::getSearchYIterator()
			{
				METHODNAME_DEF(OrderHeadProtocolDM, getSearchYIterator)
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

		} // end namespace orderProposal
	} // end namespace domMod
} // end namespace libabbauw
