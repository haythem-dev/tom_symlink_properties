//--------------------------------------------------------------------------------------------------//
/*! \brief 
 *  \author     
 *  \date       
 *  \version    
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_COMMAND_LINE_ABBUW_CMDLINE_EVALUATOR_H
#define GUARD_ABBAUW_COMMAND_LINE_ABBUW_CMDLINE_EVALUATOR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <cmdlineevaluator.h>
#include "appargs.h"

//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{
namespace cmdLine
{

class AbbaUWCmdLineEvaluator : public libBatch::cmdLine::CmdLineEvaluator
{
public:
    AbbaUWCmdLineEvaluator( const libBatch::cmdLine::CmdLineConfiguration& );
    ~AbbaUWCmdLineEvaluator();

    const AppArgs&          getAppArgs() const;

private:
    AbbaUWCmdLineEvaluator( AbbaUWCmdLineEvaluator& );
    AbbaUWCmdLineEvaluator& operator=( AbbaUWCmdLineEvaluator& );

    virtual bool      evaluateAppArgs                 ();

    void                    readInCountryCode               ();
    void					readTransferDir                 ();
    void					readInputDir                    ();
    void					readForBranchnos                ();
   
	AppArgs                 m_AppArgs;
};

} // end namespace cmdLine
} // end namespace abbaUW

#endif 
