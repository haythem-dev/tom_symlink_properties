//--------------------------------------------------------------------------------------------------//
/*! \brief
 *  \author     Bjoern Bischof
 *  \date       22.12.2010
 *  \version    00.00.00.01
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_TENDERORDERCREATOR_INFRASTRUCTURE_TENDERORDERCREATORCMDLINEEVALUATOR_H
#define GUARD_TENDERORDERCREATOR_INFRASTRUCTURE_TENDERORDERCREATORCMDLINEEVALUATOR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "tenderordercreatorcmelinearguments.h"

#include <libutil/cmdline.h>
//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace tenderordercreator
{
    namespace infrastructure
    {
        namespace cmdLine
        {
            class TenderordercreatorCmdLineEvaluator : public libutil::infrastructure::cmdLine::CmdLineEvaluatorBase< AppArgs, libutil::infrastructure::cmdLine::NullCmdLineOptionCollection >
            {
                public:
                    TenderordercreatorCmdLineEvaluator();

                private:
                    virtual bool doEvaluate();

                    TenderordercreatorCmdLineEvaluator( TenderordercreatorCmdLineEvaluator& );
                    TenderordercreatorCmdLineEvaluator& operator=( TenderordercreatorCmdLineEvaluator& );
            };
        } 
    }
} 

#endif 
