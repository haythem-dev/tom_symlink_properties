#ifndef GUARD_TENDER_USECASE_I_ERROR_HANDLER_H
#define GUARD_TENDER_USECASE_I_ERROR_HANDLER_H

namespace tender
{
namespace useCase
{
class IErrorHandler
{
public:
	virtual ~IErrorHandler() {}

	virtual void handleInfo( const basar::I18nString & info ) = 0;
	virtual void handleWarning( const basar::I18nString & warning ) = 0;
	virtual void handleError( const basar::I18nString & error ) = 0;
};

} // namespace useCase
} // namespace tender

#endif // GUARD_TENDER_USECASE_I_ERROR_HANDLER_H
