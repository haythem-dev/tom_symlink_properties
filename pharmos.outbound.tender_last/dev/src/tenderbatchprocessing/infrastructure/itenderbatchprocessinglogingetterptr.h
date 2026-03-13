//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief
 *  \author Bischof Bjoern
 *  \date   12.06.2015 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_SETUP_IBATCHLOGINGETTERCONSTPTR_H
#define GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_SETUP_IBATCHLOGINGETTERCONSTPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderbatchprocessing {
namespace infrastructure {
namespace setup {

class IBatchLoginGetter;
typedef boost::shared_ptr< IBatchLoginGetter const> IBatchLoginGetterConstPtr;

} // namespace setup
} // namespace infrastructure
} // namespace tenderbatchprocessing

#endif//  GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_SETUP_IBATCHLOGINGETTERCONSTPTR_H
