#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETER_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETER_H

#include "iuserfiltersettings.h"
#include "iorderparameter.h"
#include "iordercreationparameter.h"
#include "istockparameter.h"
#include "iordercheckparameter.h"
#include "iorderreleaseparameter.h"
#include "ireserveqtyfactorparameter.h"
#include "iinterfacesettings.h"

//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace libabbauw
{
	namespace domMod
	{
		namespace parameter
		{
			class IParameter : public IUserFilterSettings, public IOrderParameter, public IOrderCreationParameter, public IStockParameter,
				public IOrderCheckParameter, public IOrderReleaseParameter, public IReserveQtyFactorParameter, public IInterfaceSettings
			{
			public:
				virtual ~IParameter() {}
			};

		} // end namespace parameter
	} // end namespace domMod
} // end namespace libabbauw

#endif	// end GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETER_H
