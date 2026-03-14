//----------------------------------------------------------------------------//
/*! \file	transferorderptr.h
 *  \brief  managed by boost shared pointer
 *  \author Jia Li
 *  \date   02.01.2013
 */
//----------------------------------------------------------------------------//

#ifndef GUARD_DOMMOD_TRANSFERORDERDM_TRANSFERORDER_PTR_H
#define GUARD_DOMMOD_TRANSFERORDERDM_TRANSFERORDER_PTR_H

#include <boost/shared_ptr.hpp>

namespace domMod
{
	namespace transferorderdm
	{
		class TransferOrder;
	    typedef boost::shared_ptr< TransferOrder > TransferOrderPtr;
    }
}

#endif //GUARD_DOMMOD_TRANSFERORDERDM_TRANSFERORDER_PTR_H
