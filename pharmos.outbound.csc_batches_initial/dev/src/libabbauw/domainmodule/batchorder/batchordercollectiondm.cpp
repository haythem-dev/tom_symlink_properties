#include "batchordercollectiondm.h"

#include "componentmanager/ibatchorderconnectedfactory.h"

#include "infrastructure/accessor/batchorder/batchordercollectionacc_definitions.h"
#include "libabbauw_properties_definitions.h"
#include "loggerpool/libabbauw_loggerpool.h"

#include <libutil/domainmodule.h>
#include <libutil/version.h>
#include <libutil/exception.h>

#include <boost/make_shared.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace batchOrder
		{
			const libutil::misc::ClassInfo& BatchOrderCollectionDM::getClassInfo()
			{
				static const libutil::misc::ClassInfo info("BatchOrderCollectionDM", libutil::misc::Version(1, 0, 0, 0));
				return info;
			}

			BatchOrderCollectionDM::BatchOrderCollectionDM()
				: m_Logger(LoggerPool::loggerLibAbbaUW)
			{
				METHODNAME_DEF(BatchOrderCollectionDM, BatchOrderCollectionDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			BatchOrderCollectionDM::~BatchOrderCollectionDM()
			{
				METHODNAME_DEF(BatchOrderCollectionDM, ~BatchOrderCollectionDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			void BatchOrderCollectionDM::injectBatchOrderCollectionAccessor(libutil::infrastructure::accessor::IAccessorPtr accessor)
			{
				m_Accessor = accessor;
			}

			libutil::infrastructure::accessor::IAccessorPtr BatchOrderCollectionDM::getAccessor() const
			{
				CHECK_INSTANCE_EXCEPTION(m_Accessor.get());
				return m_Accessor;
			}

			void BatchOrderCollectionDM::injectBatchOrderConnectedFactory(componentManager::IBatchOrderConnectedFactoryPtr factory)
			{
				m_BatchOrderConnectedFactory = factory;
			}

			componentManager::IBatchOrderConnectedFactoryPtr BatchOrderCollectionDM::getBatchOrderConnectedFactory()
			{
				CHECK_INSTANCE_EXCEPTION(m_BatchOrderConnectedFactory.get());
				return m_BatchOrderConnectedFactory;
			}

			basar::db::aspect::AccessorPropertyTableRef BatchOrderCollectionDM::get() const
			{
				return getAccessor()->getPropertyTable();
			}

			bool BatchOrderCollectionDM::isEmpty() const
			{
				return 0 == getAccessor().get() || m_Accessor->getPropertyTable().isNull() || m_Accessor->getPropertyTable().isEmpty();
			}

			void BatchOrderCollectionDM::resetSearchYit()
			{
				getSearchYIterator()->reset();
			}

			basar::db::aspect::AccessorPropertyTable_YIterator BatchOrderCollectionDM::getSearchYit()
			{
				return getSearchYIterator()->get();
			}

			void BatchOrderCollectionDM::findByPattern(basar::db::aspect::AccessorPropertyTable_YIterator searchYit)
			{
				METHODNAME_DEF(BatchOrderCollectionDM, findByPattern)
					BLOG_TRACE_METHOD(getLogger(), fun);

				using infrastructure::accessor::batchOrder::lit::SELECT_BATCH_ORDER_COLLECTION_BY_PATTERN;

				basar::db::aspect::ExecuteResultInfo executeResultInfo;
				try
				{
					executeResultInfo = getAccessor()->execute(SELECT_BATCH_ORDER_COLLECTION_BY_PATTERN, searchYit);
				}
				catch (basar::Exception& e)
				{
					libutil::exceptions::ExceptionHandler::processAccessorException(e, getLogger(), fun, SELECT_BATCH_ORDER_COLLECTION_BY_PATTERN,
						__FILE__, __LINE__);
					throw;
				}

				if (executeResultInfo.hasError())
				{
					THROW_ACCESSOR_EXECUTION_EXCEPTION(fun, SELECT_BATCH_ORDER_COLLECTION_BY_PATTERN, executeResultInfo)
				}
			}

			IBatchOrderPtr BatchOrderCollectionDM::getOrder(basar::db::aspect::AccessorPropertyTable_YIterator yit)
			{
				return getBatchOrderConnectedFactory()->create(yit);
			}

			libutil::domMod::SearchYIteratorPtr BatchOrderCollectionDM::getSearchYIterator()
			{
				if (m_SearchYIterator.get() == NULL)
				{
					m_SearchYIterator = boost::make_shared< libutil::domMod::SearchYIterator >(
						properties::BRANCHNO.toDescriptionString() +
						properties::MANUFACTURERNO.toDescriptionString() +
						properties::MANUFACTURERNAME.toDescriptionString() +
						properties::PHARMACYNO.toDescriptionString() +
						properties::PHARMACYNAME.toDescriptionString() +
						properties::ORDERNO_FROM.toDescriptionString() +
						properties::ORDERNO_TO.toDescriptionString() +
						properties::ORDERTYPE.toDescriptionString() +
						properties::STATUS.toDescriptionString() +
						properties::POS_FROM.toDescriptionString() +
						properties::POS_TO.toDescriptionString() +
						properties::ORDERACQUISITIONDATE_FROM.toDescriptionString() +
						properties::ORDERACQUISITIONDATE_TO.toDescriptionString() +
						properties::ORDERDATE_FROM.toDescriptionString() +
						properties::ORDERDATE_TO.toDescriptionString() +
						properties::EDIDELIVERYDATE_FROM.toDescriptionString() +
						properties::EDIDELIVERYDATE_TO.toDescriptionString() +
						properties::DATASOURCETYPE.toDescriptionString() +
						properties::SORT.toDescriptionString()
					);
				}

				return m_SearchYIterator;
			}

			const log4cplus::Logger& BatchOrderCollectionDM::getLogger() const
			{
				return m_Logger;
			}
		} // end namespace batchOrder
	} // end namespace domMod
} // end namespace libabbauw
