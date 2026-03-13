#ifndef GUARD_TENDERBATCHPROCESSING_USECASE_OMGTRANSFERUC_H
#define GUARD_TENDERBATCHPROCESSING_USECASE_OMGTRANSFERUC_H

#include <domainmodule/omgtransfer/iomgtransferptr.h>
#include <domainmodule/omgsuppliercalc/iomgsuppliercalcptr.h>
#include <libutil/irunnable.h>
#include <libutil/session.h>
#include <libutil/dbtransaction.h>
#include <libbasarappl_eventmacros.h>

namespace tenderbatchprocessing
{
namespace useCase
{
class OMGTransferUC : public libutil::useCase::IRunnable
{
public:
	OMGTransferUC();
	virtual ~OMGTransferUC();

	void injectOMGSupplierCalcDM( tenderbatchprocessing::domMod::omgsuppliercalc::IOMGSupplierCalcPtr omgCalcDM );
	void injectOMGTransferDM( tenderbatchprocessing::domMod::omgtransfer::IOMGTransferPtr omgTransfer );

	virtual void run();

private:
	OMGTransferUC( const OMGTransferUC & );
	OMGTransferUC & operator=( const OMGTransferUC & );

	const log4cplus::Logger getLogger() const;

	tenderbatchprocessing::domMod::omgsuppliercalc::IOMGSupplierCalcPtr		m_OMGCalcDM;
	tenderbatchprocessing::domMod::omgtransfer::IOMGTransferPtr				m_OMGTransferDM;
	const log4cplus::Logger 												m_Logger;
};

}
}//end tenderbatchprocessing

#endif // GUARD_TENDERBATCHPROCESSING_USECASE_OMGTRANSFERUC_H
