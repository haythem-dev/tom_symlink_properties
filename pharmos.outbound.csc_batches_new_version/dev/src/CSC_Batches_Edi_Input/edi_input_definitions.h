//--------------------------------------------------------------------------------------------//
/*! \file	libabbauw_definitions.h     
 *  \brief	Definitions necessary for libAbbauw     
 *  \author	Petra Kessling  
 *  \date   07.05.13    
 *  \version    
 *  history 07.05.13 pk new
 */ 
//--------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBABBAUW_DEFINITIONS_H
#define GUARD_LIBABBAUW_DEFINITIONS_H

//--------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------//
#include <libbasarcmnutil_bstring.h> 

//--------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------//
namespace libabbauw
{
	namespace domMod
	{
		namespace lit
		{

		// 
		static const basar::I18nString STATUS_DEFERRED						= "0";		// "0" = "ZU" - "zurueckgestellt"
		static const basar::I18nString STATUS_APPROVED						= "1";		// "1" = "FE" - "freigegeben"
		static const basar::I18nString STATUS_DONE							= "2";		// "2" = "ER" - "erledigt"
		static const basar::I18nString STATUS_CANCELLED						= "9";		// "9" = "ST" - "storniert"

		static const basar::I18nString OMG_ORDERTYPE						= "OM";
		static const basar::I18nString OMG_ORDERPICKINGTYPE					= "2";
		static const basar::I18nString OMG_BOOKENTRYTYPE					= "2";
		static const basar::I18nString OMG_CUSTOMERGROUP					= "18";

		static const basar::I18nString UW_ORDERTYPE							= "UW";
		static const basar::I18nString UW_ORDERPICKINGTYPE					= "0";
		static const basar::I18nString UW_BOOKENTRYTYPE						= "0";

		static const basar::I18nString DG_ORDERTYPE							= "DG";
		static const basar::I18nString DG_ORDERPICKINGTYPE					= "0";
		static const basar::I18nString DG_BOOKENTRYTYPE						= "3";

		static const basar::Int16		ARTICLE_CODE_TYPE_EAN				= 13;
		static const basar::Int16		ARTICLE_CODE_TYPE_GTIN				= 8;
		static const basar::Int16		ARTICLE_CODE_TYPE_PZN7				= 11;
		static const basar::Int16		ARTICLE_CODE_TYPE_PZN8				= 16;

		static const basar::UInt32		ARTICLE_CODE_TYPE_PZN7_LENGTH		= 7;
		static const basar::UInt32		ARTICLE_CODE_TYPE_PZN8_LENGTH		= 8;
		static const basar::UInt32		ARTICLE_CODE_TYPE_EAN_LENGTH		= 13;
		static const basar::UInt32		ARTICLE_CODE_TYPE_GTIN_LENGTH		= 14;


		// ToDo kes:	specific to CH. Change to a more programmatic approach. Then these values are going to disappear.
		static const basar::I18nString	EAN_BRANCHNO_AMEDIS_UNTERENTFELDEN	= "7601001358034 ";
		static const basar::I18nString	EAN_BRANCHNO_AMEDIS_PUIDOUX			= "7601001000209 ";
		static const basar::Int16		BRANCHNO_AMEDIS_UNTERENTFELDEN		= 12;
		static const basar::Int16		BRANCHNO_AMEDIS_PUIDOUX				= 13;

		// for PBS993 - special case 1: Novartis/Wander:
		static const basar::I18nString	EAN_BRANCHNO_AMEDIS_UNTERENTFELDEN_SPECIAL	= "7601001366923 ";
		static const basar::I18nString	EAN_BRANCHNO_AMEDIS_PUIDOUX_SPECIAL			= "7601001368248 ";
		static const basar::I18nString	EAN_BRANCHNO_AMEDIS_BRANCH14_SPECIAL		= "7601001366930 ";

		// for PBS993 - special case 2: NADMF:
		static const basar::I18nString	EAN_MANUFACTURERNO_SPECIAL_SSLHEALTHCARE		= "4006671000000 ";
		static const basar::I18nString	EAN_MANUFACTURERNO_SPECIAL_3MPROFESSIONAL		= "7610182007044 ";
		static const basar::I18nString	EAN_MANUFACTURERNO_SPECIAL_3MCONSUMERCARE		= "7610182000007 ";
		static const basar::I18nString	EAN_MANUFACTURERNO_SPECIAL_SANOFISYNTHLABO		= "7601001002340 ";
		static const basar::I18nString	EAN_MANUFACTURERNO_SPECIAL_VIFORSAFRIBOURG		= "7601001001862 ";
		static const basar::I18nString	EAN_MANUFACTURERNO_SPECIAL_PHARMASINGER			= "7601001318700 ";
		static const basar::I18nString	EAN_MANUFACTURERNO_SPECIAL_ZAMBONSCWEIZAG		= "7601001000872 ";
		static const basar::I18nString	EAN_MANUFACTURERNO_SPECIAL_INSTITUTBIOCHIMIQUE	= "7601001000834 ";

		static const basar::Int32	MANUFACTURERNO_SPECIAL_SSLHEALTHCARE			= 445;
		static const basar::Int32	MANUFACTURERNO_SPECIAL_3MPROFESSIONAL			= 1292;
		static const basar::Int32	MANUFACTURERNO_SPECIAL_3MCONSUMERCARE			= 3156;
		static const basar::Int32	MANUFACTURERNO_SPECIAL_SANOFISYNTHLABO			= 408 ;
		static const basar::Int32	MANUFACTURERNO_SPECIAL_VIFORSAFRIBOURG			= 485;
		static const basar::Int32	MANUFACTURERNO_SPECIAL_PHARMASINGER				= 485;	
		static const basar::Int32	MANUFACTURERNO_SPECIAL_ZAMBONSCWEIZAG			= 219;
		static const basar::Int32	MANUFACTURERNO_SPECIAL_INSTITUTBIOCHIMIQUE		= 54;

		// for PBS993 - special case 3: NADBY:
		static const basar::Int32	PHARMACYNO_SPECIAL_PSEUDO						= 56265;	// "Unknown pharmacyno for UW"

		// Textprefix for headertext on invoice
		// kes, 24.10.14: Save space in invoice header text . orig: static const basar::I18nString INVOICE_TEXT_UW			= "*UEWAUF FROM ";
		static const basar::I18nString INVOICE_TEXT_UW			= "*UEWAUF ";				// '*'means: This is an order from EDIFact. Without '*': Order was manually entered.
		static const basar::I18nString INVOICE_TEXT_CUST_UNKNOWN= "*UNKNOWN EAN FOR ";
		static const basar::I18nString INVOICE_TEXT_UNKNOWN		= "??? ";


		}
	} // end namespace domMod
}

#endif // GUARD_EDITRANSFERORDERMAPPER_DEFINITIONS_H
