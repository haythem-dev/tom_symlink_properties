#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_ISTOCKPARAMETER_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_ISTOCKPARAMETER_H

namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class IStockParameter
{
public:	
    virtual ~IStockParameter() {}

    virtual basar::VarString                                      getTransitStorageLocationSlot()          = 0;
};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_PARAMETER_ISTOCKPARAMETER_H
