#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSITION_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSITION_H

#include <libbasar_definitions.h>

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
		namespace transferOrder
		{

			class ITransferOrderPosition
			{
			public:
				virtual ~ITransferOrderPosition() {}

				virtual basar::db::aspect::AccessorPropertyTable_YIterator          get()                    const = 0;
				virtual bool                                                        isEmpty()                const = 0;
				virtual bool                                                        hasOrderProposalSet()    const = 0;
				virtual bool                                                        isOrderProposalAllowed() const = 0;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSITION_H
