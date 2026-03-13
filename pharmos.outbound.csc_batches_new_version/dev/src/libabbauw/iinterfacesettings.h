#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IINTERFACESETTINGS_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IINTERFACESETTINGS_H


namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class IInterfaceSettings 
{
public:
    virtual ~IInterfaceSettings() {}

	virtual basar::VarString                                     getCSVExportSeparator() = 0;

};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif	// GUARD_LIBABBAUW_DOMMOD_PARAMETER_IINTERFACESETTINGS_H
