//----------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author
 *  \date
 */
 //----------------------------------------------------------------------------//

#ifndef GUARD_LIBABBAUW_DMCOMPONENTMANAGERPTR_H
#define GUARD_LIBABBAUW_DMCOMPONENTMANAGERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class DMComponentManager;
		typedef boost::shared_ptr< DMComponentManager > DMComponentManagerPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DMCOMPONENTMANAGERPTR_H
