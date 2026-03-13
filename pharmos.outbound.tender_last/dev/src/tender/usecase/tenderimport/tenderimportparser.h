#ifndef GUARD_TENDER_USECASE_TENDER_IMPORT_PARSER_H
#define GUARD_TENDER_USECASE_TENDER_IMPORT_PARSER_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_i18nstring.h>
#include <libtender/domainmodule/tender/tenderimportstruct.h>
#include <libtender/domainmodule/scheduledDelivery/scheduleddeliveryimportstruct.h>

namespace tender
{
namespace useCase
{
class TenderImportParser
{
public:
	static bool parse( const basar::I18nString & line, libtender::domMod::tender::TenderImportStruct & importStruct );
	static bool parse( const basar::I18nString & line, libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct & importStruct );
};

} // namespace useCase
} // namespace tender

#endif // GUARD_TENDER_USECASE_TENDER_IMPORT_PARSER_H
