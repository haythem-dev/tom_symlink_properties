#ifndef GUARD_LIBTENDER_DOMMOD_ARTICLE_BUSINESS_TYPE_MAPPER_H
#define GUARD_LIBTENDER_DOMMOD_ARTICLE_BUSINESS_TYPE_MAPPER_H

#include <domainmodule/article/businesstypeenum.h>
#include <domainmodule/tender/tendertypeenum.h>
#include <libbasar_definitions.h>

namespace libtender {
namespace domMod {
namespace article {

class BusinessTypeMapper
{
public:
	static BusinessTypeEnum getBusinessType( const tender::TenderTypeEnum tenderType );
};

} // end namespace article
} // end namespace domMod
} // end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_ARTICLE_BUSINESS_TYPE_MAPPER_H
