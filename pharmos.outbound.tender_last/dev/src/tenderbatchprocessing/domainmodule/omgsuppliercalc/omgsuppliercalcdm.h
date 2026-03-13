#ifndef GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGSUPPLIERCALCDM_H
#define GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGSUPPLIERCALCDM_H

#include "iomgsuppliercalc.h"
#include <loggerpool/loggerpool.h>
#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace log4cplus 
{
	class Logger;
}

namespace tenderbatchprocessing 
{
namespace domMod 
{
namespace omgsuppliercalc 
{
class OMGSupplierCalcDM :public IOMGSupplierCalc
{

public:
    static const libutil::misc::ClassInfo & getClassInfo();

public:
    OMGSupplierCalcDM();
    virtual ~OMGSupplierCalcDM();

    virtual bool isEmpty() const;
    virtual void resetSearchYit();
    virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit() const;
    virtual basar::db::aspect::AccessorPropertyTableRef get() const;
    virtual void save( basar::db::aspect::AccessorPropertyTable_YIterator yitSave );
    virtual void markAsTransferred( basar::db::aspect::AccessorPropertyTable_YIterator yitStart, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd );

	virtual void findForTransfer();

    void injectOMGCalcAccessor( libutil::infrastructure::accessor::IAccessorPtr );

private:
    OMGSupplierCalcDM( const OMGSupplierCalcDM & );
    OMGSupplierCalcDM & operator = ( const OMGSupplierCalcDM & );

    libutil::domMod::SearchYIteratorPtr                         getSearchYIterator() const;
    libutil::infrastructure::accessor::IAccessorPtr             getOMGCalcAccessor() const;

    mutable libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;
    libutil::infrastructure::accessor::IAccessorPtr             m_Accessor;
    const log4cplus::Logger										m_Logger;
};

} // end namespace omgcalc
} // end namespace domMod
} // end namespace tenderbatchprocessing

#endif // GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGCALCDM_H