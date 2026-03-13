#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERINPREPARATIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERINPREPARATIONDM_H

#include "itransferorderheadinpreparationptr.h"
#include "itransferorderposinpreparationcollectionptr.h"

#include <libbasar_definitions.h>

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
namespace transferOrder
{

class ITransferOrderInPreparationDM
{
public:
	virtual ~ITransferOrderInPreparationDM() {}
	
    virtual void                                                                                            load()             = 0;

    virtual bool                                                                                            isComplete()       = 0;
    virtual bool                                                                                            isChanged()        = 0;
    virtual void                                                                                            save()             = 0;

    virtual libabbauw::domMod::transferOrder::ITransferOrderHeadInPreparationPtr                            getHead()          = 0;
    virtual libabbauw::domMod::transferOrder::ITransferOrderPosInPreparationCollectionPtr                   getPosCollection() = 0;

    virtual basar::db::aspect::AccessorPropertyTable_YIterator                                              addEmptyPosition() = 0;
    virtual void                                                                                            markPosAsDeleted( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
    virtual void                                                                                            markAsDeleted()    = 0;

    virtual basar::Int32                                                                                    getNextFreePosNo() = 0;
    virtual bool                                                                                            containsNonChargedQtyPosition() = 0;
};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_ITRANSFER_ORDER_INPREPARATION_DM_H
