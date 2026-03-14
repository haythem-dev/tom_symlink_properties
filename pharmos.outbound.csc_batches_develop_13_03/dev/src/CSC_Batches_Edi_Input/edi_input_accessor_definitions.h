//----------------------------------------------------------------------------
/*! \file libabbauw_accessor_definitions.h
 *  \brief  global definitions for database access of abba-uw: Accessors, Accessor-Methods, Accessor Instances
*  \author P. Kessling, M. Meier
*  \date   07.05.2013 
*  history: 
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBABBAUW_ACCESSOR_DEFINITIONS_H
#define GUARD_LIBABBAUW_ACCESSOR_DEFINITIONS_H

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
	namespace domMod
	{
		basar::ConstString NAMESPACE_NAME( "libabbauw::domMod" );

		// Vorschlag: basar::ConstString NAMESPACE_NAME( "libabbauw::accessors" );
		// old:		  basar::ConstString NAMESPACE_NAME( "dm_abbauw::lit" );


//-------------------------------------------------------------------------------------------------//
// const objects definition section
//-------------------------------------------------------------------------------------------------//

		// NAMES FOR ACCESSOR INSTANCES, THEIR ACCESSORS AND THEIR ACCESSOR METHODS ..
		// .. for ACC_ORDERHEAD
		const basar::VarString ACC_INST_ORDERHEAD		( "AccOrderheadInstance" );						// accessor instance
		const basar::VarString ACC_ORDERHEAD			( "AccOrderhead" );								// accessor
		const basar::VarString ACCMETH_SELECT_MAX_ORDERNO_BY_BRANCHNO	( "SelMaxOrdernoByBranchno" );	// accessor method
		const basar::VarString ACCMETH_INSERT_INTO_ORDERHEAD			( "InsertIntoOrderhead" );		// accessor method

		// .. for ACC_ORDERPOS
		const basar::VarString ACC_INST_ORDERPOS		( "AccOrderposInstance" );			// accessor instance
		const basar::VarString ACC_ORDERPOS				( "AccOrderpos" );					// accessor
		const basar::VarString ACCMETH_INSERT_INTO_ORDERPOS	( "InsertIntoOrderpos" );		// accessor method


		// .. for ACC_ARTICLE
		const basar::VarString ACC_INST_ARTICLE			( "AccArticleInstance" );	// accessor instance
		const basar::VarString ACC_ARTICLE				( "AccArticle" );			// accessor
		const basar::VarString ACCMETH_SELECT_MANUFACTURERNO_BY_ARTICLENO			( "SelManufacturernoByArticleno" );				// accessor method
		const basar::VarString ACCMETH_SELECT_ARTICLENO_INTERN_BY_ARTICLENOMF		( "SelArticlenoInternByArticleMFBuilder" );		// accessor method
		const basar::VarString ACCMETH_SELECT_WHOLESALERPURCHASEPRICE_BY_ARTICLENO	( "SelWholesalerpurchasepriceByArticleno" );	// accessor method
		const basar::VarString ACCMETH_SELECT_PREFERRED_ARTICLECODE_BY_ARTICLECODE_AND_CODE_TYPE	( "SelPreferredArticleCodeByArticleCodeAndCodeType" );				// accessor method
		const basar::VarString ACCMETH_SELECT_PREFERRED_ARTICLENO_BY_PREFERRED_ARTICLECODE			( "SelPreferredArticlenoByPreferredArticleCode" );				// accessor method
		const basar::VarString ACCMETH_SELECT_PREFERRED_ARTICLECODE_BY_ARTICLENO_INTERN				( "SelPreferredArticleCodeByArticlenoInternBuilder" );				// accessor method
		const basar::VarString ACCMETH_SELECT_CMI_ARTICLES("SelCMIArticlesBuilder");				// accessor method


		// .. for ACC_CUSTOMER
		const basar::VarString ACC_INST_CUSTOMER	( "AccCustomerInstance" );	// accessor instance
		const basar::VarString ACC_CUSTOMER			( "AccCustomer" );			// accessor
		const basar::VarString ACCMETH_SELECT_CUSTOMERGROUP_BY_BRANCHNO_PHARMCYNO ( "SelCustomergroupByBranchnoAndPharmacynoBuilder" ); // accessor method
		const basar::VarString ACCMETH_SELECT_PHARMACYNO_BY_BRANCHNO_PHARMCYNOEAN ( "SelPharmacynoByBranchnoAndPharmacynoBuilder" ); // accessor method
		const basar::VarString ACCMETH_SELECT_PSEUDOPHARMACYNO_BY_BRANCHNO		  ( "SelPseudopharmacynoByBranchnoBuilder" ); // accessor method

		// .. for ACC_DELIVERER
		const basar::VarString ACC_INST_DELIVERER	( "AccDelivererInstance" );	// accessor instance
		const basar::VarString ACC_DELIVERER			( "AccDeliverer" );			// accessor
		const basar::VarString ACCMETH_SELECT_BRANCHNO_BY_DELIVERERBGA ( "SelBranchnoByDelivererbgaBuilder" ); // accessor method


		// .. for ACC_MANUFACTURER
		const basar::VarString ACC_INST_MANUFACTURER( "AccManufacturerInstance" );	// accessor instance
		const basar::VarString ACC_MANUFACTURER		( "AccManufacturer" );			// accessor
		const basar::VarString ACCMETH_SELECT_DELIVERERNO_BY_MANUFACTUREREAN		( "Accmeth_SelDeliverernoByManufacturerean" ); // accessor method
		const basar::VarString ACCMETH_SELECT_MANUFACTURERNO_BY_DELIVERERNO			( "Accmeth_SelManufacturernoByDelivererno" ); // accessor method
		const basar::VarString ACCMETH_SELECT_MANUFACTURERNAME_BY_MANUFACTURERNO	( "Accmeth_SelManufacturernameByManufacturerno" ); // accessor method


        // .. for ACC_ORDERHEADPROTOCOL / ACC_ORDERPOSCOLLECTIONPROTOCOL
        const basar::VarString ACC_INST_ORDERHEADPROTOCOL	        ( "AccOrderHeadProtocolInstance" ); 
        const basar::VarString ACC_ORDERHEADPROTOCOL	            ( "AccOrderHeadProtocol" );
        const basar::VarString ACC_ORDERHEADPROTOCOL_SAVE           ( "SaveOrderHeadProtocol" );    

        const basar::VarString ACC_INST_ORDERPOSCOLLECTIONPROTOCOL	( "AccOrderPosCollectionProtocolInstance" ); 
        const basar::VarString ACC_ORDERPOSCOLLECTIONPROTOCOL	    ( "AccOrderPosCollectionProtocol" );
        const basar::VarString ACC_ORDERPOSCOLLECTIONPROTOCOL_SAVE	( "SaveOrderPosCollectionProtocol" );


		
	}	// end namespace domMod
}	// end namespace libabbauw


#endif //end GUARD_LIBABBAUW_ACCESSOR_DEFINITIONS_H
