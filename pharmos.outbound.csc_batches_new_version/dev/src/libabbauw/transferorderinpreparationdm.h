#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERINPREPARATIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERINPREPARATIONDM_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "itransferorderinpreparationdm.h"

#include "itransferorderheadinpreparationptr.h"
#include "itransferorderposinpreparationcollectionptr.h"

#include <libutil/dbtransaction.h>
//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class ConnectionRef;
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

class TransferOrderInPreparationDM : public ITransferOrderInPreparationDM
{
public:
	TransferOrderInPreparationDM();
	~TransferOrderInPreparationDM();

    void                                                     injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr );
    void                                                     injectHead( ITransferOrderHeadInPreparationPtr );
    void                                                     injectPos ( ITransferOrderPosInPreparationCollectionPtr );
   
    // defined in ITransferOrderInPreparationDM interface
    bool                                                     isComplete();
    bool                                                     isChanged();
    void                                                     save();
    void                                                     load();

    ITransferOrderHeadInPreparationPtr                       getHead();
    ITransferOrderPosInPreparationCollectionPtr              getPosCollection();

    basar::db::aspect::AccessorPropertyTable_YIterator       addEmptyPosition();
    void                                                     markPosAsDeleted( const basar::db::aspect::AccessorPropertyTable_YIterator );
    void                                                     markAsDeleted();

    basar::Int32                                             getNextFreePosNo();
    bool                                                     containsNonChargedQtyPosition();

public:
	static const libutil::misc::ClassInfo&                   getClassInfo();

private:
	TransferOrderInPreparationDM ( const TransferOrderInPreparationDM& );
	TransferOrderInPreparationDM operator = ( const TransferOrderInPreparationDM& );

    const log4cplus::Logger&							     getLogger() const;

private:
	const log4cplus::Logger&								 m_Logger;
    libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr m_DBTransactionFactory;

    ITransferOrderHeadInPreparationPtr                       m_TransferOrderHead;
    ITransferOrderPosInPreparationCollectionPtr              m_TransferOrderPosCollection;
};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERINPREPARATIONDM_H
