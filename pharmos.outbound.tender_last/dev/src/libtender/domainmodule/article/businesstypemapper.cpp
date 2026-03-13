#include "businesstypemapper.h"

namespace libtender {
namespace domMod {
namespace article {

BusinessTypeEnum BusinessTypeMapper::getBusinessType( const tender::TenderTypeEnum tenderType )
{
	BusinessTypeEnum ret = BT_NONE;

	switch( tenderType )
	{
		case tender::TT_PREWHOLESALE: ret = BT_PREWHOLESALE; break;
		case tender::TT_PLATFORM: ret = BT_PLATFORM; break;
		case tender::TT_SRA: ret = BT_SRA; break;
		case tender::TT_CAP: ret = BT_CAP; break;
		default:
			break;
	}

	return ret;
}

} // end namespace article
} // end namespace domMod
} // end namespace libtender
