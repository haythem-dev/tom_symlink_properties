#include "transferorderheadcollectionfactory.h"

#include "itransferorderheadcollectiondmgetter.h"
#include "itransferorderheadcollectiondm.h"

#include <libbasarproperty_propertystate.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasardbaspect_accessorpropertytable.h>

#include <libutil/misc/log_macro_definitions.h>

namespace libabbauw
{
	namespace componentManager
	{
		TransferOrderHeadCollectionFactory::TransferOrderHeadCollectionFactory()
		{
		}

		TransferOrderHeadCollectionFactory::~TransferOrderHeadCollectionFactory()
		{
		}

		void TransferOrderHeadCollectionFactory::injectTransferOrderHeadCollectionGetter(ITransferOrderHeadCollectionDMGetterPtr transferOrderHeadCollectionGetter)
		{
			m_TransferOrderHeadCollectionGetter = transferOrderHeadCollectionGetter;
		}

		domMod::transferOrder::ITransferOrderHeadCollectionDMPtr TransferOrderHeadCollectionFactory::create(const AccessorPropertyTableYIteratorCollection& yitColl)
		{
			domMod::transferOrder::ITransferOrderHeadCollectionDMPtr transferOrderHeadCollection = m_TransferOrderHeadCollectionGetter->getTransferOrderHeadCollectionDM();

			// create all yits inside propertytable
			for (AccessorPropertyTableYIteratorCollection::const_iterator it = yitColl.begin(); it != yitColl.end(); ++it)
			{
				transferOrderHeadCollection->addTransferOrderHead(*it);
			}

			return transferOrderHeadCollection;
		}

	} // end namespace componentManager
} // end namespace libabbauw

