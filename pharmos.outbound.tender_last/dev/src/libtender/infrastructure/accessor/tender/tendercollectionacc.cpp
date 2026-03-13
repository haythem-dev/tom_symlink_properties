// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>
#include <infrastructure/accessor/tender/tendercollectionacc.h>
#include <infrastructure/accessor/tender/tendercollectionacc_definitions.h>

namespace libtender {
namespace infrastructure {
namespace accessor {
namespace tender {

BEGIN_ACCESSOR_DEFINITION( accessor::tender::lit::TENDER_COLLECTION_ACC )

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CONTRACT_NO					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_TYPE					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::CUSTOMER_NO					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_NAME				)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_CHAIN_NO			)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::DATE_FROM					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::DATE_TO						)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PARTNER_NO					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_NO					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::TENDER_STATE					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::NO_OF_ITEMS					)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::NO_OF_DELIVERIES				)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::NO_OF_OPEN_DELIVERIES		)
		PROPERTY_DESCRIPTION_LIST_ADD( properties::PURCHASE_ORDER_PROPOSAL_FLAG )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_GROUPID				)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::OWNQUOTA_FLAG				)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::OWNQUOTA_QTY					)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::RECALL_FLAG					)

        PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_CODE					)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::ARTICLE_NAME					)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::ADD_DISCOUNT_PCT				)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNT_CALC_FROM           )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::DISCOUNT_APPLY_TO            )
        PROPERTY_DESCRIPTION_LIST_ADD( properties::CONTRACT_QTY					)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::CHANGE_DATE					)
        //PROPERTY_DESCRIPTION_LIST_ADD( properties::CREATION_DATE				)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::CURRENT_QTY					)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::SUPPLIER_NO					)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::SUPPLIER_NAME				)
        PROPERTY_DESCRIPTION_LIST_ADD( properties::PHARMACY_LOCATION			)
	END_PROPERTY_DESCRIPTION_LIST

    ACCESS_METHOD( accessor::tender::lit::SELECT_SUCCESSOR_TENDER );
        SQL_BUILDER_CREATE( SelectSuccessorTender )
        SQL_BUILDER_PUSH_BACK( SelectSuccessorTender )
    ACCESS_METHOD( accessor::tender::lit::SELECT_TENDER_BY_ENDDATE );
        SQL_BUILDER_CREATE( SelectTenderByEndDate )
        SQL_BUILDER_PUSH_BACK( SelectTenderByEndDate )
	ACCESS_METHOD( accessor::tender::lit::SELECT_EXPIRED_TENDERS );
        SQL_BUILDER_CREATE( SelectExpiredTenders )
        SQL_BUILDER_PUSH_BACK( SelectExpiredTenders )
    ACCESS_METHOD( accessor::tender::lit::SELECT_VALID_TENDERS );
        SQL_BUILDER_CREATE( SelectValidTenders )
        SQL_BUILDER_PUSH_BACK( SelectValidTenders )

	ACCESS_METHOD( accessor::tender::lit::SELECT_TENDER_BY_PATTERN );
		// string builder will be added dynamically in component manager

END_ACCESSOR_DEFINITION


bool SelectTenderByPatternBase::isExecutable() const
{
	return doIsExecutable();
}

void SelectTenderByPatternBase::buildSQLString()
{
	doBuildSQLString();
}


// standard select by pattern
BUILDER_DEFINITION( SelectTenderByPattern )

bool SelectTenderByPattern::doIsExecutable() const
{
	METHODNAME_DEF( SelectTenderByPattern, doIsExecutable );
	static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = true;
	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
	return executable;
}

