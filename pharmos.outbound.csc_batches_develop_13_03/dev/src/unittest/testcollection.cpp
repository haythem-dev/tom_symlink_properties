// includes
// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnmain.h" /Fp$(IntDir)/cmnmain.pch
#include "cmnut.h"

#include "testcollection.h"

namespace unittest
{  
TestCollection::~TestCollection()
{
}

void TestCollection::add( const bool runDefault, const basar::VarString & name, const basar::VarString & testName )
{
	InnerTestCollection::iterator iter = m_testCollection.find( name );
	if( iter != m_testCollection.end() )
	{
		iter->second.add( testName );
	}
	else
	{
		m_testCollection[ name ] = TestNameCollection( runDefault, name, testName );
	}
}

void TestCollection::add( const bool runDefault, const basar::VarString & name )
{
	m_testCollection[ name ] = TestNameCollection( runDefault, name );
}	

void TestCollection:: add( const bool runDefault, const basar::VarString & name, const std::vector<basar::VarString> & testNames )
{
	m_testCollection[ name ] = TestNameCollection( runDefault, name, testNames );
}

int TestCollection::runDefaultTests( UnitTest::TestReporterStdout & reporter )
{
	std::vector<basar::VarString> list;
	addDefaultTestsToVec( list );
	return runTestList( list, reporter );
}

int TestCollection::runOneTest( const basar::VarString & name, UnitTest::TestReporterStdout & reporter )
{
	std::vector<basar::VarString> list;
	addTestsToVec( name, list );
	return runTestList( list, reporter );
}

void TestCollection::addDefaultTestsToVec( std::vector<basar::VarString> & vec )
{
	InnerTestCollection::const_iterator iter = m_testCollection.begin();
	for( ; iter != m_testCollection.end(); ++iter )
	{
		if( iter->second.isRunDefault() )
		{
			std::vector<basar::VarString>::const_iterator veciter = iter->second.getTestNames().begin(); 
			for( ; veciter != iter->second.getTestNames().end(); ++veciter )
			{
				vec.push_back( *veciter );
			}
		}
	}
}

void TestCollection::addTestsToVec( const basar::VarString & name, std::vector<basar::VarString> & vec )
{
	InnerTestCollection::const_iterator iter = m_testCollection.find( name );
	if( iter != m_testCollection.end() )
	{
		std::vector<basar::VarString>::const_iterator veciter = iter->second.getTestNames().begin(); 
		for( ; veciter != iter->second.getTestNames().end(); ++veciter )
		{
			vec.push_back( *veciter );
		}
	}
	else
	{
		vec.push_back( name ); 
		std::cout << "Warning: unknown TestSuite: " << name << "!\n" << std::endl;
	}
}

int TestCollection::runTestList( const std::vector< basar::VarString > & testList, UnitTest::TestReporterStdout & reporter ) const
{
	int result = 0;
	std::vector<basar::VarString> failed;
	std::vector<basar::VarString>::const_iterator iter = testList.begin();
	for( ; iter != testList.end(); ++iter )
	{
		std::cout << "++++++++++++ Start TestSuite " << iter->c_str() << " ++++++++++++" << std::endl;
		UnitTest::TestRunner runner( reporter );
		int miniresult = runner.RunTestsIf( UnitTest::Test::GetTestList(), iter->c_str(), UnitTest::True(), 0 );
		if( 0 < miniresult )
		{
			failed.push_back( iter->c_str() );
		}
		result += miniresult;
		std::cout << "++++++++++++   End TestSuite " << iter->c_str() << " ++++++++++++" << std::endl;
		std::cout << std::endl;
	}
	if( 0 < failed.size() )
	{
		std::cout << "failed: "  << std::endl;
		for( iter = failed.begin(); iter != failed.end(); ++iter )
		{
			std::cout << " - " << iter->c_str() << std::endl;
		}
	} 
	return result;
}

} // end namespace unittest
