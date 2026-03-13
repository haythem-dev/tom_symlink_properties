//----------------------------------------------------------------------------
/*! \file abbauw_properties_definitions.h
 *  \brief  global definitions for database access of abba-uw
*  \author Jia Li, P. Kessling, M. Meier
*  \date   02.01.2013 
*  history: 17.04.13 pk rework
*           07.05.13 pk,mm redesign according to BASAR standard
*           26.06.14 pk new Property orderhead->PHARMACYNOMF
*			25.111.14 pk new property DELIVERERBGA
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBABBAUW_PROPERTIES_DEFINITIONS_H
#define GUARD_LIBABBAUW_PROPERTIES_DEFINITIONS_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include <libbasarcmnutil_parameterlist.h>
#include <libbasarproperty_propertydescription.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace libabbauw
{
	namespace properties
	{
		basar::ConstString NAMESPACE_NAME( "libabbauw::properties" );

		// definition of properties 
		// orderhead
		CONST_PROPERTY_DESCRIPTION(BRANCHNO,				"branchno",					basar::INT16	)
		CONST_PROPERTY_DESCRIPTION(ORDERNO,					"orderno",					basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(STATUS,					"status",					basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(ORDERTYPE,				"ordertype",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(ORDERPICKINGTYPE,		"orderpickingtype",			basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(BOOKENTRYTYPE,			"bookentrytype",			basar::STRING	) 
		CONST_PROPERTY_DESCRIPTION(ORDERDATE,				"orderdate",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(ORDERVALUE,				"ordervalue",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(DEFERREDPAYMENTDATE,		"deferredpaymentdate",		basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(PHARMACYNO,				"pharmacyno",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(HEADERTEXT,				"headertext",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(ORDERACQUISITIONDATE,	"orderacquisitiondate",		basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(DATASOURCETYPE,			"datasourcetype",			basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(EDIDELIVERYDATE,			"edideliverydate",			basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(REFERENCE,				"reference",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(ORDERNOCSC,				"ordernocsc",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(ORDERNOMF,				"ordernomf",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(MANUFACTURERNAME,		"manufacturername",			basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(MANUFACTURERNO,			"manufacturerno",			basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(EDIDELIVERYDATEMF,		"edideliverydatemf",		basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(PHARMACYNOMF,			"pharmacynomf",				basar::STRING	)


		// orderpos (without those that have been mentioned above)
		CONST_PROPERTY_DESCRIPTION(POSNO,					"posno",					basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(POSQTY,					"posqty",					basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(ARTICLENO,				"articleno",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(ARTICLE_CODE,			"article_code",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(ORDEREDQTY,				"orderedqty",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(NONCHARGEDQTY,			"nonchargedqty",			basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(TEXTFIELD,				"textfield",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(CUSTOMERPOSNO,			"customerposno",			basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(WHOLESALERPURCHASEPRICE,	"wholesalerpurchaseprice",	basar::DECIMAL	)
		CONST_PROPERTY_DESCRIPTION(DISCOUNTPCT,				"discountpct",			    basar::DECIMAL	)
		CONST_PROPERTY_DESCRIPTION(DISCOUNTTYPE,			"discounttype",			    basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(DISCOUNTCALCFROM,		"discountcalcfrom",			basar::INT16	)
		CONST_PROPERTY_DESCRIPTION(DISCOUNTAPPLYTO,			"discountapplyto",			basar::INT16	)
        //orderprotocol
        CONST_PROPERTY_DESCRIPTION( PROTOCOL_DATE,              "protocoldate",         basar::DATETIME )
		CONST_PROPERTY_DESCRIPTION( ACTIVITY_TYPE,              "activitytype",         basar::STRING   )
		CONST_PROPERTY_DESCRIPTION( EVENT_TYPE,					"eventtype",			basar::INT16	)
		CONST_PROPERTY_DESCRIPTION( CHANGED_ATTRIBUTE,			"changedattribute",		basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( OLD_VALUE,					"oldvalue",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( NEW_VALUE,					"newvalue",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( SPLIT_FROM,					"splitfrom",			basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( SPLIT_TO,					"splitto",			    basar::INT32	)
        CONST_PROPERTY_DESCRIPTION( PROCESSED_BY,				"processedby",			basar::INT16	)
		CONST_PROPERTY_DESCRIPTION( VIEW,						"view",					basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( ORDERREQUESTQTY,			"orderrequestqty",		basar::INT32	)
        CONST_PROPERTY_DESCRIPTION( EXTRATEXT,			        "extratext",		    basar::STRING	)
        CONST_PROPERTY_DESCRIPTION( ITEMTEXT,			        "itemtext",		        basar::STRING	)

		//// articles
		//CONST_PROPERTY_DESCRIPTION(VALIDFROMDATE,			"validfromdate",			basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(VALIDTODATE,				"validtodate",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION(CODE_TYPE,				"code_type",				basar::INT16	)
		CONST_PROPERTY_DESCRIPTION(PREFERRED_FLAG,			"preferred_flag",			basar::INT16	)
		CONST_PROPERTY_DESCRIPTION(ARTICLENOMF,				"articlenomf",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(ISCMI,					"cmi_flag",					basar::INT16	)


		// ELSE
		CONST_PROPERTY_DESCRIPTION(PHARMACYNAME,			"pharmacyname",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(CUSTOMERGROUP,			"customergroup",			basar::STRING	)
  		CONST_PROPERTY_DESCRIPTION(PHARMACYNOEAN,			"pharmacynoean",			basar::STRING	)
  		CONST_PROPERTY_DESCRIPTION(DELIVERERBGA,			"bganr",					basar::STRING	)


  		CONST_PROPERTY_DESCRIPTION(MANUFACTUREREAN,			"manufacturerean",			basar::STRING	)
		CONST_PROPERTY_DESCRIPTION(DELIVERERNO,				"delivererno",				basar::INT32	)


		CONST_PROPERTY_DESCRIPTION(SHORTFALLPOS,			"shortfallpos",				basar::INT32	)


	}	// end namespace properties
}	// end namespace libabbauw


#endif //end GUARD_LIBABBAUW_PROPERTIES_DEFINITIONS_H
