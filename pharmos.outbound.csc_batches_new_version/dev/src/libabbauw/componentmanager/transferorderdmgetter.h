#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERDMGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERDMGETTER_H

#include "itransferorderdmgetter.h"

#include "itransferorderheadgetterptr.h"
#include "itransferorderposcollectiongetterptr.h"
#include "icscordergetterptr.h"
#include "iorderreleaseparameterptr.h"

#include <libbasardbaspect_connectionref.h>
#include <libutil/dbtransaction.h>

namespace libabbauw
{
	namespace componentManager
	{
		class TransferOrderDMGetter : public ITransferOrderDMGetter
		{
		public:
			TransferOrderDMGetter();
			~TransferOrderDMGetter();

			void                                                                 injectConnection(basar::db::aspect::ConnectionRef);
			void                                                                 injectTransactionFactory(libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr);

			void                                                                 injectTransferOrderHeadGetter(ITransferOrderHeadGetterPtr);
			void                                                                 injectTransferOrderPosCollectionGetter(ITransferOrderPosCollectionGetterPtr);
			void                                                                 injectCscOrderGetter(ICSCOrderGetterPtr);
			void                                                                 injectOrderReleaseParameter(domMod::parameter::IOrderReleaseParameterPtr);

			// from ITransferOrderDMGetter interface
			domMod::transferOrder::ITransferOrderPtr                             getTransferOrderDM();

		private:
			// forbidden
			TransferOrderDMGetter(const TransferOrderDMGetter&);
			TransferOrderDMGetter& operator = (const TransferOrderDMGetter&);

		private:
			basar::db::aspect::ConnectionRef	                                 m_Connection;
			libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr m_TransactionFactory;

			ITransferOrderHeadGetterPtr                                          m_TransferOrderHeadGetter;
			ITransferOrderPosCollectionGetterPtr                                 m_TransferOrderPosCollectionGetter;
			ICSCOrderGetterPtr                                                   m_CscOrderGetter;
			domMod::parameter::IOrderReleaseParameterPtr                         m_OrderReleaseParameterPtr;
		};
	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERDMGETTER_H
