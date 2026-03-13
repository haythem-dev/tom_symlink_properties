#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERPOSITIONSACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERPOSITIONSACC_DEFINITIONS_H

#include <libbasarcmnutil_parameterlist.h>

namespace libtender
{
namespace infrastructure
{
namespace accessor
{
namespace tender
{
namespace lit
{
const basar::VarString TENDER_POSITIONS_ACC( "AccTenderPositions" );

const basar::VarString SELECT_TENDER_POSITIONS_BY_KEY_FR( "SelectTenderPositionsByKeyFR" );
const basar::VarString SELECT_TENDER_POSITIONS_BY_KEY_RS( "SelectTenderPositionsByKeyRS" );
const basar::VarString SELECT_TENDER_POSITIONS_BY_KEY_BG( "SelectTenderPositionsByKeyBG" );
const basar::VarString SELECT_TENDER_POSITIONS_BY_KEY_CH( "SelectTenderPositionsByKeyCH" );

const basar::VarString SAVE_TENDER_POSITION( "SaveTenderPosition" );

} // end namespace lit
} // end namespace tender
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERPOSITIONSACC_DEFINITIONS_H
