#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERHEADPROTOCOLDM_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERHEADPROTOCOLDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "iorderheadprotocol.h"

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

			class OrderHeadProtocolDM : public IOrderHeadProtocol
			{
			public:
				static const libutil::misc::ClassInfo& getClassInfo();

			public:
				OrderHeadProtocolDM();
				~OrderHeadProtocolDM();

				void												injectAccessor(libutil::infrastructure::accessor::IAccessorPtr);

				//from IOrderProtocol
				AccessorPropertyTable_YIterator						get() const;
				void												resetSearchYit();
				AccessorPropertyTable_YIterator						getSearchYit();

				void												findByKey(const AccessorPropertyTable_YIterator);
				bool												isEmpty() const;

				void												save();

				void												setBackupYit(const AccessorPropertyTable_YIterator);
				void												setBackupHeadCollection(const AccessorPropertyTable_YIterator);

				AccessorPropertyTable_YIterator						getBackupYit();
				std::vector<AccessorPropertyTable_YIterator>		getBackupHeadCollection();

				void												protocolCreateOrder(const AccessorPropertyTable_YIterator);
				void												protocolDeleteOrder();
				void												protocolChangeOrder(const AccessorPropertyTable_YIterator, const AccessorPropertyTable_YIterator yitBackup, libabbauw::domMod::orderProtocol::EventTypeEnum = NO_EVENT_TYPE);
				void												protocolAddDefferredPaymentDate(const AccessorPropertyTable_YIterator);
				void											    protocolReleaseOrder(EventTypeEnum, const basar::Int32 orderNoCsc);
				void											    protocolCreateTender();
				void											    protocolSplitOrder(const AccessorPropertyTable_YIterator, const basar::VarString, const EventTypeEnum, const basar::Int32 splitFromOrderNo, const basar::Int32 splitToOrderNo);

			private:
				// forbidden
				OrderHeadProtocolDM(const OrderHeadProtocolDM&);
				OrderHeadProtocolDM& operator = (const OrderHeadProtocolDM&);

				libutil::infrastructure::accessor::IAccessorPtr		getAccessor() const;
				libutil::domMod::SearchYIteratorPtr                 getSearchYIterator();
				const log4cplus::Logger& getLogger() const;

			private:
				const log4cplus::Logger& m_Logger;
				libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;
				libutil::infrastructure::accessor::IAccessorPtr     m_Accessor;

				AccessorPropertyTable_YIterator						m_BackUpYit;
				std::vector<AccessorPropertyTable_YIterator>  		m_BackUpHeadCollection;
			};

		} // end namespace orderProtocol
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERHEADPROTOCOLDM_H
