#ifndef GUARD_DOMMOD_TENDERBATCHPROCESSING_I_OMGTRANSFER_H
#define GUARD_DOMMOD_TENDERBATCHPROCESSING_I_OMGTRANSFER_H

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
namespace omgtransfer
{

class IOMGTransfer
{

public:
    virtual ~IOMGTransfer() {}

    virtual void addPositions( basar::db::aspect::AccessorPropertyTable_YIterator yitStart, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd ) = 0;
    virtual void save() = 0;
	virtual bool hasError() const = 0;
	virtual basar::Int32 getPositionsTransferred() const = 0;
	virtual basar::Int32 getMaxPositionsCount() const = 0;
};

} // end namespace omgcalc
} // end namespace domMod
} // end namespace tenderbatchprocessing

#endif // GUARD_DOMMOD_TENDERBATCHPROCESSING_I_OMGTRANSFER_H