//----------------------------------------------------------------------------
/*! \file
    \brief  
 *  \author Hendrik Polzin 
 *  \date 15.08.2013
 */
//----------------------------------------------------------------------------//

#ifndef GUARD_PU_BATCH_ORDER_POS_DM_DEFINITIONS_H
#define GUARD_PU_BATCH_ORDER_POS_DM_DEFINITIONS_H

//----------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------//
#include <libbasarcmnutil_parameterlist.h>
#include <libbasarproperty_propertydescription.h>

//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace pubatchoderposdm
{    
    const basar::Int32 MIN_NUM_CHARS_FOR_ARTICLE_SEARCH = 4;
}

namespace pubatchorderposdm
{    
namespace lit
{
	const basar::VarString ACC_PUBATCHORDERPOS				( "AccPuBatchOrderPos");
	const basar::VarString ACC_PUBATCHORDERPOS_INST         ( "AccPuBatchOrderPosInstance");
	const basar::VarString SELECT_PU_BATCH_ORDER_POSITIONS	( "SelectPuBatchOrderPositions");
	const basar::VarString INSERT_PU_BATCH_ORDER_POSITIONS	( "InsertPuBatchOrderPositions");

} // namespace 
} // namespace 

#endif	// end 
