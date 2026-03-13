#ifndef GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGTRANSFER_DM_H
#define GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGTRANSFER_DM_H

#include "iomgtransfer.h"
#include <loggerpool/loggerpool.h>
#include <libutil/accessor.h>
#include <libutil/classinfo.h>
#include <libutil/util.h>
#include <libutil/domainmodule.h>
#include <ostream>

namespace tenderbatchprocessing
{
namespace domMod
{
namespace omgtransfer
{
class OMGTransferDM :public IOMGTransfer
{
public:
    static const libutil::misc::ClassInfo & getClassInfo();

public:
    OMGTransferDM();
    virtual ~OMGTransferDM();

    virtual void addPositions( basar::db::aspect::AccessorPropertyTable_YIterator yitStart, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd );
    virtual void save();
	virtual bool hasError() const;
	virtual basar::Int32 getPositionsTransferred() const;
	virtual basar::Int32 getMaxPositionsCount() const;

    void injectRemoteProcedureAccessor( libutil::infrastructure::accessor::IAccessorPtr );

private:
    OMGTransferDM( const OMGTransferDM & );
    OMGTransferDM & operator = ( const OMGTransferDM & );

    libutil::infrastructure::accessor::IAccessorPtr getOMGTransferAccessor() const;
	void headToStream( std::ostream & str, basar::db::aspect::AccessorPropertyTable_YIterator yit, const basar::Int32 posCounter ) const;
	void posToStream( std::ostream & str, basar::db::aspect::AccessorPropertyTable_YIterator yit ) const;

    libutil::infrastructure::accessor::IAccessorPtr m_Accessor;
    const log4cplus::Logger 						m_Logger;
	basar::Int32									m_ReturnCode;
	basar::Int32									m_PositionsProcessed;
};

} // end namespace omgcalc
} // end namespace domMod
} // end namespace tenderbatchprocessing

#endif // GUARD_DOMMOD_TENDERBATCHPROCESSING_OMGTRANSFER_DM_H