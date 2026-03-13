/*
 *  \brief  
 *  \author 
 *  \date   
*/

#ifndef GUARD_CSC_ORDER_DM_PTR_H
#define GUARD_CSC_ORDER_DM_PTR_H

#include <boost/shared_ptr.hpp>

namespace libcscbatch
{
namespace domMod 
{
    class CSCOrderDM;
    typedef boost::shared_ptr< CSCOrderDM > CSCOrderDMPtr;
	
} // end namespace domMod
}
#endif // end GUARD_CSC_ORDER_DM_PTR_H
