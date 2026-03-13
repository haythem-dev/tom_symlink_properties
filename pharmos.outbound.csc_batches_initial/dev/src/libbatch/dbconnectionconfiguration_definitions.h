//--------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Bischof Bjoern
 *  \date       26.12.2010
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBATCH_DB_CONNECTION_DEFINITIONS_H
#define GUARD_LIBBATCH_DB_CONNECTION_DEFINITIONS_H

//--------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------//
#include <libbasarcmnutil.h>

//--------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------//
namespace libBatch
{
    namespace dbConnection
    {
        basar::ConstString DBSRV_PARAMETERNAME    = "DBSRV";
        basar::ConstString DB_PARAMETERNAME       = "DB";

    } // end namespace dbConnection
} // end namespace libBatch

#endif // GUARD_LIBBATCH_DB_CONNECTION_DEFINITIONS_H
