#ifndef GUARD_TENDERORDERCREATORCMDLINE_DEFINITIONS_H
#define GUARD_TENDERORDERCREATORCMDLINE_DEFINITIONS_H

#include <libbasar_definitions.h>

namespace tenderordercreator
{
    namespace infrastructure
    {
        basar::ConstString OPTION_COUNTRY		= "-country";
        basar::ConstString OPTION_DATE_FROM		= "-datefrom";
        basar::ConstString OPTION_DATE_TO		= "-dateto";
        basar::ConstString OPTION_NEXT_DAYS		= "-nextdays";

		basar::ConstString OPTION_COUNTRY_PFR	= "PFR";
        basar::ConstString OPTION_COUNTRY_BG	= "BG";
        basar::ConstString OPTION_COUNTRY_RS	= "RS";
    }
}


#endif
