//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  application (main) functions for cscbatchauto
 *  \author
 *  \date
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "cscbatchautoappl.h"
#include <libbasarlogin.h>
#include "libcscbatch_loggerpool.h"
#include "libcscbatch_definitions.h"
#include "basarutils.h"
#include "batchorderautouc.h"

//**************************************************************************************************
// using declarations
//**************************************************************************************************
using basar::appl::HANDLER_OK;
using basar::appl::HANDLER_ERROR;

	namespace basar
	{
		namespace cmnutil
		{
			using cscbatchauto::CscBatchAutoApplBase;

			template <>
			typename Singleton< CscBatchAutoApplBase >::InstancePtr &
			Singleton< CscBatchAutoApplBase >::inst()
			{
				// the one and only instance
				static InstancePtr s_Inst;

				return s_Inst;
			}
		}
	}

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace cscbatchauto {
	//**************************************************************************************************
	//         Konstruktoren / Destruktoren
	//**************************************************************************************************
	CscBatchAutoApplBase::CscBatchAutoApplBase() {
	}

	CscBatchAutoApplBase::~CscBatchAutoApplBase() {
		shutdown();
	}

	//**************************************************************************************************
	//         Get-/Set-Methoden
	//**************************************************************************************************
	ConnectionRef CscBatchAutoApplBase::getConnection() {
		return m_ConnectionCsc.getConnectionCsc();
	}// getConnection

	//**************************************************************************************************
	//         Methoden
	//**************************************************************************************************
	//*************************************** init ******************************************************
	//
	// Descr.:	Checks whether there is a file cscbatchauto_test.txt in the current working directory
	//				If yes, gets the parameters (database server and database) from the test database
	//				If not, gets the parameters from the PRODUCTIVE database!!!
	//			Checks whether the parameter START is set in UsrAdmin
	//			Connects to the databases
	//			Runs UseCase
	//
	//**************************************************************************************************
	basar::appl::EventReturnType CscBatchAutoApplBase::init()
	{
		BLOG_TRACE_METHOD(libcscbatch::LoggerPool::loggerMain, "CscBatchAutoApplBase::init().");

		basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);
		basar::I18nString application=UADM_APPLICATION_NAME;  // 1st parameter of showDlg() mustn't be constant!

		if(!basar::login::Manager::getInstance().nonInteractiveLogin(application, UADM_BATCH_USERNAME, UADM_BATCH_PASSWORD, m_BranchNo))
		{
			result.ret=basar::appl::HANDLER_ERROR;
			result.message="Login failed! Username or password wrong or login canceled by user!";
			return result;
		}
		BLOG_INFO(libcscbatch::LoggerPool::loggerMain, "Login User Admin successful!");

		// set member properties
		if(false == getUadmParameter())
		{
			result.ret=basar::appl::HANDLER_ERROR;
			result.message="Could not set Properties from UsrAdmin!";
			return result;
		}

		//--- connect to CSC database:
		if(false == m_ConnectionCsc.connect(m_Host, m_Db))  //connect to CSC database
		{
			result.ret=basar::appl::HANDLER_ERROR;
			result.message="Could not connect to database!";
			return result;
		}

		m_BatchOrderAutoUC = new cscbatchauto::useCase::BatchOrderAutoUC(m_ConnectionCsc.getConnectionCsc());

		return(HANDLER_OK);
	}// init

	//************************************ run ************************************************
	basar::appl::EventReturnType CscBatchAutoApplBase::run(basar::appl::IEventSource &, AccessorPropertyTable_YIterator yitMain)
	{
		BLOG_TRACE_METHOD(libcscbatch::LoggerPool::loggerMain, "CscBatchAutoApplBase::run().");

		basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);

		//--- check yitMain:
		if(!nsBasarUtils::BasarUtils::isPropSet(yitMain,"branchno"))
		{  // branchno is unset
			basar::VarString msg = "INTERNAL ERROR: yit doesn't contain 'filename'";
			BLOG_ERROR(libcscbatch::LoggerPool::loggerUseCases, msg);
			return(result);
		}
		m_BranchNo = static_cast<basar::Int16>(yitMain.getString("branchno").stoi());

		//--- init
		result=init();
		if(result.ret==HANDLER_ERROR)
		{
			return result;
		}

		//--- create yit for use case and call use case:
		basar::I18nString strYit = "";
		basar::I18nString strYit1 = "";
		// set branchno:
		strYit1.format("%s=%d;", "branchno", m_BranchNo);
		strYit.append(strYit1);
		// datasourcetype is not set, because everything is processed, what's in cscini:
		// set kscserver:
		strYit1.format("%s=%s;", "kscserver", m_KscServer.c_str());
		strYit.append(strYit1);
		// set kscport:
		strYit1.format("%s=%s;", "kscport", m_KscPort.c_str());
		strYit.append(strYit1);
		AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);

		return m_BatchOrderAutoUC->run(basar::appl::NullEventSource::getInstance(), yit);
	}

  	//************************************ shutdown ************************************************
	void CscBatchAutoApplBase::shutdown()
	{
		m_ConnectionCsc.disconnect();
	}

	//************************************ getUadmParameter ************************************************
	bool CscBatchAutoApplBase::getUadmParameter()
	{
		BLOG_TRACE_METHOD(libcscbatch::LoggerPool::loggerMain, "CscBatchAutoApplBase::getUadmParameter().");

		// get user³s branch he logged in for to get area-specific database
		basar::I18nString branch;
		branch.format("%d", m_BranchNo);

		// get user-specific GROUP parameter
		basar::I18nString group;
		if (!basar::login::Manager::getInstance().getParaValue(branch, basar::login::Manager::getInstance().getUserName(), UADM_GROUP, group))
		{
			// no user-specific GROUP parameter found (for individual testing!), then take branch-specific group
			if (!basar::login::Manager::getInstance().getParaValue(branch, UADM_GROUP, group))
				return false;
		}

		// get group-specific host parameter
		if(!basar::login::Manager::getInstance().getParaValue(group, UADM_HOST, m_Host))
			return false;

		// get group-specific db parameter
		if(!basar::login::Manager::getInstance().getParaValue(group, UADM_DB, m_Db))
			return false;

		// get group-specific ksc server parameter
		if(!basar::login::Manager::getInstance().getParaValue(group, UADM_KSCSERVER, m_KscServer))
			return false;

		// get group-specific ksc port parameter
		if (!basar::login::Manager::getInstance().getParaValue(group, UADM_KSCPORT, m_KscPort))
			return false;

		BLOG_INFO(libcscbatch::LoggerPool::loggerMain, "Group " + group);
		BLOG_INFO(libcscbatch::LoggerPool::loggerMain, "KSCServer " + m_KscServer);
		BLOG_INFO(libcscbatch::LoggerPool::loggerMain, "KSCPort "+ m_KscPort);
		BLOG_INFO(libcscbatch::LoggerPool::loggerMain, "Host "+ m_Host);
		BLOG_INFO(libcscbatch::LoggerPool::loggerMain, "DB "+ m_Db);

		return true;
	}

} // namespace CscBatches
