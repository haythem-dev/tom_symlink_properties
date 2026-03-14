#ifndef  GUARD_TEST_COLLECTION_H 
#define  GUARD_TEST_COLLECTION_H 

#include <TestReporterStdout.h>  
#include <libbasarcmnutil.h> 
#include <map>
#include "testnamecollection.h"

// todo DK: move class to libutil

namespace unittest
{	 
	/*!
	* A Selection of a list of grouped  unitTest Suite Names. The groups can be called seperately, 
	* with runOneTest(groupname, reporter) or as one group, with runDefaultTests(reporter), then all
	* groups that have the m_runDefault Parameter as true, are run sequential.
	* The Testgroups can be added by the various add methods. You can add a testsuite 
	* more than once, it is usefull then to set the runDefault only once.
	*/
class TestCollection
{
public:
	virtual ~TestCollection();
	void add( const bool runDefault, const basar::VarString & name, const basar::VarString & testName );
	void add( const bool runDefault, const basar::VarString & name );
	void add( const bool runDefault, const basar::VarString & name, const std::vector<basar::VarString> & testNames );

	int runDefaultTests( UnitTest::TestReporterStdout & reporter ); 
	int runOneTest( const basar::VarString & name, UnitTest::TestReporterStdout & reporter ); 
	int runTestList( const std::vector<basar::VarString> & testList, UnitTest::TestReporterStdout & reporter ) const; 

private:
	void addDefaultTestsToVec( std::vector<basar::VarString> & vec );
	void addTestsToVec( const basar::VarString & name, std::vector<basar::VarString> & );

	typedef std::map<basar::VarString, TestNameCollection> InnerTestCollection;
	InnerTestCollection		m_testCollection;
}; 

} // end namespace unittest

#endif // GUARD_TEST_COLLECTION_H
