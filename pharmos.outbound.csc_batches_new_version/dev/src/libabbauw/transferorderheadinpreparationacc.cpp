#include "cmnaccessor.h"

#include "transferorderheadinpreparationacc.h"
#include "transferorderheadinpreparationpropertystatechecker.h"

#include "transferorderheadinpreparation_definitions.h"
#include <libbasarcmnutil_logging.h>

namespace libabbauw
{
namespace acc_transfer_order_in_preparation
{
	using namespace libabbauw::properties;
	using basar::VarString;

BEGIN_ACCESSOR_DEFINITION( domMod::transferOrder::ACC_TRANSFER_ORDER_HEAD_INPREPARATION )
	
	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD( BRANCHNO                                  );
		PROPERTY_DESCRIPTION_LIST_ADD( USER                                      );
        PROPERTY_DESCRIPTION_LIST_ADD( ORDERTYPE                                 );
			PROPERTY_DESCRIPTION_LIST_ADD(ORDERNOMF);
			PROPERTY_DESCRIPTION_LIST_ADD(HEADERTEXT);
			PROPERTY_DESCRIPTION_LIST_ADD(ISMANUALHEADERTEXTFLAG);
        PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTURERNO                            );
        PROPERTY_DESCRIPTION_LIST_ADD( MANUFACTURERNAME                          );
        PROPERTY_DESCRIPTION_LIST_ADD( DEFERREDPAYMENTDATE                       );
		PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNO                                );
        PROPERTY_DESCRIPTION_LIST_ADD( PHARMACYNAME                              );
        PROPERTY_DESCRIPTION_LIST_ADD( LOCATION                                  );
		PROPERTY_DESCRIPTION_LIST_ADD( EDIDELIVERYDATE                           );
	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD( domMod::transferOrder::SELECT_CURRENT_TRANSFER_ORDER_HEAD_INPREPARATION )
		SQL_BUILDER_CREATE(    SelectCurrentTransferOrderHeadInPreparation )
		SQL_BUILDER_PUSH_BACK( SelectCurrentTransferOrderHeadInPreparation )

    ACCESS_METHOD( domMod::transferOrder::SAVE_TRANSFER_ORDER_HEAD_INPREPARATION )
		SQL_BUILDER_CREATE(    InsertTransferOrderHeadInPreparation )
		SQL_BUILDER_PUSH_BACK( InsertTransferOrderHeadInPreparation )
        SQL_BUILDER_CREATE(    UpdateTransferOrderHeadInPreparation )
        SQL_BUILDER_PUSH_BACK( UpdateTransferOrderHeadInPreparation )

    ACCESS_METHOD( domMod::transferOrder::DELETE_TRANSFER_ORDER_HEAD_INPREPARATION )
        SQL_BUILDER_CREATE(    DeleteTransferOrderHeadInPreparation )
		SQL_BUILDER_PUSH_BACK( DeleteTransferOrderHeadInPreparation )

END_ACCESSOR_DEFINITION

