#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPOSCOLLECTIONPROTOCOL_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPOSCOLLECTIONPROTOCOL_H

#include <libbasar_definitions.h>
#include "orderprotocolenums.h"

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
			using basar::db::aspect::AccessorPropertyTable_YIterator;
			using basar::db::aspect::AccessorPropertyTableRef;

			class IOrderPosCollectionProtocol
			{
			public:
				virtual ~IOrderPosCollectionProtocol() {}

				virtual AccessorPropertyTableRef							get() const = 0;
				virtual void												resetSearchYit() = 0;
				virtual AccessorPropertyTable_YIterator						getSearchYit() = 0;

				virtual void												findByKey(const AccessorPropertyTable_YIterator) = 0;
				virtual bool												isEmpty() const = 0;

				virtual void												save() = 0;

				virtual void												setBackupAllHeadPosCollection(std::vector<AccessorPropertyTable_YIterator>) = 0;
				
				virtual std::vector<AccessorPropertyTable_YIterator>		getBackupAllHeadPosCollection() = 0;

				virtual void												setBackupPos(const AccessorPropertyTable_YIterator) = 0;
				virtual void												setBackupPos(const AccessorPropertyTable_YIterator, const basar::Int16 branchNo) = 0;

				virtual void												protocolCreateOrder(AccessorPropertyTableRef) = 0;
				virtual void												protocolChangeOrderPositionDiscount(const basar::Decimal, const AccessorPropertyTable_YIterator) = 0; 
				virtual void												protocolDeletePositon() = 0;
				virtual void												protocolOrderRequest(AccessorPropertyTable_YIterator, bool isOrderViewActive) = 0;
				virtual void												protocolCancelOrderRequest(bool isOrderViewActive) = 0;
				virtual void												protocolSplitOrder(AccessorPropertyTableRef, const basar::VarString, const EventTypeEnum, bool  isOrderViewActive) = 0;
				virtual void												protocolCreateTender(const basar::Int32 tenderNo) = 0;
			};

		} // end namespace orderProtocol
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPOSCOLLECTIONPROTOCOL_H
