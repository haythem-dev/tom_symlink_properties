// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/tender/tenderpositionsacc.h>
#include <infrastructure/accessor/tender/tenderpositionsacc_definitions.h>

namespace libtender
{
namespace infrastructure
{
namespace accessor
{
namespace tender
{
BEGIN_ACCESSOR_DEFINITION( lit::TENDER_POSITIONS_ACC )

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_NO			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_STATE			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NO			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_CODE			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NAME			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CONTRACT_QTY			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CONTRACT_PRICE		)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::AEP					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::AGP					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::GEP					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PRICE_INCL_VAT		)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::ADD_DISCOUNT_PCT		)
	    PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNT_CALC_FROM   )
	    PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNT_APPLY_TO    )
	    PROPERTY_DESCRIPTION_LIST_ADD( properties::REMAINING_QUANTITY	)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::OWNQUOTA_FLAG		)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::OWNQUOTA_QTY			)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::RECALL_FLAG			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY_AVAILABLE	)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::QUANTITY_ALLOCATED	)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::NAME_OF_USER			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::POSITION_STATUS		)
	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD( accessor::tender::lit::SELECT_TENDER_POSITIONS_BY_KEY_FR );
		SQL_BUILDER_CREATE( SelectTenderPositionsByKeyFR )
		SQL_BUILDER_PUSH_BACK( SelectTenderPositionsByKeyFR )

	ACCESS_METHOD( accessor::tender::lit::SELECT_TENDER_POSITIONS_BY_KEY_RS );
		SQL_BUILDER_CREATE( SelectTenderPositionsByKeyRS )
		SQL_BUILDER_PUSH_BACK( SelectTenderPositionsByKeyRS )

	ACCESS_METHOD( accessor::tender::lit::SELECT_TENDER_POSITIONS_BY_KEY_BG );
		SQL_BUILDER_CREATE( SelectTenderPositionsByKeyBG )
		SQL_BUILDER_PUSH_BACK( SelectTenderPositionsByKeyBG )

	ACCESS_METHOD( accessor::tender::lit::SELECT_TENDER_POSITIONS_BY_KEY_CH );
		SQL_BUILDER_CREATE( SelectTenderPositionsByKeyCH )
		SQL_BUILDER_PUSH_BACK( SelectTenderPositionsByKeyCH )

	ACCESS_METHOD( accessor::tender::lit::SAVE_TENDER_POSITION )
		SQL_BUILDER_CREATE( DeleteTenderPosition )
		SQL_BUILDER_PUSH_BACK( DeleteTenderPosition )
		SQL_BUILDER_CREATE( UpdateTenderPosition )
		SQL_BUILDER_PUSH_BACK( UpdateTenderPosition )
		SQL_BUILDER_CREATE( InsertTenderPosition )
		SQL_BUILDER_PUSH_BACK( InsertTenderPosition )

END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelectTenderPositionsByKeyCH )

bool SelectTenderPositionsByKeyCH::isExecutable() const
{
	METHODNAME_DEF( SelectTenderPositionsByKeyCH, isExecutable );
	static const log4cplus::Logger & logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = arePropertiesSet( properties::TENDER_NO.toPropertyString() );

	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

	return executable;
}

