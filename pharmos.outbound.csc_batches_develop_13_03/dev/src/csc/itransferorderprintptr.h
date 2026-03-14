//----------------------------------------------------------------------------//
/*! \file	
 *  \brief  
 *  \author 
 *  \date
 */
//----------------------------------------------------------------------------//

#ifndef GUARD_ITRANSFER_ORDER_PRINT_PTR_H
#define GUARD_ITRANSFER_ORDER_PRINT_PTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
namespace print
{
	class ITransferOrderPrint;
    typedef boost::shared_ptr< ITransferOrderPrint > ITransferOrderPrintPtr;
}
}
#endif // GUARD_ITRANSFER_ORDER_PRINT_PTR_H