BUILDER_DEFINITION( SelectCurrentTransferOrderHeadInPreparation )

    bool SelectCurrentTransferOrderHeadInPreparation::isExecutable() const
    {
        accessor::transferOrder::HeadInPreparationPropertyStateChecker checker( getPropertyList() );
        return checker.isSelectable();
    }

	void SelectCurrentTransferOrderHeadInPreparation::buildSQLString()
	{
		static const VarString sqlStatement(
            "SELECT oh.branchno AS "                                                         + BRANCHNO.getName()                +
            ", oh.user AS "                                                                  + USER.getName()                    +
            ", oh.manufacturerno AS "                                                        + MANUFACTURERNO.getName()          +
				", oh.ordernomf AS " + ORDERNOMF.getName() +
				", oh.headertext AS " + HEADERTEXT.getName() +
				", oh.ismanualheadertextflag AS " + ISMANUALHEADERTEXTFLAG.getName() +
			", NVL(h.name, '') AS "                                                          + MANUFACTURERNAME.getName()        +
            ", oh.pharmacyno AS "                                                            + PHARMACYNO.getName()              +
			", k.nameapo AS "                                                                + PHARMACYNAME.getName()            +
            ", k.ort AS "                                                                    + LOCATION.getName()                +
			", oh.ordertype || oh.orderpickingtype || oh.bookentrytype AS "                  + ORDERTYPE.getName()               +
			", TO_DATE( CASE WHEN oh.EDIDeliveryDate = 0 THEN '99991231' "
			               " ELSE TO_CHAR(oh.EDIDeliveryDate) END, '%Y%m%d')::DATE AS "      + EDIDELIVERYDATE.getName()         +
			", TO_DATE( CASE WHEN oh.deferredpaymentdate = 0 THEN '99991231' "
			               " ELSE TO_CHAR(oh.deferredpaymentdate) END, '%Y%m%d')::DATE AS "  + DEFERREDPAYMENTDATE.getName()     +
            " FROM orderheadinprep oh"
            " LEFT JOIN manufacturer h ON oh.manufacturerno = h.hersteller_nr"
            " INNER JOIN kunde  k ON oh.pharmacyno = k.idfnr AND oh.branchno = k.filialnr"
            " WHERE oh.branchno = "                                                          + BRANCHNO.toSQLReplacementString() +
            " AND oh.user = '"                                                               + USER.toSQLReplacementString()     + "'"
		);

		resolve( sqlStatement );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
	}

BUILDER_DEFINITION( InsertTransferOrderHeadInPreparation )

    bool InsertTransferOrderHeadInPreparation::isExecutable() const
    {
        accessor::transferOrder::HeadInPreparationPropertyStateChecker checker( getPropertyList() );
        return checker.isInsertable();
    }

	void InsertTransferOrderHeadInPreparation::buildSQLString()
	{
        const basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

		VarString sqlStatement(
            "INSERT INTO orderheadinprep"
				"(branchno, user, ordertype, orderpickingtype, bookentrytype, pharmacyno, manufacturerno, deferredpaymentdate, edideliverydate, ismanualheadertextflag"
        );

			if (isContainedAndSet(HEADERTEXT))
				sqlStatement.append(", headertext");
			if (isContainedAndSet(ORDERNOMF))
				sqlStatement.append(", ordernomf");
			
			sqlStatement.append(")");

        sqlStatement.append(
            "VALUES ( " + 
				BRANCHNO.toSQLReplacementString() + ", " +
				USER.toSQLReplacementString("'", "'") +
                     ", SUBSTRING(" + ORDERTYPE.toSQLReplacementString( "'", "'" )  + " FROM 1 FOR 2)"
                     ", SUBSTRING(" + ORDERTYPE.toSQLReplacementString( "'", "'" )  + " FROM 3 FOR 1)::INT"
                     ", SUBSTRING(" + ORDERTYPE.toSQLReplacementString( "'", "'" )  + " FROM 4 FOR 1)::INT" +
				"," + PHARMACYNO.toSQLReplacementString() + ", " +
				MANUFACTURERNO.toSQLReplacementString() 
        );

        if( isContainedAndSet( DEFERREDPAYMENTDATE ) && yit.getDate( DEFERREDPAYMENTDATE ).isValid() )
            sqlStatement.append(
                     ", TO_CHAR( TO_DATE("                                          +   DEFERREDPAYMENTDATE.toSQLReplacementString( "'", "'" ) +
                     ", '%m/%d/%Y')::DATE, '%Y%m%d')::INT"
            );
        else
            sqlStatement.append(",0");

        if( isContainedAndSet( EDIDELIVERYDATE ) && yit.getDate( EDIDELIVERYDATE ).isValid() )
            sqlStatement.append(
                     ", TO_CHAR( TO_DATE("                                          +   EDIDELIVERYDATE.toSQLReplacementString( "'", "'" ) +
                     ", '%m/%d/%Y')::DATE, '%Y%m%d')::INT"
            );
        else
            sqlStatement.append(",0");

			sqlStatement.append("," + ISMANUALHEADERTEXTFLAG.toSQLReplacementString("'", "'"));

			if (isContainedAndSet(HEADERTEXT))
				sqlStatement.append("," + HEADERTEXT.toSQLReplacementString("'", "'"));

			if (isContainedAndSet(ORDERNOMF))
				sqlStatement.append("," + ORDERNOMF.toSQLReplacementString("'", "'"));

			sqlStatement.append(")");
		
		resolve( sqlStatement );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
	}

