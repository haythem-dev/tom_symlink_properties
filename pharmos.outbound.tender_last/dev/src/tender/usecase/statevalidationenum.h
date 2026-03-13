#ifndef GUARD_TENDER_USECASE_STATE_VALIDATION_ENUM_H
#define GUARD_TENDER_USECASE_STATE_VALIDATION_ENUM_H

namespace tender
{
namespace useCase
{
 enum StateValidationEnum
{
    NONE = 0,
    ACTIVE_TENDER_ALREADY_EXIST,
    WRONG_TENDER_STATE_CHANGE,
    UNKNOWN
};

} // end namespace infrastructure
} // end namespace libtender

#endif //end GUARD_TENDER_USECASE_STATE_VALIDATION_ENUM_H
