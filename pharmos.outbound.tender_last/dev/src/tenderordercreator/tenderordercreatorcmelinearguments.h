#ifndef GUARD_TENDERORDERCREATORCMELINEARGUMENTS_H
#define GUARD_TENDERORDERCREATORCMELINEARGUMENTS_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>
#include <libbasarcmnutil_date.h>

namespace tenderordercreator {
namespace infrastructure {
namespace cmdLine {

class AppArgs
{
public:
	AppArgs() :
		m_Country(),
		m_DateFrom(),
		m_DateTo(),
		m_NextDays(0)
	{
		m_DateFrom.setInvalid();
		m_DateTo.setInvalid();
	}

	basar::VarString	m_Country;
	basar::Date			m_DateFrom;
	basar::Date			m_DateTo;
	basar::Int16		m_NextDays;

};

} // namespace cmdLine
} // namespace infrastructure
} // namespace tenderordercreator

#endif
