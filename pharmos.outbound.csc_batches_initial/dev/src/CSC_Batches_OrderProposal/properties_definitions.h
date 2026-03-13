//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief      
 *  \author     Hendrik Polzin
 *  \date       15.08.2013
 *  \version    
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_PROPERTIES_DEFINITIONS_H
#define GUARD_PROPERTIES_DEFINITIONS_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasarproperty_definitions.h>
#include <libbasarproperty_propertydescription.h>
#include <libbasarproperty_definitions.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbsql_databaseinfo.h>
#include <libbasarproperty_propertytable_xiterator.h>
#include <libbasarproperty_propertystate.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace properties
{
    basar::ConstString NAMESPACE_NAME( "properties" );

	CONST_PROPERTY_DESCRIPTION( ARTICLE_NO,					"articleno",                  basar::INT32    )
    CONST_PROPERTY_DESCRIPTION( BRANCH_NO,					"branchno",                   basar::INT16    )
    CONST_PROPERTY_DESCRIPTION( DATE_OF_PURCHASE,			"dateofpurchase",             basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( ORDER_NO_BATCH,				"ordernobatch",               basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( ORDER_PROP_DISCLAIM_NOTE,	"orderproposaldisclaimnote",  basar::STRING   )
	CONST_PROPERTY_DESCRIPTION( ORDER_PROP_PROCESSED_TIME,	"orderproposalprocessedtime", basar::DATETIME )
	CONST_PROPERTY_DESCRIPTION( ORDER_PROPOSAL_TIME,		"orderproposaltime",          basar::DATETIME )
	CONST_PROPERTY_DESCRIPTION( ORDER_PROPOSAL_QTY,			"orderproposalqty",           basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( ORDER_PROPOSAL_NO,			"orderproposalno",            basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( POS_NO_BATCH,				"posnobatch",                 basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( POSID,						"posid",                      basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( PURCHASE_ORDER_NO,			"purchaseorderno",            basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( REASON_ID ,					"reasonid",                   basar::INT16    )
	CONST_PROPERTY_DESCRIPTION( TRANSFER_TIME,				"transfertime",               basar::DATETIME )
	CONST_PROPERTY_DESCRIPTION( TRANSFER_TIME_BI,			"transfertimebi",             basar::DATETIME )
    CONST_PROPERTY_DESCRIPTION( EXPECTED_GI_DATE,           "expectedgoodsindate",        basar::INT32    )

} // end namespace 

#endif	// GUARD_PROPERTIES_DEFINITIONS_H