void SelectTenderByPattern::doBuildSQLString()
{
	literal(
		"SELECT FIRST 1000 DISTINCT "
			"head.tenderno AS "										+ properties::TENDER_NO.getName()			+ ", "
			"head.contractno AS "									+ properties::CONTRACT_NO.getName()			+ ", "
			"head.customerno AS "									+ properties::CUSTOMER_NO.getName()			+ ", "
			"k.nameapo AS "											+ properties::PHARMACY_NAME.getName()		+ ", "
			"head.pharmacychainno AS "								+ properties::PHARMACY_CHAIN_NO.getName()	+ ", "
			"head.tendertype AS "									+ properties::TENDER_TYPE.getName()			+ ", "
			"TO_DATE(head.datefrom::CHAR(8), '%Y%m%d')::DATE AS "	+ properties::DATE_FROM.getName()			+ ", "
			"TO_DATE(head.dateto::CHAR(8), '%Y%m%d')::DATE AS "		+ properties::DATE_TO.getName()				+ ", " +
			//"CAST( (SELECT COUNT(*) FROM tenderpos AS p where p.tenderno = head.tenderno) AS SMALLINT ) AS "
			//														+ properties::NO_OF_ITEMS.getName()			+ ", "
			//"CAST( (SELECT COUNT(*) FROM tenderdeliveryschedule AS s where s.cscorderno=0 and s.deliverydate > CURRENT and s.tenderid = head.tenderno) AS SMALLINT ) AS "
			//														+ properties::NO_OF_OPEN_DELIVERIES.getName() + ", "
			//"CAST( (SELECT COUNT(*) FROM tenderdeliveryschedule AS s where s.tenderid = head.tenderno) AS SMALLINT ) AS "
			//														+ properties::NO_OF_DELIVERIES.getName() + ", "
			"head.purchaseorderproposalflag AS "					+ properties::PURCHASE_ORDER_PROPOSAL_FLAG.getName() + " "
		"FROM "
			"tenderhead AS head " +
			"LEFT JOIN kunde as k ON head.customerno=k.idfnr AND head.branchno=k.filialnr "
	);
	if( isContainedAndSet(properties::ARTICLE_CODE) )
	{
		literal("LEFT OUTER JOIN tenderpos AS pos ON head.tenderno=pos.tenderno ");
	}

	literal("WHERE head.deletedflag=0 ");

	if( isContainedAndSet(properties::TENDER_NO) )
	{
		resolve("AND head.tenderno=" + properties::TENDER_NO.toSQLReplacementString() + " ");
	}

	if( isContainedAndSet(properties::CONTRACT_NO) )
	{
		resolve("AND UPPER(head.contractno) LIKE UPPER('" + properties::CONTRACT_NO.toSQLReplacementString() + "%') ");
	}

	if( isContainedAndSet(properties::PHARMACY_CHAIN_NO) )
	{
		resolve("AND head.pharmacychainno='" + properties::PHARMACY_CHAIN_NO.toSQLReplacementString() + "' ");
	}

	if( isContainedAndSet(properties::PARTNER_NO) )
	{
		resolve("AND head.partnerno=" + properties::PARTNER_NO.toSQLReplacementString() + " ");
	}

	if( isContainedAndSet(properties::TENDER_TYPE) )
	{
		resolve("AND head.tendertype=" + properties::TENDER_TYPE.toSQLReplacementString() + " ");
	}

	if( isContainedAndSet(properties::TENDER_STATE) )
	{
		resolve("AND head.tenderstate=" + properties::TENDER_STATE.toSQLReplacementString() + " ");
	}

	if( isContainedAndSet(properties::ARTICLE_CODE) )
	{
		resolve("AND pos.articlecode='" + properties::ARTICLE_CODE.toSQLReplacementString() + "' " );
	}

	if( isContainedAndSet(properties::CUSTOMER_NO) )
	{
		resolve("AND head.customerno=" + properties::CUSTOMER_NO.toSQLReplacementString() + " ");
	}

	if( isContainedAndSet(properties::DATE_FROM) )
	{
		resolve("AND TO_DATE(head.datefrom::CHAR(8), '%Y%m%d')::DATE>=TO_DATE('" + properties::DATE_FROM.toSQLReplacementString() + "', '%m/%d/%Y') ");
	}

	if( isContainedAndSet(properties::DATE_TO) )
	{
		resolve("AND TO_DATE(head.dateto::CHAR(8), '%Y%m%d')::DATE<=TO_DATE('" + properties::DATE_TO.toSQLReplacementString() + "', '%m/%d/%Y') ");
	}

	literal("ORDER BY head.tenderno DESC");
	BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}


