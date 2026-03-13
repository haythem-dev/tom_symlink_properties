//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "batchorderautouc.h"
#include "cscbatchautoappl.h"  // don't include cyclicly, so put this into cpp
#include "cscorderdm.h"
#include "libcscbatch_loggerpool.h"
#include "libcscbatch_definitions.h"

//-------------------------------------------------------------------------------------------------//
// usings
//-------------------------------------------------------------------------------------------------//
using basar::appl::HANDLER_OK;
using basar::appl::HANDLER_ERROR;
using libcscbatch::LoggerPool;

namespace cscbatchauto {
namespace useCase {
	//**************************************************************************************************
	//         Konstruktoren / Destruktoren
	//**************************************************************************************************
	BatchOrderAutoUC::BatchOrderAutoUC() {
		BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BatchOrderAutoUC::BatchOrderAutoUC().");
		m_branchNo=0;
	}

	BatchOrderAutoUC::BatchOrderAutoUC(basar::db::aspect::ConnectionRef ConnectionCsc) {
		BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BatchOrderAutoUC::BatchOrderAutoUC().");
		m_ConnectionCsc=ConnectionCsc;
		m_branchNo=0;
	}

	BatchOrderAutoUC::~BatchOrderAutoUC() {
		BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BatchOrderAutoUC::~BatchOrderAutoUC().");
	}
	//**************************************************************************************************
	//         Methoden
	//**************************************************************************************************
	//*************************************** run ******************************************************
	EventReturnType BatchOrderAutoUC::run(IEventSource& /*rSource*/, AccessorPropertyTable_YIterator yitMain) {
		BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BatchOrderAutoUC::run().");
		try {
			basar::appl::EventReturnType result(basar::appl::HANDLER_ERROR);

			//--- check yitMain:
			if(!yitMain.isContainedAndSet("branchno")) {
				basar::VarString msg = "INTERNAL ERROR: yit doesn't contain 'branchno'";
				BLOG_ERROR(LoggerPool::loggerUseCases, msg); 
				return(result);	
			}// if(!yitMain.contains("branchno")
			m_branchNo = static_cast<basar::Int16>(yitMain.getString("branchno").stoi());
			
			if(!yitMain.isContainedAndSet("kscserver")) {
				basar::VarString msg = "INTERNAL ERROR: yit doesn't contain 'kscserver'";
				BLOG_ERROR(LoggerPool::loggerUseCases, msg); 
				return(result);	
			}// if(!yitMain.contains("kscserver")
			basar::I18nString KscServer = yitMain.getString("kscserver");

			if(!yitMain.isContainedAndSet("kscport")) {
				basar::VarString msg = "INTERNAL ERROR: yit doesn't contain 'kscport'";
				BLOG_ERROR(LoggerPool::loggerUseCases, msg); 
				return(result);	
			}// if(!yitMain.contains("kscport")

			basar::Int32 KscPort = yitMain.getString("kscport").stoi();

			// initialize domain module with opened connection: 
			m_ConnectionCsc = getManager().getConnection(); // getManager heißt: er holt die Instanz aus ApplicationController		
			m_BatchOrderDM.init(m_ConnectionCsc);  

			if( m_CscOrderDM.get() == NULL ) {
				m_CscOrderDM = libcscbatch::domMod::CSCOrderDMPtr( new libcscbatch::domMod::CSCOrderDM( m_ConnectionCsc, m_branchNo, KscServer, KscPort ) );
				m_CscOrderDM->init();
			}

			// release orders:
			result=releaseOrders();
			if(HANDLER_ERROR==result.ret) {
				return result;
			}// if(HANDLER_ERROR==result.ret)

			// release domain module
			m_BatchOrderDM.shutdown();

			if( m_CscOrderDM.get() != NULL )
				m_CscOrderDM->shutdown();

			if(HANDLER_ERROR==result.ret)
				return(result);

			return basar::appl::HANDLER_OK;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerUseCases, "BatchOrderAutoUC::run()."); 
			throw;
		}// catch
	}// run

