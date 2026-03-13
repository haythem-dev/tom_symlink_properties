#ifndef GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGCALCDM_H
#define GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGCALCDM_H

//-----------------------------------------------------------------------------------------

#include "iomgcalc.h"
#include <loggerpool/loggerpool.h>
#include <libutil/searchyiterator.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>
//-----------------------------------------------------------------------------------------

namespace log4cplus {
    class Logger;
}

//-----------------------------------------------------------------------------------------

namespace tenderbatchprocessing {
namespace domMod {
namespace omgcalc {

//-----------------------------------------------------------------------------------------

class OMGCalcDM :public IOMGCalc
{

public:
    static const libutil::misc::ClassInfo & getClassInfo();

public:
    OMGCalcDM( );
    //OMGCalcDM( const basar::db::aspect::AccessorPropertyTable_YIterator );
    ~OMGCalcDM();

    virtual bool                                                isEmpty() const;
    virtual void                                                resetSearchYit();
    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const;
    virtual basar::db::aspect::AccessorPropertyTableRef         get()const;
    virtual void                                                save();
    virtual void                                                findByID( const basar::db::aspect::AccessorPropertyTable_YIterator )      ;
    virtual void                                                findByPattern( const basar::db::aspect::AccessorPropertyTable_YIterator ) ;

    void                                                        injectOMGCalcAccessor( libutil::infrastructure::accessor::IAccessorPtr );

private:
    OMGCalcDM( const OMGCalcDM & );
    OMGCalcDM & operator = ( const OMGCalcDM & );


    libutil::domMod::SearchYIteratorPtr                         getSearchYIterator() const;
    libutil::infrastructure::accessor::IAccessorPtr             getOMGCalcAccessor() const;

    mutable libutil::domMod::SearchYIteratorPtr                 m_SearchYIterator;
    libutil::infrastructure::accessor::IAccessorPtr             m_Accessor;
    //basar::db::aspect::AccessorPropertyTable_YIterator          m_Yit;
    const log4cplus::Logger                                     m_Logger;
};

//-----------------------------------------------------------------------------------------

} // end namespace omgcalc
} // end namespace domMod
} // end namespace tenderbatchprocessing

//-----------------------------------------------------------------------------------------
#endif // GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGCALCDM_H