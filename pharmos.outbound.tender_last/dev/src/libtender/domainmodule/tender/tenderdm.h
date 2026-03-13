#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDERDM_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDERDM_H

#include <libutil/searchyiterator.h>
#include <libutil/dbtransaction.h>
#include <libutil/classinfo.h>
#include <libutil/util.h>
#include "itender.h"
#include "itenderheaderptr.h"
#include "domainmodule/customerlist/icustomerlistdmptr.h"

namespace libtender
{
namespace domMod
{
namespace tender
{

//TODO: this have to be classes not just typedefs. also not sure if they should reside inside the DM
//implement function to read get the conversion between id and string, actually the VC must know that it is implemented as map
typedef std::map<int, std::string> StatusMapping;
typedef std::map<int, std::string> TypeMapping;

class TenderDM : public ITender
{
public:
	static const libutil::misc::ClassInfo& getClassInfo();

	TenderDM();
	~TenderDM();

	void injectHeader( ITenderHeaderPtr );
	void injectPositionCollection( ITenderPositionCollectionPtr );
	void injectCustomerList( libtender::domMod::customerlist::ICustomerListDMPtr );
	void injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr );
	void setAreaID(basar::Int16 areaID);

	virtual basar::db::aspect::AccessorPropertyTable_YIterator getHeader();
	virtual ITenderPositionCollectionPtr getPositionCollection();
	virtual libtender::domMod::customerlist::ICustomerListDMPtr getCustomerList();

	virtual basar::db::aspect::AccessorPropertyTable_YIterator addEmptyPosition();
	virtual void deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	virtual void save(bool importing = false);
	virtual void deleteTender();
	virtual void findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator );
	virtual void setTenderState( const TenderStateEnum state );
	virtual TenderStateEnum getTenderState() const;

	//virtual bool isEmpty() const;
	//virtual basar::db::aspect::AccessorPropertyTable_YIterator get();
	//virtual basar::db::aspect::AccessorPropertyTable_YIterator get() const;
	//virtual void resetSearchYit();
	//virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit();

private:
	TenderDM( const TenderDM & );
	TenderDM & operator = ( const TenderDM & );

	libutil::domMod::SearchYIteratorPtr getSearchYIterator();

	const log4cplus::Logger & getLogger() const;

	const log4cplus::Logger 												m_Logger;
	libutil::domMod::SearchYIteratorPtr										m_SearchYIterator;
	ITenderHeaderPtr														m_Header;
	ITenderPositionCollectionPtr											m_PositionCollection;
	libtender::domMod::customerlist::ICustomerListDMPtr						m_CustomerList;
	libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr	m_TransactionFactory;

	basar::Int16															m_AreaID;
};

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_TENDER_TENDERDM_H
