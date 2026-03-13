#ifndef GUARD_LIBABBAUW_DOMMOD_CSCORDER_CSCORDERINITPARAMS_H
#define GUARD_LIBABBAUW_DOMMOD_CSCORDER_CSCORDERINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace cscorder
{

struct CSCOrderInitParams
{
    basar::Int16          m_BranchNo;

    CSCOrderInitParams( const basar::Int16 branchNo ) 
    : m_BranchNo( branchNo )
    {
    }

    CSCOrderInitParams() 
    : m_BranchNo( 0 )
    {
    }
};

} // end namespace cscorder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_CSCORDER_CSCORDERINITPARAMS_H
