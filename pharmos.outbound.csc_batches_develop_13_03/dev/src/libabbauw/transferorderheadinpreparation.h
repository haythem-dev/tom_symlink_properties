#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEADINPREPARATION_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEADINPREPARATION_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "itransferorderheadinpreparation.h"
#include "transferorderheadinpreparationinitparams.h"

#include "dmbase.h"
#include <libutil/misc/componentinitialization.hpp>

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
	class ClassInfo;
}

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

class TransferOrderHeadInPreparation : public libutil::misc::ComponentInitialization< TransferOrderHeadInPreparationInitParams, TransferOrderHeadInPreparation >,
                                       public ITransferOrderHeadInPreparation,
	                                   private DMBase
{
public:
	TransferOrderHeadInPreparation ( basar::db::aspect::ConnectionRef );
	~TransferOrderHeadInPreparation ();

    void                                                     injectTransactionFactory( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr );

	// defined in transfer order in preparation interface
	void                                                     resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator       getSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator       get();

    bool                                                     isComplete();
    bool                                                     isChanged();
    bool                                                     isDeleted();

	void                                                     findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator );
    void                                                     save();
    void                                                     markAsDeleted();

public:
	static const libutil::misc::ClassInfo&                   getClassInfo();

protected:
	// from ComponentInitialization
	void                                                     doInit( const TransferOrderHeadInPreparationInitParams& );
	void                                                     doShutdown();

	// from DMBase
	AccessorExecuteParameterization                          getFindByPatternExecuteParameters() const;
	basar::VarString                                         getSearchYIteratorAttributeNames()  const;

    void                                                     remove();

private:
	TransferOrderHeadInPreparation ( const TransferOrderHeadInPreparation& );
	TransferOrderHeadInPreparation operator = ( const TransferOrderHeadInPreparation& );

    void                                                     clearDeleteMark();
    void                                                     markForInsert();

    bool                                                     containsSavableData();

private:
	TransferOrderHeadInPreparationInitParams				 m_InitParams;
    libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr m_DBTransactionFactory;
};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEADINPREPARATION_H
