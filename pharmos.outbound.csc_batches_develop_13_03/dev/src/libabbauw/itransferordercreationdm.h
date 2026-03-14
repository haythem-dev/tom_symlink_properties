#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERCREATIONDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERCREATIONDM_H

#include <libbasar_definitions.h>

#include "itransferorderptr.h"
#include "itransferorderinpreparationdmptr.h"

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

class ITransferOrderCreationDM
{
public:
	virtual ~ITransferOrderCreationDM() {}
	
    virtual transferOrder::ITransferOrderPtr create( transferOrder::ITransferOrderInPreparationDMPtr ) = 0;
};

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERCREATIONDM_H