	//************************************ searchOrders ************************************************
	// gets the list of batch-orders to be released. 
	// Gets only orders of a ordertype with an explicit entry in cscini (this should never happen for OM and UW)
	basar::appl::EventReturnStruct BatchOrderAutoUC::searchOrders(AccessorPropertyTableRef &PropTabResult) 
	{
		const char *function="BatchOrderAutoUC::searchOrder.";
		BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, function);
		try 
		{
			basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);

			// create yit for search:
			basar::I18nString strYit="";
			basar::I18nString strYit1="";
			strYit1.format("%s=%d;", "branchno", m_branchNo);
			strYit.append(strYit1);	
			strYit1.format("%s=%d;", "status", CSC_Batches::ZU);
			strYit.append(strYit1);	
			basar::Date today;  // today is initialized with current timestamp
			strYit1.format("%s=%d;", "weekday", today.getDayOfWeek());
			strYit.append(strYit1);	
			strYit1.format("%s=%s;", "auto", "0");  // value doesn't matter, but don't take "", because this is interpreted as unset!
													// "auto" set: Enabled for release by CSC_Batches_Auto by an entry in cscini
													// "auto" not set: Enabled for release by CSC_Batches-Gui (manually) by an entry in cscini
			strYit.append(strYit1);	

			// Prepare one line of search criteria:
			// set  yitSearch to "branchno=<m_branchNo>;status=CSC_Batches::ZU;weekday=<today.getDayOfWeek()>;auto="0";"
			// that is: all ZU-orders of this branch for the current weekday which are enabled for auto-Release by an entry in cscini
			AccessorPropertyTable_YIterator yitSearch = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);

			m_BatchOrderDM.findBatchOrdersByPattern(yitSearch);
			PropTabResult = m_BatchOrderDM.getOrderHeaderPropTab();

			return basar::appl::HANDLER_OK;
		}// try
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerUseCases, function); 
			m_BatchOrderDM.rollbackTransaction();
			throw;
		}// catch
	}// searchOrders	

	//************************************ releaseOrders ************************************************
	//
	// Descr.: Releases all batch orders one by one and put them into KSC
	//
	//****************************************************************************************************
	basar::appl::EventReturnStruct BatchOrderAutoUC::releaseOrders() 
	{
		const char *function="BatchOrderAutoUC::releaseOrders.";
		BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, function);
		try 
		{
			basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);

			AccessorPropertyTableRef PropTabSearchResult;
			result=searchOrders(PropTabSearchResult);
			if(HANDLER_ERROR==result.ret)
				return(result);

			// Prepare one line of additional info for KSC-Client in yitUpdateKscExtra 
			// for creating the KSC order:
			// set  yitUpdateKscExtra to "deferredpaymentdate=0;edideliverynote="0";"	
			basar::I18nString strYit = "";
			basar::I18nString strYit1 = "";
			strYit1.format("%s=%d;", "deferredpaymentdate", 0);		
			strYit.append(strYit1);						
			strYit1.format("%s=%s;", "edideliverynote", "0");		
			strYit.append(strYit1);						
			AccessorPropertyTable_YIterator yitUpdateKscExtra = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);

			// Iterate SearchResult-Proptab and create a CSC-Order for each entry
			for( AccessorPropertyTable_YIterator yitSearchResult=PropTabSearchResult.begin(); !yitSearchResult.isEnd(); ++yitSearchResult ) 
			{

				// transfer orders into KSC:
				result=m_CscOrderDM->createCSCOrder(yitSearchResult, yitUpdateKscExtra );
			}

			return basar::appl::HANDLER_OK;
		}// try
		catch(...) 
		{
			BLOG_ERROR(LoggerPool::loggerUseCases, function); 
			m_BatchOrderDM.rollbackTransaction();
			throw;
		}// catch
	}// releaseOrders	
  }// namespace useCase
}//namespace
