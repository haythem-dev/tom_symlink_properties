#ifndef GUARD_LIBABBAUW_DOMMOD_STOCK_STOCKCOLLECTIONINITPARAMS_H
#define GUARD_LIBABBAUW_DOMMOD_STOCK_STOCKCOLLECTIONINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace stock
{

    struct StockCollectionInitParams
    {
	    basar::Int16 m_BranchNo;

        StockCollectionInitParams( const basar::Int16 branchNo ) 
	    : m_BranchNo( branchNo )
	    {
	    }

	    StockCollectionInitParams()
	    : m_BranchNo( 0 )
	    {
	    }
    };

} // end namespace stock
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_STOCK_STOCKCOLLECTIONINITPARAMS_H
