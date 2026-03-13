#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERPOSCOLLECTIONPROTOCOLDM_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERPOSCOLLECTIONPROTOCOLDM_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "iorderposcollectionprotocol.h"

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

			class OrderPosCollectionProtocolDM : public IOrderPosCollectionProtocol
			{
			public:
				static const libutil::misc::ClassInfo& getClassInfo();

			public:
				OrderPosCollectionProtocolDM();
				~OrderPosCollectionProtocolDM();

				void												injectAccessor(libutil::infrastructure::accessor::IAccessorPtr);

				//from IOrderProtocol
				AccessorPropertyTableRef							get() const;
				void                                                resetSearchYit();
				AccessorPropertyTable_YIterator						getSearchYit();

				void                                                findByKey(const AccessorPropertyTable_YIterator);
				bool                                                isEmpty() const;

				void                                                save();

				void												setBackupAllHeadPosCollection(std::vector<AccessorPropertyTable_YIterator>);
				std::vector<AccessorPropertyTable_YIterator>		getBackupAllHeadPosCollection();

				void												setBackupPos(const AccessorPropertyTable_YIterator);
				void												setBackupPos(const AccessorPropertyTable_YIterator, const basar::Int16 branchNo);

				void												protocolCreateOrder(AccessorPropertyTableRef);
				void												protocolChangeOrderPositionDiscount(const basar::Decimal, const AccessorPropertyTable_YIterator);
				void												protocolDeletePositon();
				void												protocolOrderRequest(AccessorPropertyTable_YIterator, bool isOrderViewActive);
				void												protocolCancelOrderRequest(bool isOrderViewActive);
				void												protocolSplitOrder(AccessorPropertyTableRef, const basar::VarString, const EventTypeEnum, bool isOrderViewActive);
				void												protocolCreateTender(const basar::Int32 tenderNo);

			private:
				// forbidden
				OrderPosCollectionProtocolDM(const OrderPosCollectionProtocolDM&);
				OrderPosCollectionProtocolDM& operator = (const OrderPosCollectionProtocolDM&);

				libutil::infrastructure::accessor::IAccessorPtr		getAccessor() const;
				libutil::domMod::SearchYIteratorPtr                 getSearchYIterator();
				const log4cplus::Logger& getLogger() const;

				void setProtocolValues(AccessorPropertyTable_YIterator& yitPosProtocol, basar::I18nString activityType, libabbauw::domMod::orderProtocol::EventTypeEnum eventType, basar::I18nString view, int orderRequestQuantity = 0);

			private:
				const log4cplus::Logger& m_Logger;
				libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;
				libutil::infrastructure::accessor::IAccessorPtr     m_Accessor;

				AccessorPropertyTable_YIterator						m_BackUpPos;
				std::vector<AccessorPropertyTable_YIterator>		m_BackUpAllHeadPosCollection;
			};

		} // end namespace orderProtocol
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERPOSCOLLECTIONPROTOCOLDM_H
