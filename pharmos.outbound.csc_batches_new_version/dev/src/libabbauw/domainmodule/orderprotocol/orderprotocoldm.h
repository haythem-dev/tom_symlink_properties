#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERPROTOCOLDM_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERPROTOCOLDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "iorderprotocol.h"

#include "domainmodule/orderprotocol/iorderheadprotocolptr.h"
#include "domainmodule/orderprotocol/iorderposcollectionprotocolptr.h"

#include <libutil/dbtransaction.h>
#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libabbauw
{
	namespace domMod
	{
		namespace orderProtocol
		{
			using basar::db::aspect::AccessorPropertyTable_YIterator;
			using basar::db::aspect::AccessorPropertyTableRef;

			class OrderProtocolDM : public IOrderProtocol
			{
			public:
				static const libutil::misc::ClassInfo& getClassInfo();

			public:
				OrderProtocolDM();
				~OrderProtocolDM();

				void											injectOrderHeadProtocol(IOrderHeadProtocolPtr);
				void											injectOrderPosCollectionProtocol(IOrderPosCollectionProtocolPtr);
				void											injectTransactionFactory(libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory);

				//from IOrderProtocol
				IOrderHeadProtocolPtr							getOrderHeadProtocol() const;
				IOrderPosCollectionProtocolPtr					getOrderPosCollectionProtocol() const;

				void											resetSearchYit();
				AccessorPropertyTable_YIterator					getSearchYit();

				void											findByKey(const AccessorPropertyTable_YIterator);
				bool											isEmpty() const;

				void											save();

				bool											isOrderProposalAllowed(const AccessorPropertyTable_YIterator);

				void											setBackupHead(const AccessorPropertyTable_YIterator);
				void											setBackupHeadCollection(const AccessorPropertyTable_YIterator);

				void											setBackupAllHeadPosCollection(const std::vector<AccessorPropertyTable_YIterator>);

				std::vector<AccessorPropertyTable_YIterator>   	getBackupHeadCollection();
				std::vector<AccessorPropertyTable_YIterator>	getBackupAllHeadPosCollection();

				void											setBackupPos(const AccessorPropertyTable_YIterator);
				void											setBackupPos(const AccessorPropertyTable_YIterator, const basar::Int16 branchNo);

				void											setEventType(const EventTypeEnum);

				void											protocolCreateOrder(const AccessorPropertyTable_YIterator, const AccessorPropertyTableRef);
				void											protocolChangeOrder(const AccessorPropertyTable_YIterator, const AccessorPropertyTable_YIterator);
				void											protocolChangeOrderPositionDiscount(const basar::Decimal, const AccessorPropertyTable_YIterator);
				void											protocolDeletePositon(bool deleteHead);
				void											protocolOrderRequest(AccessorPropertyTable_YIterator, bool isOrderViewActive);
				void											protocolCancelOrderRequest(bool isOrderViewActive);
				void											protocolReleaseOrder(const basar::Int32 orderNoCsc);
				void											protocolCreateTender();
				void											protocolCreateTender(const basar::Int32 tenderNo);
				void											protocolSplitOrder(const AccessorPropertyTable_YIterator, const AccessorPropertyTableRef, const AccessorPropertyTable_YIterator, const AccessorPropertyTableRef, const EventTypeEnum, bool isOrderViewActive);


			private:
				// forbidden
				OrderProtocolDM(const OrderProtocolDM&);
				OrderProtocolDM& operator = (const OrderProtocolDM&);

				libutil::domMod::SearchYIteratorPtr                 getSearchYIterator();
				const log4cplus::Logger& getLogger() const;

			private:
				const log4cplus::Logger& m_Logger;
				libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;

				IOrderHeadProtocolPtr								m_Head;
				IOrderPosCollectionProtocolPtr						m_PosCollection;

				libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr m_TransactionFactory;

				//Is only needed for release order!
				EventTypeEnum                                       m_EventType;
			};

		} // end namespace orderProtocol
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERPROTOCOLDM_H
