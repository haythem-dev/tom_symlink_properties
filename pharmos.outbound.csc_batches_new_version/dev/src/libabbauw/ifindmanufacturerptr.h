//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  
 *  \author Hendrik Polzin 
 *  \date 15.08.2013   */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_LIBABBAUW_DOMMOD_MANUFACTURERDM_IFINDMANUFACTURERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_MANUFACTURERDM_IFINDMANUFACTURERPTR_H

#include <boost/shared_ptr.hpp>
namespace libabbauw
{
namespace domMod 
{
    namespace manufacturerdm
    {
        class IFindManufacturer;
		typedef boost::shared_ptr< IFindManufacturer > IFindManufacturerPtr;
    }
}
}
#endif 