void SelectTenderPositionsByKeyCH::buildSQLString()
{
	using namespace properties;
	using basar::VarString;

	// to catch hold of a little bit readability
	static const VarString& tenderid = TENDER_NO.getName();
	static const VarString& articleno = ARTICLE_NO.getName();
	static const VarString& articlecode = ARTICLE_CODE.getName();
	static const VarString& articlename = ARTICLE_NAME.getName();
	static const VarString& contractqty = CONTRACT_QTY.getName();
	static const VarString& contractprice = CONTRACT_PRICE.getName();
	static const VarString& adddiscountpct = ADD_DISCOUNT_PCT.getName();
	static const VarString& discountcalcfrom = DISCOUNT_CALC_FROM.getName();
	static const VarString& discountapplyto = DISCOUNT_APPLY_TO.getName();
	static const VarString& nameofuser = NAME_OF_USER.getName();
    static const VarString& ownquotaqty = OWNQUOTA_QTY.getName();
    static const VarString& ownqtyflag = OWNQUOTA_FLAG.getName();
    static const VarString& recallflag = RECALL_FLAG.getName();
    static const VarString& tenderstate = TENDER_STATE.getName();
    static const VarString& quantityallocated = QUANTITY_ALLOCATED.getName();
    static const VarString& remainingquantity = REMAINING_QUANTITY.getName();
	static const VarString& pricegep = GEP.getName();
	static const VarString& priceagp = AGP.getName();
	static const VarString& posstate = POSITION_STATUS.getName();

	static const VarString sql_tenderid( TENDER_NO.toSQLReplacementString() );

	static const basar::VarString constSQL(
		"SELECT "
		"tpos.tenderno AS "              + tenderid + ", "
		"tpos.articleno AS "             + articleno + ", "
		"tpos.articlecode AS "           + articlecode + ", "
		"NVL(az.artikel_name,'-UNKNOWN-') AS " + articlename + ", "
		"tpos.contractqty AS "           + contractqty + ", "
		"tpos.contractprice AS "         + contractprice + ", "
		"tpos.adddiscountpct AS "        + adddiscountpct + ", "
		"tpos.discountcalcfrom AS "      + discountcalcfrom + ", "
		"tpos.discountapplyto AS "       + discountapplyto + ", "
		"tpos.nameofuser AS "            + nameofuser + ", "
		"tpos.ownquotaqty AS "           + ownquotaqty + ", " 
		"tpos.ownquotaflag AS "          + ownqtyflag + ", " 
		"tpos.recallflag AS "            + recallflag + ", " 
		"tpos.tenderstate::SMALLINT AS " + tenderstate + ", "
		"0 AS "                          + quantityallocated + ", "
		"CASE WHEN tpos.ownquotaflag = 1 then "
		"    CAST( tpos.contractqty - tpos.ownquotaqty + SUM( NVL( tprog.returnqty, 0 ) ) - SUM( NVL( tprog.deliveryqty, 0 ) ) AS INT ) "
		"ELSE "
		"    CAST( tpos.contractqty + SUM( NVL( tprog.returnqty, 0 ) ) - SUM( NVL( tprog.deliveryqty, 0 ) ) AS INT ) "
		"END AS " + remainingquantity + ", "
		"NVL(ap.preisekgrosso,0.0) AS " + pricegep + ", "
		"NVL(ap.preisagp, 0.0) AS " + priceagp + ", "
		"'' AS " + posstate + " "

		"FROM tenderpos tpos "
		"LEFT OUTER JOIN artikelzentral az ON az.artikel_nr=tpos.articleno "
		"LEFT OUTER JOIN artikelpreis ap ON ap.artikel_nr=tpos.articleno "
		"LEFT OUTER JOIN tenderprogress tprog ON tprog.tenderno = tpos.tenderno and tprog.articleno = tpos.articleno " 

		"WHERE "
		"(tpos.tenderno = " + sql_tenderid + ") "

		"GROUP BY "
		"tpos.tenderno, tpos.articleno, tpos.articlecode, articlename, tpos.contractqty, tpos.contractprice, "
		"tpos.adddiscountpct, tpos.nameofuser, tpos.ownquotaqty, tpos.ownquotaflag, tpos.recallflag, "
		"tpos.discountcalcfrom, tpos.discountapplyto, tenderstate, quantityallocated, " + pricegep + ", " + priceagp + " "
	);

	resolve( constSQL );
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( SelectTenderPositionsByKeyFR )

bool SelectTenderPositionsByKeyFR::isExecutable() const
{
	METHODNAME_DEF( SelectTenderPositionsByKeyFR, isExecutable );
	static const log4cplus::Logger & logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = arePropertiesSet( properties::TENDER_NO.toPropertyString() );

	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

	return executable;
}

void SelectTenderPositionsByKeyFR::buildSQLString()
{
	using namespace properties;
	using basar::VarString;

	static const VarString & tenderid			= TENDER_NO.getName();
	static const VarString & articleno			= ARTICLE_NO.getName();
	static const VarString & articlecode		= ARTICLE_CODE.getName();
	static const VarString & articlename		= ARTICLE_NAME.getName();
	static const VarString & contractqty		= CONTRACT_QTY.getName();
	static const VarString & contractprice		= CONTRACT_PRICE.getName();
	static const VarString & prince_incl_vat	= PRICE_INCL_VAT.getName();
	static const VarString & adddiscountpct		= ADD_DISCOUNT_PCT.getName();
	static const VarString & tenderstate		= TENDER_STATE.getName();
	static const VarString & nameofuser			= NAME_OF_USER.getName();
	static const VarString & ownquotaqty		= OWNQUOTA_QTY.getName();
    static const VarString & ownqtyflag			= OWNQUOTA_FLAG.getName();
    static const VarString & recallflag			= RECALL_FLAG.getName();

	static const VarString sql_tenderid( TENDER_NO.toSQLReplacementString() );

	static const basar::VarString constSQL(
		"SELECT "
		"tp.tenderno										AS " + tenderid					+ ", "
		"tp.articleno										AS " + articleno				+ ", "
		"tp.articlecode										AS " + articlecode				+ ", "
		"NVL(az.artikel_name,'-UNKNOWN-')					AS " + articlename				+ ", "
		"tp.contractqty										AS " + contractqty				+ ", "
		"tp.contractprice									AS " + contractprice			+ ", "
		"round(tp.contractprice*(1+tr.salestaxrate/100),2)	AS " + prince_incl_vat          + ", "
		"ap.preisekapo										AS " + AEP.getName()			+ ", "
		"tp.adddiscountpct									AS " + adddiscountpct			+ ", "
		"tp.tenderstate::SMALLINT							AS " + tenderstate				+ ", "
		"tp.nameofuser										AS " + nameofuser				+ ", "
		"tp.ownquotaqty										AS " + ownquotaqty				+ ", " 
		"tp.ownquotaflag									AS " + ownqtyflag				+ ", " 
		"tp.recallflag										AS " + recallflag				+ " " 

		"FROM tenderpos tp "
		"LEFT OUTER JOIN artikelzentral az ON az.artikel_nr	= tp.articleno "
		"LEFT OUTER JOIN artikelpreis   ap ON ap.artikel_nr = tp.articleno AND preis_typ=0 "
		"LEFT OUTER JOIN taxrates		tr ON tr.taxlevel	= az.taxlevel  "

		"WHERE "
		"(tp.tenderno = " + sql_tenderid + ") "
			);

	resolve( constSQL );
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

bool SelectTenderPositionsByKeyRS::isExecutable() const
{
	METHODNAME_DEF( SelectTenderPositionsByKeyRS, isExecutable );
	static const log4cplus::Logger & logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = arePropertiesSet( properties::TENDER_NO.toPropertyString() );

	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

	return executable;
}

BUILDER_DEFINITION( SelectTenderPositionsByKeyRS )
void SelectTenderPositionsByKeyRS::buildSQLString()
{
	using namespace properties;
	using basar::VarString;

	static const VarString & tenderid			= TENDER_NO.getName();
	static const VarString & articleno			= ARTICLE_NO.getName();
	static const VarString & articlecode		= ARTICLE_CODE.getName();
	static const VarString & articlename		= ARTICLE_NAME.getName();
	static const VarString & contractqty		= CONTRACT_QTY.getName();
	static const VarString & contractprice		= CONTRACT_PRICE.getName();
	static const VarString & prince_incl_vat	= PRICE_INCL_VAT.getName();
	static const VarString & adddiscountpct		= ADD_DISCOUNT_PCT.getName();
	static const VarString & tenderstate		= TENDER_STATE.getName();
	static const VarString & nameofuser			= NAME_OF_USER.getName();
	static const VarString & ownquotaqty		= OWNQUOTA_QTY.getName();
    static const VarString & ownqtyflag			= OWNQUOTA_FLAG.getName();
    static const VarString & recallflag			= RECALL_FLAG.getName();

	static const VarString sql_tenderid( TENDER_NO.toSQLReplacementString() );

	static const basar::VarString constSQL(
		"SELECT "
		"tp.tenderno										AS " + tenderid					+ ", "
		"tp.articleno										AS " + articleno				+ ", "
		"tp.articlecode										AS " + articlecode				+ ", "
		"NVL(az.artikel_name,'-UNKNOWN-')					AS " + articlename				+ ", "
		"tp.contractqty										AS " + contractqty				+ ", "
		"tp.contractprice									AS " + contractprice			+ ", "
		"round(tp.contractprice*(1+tr.salestaxrate/100),2)	AS " + prince_incl_vat          + ", "
		"ap.preisekapo										AS " + AEP.getName()			+ ", "
		"tp.adddiscountpct									AS " + adddiscountpct			+ ", "
		"tp.tenderstate::SMALLINT							AS " + tenderstate				+ ", "
		"tp.nameofuser										AS " + nameofuser				+ ", "
		"tp.ownquotaqty										AS " + ownquotaqty				+ ", " 
		"tp.ownquotaflag									AS " + ownqtyflag				+ ", " 
		"tp.recallflag										AS " + recallflag				+ " " 

		"FROM tenderpos tp "
		"LEFT OUTER JOIN artikelzentral az ON az.artikel_nr	= tp.articleno "
		"LEFT OUTER JOIN artikelpreis   ap ON ap.artikel_nr = tp.articleno AND preis_typ=0 "
		"LEFT OUTER JOIN taxrates		tr ON tr.taxlevel	= az.taxlevel  "

		"WHERE "
		"(tp.tenderno = " + sql_tenderid + ") ORDER BY articlename ASC"
			);

	resolve( constSQL );
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}

BUILDER_DEFINITION( SelectTenderPositionsByKeyBG )
bool SelectTenderPositionsByKeyBG::isExecutable() const
{
	METHODNAME_DEF( SelectTenderPositionsByKeyBG, isExecutable );
	static const log4cplus::Logger & logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = arePropertiesSet( properties::TENDER_NO.toPropertyString() );

	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );

	return executable;
}

void SelectTenderPositionsByKeyBG::buildSQLString()
{
	using namespace properties;
	using basar::VarString;

	static const VarString & tenderid			= TENDER_NO.getName();
	static const VarString & articleno			= ARTICLE_NO.getName();
	static const VarString & articlecode		= ARTICLE_CODE.getName();
	static const VarString & articlename		= ARTICLE_NAME.getName();
	static const VarString & contractqty		= CONTRACT_QTY.getName();
	static const VarString & contractprice		= CONTRACT_PRICE.getName();
	static const VarString & price_incl_vat	    = PRICE_INCL_VAT.getName();
	static const VarString & adddiscountpct		= ADD_DISCOUNT_PCT.getName();
	static const VarString & tenderstate		= TENDER_STATE.getName();
	static const VarString & nameofuser			= NAME_OF_USER.getName();
	static const VarString & ownquotaqty		= OWNQUOTA_QTY.getName();
    static const VarString & ownqtyflag			= OWNQUOTA_FLAG.getName();
    static const VarString & recallflag			= RECALL_FLAG.getName();
	static const VarString & tenderno			= TENDER_NO.toSQLReplacementString();

	static const basar::VarString constSQL(
		"SELECT "
		"tp.tenderno										AS " + tenderid					+ ", "
		"tp.articleno										AS " + articleno				+ ", "
		"tp.articlecode										AS " + articlecode				+ ", "
		"NVL(az.artikel_name,'-UNKNOWN-')					AS " + articlename				+ ", "
		"tp.contractqty										AS " + contractqty				+ ", "
		"tp.contractprice									AS " + contractprice			+ ", "
		"round(tp.contractprice*(1+tr.salestaxrate/100),2)	AS " + price_incl_vat           + ", "
		"round(ap.preisekapo*(1+tr.salestaxrate/100),2)		AS " + AEP.getName()			+ ", "
		"tp.adddiscountpct									AS " + adddiscountpct			+ ", "
		"tp.tenderstate::SMALLINT							AS " + tenderstate				+ ", "
		"tp.nameofuser										AS " + nameofuser				+ ", "
		"tp.ownquotaqty										AS " + ownquotaqty				+ ", " 
		"tp.ownquotaflag									AS " + ownqtyflag				+ ", " 
		"tp.recallflag										AS " + recallflag				+ " " 
		
		"FROM tenderpos tp "
		"LEFT OUTER JOIN artikelzentral az ON az.artikel_nr	= tp.articleno "
		"LEFT OUTER JOIN artikelpreis   ap ON ap.artikel_nr = tp.articleno AND preis_typ=0 "
		"LEFT OUTER JOIN taxrates		tr ON tr.taxlevel	= az.taxlevel  "

		"WHERE "
		"(tp.tenderno = " + tenderno + ") "
		);

	resolve( constSQL );
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}


BUILDER_DEFINITION( DeleteTenderPosition )
	
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( DeleteTenderPosition,  
	properties::TENDER_NO.toPropertyString() +
	properties::ARTICLE_NO.toPropertyString(),

	properties::TENDER_NO.toPropertyString() +
	properties::ARTICLE_NO.toPropertyString(),

	basar::SS_DELETE )

void DeleteTenderPosition::buildSQLString()
{
	using namespace properties;
	using basar::VarString;

	// to catch hold of a little bit readability
	static const VarString sql_tenderid( TENDER_NO.toSQLReplacementString() );
	static const VarString sql_articleno( ARTICLE_NO.toSQLReplacementString() );

	static const VarString sqlStatement( 
		"DELETE tenderpos "
		"WHERE "
		"(tenderno = " + sql_tenderid + ") "
		"AND "
		"(articleno = " + sql_articleno + ") "
			);

	resolve( sqlStatement );
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}


BUILDER_DEFINITION( UpdateTenderPosition )
	
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( UpdateTenderPosition,  
	properties::TENDER_NO.toPropertyString() +
	properties::TENDER_STATE.toPropertyString() +
	properties::ARTICLE_NO.toPropertyString() +
	properties::CONTRACT_QTY.toPropertyString() +
	properties::CONTRACT_PRICE.toPropertyString() +
	properties::ADD_DISCOUNT_PCT.toPropertyString() +
	properties::DISCOUNT_CALC_FROM.toPropertyString() +
	properties::DISCOUNT_APPLY_TO.toPropertyString() +
	properties::RECALL_FLAG.toPropertyString() +
	properties::OWNQUOTA_QTY.toPropertyString() +
	properties::OWNQUOTA_FLAG.toPropertyString(),

	properties::TENDER_NO.toPropertyString() +
	properties::TENDER_STATE.toPropertyString() +
	properties::ARTICLE_NO.toPropertyString() +
	properties::CONTRACT_QTY.toPropertyString() +
	properties::CONTRACT_PRICE.toPropertyString() +
	properties::ADD_DISCOUNT_PCT.toPropertyString() +
	properties::DISCOUNT_CALC_FROM.toPropertyString() +
	properties::DISCOUNT_APPLY_TO.toPropertyString() +
	properties::RECALL_FLAG.toPropertyString() +
    properties::OWNQUOTA_QTY.toPropertyString() +
	properties::OWNQUOTA_FLAG.toPropertyString(),

	basar::SS_UPDATE )

void UpdateTenderPosition::buildSQLString()
{
	using namespace properties;
	using basar::VarString;

	// to catch hold of a little bit readability
	static const VarString sql_tenderid( TENDER_NO.toSQLReplacementString() );
	static const VarString sql_tenderstate( TENDER_STATE.toSQLReplacementString() );
	static const VarString sql_articleno( ARTICLE_NO.toSQLReplacementString() );
	static const VarString sql_contractqty( CONTRACT_QTY.toSQLReplacementString() );
	static const VarString sql_contractprice( CONTRACT_PRICE.toSQLReplacementString() );
	static const VarString sql_adddiscountpct( ADD_DISCOUNT_PCT.toSQLReplacementString() );
	static const VarString sql_discountcalcfrom( DISCOUNT_CALC_FROM.toSQLReplacementString() );
	static const VarString sql_discountapplyto( DISCOUNT_APPLY_TO.toSQLReplacementString() );
	static const VarString sql_nameofuser( NAME_OF_USER.toSQLReplacementString() );
    static const VarString sql_ownquotaqty( OWNQUOTA_QTY.toSQLReplacementString() );
    static const VarString sql_ownqtyflag( OWNQUOTA_FLAG.toSQLReplacementString() );
	static const VarString sql_recallflag( RECALL_FLAG.toSQLReplacementString() );

	static const VarString sqlStatement( 
		"UPDATE tenderpos "
		"SET "
        "tenderstate = '"										+ sql_tenderstate		+ "', "
		"contractqty = "										+ sql_contractqty		+ ", "
		"contractprice = "										+ sql_contractprice		+ ", "
		"adddiscountpct = "										+ sql_adddiscountpct	+ ", "
		"discountcalcfrom = "                                   + sql_discountcalcfrom  + ", "
		"discountapplyto = "                                    + sql_discountapplyto   + ", "
		"changedate = TO_CHAR(CURRENT, '%Y%m%d')::INTEGER "								+ ", "
        "nameofuser = '"										+ sql_nameofuser		+ "', "
        "recallflag = "											+ sql_recallflag        + ", " 
        "ownquotaqty = "										+ sql_ownquotaqty       + ", "
        "ownquotaflag = "										+ sql_ownqtyflag        + " "
		"WHERE "
		"tenderno = "											+ sql_tenderid			+ " AND "
		"articleno = "											+ sql_articleno
		);

	resolve( sqlStatement );
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}


BUILDER_DEFINITION( InsertTenderPosition )
	
ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE( InsertTenderPosition,  
	properties::TENDER_NO.toPropertyString() +
	properties::ARTICLE_NO.toPropertyString() +
	properties::ARTICLE_CODE.toPropertyString() +
	properties::CONTRACT_QTY.toPropertyString() +
	properties::CONTRACT_PRICE.toPropertyString() +
	properties::ADD_DISCOUNT_PCT.toPropertyString() +
	properties::DISCOUNT_CALC_FROM.toPropertyString() +
	properties::DISCOUNT_APPLY_TO.toPropertyString() +
	properties::OWNQUOTA_QTY.toPropertyString() +
	properties::OWNQUOTA_FLAG.toPropertyString(),

	properties::TENDER_NO.toPropertyString() +
	properties::ARTICLE_NO.toPropertyString() +
	properties::ARTICLE_CODE.toPropertyString() +
	properties::CONTRACT_QTY.toPropertyString() +
	properties::CONTRACT_PRICE.toPropertyString() +
	properties::ADD_DISCOUNT_PCT.toPropertyString() +
	properties::DISCOUNT_CALC_FROM.toPropertyString() +
	properties::DISCOUNT_APPLY_TO.toPropertyString() +
	properties::RECALL_FLAG.toPropertyString() +
    properties::OWNQUOTA_QTY.toPropertyString() +
	properties::OWNQUOTA_FLAG.toPropertyString(),

	basar::SS_INSERT )

void InsertTenderPosition::buildSQLString()
{
	using namespace properties;
	using basar::VarString;

	// to catch hold of a little bit readability
	static const VarString sql_tenderid( TENDER_NO.toSQLReplacementString() );
	static const VarString sql_tenderstate( TENDER_STATE.toSQLReplacementString() );
	static const VarString sql_articleno( ARTICLE_NO.toSQLReplacementString() );
	static const VarString sql_articlecode( ARTICLE_CODE.toSQLReplacementString() );
	static const VarString sql_contractqty( CONTRACT_QTY.toSQLReplacementString() );
	static const VarString sql_contractprice( CONTRACT_PRICE.toSQLReplacementString() );
	static const VarString sql_adddiscountpct( ADD_DISCOUNT_PCT.toSQLReplacementString() );
	static const VarString sql_discountcalcfrom( DISCOUNT_CALC_FROM.toSQLReplacementString() );
	static const VarString sql_discountapplyto( DISCOUNT_APPLY_TO.toSQLReplacementString() );
	static const VarString sql_nameofuser( NAME_OF_USER.toSQLReplacementString() );
    static const VarString sql_ownquotaqty( OWNQUOTA_QTY.toSQLReplacementString() );
    static const VarString sql_ownqtyflag( OWNQUOTA_FLAG.toSQLReplacementString() );
    static const VarString sql_recallflag( RECALL_FLAG.toSQLReplacementString() );

	static const VarString sqlStatement( 
		"INSERT INTO tenderpos ( "
		"tenderno, "
		"tenderstate, "
		"articleno, "
		"articlecode, "
		"contractqty, "
		"contractprice, "
		"adddiscountpct, "
		"discountcalcfrom, "
		"discountapplyto, "
		"changedate, "
		"nameofuser, "
        "recallflag, "
        "ownquotaqty,"
        "ownquotaflag, "
        "creationdate "
		") "
		"VALUES ( "
		+ sql_tenderid +		", "
		+ sql_tenderstate +		", "
		+ sql_articleno +		", "
		+ sql_articlecode +		", "
		+ sql_contractqty +		", "
		+ sql_contractprice +	", "
		+ sql_adddiscountpct +	", "
		+ sql_discountcalcfrom + ", "
		+ sql_discountapplyto + ", "
		"TO_CHAR(TODAY, '%Y%m%d')::INTEGER, "
		"'" + sql_nameofuser + "', "
        + sql_recallflag +      ", "
        + sql_ownquotaqty +     ", "
        + sql_ownqtyflag +      ", "
		"TO_CHAR(TODAY, '%Y%m%d')::INTEGER "
		") "
		);

	resolve( sqlStatement );
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}


} // end namespace tender
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender
