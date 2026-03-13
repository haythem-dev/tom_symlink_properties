#ifndef GUARD_LIBTENDER_DOMMOD_ARTICLE_BUSINESS_TYPE_ENUM_H
#define GUARD_LIBTENDER_DOMMOD_ARTICLE_BUSINESS_TYPE_ENUM_H

namespace libtender {
namespace domMod {
namespace article {

enum BusinessTypeEnum
{
	BT_NONE = -1,
	BT_WHOLESALE = 0,
	BT_PREWHOLESALE = 1,
	BT_PLATFORM = 2,
	BT_SRA = 3,
	BT_CAP = 4

//0 - Wholesale
//1 - Prewholesale
//2 - Platform
//3 - SRA
//4 - CAP

};

} // end namespace article
} // end namespace domMod
} // end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_ARTICLE_BUSINESS_TYPE_ENUM_H
