#include <libbasardbaspect.h>
#include <libbasardbaspect_exceptions.h>
#include <libbasardbsql_exceptions.h>
#include <libbasarproperty.h>
#include <libbasardbsql_connectionref.h>
#include "loggerpool.h"
#include "searchyiterator.h"

namespace domMod 
{
	namespace searchyiterator 
	{
		SearchYIterator::SearchYIterator(const basar::VarString& valuePairs, log4cplus::Logger& logger)
			: m_Logger(logger)
		{
			static basar::Int32 counter = 1;
			basar::VarString strCounter;
			strCounter.itos(counter++);
			m_AccessorName = "SearchAccessor";
			m_AccessorName.append(strCounter);

			basar::db::aspect::AccessorDefinitionRef adr = basar::db::aspect::Manager::getInstance().createAccessorDefinition(m_AccessorName);
			basar::property::PropertyDescriptionListRef pdl = adr.getPropertyDescriptionList();
			pdl.construct(valuePairs);
		}

		SearchYIterator::~SearchYIterator()
		{
			if (false == m_SearchAcc.isNull())
			{
				m_SearchAcc.reset();
			}
		}

		basar::db::aspect::AccessorPropertyTableRef SearchYIterator::getPropTab() const
		{
			return getAccessor().getPropertyTable();
		}

		basar::db::aspect::AccessorPropertyTable_YIterator SearchYIterator::get() const
		{
			init();
			return getPropTab().begin();
		}

		void SearchYIterator::init() const
		{
			if (getPropTab().empty())
			{
				getPropTab().insert(basar::FOR_UNKNOWN);
			}
		}

		void SearchYIterator::reset() const
		{
			basar::db::aspect::AccessorPropertyTable_YIterator yit = get();
			basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin();
			while (false == xit.isEnd())
			{
				xit.setPropertyState(basar::SS_UNSET);
				++xit;
			}
		}

		basar::db::aspect::AccessorInstanceRef SearchYIterator::getAccessor() const
		{
			initAccessor();
			return m_SearchAcc;
		}

		void SearchYIterator::initAccessor() const
		{
			if (m_SearchAcc.isNull())
			{
				basar::VarString instanceName = createAccessorInstanceName(m_AccessorName);
				m_SearchAcc = basar::db::aspect::Manager::getInstance().createAccessorInstance(instanceName, m_AccessorName,
					basar::db::aspect::ConnectionRef(), basar::db::aspect::FULL_CACHING);
			}
		}

		const basar::VarString SearchYIterator::createAccessorInstanceName(const basar::VarString& accessorName) const
		{
			static basar::Int32 instanceCounter = 1;
			std::stringstream instanceName; instanceName << accessorName << "Instance_" << instanceCounter++;
			return instanceName.str().c_str();
		}

	} // end namespace searchyiterator
} // end namespace domMod
