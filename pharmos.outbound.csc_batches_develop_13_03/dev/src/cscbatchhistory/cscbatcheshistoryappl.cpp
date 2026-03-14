#include "cscbatcheshistoryappl.h"
#include <libbasarlogin.h>
#include "loggerpool.h"
#include "libcscbatch_definitions.h"

namespace basar
{
	namespace cmnutil
	{
		template<> typename Singleton<cscbatchhistory::CscBatchesHistoryApplBase>::InstancePtr& Singleton<cscbatchhistory::CscBatchesHistoryApplBase>::inst()
		{
			static InstancePtr s_Inst;
			return s_Inst;
		}
	}
}

namespace cscbatchhistory
{
	CscBatchesHistoryApplBase::CscBatchesHistoryApplBase()
	{ 
	}

	CscBatchesHistoryApplBase::~CscBatchesHistoryApplBase()
	{
		shutdown();
	}

	basar::db::aspect::ConnectionRef CscBatchesHistoryApplBase::getConnectionCsc()
	{
		return(m_ConnectionCsc.getConnectionCsc());
	}

	basar::appl::EventReturnType CscBatchesHistoryApplBase::init()
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerMain, "CscBatchesHistoryApplBase::init()");

		basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);
		basar::I18nString application = UADM_APPLICATION_NAME;  // 1st parameter of showDlg() mustn't be constant!

		if (!basar::login::Manager::getInstance().nonInteractiveLogin(application, UADM_BATCH_USERNAME, UADM_BATCH_PASSWORD, m_BranchNo))
		{
			result.ret = basar::appl::HANDLER_ERROR;
			result.message = "Login failed! Username or password wrong or login canceled by user!";
			return result;
		}

		// set member properties
		if (false == getUadmParameter())
		{
			result.ret = basar::appl::HANDLER_ERROR;
			result.message = "Could not set Properties from UsrAdmin!";
			return result;
		}

		//--- connect to CSC database:
		if (false == m_ConnectionCsc.connectCsc(m_Host, m_Db))  //connect to CSC database  
		{
			result.ret = basar::appl::HANDLER_ERROR;
			result.message = "Could not connect to database!";
			return result;
		}

		return(basar::appl::HANDLER_OK);
	}

	basar::appl::EventReturnType CscBatchesHistoryApplBase::run(basar::db::aspect::AccessorPropertyTable_YIterator yitMain)
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerMain, "CscBatchesHistoryApplBase::run().");

		basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);

		if (false == yitMain.isContainedAndSet("branchno"))
		{  // branchno is unset
			result.ret = basar::appl::HANDLER_ERROR;
			result.message = "Field 'BranchNo' was not set!";
			return result;
		}

		m_BranchNo = static_cast<basar::Int16>(yitMain.getString("branchno").stoi());

		if (false == yitMain.isContainedAndSet("lessthanorderdate"))
		{  // lessthanorderdate is unset
			result.ret = basar::appl::HANDLER_ERROR;
			result.message = "Field 'lessthanorderdate' was not set!";
			return result;
		}

		result = init();
		if (result.ret == basar::appl::HANDLER_ERROR)
		{
			return result;
		}

		return m_CscBatchesHistoryUC.run(yitMain);
	}

	void CscBatchesHistoryApplBase::shutdown()
	{
		m_ConnectionCsc.disconnectCsc();
	}

	bool CscBatchesHistoryApplBase::getUadmParameter()
	{
		BLOG_TRACE_METHOD(LoggerPool::loggerMain, "CscBatchesHistoryApplBase::getUadmParameter().");

		// get user³s branch he logged in for to get area-specific database
		basar::I18nString branch;
		branch.format("%d", m_BranchNo);

		// get user-specific GROUP parameter
		basar::I18nString group;
		if (!basar::login::Manager::getInstance().getParaValue(branch, basar::login::Manager::getInstance().getUserName(), UADM_GROUP, group))
		{
			// no user-specific GROUP parameter found (for individual testing!), then take branch-specific group
			if (!basar::login::Manager::getInstance().getParaValue(branch, UADM_GROUP, group))
			{
				return false;
			}
		}

		// get group-specific host parameter
		if (!basar::login::Manager::getInstance().getParaValue(group, UADM_HOST, m_Host))
		{
			return false;
		}

		// get group-specific db parameter
		if (!basar::login::Manager::getInstance().getParaValue(group, UADM_DB, m_Db))
		{
			return false;
		}

		return true;
	}
}
