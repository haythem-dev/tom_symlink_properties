// includes
// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnmain.h" /Fp$(IntDir)/cmnmain.pch
#include "cmnut.h"

#include "unittestappl.h"

namespace unittest
{	 
UnitTestAppl::UnitTestAppl( int argc, char * argv[], unittest::TestCollection & testCollection )
:	m_TestCollection( testCollection ),
	m_Application( argc, argv )
{
	init( argc, argv );
}

int UnitTestAppl::run()
{
	int ret = 0;

#ifdef _DEBUG  
	// DEBUG: optionally run specific tests only
	UnitTest::TestReporterStdout reporter;
	if( m_TestNames.empty() )
	{
		UnitTest::TestRunner runner( reporter );
		ret = runner.RunTestsIf ( UnitTest::Test::GetTestList(), NULL, UnitTest::True(), 0 );
	}
	else
	{
		std::vector<basar::VarString>::const_iterator iter = m_TestNames.begin();
		for( ; iter != m_TestNames.end(); ++iter )
		{
			ret += m_TestCollection.runOneTest( *iter, reporter );
		}
	}

	std::cout << std::endl << "done with " << ret << " Test-Failures" << std::endl;

#else	
	// RELEASE: run all tests
	ret = UnitTest::RunAllTests();
#endif

	return ret;
}

void UnitTestAppl::init( int argc, char * argv[] )
{
#ifdef _DEBUG  
	// DEBUG: optionally run specific tests only
	if( 1 < argc )
	{
		basar::Int32 counter = 1;
		while( counter < argc )
		{
			m_TestNames.push_back( argv[ counter++ ] );
		}
	}
#else
	argc = argc; argv = argv;
#endif	

	initRandom();
}

void UnitTestAppl::shutdown()
{
}

void UnitTestAppl::initRandom() const
{
	// seed random-number generator with current time so that numbers will be different every time we run
	std::srand( static_cast<unsigned int>( time( 0 ) ) );
}

} // end namespace unittest