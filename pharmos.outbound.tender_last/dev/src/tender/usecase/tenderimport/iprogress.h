#ifndef GUARD_TENDER_USECASE_I_PROGRESS_H
#define GUARD_TENDER_USECASE_I_PROGRESS_H

namespace tender
{
namespace useCase
{
class IProgress
{
public:
	virtual ~IProgress() {}

	virtual void progress( const basar::Int32 percent ) = 0;
};

} // namespace useCase
} // namespace tender

#endif // GUARD_TENDER_USECASE_I_PROGRESS_H
