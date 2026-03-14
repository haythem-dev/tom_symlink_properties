#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSINPREPARATIONCOLLECTION_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSINPREPARATIONCOLLECTION_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "itransferorderposinpreparationcollection.h"
#include "transferorderposinpreparationcollectioninitparams.h"

#include "dmbase.h"
#include <libutil/misc/componentinitialization.hpp>
#include <libbasardbaspect_connectionref.h>
#include <libbasardbaspect_accessor.h>

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

namespace libutil
{
	class ClassInfo;
}

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

class TransferOrderPosInPreparationCollection : public libutil::misc::ComponentInitialization< TransferOrderPosInPreparationCollectionInitParams, TransferOrderPosInPreparationCollection >,
                                                public ITransferOrderPosInPreparationCollection,
	                                            private DMBase
{
public:
	TransferOrderPosInPreparationCollection ( basar::db::aspect::ConnectionRef );
	~TransferOrderPosInPreparationCollection ();

	// defined in transfer order in preparation interface
	void                                                     resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator       getSearchYit();
	basar::db::aspect::AccessorPropertyTableRef				 get();
    basar::db::aspect::AccessorPropertyTableRef				 get() const;

    bool                                                     isEmpty() const;
    bool                                                     isChanged() const;
	void                                                     findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator );
    void                                                     save();

    basar::db::aspect::AccessorPropertyTable_YIterator       addEmptyPosition();
    void                                                     markPosAsDeleted( const basar::db::aspect::AccessorPropertyTable_YIterator );
    void                                                     markAsDeleted();

    basar::Int32                                             getNextFreePosNo();
    bool                                                     containsNonChargedQtyPosition();

public:
	static const libutil::misc::ClassInfo&                   getClassInfo();

protected:
	// from ComponentInitialization
    void                                                     doInit( const TransferOrderPosInPreparationCollectionInitParams& );
	void                                                     doShutdown();

	// from DMBase
	AccessorExecuteParameterization                          getFindByPatternExecuteParameters() const;
	basar::VarString                                         getSearchYIteratorAttributeNames()  const;

private:
	TransferOrderPosInPreparationCollection ( const TransferOrderPosInPreparationCollection& );
	TransferOrderPosInPreparationCollection operator = ( const TransferOrderPosInPreparationCollection& );

private:
	TransferOrderPosInPreparationCollectionInitParams        m_InitParams;
    basar::Int32                                             m_NumberOfPositions;
};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSINPREPARATIONCOLLECTION_H
