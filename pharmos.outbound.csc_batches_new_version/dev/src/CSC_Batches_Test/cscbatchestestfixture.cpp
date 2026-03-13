//-------------------------------------------------------------------------------------------------//
// Descr.:	Prepares the database for test.
//-------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "cscbatchestestfixture.h"
#include <libbasarlogin.h>

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using basar::VarString;

namespace CscBatchesTest {
	//**************************************************************************************************
	//         Konstruktoren / Destruktoren
	//**************************************************************************************************
	CscBatchesTestFixture::CscBatchesTestFixture() :m_result(false) {
		extern pdm::testutils::UnitTestConnection g_utConnectCsc;
		m_BranchNo=-1;

/*
		//--- reference to accessors, necessary for linking a static lib!
		{
			copybatchorderacc::saveArchiveBatchOrderHeads dummy1;
		}
*/
//		m_DM.init(g_utConnectCsc.getCurrentConnection());
		prepareDbTables();
	}


	CscBatchesTestFixture::~CscBatchesTestFixture() {
		cleanDbTables();
//		m_DM.shutdown();
	}

	//**************************************************************************************************
	//         Methoden
	//**************************************************************************************************
	//*************************************** cleanDbTables ******************************************************
	void CscBatchesTestFixture::cleanDbTables() {
		printf("starting cleanDbTables ...");
		extern pdm::testutils::UnitTestConnection g_utConnectCsc;
		
		//--- rollback DB changes:
		g_utConnectCsc.getSqlConnection().rollback();

		printf("done.\n");	
	}// cleanDbTables

	//*************************************** prepareDbTables ******************************************************
	//--- DB preparation for tests
	void CscBatchesTestFixture::prepareDbTables() {
		printf("starting prepareDbTables ... ");
		extern pdm::testutils::UnitTestConnection g_utConnectCsc;

		try	{
			//--- create SQL statement and begin transactions
			//--- Pay attention, that concurrTransaction flag of m_DbInfo ist set to true!
			//--- Otherwise second transactions fails
			basar::db::sql::StatementRef statement = g_utConnectCsc.getSqlConnection().createStatement();
			g_utConnectCsc.getSqlConnection().begin();

			basar::db::sql::ExecuteReturnInfo  ret;
			basar::I18nString strSql="";
			basar::I18nString strValues="";

			//--- cscini:
			// recordset for branchno m_BranchNo
			//'CL'
			strSql="INSERT INTO cscini (branchno, key, value, user) ";
			strValues.format(" VALUES (%d, 'datasourcetype_gui','CL','')", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			//'SH'
			strSql="INSERT INTO cscini (branchno, key, value, user) ";
			strValues.format(" VALUES (%d, 'datasourcetype_gui','SH','')", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);

			//--- orderhead:
			// order for branchno m_BranchNo
			//'ER'
			strSql="INSERT INTO orderhead (BranchNo, OrderNo, Status, OrderType, OrderDate, OrderValue, PharmacyNo, OrderAcquisitionDate, DataSourceType) ";
			strValues.format(" VALUES (%d, 111,'0','NO',20080520, 109515, 4489587,20080519,'ER')", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// order for branchno m_BranchNo
			// 'SH'
			strSql="INSERT INTO orderhead (BranchNo, OrderNo, Status, OrderType, OrderDate, PharmacyNo, OrderAcquisitionDate, DataSourceType) ";
			strValues.format(" VALUES (%d, 112,'0','NO',20080520, 9669816, 20080519,'SH')", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// order for branchno m_BranchNo
			// 'SH'
			strSql="INSERT INTO orderhead (BranchNo, OrderNo, Status, OrderType, OrderDate, PharmacyNo, OrderAcquisitionDate, DataSourceType) ";
			strValues.format(" VALUES (%d, 113,'0','NO',20080521, 9669816, 20080519,'SH')", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);

			//--- orderpos:
			// position for branchno m_BranchNo
			// orderno=111
			strSql="INSERT INTO orderpos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,111,1,3681911,1)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// position for branchno m_BranchNo
			// orderno=111
			strSql="INSERT INTO orderpos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,111,2,4590116,8)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// position for branchno m_BranchNo
			// orderno=111
			strSql="INSERT INTO orderpos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,111,3,2339725,10)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// position for branchno m_BranchNo
			// orderno=112
			strSql="INSERT INTO orderpos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,112,1,3413582,1)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// position for branchno m_BranchNo
			// orderno=112
			strSql="INSERT INTO orderpos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,112,2,0,1)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// position for branchno m_BranchNo
			// orderno=113
			strSql="INSERT INTO orderpos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,113,2,3728564,3)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
		}// try
		catch(const basar::Exception & except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			g_utConnectCsc.getSqlConnection().rollback();
			throw;
		}// catch basar
		catch (...)	{
			g_utConnectCsc.getSqlConnection().rollback();
			throw;
		}// catch(...)
		printf("done.\n");	
	}// prepareDbTables

	//************************************ checkCount_Recordsets ************************************************
	int CscBatchesTestFixture::checkCount_Recordsets(AccessorPropertyTableRef PropSet) {
		try {
			basar::Int32 count=0;

			if(PropSet.isNull())
				return count;
			if(PropSet.empty())
				return count;
			for(AccessorPropertyTable_YIterator yit=PropSet.begin(); yit != PropSet.end(); yit++)
				count++;

			//--- return:
			return count;
		}// try
		catch(basar::Exception &except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			return -1;
		}// catch
	}//checkCount_Recordsets
	
	//************************************ checkCount_orderhead ************************************************
	int CscBatchesTestFixture::checkCount_orderhead() {
		try {
		extern pdm::testutils::UnitTestConnection g_utConnectCsc;
			basar::Int32 retval(-1);

			VarString strSql("SELECT count(*)::int as count FROM orderhead ");
			VarString strWhere;
			strWhere.format(" WHERE branchno<0");
			strSql.append(strWhere);
			basar::db::sql::StatementRef stmtRef = g_utConnectCsc.getSqlConnection().createStatement();
			basar::db::sql::ResultsetRef rsltRef = stmtRef.executeQuery(strSql);	
			if(rsltRef.isNull())
				return retval;
			if(!rsltRef.isOpen())
				return retval;
			rsltRef.next(); // goto first recordset
			retval = rsltRef.getInt32("count");

			//--- return:
			return retval;
		}// try
		catch(basar::Exception &except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			return -1;
		}// catch
	}//checkCount_orderhead
	
	//************************************ checkCount_orderpos ************************************************
	int CscBatchesTestFixture::checkCount_orderpos() {
		try {
			extern pdm::testutils::UnitTestConnection g_utConnectCsc;
			basar::Int32 retval(-1);

			VarString strSql("SELECT count(*)::int as count FROM orderpos ");
			VarString strWhere;
			strWhere.format(" WHERE branchno<0");
			strSql.append(strWhere);
			basar::db::sql::StatementRef stmtRef = g_utConnectCsc.getSqlConnection().createStatement();
			basar::db::sql::ResultsetRef rsltRef = stmtRef.executeQuery(strSql);	
			if(rsltRef.isNull())
				return retval;
			if(!rsltRef.isOpen())
				return retval;
			rsltRef.next(); // goto first recordset
			retval = rsltRef.getInt32("count");

			//--- return:
			return retval;
		}// try
		catch(basar::Exception &except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			return -1;
		}// catch
	}//checkCount_orderpos
}// namespace