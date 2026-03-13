#ifndef  GUARD_UNIT_TEST_APPL_H 
#define  GUARD_UNIT_TEST_APPL_H 

#ifdef WIN32
	#pragma warning (push)
	#pragma warning(disable: 4127 4244 4311 4312 4800 4251)
	#include <QCoreApplication>
	#pragma warning (pop)
#else 
	#include  <QCoreApplication>
#endif 

#include "testcollection.h"

namespace unittest
{	 
class UnitTestAppl
{
public:
	UnitTestAppl( int argc, char * argv[], unittest::TestCollection & testCollection );
	int run();

private:
	UnitTestAppl( const UnitTestAppl & );
	UnitTestAppl & operator= ( const UnitTestAppl & );

	void init( int argc, char * argv[] );
	void shutdown();
	void initRandom() const;

	unittest::TestCollection &		m_TestCollection;
	QCoreApplication				m_Application;
	std::vector<basar::VarString>	m_TestNames;
};

} // end namespace unittest

#endif // GUARD_UNIT_TEST_APPL_H