// select by pattern for Switzerland
BUILDER_DEFINITION( SelectTenderByPatternCH )

bool SelectTenderByPatternCH::doIsExecutable() const
{
	METHODNAME_DEF( SelectTenderByPatternCH, doIsExecutable );
	static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
	BLOG_TRACE_METHOD( logger, fun );

	const bool executable = true;
	CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
	return executable;
}

void SelectTenderByPatternCH::doBuildSQLString()
{
    literal(
    "SELECT FIRST 1000 DISTINCT "
    "head.tenderno AS "                                         + properties::TENDER_NO.getName()              + ", "
    "head.contractno AS "                                       + properties::CONTRACT_NO.getName()            + ", "
    "head.customerno AS "                                       + properties::CUSTOMER_NO.getName()            + ", "
    "k.nameapo AS "                                             + properties::PHARMACY_NAME.getName()          + ", "
    "k.ort AS "                                                 + properties::PHARMACY_LOCATION.getName()      + ", "
    "head.pharmacychainno AS "                                  + properties::PHARMACY_CHAIN_NO.getName()      + ", "
    "head.tendertype AS "                                       + properties::TENDER_TYPE.getName()            + ", "
    "head.purchaseorderproposalflag AS "                        + properties::PURCHASE_ORDER_PROPOSAL_FLAG.getName() + ", "
    "TO_DATE(head.datefrom::CHAR(8), '%Y%m%d')::DATE AS "       + properties::DATE_FROM.getName()              + ", "
    "TO_DATE(head.dateto::CHAR(8), '%Y%m%d')::DATE AS "         + properties::DATE_TO.getName()                + ", "
    "tenpos.articlecode AS "                                    + properties::ARTICLE_CODE.getName()           + ", "
    //"tenpos.articlename AS "                                  + properties::ARTICLE_NAME.getName()           + ", "
    "az.artikel_name AS "                                       + properties::ARTICLE_NAME.getName()           + ", "
    "tenpos.adddiscountpct AS "                                 + properties::ADD_DISCOUNT_PCT.getName()       + ", "
    "tenpos.discountcalcfrom AS "                               + properties::DISCOUNT_CALC_FROM.getName()     + ", "
    "tenpos.discountapplyto AS "                                + properties::DISCOUNT_APPLY_TO.getName()      + ", "
    "tenpos.contractqty AS "                                    + properties::CONTRACT_QTY.getName()           + ", "
    "TO_DATE(tenpos.changedate::CHAR(8), '%Y%m%d')::DATE  AS "  + properties::CHANGE_DATE.getName()            + ", "
    //"TO_DATE("tenpos.creationdate::CHAR(8), '%Y%m%d')::DATE AS "                                    + properties::CREATION_DATE.getName()         + ", " //currently not in the db
    //"( CAST ( tenpos.contractqty AS INT) - ( CAST ( tenprog.deliveryqty AS INT ) - CAST ( tenprog.returnqty AS INT) ) )   AS " + properties::CURRENT_QTY.getName() + ", " //to decide empty col for moment
    "tenpos.tenderstate::SMALLINT AS "                          + properties::TENDER_STATE.getName()           + ", "
    "tenpos.ownquotaqty AS "                                    + properties::OWNQUOTA_QTY.getName()           + ", "
    "tenpos.ownquotaflag::SMALLINT AS "                         + properties::OWNQUOTA_FLAG.getName()          + ", "
    "tenpos.recallflag::SMALLINT AS "                           + properties::RECALL_FLAG.getName()            + ", "
    "head.pharmacygroupid AS "                                  + properties::PHARMACY_GROUPID.getName()       + " "
    "FROM "
    "tenderpos AS tenpos "
    "INNER JOIN articlecodes as ac ON tenpos.articlecode = ac.article_code "
    "INNER JOIN artikelzentral az ON ac.articleno = az.artikel_nr "
    "LEFT JOIN tenderprogress tenprog ON tenprog.tenderno = tenpos.tenderno and tenprog.articleno = tenpos.articleno "
    "INNER JOIN tenderhead head ON head.tenderno= tenpos.tenderno "
    "LEFT JOIN kunde as k ON head.customerno = k.idfnr AND head.branchno = k.filialnr "
    );

    if( isContainedAndSet( properties::SUPPLIER_NO ) )
    {
        literal( "INNER JOIN supplier as lf ON lf.lant_nr = az.hersteller_nr " );
    }

    literal("WHERE head.deletedflag = 0 ");

    if( isContainedAndSet(properties::PHARMACY_GROUPID) )
    {
        resolve("AND head.pharmacygroupid=" + properties::PHARMACY_GROUPID.toSQLReplacementString() + " ");
    }

    if( isContainedAndSet(properties::TENDER_NO) )
    {
        resolve("AND head.tenderno=" + properties::TENDER_NO.toSQLReplacementString() + " ");
    }

    if( isContainedAndSet(properties::CONTRACT_NO) )
    {
        resolve("AND UPPER(head.contractno) LIKE UPPER('" + properties::CONTRACT_NO.toSQLReplacementString() + "%') ");
    }

    if( isContainedAndSet(properties::PHARMACY_CHAIN_NO) )
    {
        resolve("AND head.pharmacychainno='" + properties::PHARMACY_CHAIN_NO.toSQLReplacementString() + "' ");
    }

    if( isContainedAndSet(properties::TENDER_TYPE) )
    {
        resolve("AND head.tendertype=" + properties::TENDER_TYPE.toSQLReplacementString() + " ");
    }

    if( isContainedAndSet(properties::TENDER_STATE) )
    {
        resolve("AND tenpos.tenderstate=" + properties::TENDER_STATE.toSQLReplacementString() + " ");
    }

    if( isContainedAndSet(properties::ARTICLE_CODE) )
    {
        resolve("AND tenpos.articlecode='" + properties::ARTICLE_CODE.toSQLReplacementString() + "' " );
    }

    if( isContainedAndSet(properties::CUSTOMER_NO) )
    {
        resolve("AND head.customerno=" + properties::CUSTOMER_NO.toSQLReplacementString() + " ");
    }

    if( isContainedAndSet(properties::DATE_FROM) )
    {
        resolve("AND TO_DATE(head.datefrom::CHAR(8), '%Y%m%d')::DATE>=TO_DATE('" + properties::DATE_FROM.toSQLReplacementString() + "', '%m/%d/%Y') ");
    }

    if( isContainedAndSet(properties::DATE_TO) )
    {
        resolve("AND TO_DATE(head.dateto::CHAR(8), '%Y%m%d')::DATE<=TO_DATE('" + properties::DATE_TO.toSQLReplacementString() + "', '%m/%d/%Y') ");
    }

    if( isContainedAndSet(properties::SUPPLIER_NO) )
    {
        resolve( "AND lf.lant_nr =  "  + properties::SUPPLIER_NO.toSQLReplacementString() + " ");
    }

    literal("ORDER BY head.tenderno ASC");
    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}


BUILDER_DEFINITION( SelectSuccessorTender )

bool SelectSuccessorTender::isExecutable() const
{
    METHODNAME_DEF( SelectSuccessorTender, doIsExecutable );
    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
    BLOG_TRACE_METHOD( logger, fun );
    
    bool executable = false;
    if( arePropertiesSet( properties::TENDER_NO.toPropertyString() ) &&
        arePropertiesSet( properties::CUSTOMER_NO.toPropertyString() ) &&
        arePropertiesSet( properties::ARTICLE_NO.toPropertyString() ) &&
        arePropertiesSet( properties::DATE_TO.toPropertyString() ) && 
        arePropertiesSet( properties::TENDER_STATE.toPropertyString() ) 
      )
    {
        executable = true;
    }
    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    return executable;
}

void SelectSuccessorTender::buildSQLString()
{
     static const basar::VarString constSQL
     (
        "SELECT DISTINCT "
        "thead.tenderno AS "                                          + properties::TENDER_NO.getName()           + ", "
        "cpgr.customerno AS "                                         + properties::CUSTOMER_NO.getName()         + ", "
        "cpgr.branchno AS "                                           + properties::BRANCH_NO.getName()           + ", "
        "thead.pharmacygroupid AS "                                   + properties::PHARMACY_GROUPID.getName()    + ", "
        "tpos.adddiscountpct AS "                                     + properties::ADD_DISCOUNT_PCT.getName()    + ", "
        "tpos.discountcalcfrom AS "                                   + properties::DISCOUNT_CALC_FROM.getName()  + ", "
        "tpos.discountapplyto AS "                                    + properties::DISCOUNT_APPLY_TO.getName()   + ", "
        "tpos.contractprice AS "                                      + properties::CONTRACT_PRICE.getName()      + " "
        "FROM "
        "customerpharmacygr as cpgr "
        "INNER JOIN kundeschalter ks ON ks.idfnr = cpgr.customerno AND cpgr.branchno = ks.vertriebszentrumnr "
        "INNER JOIN kunde kun on kun.idfnr = cpgr.customerno AND kun.filialnr = cpgr.branchno "
        "INNER JOIN filiale f on f.filialnr = cpgr.branchno "
        "INNER JOIN tenderhead thead on thead.pharmacygroupid = cpgr.pharmacygroupid "
        "INNER JOIN tenderpos tpos on tpos.tenderno = thead.tenderno "
        "WHERE cpgr.customerno = "+ properties::CUSTOMER_NO.toSQLReplacementString() +" "
        "AND thead.tenderno <> "+ properties::TENDER_NO.toSQLReplacementString() +" "
        "AND ks.skdkundeaegesperrt = '0' AND kun.originalfilialnr = 0 "
        "AND thead.tenderstate=" + properties::TENDER_STATE.toSQLReplacementString() + " "
        "AND TO_DATE(thead.dateto::CHAR(8), '%Y%m%d')::DATE > TO_DATE('" + properties::DATE_TO.toSQLReplacementString() + "', '%m/%d/%Y') "
        "AND tpos.articleno = " + properties::ARTICLE_NO.toSQLReplacementString() + " "
    );

    resolve ( constSQL );
    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}


