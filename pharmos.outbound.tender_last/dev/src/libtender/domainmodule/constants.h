#ifndef GUARD_CONSTANTS_H
#define GUARD_CONSTANTS_H

namespace constants
{
	//Regions
	const basar::Int16 REGION_BG = 2008;
	const basar::Int16 REGION_RS = 2009;
	const basar::Int16 REGION_FR = 2011;
    const basar::Int16 REGION_CH = 2002;

	//Order type batch for purchase order proposal
	const basar::Int16 ORDER_TYPE_BATCH = 2;

	//CSC Order 
	//Order type is maintained in UADM!!! 
	const basar::VarString PICKING_TYPE = "2";
	const basar::VarString BOOKING_TYPE = "2";

	const basar::VarString ORDER_TYPE_TO = "TO";
	const basar::VarString ORDER_TYPE_TI = "TI";
	const basar::VarString ORDER_TYPE_OM = "OM";
    const basar::VarString PICKING_TYPE_OM = "2";
    const basar::VarString BOOKING_TYPE_OM = "2";
}

#endif // GUARD_CONSTANTS_H