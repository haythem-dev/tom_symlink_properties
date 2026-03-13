//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief 
 *  \author Hendrik Polzin 
 *  \date 15.08.2013
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_ACC_PU_BATCH_ORDER_POS_H
#define GUARD_ACC_PU_BATCH_ORDER_POS_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasardbaspect_macros.h>
#include <libbasardbaspect_definitions.h>
#include <libbasardbaspect_sqlstringbuilder.h>

//-------------------------------------------------------------------------------------------------//
// sqlstring builder declaration section
//-------------------------------------------------------------------------------------------------//
namespace acc_pubatchorderpos
{

	BEGIN_QUERY_BUILDER_DECLARATION( SelectPuBatchOrderPositions )
    END_BUILDER_DECLARATION

	BEGIN_WRITE_BUILDER_DECLARATION( InsertPuBatchOrderPositions )
    END_BUILDER_DECLARATION
}

#endif 
