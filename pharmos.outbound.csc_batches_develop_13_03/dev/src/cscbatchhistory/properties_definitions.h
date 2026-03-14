#ifndef GUARD_PROPERTIES_DEFINITIONS_H
#define GUARD_PROPERTIES_DEFINITIONS_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_parameterlist.h>
#include <libbasarproperty_definitions.h>
#include <libbasarproperty_propertydescription.h>

namespace cscbatchhistory 
{
	namespace properties 
	{

		basar::ConstString NAMESPACE_NAME( "libabbauw::properties" );

		// definition of properties
		// orderhead
		CONST_PROPERTY_DESCRIPTION( BRANCHNO,					"branchno",						basar::INT16	)
		CONST_PROPERTY_DESCRIPTION( ORDERNO,					"orderno",						basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( STATUS,						"status",						basar::STRING	)	// Status als Kennzeichen: "FE", "ER", ZU", "ST"
		CONST_PROPERTY_DESCRIPTION( ORDERTYPE,					"ordertype",					basar::STRING	)	// =Konkatenation z.B. "UW00", "OM22"
		CONST_PROPERTY_DESCRIPTION( ORDERPICKINGTYPE,			"orderpickingtype",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( BOOKENTRYTYPE,				"bookentrytype",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( ORDERACQUISITIONDATE,       "orderacquisitiondate",	        basar::DATE     )
		CONST_PROPERTY_DESCRIPTION( ORDERDATE,					"orderdate",					basar::DATE   	)	// =orderhead.orderdate
		CONST_PROPERTY_DESCRIPTION( ORDERVALUE,					"ordervalue",					basar::DECIMAL	)	// Summe der wholesalerpurchaseprices der Positionen des Auftrag
		CONST_PROPERTY_DESCRIPTION( DEFERREDPAYMENTDATE,		"deferredpaymentdate",			basar::DATE	    )
		CONST_PROPERTY_DESCRIPTION( PHARMACYNOMF,				"pharmacynomf",					basar::STRING	)  
		CONST_PROPERTY_DESCRIPTION( HEADERTEXT,					"headertext",					basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( DATASOURCETYPE,				"datasourcetype",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( EDIDELIVERYDATE,			"edideliverydate",				basar::DATE	    )  // =Liefertermin
		CONST_PROPERTY_DESCRIPTION( REFERENCE,					"reference",					basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( ORDERNOCSC,					"ordernocsc",					basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( ORDERNOMF,					"ordernomf",					basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( ORDERREQUEST,               "orderrequest",                 basar::INT32    )
		CONST_PROPERTY_DESCRIPTION( MANUFACTURERNAME,			"manufacturername",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( MANUFACTURERNO,				"manufacturerno",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( NOTICE,				        "notice",				        basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( LARGESCALEORDER,            "largescaleorder",              basar::INT32    )
		CONST_PROPERTY_DESCRIPTION( LARGESCALEORDERLIMIT,       "largescaleorderlimit",         basar::INT32    )
		CONST_PROPERTY_DESCRIPTION( NONSTOCKITEM,               "nonstockitem",                 basar::INT16    )

		// orderpos (without those that have been mentioned above)
		CONST_PROPERTY_DESCRIPTION( POSNO,						"posno",						basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( POSQTY,						"posqty",						basar::INT32	)  // Anzahl Positionen eines Auftrags
		CONST_PROPERTY_DESCRIPTION( ARTICLENO,					"articleno",					basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( PZN,                        "pzn",                          basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( UNIT,						"unit",							basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( ARTICLENAME,                "articlename",                  basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( PHARMACEUTICALFORM,			"pharmaceuticalform",			basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( STORAGELOCATIONSLOT,        "storagelocationslot",          basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( EXPIRYDATE,					"expirydate",					basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( DISCOUNTPCT,                "discountpct",                  basar::DECIMAL	)
		CONST_PROPERTY_DESCRIPTION( DISCOUNTTYPE,               "discounttype",                 basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( ORDEREDQTY,                 "orderedqty",                   basar::INT32	)  // Bestellte Menge einer Auftragszeile
		CONST_PROPERTY_DESCRIPTION( NONCHARGEDQTY,              "nonchargedqty",                basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( WHOLESALERPURCHASEPRICE,    "wholesalerpurchaseprice",      basar::DECIMAL	)
		CONST_PROPERTY_DESCRIPTION( DELIVERYNOTE,				"deliverynote",					basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( ARTICLECLASS,				"articleclass",					basar::INT32	) //articlezentral.etartklasse1
		CONST_PROPERTY_DESCRIPTION( LOCALARTICLEBITFIELD,		"localarticlebitfield",			basar::INT32	) //artikellokal.etartschalter1
		CONST_PROPERTY_DESCRIPTION( LOCALARTICLEBITFIELD2,		"localarticlebitfield2",		basar::INT32	) //artikellokal.etartschalter2
		CONST_PROPERTY_DESCRIPTION( CENTRALARTICLEBITFIELD,		"centralarticlebitfield",		basar::INT32	) //artikelzentral.etartschalter1
		CONST_PROPERTY_DESCRIPTION( CENTRALARTICLEBITFIELD2,	"centralarticlebitfield2",		basar::INT32	) //artikelzentral.etartschalter2
		CONST_PROPERTY_DESCRIPTION( ORDERPROPOSALTIME,			"orderproposaltime",			basar::DATETIME )
		CONST_PROPERTY_DESCRIPTION( ORDERPROPOSALQTY,			"orderproposalqty",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( PURCHASEORDERNO,            "purchaseorderno",              basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( TEXTFIELD,                  "textfield",                    basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( CUSTOMERPOSNO,              "customerposno",                basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( SUCCESSOR,                  "successor",                    basar::INT16    )
		CONST_PROPERTY_DESCRIPTION( ENTRYTYPE,                  "entrytype",                    basar::STRING   )
		CONST_PROPERTY_DESCRIPTION( DISCOUNTCALCFROM,           "discountcalcfrom",             basar::INT16    )
		CONST_PROPERTY_DESCRIPTION( DISCOUNTAPPLYTO,            "discountapplyto",              basar::INT16    )

		   // customer
		CONST_PROPERTY_DESCRIPTION( PHARMACYNO,					"pharmacyno",					basar::INT32	)  //idfnr
		CONST_PROPERTY_DESCRIPTION( PHARMACYNAME,				"pharmacyname",					basar::STRING	) //nameapo
		CONST_PROPERTY_DESCRIPTION( TELEPHONY_INFO,             "telephonyinfo",                basar::STRING   )
		CONST_PROPERTY_DESCRIPTION( HAS_TELEPHONY_INFO,         "has_telephonyinfo",            basar::INT16    )

		// orderheadprotocol/orderposprotocol
		CONST_PROPERTY_DESCRIPTION( PROTOCOL_DATE,              "protocoldate",                 basar::DATETIME )
		CONST_PROPERTY_DESCRIPTION( ACTIVITY_TYPE,              "activitytype",                 basar::STRING   )
		CONST_PROPERTY_DESCRIPTION( EVENT_TYPE,					"eventtype",					basar::INT16	)
		CONST_PROPERTY_DESCRIPTION( CHANGED_ATTRIBUTE,			"changedattribute",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( OLD_VALUE,					"oldvalue",						basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( NEW_VALUE,					"newvalue",						basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( SPLIT_FROM,					"splitfrom",					basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( SPLIT_TO,					"splitto",					    basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( PROCESSED_BY,				"processedby",					basar::INT16	)

		CONST_PROPERTY_DESCRIPTION( ITEMTEXT,					"itemtext",						basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( EXTRATEXT,				    "extratext",					basar::STRING	)
       
		CONST_PROPERTY_DESCRIPTION( VIEW,						"view",							basar::STRING	)

		CONST_PROPERTY_DESCRIPTION( ORDERREQUESTQTY,			"orderrequestqty",				basar::INT32	)

		//needed for cscbatchhistory
		CONST_PROPERTY_DESCRIPTION( MINAGEFORORDERPROTOCOL,		"minagefororderprotocol",		basar::STRING   )

	}
}

#endif //end GUARD_PROPERTIES_DEFINITIONS_H
