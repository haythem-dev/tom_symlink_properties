#include "unittestappl.h"

// In DEBUG mode all the various tests can be executed seperately
// by using the testname as programparameter, eg.
// UnitTest.exe aspectmanagertools datetime
class CSCTestCollection : public unittest::TestCollection
{
public:
	CSCTestCollection()
	{ 
	    // selected tests, true means it's part of the default test:
		
		// Unit test for CSC_Batches_Orderpropsal.exe
		add( true, "orderproposalbatcho", "TestOPBatchOrderProposalDM" );
		add( true, "orderproposalbatchp", "TestOPBatchPuBatchOrderPosDM" );

		// Unit tests for CSC_Batches_History.exe
		add( true, "orderproposal", "TestOrderProposalDM" );
		add( true, "copybatchorders", "TestCopyBatchOrderDM" );
	}
} testCollection;

int main( int argc, char *argv[] )
{
	unittest::UnitTestAppl appl( argc, argv, testCollection );
	int ret = appl.run();

	std::cout << "Hit <Enter> to quit..." << std::endl;
	char ch;
	std::cin.read( &ch, 1 );
	return ret;
}
