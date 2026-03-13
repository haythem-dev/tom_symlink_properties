//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "batchorderdm.h"

#ifdef WIN32
#pragma warning (push)
#pragma warning(disable: 4512)
#endif
#include <QtCore/QCoreApplication>
#ifdef WIN32
#pragma warning (pop)
#endif

#include "basarutils.h"

#include "batchorderacc.h"
#include "batchorderposacc.h"
#include "cscinfosacc.h"
#include "customerblockedacc.h"
#include "searchorderacc.h"
#include "actionorderacc.h"

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using libcscbatch::LoggerPool;
using nsBasarUtils::BasarUtils;
using basar::Int16;
using basar::Int32;
using basar::cmnutil::ParameterList;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorInstanceRef;
using basar::appl::HANDLER_ERROR;
using basar::appl::HANDLER_OK;

namespace libcscbatch {
namespace domMod {
	//**************************************************************************************************
	//         Konstruktoren / Destruktoren
	//**************************************************************************************************
	BatchOrderDM::BatchOrderDM() : m_Initialized(0) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::BatchOrderDM().");
	}

	BatchOrderDM::~BatchOrderDM() {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::~BatchOrderDM().");
	}

	//**************************************************************************************************
	//         Methoden
	//**************************************************************************************************
	//*************************************** init *****************************************************
	void BatchOrderDM::init(const ConnectionRef activeConn) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::init().");		
		try {		
			// accessors reference for linking in a static lib!
			{
				batchorderacc::SelBatchOrdersByPattern();
				batchorderposacc::SelBatchOrderPosByBranchNoAndOrderNo();
				cscinfosacc::SelOrderTypesByOrderNo();
				customerblockedacc::SelCustomerBlocked();
				searchorderacc::Dummy();
				actionorderacc::Dummy();
			}

			if(0 == m_Initialized) {
				m_BatchOrderSearchACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("BatchOrderSearchAccInstance", "SelBatchOrdersAcc", activeConn);  
				m_BatchOrderSaveACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("BatchOrderSaveAccInstance", "SelBatchOrdersAcc", activeConn);  
				m_CscInfosACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("CscInfosAccInstance", "CscInfosAcc", activeConn );  
				m_CustomerBlockedACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("CustomerBlockedAccInstance", "CustomerBlockedAcc", activeConn ); 
				m_BatchOrderPosACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("BatchOrderPosAccInstance", "SelBatchOrderPosAcc", activeConn ); 
				m_BatchOrderPosFindACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("BatchOrderPosFindAccInstance", "SelBatchOrderPosAcc", activeConn ); 
				m_searchBatchOrderACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("SearchBatchOrderAccInstance", "SearchBatchOrdersAcc", activeConn );
				m_searchBatchOrderACC.getPropertyTable().insert(basar::FOR_UNKNOWN);
				m_actionBatchOrderACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("ActionBatchOrderAccInstance", "ActionBatchOrdersAcc", activeConn );
				m_actionBatchOrderACC.getPropertyTable().insert(basar::FOR_UNKNOWN);
			}
			++m_Initialized;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::init()."); 
			throw; 
		}// catch
	}// init

	//*************************************** shutdown *****************************************************
	void BatchOrderDM::shutdown() {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::shutdown().");
		try {		
			// release accessor instance if instance was created WITH storing (default: without) in instance list
			//basar::db::aspect::Manager::getInstance().releaseAccessorInstance("AccInstance");
			
			if(0==m_Initialized)
				return;
			
			--m_Initialized;
			if(0==m_Initialized) {
				if(!m_BatchOrderSearchACC.getPropertyTable().isNull())
					m_BatchOrderSearchACC.getPropertyTable().clear();
				m_BatchOrderSearchACC.reset();

				if(!m_BatchOrderSaveACC.getPropertyTable().isNull())
					m_BatchOrderSaveACC.getPropertyTable().clear();
				m_BatchOrderSaveACC.reset();

				if(!m_CscInfosACC.getPropertyTable().isNull())
					m_CscInfosACC.getPropertyTable().clear();
				m_CscInfosACC.reset();

				if(!m_CustomerBlockedACC.getPropertyTable().isNull())
					m_CustomerBlockedACC.getPropertyTable().clear();
				m_CustomerBlockedACC.reset();

				if(!m_BatchOrderPosACC.getPropertyTable().isNull())
					m_BatchOrderPosACC.getPropertyTable().clear();
				m_BatchOrderPosACC.reset();

				if(!m_BatchOrderPosFindACC.getPropertyTable().isNull())
					m_BatchOrderPosFindACC.getPropertyTable().clear();
				m_BatchOrderPosFindACC.reset();

				if(!m_searchBatchOrderACC.getPropertyTable().isNull())
					m_searchBatchOrderACC.getPropertyTable().clear();
				m_searchBatchOrderACC.reset();

				if(!m_actionBatchOrderACC.getPropertyTable().isNull())
					m_actionBatchOrderACC.getPropertyTable().clear();
				m_actionBatchOrderACC.reset();
			}// if(0==m_Initialized)
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::shutdown()."); 
			throw;
		}// catch
	}// shutdown

	//*************************************** findBatchOrdersByPattern *****************************************************
	AccessorPropertyTableRef BatchOrderDM::findBatchOrdersByPattern(const AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::findBatchOrdersByPattern().");
		try {		
			// call execute if your connection has been established!
			m_BatchOrderSearchACC.execute("SelBatchOrdersByPattern", yit, true, true, basar::db::aspect::HOLD_RECORDSET);
			return m_BatchOrderSearchACC.getPropertyTable();
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::findBatchOrdersByPattern()."); 
			throw;
		}// catch
	}// findBatchOrdersByPattern

	//*************************************** selOrderTypesByOrderNo *****************************************************
	AccessorPropertyTableRef BatchOrderDM::selOrderTypesByOrderNo(const AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::selOrderTypesByOrderNo().");
		try {		
			// call execute if your connection has been established!
			m_CscInfosACC.execute("SelOrderTypesByOrderNo", yit);
			return(m_CscInfosACC.getPropertyTable());
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::selOrderTypesByOrderNo()."); 
			throw; 
		}// catch
	}// selOrderTypesByOrderNo

	//*************************************** SelCustomerBlocked *****************************************************
	AccessorPropertyTableRef BatchOrderDM::SelCustomerBlocked(const AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::SelCustomerBlocked().");
		try {		
			// call execute if your connection has been established!
			m_CustomerBlockedACC.execute("SelCustomerBlocked", yit);
			return(m_CustomerBlockedACC.getPropertyTable());
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::SelCustomerBlocked()."); 
			throw; 
		}// catch
	}// SelCustomerBlocked

	//*************************************** saveBatchOrderByPattern *****************************************************
	basar::appl::EventReturnStruct BatchOrderDM::saveBatchOrderByPattern(AccessorPropertyTable_YIterator yit) 
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::saveBatchOrderByPattern().");
	
		try {		
			basar::appl::EventReturnStruct result(HANDLER_ERROR);
			// call execute if your connection has been established!
			basar::db::aspect::ExecuteResultInfo res;

			// check, whether there is something to be deleted:
			if(nsBasarUtils::BasarUtils::isPropSet(yit,"deliveryrun")) { // deliveryrun is st
				if(0==yit.getString("deliveryrun").stoi()) {
					yit.setString("deliveryrun","");
				}// if(0==yit.getString("deliveryrun").stoi())
			}// if(!nsBasarUtils::BasarUtils::isPropSet(yit,"deliveryrun"))

			res = m_BatchOrderSaveACC.execute("SaveBatchOrderByPattern", yit);
			if(!res.hasError()) {
				return HANDLER_OK;
			}// if(res>=0)
			result.message = QCoreApplication::translate("BatchOrderDM", "Nothing done!").toLocal8Bit().constData();
			return result;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::saveBatchOrderByPattern()."); 
			throw; 
		}// catch
	}// saveBatchOrderByPattern
	
	//*************************************** saveBatchOrderStatusChange *****************************************************
	basar::appl::EventReturnStruct BatchOrderDM::saveBatchOrderStatusChange(AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::saveBatchOrderStatusChange().");
		try {		
			basar::appl::EventReturnStruct result(HANDLER_ERROR);
			// call execute if your connection has been established!
			basar::db::aspect::ExecuteResultInfo res;

			res = m_BatchOrderSaveACC.execute("SaveBatchOrderStatusChange", yit);
			if(!res.hasError()) {
				return HANDLER_OK;
			}// if(res>=0)
			result.message = QCoreApplication::translate("BatchOrderDM", "Nothing done!").toLocal8Bit().constData();
			return result;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::saveBatchOrderStatusChange()."); 
			throw; 
		}// catch
	}// saveBatchOrderStatusChange

	//*************************************** saveBatchOrderForRelease *****************************************************
	basar::appl::EventReturnStruct BatchOrderDM::saveBatchOrderForRelease(const AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::saveBatchOrderForRelease().");
		try {		
			basar::appl::EventReturnStruct result;
			// call execute if your connection has been established!
			basar::db::aspect::ExecuteResultInfo res;

			res = m_BatchOrderSaveACC.execute("SaveBatchOrderForRelease", yit);
			if(false == res.hasError() && res.getAffectedRows() == 1)
			{
				result.ret = basar::appl::HANDLER_OK;
			}
			else
			{
				result.ret = basar::appl::HANDLER_ERROR;
//				result.message = QCoreApplication::translate("BatchOrderDM", "Nothing done!").toAscii().constData();
			}
			return result;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::saveBatchOrderForRelease()."); 
			throw; 
		}// catch
	}// saveBatchOrderForRelease

	//*************************************** findPositionsByBranchNoAndOrderNo *****************************************************
	AccessorPropertyTableRef BatchOrderDM::findPositionsByBranchNoAndOrderNo(const AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::findPositionsByBranchNoAndOrderNo().");
		try {		
			// call execute if your connection has been established!
			m_BatchOrderPosACC.execute("SelBatchOrderPosByBranchNoAndOrderNo", yit);
			return m_BatchOrderPosACC.getPropertyTable();
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::findPositionsByBranchNoAndOrderNo()."); 
			throw; 
		}// catch
	}// findPositionsByBranchNoAndOrderNo

	//*************************************** savePositions *****************************************************
	bool BatchOrderDM::savePositions(const AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::savePositions().");
		try {		
			// call execute if your connection has been established!
			basar::db::aspect::ExecuteResultInfo res;

			// Check, whether article_code exists in table articlecodes:
			res = m_BatchOrderPosFindACC.execute("SelPZNByArticleCode", yit);
			if(res.hasError())
				return false;
			AccessorPropertyTableRef PropTabSelPZN;
			PropTabSelPZN = m_BatchOrderPosFindACC.getPropertyTable();
			if(PropTabSelPZN.empty()) 
				return false;
			AccessorPropertyTable_YIterator yitSelPZN;
			yitSelPZN=PropTabSelPZN.begin(); 			
			if(yit.isEnd()) 
				return false;

/*+++ loeschen
			basar::I18nString strYit = BasarUtils::createYitString(yit);
			basar::I18nString strYit1 = "";
			strYit1.format("%s=%d;", "newpzn", yitSelPZN.getInt32("newpzn"));
			strYit.append(strYit1);						
			AccessorPropertyTable_YIterator yitNew = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);
*/
			res = m_BatchOrderPosACC.execute("SaveBatchOrderPos", yit);
			if(!res.hasError())
				return true;
			return false;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::savePositions()."); 
			throw; 
		}// catch
	}// savePositions

	//*************************************** findArticleByBranchNoAndArticleNo *****************************************************
	AccessorPropertyTableRef BatchOrderDM::findArticleByBranchNoAndArticleNo(const AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::findArticleByBranchNoAndArticleNo().");
		try {		
			// call execute if your connection has been established!
			AccessorPropertyTable_YIterator yitDummy;
			
			basar::db::aspect::ExecuteResultInfo resInfo;
			resInfo = m_BatchOrderPosFindACC.execute("SelArticleByBranchNoAndArticleNo", yit);
			return m_BatchOrderPosFindACC.getPropertyTable();
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::findArticleByBranchNoAndArticleNo()."); 
			throw; 
		}// catch
	}// findArticleByBranchNoAndArticleNo

	//*************************************** getOrderHeaderPropTab *****************************************************
	AccessorPropertyTableRef BatchOrderDM::getOrderHeaderPropTab() {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::getOrderHeaderPropTab().");
		try {		
			return m_BatchOrderSearchACC.getPropertyTable();
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::getOrderHeaderPropTab()."); 
			throw; 
		}// catch
	}// getOrderHeaderPropTab

	//*************************************** clearOrderHeaderPropTab *****************************************************
	void BatchOrderDM::clearOrderHeaderPropTab() {
		const char *function="BatchOrderDM::clearOrderHeaderPropTab().";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);
		try {		
			m_BatchOrderSearchACC.getPropertyTable().clear();
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw; 
		}// catch
	}// clearOrderHeaderPropTab

	//*************************************** getOrderPosPropTab *****************************************************
	AccessorPropertyTableRef BatchOrderDM::getOrderPosPropTab() {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::getOrderPosPropTab().");
		try {		
			return m_BatchOrderPosACC.getPropertyTable();
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::getOrderPosPropTab()."); 
			throw; 
		}// catch
	}// getOrderPosPropTab

	//*************************************** getSearchOrderPropTab *****************************************************
	AccessorPropertyTableRef BatchOrderDM::getSearchOrderPropTab() {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::getSearchOrderPropTab().");
		try {		
			return m_searchBatchOrderACC.getPropertyTable();
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::getSearchOrderPropTab()."); 
			throw; 
		}// catch
	}// getSearchOrderPropTab

	//*************************************** getActionOrderPropTab *****************************************************
	AccessorPropertyTableRef BatchOrderDM::getActionOrderPropTab() {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::getActionOrderPropTab().");
		try {		
			return m_actionBatchOrderACC.getPropertyTable();
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::getActionOrderPropTab()."); 
			throw; 
		}// catch
	}// getActionOrderPropTab

	//*************************************** clearSearchIterator *****************************************************
	void BatchOrderDM::clearSearchIterator() {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::clearSearchIterator().");
		try {		
			m_searchBatchOrderACC.getPropertyTable().begin().setPropertyState("ordernofroms", basar::SS_UNSET);
			m_searchBatchOrderACC.getPropertyTable().begin().setPropertyState("ordernotos", basar::SS_UNSET);
			m_searchBatchOrderACC.getPropertyTable().begin().setPropertyState("status", basar::SS_UNSET);
			m_searchBatchOrderACC.getPropertyTable().begin().setPropertyState("datasource", basar::SS_UNSET);
			m_searchBatchOrderACC.getPropertyTable().begin().setPropertyState("pharmacyno", basar::SS_UNSET);
			m_searchBatchOrderACC.getPropertyTable().begin().setPropertyState("favoredduedate", basar::SS_UNSET);
			m_searchBatchOrderACC.getPropertyTable().begin().setPropertyState("manufacturer", basar::SS_UNSET);
			m_searchBatchOrderACC.getPropertyTable().begin().setPropertyState("salesman", basar::SS_UNSET);
			m_searchBatchOrderACC.getPropertyTable().begin().setPropertyState("department", basar::SS_UNSET);
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "BatchOrderDM::clearSearchIterator()."); 
			throw; 
		}// catch
	}// clearSearchIterator

	//*************************************** beginTransaction *****************************************************
	basar::appl::EventReturnStruct BatchOrderDM::beginTransaction() {
		const char *function="BatchOrderDM::beginTransaction().";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);
		try {		
			nsBasarUtils::BasarUtils::beginTransaction(m_BatchOrderSaveACC);
			return HANDLER_OK;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw; 
		}// catch
	}// beginTransaction

	//*************************************** commitTransaction *****************************************************
	basar::appl::EventReturnStruct BatchOrderDM::commitTransaction() {
		const char *function="BatchOrderDM::commitTransaction().";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);
		try {		
			nsBasarUtils::BasarUtils::commitTransaction(m_BatchOrderSaveACC);
			return HANDLER_OK;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw; 
		}// catch
	}// commitTransaction

	//*************************************** rollbackTransaction *****************************************************
	basar::appl::EventReturnStruct BatchOrderDM::rollbackTransaction() {
		const char *function="BatchOrderDM::rollbackTransaction().";
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, function);
		try {		
			nsBasarUtils::BasarUtils::rollbackTransaction(m_BatchOrderSaveACC);
			return HANDLER_OK;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, function); 
			throw; 
		}// catch
	}// rollbackTransaction
} // namespace domMod
}