BUILDER_DEFINITION( SelectTenderByEndDate )

bool SelectTenderByEndDate::isExecutable() const
{
    METHODNAME_DEF( SelectTenderByEndDate, doIsExecutable );
    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
    BLOG_TRACE_METHOD( logger, fun );
    
    bool executable = false;
    if( arePropertiesSet( properties::DATE_TO.toPropertyString() ) )
    {
        executable = true;
    }
    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    return executable;
}

void SelectTenderByEndDate::buildSQLString()
{
    static const basar::VarString constSQL(
                                            "SELECT DISTINCT "
                                            "thead.tenderno AS " + properties::TENDER_NO.getName() + " "
                                            "from tenderhead thead "
                                            "WHERE "
                                            "thead.deletedflag = 0 "
                                            "AND thead.tenderstate = 1 "
                                            "AND ( branchno = " + accessor::tender::lit::BRANCH_NO_TWELVE +
                                            " OR  branchno = " + accessor::tender::lit::BRANCH_NO_THIRTEEN +
                                            " OR branchno = " + accessor::tender::lit::BRANCH_NO_SIXTEEN +
                                            " ) "
                                            "AND "
                                            "TO_DATE(thead.dateto::CHAR(8), '%Y%m%d')::DATE = TO_DATE('" + properties::DATE_TO.toSQLReplacementString() + "', '%m/%d/%Y') "
                                            "ORDER BY thead.tenderno ASC"
                                           );
    resolve ( constSQL );
    BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
}


BUILDER_DEFINITION( SelectExpiredTenders )

bool SelectExpiredTenders::isExecutable() const
{
    METHODNAME_DEF( SelectExpiredTenders, doIsExecutable );
    static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
    BLOG_TRACE_METHOD( logger, fun );
    
    bool executable = false;
    if( arePropertiesSet(properties::DATE_TO.toPropertyString()) )
    {
        executable = true;
    }

    CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    return executable;
}

