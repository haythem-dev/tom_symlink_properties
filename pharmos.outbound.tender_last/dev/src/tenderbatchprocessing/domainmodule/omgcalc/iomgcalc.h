#ifndef GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGCALC_H
#define GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGCALC_H

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
namespace omgcalc 
{

//-----------------------------------------------------------------------------------------

class IOMGCalc
{

public:
    virtual ~IOMGCalc() {}

    virtual bool isEmpty() const = 0;
    virtual void resetSearchYit() = 0;
    virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit() const        = 0;
    virtual basar::db::aspect::AccessorPropertyTableRef        get()          const        = 0;

    virtual void                                               save()                      = 0;
    virtual void findByID( const basar::db::aspect::AccessorPropertyTable_YIterator )      = 0;
    virtual void findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

};

//-----------------------------------------------------------------------------------------

} // end namespace omgcalc
} // end namespace domMod
} // end namespace tenderbatchprocessing

//-----------------------------------------------------------------------------------------
#endif // GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGCALC_H