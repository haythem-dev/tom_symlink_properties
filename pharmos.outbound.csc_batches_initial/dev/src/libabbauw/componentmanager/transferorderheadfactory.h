#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADFACTORY_H

#include "itransferorderheadfactory.h"
#include "itransferorderheadgetterptr.h"

namespace libabbauw
{
	namespace componentManager
	{
		class TransferOrderHeadFactory : public ITransferOrderHeadFactory
		{
		public:
			TransferOrderHeadFactory();
			void                                           injectTransferOrderHeadGetter(ITransferOrderHeadGetterPtr);

			// from ITransferOrderHeadFactory interface
			domMod::transferOrder::ITransferOrderHeadPtr create(basar::db::aspect::AccessorPropertyTable_YIterator);

		private:
			// don't allow assignment and copying of this class
			TransferOrderHeadFactory(const TransferOrderHeadFactory&);
			TransferOrderHeadFactory& operator=(const TransferOrderHeadFactory&);

		private:
			ITransferOrderHeadGetterPtr      m_TransferOrderHeadGetter;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADFACTORY_H
