//----------------------------------------------------------------------------
/*! \file
 *  \brief
 *  \author Bischof Bjoern
 *  \date   12.07.2015
 */
//----------------------------------------------------------------------------
#ifndef GUARD_TENDERORDERCREATOR_INFRASTRUCTURE_SETUP_IBATCHLOGINGETTER_H
#define GUARD_TENDERORDERCREATOR_INFRASTRUCTURE_SETUP_IBATCHLOGINGETTER_H

//------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------//
#include <libutil/login.h>

//------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------//
namespace tenderordercreator {
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
} // namespace tenderordercreator

#endif // GUARD_TENDERORDERCREATOR_INFRASTRUCTURE_SETUP_IBATCHLOGINGETTER_H
