#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_STATE_ENUM_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_STATE_ENUM_H

namespace libtender
{
namespace domMod
{
namespace tender
{
enum TenderStateEnum
{
	TS_UNCOMPLETED = 0,
	TS_OK = 1,
	TS_MANUALLY_CLOSED = 8,
	TS_AUTOMATICALLY_CLOSED = 9
};

}
}
}

#endif
