//--------------------------------------------------------------------------------------------------//
/*! \brief
 *  \author     Bjoern Bischof
 *  \date       22.12.2010
 *  \version    00.00.00.01
 */
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "tenderordercreatorcmdlineevaluator.h"
#include "tenderordercreatorcmdline_definitions.h"

#include "loggerpool/loggerpool.h"

#include "libbasarcmnutil_logging.h"
//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//
namespace tenderordercreator {
namespace infrastructure {
namespace cmdLine {

//--------------------------------------------------------------------------------------------------//

TenderordercreatorCmdLineEvaluator::TenderordercreatorCmdLineEvaluator()
{
}

//--------------------------------------------------------------------------------------------------//

bool TenderordercreatorCmdLineEvaluator::doEvaluate()
{
	AppArgs& appArgs = getAppArgs();

	getCmdLineParser()->getMandatoryVarString( OPTION_COUNTRY, appArgs.m_Country );
	appArgs.m_Country.upper();

	getCmdLineParser()->getDate(OPTION_DATE_FROM,	appArgs.m_DateFrom);
	getCmdLineParser()->getDate(OPTION_DATE_TO,		appArgs.m_DateTo);
	getCmdLineParser()->getInt16(OPTION_NEXT_DAYS,	appArgs.m_NextDays);

	return (appArgs.m_Country == OPTION_COUNTRY_PFR ||
		appArgs.m_Country == OPTION_COUNTRY_BG ||
		appArgs.m_Country == OPTION_COUNTRY_RS);
}

//--------------------------------------------------------------------------------------------------//

} // end namespace cmdLine
} // end namespace infrastructure
} // end namespace tenderordercreator

//--------------------------------------------------------------------------------------------------//
