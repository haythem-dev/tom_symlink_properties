#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPROTOCOL_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPROTOCOL_H

#include "domainmodule/orderprotocol/iorderheadprotocolptr.h"
#include "domainmodule/orderprotocol/iorderposcollectionprotocolptr.h"

#include "orderprotocolenums.h"

#include <libbasar_definitions.h>

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
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

//----------------------------------------------------------------------------//
// interface declaration section
//----------------------------------------------------------------------------//
namespace libabbauw
{
	namespace domMod
	{
		namespace orderProtocol
		{
			using basar::db::aspect::AccessorPropertyTableRef;
			using basar::db::aspect::AccessorPropertyTable_YIterator;

			class IOrderProtocol
			{
			public:
				virtual ~IOrderProtocol() {}

				virtual IOrderHeadProtocolPtr							getOrderHeadProtocol() const = 0;
				virtual IOrderPosCollectionProtocolPtr					getOrderPosCollectionProtocol() const = 0;

				virtual void											resetSearchYit() = 0;
				virtual AccessorPropertyTable_YIterator					getSearchYit() = 0;

				virtual void											findByKey(const AccessorPropertyTable_YIterator) = 0;
				virtual bool											isEmpty() const = 0;

				virtual void											save() = 0;

				virtual bool											isOrderProposalAllowed(const AccessorPropertyTable_YIterator) = 0;

				virtual void											setBackupHead(const AccessorPropertyTable_YIterator) = 0;
				virtual void											setBackupHeadCollection(const AccessorPropertyTable_YIterator) = 0;

				virtual void											setBackupAllHeadPosCollection(const std::vector<AccessorPropertyTable_YIterator>) = 0;

				virtual std::vector<AccessorPropertyTable_YIterator>  	getBackupHeadCollection() = 0;
				virtual std::vector<AccessorPropertyTable_YIterator>    getBackupAllHeadPosCollection() = 0;

				virtual void											setBackupPos(const AccessorPropertyTable_YIterator) = 0;
				virtual void											setBackupPos(const AccessorPropertyTable_YIterator, const basar::Int16 branchNo) = 0;

				virtual void											setEventType(const EventTypeEnum) = 0;

				virtual void											protocolCreateOrder(const AccessorPropertyTable_YIterator, const AccessorPropertyTableRef) = 0;
				virtual void											protocolChangeOrder(const AccessorPropertyTable_YIterator, const AccessorPropertyTable_YIterator) = 0;
				virtual void											protocolChangeOrderPositionDiscount(const basar::Decimal, const AccessorPropertyTable_YIterator) = 0;
				virtual void											protocolDeletePositon(bool deleteHead) = 0;
				virtual void											protocolOrderRequest(AccessorPropertyTable_YIterator, bool isOrderViewActive) = 0;
				virtual void											protocolCancelOrderRequest(bool isOrderViewActive) = 0;
				virtual void											protocolReleaseOrder(const basar::Int32 orderNoCsc) = 0;
				virtual void											protocolCreateTender() = 0;
				virtual void											protocolCreateTender(const basar::Int32 tenderNo) = 0;
				virtual void											protocolSplitOrder(const AccessorPropertyTable_YIterator, const AccessorPropertyTableRef, const AccessorPropertyTable_YIterator, const AccessorPropertyTableRef, const EventTypeEnum, bool  isOrderViewActive) = 0;

			};

		} // end namespace orderProtocol
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPROTOCOL_H
