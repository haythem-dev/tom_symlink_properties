#ifndef GUARD_LIBTENDER_PROPERTIES_DEFINITIONS_H
#define GUARD_LIBTENDER_PROPERTIES_DEFINITIONS_H

#include <libbasarproperty_definitions.h>
#include <libbasarproperty_propertydescription.h>

namespace properties
{
	CONST_PROPERTY_DESCRIPTION( ARTICLE_NO,							"articleno",					basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( ARTICLE_CODE,						"articlecode",					basar::STRING	)
	CONST_PROPERTY_DESCRIPTION( ARTICLE_NAME,						"articlename",					basar::STRING	)
	CONST_PROPERTY_DESCRIPTION( PACKGROESSE,						"packgroesse",					basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( DARREICHFORM,						"darreichform",					basar::STRING	)
	CONST_PROPERTY_DESCRIPTION( BUSINESS_TYPE,						"businesstype",					basar::INT16	)

	CONST_PROPERTY_DESCRIPTION( MANUFACTURER_NO,					"hersteller_no",				basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( MANUFACTURER_NAME,					"hersteller_name",				basar::STRING	)

	CONST_PROPERTY_DESCRIPTION( TAX_RATE,							"salestaxrate",					basar::DECIMAL  )
	CONST_PROPERTY_DESCRIPTION( AEP,								"preisekapo",					basar::DECIMAL  )
	CONST_PROPERTY_DESCRIPTION( AGP,								"preisagp",						basar::DECIMAL  )
	CONST_PROPERTY_DESCRIPTION( PRICE_INCL_VAT,						"price_incl_vat",				basar::DECIMAL	)
	CONST_PROPERTY_DESCRIPTION( ADD_DISCOUNT_PCT,					"adddiscountpct",				basar::DECIMAL	)
	CONST_PROPERTY_DESCRIPTION( DISCOUNT_CALC_FROM,                 "discountcalcfrom",             basar::INT16    )
	CONST_PROPERTY_DESCRIPTION( DISCOUNT_APPLY_TO,                  "discountapplyto",              basar::INT16    )
	CONST_PROPERTY_DESCRIPTION( POSITION_STATUS,					"position_status",				basar::STRING   )

	CONST_PROPERTY_DESCRIPTION( DATE_FROM,							"datefrom",						basar::DATE		)
	CONST_PROPERTY_DESCRIPTION( DATE_TO,							"dateto",						basar::DATE		)
	
    CONST_PROPERTY_DESCRIPTION( CHANGE_DATE,						"changedate",                    basar::DATE	)
    CONST_PROPERTY_DESCRIPTION( CREATION_DATE,						"creationdate",                  basar::DATE	)
    CONST_PROPERTY_DESCRIPTION( CURRENT_QTY,						"currentquantity",               basar::INT32	)

	CONST_PROPERTY_DESCRIPTION( CONTRACT_NO,						"contractno",					basar::STRING	)
	CONST_PROPERTY_DESCRIPTION( CONTRACT_PRICE,						"contractprice",				basar::DECIMAL	)
	CONST_PROPERTY_DESCRIPTION( CONTRACT_QTY,						"contractqty",					basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( BRANCH_NO,							"branchno",						basar::INT16	)
	CONST_PROPERTY_DESCRIPTION( ACC_BRANCH_NO,						"accbranchno",					basar::INT16	)
    CONST_PROPERTY_DESCRIPTION( CUSTOMER_NO,						"customerno",					basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( ACC_CUSTOMER_NO,					"acccustomerno",				basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( PHARMACY_NAME,						"pharmacyname",					basar::STRING	)
	CONST_PROPERTY_DESCRIPTION( PHARMACY_STREET,					"street",					    basar::STRING	)
	CONST_PROPERTY_DESCRIPTION( PHARMACY_LOCATION,					"pharmacylocation",				basar::STRING	)
    CONST_PROPERTY_DESCRIPTION( PHARMACY_GROUPID,					"pharmacygroupid",              basar::STRING	)
    CONST_PROPERTY_DESCRIPTION( PHARMACY_RANKING,					"ranking",                      basar::INT16	)
    CONST_PROPERTY_DESCRIPTION( UPD_FLAG,							"upd_flag",                     basar::STRING	)
    CONST_PROPERTY_DESCRIPTION( OWNQUOTA_QTY,						"ownquotaqty",                  basar::INT32	)
    CONST_PROPERTY_DESCRIPTION( OWNQUOTA_FLAG,						"ownquotaflag",                 basar::INT16	)
    CONST_PROPERTY_DESCRIPTION( RECALL_FLAG,						"recallflag",                   basar::INT16	)
    CONST_PROPERTY_DESCRIPTION( REMAINING_QUANTITY,					"remainingquantity",			basar::INT32	)

	CONST_PROPERTY_DESCRIPTION( DELETED_FLAG,						"deletedflag",					basar::INT16	)
	CONST_PROPERTY_DESCRIPTION( EXCEEDANCE_PCT,						"exceedancepct",				basar::DECIMAL	)
	CONST_PROPERTY_DESCRIPTION( NO_OF_ITEMS,						"noofitems",                    basar::INT16	)
	CONST_PROPERTY_DESCRIPTION( NO_OF_DELIVERIES,					"noofdeliveries",               basar::INT16	)
	CONST_PROPERTY_DESCRIPTION( NO_OF_OPEN_DELIVERIES,				"noofopendeliveries",           basar::INT16	)
    CONST_PROPERTY_DESCRIPTION( PARTNER_NO,							"partnerno",					basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( PARTNER_NAME,						"partnername",					basar::STRING	)
	CONST_PROPERTY_DESCRIPTION( PARTNER_STREET,						"street",						basar::STRING	)
	CONST_PROPERTY_DESCRIPTION( PARTNER_LOCATION,					"location",						basar::STRING	)
	CONST_PROPERTY_DESCRIPTION( PHARMACY_CHAIN_NO,					"pharmacychainno",				basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( CUSTOMERGROUPID,					"customergroupid",				basar::STRING	)
	CONST_PROPERTY_DESCRIPTION( DELIVERY_DATE,						"deliverydate",					basar::DATE		)
	CONST_PROPERTY_DESCRIPTION( DELIVERY_DATE_FROM,					"deliverydatefrom",				basar::DATE		)
	CONST_PROPERTY_DESCRIPTION( DELIVERY_DATE_TO,					"deliverydateto",				basar::DATE		)
    CONST_PROPERTY_DESCRIPTION( TENDER_NO,							"tenderno",						basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( TENDER_STATE,						"tenderstate",					basar::INT16	)
	CONST_PROPERTY_DESCRIPTION( TENDER_TYPE,						"tendertype",					basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( PERSONAL_NR_VERTR,					"personalnrvertr",				basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( PURCHASE_ORDER_PROPOSAL_FLAG,		"purchaseorderproposalflag",	basar::INT16	)

	CONST_PROPERTY_DESCRIPTION( NAME_OF_USER,						"nameofuser",					basar::STRING	)
   
	// scheduled delivery
	CONST_PROPERTY_DESCRIPTION( TENDER_DELIVERY_SCHEDULE_HEAD_ID,	"tenderdeliveryscheduleheadid",	basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( TENDER_ID,							"tenderid",						basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( CSC_ORDER_NO,						"cscorderno",					basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( CSC_ORDER_DATE,						"cscorderdate",					basar::DATE     )
	CONST_PROPERTY_DESCRIPTION( LAST_CHANGED_DATETIME,				"lastchangeddatetime",			basar::DATETIME )
	CONST_PROPERTY_DESCRIPTION( CREATION_DATETIME,					"creationdatetime",				basar::DATETIME )
	CONST_PROPERTY_DESCRIPTION( QUANTITY,							"quantity",						basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( QUANTITY_DELIVERED,					"quantitydelivered",			basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( QUANTITY_AVAILABLE,					"quantityavailable",			basar::INT32    )
    CONST_PROPERTY_DESCRIPTION( QUANTITY_ALLOCATED,					"quantityallocated",			basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( QUANTITY_CHARGED,					"quantitycharged",				basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( QUANTITY_TO_CHARGE,					"quantitytocharge",				basar::INT32    )
	CONST_PROPERTY_DESCRIPTION( PURCHASE_DB,						"purchasedb",					basar::STRING   )
	CONST_PROPERTY_DESCRIPTION( PURCHASE_DBSRV,						"purchasedbsrv",				basar::STRING   )

	// purchase order proposal
	CONST_PROPERTY_DESCRIPTION( POS_ID,								"posid",						basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( BRANCH_NO_POP,						"branchnopop",					basar::INT16	)
	CONST_PROPERTY_DESCRIPTION( ORDER_PROPOSAL_QUANTITY,			"orderproposalqty",				basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( ORDER_NO_BATCH,						"ordernobatch",					basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( ORDER_TYPE_BATCH,					"ordertypebatch",				basar::INT16	) // tender: immer 2
	CONST_PROPERTY_DESCRIPTION( ORDER_PROPOSAL_TIME,				"orderproposaltime",			basar::DATETIME )
	CONST_PROPERTY_DESCRIPTION( TRANSFER_TIME,						"transfertime",					basar::DATETIME ) // current
	CONST_PROPERTY_DESCRIPTION( ORDER_PROPOSAL_NO,					"orderproposalno",				basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( PURCHASE_ORDER_NO,					"purchaseorderno",				basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( EXPECTED_GOODSIN_DATE,				"expectedgoodsindate",			basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( SUPPLIER_NO,						"supplierno",					basar::INT32	) // liefer::lant_nr
    CONST_PROPERTY_DESCRIPTION( SUPPLIER_NAME,                      "suppliername",                 basar::STRING   )
    CONST_PROPERTY_DESCRIPTION( SUPPLIER_STREET,                    "supplierstreet",               basar::STRING   ) // liefer::lant_nr
    CONST_PROPERTY_DESCRIPTION( SUPPLIER_LOCATION,                  "supplierlocation",             basar::STRING   )
	// check customer
	CONST_PROPERTY_DESCRIPTION( ORIGINAL_BRANCH_NO,					"originalbranchno",				basar::INT16	)

	// protocol
	CONST_PROPERTY_DESCRIPTION( ORDER_DATE,							"orderdate",					basar::DATE 	)
	CONST_PROPERTY_DESCRIPTION( ORDER_NO,							"orderno",						basar::INT32	)
	CONST_PROPERTY_DESCRIPTION( SHOW_INVOICED_ORDERS,				"showinoicedorders",			basar::INT16    )

    //tenderprogress
    CONST_PROPERTY_DESCRIPTION( CHARGED_PRICE,                      "chargedprice",                 basar::DECIMAL  )
    CONST_PROPERTY_DESCRIPTION( RETURN_QUANTITY,                    "returnqty",                    basar::INT32    )
    CONST_PROPERTY_DESCRIPTION( DELIVERY_QTY,                       "deliveryqty",                  basar::INT32    )
    CONST_PROPERTY_DESCRIPTION( INVOICE_NO,                         "invoiceno",                    basar::INT32    )
    CONST_PROPERTY_DESCRIPTION( INVOICE_DATE,                       "invoicedate",                  basar::DATE     )
    CONST_PROPERTY_DESCRIPTION( GEP,                                "preisgep",                     basar::DECIMAL  )
    CONST_PROPERTY_DESCRIPTION( ORDER,                              "kdauftragart",                 basar::DECIMAL  )
    CONST_PROPERTY_DESCRIPTION( ORDER_TIME,                         "ordertime",                    basar::DATETIME )
    CONST_PROPERTY_DESCRIPTION( INVOICE_TIME,                       "invoicetime",                  basar::DATETIME )
    CONST_PROPERTY_DESCRIPTION( BOOKING_TENDERNO,                   "bookingtenderno",              basar::INT32    )
    CONST_PROPERTY_DESCRIPTION( DISCOUNT_PCT,                       "discountpct",                  basar::DECIMAL  )

    CONST_PROPERTY_DESCRIPTION( RECLAIM_OMG_DISC,                   "reclaim_omg_disc",             basar::STRING   )
    CONST_PROPERTY_DESCRIPTION( PHARMACY_GROUP_NAME,                "group_name",                   basar::STRING   )

    CONST_PROPERTY_DESCRIPTION( PRICE_DIFFERENCE,                   "pricedifference",              basar::DECIMAL  )
    CONST_PROPERTY_DESCRIPTION( TRANSFERED_FLAG,                    "transferredflag",              basar::INT16    )
    CONST_PROPERTY_DESCRIPTION( BASE_PRICE,                         "baseprice",                    basar::DECIMAL  )

} // end namespace properties

#endif    // GUARD_LIBTENDER_PROPERTIES_DEFINITIONS_H
