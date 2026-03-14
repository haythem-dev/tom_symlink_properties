#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSCOLLECTIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSCOLLECTIONDM_H

#include "itransferorderpositionptr.h"

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

class ITransferOrderPosCollection
{
public:
	virtual ~ITransferOrderPosCollection() {}

    virtual void                                               findById( basar::Int32 orderNo )             = 0;
    virtual void                                               clear()                                      = 0;

	virtual basar::db::aspect::AccessorPropertyTableRef        get()                                        = 0;
    virtual basar::db::aspect::AccessorPropertyTableRef        get() const                                  = 0;

    virtual ITransferOrderPositionPtr                          getPosition( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

    virtual basar::db::aspect::AccessorPropertyTable_YIterator addEmptyPosition()                           = 0;
    virtual void                                               removePosition( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

    virtual void                                               markForDelete()                              = 0;
    virtual void                                               save()                                       = 0;

    virtual bool                                               isChanged() const                            = 0;
    virtual bool                                               isEmpty() const                              = 0;
    virtual bool                                               containsNonStockItem() const                 = 0;

    virtual void                                               setOrderNo( const basar::Int32 headOrderNo ) = 0;
};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSCOLLECTIONDM_H
