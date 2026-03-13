#include "cscbatcheshistoryappl.h"
#include <libbasarcmnutil.h> 
#include <libbasarlogin.h>
#include <libbasar_pkgversion.h>
#include "loggerpool.h"

#include <libbasarproperty.h>
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

//-------------------------------------------------------------------------------------------------//
// constants
//-------------------------------------------------------------------------------------------------//
//! constant names of command line parameters
basar::ConstString PARAM_VERSION = "-version";
basar::ConstString PARAM_BRANCHNO = "-branch";  
basar::ConstString PARAM_LTORDERDATE = "-ltorderdate";
// xz:the number of days as a new parameter for archiving orderproposal
basar::ConstString PARAM_MINAGEFORORDERPROPOSAL = "-minagefororderproposal";
// xz:the number of days as a new parameter for deleting orderheadprotocol / orderposprotocol
basar::ConstString PARAM_MINAGEFORORDERPROTOCOL = "-minagefororderprotocol";

bool checkParams(int argc, char *argv[], basar::I18nString &BranchNo, basar::I18nString &LtOrderdate, basar::I18nString &DayIntProposal, basar::I18nString &DayIntProtocol)
{
	const char *function="checkParams().";
	BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerMain, function); 

	try 
    {
		//--- check existance of parameters:
		bool FoundBranchNo             = false;
		bool FoundLtOrderdate          = false;
        bool FoundMinAgeForOrderProtol = false;

		for(int i=1; i<argc; i++)
		{
			basar::VarString arg = argv[i];

			if(arg == PARAM_BRANCHNO) 
            {
				BranchNo.format("%s", argv[i+1]);
				FoundBranchNo=true;
			}
			if(arg == PARAM_LTORDERDATE) 
            {
				LtOrderdate.format("%s", argv[i+1]);
				FoundLtOrderdate=true;
			}			
			
			//--check the new parameter -minagefororderproposal		
			if(arg == PARAM_MINAGEFORORDERPROPOSAL) 
            {
				if (argv[i+1])
                {
					DayIntProposal.format("%s", argv[i+1]); 
				}
			}
	
            if(arg == PARAM_MINAGEFORORDERPROTOCOL) 
            {
				if (argv[i+1])
                {
					DayIntProtocol.format("%s", argv[i+1]);
                    FoundMinAgeForOrderProtol = true;
				}
			}
		}

		if(!FoundBranchNo)
		{
			return false;
		}
		
		if(!FoundLtOrderdate)
		{
			return false;
		}

        if(!FoundMinAgeForOrderProtol)
		{
            return false;
		}

		return true;
	}
	catch(...) 
    {
		BLOG_ERROR(cscbatchhistory::LoggerPool::loggerMain, function); 
		throw;
	}
}

//*************************************** main ******************************************************
int main(int argc, char *argv[]) 
{
	int retval=0;  // return value
	basar::appl::EventReturnStruct ret(basar::appl::HANDLER_ERROR);
	basar::VarString msg;
	BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerMain, "main()"); 

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
		msg.format("START APPLICATION: %s", cmdline.getCmdLine().c_str());
		BLOG_INFO(cscbatchhistory::LoggerPool::loggerMain, msg);

		//--- log program name and version:
        msg.format("%s", versioninfo);
        BLOG_INFO(cscbatchhistory::LoggerPool::loggerMain, msg);

		//--- log basar name and version:
		msg.format("Basar-Fullversion-Info: %s", basar::cmnutil::getFullVersion());
		BLOG_INFO(cscbatchhistory::LoggerPool::loggerMain, msg);

		basar::I18nString BranchNo       = "";
		basar::I18nString LtOrderdate    = "";
		basar::I18nString DayIntProposal = "";
        basar::I18nString DayIntProtocol = "";

		//--- check correct usage of program:
		if(!checkParams(argc, argv, BranchNo, LtOrderdate, DayIntProposal, DayIntProtocol))
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerMain, "Parameter is missing.");
			retval=1;
			return retval;
		}

		//--- run use case:
		basar::I18nString strYit  = "";
		basar::I18nString strYit1 = "";

		strYit1.format("%s=%s;", "branchno", BranchNo.c_str()); 
		strYit.append(strYit1);
		strYit1.format("%s=%s;", "lessthanorderdate", LtOrderdate.c_str()); 
		strYit.append(strYit1);

		if (false == DayIntProposal.empty())
		{
			strYit1.format("%s=%s;", "minimumagefororderproposal", DayIntProposal.c_str()); 
			strYit.append(strYit1);
		}

        strYit1.format("%s=%s;", "minagefororderprotocol", DayIntProtocol.c_str()); 
		strYit.append(strYit1);

		basar::Date deletedateforarchive;
		deletedateforarchive.addYears(-3);

		strYit1.format("%s=%s;", "deletedateforarchive", deletedateforarchive.toString("yyyyMMdd").c_str()); 
		strYit.append(strYit1);

		basar::db::aspect::AccessorPropertyTable_YIterator yitMain = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);

		ret = cscbatchhistory::CscBatchesHistoryAppl::getInstance().run(yitMain);

		if (basar::appl::HANDLER_OK != ret.ret)
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerMain, ret.message); 
			retval=2; //cscbatchhistory::RET_BASAR_EXCEPTION;
		}
	}
	catch(const basar::Exception &except) 
    {
		msg.format("\n=== basar exception caught ===========================================================\n\n%s\n",except.what().c_str());
		BLOG_ERROR(cscbatchhistory::LoggerPool::loggerMain, msg); 
		retval=3; //cscbatchhistory::RET_BASAR_EXCEPTION;
	}
	catch(const std::exception &) 
    {
		msg.format("\n=== std exception caught ===========================================================\n");
		BLOG_ERROR(cscbatchhistory::LoggerPool::loggerMain, msg); 
		retval=4; //cscbatchhistory::RET_STD_EXCEPTION;
	}
    catch(...) 
    {
		msg.format("\n=== unknown exception caught ===========================================================\n");	
		BLOG_ERROR(cscbatchhistory::LoggerPool::loggerMain, msg); 
		retval=5; //cscbatchhistory::RET_MISC_EXCEPTION;
	}

	//--- clear:
	cscbatchhistory::CscBatchesHistoryAppl::clear();
	basar::login::Manager::clear();
	basar::db::aspect::Manager::clear();

	//--- log exit including return code:
	msg.format("EXIT APPLICATION: %s, basar return code <%ld>, program return code <%d>", cmdline.getCmdLine().c_str(), ret.ret, retval);
	BLOG_INFO(cscbatchhistory::LoggerPool::loggerMain, msg); 
	return retval;
}
