/*! $Id: unittesthelper.cpp 17169 2010-03-17 14:25:58Z dirk.kapusta $                                          
*                                                  
* @file                                            
* @brief ...                                       
* @author Anke Klink                      
* @date 2009                            
*/    

// includes
// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnmain.h" /Fp$(IntDir)/cmnmain.pch
#include "cmnut.h"

#include "unittesthelper.h"

namespace test
{
	// ---------------------------------------------------------------------------------------------------------

	int runTestList(std::vector< basar::VarString > & testList, UnitTest::TestReporterStdout & reporter)
	{
		int result = 0;
		std::vector<basar::VarString> failed;
		std::vector< basar::VarString  >::const_iterator iter;
		for (iter = testList.begin(); iter != testList.end(); ++iter)
		{
			std::cout << "++++++++++++ Start TestSuite " << iter->c_str() << " ++++++++++++" << std::endl;
			UnitTest::TestRunner runner( reporter );
			int miniresult = runner.RunTestsIf ( UnitTest::Test::GetTestList(), 
									iter->c_str(),
									UnitTest::True(),
									0);
			if (miniresult > 0)
			{
				failed.push_back(iter->c_str());
			}
			result += miniresult;
			std::cout << "++++++++++++   End TestSuite " << iter->c_str() << " ++++++++++++" << std::endl;
			std::cout << std::endl;
		}
		if (failed.size() > 0)
		{
			std::cout << "failed: "  << std::endl;
			for (iter = failed.begin(); iter != failed.end(); ++iter)
			{
				std::cout << " - " << iter->c_str() << std::endl;
			}
		} 
		return result;
	}
	// ---------------------------------------------------------------------------------------------------------

}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
