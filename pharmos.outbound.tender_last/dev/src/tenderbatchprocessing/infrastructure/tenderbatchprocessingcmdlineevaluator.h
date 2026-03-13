//--------------------------------------------------------------------------------------------------//
/*! \brief
 *  \author     Bjoern Bischof
 *  \date       22.12.2010
 *  \version    00.00.00.01
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_TENDERBATCHPROCESSINGCMDLINEEVALUATOR_H
#define GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_TENDERBATCHPROCESSINGCMDLINEEVALUATOR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "tenderbatchprocessingcmelinearguments.h"

#include <libutil/cmdline.h>
//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace tenderbatchprocessing
{
    namespace infrastructure
    {
        namespace cmdLine
        {
            class TenderbatchprocessingCmdLineEvaluator : public libutil::infrastructure::cmdLine::CmdLineEvaluatorBase< AppArgs, libutil::infrastructure::cmdLine::NullCmdLineOptionCollection >
            {
                public:
                    TenderbatchprocessingCmdLineEvaluator();

                private:
                    virtual bool doEvaluate();

                    TenderbatchprocessingCmdLineEvaluator( TenderbatchprocessingCmdLineEvaluator& );
                    TenderbatchprocessingCmdLineEvaluator& operator=( TenderbatchprocessingCmdLineEvaluator& );
            };
        } 
    }
} 

#endif 
