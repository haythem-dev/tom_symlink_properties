//----------------------------------------------------------------------------
/*! \file	definitions.h
 *  \brief  global definitions for appl_template
 *  \author Beatrix Trömel
 *  \date   29.10.2008
 */
//----------------------------------------------------------------------------
#ifndef GUARD_LIBCSCBATCH_DEFINITIONS_H
#define GUARD_LIBCSCBATCH_DEFINITIONS_H


basar::ConstString UADM_APPLICATION_NAME	= "CSC"; //is used for user admin login
basar::ConstString UADM_BATCH_USERNAME		= "cscbatchuser";
basar::ConstString UADM_BATCH_PASSWORD		= "((i3Nlr)";

static const basar::UInt32	CSC_POS_ITEMNOTE_LENGTH	= 29;	// Max. Number of netto characters in Position-Field ITEMNOTE that CSC-Client can stand 


basar::ConstString UADM_START_APPL	= "START";		//!< parameter in useradministration (START)
basar::ConstString UADM_GROUP		= "GROUP";		//!< parameter in useradministration to read group
basar::ConstString UADM_HOST		= "HOST";		//!< parameter in useradministration to read server
basar::ConstString UADM_DB			= "DB";			//!< parameter in useradministration to read database
basar::ConstString UADM_KSCSERVER	= "KSCSERVER";	//!< parameter in useradministration to read KSC server
basar::ConstString UADM_KSCPORT		= "KSCPORT";	//!< parameter in useradministration to read KSC port
	
#define DATASRC_SH					"SH"		//!< datasource "Stammhaus"
#define DATASRC_MW					"MW"		//!< datasource "Mandatslager" ("Mandate Warehouse") = Stammhaus
#define DATASRC_CL					"CL"		//!< datasource "Clearing"
#define DATASRC_UW					"UW"		//!< datasource "Überweiser"
#define DATASRC_UE					"UE"		//!< datasource "Überweiser"
#define DATASRC_ER					"ER"		//!< datasource "Ersteinrichter"
#define DATASRC_IR					"IR"		//!< datasource "Industrieretoure = WADI"
#define DATASRC_BC					"BC"		//!< datasource "Batch Callback" = "Chargenrueckrufe"
#define DATASRC_DI					"DI"		//!< datasource "Disposition", z.B. für reine Abbuchung im Zentrallager BG
#define DATASRC_VU					"VU"		//!< datasource "Verbundumlistungen"

#define ORDERTYPE_UW "UW"				// Ordertype UW - transfer order
#define ORDERTYPE_UN "UN"               // Ordertype UN - PRISMA - Netto/Netto order
#define ORDERTYPE_MD "MD"               // Ordertype MD - HAGEDA special UW order
#define ORDERTYPE_OM "OM"				// Ordertype OM - OMG order
#define ORDERTYPE_HZ "HZ"
#define ORDERTYPE_AK "AK"
#define ORDERTYPE_DG "DG"				// Ordertype DG (Dienstleistergeschäft) for example CMI orders from PharmaMall
#define ORDERTYPE_GM "GM"				// Grippemittel

#define ORDERRELATIONNO_TYPE_PURCHASEORDER 1


namespace CSC_Batches {
	enum StatusEnum {
	        ZU = 0,						//!< status "zurückgestellt"
		    FE = 1,						//!< status "freigegeben"
			ER = 2,						//!< status "erledigt"
			ST = 9						//!< status "stormiert"
	};  // StatusEnum
}// namespace


enum CountryEnum
{
	COUNTRY_UNKNOWN,
	COUNTRY_AUSTRIA,
	COUNTRY_BULGARIA,
	COUNTRY_CROATIA,
	COUNTRY_GERMANY,
	COUNTRY_FRANCE,
	COUNTRY_SERBIA,
	COUNTRY_SWITZERLAND
};

#endif	// GUARD_LIBCSCBATCH_DEFINITIONS_H
