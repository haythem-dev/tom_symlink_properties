#ifndef GUARD_CSCBATCHES_USECASE_I_TENDERCREATOR_H
#define GUARD_CSCBATCHES_USECASE_I_TENDERCREATOR_H

#include <libbasarappl_systemeventmanager.h>
#include "itransferorderptr.h"
#include <domainmodule/tender/itendercollectiondmptr.h>
#include <domainmodule/pharmacygroup/ipharmacygroupcollectiondmptr.h>
#include <list>

namespace CSC_Batches
{
namespace useCase
{
class ITenderCreator
{
public:
	typedef std::list<basar::VarString> ErrorList;

    virtual ~ITenderCreator() {}
    virtual basar::appl::EventReturnType createTender( libabbauw::domMod::transferOrder::ITransferOrderPtr ) = 0;
	virtual bool hasErrors() const = 0;
	virtual const ErrorList & getErrors() const = 0;
};

} // namespace useCase
} // namespace CSC_Batches

#endif // GUARD_CSCBATCHES_USECASE_I_TENDERCREATOR_H
