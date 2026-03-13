#ifndef GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGSUPPLIERCALC_H
#define GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGSUPPLIERCALC_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>
#include <libutil/accessor.h>

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

namespace tenderbatchprocessing 
{
namespace domMod 
{
namespace omgsuppliercalc 
{
class IOMGSupplierCalc
{
public:
    virtual ~IOMGSupplierCalc() {}

    virtual bool isEmpty() const = 0;
    virtual void resetSearchYit() = 0;
    virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit() const = 0;
    virtual basar::db::aspect::AccessorPropertyTableRef get() const = 0;
    virtual void save( basar::db::aspect::AccessorPropertyTable_YIterator yitSave ) = 0;
    virtual void markAsTransferred( basar::db::aspect::AccessorPropertyTable_YIterator yitStart, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd ) = 0;
	virtual void findForTransfer() = 0;
};

} // end namespace omgsuppliercalc
} // end namespace domMod
} // end namespace tenderbatchprocessing

#endif // GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGSUPPLIERCALC_H
