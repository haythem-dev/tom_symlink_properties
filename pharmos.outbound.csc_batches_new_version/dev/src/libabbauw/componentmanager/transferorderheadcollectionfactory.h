#ifndef LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADCOLLECTIONFACTORY_H
#define LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADCOLLECTIONFACTORY_H

#include "itransferorderheadcollectionfactory.h"

#include "itransferorderheadcollectiondmgetterptr.h"
#include "itransferorderheadcollectiondmptr.h"

namespace libabbauw
{
namespace componentManager
{
    class TransferOrderHeadCollectionFactory : public ITransferOrderHeadCollectionFactory
    {
    public:
        TransferOrderHeadCollectionFactory();
        ~TransferOrderHeadCollectionFactory();

        void                                                                          injectTransferOrderHeadCollectionGetter( ITransferOrderHeadCollectionDMGetterPtr );

        // from ITransferOrderHeadCollectionFactory interface
        domMod::transferOrder::ITransferOrderHeadCollectionDMPtr                      create( const AccessorPropertyTableYIteratorCollection& );

    private:
	    // don't allow assignment and copying of this class
	    TransferOrderHeadCollectionFactory( const TransferOrderHeadCollectionFactory& );
        TransferOrderHeadCollectionFactory& operator=( const TransferOrderHeadCollectionFactory& );

    private:
        ITransferOrderHeadCollectionDMGetterPtr                                         m_TransferOrderHeadCollectionGetter;
    };

} // end namespace componentManager
} // end namespace libabbauw

#endif // LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERHEADCOLLECTIONFACTORY_H

