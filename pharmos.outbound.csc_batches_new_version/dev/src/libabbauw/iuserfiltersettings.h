#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IUSERFILTERSETTINGS_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IUSERFILTERSETTINGS_H

#include "iordertypecollectionparameter.h"
#include "sortorder.h"

//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class IUserFilterSettings : public IOrderTypeCollectionParameter
{
public:
    virtual ~IUserFilterSettings() {}

	virtual util::SortOrder                                     getOrderSort()               = 0;
	virtual basar::Date                                         getFilterStartDate()         = 0;
	virtual basar::Date                                         getFilterEndDate()           = 0;
};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif	// GUARD_LIBABBAUW_DOMMOD_PARAMETER_IUSERFILTERSETTINGS_H
