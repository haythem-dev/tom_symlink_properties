#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERFACTORY_H

#include "itransferorderfactory.h"
#include "icreatesplittedheadfactory.h"
#include "itransferorderdmgetterptr.h"

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning(disable: 4127 4512 )
#endif 
#include <QtCore/QCoreApplication>
#ifdef _MSC_VER
#pragma warning (pop)
#endif 

namespace libabbauw
{
	namespace componentManager
	{
		class TransferOrderFactory : public ITransferOrderFactory, public ICreateSplittedHeadFactory
		{
			Q_DECLARE_TR_FUNCTIONS(TransferOrderFactory)

		public:
			TransferOrderFactory();
			void                                           injectTransferOrderDMGetter(ITransferOrderDMGetterPtr);

			// from ITransferOrderFactory interface
			domMod::transferOrder::ITransferOrderPtr       create(basar::db::aspect::AccessorPropertyTable_YIterator);

			// used only for CPR0002688 to avoid double locking
			domMod::transferOrder::ITransferOrderPtr       createWithoutLock(basar::db::aspect::AccessorPropertyTable_YIterator);

			// from ICreateSplittedHeadFactory interface
			domMod::transferOrder::ITransferOrderPtr       createSplittedHead(domMod::transferOrder::ITransferOrderPtr);

		private:
			// don't allow assignment and copying of this class
			TransferOrderFactory(const TransferOrderFactory&);
			TransferOrderFactory& operator=(const TransferOrderFactory&);

		private:
			ITransferOrderDMGetterPtr                      m_TransferOrderDMGetter;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERFACTORY_H
