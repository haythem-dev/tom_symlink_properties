//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "cscinidm.h"

#ifdef WIN32
#pragma warning (push)
#pragma warning(disable: 4512)
#endif
#include <QtCore/QCoreApplication>
#ifdef WIN32
#pragma warning (pop)
#endif

#include "csciniacc.h"
#include "basarutils.h"

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using libcscbatch::LoggerPool;
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
	CscIniDM::CscIniDM() : m_Initialized(0) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CscIniDM::CscIniDM().");
	}

	CscIniDM::~CscIniDM() {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CscIniDM::~CscIniDM().");
	}

	//**************************************************************************************************
	//         Methoden
	//**************************************************************************************************
	//*************************************** init *****************************************************
	void CscIniDM::init(const ConnectionRef activeConn) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CscIniDM::init().");		
		try {		
			// accessors reference for linking in a static lib!
			{
				csciniacc::SaveEnterByBranch();
			}

			if(0 == m_Initialized) {
				m_CscInitACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("CscInitAccInstance", "csciniacc", activeConn);  
			}
			++m_Initialized;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "CscIniDM::init()."); 
			throw; 
		}// catch
	}// init

	//*************************************** shutdown *****************************************************
	void CscIniDM::shutdown() {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "CscIniDM::shutdown().");
		try {		
			if(0==m_Initialized)
				return;
			
			--m_Initialized;
			if(0==m_Initialized) {
				if(!m_CscInitACC.getPropertyTable().isNull())
					m_CscInitACC.getPropertyTable().clear();
				m_CscInitACC.reset();
			}// if(0==m_Initialized)
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "CscIniDM::shutdown()."); 
			throw;
		}// catch
	}// shutdown

	//*************************************** SelEnterByBranch *****************************************************
	AccessorPropertyTableRef CscIniDM::SelEnterByBranch(const AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::SelEnterByBranch().");
		try {		
			// call execute
			m_CscInitACC.execute("SelEnterByBranch", yit);
			return m_CscInitACC.getPropertyTable();
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "CscIniDM::SelEnterByBranch()."); 
			throw; 
		}// catch
	}// SelEnterByBranch

	//*************************************** SaveEnterByBranch *****************************************************
	basar::appl::EventReturnStruct CscIniDM::SaveEnterByBranch(const AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::SaveEnterByBranch().");
		try {		
			basar::appl::EventReturnStruct result(HANDLER_ERROR);
			// call execute if your connection has been established!
			basar::db::aspect::ExecuteResultInfo res;
			res = m_CscInitACC.execute("SaveEnterByBranch", yit);
			if(!res.hasError()) {
				return HANDLER_OK;
			}// if(res>=0)
			result.message = QCoreApplication::translate("CscIniDM", "Program is used by another user!").toLocal8Bit().constData();
			return result;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "CscIniDM::SaveEnterByBranch()."); 
			throw; 
		}// catch
	}// SaveEnterByBranch

	//*************************************** DeleteEnterByBranch *****************************************************
	basar::appl::EventReturnStruct CscIniDM::DeleteEnterByBranch(const AccessorPropertyTable_YIterator yit) {
		BLOG_TRACE_METHOD(LoggerPool::loggerDomModules, "BatchOrderDM::DeleteEnterByBranch().");
		try {		
			basar::appl::EventReturnStruct result(HANDLER_ERROR);
			// call execute if your connection has been established!
			basar::db::aspect::ExecuteResultInfo res;
			res = m_CscInitACC.execute("DeleteEnterByBranch", yit);
			if(!res.hasError()) {
				return HANDLER_OK;
			}// if(res>=0)
			result.message = QCoreApplication::translate("CscIniDM", "Error when deleting user in cscini!").toLocal8Bit().constData();
			return result;
		}// try
		catch(...) {
			BLOG_ERROR(LoggerPool::loggerDomModules, "CscIniDM::DeleteEnterByBranch()."); 
			throw; 
		}// catch
	}// DeleteEnterByBranch
} // namespace domMod
}
