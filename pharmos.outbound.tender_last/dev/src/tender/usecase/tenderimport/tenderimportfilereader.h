#ifndef GUARD_TENDER_USECASE_IMPORT_FILE_READER_H
#define GUARD_TENDER_USECASE_IMPORT_FILE_READER_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_i18nstring.h>
#include <vector>
#include <libtender/domainmodule/tender/tenderimportstruct.h>
#include <libtender/domainmodule/scheduleddelivery/scheduleddeliveryimportstruct.h>
#include "iprogressptr.h"
#include "ierrorhandlerptr.h"

namespace tender
{
namespace useCase
{
class TenderImportFileReader
{
public:
	struct TenderLineValue
	{
		basar::Int32										lineNumber;
		basar::I18nString									text;
		libtender::domMod::tender::TenderImportStruct		positionData;
	};

	struct ScheduledDeliveryLineValue
	{
		basar::Int32															lineNumber;
		basar::I18nString														text;
		libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct		positionData;
	};

	TenderImportFileReader();
	virtual ~TenderImportFileReader();

	void inject( IProgressPtr progress );
	void inject( IErrorHandlerPtr errorHandler );

	void readTenderPositions( const basar::VarString & filename );
	basar::Int32 getTenderLineCount() const;
	libtender::domMod::tender::TenderImportStruct & getTenderPosition( const basar::Int32 index );
	TenderLineValue & getTenderLineValue( const basar::Int32 index );

	void readScheduledDeliveryPositions( const basar::VarString & filename );
	basar::Int32 getScheduledDeliveryLineCount() const;
	libtender::domMod::scheduledDelivery::ScheduledDeliveryImportStruct & getScheduledDeliveryPosition( const basar::Int32 index );
	ScheduledDeliveryLineValue & getScheduledDeliveryLineValue( const basar::Int32 index );

private:
	TenderImportFileReader( const TenderImportFileReader & );
	TenderImportFileReader operator=( const TenderImportFileReader & );

	typedef std::vector<TenderLineValue> TenderLineValueCollection;
	typedef std::vector<ScheduledDeliveryLineValue> ScheduledDeliveryLineValueCollection;

	TenderLineValueCollection				m_TenderLineValueCollection;
	ScheduledDeliveryLineValueCollection	m_ScheduledDeliveryLineValueCollection;
	IProgressPtr							m_Progress;
	IErrorHandlerPtr						m_ErrorHandler;
	basar::Int16							m_Area;
};

} // namespace useCase
} // namespace tender

#endif // GUARD_TENDER_USECASE_IMPORT_FILE_READER_H
