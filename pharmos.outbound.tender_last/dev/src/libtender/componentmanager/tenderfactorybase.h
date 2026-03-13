#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_TENDERFACTORYBASE_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_TENDERFACTORYBASE_H

#include <domainmodule/tender/itenderheaderptr.h>
#include <domainmodule/tender/itenderpositioncollectionptr.h>
#include <domainmodule/tender/itenderptr.h>
#include <domainmodule/customerlist/icustomerlistdmptr.h>
#include <libutil/accessor.h>
#include <libutil/dbconnection.h>
#include <libutil/infrastructure/session/isessionptr.h>
#include <libutil/dbtransaction.h>
#include <infrastructure/countryidenum.h>

namespace libtender
{
namespace componentManager
{
class TenderFactoryBase
{
public:
	virtual ~TenderFactoryBase();
	void injectDBConnection( basar::db::aspect::ConnectionRef );
	void injectSession( libutil::infrastructure::session::ISessionPtr session );
	void injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr	transactionFactory );

protected:
	TenderFactoryBase();
	domMod::tender::ITenderPtr create() const;

private:
	TenderFactoryBase( const TenderFactoryBase & );
	TenderFactoryBase & operator=( const TenderFactoryBase & );

	libtender::domMod::tender::ITenderHeaderPtr getTenderHeader() const;
	libtender::domMod::tender::ITenderPositionCollectionPtr getTenderPositionCollection() const;
	libtender::domMod::customerlist::ICustomerListDMPtr getCustomerList() const;

	libutil::infrastructure::accessor::IAccessorPtr getTenderHeaderAccessor() const;
	libutil::infrastructure::accessor::IAccessorPtr getTenderPositionCollectionAccessor() const;
	libutil::infrastructure::accessor::IAccessorPtr getCustomerListAccessor() const;

	basar::db::aspect::ConnectionRef getDBConnection() const;
	libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr getTransactionFactory() const;
	infrastructure::CountryIDEnum getCountryID() const;

private:
	basar::db::aspect::ConnectionRef										m_DBConnection;
	libutil::infrastructure::session::ISessionPtr							m_Session;
	libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr	m_TransactionFactory;
};

}
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_TENDERFACTORYBASE_H
