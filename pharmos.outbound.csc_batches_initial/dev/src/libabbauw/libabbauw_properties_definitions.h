#ifndef GUARD_LIBABBAUW_PROPERTIES_DEFINITIONS_H
#define GUARD_LIBABBAUW_PROPERTIES_DEFINITIONS_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include <libbasarcmnutil_parameterlist.h>
#include <libbasarproperty_definitions.h>
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
		CONST_PROPERTY_DESCRIPTION( BRANCHNO,					"branchno",						basar::INT16	)
		CONST_PROPERTY_DESCRIPTION( BRANCHNO_ORIG,				"branchnoorig",					basar::INT16	)
		CONST_PROPERTY_DESCRIPTION( ORDERNO,					"orderno",						basar::INT32	)
        CONST_PROPERTY_DESCRIPTION( ORDERNO_UPDATE,			    "orderno_upd",				    basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( STATUS,						"status",						basar::STRING	)	// Status als Kennzeichen: "FE", "ER", ZU", "ST"
		CONST_PROPERTY_DESCRIPTION( ORDERTYPE,					"ordertype",					basar::STRING	)	// =Konkatenation z.B. "UW00", "OM22"
		CONST_PROPERTY_DESCRIPTION( ORDERPICKINGTYPE,			"orderpickingtype",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( BOOKENTRYTYPE,				"bookentrytype",				basar::STRING	)
        CONST_PROPERTY_DESCRIPTION( ORDERACQUISITIONDATE,       "orderacquisitiondate",	        basar::DATE     )
		CONST_PROPERTY_DESCRIPTION( ORDERDATE,					"orderdate",					basar::DATE   	)	// =orderhead.orderdate
		CONST_PROPERTY_DESCRIPTION( ORDERVALUE,					"ordervalue",					basar::DECIMAL	)	// Summe der wholesalerpurchaseprices der Positionen des Auftrag
		CONST_PROPERTY_DESCRIPTION( DISCOUNTPCTRANGE,           "discountpctrange",             basar::STRING   )
		CONST_PROPERTY_DESCRIPTION( DEFERREDPAYMENTDATE,		"deferredpaymentdate",			basar::DATE	    )
		CONST_PROPERTY_DESCRIPTION( PHARMACYNOMF,				"pharmacynomf",					basar::STRING	)  
		CONST_PROPERTY_DESCRIPTION( HEADERTEXT,					"headertext",					basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( ISMANUALHEADERTEXTFLAG,     "ismanualheadertextflag",       basar::STRING   )
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
		CONST_PROPERTY_DESCRIPTION( EDIDELIVERYDATEMF,			"edideliverydatemf",			basar::DATE     )
        CONST_PROPERTY_DESCRIPTION( SUM_UNKNOWN_ARTICLES,       "sum_unknown_articles",         basar::INT32	)

		// search properties
		CONST_PROPERTY_DESCRIPTION( ORDERNO_TO,					"orderno_to",					basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( ORDERNO_FROM,				"orderno_from",					basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( POS_TO,						"pos_to",						basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( POS_FROM,					"pos_from",						basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( ORDERDATE_TO,				"orderdate_to",					basar::DATE   	)
		CONST_PROPERTY_DESCRIPTION( ORDERDATE_FROM,				"orderdate_from",				basar::DATE   	)
		CONST_PROPERTY_DESCRIPTION( EDIDELIVERYDATE_TO,			"edideliverydate_to",			basar::DATE	    )
		CONST_PROPERTY_DESCRIPTION( EDIDELIVERYDATE_FROM,		"edideliverydate_from",			basar::DATE	    )
		CONST_PROPERTY_DESCRIPTION( DEFERREDPAYMENTDATE_TO,		"deferredpaymentdate_to",		basar::DATE	    )
		CONST_PROPERTY_DESCRIPTION( DEFERREDPAYMENTDATE_FROM,	"deferredpaymentdate_from",	    basar::DATE	    )
        CONST_PROPERTY_DESCRIPTION( ORDERACQUISITIONDATE_TO,    "orderacquisitiondate_to",      basar::DATE     )
        CONST_PROPERTY_DESCRIPTION( ORDERACQUISITIONDATE_FROM,  "orderacquisitiondate_from",    basar::DATE     )
		CONST_PROPERTY_DESCRIPTION( ORDERVALUE_TO,				"ordervalue_to",				basar::DECIMAL	)
		CONST_PROPERTY_DESCRIPTION( ORDERVALUE_FROM,            "ordervalue_from",				basar::DECIMAL	)

		// sort properties
		CONST_PROPERTY_DESCRIPTION( SORT,                       "sort",                         basar::STRING   )

        // color properties
        CONST_PROPERTY_DESCRIPTION( DAYS_TO_COLOR,              "days_to_color",                basar::INT32    )

		// orderpos (without those that have been mentioned above)
		CONST_PROPERTY_DESCRIPTION( POSNO,						"posno",						basar::INT32	)
        CONST_PROPERTY_DESCRIPTION( POSNO_ORIG,			        "posnoorig",					basar::INT32	)
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
		CONST_PROPERTY_DESCRIPTION( POSITIONSTATUS,			    "positionstatus",		        basar::STRING   )

        // customer
        CONST_PROPERTY_DESCRIPTION( PHARMACYNO,					"pharmacyno",					basar::INT32	)  //idfnr
		CONST_PROPERTY_DESCRIPTION( PHARMACYNAME,				"pharmacyname",					basar::STRING	) //nameapo
        CONST_PROPERTY_DESCRIPTION( TELEPHONY_INFO,             "telephonyinfo",                basar::STRING   )
        CONST_PROPERTY_DESCRIPTION( HAS_TELEPHONY_INFO,         "has_telephonyinfo",            basar::INT16    )

		// supplier
        CONST_PROPERTY_DESCRIPTION( PURCHASEORDERSUPPLIERNO,    "purchaseordersuppliernumber",  basar::INT32    ) // besla_nr
        CONST_PROPERTY_DESCRIPTION( PURCHASEORDERSUPPLIERNAME,  "purchaseordersuppliername",    basar::STRING   )
		CONST_PROPERTY_DESCRIPTION( MATCHCODE,					"matchcode",					basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( CUSTOMER_NAME,				"customername",					basar::STRING	) //nameinhaber
		CONST_PROPERTY_DESCRIPTION( LOCATION,					"location",						basar::STRING	) //ort
		CONST_PROPERTY_DESCRIPTION( ADDRESS,                    "address",                      basar::STRING	) //strasse
		CONST_PROPERTY_DESCRIPTION( POSTCODE,					"postcode",						basar::STRING	) //postleitzahl, plz
		CONST_PROPERTY_DESCRIPTION( PHONENO,                    "phoneno",                      basar::STRING	) //telnr
        CONST_PROPERTY_DESCRIPTION( SPEED_DIAL_NO,              "speeddialno",                  basar::STRING   ) //kurzwahl
        CONST_PROPERTY_DESCRIPTION( CUSTOMERLOCKED,             "customerlocked",               basar::INT16    )

        // stock
		CONST_PROPERTY_DESCRIPTION( SHORTFALLPOS,				"shortfallpos",					basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( SHORTFALLQTY,				"shortfallqty",					basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( SHORTFALLQTYACCUMULATED,    "shortfallqtyaccumulated",		basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( COLOR,						"color",						basar::INT16	)
		CONST_PROPERTY_DESCRIPTION( STOCK,						"stock",						basar::INT32    )  // Freiverkaufsmenge = Bestand ohne Reservierungen
		CONST_PROPERTY_DESCRIPTION( STOCKACCUMULATED,			"stockaccumulated",				basar::INT32    )  // Freiverkaufsmenge kumuliert = Bestand ohne Reservierungen
		CONST_PROPERTY_DESCRIPTION( BACKORDER,					"backorder",					basar::INT32    )  // OBM-W= wirksamer Orderrückstand
		CONST_PROPERTY_DESCRIPTION( BACKORDER_NOT_EFFECTIVE,	"backorder_noteffective",		basar::INT32	)  // OBM-U= unwirksamer Orderrückstand
        CONST_PROPERTY_DESCRIPTION( PSEUDOSTORAGELOCATIONSLOT,  "pseudostoragelocationslot",    basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( RESERVEDQTY,                "reservedqty",					basar::INT32    )
		CONST_PROPERTY_DESCRIPTION( CMIRESERVEDQTY,				"cmireservedqty",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( MINSTOCK,					"minstock",						basar::INT32    )
		CONST_PROPERTY_DESCRIPTION( PREDICTION,					"prediction",					basar::FLOAT64  )
        CONST_PROPERTY_DESCRIPTION( PREDICTIONDATE,             "predictiondate",               basar::INT32    )

		// parameter
        CONST_PROPERTY_DESCRIPTION( PARAMETERVALUE,				"parametervalue",				basar::INT32	)
		CONST_PROPERTY_DESCRIPTION( PROGNAME,					"progname",						basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( PURPOSE,                    "purpose",                      basar::STRING	) // zweck
		CONST_PROPERTY_DESCRIPTION( PARAMETERNAME,				"parametername",				basar::STRING	)
		CONST_PROPERTY_DESCRIPTION( MANIFESTATION,				"manifestation",				basar::STRING	) // Ausprägung

        // orderproposal
		CONST_PROPERTY_DESCRIPTION( DATE_OF_PURCHASE,			"dateofpurchase",				basar::INT32    )
        CONST_PROPERTY_DESCRIPTION( DELIVERY_TIME,              "deliverytime",                 basar::FLOAT64  )
        CONST_PROPERTY_DESCRIPTION( EXPECTED_GI_DATE,           "expectedgidate",               basar::DATE     )
        CONST_PROPERTY_DESCRIPTION( NEXT_MAIN_ORDER_DATE,       "nextmainorderdate",            basar::DATE     )
		CONST_PROPERTY_DESCRIPTION( ORDER_PROP_DISCLAIM_NOTE,	"orderproposaldisclaimnote",	basar::STRING   )
		CONST_PROPERTY_DESCRIPTION( ORDER_PROP_PROCESSED_TIME,	"orderproposalprocessedtime",	basar::DATETIME )
		CONST_PROPERTY_DESCRIPTION( REASON_ID ,					"reasonid",						basar::INT16    )
		CONST_PROPERTY_DESCRIPTION( TRANSFER_TIME,				"transfertime",					basar::DATETIME )
		CONST_PROPERTY_DESCRIPTION( ORDER_PROP_NO,				"orderproposalno",				basar::INT32    )

		// article
        CONST_PROPERTY_DESCRIPTION( ARTICLEPRICE,               "articleprice",                 basar::DECIMAL  )
        CONST_PROPERTY_DESCRIPTION( ARTICLE_CLASS_ATTRIBUTES,   "article_class_attributes",     basar::INT32    )
        CONST_PROPERTY_DESCRIPTION( ARTICLE_ATTRIBUTES,         "article_attributes",           basar::INT32    )
		CONST_PROPERTY_DESCRIPTION( NORMPACK,					"normpackage",					basar::STRING   )
		CONST_PROPERTY_DESCRIPTION( EAN,					    "ean",					        basar::STRING   )
		CONST_PROPERTY_DESCRIPTION( AGP,					    "agp",					        basar::DECIMAL  )
		CONST_PROPERTY_DESCRIPTION( GEP,					    "gep",					        basar::DECIMAL  )

        // order head/pos in preparation
        CONST_PROPERTY_DESCRIPTION( USER,                       "user",                         basar::STRING   )

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

		CONST_PROPERTY_DESCRIPTION( TRAFFIC_LIGHT_INFO,			"trafficlightinfo",				basar::STRING	)

		//needed for cscbatchhistory
		CONST_PROPERTY_DESCRIPTION( MINAGEFORORDERPROTOCOL,		"minagefororderprotocol",		basar::DATETIME )

	}	// end namespace properties
}	// end namespace libabbauw

#endif //end GUARD_LIBABBAUW_PROPERTIES_DEFINITIONS_H
