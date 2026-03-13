//----------------------------------------------------------------------------
/*! \file
 *  \brief
 *  \author Bischof Bjoern
 *  \date   12.07.2015
 */
//----------------------------------------------------------------------------
#ifndef GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_SETUP_IBATCHLOGINGETTER_H
#define GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_SETUP_IBATCHLOGINGETTER_H

//------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------//
#include <libutil/login.h>

//------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------//
namespace tenderbatchprocessing {
namespace infrastructure {
namespace setup {

//------------------------------------------------------------------------------//
// class 
//------------------------------------------------------------------------------//
class IBatchLoginGetter
{
public:
	virtual ~IBatchLoginGetter(){};
	virtual libutil::infrastructure::login::ILoginPtr getNonInteractiveLogin( const libutil::infrastructure::login::UserLoginData& ) const = 0;
};

} // namespace setup
} // namespace infrastructure
} // namespace tenderbatchprocessing

#endif // GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_SETUP_IBATCHLOGINGETTER_H
