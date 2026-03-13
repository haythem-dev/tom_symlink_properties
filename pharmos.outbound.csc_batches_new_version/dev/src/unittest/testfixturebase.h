//---------------------------------------------------------------------------//
/*! \file testfixturebase.h
 *  \brief declaration of fixture base class
 *  \author Dirk Kapusta
 *  \date 24.06.2008
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_TESTFIXTUREBASE_H
#define GUARD_TESTFIXTUREBASE_H

namespace cscUnitTest
{
//---------------------------------------------------------------------------//
/*! \brief TestFixtureBase base class for test fixtures
		   \n throws no-throw  
 *  \author Dirk Kapusta
 *  \date 24.06.2008
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//
class TestFixtureBase
{
public:
	/*! \brief std constructor \n no-throw */
	TestFixtureBase();
	/*! \brief virtual destructor \n no-throw */
	virtual	~TestFixtureBase();

protected:
	void processException( 
		const basar::Exception & e, 
		basar::ConstString functionName
			) const;

private:
	/*! \brief copy constructor \n no-throw */
	TestFixtureBase ( const TestFixtureBase & r );
	/*! \brief assign operator \n no-throw */
	TestFixtureBase operator = ( const TestFixtureBase & r );
};

} // end namespace cscUnitTest

#endif	// GUARD_TESTFIXTUREBASE_H
