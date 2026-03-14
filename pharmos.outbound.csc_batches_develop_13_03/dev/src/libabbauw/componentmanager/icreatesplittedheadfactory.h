#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ICREATESPLITTEDHEADFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ICREATESPLITTEDHEADFACTORY_H

#include "itransferorderptr.h"

namespace libabbauw
{
namespace componentManager
{
class ICreateSplittedHeadFactory
{
public:
    virtual ~ICreateSplittedHeadFactory() {}

    virtual domMod::transferOrder::ITransferOrderPtr createSplittedHead( domMod::transferOrder::ITransferOrderPtr ) = 0;
};

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ICREATESPLITTEDHEADFACTORY_H
