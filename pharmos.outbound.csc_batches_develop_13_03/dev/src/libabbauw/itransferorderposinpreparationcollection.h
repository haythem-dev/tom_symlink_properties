#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSINPREPARATIONCOLLECTION_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSINPREPARATIONCOLLECTION_H

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

class ITransferOrderPosInPreparationCollection
{
public:
	virtual ~ITransferOrderPosInPreparationCollection() {}

	virtual void                                               resetSearchYit()                                                             = 0;
	virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit()                                                               = 0;
	virtual basar::db::aspect::AccessorPropertyTableRef        get() const                                                                  = 0;
    virtual basar::db::aspect::AccessorPropertyTableRef        get()                                                                        = 0;
	
	virtual void                                               findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator )    = 0;
    virtual bool                                               isEmpty() const                                                              = 0;
    virtual bool                                               isChanged() const                                                            = 0;
    virtual void                                               save()                                                                       = 0;

    virtual basar::db::aspect::AccessorPropertyTable_YIterator addEmptyPosition()                                                           = 0;
    virtual void                                               markPosAsDeleted( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
    virtual void                                               markAsDeleted()                                                              = 0;

    virtual basar::Int32                                       getNextFreePosNo()                                                           = 0;
    virtual bool                                               containsNonChargedQtyPosition()                                              = 0;
};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSINPREPARATIONCOLLECTION_H
