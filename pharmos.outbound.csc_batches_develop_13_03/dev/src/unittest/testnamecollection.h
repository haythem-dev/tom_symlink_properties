#ifndef  GUARD_TEST_NAME_COLLECTION_H 
#define  GUARD_TEST_NAME_COLLECTION_H 

#include <vector>  
#include <libbasarcmnutil.h> 

// todo DK: move class to libutil

namespace unittest
{	 
class TestNameCollection
{
public:
	TestNameCollection();
	TestNameCollection( const bool runDefault, const basar::VarString & name );
	TestNameCollection( const basar::VarString & name, const basar::VarString & testName );
	TestNameCollection( const bool runDefault, const basar::VarString & name, const basar::VarString & testName );
	TestNameCollection( const bool runDefault, const basar::VarString & name, const std::vector<basar::VarString> & testNames );
	void add( const basar::VarString & testName );
	bool isRunDefault() const;
	const std::vector<basar::VarString>	& getTestNames() const;

private:
	basar::VarString				m_name;
	bool							m_runDefault;
	std::vector<basar::VarString>	m_testNames;
};

} // end namespace unittest

#endif // GUARD_TEST_NAME_COLLECTION_H