void SelectExpiredTenders::buildSQLString()
{
    static const basar::VarString constSQL(
        "SELECT DISTINCT "
        "thead.tenderno AS " + properties::TENDER_NO.getName() + " "
        "from tenderhead thead "
        "WHERE "
        "thead.deletedflag = 0 "
        "AND thead.tenderstate = 1 "
        "AND ( branchno = " + accessor::tender::lit::BRANCH_NO_TWELVE +
        " OR  branchno = " + accessor::tender::lit::BRANCH_NO_THIRTEEN +
        " OR branchno = " + accessor::tender::lit::BRANCH_NO_SIXTEEN + " ) "
        "AND "
        "TO_DATE(thead.dateto::CHAR(8), '%Y%m%d')::DATE <= TO_DATE('" + properties::DATE_TO.toSQLReplacementString() + "', '%m/%d/%Y') "
        "ORDER BY thead.tenderno ASC");

    resolve(constSQL);
    BLOG_TRACE_SQLSTRING(libtender::LoggerPool::getLoggerDomModules());
}


BUILDER_DEFINITION( SelectValidTenders )

bool SelectValidTenders::isExecutable() const
    {
        METHODNAME_DEF( SelectActiveTenders, isExecutable );
        static const log4cplus::Logger& logger = libtender::LoggerPool::getLoggerDomModules();
        BLOG_TRACE_METHOD( logger, fun );
        
        bool executable = false;
        
        if( arePropertiesSet( properties::PHARMACY_GROUPID.toPropertyString()) ||  
            arePropertiesSet( properties::TENDER_NO.toPropertyString() ) ||
            arePropertiesSet( properties::TENDER_STATE.toPropertyString() ) ||
            arePropertiesSet( properties::ARTICLE_NO.toPropertyString() ) ||
            arePropertiesSet( properties::DATE_TO.toPropertyString() ) )
        {
            executable = true;
        }
    
        CHECK_EXECUTABILITY( getPropertyList(), logger, fun );
    
        return executable;
    }

void SelectValidTenders::buildSQLString()
    {

        static const basar::VarString constSQL(
                  "SELECT thead.tenderno AS "                  + properties::TENDER_NO.getName() + ", "
                  "thead.tenderstate::SMALLINT AS "            + properties::TENDER_STATE.getName() + " "
                  "FROM tenderhead AS thead "
                  "INNER JOIN tenderpos tpos ON tpos.tenderno = thead.tenderno "
                  "WHERE 1=1 "
                 );
                  
        basar::VarString variableSQL;
        
        if( arePropertiesSetAndOneMustBeInState( properties::PHARMACY_GROUPID , basar::SS_CLEAN))
        {
            variableSQL.append( "AND thead.pharmacygroupid = '" + properties::PHARMACY_GROUPID.toSQLReplacementString() + "' " );
        }

        if( arePropertiesSetAndOneMustBeInState( properties::TENDER_STATE , basar::SS_CLEAN))
        {
            variableSQL.append( "AND thead.tenderstate = '" + properties::TENDER_STATE.toSQLReplacementString() + "' " );
        }

        if( arePropertiesSetAndOneMustBeInState( properties::TENDER_NO , basar::SS_CLEAN))
        {
            variableSQL.append( "AND thead.tenderno <> " + properties::TENDER_NO.toSQLReplacementString() + " " );
        }

        if( arePropertiesSetAndOneMustBeInState( properties::ARTICLE_NO , basar::SS_CLEAN))
        {
            variableSQL.append( "AND tpos.articleno = " + properties::ARTICLE_NO.toSQLReplacementString() + " " );
        }

        if( arePropertiesSetAndOneMustBeInState( properties::DATE_TO , basar::SS_CLEAN))
        {
            variableSQL.append( "AND TO_DATE(thead.dateto::CHAR(8), '%Y%m%d')::DATE > TO_DATE('" + properties::DATE_TO.toSQLReplacementString() + "', '%m/%d/%Y') " );
        }

        basar::VarString organizeResult( "ORDER BY thead.dateto DESC " );
        resolve( constSQL + variableSQL + organizeResult );
        BLOG_TRACE_SQLSTRING( libtender::LoggerPool::getLoggerDomModules() );
   }

} // end namespace tender
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender
