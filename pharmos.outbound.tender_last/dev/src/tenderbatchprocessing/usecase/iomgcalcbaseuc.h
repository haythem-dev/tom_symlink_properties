#ifndef GUARD_TENDERBATCHPROCESSING_USECASE_IOMGCALCBSAEUC_H
#define GUARD_TENDERBATCHPROCESSING_USECASE_IOMGCALCBSAEUC_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------/
#include <libutil/irunnable.h>
#include <domainmodule/tender/itenderptr.h> 


namespace tenderbatchprocessing
{
namespace useCase
{
//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
    class IOMGCalcBaseUC : public libutil::useCase::IRunnable
    {
        public:
            virtual ~IOMGCalcBaseUC() {}
            virtual void setTender( libtender::domMod::tender::ITenderPtr ) = 0;
            virtual void run()  =0;
    };

} //end useCase
}//end tenderbatchprocessing
#endif //end GUARD_TENDERBATCHPROCESSING_USECASE_IOMGCALCBSAEUC_H