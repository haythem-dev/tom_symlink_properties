#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_IMPORT_STRUCT_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_IMPORT_STRUCT_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_i18nstring.h>
#include <libbasarcmnutil_date.h>
#include <libbasarcmnutil_decimal.h>

namespace libtender
{
namespace domMod
{
namespace tender
{
struct TenderImportStruct
{
	basar::Int32		id;
	basar::Int16		branchno;
	basar::I18nString	contractno;
	basar::Int32		tendertype;
	basar::Int32		customerno;
	basar::Int32		pharmacychainno;
	basar::I18nString	customergroupid;
	basar::Int32		partnerno;
	basar::Date			datefrom;
	basar::Date			dateto;
	basar::Int16		tenderstate;

	basar::VarString	articlecode;
	basar::Int32		contractqty;
	basar::Decimal		contractprice;
	basar::Decimal		adddiscountpct;
	basar::Int16		discountcalcfrom;
	basar::Int16		discountapplyto;

	// get's determined
	basar::Int32		articleno;	
};

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_TENDER_TENDER_IMPORT_STRUCT_H
