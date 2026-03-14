//--------------------------------------------------------------------------------------------------//
/*! \brief 
 *  \author     
 *  \date       
 *  \version    
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_COMMAND_LINE_DEFINITIONS_H
#define GUARD_ABBAUW_COMMAND_LINE_DEFINITIONS_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include <libbasarcmnutil.h>

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{
    namespace cmdLine
    {
        static basar::ConstString COUNTRY_CODE  = "-countrycode";
        static basar::ConstString TRANSFER_DIR  = "-transferdir";    
        static basar::ConstString INPUT_DIR     = "-inputdir";    
        static basar::ConstString FOR_BRANCHNOS = "-forbranchnos";    
}
}

#endif 
