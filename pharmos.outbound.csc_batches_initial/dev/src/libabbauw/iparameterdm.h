#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETERDM_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETERDM_H

#include "iparameteredit.h"

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libabbauw
{
	namespace domMod
	{
		namespace parameter
		{
			class IParameterDM : public IParameterEdit
			{
			public:
				virtual ~IParameterDM() {}

				virtual void                                               load() = 0;
				virtual void                                               save() = 0;
				virtual bool                                               setInvalidParametersToDefault() = 0;
			};

		} // end namespace parameter
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETERDM_H
