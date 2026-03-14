//**************************************************************************************************
// includes
//**************************************************************************************************
#include <libbasardbaspect_connectionref.h>
#include <libbasarlogin.h>
#include <libbasardbaspect_manager.h>
#include <libbasarlogin_manager.h>
#include "libcscbatch_loggerpool.h"
#include "connection.h"

//**************************************************************************************************
// using declarations
//**************************************************************************************************
using libcscbatch::LoggerPool;

namespace cscbatchauto {
	//**************************************************************************************************
	//         Konstruktoren / Destruktoren
	//**************************************************************************************************
	Connection::Connection() {
	}

	Connection::~Connection() {
	}

	//**************************************************************************************************
	//         Methoden
	//**************************************************************************************************
	//*************************************** connect *****************************************************
	bool Connection::connect(basar::I18nString Server, basar::I18nString Db) {
		m_DbInfo.eConnectType		= basar::INFORMIX;
		m_DbInfo.dbServer			= Server;
		m_DbInfo.database			= Db;

		m_ActivConn = basar::db::aspect::Manager::getInstance().createConnect(m_DbInfo);

		m_ActivConn.setIsolationLevel(basar::INFX_COMITTEDREAD); // do NOT use INFX_DIRTYREAD !!!
		m_ActivConn.setLockModeWait  (basar::WAITSECS, 20);
		//m_ActivConn.setFetBufSize();

		if(LoggerPool::loggerMain.isEnabledFor(log4cplus::INFO_LOG_LEVEL)) {
			if(m_ActivConn.isOpen()) {
				basar::VarString msg;
				msg.format("Connected successfully < %s, %s >.", m_DbInfo.dbServer.c_str(), m_DbInfo.database.c_str());
				LoggerPool::loggerMain.forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
				return true;
			}
			else {
				basar::VarString msg;
				msg.format("ERROR: Could not connect < %s, %s >.", m_DbInfo.dbServer.c_str(), m_DbInfo.database.c_str());
				LoggerPool::loggerMain.forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
			}// else
		}
		return false;
	}// connect

	//*************************************** disconnect *****************************************************
	void Connection::disconnect() {
		//--- disconnect Informix:
		if (!m_ActivConn.isNull()) {
			m_ActivConn.close();
			if (LoggerPool::loggerMain.isEnabledFor(log4cplus::INFO_LOG_LEVEL)) {
				basar::VarString msg;
				msg.format("Disconnected successfully < %s, %s >.", m_DbInfo.dbServer.c_str(), m_DbInfo.database.c_str());
				LoggerPool::loggerMain.forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
			}
		}// if (!m_ActivConn.isNull())
	}// disconnect

	//*************************************** getConnectionCsc *****************************************************
	const ConnectionRef Connection::getConnectionCsc() const {
		return m_ActivConn;
	}// getConnectionCsc
}// namespace
