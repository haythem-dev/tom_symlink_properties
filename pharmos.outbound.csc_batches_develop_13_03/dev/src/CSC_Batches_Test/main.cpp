//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <UnitTest++.h>
#include <iostream>
#include <unittestconnection.h>
#include <TestReporterStdout.h>

//-------------------------------------------------------------------------------------------------//
// global declaraions
//-------------------------------------------------------------------------------------------------//
pdm::testutils::UnitTestConnection g_utConnectCsc;

//-------------------------------------------------------------------------------------------------//
// main
//-------------------------------------------------------------------------------------------------//
int main() {
	int result = -1;
	//--- connect to DB for all tests:
	g_utConnectCsc.connect("CscBatches");  
	
	//--- run all tests:
	UnitTest::TestReporterStdout reporter;
	result = UnitTest::RunAllTests(reporter, UnitTest::Test::GetTestList(), NULL); 	// 3rd parameter is suite name or NULL (for all suites):

	//--- disconnect DB:
	g_utConnectCsc.disconnect();

	//--- end:
	std::cout << "\n\nPlease press return!";
	getchar();  
	return result;
}// main
