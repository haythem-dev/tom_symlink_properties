#ifndef GUARD_CSCBATCHESHISTORY_CONNECTION_H
#define GUARD_CSCBATCHESHISTORY_CONNECTION_H

#include <libbasardbaspect_connectionref.h>
#include <libbasarcmnutil_i18nstring.h>
#include <libbasardbaspect.h>
#include <libbasardbsql_databaseinfo.h>


namespace cscbatchhistory 
{
	class Connection
	{

	public:	
		Connection();
		virtual ~Connection();
		
		bool connectCsc(const basar::I18nString& host, const basar::I18nString& db);
		void disconnectCsc();
		const basar::db::aspect::ConnectionRef getConnectionCsc() const;

	private:
		Connection(const Connection & r);
		Connection & operator =	(const Connection &r);
		
		basar::db::sql::DatabaseInfo		m_DbInfoCSC;
		basar::db::aspect::ConnectionRef	m_ActiveConnCSC;	
	};
}

#endif	// GUARD_CSCBATCHESHISTORY_CONNECTION_H
