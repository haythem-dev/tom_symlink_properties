#ifndef GUARD_TENDER_USECASE_I_ERROR_HANDLER_PTR_H
#define GUARD_TENDER_USECASE_I_ERROR_HANDLER_PTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
namespace useCase
{
class IErrorHandler;
typedef boost::shared_ptr<tender::useCase::IErrorHandler> IErrorHandlerPtr;

} // namespace useCase
} // namespace tender

#endif // GUARD_TENDER_USECASE_I_ERROR_HANDLER_PTR_H
