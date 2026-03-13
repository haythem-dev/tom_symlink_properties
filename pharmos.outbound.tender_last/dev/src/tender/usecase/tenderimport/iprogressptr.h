#ifndef GUARD_TENDER_USECASE_I_PROGRESS_PTR_H
#define GUARD_TENDER_USECASE_I_PROGRESS_PTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
namespace useCase
{
class IProgress;
typedef boost::shared_ptr<tender::useCase::IProgress> IProgressPtr;

} // namespace useCase
} // namespace tender

#endif // GUARD_TENDER_USECASE_I_PROGRESS_PTR_H
