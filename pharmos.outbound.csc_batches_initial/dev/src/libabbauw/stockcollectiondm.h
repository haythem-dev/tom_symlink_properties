#ifndef GUARD_LIBABBAUW_DOMMOD_STOCK_STOCKCOLLECTIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_STOCK_STOCKCOLLECTIONDM_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "istockcollectiondm.h"
#include "stockcollectioninitparams.h"
#include "istockparameterptr.h"

#include "dmbase.h"
#include <libutil/misc/componentinitialization.hpp>
#include <boost/unordered_map.hpp>

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
namespace stock
{

class StockCollectionDM : public libutil::misc::ComponentInitialization< StockCollectionInitParams, StockCollectionDM >, 
                          public IStockCollectionDM,
	                      private DMBase
{
public:
	StockCollectionDM( basar::db::aspect::ConnectionRef );
	~StockCollectionDM();

	void                                                   injectStockCalcParam( parameter::IStockParameterPtr );

    // defined in transfer order stock interface
    bool                                                   orderContainsNonStockItem( const basar::Int32 orderNo );

	// defined in stock interface
	void                                                   resetSearchYit();
	basar::db::aspect::AccessorPropertyTable_YIterator     getSearchYit();
    bool                                                   itemInStock( const basar::db::aspect::AccessorPropertyTable_YIterator );
    bool                                                   itemInTransitStorage( const basar::db::aspect::AccessorPropertyTable_YIterator );

	void                                                   findStock( const basar::db::aspect::AccessorPropertyTable_YIterator );
	basar::Int32                                           findCMIReservedQuantity(basar::Int32 articleNo);
	basar::db::aspect::AccessorPropertyTable_YIterator     findByOrderNo( basar::Int32 orderNo );
	basar::db::aspect::AccessorPropertyTable_YIterator     findOrderPositionInOrder( basar::Int32 orderNo, basar::Int32 position );

public:
	static const libutil::misc::ClassInfo&                 getClassInfo();

protected:

	// from ComponentInitialization
	void                                                   doInit( const StockCollectionInitParams& );
	void                                                   doShutdown();

	// from DMBase
	AccessorExecuteParameterization                        getFindByPatternExecuteParameters() const;
	basar::VarString                                       getSearchYIteratorAttributeNames()  const;

	// own
    basar::db::aspect::AccessorPropertyTableRef            get();
    basar::VarString                                       findPseudoStorageLocationSlot();
    basar::Int32                                           findLastPredictionDate();

	void                                                   createIndex();	// Method for creation of an index on stock-list

private:
	StockCollectionDM ( const StockCollectionDM& );
	StockCollectionDM operator = ( const StockCollectionDM& );

	StockCollectionInitParams                                                                        m_InitParams;
	typedef boost::unordered_map< basar::Int32, basar::db::aspect::AccessorPropertyTable_YIterator > PropTabIndex;
	PropTabIndex                                                                                     m_IndexOrderNo;
	parameter::IStockParameterPtr                                                                    m_StockParameterPtr;
    basar::VarString                                                                                 m_PseudoStorageLocationSlot;
    basar::Int32                                                                                     m_PredictionDate;
};

} // end namespace stock
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_STOCK_STOCKCOLLECTIONDM_H
