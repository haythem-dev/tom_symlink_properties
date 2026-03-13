#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERDM_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "itransferorder.h"

#include "itransferorderheadptr.h"
#include "itransferorderposcollectionptr.h"

#include "icscorderreleaserptr.h"
#include "iorderreleaseparameterptr.h"

#include <libutil/dbtransaction.h>

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning(disable: 4512 )
#endif
#include <QtCore/QCoreApplication>
#ifdef _MSC_VER
#pragma warning (pop)
#endif


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

namespace libutil
{
	namespace misc
	{
		class ClassInfo;
	}
}

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{

			class TransferOrderDM : public ITransferOrder
			{
				Q_DECLARE_TR_FUNCTIONS(TransferOrderDM)

			public:
				TransferOrderDM();
				~TransferOrderDM();

				void                                                     injectTransactionFactory(libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr);
				void                                                     injectCscOrderReleaser(cscorder::ICSCOrderReleaserPtr);

				void                                                     injectHead(ITransferOrderHeadPtr);
				void                                                     injectPosCollection(ITransferOrderPosCollectionPtr);

				void                                                     injectOrderReleaseParameter(parameter::IOrderReleaseParameterPtr);

				// defined in ITransferOrderGetter interface
				ITransferOrderHeadPtr                                    getHead();
				ITransferOrderHeadPtr                                    getHead() const;
				ITransferOrderPosCollectionPtr                           getPosCollection();
				ITransferOrderPosCollectionPtr                           getPosCollection() const;

				bool                                                     isEmpty() const;

				basar::Int32                                             getOrderNo()       const;
				basar::Int32                                             getPositionCount() const;
				basar::Decimal                                           getOrderValue()    const;
				OrderStatus                                              getOrderStatus()   const;
				TrafficLightEnum                                         getColor()         const;

				// defined in ITransferOrderDM interface
				void                                                     findById(basar::Int32 orderNo);

				// used only for CPR0002688 to avoid double locking
				void                                                     findByIdWithoutLock(basar::Int32 orderNo);

				void                                                     save();
				void                                                     clear();
				bool                                                     isChanged() const;

				basar::db::aspect::AccessorPropertyTable_YIterator       addEmptyPosition();
				void                                                     removePosition(basar::db::aspect::AccessorPropertyTable_YIterator);

				void                                                     release(bool postpone);

				// defined in IOrderTypeGetter interface
				bool                                                     hasOrderTypeOM() const;

			public:
				static const libutil::misc::ClassInfo& getClassInfo();

			private:
				TransferOrderDM(const TransferOrderDM&);
				TransferOrderDM operator = (const TransferOrderDM&);

				const log4cplus::Logger& getLogger()            const;
				void                                                     checkReleasable()      const;
				void                                                     checkForNonStockItem() const;
				void                                                     calculateOrderColor();

			private:
				const log4cplus::Logger& m_Logger;

				libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr m_DBTransactionFactory;

				ITransferOrderHeadPtr                                    m_TransferOrderHead;
				ITransferOrderPosCollectionPtr                           m_TransferOrderPosCollection;

				cscorder::ICSCOrderReleaserPtr                           m_CscOrderReleaser;
				parameter::IOrderReleaseParameterPtr                     m_OrderReleaseParameterPtr;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERDM_H
