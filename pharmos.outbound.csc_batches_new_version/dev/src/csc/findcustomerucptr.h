//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  
 *  \author  
 *  \date    */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_CSC_BATCHES_USECASE_FINDCUSTOMERUCPTR_H
#define GUARD_CSC_BATCHES_USECASE_FINDCUSTOMERUCPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
    namespace useCase
    {
        class FindCustomerUC;
        typedef boost::shared_ptr< FindCustomerUC > FindCustomerUCPtr;
    }
}

#endif
