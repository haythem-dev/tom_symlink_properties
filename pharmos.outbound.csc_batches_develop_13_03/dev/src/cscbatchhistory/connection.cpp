#include <libbasarlogin.h>
#include <libbasardbaspect_manager.h>
#include <libbasarlogin_manager.h>
#include "loggerpool.h"
#include "connection.h"

namespace cscbatchhistory 
{
	Connection::Connection() 
	{ 
	}

	Connection::~Connection() 
	{ 
	}

	bool Connection::connectCsc(const basar::I18nString& host, const basar::I18nString& db) 
	{
		const char *function="Connection::connectCsc().";
		BLOG_TRACE_METHOD(LoggerPool::loggerMain, function);

		try 
		{
			m_DbInfoCSC.eConnectType		= basar::INFORMIX;
			m_DbInfoCSC.dbServer			= host;
			m_DbInfoCSC.database			= db;
			m_DbInfoCSC.concurrTransaction  = true;
			m_ActiveConnCSC = basar::db::aspect::Manager::getInstance().createConnect(m_DbInfoCSC);

			m_ActiveConnCSC.setIsolationLevel(basar::INFX_COMITTEDREAD); // do NOT use INFX_DIRTYREAD !!!
			m_ActiveConnCSC.setLockModeWait  (basar::WAITSECS, 20);

			if (LoggerPool::loggerMain.isEnabledFor(log4cplus::INFO_LOG_LEVEL) && m_ActiveConnCSC.isOpen()) 
			{
				if(m_ActiveConnCSC.isOpen()) 
				{
					basar::VarString msg;
					msg.format("cscbatchhistory connected successfully < %s, %s >.", m_DbInfoCSC.dbServer.c_str(), m_DbInfoCSC.database.c_str());
					LoggerPool::loggerMain.forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
					return true;
				}
				else 
				{
					basar::VarString msg;
					msg.format("ERROR: Could not connect < %s, %s >.", m_DbInfoCSC.dbServer.c_str(), m_DbInfoCSC.database.c_str());
					LoggerPool::loggerMain.forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
				}
			}

			return false;
		}
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerMain, function); 
			throw;
		}
	}

	void Connection::disconnectCsc()
	{
		const char *function="Connection::disconnectCsc().";
		BLOG_TRACE_METHOD(LoggerPool::loggerMain, function);

		try 
		{
			//--- disconnect Informix:
			if (!m_ActiveConnCSC.isNull()) 
			{
				m_ActiveConnCSC.close();
				if (LoggerPool::loggerMain.isEnabledFor(log4cplus::INFO_LOG_LEVEL)) 
				{
					basar::VarString msg;
					msg.format("cscbatchhistory disconnected successfully < %s, %s >.", m_DbInfoCSC.dbServer.c_str(), m_DbInfoCSC.database.c_str());
					LoggerPool::loggerMain.forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
				}
			}
		}
		catch(...)
		{
			BLOG_ERROR(LoggerPool::loggerMain, function); 
			throw;
		}
	}

	const basar::db::aspect::ConnectionRef Connection::getConnectionCsc() const 
	{
		return m_ActiveConnCSC;
	}
}