BUILDER_DEFINITION( UpdateTransferOrderHeadInPreparation )

    bool UpdateTransferOrderHeadInPreparation::isExecutable() const
    {
        accessor::transferOrder::HeadInPreparationPropertyStateChecker checker( getPropertyList() );
        return checker.isUpdatable();
    }

	void UpdateTransferOrderHeadInPreparation::buildSQLString()
	{
		static const VarString sqlStatement(
            "UPDATE orderheadinprep op SET "
             "ordertype = SUBSTRING("          + ORDERTYPE.toSQLReplacementString( "'", "'" )  + " FROM 1 FOR 2)"
             ", orderpickingtype = SUBSTRING(" + ORDERTYPE.toSQLReplacementString( "'", "'" )  + " FROM 3 FOR 1)::INT"
             ", bookentrytype = SUBSTRING("    + ORDERTYPE.toSQLReplacementString( "'", "'" )  + " FROM 4 FOR 1)::INT"
             ", pharmacyno = "                                                                 + PHARMACYNO.toSQLReplacementString( "'", "'" )     +
             ", manufacturerno = "                                                             + MANUFACTURERNO.toSQLReplacementString( "'", "'" ) 
		);

        const basar::db::aspect::AccessorPropertyTable_YIterator yit = getPropertyList();

        basar::VarString variableStatement;
        if( isContainedAndSet( DEFERREDPAYMENTDATE ) && yit.getDate( DEFERREDPAYMENTDATE ).isValid() )
            variableStatement.append(
                     ", deferredpaymentdate = TO_CHAR( TO_DATE("                                +   DEFERREDPAYMENTDATE.toSQLReplacementString( "'", "'" ) +
                     ", '%m/%d/%Y')::DATE, '%Y%m%d')::INT"
            );
        else
            variableStatement.append( ", deferredpaymentdate = 0" );

        if( isContainedAndSet( EDIDELIVERYDATE ) && yit.getDate( EDIDELIVERYDATE ).isValid() )
            variableStatement.append(
                     ", edideliverydate = TO_CHAR( TO_DATE("                                   +   EDIDELIVERYDATE.toSQLReplacementString( "'", "'" ) +
                     ", '%m/%d/%Y')::DATE, '%Y%m%d')::INT"
            );
        else
            variableStatement.append( ", edideliverydate = 0" );

			variableStatement.append(", ismanualheadertextflag = " + ISMANUALHEADERTEXTFLAG.toSQLReplacementString("'", "'"));

			if (isContainedAndSet(HEADERTEXT))
				variableStatement.append(", headertext = " + HEADERTEXT.toSQLReplacementString("'", "'"));

			if (isContainedAndSet(ORDERNOMF))
				variableStatement.append(", ordernomf = " + ORDERNOMF.toSQLReplacementString("'", "'"));

        static const basar::VarString whereCondition = " WHERE branchno = "  + BRANCHNO.toSQLReplacementString() +
                                                       " AND op.user = "     + USER.toSQLReplacementString( "'", "'" );

		resolve( sqlStatement + variableStatement + whereCondition );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
	}

BUILDER_DEFINITION( DeleteTransferOrderHeadInPreparation )

    bool DeleteTransferOrderHeadInPreparation::isExecutable() const
    {
        return arePropertiesSet( properties::BRANCHNO.toPropertyString() + properties::USER.toPropertyString() );
    }

	void DeleteTransferOrderHeadInPreparation::buildSQLString()
	{
		static const VarString sqlStatement(
			"DELETE FROM orderheadinprep oh"
            " WHERE oh.branchno = "                                                           + BRANCHNO.toSQLReplacementString()      +
            " AND oh.user = "                                                                 + USER.toSQLReplacementString("'", "'")
		);

		resolve( sqlStatement );
        BLOG_TRACE_SQLSTRING( libabbauw::LoggerPool::loggerTransferOrder );
	}

} // end namespace acc_transfer_order_in_preparation
} // end namespace libabbauw
