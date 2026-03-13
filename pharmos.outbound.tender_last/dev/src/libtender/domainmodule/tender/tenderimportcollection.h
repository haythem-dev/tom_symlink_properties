#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_IMPORT_COLLECTION_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_IMPORT_COLLECTION_H

#include "itenderimportcollection.h"
#include <componentmanager/itenderconnectedfactoryptr.h>
#include <map>
#include <libbasarcmnutil_i18nstring.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libutil/dbtransaction.h>

namespace log4cplus
{
class Logger;
}

namespace libtender
{
namespace domMod
{
namespace tender
{
class TenderImportCollection : public ITenderImportCollection
{
public:
	TenderImportCollection( const log4cplus::Logger logger );
	virtual ~TenderImportCollection();

	void injectTenderFactory( componentManager::ITenderConnectedFactoryPtr tenderFactory );
	void injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory );

	virtual basar::Int32 getTenderCount() const;
	virtual basar::Int32 getStartingTenderNo() const;
	virtual void addPosition( const TenderImportStruct & values );
	virtual void save(bool importing = false);
	virtual void clear();
	virtual bool isValidID( const basar::Int32 id ) const;
	virtual basar::Int32 getTenderID( const basar::Int32 id ) const;
	virtual ITenderPtr getTender( const basar::Int32 id ) const;
	
private:
	TenderImportCollection( const TenderImportCollection & );
	TenderImportCollection & operator=( const TenderImportCollection & );

	ITenderPtr getTender( const TenderImportStruct & values );
	void matchToHeader( basar::db::aspect::AccessorPropertyTable_YIterator yitHead, const TenderImportStruct & values );
	void matchToPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitPos, const TenderImportStruct & values );

	typedef std::map<basar::Int32, ITenderPtr> InnerCollection;

	const log4cplus::Logger													m_Logger;
	InnerCollection															m_Tenders;
	componentManager::ITenderConnectedFactoryPtr							m_TenderFactory;
	libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr	m_TransactionFactory;
};

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_IMPORT_COLLECTION_H
