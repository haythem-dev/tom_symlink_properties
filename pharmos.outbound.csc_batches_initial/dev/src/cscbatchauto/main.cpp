#include "cscbatchautoappl.h"
#include <libbasarcmnutil.h> 
#include <libbasarlogin.h>
#include <libbasar_pkgversion.h>
#include "libcscbatch_loggerpool.h"
/*
#ifndef WIN32
#include "gitversioninfo.h"
#endif
*/

extern "C"
{
	extern const char versioninfo[];
	extern const char BASAR_versioninfo[];
}

basar::ConstString PARAM_VERSION = "-version";  
basar::ConstString PARAM_BRANCHNO = "-branch";  
//basar::ConstString PARAM_SERVER_CSC = "-kscserv";
//basar::ConstString PARAM_PORT_CSC = "-kscport";
//basar::ConstString PARAM_DB_CSC = "-db";

//*************************************** main ******************************************************
//
// Returns:		0, if ok
//				>0 on error 
//				1: Wrong usage
//				2: caught error from use case
//				3: basar error
//				4: std error
//				5: unknown error
//
//**************************************************************************************************
int main(int argc, char *argv[]) {
	int retval=0;  // return value
	basar::appl::EventReturnStruct ret(basar::appl::HANDLER_ERROR);
	basar::VarString msg;
	BLOG_TRACE_METHOD(libcscbatch::LoggerPool::loggerMain, "main()."); 

	basar::cmnutil::CmdLineTokenizer cmdline;
	cmdline.setCmdLine(argc, argv);
	if (cmdline.isParam(PARAM_VERSION))
	{
		std::cout << versioninfo << std::endl;
		std::cout << BASAR_versioninfo << std::endl;
        /*
#ifndef WIN32
		std::cout << GitVersionInfo::instance() << std::endl;
#endif
        */
		exit( 0 );
	}

	try	
	{
		//--- log call:
		msg.format("START APPLICATION: %s, Version: %s", cmdline.getCmdLine().c_str(), versioninfo);
		BLOG_INFO(libcscbatch::LoggerPool::loggerMain, msg); 

		//--- log basar name and version:
		msg.format("Basar-Fullversion-Info: %s", basar::cmnutil::getFullVersion());
		BLOG_INFO(libcscbatch::LoggerPool::loggerMain, msg);

		basar::I18nString BranchNo;
		cmdline.getValOfParam(PARAM_BRANCHNO, BranchNo);

		//--- check correct usage of program:
		if (BranchNo.empty())
		{
			msg.format("Parameter %s is missing.", PARAM_BRANCHNO); 
			BLOG_ERROR(libcscbatch::LoggerPool::loggerMain, msg); 
			retval=1;
			return retval;
		}

		//--- run use case:
		basar::I18nString strYit;
		strYit.format("%s=%s;", "branchno", BranchNo.c_str()); 
		
		basar::db::aspect::AccessorPropertyTable_YIterator yitMain=basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);
		ret = cscbatchauto::CscBatchAutoAppl::getInstance().run(basar::appl::NullEventSource::getInstance(), yitMain);
		if (basar::appl::HANDLER_OK != ret.ret)
		{
			msg.format("main %s.", ret.message.c_str());
			BLOG_ERROR(libcscbatch::LoggerPool::loggerMain, msg); 
			cscbatchauto::CscBatchAutoAppl::clear();
			basar::login::Manager::clear();
			basar::db::aspect::Manager::clear();
			retval=2;
		}// if(basar::appl::HANDLER_OK != ret.ret)
	}// try
	catch(const basar::Exception &except) 
	{
		msg.format("\n=== basar exception caught ===========================================================\n\n%s\n",except.what().c_str());
		BLOG_ERROR(libcscbatch::LoggerPool::loggerMain, msg); 
		retval=3;
	}
	catch(const std::exception &) 
	{
		msg.format("\n=== std exception caught ===========================================================\n");
		BLOG_ERROR(libcscbatch::LoggerPool::loggerMain, msg); 
		retval=4;
	}
    catch(...) 
	{
		msg.format("\n=== unknown exception caught ===========================================================\n");	
		BLOG_ERROR(libcscbatch::LoggerPool::loggerMain, msg); 
		retval=5;
	}

	//--- clear:
	cscbatchauto::CscBatchAutoAppl::clear();
	basar::login::Manager::clear();
	basar::db::aspect::Manager::clear();

	//--- log exit including return code:
	msg.format("EXIT APPLICATION: %s, basar return code <%ld>, program return code <%d>", cmdline.getCmdLine().c_str(), ret.ret, retval);
	BLOG_INFO(libcscbatch::LoggerPool::loggerMain, msg); 

	return retval;
}// main
