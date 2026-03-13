#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSCOLLECTION_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSCOLLECTION_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "itransferorderposcollection.h"
#include "transferorderposcollectioninitparams.h"
#include "iordertypegetterptr.h"
#include "istockcollectiondmptr.h"

#include "componentmanager/itransferorderpositionconnectedfactoryptr.h"

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

class TransferOrderPosCollection : public libutil::misc::ComponentInitialization< TransferOrderPosInitParams, TransferOrderPosCollection >, 
                                   public ITransferOrderPosCollection,
	                               private DMBase
{
public:
	TransferOrderPosCollection  ( basar::db::aspect::ConnectionRef );
	~TransferOrderPosCollection ();

	void                                                        injectStockCollection( stock::IStockCollectionDMPtr );
    void                                                        injectOrderTypeGetter( IOrderTypeGetterPtr );
    void                                                        injectTransferOrderPositionConnectedFactory( componentManager::ITransferOrderPositionConnectedFactoryPtr );

	// defined in transfer order pos interface
    void                                                        findById( basar::Int32 orderNo );
	basar::db::aspect::AccessorPropertyTableRef                 get();
    basar::db::aspect::AccessorPropertyTableRef                 get() const;
    void                                                        clear();

    ITransferOrderPositionPtr                                   getPosition( const basar::db::aspect::AccessorPropertyTable_YIterator );
    basar::db::aspect::AccessorPropertyTable_YIterator          addEmptyPosition();
	void                                                        removePosition( const basar::db::aspect::AccessorPropertyTable_YIterator );

    bool                                                        isEmpty()              const;
    bool                                                        isChanged()            const;
    bool                                                        containsNonStockItem() const;

    void                                                        markForDelete();
    void                                                        save();

    void                                                        setOrderNo( const basar::Int32 headOrderNo );

public:
	static const libutil::misc::ClassInfo&                      getClassInfo();

protected:
	// from ComponentInitialization
	void                                                        doInit( const TransferOrderPosInitParams& );
	void                                                        doShutdown();

	// from DMBase
	AccessorExecuteParameterization                             getFindByPatternExecuteParameters() const;
	basar::VarString                                            getSearchYIteratorAttributeNames()  const;

private:
	TransferOrderPosCollection ( const TransferOrderPosCollection& );
	TransferOrderPosCollection operator= ( const TransferOrderPosCollection& );

    void                                                        resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator          getSearchYit();

	void                                                        calculateMissingItems(); // Fehlbestandberechnung

	void														resolveBitFields   (basar::db::aspect::AccessorPropertyTableRef        posCollection);
	void														resolveDeliveryInfo(basar::db::aspect::AccessorPropertyTable_YIterator pos          );
	void														resolveNotice      (basar::db::aspect::AccessorPropertyTable_YIterator pos          );

private:
	TransferOrderPosInitParams                                  m_InitParams;
	stock::IStockCollectionDMPtr                                m_StockCollection;
    IOrderTypeGetterPtr                                         m_OrderType;
    componentManager::ITransferOrderPositionConnectedFactoryPtr m_TransferOrderPositionConnectedFactory;
};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSCOLLECTION_H
