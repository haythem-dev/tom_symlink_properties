#ifndef GUARD_LIBTENDER_COUNTRY_CODE_MAPPER_H
#define GUARD_LIBTENDER_COUNTRY_CODE_MAPPER_H

#include "countryidenum.h"
#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>

namespace libtender
{
namespace infrastructure
{
class CountryCodeMapper
{
public:
	static CountryIDEnum getCountryID( const basar::Int16 areaID );

	static CountryIDEnum getCountryID( const basar::VarString countryStr ); // for batch only!

	static basar::Int16 getRegionID( const CountryIDEnum country ); // for batch only!!!
};

} // end namespace infrastructure
} // end namnespace libtender

#endif //end GUARD_LIBTENDER_COUNTRY_CODE_MAPPER_H
