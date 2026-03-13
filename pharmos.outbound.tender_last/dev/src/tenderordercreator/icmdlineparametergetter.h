#ifndef GUARD_ICMDLINEPARAMETERGETTER_H
#define GUARD_ICMDLINEPARAMETERGETTER_H

#include <libbasar_definitions.h>

namespace tenderordercreator {
namespace infrastructure {

class ICmdLineParameterGetter
{
public:
	virtual ~ICmdLineParameterGetter() {};

	virtual basar::VarString	getCountry() const = 0;
	virtual basar::Date			getDateFrom() const = 0;
	virtual basar::Date			getDateTo() const = 0;
	virtual basar::Int16		getNextDays() const = 0;
};

} // namespace infrastructure
} // namespace tenderordercreator

#endif // GUARD_ICMDLINEPARAMETERGETTER_H
