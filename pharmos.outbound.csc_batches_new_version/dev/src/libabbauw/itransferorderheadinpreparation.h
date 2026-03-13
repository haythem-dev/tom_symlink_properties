#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADINPREPARATION_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADINPREPARATION_H

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

class ITransferOrderHeadInPreparation
{
public:
	virtual ~ITransferOrderHeadInPreparation() {}

	virtual void                                               resetSearchYit()                                                            = 0;
	virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit()                                                              = 0;
	virtual basar::db::aspect::AccessorPropertyTable_YIterator get()                                                                       = 0;

	virtual void                                               findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator )   = 0;
    virtual bool                                               isChanged()                                                                 = 0;
    virtual bool                                               isComplete()                                                                = 0;
    virtual bool                                               isDeleted()                                                                 = 0;
    virtual void                                               save()                                                                      = 0;
    virtual void                                               markAsDeleted()                                                             = 0;
};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADINPREPARATION_H
