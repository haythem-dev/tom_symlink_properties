#include "cmndm.h"

#include "transferorderhead.h"
#include "transferorderhead_definitions.h"

#include "transferorderheadacc.h"
#include "transferorderheadpropertystatechecker.h"

#include "dmbase.h"

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			const libutil::misc::ClassInfo& TransferOrderHead::getClassInfo()
			{
				static const libutil::misc::ClassInfo info("transferOrder::TransferOrderHead", libutil::misc::Version(1, 0, 0, 0));
				return info;
			}

			TransferOrderHead::TransferOrderHead(basar::db::aspect::ConnectionRef connection)
				: DMBase(connection, domMod::transferOrder::ACC_TRANSFER_ORDER_HEAD, basar::db::aspect::FULL_CACHING, LoggerPool::loggerTransferOrder)
			{
				METHODNAME_DEF(TransferOrderHead, TransferOrderHead)
					BLOG_TRACE_METHOD(getLogger(), fun);

				// avoid runtime error
				acc_transfer_order_head::UpdateTransferOrderHead dummy;
				(void)dummy;
			}

			TransferOrderHead::~TransferOrderHead()
			{
				METHODNAME_DEF(TransferOrderHead, ~TransferOrderHead)
					BLOG_TRACE_METHOD(getLogger(), fun);

				shutdown();
			}

			AccessorExecuteParameterization TransferOrderHead::getFindByPatternExecuteParameters() const
			{
				METHODNAME_DEF(TransferOrderHead, getFindByPatternExecuteParameters)
					BLOG_TRACE_METHOD(getLogger(), fun);

				return AccessorExecuteParameterization(SELECT_TRANSFER_ORDER_HEAD);
			}

			basar::VarString TransferOrderHead::getSearchYIteratorAttributeNames()  const
			{
				METHODNAME_DEF(TransferOrderHead, getSearchYIteratorAttributeNames)
					BLOG_TRACE_METHOD(getLogger(), fun);

				return properties::BRANCHNO.toDescriptionString() +
					properties::ORDERNO.toDescriptionString();
			}

			void TransferOrderHead::doInit(const TransferOrderHeadInitParams& p)
			{
				METHODNAME_DEF(TransferOrderHead, doInit)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_InitParams = p;
			}

			void TransferOrderHead::doShutdown()
			{
				METHODNAME_DEF(TransferOrderHead, doShutdown)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			void TransferOrderHead::resetSearchYit()
			{
				DMBase::resetSearchYit();
			}

			basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderHead::get()
			{
				const TransferOrderHead& me = *this; // force use of const method
				return me.get();
			}

			basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderHead::get() const
			{
				// get transfer order or create new one if not existing
				basar::db::aspect::AccessorPropertyTable_YIterator yit;
				if (!isEmpty())
				{
					yit = DMBase::getPropTab().begin();
				}
				else
				{
					yit = DMBase::getPropTab().insert(basar::FOR_INSERT);
					yit.setInt16(libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo);
				}

				return yit;
			}

			basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderHead::getSearchYit()
			{
				return DMBase::getSearchYit();
			}

			void TransferOrderHead::findById(basar::Int32 orderNo)
			{
				METHODNAME_DEF(TransferOrderHead, findByPattern)
					BLOG_TRACE_METHOD(getLogger(), fun);

				resetSearchYit();
				basar::db::aspect::AccessorPropertyTable_YIterator yit = getSearchYit();
				yit.setInt16(properties::BRANCHNO, m_InitParams.m_BranchNo);
				yit.setInt32(properties::ORDERNO, orderNo);

				DMBase::findByPattern(yit);
			}

			bool TransferOrderHead::isChanged() const
			{
				accessor::transferOrder::HeadPropertyStateChecker checker(get());
				return checker.isSavable();
			}

			bool TransferOrderHead::isEmpty() const
			{
				return DMBase::getPropTab().isNull() || DMBase::isEmpty();
			}

			void TransferOrderHead::markForDelete()
			{
				getPropTab().markForDelete(get());
			}

			bool TransferOrderHead::save()
			{
				METHODNAME_DEF(TransferOrderHead, save)
					BLOG_TRACE_METHOD(getLogger(), fun);

				if (!isChanged())
				{
					return false;
				}

				// make sure that only one last serial (orderno) exists which is needed to insert
				// new positions that use our head later
				// serial is automatically created by the DB when new head is inserted
				getAccessor()->clearLastInsertedSerials();

				try
				{
					basar::db::aspect::ExecuteResultInfo res = getAccessor()->execute(SAVE_TRANSFER_ORDER_HEAD, get());
					if (res.hasError())
					{
						THROW_ACCESSOR_EXECUTION_EXCEPTION(fun, SAVE_TRANSFER_ORDER_HEAD, res);
					}

					// a serial is only created when creating a new order in DB
					if (getAccessor()->hasLastInsertedSerial())
					{
						get().setInt32(properties::ORDERNO, static_cast<basar::Int32>(getAccessor()->getLastInsertedSerial()));
					}

				}
				catch (basar::Exception& e)
				{
					BLOG_ERROR(getLogger(), e.what());
					throw;
				}

				return true;
			}

			basar::Decimal TransferOrderHead::getDiscountPct() const
			{
				return get().getDecimal(properties::DISCOUNTPCT);
			}

			basar::VarString TransferOrderHead::getDiscountPctRange() const
			{
				return get().getString(properties::DISCOUNTPCTRANGE);
			}

			basar::Int32 TransferOrderHead::getOrderNo() const
			{
				return get().getInt32(properties::ORDERNO);
			}

			OrderStatus TransferOrderHead::getOrderStatus() const
			{
				return OrderStatus(get().getString(properties::STATUS));
			}

			void TransferOrderHead::clear()
			{
				get().clear();
			}
		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw
