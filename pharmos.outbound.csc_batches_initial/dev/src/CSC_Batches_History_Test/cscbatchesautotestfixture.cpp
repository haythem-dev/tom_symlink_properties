//-------------------------------------------------------------------------------------------------//
// Descr.:	Prepares the database for test.
//-------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "cscbatchesautotestfixture.h"
#include <libbasarlogin.h>
#include "copybatchorderacc.h"

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using basar::VarString;

namespace CscBatchesAutoTest {
	//**************************************************************************************************
	//         Konstruktoren / Destruktoren
	//**************************************************************************************************
	CscBatchesAutoTestFixture::CscBatchesAutoTestFixture() :m_result(false) {
		extern pdm::testutils::UnitTestConnection g_utConnect;
		m_BranchNo=-90;

		//--- reference to accessors, necessary for linking a static lib!
		{
			copybatchorderacc::saveArchiveBatchOrderHeads dummy1;
		}
		m_CopyBatchOrdersDM.init(g_utConnect.getCurrentConnection());
		prepareDbTables();
	}

	CscBatchesAutoTestFixture::~CscBatchesAutoTestFixture() {
		cleanDbTables();
		m_CopyBatchOrdersDM.shutdown();
	}

	//**************************************************************************************************
	//         Methoden
	//**************************************************************************************************
	//*************************************** cleanDbTables ******************************************************
	void CscBatchesAutoTestFixture::cleanDbTables() {
		printf("starting cleanDbTables ...");
		extern pdm::testutils::UnitTestConnection g_utConnect;
		
		//--- rollback DB changes:
		g_utConnect.getSqlConnection().rollback();

		printf("done.\n");	
		printf("\n");
	}// cleanDbTables

	//*************************************** prepareDbTables ******************************************************
	void CscBatchesAutoTestFixture::prepareDbTables() {
		printf("starting prepareDbTables ...");
		printf("\n");

		extern pdm::testutils::UnitTestConnection g_utConnect;

		//--- create SQL statement and begin transaction
		basar::db::sql::StatementRef statement = g_utConnect.getSqlConnection().createStatement();

		//--- DB preparation for tests
		try	{
			g_utConnect.getSqlConnection().begin();

			basar::db::sql::ExecuteReturnInfo  ret;
			basar::I18nString strSql="";
			basar::I18nString strWhere="";

			//--- Delete everything for branchno 90, if there is any:
			strSql="DELETE FROM OrderHead ";
			strWhere.format(" WHERE branchno=%d", m_BranchNo);
			strSql.append(strWhere);
			ret = statement.execute(strSql);

			strSql="DELETE FROM OrderPos ";
			strWhere.format(" WHERE branchno=%d", m_BranchNo);
			strSql.append(strWhere);
			ret = statement.execute(strSql);

			strSql="DELETE FROM aOrderHead ";
			strWhere.format(" WHERE branchno=%d", m_BranchNo);
			strSql.append(strWhere);
			ret = statement.execute(strSql);

			strSql="DELETE FROM aOrderPos ";
			strWhere.format(" WHERE branchno=%d", m_BranchNo);
			strSql.append(strWhere);
			ret = statement.execute(strSql);

			//--- Inserts into OrderHead:
			insertIntoOrderHead(statement);
			//--- Inserts into OrderPos:
			insertIntoOrderPos(statement);
		}// try
		catch(const basar::Exception & except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			g_utConnect.getSqlConnection().rollback();
			throw;
		}// catch basar
		catch (...)	{
			g_utConnect.getSqlConnection().rollback();
			throw;
		}// catch(...)
		printf("done.\n");	
	}// prepareDbTables

	//*************************************** insertIntoOrderHead ******************************************************
	// 
	// 9 orders
	// status '2' are OrderNos: 111, 113
	// status '9' are OrderNos: 115, 117
	//
	//******************************************************************************************************************
	void CscBatchesAutoTestFixture::insertIntoOrderHead(basar::db::sql::StatementRef statement) {
		printf("starting insertIntoOrderHead ...");

		try	{
			basar::db::sql::ExecuteReturnInfo  ret;
			basar::I18nString strSql="";
			basar::I18nString strValues="";

			strSql="INSERT INTO OrderHead (BranchNo, OrderNo, Status, OrderType, OrderDate, OrderValue, PharmacyNo, OrderAcquisitionDate, DataSourceType) ";
			strValues.format(" VALUES (%d,111,'2','NO',20080520, 109515, 4489587,20080519,'ER')", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			strSql="INSERT INTO OrderHead (BranchNo, OrderNo, Status, OrderType, OrderDate, OrderValue, PharmacyNo, OrderAcquisitionDate, DataSourceType) ";
			strValues.format(" VALUES (%d,112,'0','NO',20080520, 109515, 4489587,20080519,'UE')", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// without OrderValue:
			strSql="INSERT INTO OrderHead (BranchNo, OrderNo, Status, OrderType, OrderDate, PharmacyNo, OrderAcquisitionDate, DataSourceType) ";
			strValues.format(" VALUES (%d,113,'2','NO',20080520, 9669816, 20080519,'SH')", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// no OrderAquisitionDate
			strSql="INSERT INTO OrderHead (BranchNo, OrderNo, Status, OrderType, OrderDate, OrderValue, PharmacyNo, DataSourceType) ";
			strValues.format(" VALUES (%d,114,'1','NO',20080520,1821,20080519,'CL')", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			strSql="INSERT INTO OrderHead (BranchNo, OrderNo, Status, OrderType, OrderDate, PharmacyNo, OrderAcquisitionDate, DataSourceType, EDIDeliveryDate) ";
			strValues.format(" VALUES (%d,115,'9','NO',20080520, 4489587, 20080519,'UE',20080731)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// not relevant because of date; without OrderValue:
			strSql="INSERT INTO OrderHead (BranchNo, OrderNo, Status, OrderType, OrderDate, PharmacyNo, OrderAcquisitionDate, DataSourceType) ";
			strValues.format(" VALUES (%d,116,'2','NO',20080914,0056265,20080519,'IR')", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// with HeaderText and EDIDeliveryDate
			strSql="INSERT INTO OrderHead (BranchNo, OrderNo, Status, OrderType, OrderDate, OrderValue, PharmacyNo, HeaderText, OrderAcquisitionDate, DataSourceType, EDIDeliveryDate) ";
			strValues.format(" VALUES (%d,117,'9','NO',20080521,45899, 4545989, 'Unbequemer Kunde!',20080519,'UE',20080731)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);

			// branchno -91; don't save these
			// OrderNo already used for BranchNo -90
			strSql="INSERT INTO OrderHead (BranchNo, OrderNo, Status, OrderType, OrderDate, OrderValue, PharmacyNo, DataSourceType) ";
			strValues.format(" VALUES (%d,111,'2','NO',20080520,20830,20080519,'SH')", -91);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// new OrderNo
			strSql="INSERT INTO OrderHead (BranchNo, OrderNo, Status, OrderType, OrderDate, OrderValue, PharmacyNo, DataSourceType) ";
			strValues.format(" VALUES (%d,118,'9','NO',20080520,483,20080519,'IR')", -91);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
		}// try
		catch(const basar::Exception & except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			throw;
		}// catch basar
		catch (...)	{
			throw;
		}// catch(...)
		printf("done.\n");	
	}// insertIntoOrderHead

	//*************************************** insertIntoOrderPos ******************************************************
	//
	// count positions of OrderNo 111: 5 (but only 4 relevant)
	// count positions of OrderNo 112: 2 (not relevant)
	// count positions of OrderNo 113: 0
	// count positions of OrderNo 115: 3
	// count positions of OrderNo 116: 1 (not relevant because of orderdate)
	// count positions of OrderNo 117: 1
	// count positions of OrderNo 118: 1 (not relevant)
	// count positions of OrderNo 120: 2 (not relevant)
	//
	//******************************************************************************************************************
	void CscBatchesAutoTestFixture::insertIntoOrderPos(basar::db::sql::StatementRef statement) {
		printf("starting insertIntoOrderPos ...");

		try	{
			basar::db::sql::ExecuteReturnInfo  ret;
			basar::I18nString strSql="";
			basar::I18nString strValues="";

			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,111,1,3681911,1)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,111,2,4590116,8)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,111,3,23397290,20)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,111,4,2414059,2)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			// not relevant because of wrong BranchNo
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,111,5,7219581,2)", -91);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);

			// OrderNo 112 not in OrderHead
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,112,1,3681911,25)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,112,2,4590116,3)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);

			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,115,1,7219581,5)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,115,2,4544317,18)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,115,3,3413582,1)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);

			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,116,1,3413582,1)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);

			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,117,1,996867,9)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);

			// OrderNo 118 has branchno -91
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,118,1,2073486,18)", -91);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);

			// OrderNo 120 not in OrderHead
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,120,1,23397290,20)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
			strSql="INSERT INTO OrderPos (BranchNo, OrderNo, PosNo, ArticleNo, OrderedQty) ";
			strValues.format(" VALUES (%d,120,2,2414059,2)", m_BranchNo);
			strSql.append(strValues);
			ret = statement.execute(strSql);
			if(1!=ret.m_affectedRows)
				throw(-1);
		}// try
		catch(const basar::Exception & except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			throw;
		}// catch basar
		catch (...)	{
			throw;
		}// catch(...)
		printf("done.\n");	
	}// insertIntoOrderPos

	//************************************ checkCount_aorderhead ************************************************
	int CscBatchesAutoTestFixture::checkCount_aorderhead() {
		try {
			extern pdm::testutils::UnitTestConnection g_utConnect;
			basar::Int32 retval(-1);

			VarString strSql("SELECT count(*)::int as count FROM aorderhead ");
			VarString strWhere;
			strWhere.format(" WHERE branchno<0");
			strSql.append(strWhere);
			basar::db::sql::StatementRef stmtRef = g_utConnect.getSqlConnection().createStatement();
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
	}//checkCount_aorderhead

	//************************************ checkCount_orderhead ************************************************
	int CscBatchesAutoTestFixture::checkCount_orderhead() {
		try {
			extern pdm::testutils::UnitTestConnection g_utConnect;
			basar::Int32 retval(-1);

			VarString strSql("SELECT count(*)::int as count FROM orderhead ");
			VarString strWhere;
			strWhere.format(" WHERE branchno<0");
			strSql.append(strWhere);
			basar::db::sql::StatementRef stmtRef = g_utConnect.getSqlConnection().createStatement();
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

	//************************************ checkCount_aorderpos ************************************************
	int CscBatchesAutoTestFixture::checkCount_aorderpos() {
		try {
			extern pdm::testutils::UnitTestConnection g_utConnect;
			basar::Int32 retval(-1);

			VarString strSql("SELECT count(*)::int as count FROM aorderpos ");
			VarString strWhere;
			strWhere.format(" WHERE branchno<0");
			strSql.append(strWhere);
			basar::db::sql::StatementRef stmtRef = g_utConnect.getSqlConnection().createStatement();
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
	}//checkCount_aorderpos

	//************************************ checkCount_orderpos ************************************************
	int CscBatchesAutoTestFixture::checkCount_orderpos() {
		try {
			extern pdm::testutils::UnitTestConnection g_utConnect;
			basar::Int32 retval(-1);

			VarString strSql("SELECT count(*)::int as count FROM orderpos ");
			VarString strWhere;
			strWhere.format(" WHERE branchno<0");
			strSql.append(strWhere);
			basar::db::sql::StatementRef stmtRef = g_utConnect.getSqlConnection().createStatement();
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
