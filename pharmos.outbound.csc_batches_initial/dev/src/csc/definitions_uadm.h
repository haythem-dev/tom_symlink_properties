//----------------------------------------------------------------------------
/*! \file	definitions_uadm.h
 *  \brief  global definitions for csc_batches
 *  \author Beatrix Trömel
 *  \date   29.10.2008
 */
//----------------------------------------------------------------------------
#ifndef GUARD_DEFINITIONS_UADM_H
#define GUARD_DEFINITIONS_UADM_H

namespace CSC_Batches
{
    const char* const UADM_LOGIN_APPLNAME					= "CSC";
    const char* const UADM_USER								= "cscguiuser";
	const char* const UADM_APPL								= "CSC_BATCH_GUI";		//!< condition in useradministration (application)
	const int UADM_NO_OF_LOGIN_TRIALS						= 5;	                //!< how often can a user put in a wrong password before user account 
																					//   is deactivated
	const int UADM_MOD_UNIVERSAL							= 1;		            //!< module containing universal rights
	const char* const UADM_RIGHT_GENERAL					= "GENERAL";	        //!< userright to read in all gi dialogs
    const char* const UADM_RIGHT_UW_ACCESS					= "UW_ACCESS";
    const char* const UADM_RIGHT_UW_EDIT_ACCESS				= "UW_EDIT_ACCESS";
    const char* const UADM_RIGHT_UW_CHANGE_ACCESS			= "UW_CHANGE_ACCESS";
    const char* const UADM_RIGHT_SETTINGS_ACCESS			= "SETTINGS_ACCESS";
    const char* const UADM_RIGHT_PRIV_SETTINGS_ACCESS		= "PRIV_SETTINGS_ACCESS";
    const char* const UADM_RIGHT_UW_PURCHASE_ACCESS			= "UW_PURCHASE_ACCESS";
    const char* const UADM_RIGHT_DEVELOPER_INFO				= "DEVELOPER_INFO";
	const char* const UADM_START_APPL						= "START";				//!< parameter in useradministration (START)

	const char* const UADM_GROUP							= "GROUP"; 				//!< parameter in useradministration to read group
	const char* const UADM_HOST								= "HOST";				//!< parameter in useradministration to read server
	const char* const UADM_DB								= "DB";    				//!< parameter in useradministration to read database
	const char* const UADM_KSCSERVER						= "KSCSERVER"; 			//!< parameter in useradministration to read KSC server
	const char* const UADM_KSCPORT							= "KSCPORT"; 			//!< parameter in useradministration to read KSC port
	
	const char* const UADM_USER_MANUAL_REF					= "USER_MANUAL_REFERENCE";	//!< parameter in useradministration to read User Manual
    const char* const UADM_TENDER_CREATION_END_DATE_OFFSET	= "TENDER_CREATION_END_DATE_OFFSET";
	const char* const UADM_OMG_DURATION                      = "OMG_DURATION"; 	

} // end namespace CSC_Batches

#endif	// GUARD
