#include "cscclienthelper.h"

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace domMod {
namespace cscorder {

//-----------------------------------------------------------------------------------------

basar::VarString CscClientHelper::getStringFromStruct(const char* p, const basar::Int32 len)
{
	basar::VarString s = basar::VarString(p).substr(0,len);
	s.trim(basar::cmnutil::BString::FIRST_END);
	return s;
}

//-----------------------------------------------------------------------------------------

} // end namespace cscorder
} // end namespace domMod
} // end namespace libtender

//-----------------------------------------------------------------------------------------
