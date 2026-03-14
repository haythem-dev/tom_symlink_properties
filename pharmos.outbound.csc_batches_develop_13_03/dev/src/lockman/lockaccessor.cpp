#include "lockaccessor.h"

#include "ikey.h"
#include "ilockentry.h"

#include "metaclassmacro_definitions.h"
#include "instancechecker_macro_definition.h"
#include "sqlbuilder.h"

#include "lockaccessorexception.h"
#include "alreadylockedexception.h"

#include "liblockmanager_loggerpool.h"
#include <libbasarcmnutil_logging.h>
#include <log4cplus/logger.h>

#include <libbasar_definitions.h>
#include <libbasardbsql.h>

namespace libLockManager
{
	namespace accessor
	{

		LockAccessor::LockAccessor(const basar::VarString& tableName)
			: m_Logger(LoggerPool::loggerAccessor), m_TableName(tableName)
		{
			METHODNAME_DEF(LockAccessor, LockAccessor);
			BLOG_TRACE_METHOD(m_Logger, fun);
		}

		LockAccessor::~LockAccessor()
		{
			METHODNAME_DEF(LockAccessor, ~LockAccessor);
			BLOG_TRACE_METHOD(m_Logger, fun);
		}

		void LockAccessor::injectConnection(basar::db::sql::ConnectionRef connection)
		{
			m_Connection = connection;
		}

		basar::db::sql::ConnectionRef LockAccessor::getConnection()
		{
			return m_Connection;
		}

		void LockAccessor::checkForTransaction()
		{
			// throws exception if this method has been called and a transaction is open
			METHODNAME_DEF(LockAccessor, checkForTransaction);

			// getConnection().isInTransaction() doesn't do the right thing in this case because it
			// only works for transactions opened with the basar bindings and ignores those which
			// are opened from ESQLC

			try
			{
				getConnection().begin(); // if transaction is already open this will throw
				getConnection().rollback();
			}
			catch (basar::Exception&)
			{
				basar::VarString msg = "Open transaction detected!";
				throw exceptions::LockAccessorException(basar::ExceptInfo(fun, msg, __FILE__, __LINE__));
			}
		}

		void LockAccessor::eraseStaleLocks()
		{
			METHODNAME_DEF(LockAccessor, eraseStaleLocks);
			BLOG_TRACE_METHOD(m_Logger, fun);

			checkForTransaction();

			static const basar::VarString sql(
				"DELETE FROM " + m_TableName + " "
				"WHERE sessionid NOT IN( SELECT sid FROM sysmaster:syssessions )"
			);

			BLOG_DEBUG(m_Logger, sql);

			basar::db::sql::StatementRef statement = getConnection().createStatement();
			statement.execute(sql);
		}

		bool LockAccessor::findByKey(const IKey& key)
		{
			METHODNAME_DEF(LockAccessor, findByKey);
			BLOG_TRACE_METHOD(m_Logger, fun);

			util::SQLBuilder builder(m_TableName);
			basar::VarString sql = builder.createCountStatement(key);
			BLOG_DEBUG(m_Logger, sql);

			basar::db::sql::StatementRef statement = getConnection().createStatement();
			basar::db::sql::ResultsetRef result = statement.executeSingleQuery(sql);

			return result.next() && result.getInt32(0) == 1;
		}

		void LockAccessor::save(const ILockEntry& lockEntry)
		{
			METHODNAME_DEF(LockAccessor, save);
			BLOG_TRACE_METHOD(m_Logger, fun);

			checkForTransaction();

			util::SQLBuilder builder(m_TableName);
			basar::VarString sql = builder.createInsertStatement(lockEntry);
			BLOG_DEBUG(m_Logger, sql);

			basar::db::sql::StatementRef statement = getConnection().createStatement();
			basar::db::sql::ExecuteReturnInfo info = statement.execute(sql, false);
			if (info.hasError())
			{
				if (info.getError() == basar::db::sql::DBErrorCode::INSERT_DUPL)
				{
					eraseStaleLocks(); // When the DB connection aborts, stale locks can occur. Delete them here and try to create the lock again
					// to ensure the holding lock is still valid!

					info = statement.execute(sql, false);
					if (info.hasError() && info.getError() == basar::db::sql::DBErrorCode::INSERT_DUPL)
					{
						basar::VarString msg = "Already locked!";
						throw libLockManager::exceptions::AlreadyLockedException(basar::ExceptInfo(fun, msg, __FILE__, __LINE__));
					}
				}
				else
				{
					statement.execute(sql, true); // execute again to deliver exception to client
				}
			}
		}

		void LockAccessor::erase(const IKey& key)
		{
			METHODNAME_DEF(LockAccessor, erase);
			BLOG_TRACE_METHOD(m_Logger, fun);

			checkForTransaction();

			util::SQLBuilder builder(m_TableName);
			basar::VarString sql = builder.createDeleteStatement(key);
			BLOG_DEBUG(m_Logger, sql);

			basar::db::sql::StatementRef statement = getConnection().createStatement();
			statement.execute(sql);
		}
	} // end namespace accessor
} // end namespace libLockManager

