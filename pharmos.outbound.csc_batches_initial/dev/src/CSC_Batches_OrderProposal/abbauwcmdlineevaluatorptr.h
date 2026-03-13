//--------------------------------------------------------------------------------------------------//
/*! \file
 *  \author     
 *  \date       
 *  \date    
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_CMDLINE_ABBAUW_CMDLINE_EVALUATORPTR_H
#define GUARD_ABBAUW_CMDLINE_ABBAUW_CMDLINE_EVALUATORPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{
    namespace cmdLine
    {
        class AbbaUWCmdLineEvaluator;
        typedef boost::shared_ptr< AbbaUWCmdLineEvaluator > AbbaUWCmdLineEvaluatorPtr;
    }
}

#endif 
