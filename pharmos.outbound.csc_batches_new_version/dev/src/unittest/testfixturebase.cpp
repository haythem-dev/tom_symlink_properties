//---------------------------------------------------------------------------//
/*! \file testfixturebase.cpp
 *  \brief definition of fixture base class
 *  \author Dirk Kapusta
 *  \date 24.06.2008
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnut.h" /Fp$(IntDir)/cmnut.pch
#include "cmnut.h"

#include "testfixturebase.h"

namespace cscUnitTest
{
TestFixtureBase::TestFixtureBase()
{
}

TestFixtureBase::~TestFixtureBase()
{
}

void TestFixtureBase::processException( 
	const basar::Exception & e, 
	basar::ConstString functionName
		) const
{
	std::cout << functionName << "(): Exception caught: ";
	std::cout << e.what() << std::endl;
	throw;
}

} // end namespace cscUnitTest
