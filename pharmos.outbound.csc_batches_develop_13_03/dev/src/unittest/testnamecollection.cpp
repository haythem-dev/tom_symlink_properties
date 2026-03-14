#include "testnamecollection.h"

namespace unittest
{  
TestNameCollection::TestNameCollection()
:	m_name( "" ),
	m_runDefault( false )
{ 
}

TestNameCollection::TestNameCollection( const bool runDefault, const basar::VarString & name )
:	m_name( name ),
	m_runDefault( runDefault )
{
}

TestNameCollection::TestNameCollection( const basar::VarString & name, const basar::VarString & testName )
:	m_name( name ),
	m_runDefault( true )
{
	m_testNames.push_back( testName );
}

TestNameCollection::TestNameCollection( const bool runDefault, const basar::VarString & name, const basar::VarString & testName )
:	m_name( name ),
	m_runDefault( runDefault )
{
	m_testNames.push_back( testName );
}

TestNameCollection::TestNameCollection( const bool runDefault, const basar::VarString & name, const std::vector<basar::VarString> & testNames )
:	m_name( name ),
	m_runDefault( runDefault ),
	m_testNames( testNames )
{
}

void TestNameCollection::add( const basar::VarString & testName )
{
	m_testNames.push_back( testName );
}

bool TestNameCollection::isRunDefault() const
{
	return m_runDefault;
}

const std::vector<basar::VarString>	& TestNameCollection::getTestNames() const
{
	return m_testNames;
}

} // end namespace unittest
