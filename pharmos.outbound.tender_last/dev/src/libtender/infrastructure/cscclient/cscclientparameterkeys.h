#ifndef GUARD_CSCCLIENTPARAMETERKEYS_H
#define GUARD_CSCCLIENTPARAMETERKEYS_H

#include <libbasar_definitions.h>

namespace libtender {
namespace constants {

	basar::ConstString UADM_CSC_SERVER_KEY		= "CSC_SERVER";
	basar::ConstString UADM_CSC_PORT_KEY		= "CSC_PORT";
	basar::ConstString UADM_CSC_DEVICENAME_KEY	= "CSC_DEVICENAME";
	basar::ConstString UADM_CSC_UID_KEY			= "CSC_UID";
	
    basar::ConstString UADM_CSC_ORDER_TYPE         = "CSC_ORDER_TYPE";
    basar::ConstString UADM_CSC_ORDER_PICKING_TYPE = "CSC_ORDER_PICKING_TYPE";
    basar::ConstString UADM_CSC_ORDER_BOOKING_TYPE = "CSC_ORDER_BOOKING_TYPE";

}
}

#endif // GUARD_CSCCLIENTPARAMETERKEYS_H
