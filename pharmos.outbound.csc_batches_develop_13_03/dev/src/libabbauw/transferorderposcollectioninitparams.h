#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSCOLLECTIONINITPARAMS_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSCOLLECTIONINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

struct TransferOrderPosInitParams
{
    basar::Int16 m_BranchNo;

    TransferOrderPosInitParams( const basar::Int16 branchNo ) 
    : m_BranchNo( branchNo )
    {
    }

    TransferOrderPosInitParams() 
    : m_BranchNo( 0 )
    {
    }
};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSCOLLECTIONINITPARAMS_H
