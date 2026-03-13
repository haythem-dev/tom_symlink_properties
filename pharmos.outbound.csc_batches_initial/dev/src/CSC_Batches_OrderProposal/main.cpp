//----------------------------------------------------------------------------
/*! \file   
 *  \brief  
 *  \author Hendrik Polzin	
 *  \date   15.08.2013
 */
//----------------------------------------------------------------------------
#include "abbauwappl.h"
/*
#ifndef WIN32
#include "gitversioninfo.h"
#endif
*/
extern "C"
{
	extern const char versioninfo[];
	extern const char BASAR_versioninfo[];
}

//----------------------------------------------------------------------------
// start-up of ABBAUW-CSC_Batches_OrderProposal.exe
// 
// Arguments with example: -countrycode CH -db ich21 -server zdev21_tcp
//----------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
	basar::cmnutil::CmdLineTokenizer cmdline;
	cmdline.setCmdLine(argc, argv);
	if (cmdline.isParam("-version"))
	{
		std::cout << versioninfo << std::endl;
		std::cout << BASAR_versioninfo << std::endl;
        /*
#ifndef WIN32
		std::cout << GitVersionInfo::instance() << std::endl;
#endif
        */
		exit( 0 );
	}
	int retvalue = 0;		// return = 0: everything ok 
							// return = 1: some error(s) occurred, couldn't handle one or more flatfiles				
    try 
	{
        abbaUW::ABBAUWAppl::getInstance().init( argc, argv );	
		if ( !abbaUW::ABBAUWAppl::getInstance().run())
		{
			retvalue = 1;
		}

        abbaUW::ABBAUWAppl::getInstance().shutdown(); 
		abbaUW::ABBAUWAppl::clear();  	
    } 
	catch( ... )
	{
		retvalue = 1;
    }

    return retvalue;
}
