#ifndef GUARD_DOMMOD_CSCORDER_CSCCLIENTHELPER_H
#define GUARD_DOMMOD_CSCORDER_CSCCLIENTHELPER_H

//-----------------------------------------------------------------------------------------

#include <libbasarcmnutil_bstring.h>

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace cscorder {

//-----------------------------------------------------------------------------------------

class CscClientHelper
{
public:
	static basar::VarString getStringFromStruct(const char* p, const basar::Int32 len);

};

//-----------------------------------------------------------------------------------------

} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

//-----------------------------------------------------------------------------------------

#endif // GUARD_DOMMOD_CSCORDER_CSCCLIENTHELPER_H

//-----------------------------------------------------------------------------------------
