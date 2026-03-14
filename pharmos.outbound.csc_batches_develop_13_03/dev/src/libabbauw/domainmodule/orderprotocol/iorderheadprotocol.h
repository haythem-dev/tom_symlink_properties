#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERHEADPROTOCOL_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERHEADPROTOCOL_H

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

			class IOrderHeadProtocol
			{
			public:
				virtual ~IOrderHeadProtocol() {}

				virtual AccessorPropertyTable_YIterator					get() const = 0;
				virtual void											resetSearchYit() = 0;
				virtual AccessorPropertyTable_YIterator					getSearchYit() = 0;

				virtual void											findByKey(const AccessorPropertyTable_YIterator) = 0;
				virtual bool											isEmpty() const = 0;

				virtual void											save() = 0;

				virtual void											setBackupYit(const AccessorPropertyTable_YIterator) = 0;
				virtual void											setBackupHeadCollection(const AccessorPropertyTable_YIterator) = 0;

				virtual AccessorPropertyTable_YIterator					getBackupYit() = 0;
				virtual std::vector<AccessorPropertyTable_YIterator>  	getBackupHeadCollection() = 0;


				virtual void											protocolCreateOrder(const AccessorPropertyTable_YIterator) = 0;
				virtual void											protocolDeleteOrder() = 0;
				virtual void											protocolChangeOrder(const AccessorPropertyTable_YIterator, const AccessorPropertyTable_YIterator, libabbauw::domMod::orderProtocol::EventTypeEnum) = 0;
				virtual void											protocolAddDefferredPaymentDate(const AccessorPropertyTable_YIterator) = 0;
				virtual void										    protocolReleaseOrder(EventTypeEnum, const basar::Int32 orderNoCsc) = 0;
				virtual void										    protocolCreateTender() = 0;
				virtual void										    protocolSplitOrder(const AccessorPropertyTable_YIterator, const basar::VarString, const EventTypeEnum, const basar::Int32 splitFromOrderNo, const basar::Int32 splitToOrderNo) = 0;

			};

		} // end namespace orderProtocol
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERHEADPROTOCOL_H
